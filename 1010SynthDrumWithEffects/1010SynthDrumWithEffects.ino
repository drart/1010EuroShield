#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>


// GUItool: begin automatically generated code
AudioSynthSimpleDrum     drum2;          //xy=63,308
AudioSynthSimpleDrum     drum1;          //xy=68,127
AudioEffectGranular      granular1;      //xy=214,345
AudioEffectReverb        reverb1;        //xy=220,168
AudioFilterStateVariable filter1;        //xy=220,216
AudioEffectWaveshaper    waveshape1;     //xy=225,383
AudioEffectBitcrusher    bitcrusher1;    //xy=226,427
AudioMixer4              mixer1;         //xy=437,144
AudioMixer4              mixer2;         //xy=480,314
AudioOutputI2S           i2s1;           //xy=589,139
AudioConnection          patchCord1(drum2, granular1);
AudioConnection          patchCord2(drum2, waveshape1);
AudioConnection          patchCord3(drum2, bitcrusher1);
AudioConnection          patchCord4(drum2, 0, mixer2, 0);
AudioConnection          patchCord5(drum1, reverb1);
AudioConnection          patchCord6(drum1, 0, filter1, 0);
AudioConnection          patchCord7(drum1, 0, mixer1, 0);
AudioConnection          patchCord8(granular1, 0, mixer2, 1);
AudioConnection          patchCord9(reverb1, 0, mixer1, 1);
AudioConnection          patchCord10(filter1, 0, mixer1, 2);
AudioConnection          patchCord11(filter1, 2, mixer1, 3);
AudioConnection          patchCord12(waveshape1, 0, mixer2, 2);
AudioConnection          patchCord13(bitcrusher1, 0, mixer2, 3);
AudioConnection          patchCord14(mixer1, 0, i2s1, 0);
AudioConnection          patchCord15(mixer2, 0, i2s1, 1);

// GUItool: end automatically generated code
AudioControlSGTL5000     sgtl5000_1;   


#include <MIDI.h>

// 1010MIDIMonWithThru - target: the 1010music Euroshield with the PJRC Teensy 3.2
//
// This sketch demonstrates MIDI setup with automatic thruing. The LEDs will blink
// each time a message is received.
//
// This code is in the public domain.

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

#define ledPinCount 4
int ledPins[ledPinCount] = { 3, 4, 5, 6 };
int ledPos = 0;
const int ledPin = 13; // teensy 3.6

void handleNoteOn(byte channel, byte pitch, byte velocity) {
  drum1.noteOn();
  MIDI.sendNoteOn(pitch, velocity, channel);
}


void setup() {
  AudioMemory(15);
  AudioNoInterrupts();

  drum1.frequency(40);
  drum1.length(500);
  drum1.secondMix(0.80);
  drum1.pitchMod(0.45);

  //drum1.frequency(400);
  //drum1.length(500);
  
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.9);

  AudioInterrupts();
  
  
  for (int i = 0; i < ledPinCount; i++)
    pinMode(ledPins[i], OUTPUT);

  pinMode(ledPin, OUTPUT);

  MIDI.begin();
  MIDI.setHandleNoteOn(handleNoteOn);

}

elapsedMillis kickit;

void loop() {

  if (kickit >= 1000)
  {
    kickit = 0;

    drum1.noteOn();
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }

}
