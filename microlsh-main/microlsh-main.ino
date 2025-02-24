/* Author: Abhipol Vibhatasilpin
 * Description: Realtime Audio LSH (Locality Sensitive Hasing) Prototype    
 *
 * Modified from SPH0645 MEMS Microphone Test (Adafruit product #3421) Example
 *
 *  Forum thread with connection details and other info:
 * https://forum.pjrc.com/threads/60599?p=238070&viewfull=1#post238070
 *
 */


#include <Audio.h>
#include <arm_math.h>
#include "SPI.h"

// GUItool: begin automatically generated code
AudioInputI2S            i2s1;           //xy=180,111
AudioFilterStateVariable filter1;        //xy=325,101
AudioAmplifier           amp1;           //xy=470,93
AudioAnalyzeFFT1024      fft1024_1;      //xy=616,102
AudioConnection          patchCord1(i2s1, 0, filter1, 0);
AudioConnection          patchCord2(filter1, 2, amp1, 0);
AudioConnection          patchCord3(amp1, fft1024_1);
// GUItool: end automatically generated code

#define PREVIEW_SIZE 512
#define LSH_DIAGNOSTICS 1
#define SPECWIDTH 512 // FFT1024 produces 512 bins 
#define SPECHEIGHT 128
 
uint16_t *spec;
int specSliceIdx = 0;
void printSpec(uint16_t *specHandle);
void printSpecSlice(uint16_t *specHandle, int slice);

void setup() {
  /* Teensy audio components initialization */
  AudioMemory(50);
  filter1.frequency(30); // filter out DC & extremely low frequencies
  amp1.gain(8.5);        // amplify sign to useful range
  
  /* Allocate memory for the spectrogram */
  spec = (uint16_t*)calloc(SPECHEIGHT, SPECWIDTH * sizeof(uint16_t));
  if(!spec) {
    Serial.println("ERROR: spectrogram memory allocation failed");
  }
  else {
    Serial.println("SUCCESS: spectrogram memory allocated!");
  }
}

#if LSH_DIAGNOSTICS
bool printed = false;  
#endif

void loop() {
  if (fft1024_1.available()) {
    if(specSliceIdx >= SPECHEIGHT) {specSliceIdx = 0;}
    const int specOffset = specSliceIdx * SPECWIDTH;
    for (int i = 0; i < SPECWIDTH; i++) {
      memcpy(&spec[specOffset], fft1024_1.output, sizeof(fft1024_1.output));
    }
    #if LSH_DIAGNOSTICS
    printSpecSlice(spec, specSliceIdx);
    if(!printed && specSliceIdx == (SPECHEIGHT - 1)) {
      printSpec(spec);
      printed = true;
    }
    #endif

    ++specSliceIdx;
  }
}

void printSpecSlice(uint16_t *specHandle, int specSliceIdx) {
  if(specSliceIdx >= SPECHEIGHT || specSliceIdx < 0) {
    Serial.println("ERROR: spectogram slide index outside of valid range! printing nothing...");
    return;
  }

  const int specOffset = specSliceIdx * SPECWIDTH;
  Serial.printf("Spec @ row %d: ", specSliceIdx);
  
  for (int i = 0; i < PREVIEW_SIZE; i++) {
    Serial.print(specHandle[specOffset + i]);
    Serial.print(" ");
  }
  Serial.println();
}

void printSpec(uint16_t *specHandle) {
  if(!specHandle) {
    Serial.println("ERROR: Invalid pointer to the spectrogram! printing nothing...");
    return;
  }

  Serial.println("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  SPECTROGRAM BEGIN  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"); 
  for(int i = 0; i < SPECHEIGHT; i++) {
    printSpecSlice(spec, i);
  }
  Serial.println("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@   SPECTROGRAM END   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");  
}