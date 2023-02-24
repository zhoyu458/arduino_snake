
#include <MemoryFree.h>
#include <Arduino.h>
#include <LinkedList.h>
#include <FastLED.h>
#include "Constant.h"
#include "Node.h"
#include "Utils.h"
#include "Snake.h"
#include "BitMapStorage.h"
#include "FruitList.h"

/*----------------------GAME FUNCTIONS-----------------------------*/
void runGame2();
void runGame();
void renderSingleNode(Node *n);
void renderGame();
void pause();
void gameOver();
/*----------------------GAME VARIABLE-----------------------------*/
Snake *snake = new Snake();
Node *fruit = new Node(ROWS - 1, COLS - 1, 0, 1, 0);
CRGB leds[NUM_LEDS];
bool justAteFruit = false;
FruitList *fruitList = new FruitList();

void setup()
{
  Serial.begin(9600);
  Serial.println("Program Start");
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  // FastLED.setMaxPowerInVoltsAndMilliamps(MAX_VOLTS, MAX_AMPS);
  randomSeed(analogRead(A0) * analogRead(A1));
  snake->addToTail(new Node(SNAKE_INIT_ROW, SNAKE_INIT_COL));
}

void loop()
{

  Serial.println(freeMemory());
  runGame();
}

//--------------------------------FUNCTION DECLARATION----------------------------------------------//
void runGame()
{
  char dir = UP;
  String fruitToSnakeHeadPath = snake->getFruitToHeadPath(fruit);
  if (fruitToSnakeHeadPath.length() != 0) // dfs found a route
  {

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
  else
  {
    snake->guideMoveWithNoPathFound(fruit);
    int stu = snake->status(fruit);
    // no path was found, snake will never eat a fruit
    if (stu == HIT_WALL || stu == HIT_SELF)
    {
      // ps->clear();
      snake->clear();
      snake->addToTail(new Node(1, 0));
      fruit->refresh(snake->list);
      gameOver();
    }

    renderGame();
  }
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
