#include <Arduino.h>
#include <LinkedList.h>
#include <FastLED.h>
#include "Constant.h"
#include "Node.h"
#include "Utils.h"
#include "Snake.h"

/*----------------------GAME VARIABLE-----------------------------*/
Snake *snake = new Snake();
Node *fruit = new Node(8, 0, 0, 1, 0);

CRGB leds[NUM_LEDS];

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
  int size = snake->list.size();
  for (int i = 0; i < size; i++)
  {
    Node *n = snake->list.get(i);
    int num = calcLedNumberFromNode(n);
    leds[num] = CRGB(n->red, n->green, n->blue);
  }

  int num = calcLedNumberFromNode(fruit);
  leds[num] = CRGB(fruit->red, fruit->green, fruit->blue);

  FastLED.show();
}


void restart(){
   snake->list.clear();
   snake->add(new Node(1, 0));
   fruit->refresh(&(snake->list));
}
void setup()
{
  Serial.begin(9600);
  Serial.println("Program Start");
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(MAX_VOLTS, MAX_AMPS);
  randomSeed(analogRead(A0));

  snake->add(new Node(1, 0));
}

bool justAteFruit = false;

void loop()
{
  

  char dir = UP;


  if (snake->list.get(0)->row < fruit->row && snake->direction != DOWN)
    dir = UP;
  else if (snake->list.get(0)->row > fruit->row && snake->direction != UP)
    dir = DOWN;
  else if (snake->list.get(0)->col < fruit->col && snake->direction != LEFT)
    dir = RIGHT;
  else
    dir = LEFT;


  snake->move(dir);

  int stu = snake->status(fruit);


  if (stu == HIT_WALL || stu == HIT_SELF)
  {
    gameOver();
    restart();
  }
  else if (stu == HIT_FRUIT)
  {
    

    snake->eatFruit(fruit);

    fruit->refresh(&(snake->list));

    justAteFruit = true;

  }



  renderGame();

  if(!justAteFruit)delay(speed);

  justAteFruit = false;
}