#include "XT_DAC_Audio.h"
#include <SD.h>
#include <SPI.h>

const long defaultWavLength = 10000;
unsigned char ANote[defaultWavLength];
unsigned char BNote[defaultWavLength];
unsigned char CNote[defaultWavLength];
unsigned char DNote[defaultWavLength];
unsigned char ENote[defaultWavLength];
unsigned char FNote[defaultWavLength];
unsigned char GNote[defaultWavLength];
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
  long counter;
  File file;
  
  Serial.println("---Stage 1 A---");
  file = SD.open("/piano-a.wav");
  
  Serial.println("---Stage 2 A---");
  InitToZero(ANote, defaultWavLength);
  
  Serial.println("---Stage 3 A---");
  counter = 0;
  while(file.available() && (counter < defaultWavLength)){
    ANote[counter] = (unsigned char)file.read();
    counter++;
  }
  
  file.close();

  Serial.println("---Stage 1 B---");
  file = SD.open("/piano-b.wav");
  
  Serial.println("---Stage 2 B---");
  InitToZero(BNote, defaultWavLength);
  
  Serial.println("---Stage 3 B---");
  counter = 0;
  while(file.available() && (counter < defaultWavLength)){
    BNote[counter] = (unsigned char)file.read();
    counter++;
  }
  
  file.close();

  Serial.println("---Stage 1 C---");
  file = SD.open("/piano-c.wav");
  
  Serial.println("---Stage 2 C---");
  InitToZero(CNote, defaultWavLength);
  
  Serial.println("---Stage 3 C---");
  counter = 0;
  while(file.available() && (counter < defaultWavLength)){
    CNote[counter] = (unsigned char)file.read();
    counter++;
  }
  
  file.close();

  Serial.println("---Stage 1 D---");
  file = SD.open("/piano-d.wav");
  
  Serial.println("---Stage 2 D---");
  InitToZero(DNote, defaultWavLength);
  
  Serial.println("---Stage 3 D---");
  counter = 0;
  while(file.available() && (counter < defaultWavLength)){
    DNote[counter] = (unsigned char)file.read();
    counter++;
  }
  
  file.close();

  Serial.println("---Stage 1 E---");
  file = SD.open("/piano-e.wav");
  
  Serial.println("---Stage 2 E---");
  InitToZero(ENote, defaultWavLength);
  
  Serial.println("---Stage 3 E---");
  counter = 0;
  while(file.available() && (counter < defaultWavLength)){
    ENote[counter] = (unsigned char)file.read();
    counter++;
  }
  
  file.close();

  Serial.println("---Stage 1 F---");
  file = SD.open("/piano-f.wav");
  
  Serial.println("---Stage 2 F---");
  InitToZero(FNote, defaultWavLength);
  
  Serial.println("---Stage 3 F---");
  counter = 0;
  while(file.available() && (counter < defaultWavLength)){
    FNote[counter] = (unsigned char)file.read();
    counter++;
  }
  
  file.close();

  Serial.println("---Stage 1 G---");
  file = SD.open("/piano-g.wav");
  
  Serial.println("---Stage 2 G---");
  InitToZero(GNote, defaultWavLength);
  
  Serial.println("---Stage 3 G---");
  counter = 0;
  while(file.available() && (counter < defaultWavLength)){
    GNote[counter] = (unsigned char)file.read();
    counter++;
  }
  
  file.close();

  XT_Wav_Class PianoA(ANote);
  XT_Wav_Class PianoB(BNote);
  XT_Wav_Class PianoC(CNote);
  XT_Wav_Class PianoD(DNote);
  XT_Wav_Class PianoE(ENote);
  XT_Wav_Class PianoF(FNote);
  XT_Wav_Class PianoG(GNote);
  int noteSelector = 0;
  while(true){
    DacAudio1.FillBuffer();
    if(PianoG.Playing==false){
      DacAudio1.Play(&PianoG);
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
