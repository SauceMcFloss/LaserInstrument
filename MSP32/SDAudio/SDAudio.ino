#include "XT_DAC_Audio.h"
#include <SD.h>
#include <SPI.h>

const long defaultWavLength = 54498;
unsigned char A4Note[defaultWavLength];
//unsigned char BNote[defaultWavLength];
//unsigned char CNote[defaultWavLength];
//unsigned char DNote[defaultWavLength];
//unsigned char ENote[defaultWavLength];
//unsigned char FNote[defaultWavLength];
unsigned char GNote[defaultWavLength];
XT_DAC_Audio_Class DacAudio2(26,0);
XT_DAC_Audio_Class DacAudio1(25,0);

void SetFileSize(unsigned char * list, long maximum){
  list[28] = 226;
  list[29] = 212;
  list[30] = 0x00;
  list[31] = 0x00;
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
  file = SD.open("/flute_A4_1_mezzo-forte_normal_mid.wav");
  
  Serial.println("---Stage 2 A---");
  //SetFileSize(A4Note, defaultWavLength);
  
  Serial.println("---Stage 3 A---");
  counter = 0;
  while(file.available() && (counter < defaultWavLength)){
    A4Note[counter] = (unsigned char)file.read();
    counter++;
  }
  
  file.close();

  XT_Wav_Class PianoA4(A4Note);
  
  while(true){
    DacAudio1.FillBuffer();
    if(PianoA4.Playing==false){
      DacAudio1.Play(&PianoA4);
      Serial.println("*note*");
    }
  }
}

void loop(){
  
}
