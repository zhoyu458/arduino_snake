// ArrayOfLedArrays - see https://github.com/FastLED/FastLED/wiki/Multiple-Controller-Examples for more info on
// using multiple controllers.  In this example, we're going to set up three NEOPIXEL strips on three
// different pins, each strip getting its own CRGB array to be played with, only this time they're going
// to be all parts of an array of arrays.



#include"BitMapStorage.h"
void setup() {

  Serial.begin(9600);

  BitMapStorage s = BitMapStorage(2);

  bool b = s.getNumberStatus(7);


  Serial.println(b);    
  

}

void loop() {

}