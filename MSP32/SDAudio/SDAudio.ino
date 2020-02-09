#include "XT_DAC_Audio.h"
#include <SD.h>
#include <SPI.h>

const long defaultWavLength = 50000;
unsigned char Scream[defaultWavLength];
XT_DAC_Audio_Class DacAudio2(26,0);
XT_DAC_Audio_Class DacAudio1(25,0);

void InitToZero(unsigned char * list, long maximum){
  long counter = 0;
  while(counter < maximum){
    list[counter] = 0x00;
    counter++;
  }
}

void setup() {
  // debug output at 115200 baud
  Serial.begin(115200);

  // set up sd
  if(!SD.begin())
  {
     Serial.println("Card Mount Failed");
     return;
  }
  File file = SD.open("/scream.wav");

  Serial.println("---Stage 1---");
  
  Serial.println("---Stage 2---");
  InitToZero(Scream, defaultWavLength);
  
  Serial.println("---Stage 3---");
  long counter = 0;
  while(file.available() && (counter < defaultWavLength)){
    Scream[counter] = (unsigned char)file.read();
    counter++;
  }
  
  file.close();

  XT_Wav_Class R2D2(Scream);
  while(true){
    DacAudio1.FillBuffer();
    if(R2D2.Playing==false){
      DacAudio1.Play(&R2D2);
      Serial.println("*SCREAMING*");
    }
  }
}

void loop(){
  /*DacAudio1.FillBuffer();
  if(R2D2.Playing==false){
    DacAudio1.Play(&R2D2);
    Serial.println("PLAYING");
  }*/
}
