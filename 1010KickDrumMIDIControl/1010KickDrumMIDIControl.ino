#include <Audio.h>
#include <Wire.h>
#include <SPI.h>

#include <MIDI.h>
#include <Bounce2.h>

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);
//MIDI_CREATE_DEFAULT_INSTANCE();

//////
// EUROSHIELD LEDS
/////
static const unsigned ledPin = 13;      // LED pin on Teensy
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


// GUItool: begin automatically generated code

AudioSynthSimpleDrum     drum1;          //xy=431,197
AudioMixer4              mixer1;         //xy=737,265
AudioOutputI2S           i2s1;           //xy=979,214
AudioConnection          patchCord3(drum1, 0, mixer1, 0);
AudioConnection          patchCord5(mixer1, 0, i2s1, 0);
AudioConnection          patchCord6(mixer1, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=930,518
// GUItool: end automatically generated code

void handleNoteOn(byte channel, byte pitch, byte velocity) {
  if (pitch == 36){
    drum1.noteOn();
  }
  if (pitch == 37){
  }
  //MIDI.sendNoteOn(pitch, velocity, channel);
}

void handleControlChange(byte channel, byte number, byte value){
}

void setup() {

  Button.attach(buttonInput, INPUT_PULLUP);
  Button.interval(25);

  // audio library init
  AudioMemory(15);
  AudioNoInterrupts();

  drum1.frequency(40);
  drum1.length(500);
  drum1.secondMix(0.80);
  drum1.pitchMod(0.45);
  
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.9);

  AudioInterrupts();

  MIDI.begin();
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleControlChange(handleControlChange);
}

void loop() {

  int upperPotValue = analogRead( upperPotPin );
  int lowerPotValue = analogRead( lowerPotPin );
  
  Button.update();
  if ( Button.fell() ) {
      drum1.noteOn();
  }

  // change audio shield parameters here
  AudioNoInterrupts();
  drum1.length(1 + upperPotValue);
  drum1.secondMix((float)lowerPotValue/1024);
  AudioInterrupts();
}
