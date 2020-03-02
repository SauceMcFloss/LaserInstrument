// Libraries
#include "XT_DAC_Audio.h"
#include <SD.h>
#include <SPI.h>
#include <stdlib.h>

// Set up DAC pin
//XT_DAC_Audio_Class DacAudio2(26,0);
XT_DAC_Audio_Class DacAudio1(25,0);

// Default setup() function
void setup() {
  // Debug output at 115200 baud
  
  Serial.begin(115200);

  // Initialize and allocate each note's arrays
  
  Serial.println("Allocating memory...");
  unsigned char* C4Note_Mid = (unsigned char*)malloc(35244 * sizeof(unsigned char));
  unsigned char* D4Note_Mid = (unsigned char*)malloc(35722 * sizeof(unsigned char));
  unsigned char* E4Note_Mid = (unsigned char*)malloc(16984 * sizeof(unsigned char));
  unsigned char* F4Note_Mid = (unsigned char*)malloc(11074 * sizeof(unsigned char));
  unsigned char* G4Note_Mid = (unsigned char*)malloc(32305 * sizeof(unsigned char));
  unsigned char* A4Note_Mid = (unsigned char*)malloc(17909 * sizeof(unsigned char));
  unsigned char* B4Note_Mid = (unsigned char*)malloc(27573 * sizeof(unsigned char));
  unsigned char* C5Note_Mid = (unsigned char*)malloc(26086 * sizeof(unsigned char));
  Serial.println("Allocated.");

  // Set up sd

  Serial.println("Mounting card...");
  if(!SD.begin())
  {
     Serial.println("Card mount failed.");
     return;
  }
  Serial.println("Card mount succeeded.");
  long counter;
  File file;

  // Prepare audio

  Serial.println("Reading audio files for playback...");

  // Prepare C4

  file = SD.open("/flute_C4_1_mezzo-forte_normal_mid.wav");
  counter = 0;
  while(file.available() && (counter < 17909)){
    C4Note_Mid[counter] = (unsigned char)file.read();
    counter++;
  }
  file.close();

  // Prepare D4

  file = SD.open("/flute_D4_1_mezzo-forte_normal_mid.wav");
  counter = 0;
  while(file.available() && (counter < 17909)){
    D4Note_Mid[counter] = (unsigned char)file.read();
    counter++;
  }
  file.close();

  // Prepare E4

  file = SD.open("/flute_E4_1_mezzo-forte_normal_mid.wav");
  counter = 0;
  while(file.available() && (counter < 17909)){
    E4Note_Mid[counter] = (unsigned char)file.read();
    counter++;
  }
  file.close();

  // Prepare F4

  file = SD.open("/flute_F4_1_mezzo-forte_normal_mid.wav");
  counter = 0;
  while(file.available() && (counter < 17909)){
    F4Note_Mid[counter] = (unsigned char)file.read();
    counter++;
  }
  file.close();

  // Prepare G4

  file = SD.open("/flute_G4_1_mezzo-forte_normal_mid.wav");
  counter = 0;
  while(file.available() && (counter < 17909)){
    G4Note_Mid[counter] = (unsigned char)file.read();
    counter++;
  }
  file.close();

  // Prepare A4

  file = SD.open("/flute_A4_1_mezzo-forte_normal_mid.wav");
  counter = 0;
  while(file.available() && (counter < 17909)){
    A4Note_Mid[counter] = (unsigned char)file.read();
    counter++;
  }
  file.close();

  // Prepare B4

  file = SD.open("/flute_B4_1_mezzo-forte_normal_mid.wav");
  counter = 0;
  while(file.available() && (counter < 17909)){
    B4Note_Mid[counter] = (unsigned char)file.read();
    counter++;
  }
  file.close();

  // Prepare C5

  file = SD.open("/flute_C5_1_mezzo-forte_normal_mid.wav");
  counter = 0;
  while(file.available() && (counter < 17909)){
    C5Note_Mid[counter] = (unsigned char)file.read();
    counter++;
  }
  file.close();

  Serial.println("Files read.");

  // Create XT_Wav_Class objects to play

  Serial.println("Creating XT_Wav_Class objects for playback...");
  
  XT_Wav_Class PianoC4_Mid(C4Note_Mid);
  PianoC4_Mid.Speed = .82;
  XT_Wav_Class PianoD4_Mid(D4Note_Mid);
  PianoD4_Mid.Speed = .82;
  XT_Wav_Class PianoE4_Mid(E4Note_Mid);
  PianoE4_Mid.Speed = .82;
  XT_Wav_Class PianoF4_Mid(F4Note_Mid);
  PianoF4_Mid.Speed = .82;
  XT_Wav_Class PianoG4_Mid(G4Note_Mid);
  PianoG4_Mid.Speed = .82;
  XT_Wav_Class PianoA4_Mid(A4Note_Mid);
  PianoA4_Mid.Speed = .82;
  XT_Wav_Class PianoB4_Mid(B4Note_Mid);
  PianoB4_Mid.Speed = .82;
  XT_Wav_Class PianoC5_Mid(C5Note_Mid);
  PianoC5_Mid.Speed = .82;
  
  Serial.println("Objects created.");

  // Playback

  Serial.println("Playback...");
  
  Serial.println("--- C4 ---");

  for (counter = 0; counter < 2; counter++){
    if(PianoC4_Mid.Playing==false){
      DacAudio1.Play(&PianoC4_Mid);
      PianoC4_Mid.Looped=false;
      Serial.println("*mid*");
    }
    while(PianoC4_Mid.Looped == false){
      DacAudio1.FillBuffer();
    }
  }

  Serial.println("--- D4 ---");

  for (counter = 0; counter < 2; counter++){
    if(PianoD4_Mid.Playing==false){
      DacAudio1.Play(&PianoD4_Mid);
      PianoD4_Mid.Looped=false;
      Serial.println("*mid*");
    }
    while(PianoD4_Mid.Looped == false){
      DacAudio1.FillBuffer();
    }
  }

  Serial.println("--- E4 ---");

  for (counter = 0; counter < 2; counter++){
    if(PianoE4_Mid.Playing==false){
      DacAudio1.Play(&PianoE4_Mid);
      PianoE4_Mid.Looped=false;
      Serial.println("*mid*");
    }
    while(PianoE4_Mid.Looped == false){
      DacAudio1.FillBuffer();
    }
  }

  Serial.println("--- F4 ---");

  for (counter = 0; counter < 2; counter++){
    if(PianoF4_Mid.Playing==false){
      DacAudio1.Play(&PianoF4_Mid);
      PianoF4_Mid.Looped=false;
      Serial.println("*mid*");
    }
    while(PianoF4_Mid.Looped == false){
      DacAudio1.FillBuffer();
    }
  }

  Serial.println("--- G4 ---");

  for (counter = 0; counter < 2; counter++){
    if(PianoG4_Mid.Playing==false){
      DacAudio1.Play(&PianoG4_Mid);
      PianoG4_Mid.Looped=false;
      Serial.println("*mid*");
    }
    while(PianoG4_Mid.Looped == false){
      DacAudio1.FillBuffer();
    }
  }

  Serial.println("--- A4 ---");

  for (counter = 0; counter < 2; counter++){
    if(PianoA4_Mid.Playing==false){
      DacAudio1.Play(&PianoA4_Mid);
      PianoA4_Mid.Looped=false;
      Serial.println("*mid*");
    }
    while(PianoA4_Mid.Looped == false){
      DacAudio1.FillBuffer();
    }
  }

  Serial.println("--- B4 ---");

  for (counter = 0; counter < 2; counter++){
    if(PianoB4_Mid.Playing==false){
      DacAudio1.Play(&PianoB4_Mid);
      PianoB4_Mid.Looped=false;
      Serial.println("*mid*");
    }
    while(PianoB4_Mid.Looped == false){
      DacAudio1.FillBuffer();
    }
  }

  Serial.println("--- C5 ---");

  for (counter = 0; counter < 2; counter++){
    if(PianoC5_Mid.Playing==false){
      DacAudio1.Play(&PianoC5_Mid);
      PianoC5_Mid.Looped=false;
      Serial.println("*mid*");
    }
    while(PianoC5_Mid.Looped == false){
      DacAudio1.FillBuffer();
    }
  }

  // End all audio
  
  Serial.println("");
  Serial.println("----------");
  Serial.println("");
  
  DacAudio1.StopAllSounds();
  Serial.println("Done.");

  // Free the allocated memory
  
  free(C4Note_Mid);
  free(D4Note_Mid);
  free(E4Note_Mid);
  free(F4Note_Mid);
  free(G4Note_Mid);
  free(A4Note_Mid);
  free(B4Note_Mid);
  free(C5Note_Mid);
}

// Default loop() function
void loop(){
  // Contains no code because of issues with objects' references
}
