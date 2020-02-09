#include "XT_DAC_Audio.h"
#include <SD.h>
#include <SPI.h>

const long defaultWavLength = 50000;
unsigned char ANote[defaultWavLength];
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
  File file = SD.open("/piano-a.wav");

  Serial.println("---Stage 1---");
  
  Serial.println("---Stage 2---");
  InitToZero(ANote, defaultWavLength);
  
  Serial.println("---Stage 3---");
  long counter = 0;
  while(file.available() && (counter < defaultWavLength)){
    ANote[counter] = (unsigned char)file.read();
    counter++;
  }
  
  file.close();

  XT_Wav_Class PianoA(ANote);
  while(true){
    DacAudio1.FillBuffer();
    if(PianoA.Playing==false){
      DacAudio1.Play(&PianoA);
      Serial.println("*note*");
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
