
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
void gameRestart();

/*----------------------GAME VARIABLE-----------------------------*/
Snake *snake = new Snake();
Node *fruit = new Node(ROWS - 1, COLS - 1, 200, 0, 200);
CRGB leds[NUM_LEDS];
bool justAteFruit = false;

String snakePath = "";
bool interruptPath = false;

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
  runGame2();
}

//--------------------------------FUNCTION DECLARATION----------------------------------------------//

void runGame2()
{
  // Serial.println("--------------------------------");
  // Serial.println(snake->list->size());
  // Serial.println(freeMemory());

  snakePath = snake->planPath(fruit, &interruptPath);

  if (snakePath.length() == 0)
  {
    // no way to go, game over

    gameOver();
    gameRestart();
  }

  for (int i = 0; i < snakePath.length(); i++)
  {
    // interrupt occurs while snake is wandering, no route to the fruit or cannot see its tail
    snake->move(snakePath.charAt(i));
    int stu = snake->status(fruit);
    if (stu == HIT_WALL or stu == HIT_SELF)
    {

      gameOver();
      gameRestart();
    }
    else if (stu == HIT_FRUIT)
    {
      snake->eatFruit(fruit);
      fruit->refresh(snake->list);
      justAteFruit = true;
    }

    if (!justAteFruit)
    {
      delay(gameSpeed);
    }
    justAteFruit = false;

    renderGame();

    if (interruptPath == true)
    {
      break;
    }
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

void runGame()
{
  snakePath = snake->getHeadToFruitPath(fruit);

  if (snakePath.length() != 0) // dfs found a route
  {

    for (int i = 0; i < snakePath.length(); i++)
    {

      char dir = snakePath.charAt(i);
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

void gameRestart()
{
  snake->clear();
  snake->addToTail(new Node(SNAKE_INIT_ROW, SNAKE_INIT_COL));
  fruit->refresh(snake->list);
}