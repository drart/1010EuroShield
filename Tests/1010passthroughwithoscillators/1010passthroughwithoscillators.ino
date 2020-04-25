#include <Bounce2.h>
#include <MIDI.h>


#include <Audio.h>
#include <Wire.h>
#include <SPI.h>

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S            i2s1;           //xy=79,234
AudioSynthWaveform       waveform1;      //xy=81,320
AudioSynthWaveformSine   sine1;          //xy=109,671
AudioEffectGranular      granular1;      //xy=119,549
AudioEffectDigitalCombine combine1;       //xy=125,507
AudioSynthWaveformModulated waveformMod1;   //xy=137,610
AudioEffectEnvelope      envelope1;      //xy=207,328
AudioAnalyzeRMS          rms1;           //xy=234,167
AudioAnalyzePeak         peak1;          //xy=235,124
AudioSynthWaveformPWM    pwm1;           //xy=241,671
AudioFilterStateVariable filter1;        //xy=339,332
AudioEffectBitcrusher    bitcrusher1;    //xy=527,348
AudioMixer4              mixer2;         //xy=708,330
AudioMixer4              mixer1;         //xy=709,246
AudioOutputI2S           i2s2;           //xy=745,471
AudioConnection          patchCord1(i2s1, 0, mixer1, 0);
AudioConnection          patchCord2(i2s1, 0, peak1, 0);
AudioConnection          patchCord3(i2s1, 0, rms1, 0);
AudioConnection          patchCord4(i2s1, 1, mixer2, 0);
AudioConnection          patchCord5(waveform1, envelope1);
AudioConnection          patchCord6(sine1, pwm1);
AudioConnection          patchCord7(envelope1, 0, filter1, 0);
AudioConnection          patchCord8(filter1, 0, mixer1, 1);
AudioConnection          patchCord9(filter1, 0, bitcrusher1, 0);
AudioConnection          patchCord10(bitcrusher1, 0, mixer2, 1);
AudioConnection          patchCord11(mixer2, 0, i2s2, 1);
AudioConnection          patchCord12(mixer1, 0, i2s2, 0);
// GUItool: end automatically generated code

AudioControlSGTL5000     sgtl5000_1;   





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

const int myInput = AUDIO_INPUT_LINEIN;

void setup()
{
  pinMode(2, INPUT);
  Button.attach(2, INPUT);
  Button.interval(25);

  bitcrusher1.bits(10);
  bitcrusher1.sampleRate(29000);

  /*
  envelope1.attack();
  envelope1.hold();
  envelope1.decay();
  envelope1.release();
  */
  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(12);

  // Enable the audio shield, select input, and enable output
  sgtl5000_1.enable();
  sgtl5000_1.inputSelect(myInput);
  sgtl5000_1.adcHighPassFilterDisable();
  sgtl5000_1.lineInLevel(0,0);
  sgtl5000_1.unmuteHeadphone();
    sgtl5000_1.volume(0.9);


  AudioInterrupts();
  MIDI.begin();
  MIDI.setHandleNoteOn(midiNotes);

  waveform1.begin(0.7, 100, WAVEFORM_SQUARE);

}


void advanceLED()
{
  digitalWrite(ledPins[ledPos % ledPinCount], LOW);
  ledPos++;
  digitalWrite(ledPins[ledPos % ledPinCount], HIGH);
}

void midiNotes(byte channel, byte note, byte velocity){
  /// https://forum.pjrc.com/threads/41862-MIDI-notes-to-trigger-Audio-Board-synth
  // freq = 440 * (2 ^ ((note - 69) / 12));
  float freq = 440.0 * powf(2.0, (float)(note - 69) * 0.08333333);  
  waveform1.frequency(freq);
}


void loop()
{
  int upperPotValue = analogRead( upperPotPin );
  int lowerPotValue = analogRead( lowerPotPin );

  Button.update();
  if ( Button.fell() ) {
    // button was pressed
    envelope1.noteOn();
    advanceLED();
    MIDI.sendNoteOn(36, 127, 10);
  }
  if ( Button.rose() ) {
    envelope1.noteOff();
    MIDI.sendNoteOff(36, 0, 10);
  }

  if (previousUpperPotValue != upperPotValue) {
    // upper pot has moved
    MIDI.sendControlChange(20, upperPotValue / 8, 1);
    waveform1.frequency(upperPotValue + 20);
    filter1.frequency(upperPotValue * 1.7 + 1);
  }
  if (previousLowerPotValue != lowerPotValue) {
    // lower pot has moved
    MIDI.sendControlChange(21, lowerPotValue / 8, 1);
    filter1.resonance((float)lowerPotValue/500.0);
  }

  if (MIDI.read())                    // If we have received a message
  {
    advanceLED();
  }

  previousUpperPotValue = upperPotValue;
  previousLowerPotValue = lowerPotValue;
  
}
