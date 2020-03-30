#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code

AudioSynthSimpleDrum     drum1;          //xy=431,197
AudioMixer4              mixer1;         //xy=737,265
AudioOutputI2S           i2s1;           //xy=979,214
AudioConnection          patchCord3(drum1, 0, mixer1, 0);
AudioConnection          patchCord5(mixer1, 0, i2s1, 0);
AudioConnection          patchCord6(mixer1, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=930,518
// GUItool: end automatically generated code

static uint32_t next;

void setup() {

  // audio library init
  AudioMemory(15);

  next = millis() + 1000;

  AudioNoInterrupts();

  drum1.frequency(40);
  drum1.length(500);
  drum1.secondMix(0.80);
  drum1.pitchMod(0.45);
  
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.9);

  AudioInterrupts();

}

void loop() {
  
  if(millis() == next)
  {
    next = millis() + 500;

    drum1.noteOn();
  }
}
