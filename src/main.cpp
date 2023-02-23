
#include <MemoryFree.h>
#include <Arduino.h>
#include <LinkedList.h>
#include <FastLED.h>
#include "Constant.h"
#include "Node.h"
#include "Utils.h"
#include "Snake.h"
#include "PathSearch.h"
#include "BitMapStorage.h"
#include "FruitList.h"

/*----------------------GAME VARIABLE-----------------------------*/
Snake *snake = new Snake();
Node *fruit = new Node(ROWS - 1, COLS - 1, 0, 1, 0);
CRGB leds[NUM_LEDS];
bool justAteFruit = false;
FruitList *fruitList = new FruitList();

void gameRun();
void gameOver()
{
  FastLED.clear();
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB(0, 1, 0);
    FastLED.show();
  }

  for (int i = NUM_LEDS - 1; i >= 0; i--)
  {
    leds[i] = CRGB(0, 0, 0);
    FastLED.show();
  }
  FastLED.clear();
}

void renderSingleNode(Node *n)
{

  FastLED.clear();

  int num = calcLedNumberFromNode(n);

  leds[num] = CRGB(n->red, n->green, n->blue);

  FastLED.show();
}

void renderGame()
{
  FastLED.clear();
  int size = snake->list->size();
  for (int i = 0; i < size; i++)
  {
    Node *n = snake->list->get(i);
    int num = calcLedNumberFromNode(n);
    leds[num] = CRGB(n->red, n->green, n->blue);
  }

  int num = calcLedNumberFromNode(fruit);
  leds[num] = CRGB(fruit->red, fruit->green, fruit->blue);

  FastLED.show();
}

void pause()
{
  while (1)
  {
  };
}

void setup()
{
  Serial.begin(9600);
  Serial.println("Program Start");
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  // FastLED.setMaxPowerInVoltsAndMilliamps(MAX_VOLTS, MAX_AMPS);
  randomSeed(analogRead(A0));
  snake->addToTail(new Node(SNAKE_INIT_ROW, SNAKE_INIT_COL));

  // Node* n = new Node(5,12);
  // int num = calcLedNumberFromNode(n);
  // FastLED.clear();
  // leds[num] = CRGB(0,1,1);
  // FastLED.show();

  // pause();
}

PathSearch *ps = new PathSearch(
    fruit,
    snake->list);

void loop()
{

  // Serial.println(freeMemory());

  char dir = UP;

  String fruitToSnakeHeadPath = ps->getPath();

  // length is 0 means dfs does not find a route to get the fruit
  if (fruitToSnakeHeadPath.length() == 0)
  {
    snake->guideMoveWithNoPathFound(fruit);
    int stu = snake->status(fruit);
    // no path was found, snake will never eat a fruit
    if (stu == HIT_WALL || stu == HIT_SELF)
    {
      ps->clear();
      snake->clear();
      snake->addToTail(new Node(1, 0));
      fruit->refresh(snake->list);
      pause();
      gameOver();
    }

    renderGame();
  }
  else // dfs found a route
  {
    //-----------------------------------------------------------------------------//
    // deep copy the snake,
    // use the copy to run the path and check if it is running to dead end after eating the fruit

    Snake *snakeCopy = new Snake();
    for (int i = 0; i < snake->list->size(); i++)
    {
      Node *snakeSection = snake->list->get(i);
      Node *copy = new Node();
      copy->deepCopyPosition(snakeSection);
      snakeCopy->addToTail(copy);
    }

    // the snakeCopy is running the path
    for (int i = fruitToSnakeHeadPath.length() - 1; i >= 0; i--)
    {
      dir = fruitToSnakeHeadPath.charAt(i);
      if (dir == UP)
        dir = DOWN;
      else if (dir == DOWN)
        dir = UP;
      else if (dir == LEFT)
        dir = RIGHT;
      else if (dir == RIGHT)
        dir = LEFT;
      snakeCopy->move(dir);
    }

    bool crossHitUp = snakeCopy->upCrossWillHitSelf();
    bool crossHitDown = snakeCopy->downCrossWillHitSelf();
    bool crossHitLeft = snakeCopy->leftCrossWillHitSelf();
    bool crossHitRight = snakeCopy->rightCrossWillHitSelf();

    // clear the snakeCopy, free all memory
    snakeCopy->clear();
    delete snakeCopy;
    snakeCopy = NULL;

    Serial.print("hitup:");
    Serial.print(crossHitUp);
    Serial.print("  hitdown:");
    Serial.print(crossHitDown);
    Serial.print("  hitleft:");
    Serial.print(crossHitLeft);
    Serial.print("  hitright:");
    Serial.print(crossHitRight);
    Serial.println("--------------------------");

    if (crossHitUp and crossHitDown and crossHitLeft and crossHitRight)
    {
      // dead end is found, snake should not eat the fruit direct, detour route is needed
      int upperCount = snake->getBodyCountOnUpperHalf();
      int lowerCount = snake->getBodyCountOnLowerHalf();
      int leftCount = snake->getBodyCountOnLeftHalf();
      int rightCount = snake->getBodyCountOnRightHalf();

      Serial.println("an dead end will be met");
      Serial.print("upperCount:");
      Serial.println(upperCount);

      Serial.print("lowerCount:");
      Serial.println(lowerCount);

      Serial.print("leftCount:");
      Serial.println(leftCount);

      Serial.print("rightCount:");
      Serial.println(rightCount);
    }

    // ----------------------------------------------------------------------//

    for (int i = fruitToSnakeHeadPath.length() - 1; i >= 0; i--)
    {
      dir = fruitToSnakeHeadPath.charAt(i);
      if (dir == UP)
        dir = DOWN;
      else if (dir == DOWN)
        dir = UP;
      else if (dir == LEFT)
        dir = RIGHT;
      else if (dir == RIGHT)
        dir = LEFT;

      snake->move(dir);
      snake->print();
      Serial.println("--------------------------------------------");

      int stu = snake->status(fruit);
      // path was found, snake will eat a fruit and will not hit self or wall

      if (stu == HIT_FRUIT)
      {

        snake->eatFruit(fruit);

        fruit->refresh(snake->list);

        justAteFruit = true;
      }

      renderGame();

      if (!justAteFruit)
      {
        delay(gameSpeed);
      }
      justAteFruit = false;
    }
  }
  ps->clear();
}
