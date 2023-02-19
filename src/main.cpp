
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

/*----------------------GAME VARIABLE-----------------------------*/
LinkedList<Node*>* snakeList = new LinkedList<Node*>();
Snake *snake = new Snake(snakeList);
Node *fruit = new Node(8, 0, 0, 1, 0);
BitMapStorage *bs = new BitMapStorage(NUM_LEDS);
CRGB leds[NUM_LEDS];
bool justAteFruit = false;

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

void restart()
{
  snake->list->clear();

  snake->addToTail(new Node(1, 0));
  fruit->refresh(snake->list);
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
  snake->addToTail(new Node(1, 0));
}

void loop()
{

  // Serial.println(freeMemory());

  PathSearch *ps = new PathSearch(
      fruit,
      snake->list,
      bs);

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
      delete ps;
      ps = NULL;

      snake->list->clear();
      delete snake->list;
      snake->list = new LinkedList<Node*>();
      
      gameOver();
      restart();
    }

    renderGame();

  }
  else // dfs found a route
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
        delay(speed);
      justAteFruit = false;

    }
  }

  if(ps!=NULL){
    delete ps;
    ps = NULL;

    }


}