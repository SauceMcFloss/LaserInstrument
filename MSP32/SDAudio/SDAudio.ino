#include "XT_DAC_Audio.h"
#include <SD.h>
#include <SPI.h>

const long defaultWavLength = 17909;
unsigned char A4Note_Beg[17174];
unsigned char A4Note_Mid[17909];
unsigned char A4Note_End[16623];
//unsigned char BNote[defaultWavLength];
//unsigned char CNote[defaultWavLength];
//unsigned char DNote[defaultWavLength];
//unsigned char ENote[defaultWavLength];
//unsigned char FNote[defaultWavLength];
unsigned char GNote[defaultWavLength];
//XT_DAC_Audio_Class DacAudio2(26,0);
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
  file = SD.open("/flute_A4_1_mezzo-forte_normal_beg.wav");
  
  Serial.println("---Stage 2 A---");
  //SetFileSize(A4Note, defaultWavLength);
  
  Serial.println("---Stage 3 A---");
  counter = 0;
  while(file.available() && (counter < 17174)){
    A4Note_Beg[counter] = (unsigned char)file.read();
    counter++;
  }
  
  file.close();
  
  Serial.println("---Stage 1 A---");
  file = SD.open("/flute_A4_1_mezzo-forte_normal_mid.wav");
  
  Serial.println("---Stage 2 A---");
  //SetFileSize(A4Note, defaultWavLength);
  
  Serial.println("---Stage 3 A---");
  counter = 0;
  while(file.available() && (counter < 17909)){
    A4Note_Mid[counter] = (unsigned char)file.read();
    counter++;
  }
  
  file.close();

  Serial.println("---Stage 1 A---");
  file = SD.open("/flute_A4_1_mezzo-forte_normal_end.wav");
  
  Serial.println("---Stage 2 A---");
  //SetFileSize(A4Note, defaultWavLength);
  
  Serial.println("---Stage 3 A---");
  counter = 0;
  while(file.available() && (counter < 16623)){
    A4Note_End[counter] = (unsigned char)file.read();
    counter++;
  }
  
  file.close();

  XT_Wav_Class PianoA4_Beg(A4Note_Beg);
  PianoA4_Beg.Speed = .82;
  XT_Wav_Class PianoA4_Mid(A4Note_Mid);
  PianoA4_Mid.Speed = .82;
  XT_Wav_Class PianoA4_End(A4Note_End);
  PianoA4_End.Speed = .82;

  Serial.println(PianoA4_Beg.DataSize);
  Serial.println(PianoA4_Mid.DataSize);
  Serial.println(PianoA4_End.DataSize);

  if(PianoA4_Beg.Playing==false){
    DacAudio1.Play(&PianoA4_Beg);
    PianoA4_Beg.Looped=false;
    Serial.println("*beg*");
  }
  while(PianoA4_Beg.Looped == false){
    DacAudio1.FillBuffer();
  }

  for(int i = 0; i < 3; i++){
    if(PianoA4_Mid.Playing==false){
      DacAudio1.Play(&PianoA4_Mid);
      PianoA4_Mid.Looped=false;
      Serial.println("*mid*");
    }
    while(PianoA4_Mid.Looped == false){
      DacAudio1.FillBuffer();
    }
  }

  while(true){
    if(PianoA4_End.Playing==false){
      DacAudio1.Play(&PianoA4_End);
      PianoA4_End.Looped=false;
      Serial.println("*end*");
    }
    while(PianoA4_End.Looped == false){
      DacAudio1.FillBuffer();
    }
  }
  
  DacAudio1.StopAllSounds();
  Serial.println("Done.");
  //}
}

void loop(){
  
}
