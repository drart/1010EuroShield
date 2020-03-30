#include <MIDI.h>
#include <Bounce2.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);
//MIDI_CREATE_DEFAULT_INSTANCE();

static const unsigned ledPin = 13;      // LED pin on Teensy


//////
// EUROSHIELD LEDS
/////

const int ledPinCount = 4;
int ledPins[ledPinCount] = { 3, 4, 5, 6 };
int ledPos = 0;

//////
// EUROSHIELD BUTTON
/////
#define buttonInput 2
Bounce Button = Bounce();

/////
// EUROSHIELD POTS
/////
const int upperPotPin = 20;
const int lowerPotPin = 21;
int previousUpperPotValue = 0;
int previousLowerPotValue = 0;

void setup()
{
  pinMode(ledPin, OUTPUT);
  for (int i = 0; i < ledPinCount; i++)
    pinMode(ledPins[i], OUTPUT);
    
  //pinMode(buttonInput, INPUT_PULLUP);
  Button.attach(buttonInput, INPUT_PULLUP);
  Button.interval(25);


  MIDI.begin();
}


void advanceLED()
{
  digitalWrite(ledPins[ledPos % ledPinCount], LOW);
  ledPos++;
  digitalWrite(ledPins[ledPos % ledPinCount], HIGH);
}

void loop()
{
  int upperPotValue = analogRead( upperPotPin );
  int lowerPotValue = analogRead( lowerPotPin );
  
  Button.update();
  if ( Button.rose() ) {
      // button was pressed
      
      advanceLED();
      MIDI.sendNoteOn(36,127,10);
  }
  if ( Button.fell() ){
    MIDI.sendNoteOff(36, 0, 10);
  }

  if (previousUpperPotValue != upperPotValue){
    // upper pot has moved
    MIDI.sendControlChange(20, upperPotValue/8, 1);
  }
  if (previousLowerPotValue != lowerPotValue){
    // lower pot has moved
    MIDI.sendControlChange(21, lowerPotValue/8, 1);
  }
  
  if (MIDI.read())                    // If we have received a message
  {
    advanceLED();
  }
  
  previousUpperPotValue = upperPotValue;
  previousLowerPotValue = lowerPotValue;
}
