#include <MIDI.h>

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

static const unsigned ledPin = 13;      // LED pin on Teensy

//////
// EUROSHIELD LEDS
/////
const int ledPinCount = 4;
int ledPins[ledPinCount] = { 3, 4, 5, 6 };
int ledPos = 0;

/////
// EUROSHIELD POTS
/////
const int upperPotPin = 20;
const int lowerPotPin = 21;

void setup()
{
  pinMode(ledPin, OUTPUT);
  for (int i = 0; i < ledPinCount; i++)
    pinMode(ledPins[i], OUTPUT);
    
  MIDI.begin();
}

void advanceLED()
{

  digitalWrite(ledPins[ledPos % 2], LOW);
  ledPos++;
  digitalWrite(ledPins[ledPos % 2], HIGH);

}

void loop()
{

  int upperPotValue = analogRead( upperPotPin );
  int lowerPotValue = analogRead( lowerPotPin );
  
  MIDI.sendNoteOn(36, (int)upperPotValue/8, 10);    // Cell 1 bitbox
  digitalWrite(ledPin, HIGH);
  advanceLED();

  delay(1000);
  MIDI.sendNoteOn(37, (int)lowerPotValue/8, 10);    // Cell 2 bitbox
  digitalWrite(ledPin, LOW);
  advanceLED();
  delay(1000);
  
}
