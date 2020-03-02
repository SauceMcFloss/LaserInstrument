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
  
  Serial.println("Allocating...");
  unsigned char* C4Note_Beg = (unsigned char*)malloc(5461 * sizeof(unsigned char));
  unsigned char* C4Note_Mid = (unsigned char*)malloc(35244 * sizeof(unsigned char));
  unsigned char* C4Note_End = (unsigned char*)malloc(2230 * sizeof(unsigned char));
  unsigned char* D4Note_Beg = (unsigned char*)malloc(3291 * sizeof(unsigned char));
  unsigned char* D4Note_Mid = (unsigned char*)malloc(35722 * sizeof(unsigned char));
  unsigned char* D4Note_End = (unsigned char*)malloc(5275 * sizeof(unsigned char));
  unsigned char* E4Note_Beg = (unsigned char*)malloc(15135 * sizeof(unsigned char));
  unsigned char* E4Note_Mid = (unsigned char*)malloc(16984 * sizeof(unsigned char));
  unsigned char* E4Note_End = (unsigned char*)malloc(11607 * sizeof(unsigned char));
  unsigned char* F4Note_Beg = (unsigned char*)malloc(6488 * sizeof(unsigned char));
  unsigned char* F4Note_Mid = (unsigned char*)malloc(11074 * sizeof(unsigned char));
  unsigned char* F4Note_End = (unsigned char*)malloc(21653 * sizeof(unsigned char));
  unsigned char* G4Note_Beg = (unsigned char*)malloc(4569 * sizeof(unsigned char));
  unsigned char* G4Note_Mid = (unsigned char*)malloc(32305 * sizeof(unsigned char));
  unsigned char* G4Note_End = (unsigned char*)malloc(4889 * sizeof(unsigned char));
  unsigned char* A4Note_Beg = (unsigned char*)malloc(17194 * sizeof(unsigned char));
  unsigned char* A4Note_Mid = (unsigned char*)malloc(17909 * sizeof(unsigned char));
  unsigned char* A4Note_End = (unsigned char*)malloc(16623 * sizeof(unsigned char));
  unsigned char* B4Note_Beg = (unsigned char*)malloc(3953 * sizeof(unsigned char));
  unsigned char* B4Note_Mid = (unsigned char*)malloc(27573 * sizeof(unsigned char));
  unsigned char* B4Note_End = (unsigned char*)malloc(9727 * sizeof(unsigned char));
  unsigned char* C5Note_Beg = (unsigned char*)malloc(3914 * sizeof(unsigned char));
  unsigned char* C5Note_Mid = (unsigned char*)malloc(26086 * sizeof(unsigned char));
  unsigned char* C5Note_End = (unsigned char*)malloc(7960 * sizeof(unsigned char));
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

  file = SD.open("/flute_C4_1_mezzo-forte_normal_beg.wav");
  counter = 0;
  while(file.available() && (counter < 17174)){
    C4Note_Beg[counter] = (unsigned char)file.read();
    counter++;
  }
  file.close();
  
  file = SD.open("/flute_C4_1_mezzo-forte_normal_mid.wav");
  counter = 0;
  while(file.available() && (counter < 17909)){
    C4Note_Mid[counter] = (unsigned char)file.read();
    counter++;
  }
  file.close();

  file = SD.open("/flute_C4_1_mezzo-forte_normal_end.wav");
  counter = 0;
  while(file.available() && (counter < 16623)){
    C4Note_End[counter] = (unsigned char)file.read();
    counter++;
  }
  file.close();

  // Prepare D4

  file = SD.open("/flute_D4_1_mezzo-forte_normal_beg.wav");
  counter = 0;
  while(file.available() && (counter < 17174)){
    D4Note_Beg[counter] = (unsigned char)file.read();
    counter++;
  }
  file.close();
  
  file = SD.open("/flute_D4_1_mezzo-forte_normal_mid.wav");
  counter = 0;
  while(file.available() && (counter < 17909)){
    D4Note_Mid[counter] = (unsigned char)file.read();
    counter++;
  }
  file.close();

  file = SD.open("/flute_D4_1_mezzo-forte_normal_end.wav");
  counter = 0;
  while(file.available() && (counter < 16623)){
    D4Note_End[counter] = (unsigned char)file.read();
    counter++;
  }
  file.close();

  // Prepare E4

  file = SD.open("/flute_E4_1_mezzo-forte_normal_beg.wav");
  counter = 0;
  while(file.available() && (counter < 17174)){
    E4Note_Beg[counter] = (unsigned char)file.read();
    counter++;
  }
  file.close();
  
  file = SD.open("/flute_E4_1_mezzo-forte_normal_mid.wav");
  counter = 0;
  while(file.available() && (counter < 17909)){
    E4Note_Mid[counter] = (unsigned char)file.read();
    counter++;
  }
  file.close();

  file = SD.open("/flute_E4_1_mezzo-forte_normal_end.wav");
  counter = 0;
  while(file.available() && (counter < 16623)){
    E4Note_End[counter] = (unsigned char)file.read();
    counter++;
  }
  file.close();

  // Prepare F4

  file = SD.open("/flute_F4_1_mezzo-forte_normal_beg.wav");
  counter = 0;
  while(file.available() && (counter < 17174)){
    F4Note_Beg[counter] = (unsigned char)file.read();
    counter++;
  }
  file.close();
  
  file = SD.open("/flute_F4_1_mezzo-forte_normal_mid.wav");
  counter = 0;
  while(file.available() && (counter < 17909)){
    F4Note_Mid[counter] = (unsigned char)file.read();
    counter++;
  }
  file.close();

  file = SD.open("/flute_F4_1_mezzo-forte_normal_end.wav");
  counter = 0;
  while(file.available() && (counter < 16623)){
    F4Note_End[counter] = (unsigned char)file.read();
    counter++;
  }
  file.close();

  // Prepare G4

  file = SD.open("/flute_G4_1_mezzo-forte_normal_beg.wav");
  counter = 0;
  while(file.available() && (counter < 17174)){
    G4Note_Beg[counter] = (unsigned char)file.read();
    counter++;
  }
  file.close();
  
  file = SD.open("/flute_G4_1_mezzo-forte_normal_mid.wav");
  counter = 0;
  while(file.available() && (counter < 17909)){
    G4Note_Mid[counter] = (unsigned char)file.read();
    counter++;
  }
  file.close();

  file = SD.open("/flute_G4_1_mezzo-forte_normal_end.wav");
  counter = 0;
  while(file.available() && (counter < 16623)){
    G4Note_End[counter] = (unsigned char)file.read();
    counter++;
  }
  file.close();

  // Prepare A4

  file = SD.open("/flute_A4_1_mezzo-forte_normal_beg.wav");
  counter = 0;
  while(file.available() && (counter < 17174)){
    A4Note_Beg[counter] = (unsigned char)file.read();
    counter++;
  }
  file.close();
  
  file = SD.open("/flute_A4_1_mezzo-forte_normal_mid.wav");
  counter = 0;
  while(file.available() && (counter < 17909)){
    A4Note_Mid[counter] = (unsigned char)file.read();
    counter++;
  }
  file.close();

  file = SD.open("/flute_A4_1_mezzo-forte_normal_end.wav");
  counter = 0;
  while(file.available() && (counter < 16623)){
    A4Note_End[counter] = (unsigned char)file.read();
    counter++;
  }
  file.close();

  // Prepare B4

  file = SD.open("/flute_B4_1_mezzo-forte_normal_beg.wav");
  counter = 0;
  while(file.available() && (counter < 17174)){
    B4Note_Beg[counter] = (unsigned char)file.read();
    counter++;
  }
  file.close();
  
  file = SD.open("/flute_B4_1_mezzo-forte_normal_mid.wav");
  counter = 0;
  while(file.available() && (counter < 17909)){
    B4Note_Mid[counter] = (unsigned char)file.read();
    counter++;
  }
  file.close();

  file = SD.open("/flute_B4_1_mezzo-forte_normal_end.wav");
  counter = 0;
  while(file.available() && (counter < 16623)){
    B4Note_End[counter] = (unsigned char)file.read();
    counter++;
  }
  file.close();

  // Prepare C5

  file = SD.open("/flute_C5_1_mezzo-forte_normal_beg.wav");
  counter = 0;
  while(file.available() && (counter < 17174)){
    C5Note_Beg[counter] = (unsigned char)file.read();
    counter++;
  }
  file.close();
  
  file = SD.open("/flute_C5_1_mezzo-forte_normal_mid.wav");
  counter = 0;
  while(file.available() && (counter < 17909)){
    C5Note_Mid[counter] = (unsigned char)file.read();
    counter++;
  }
  file.close();

  file = SD.open("/flute_C5_1_mezzo-forte_normal_end.wav");
  counter = 0;
  while(file.available() && (counter < 16623)){
    C5Note_End[counter] = (unsigned char)file.read();
    counter++;
  }
  file.close();

  Serial.println("Files read.");

  // Create XT_Wav_Class objects to play

  Serial.println("Creating XT_Wav_Class objects for playback...");
  
  XT_Wav_Class PianoC4_Beg(C4Note_Beg);
  PianoC4_Beg.Speed = .82;
  XT_Wav_Class PianoC4_Mid(C4Note_Mid);
  PianoC4_Mid.Speed = .82;
  XT_Wav_Class PianoC4_End(C4Note_End);
  PianoC4_End.Speed = .82;

  XT_Wav_Class PianoD4_Beg(D4Note_Beg);
  PianoD4_Beg.Speed = .82;
  XT_Wav_Class PianoD4_Mid(D4Note_Mid);
  PianoD4_Mid.Speed = .82;
  XT_Wav_Class PianoD4_End(D4Note_End);
  PianoD4_End.Speed = .82;

  XT_Wav_Class PianoE4_Beg(E4Note_Beg);
  PianoE4_Beg.Speed = .82;
  XT_Wav_Class PianoE4_Mid(E4Note_Mid);
  PianoE4_Mid.Speed = .82;
  XT_Wav_Class PianoE4_End(E4Note_End);
  PianoE4_End.Speed = .82;

  XT_Wav_Class PianoF4_Beg(F4Note_Beg);
  PianoF4_Beg.Speed = .82;
  XT_Wav_Class PianoF4_Mid(F4Note_Mid);
  PianoF4_Mid.Speed = .82;
  XT_Wav_Class PianoF4_End(F4Note_End);
  PianoF4_End.Speed = .82;

  XT_Wav_Class PianoG4_Beg(G4Note_Beg);
  PianoG4_Beg.Speed = .82;
  XT_Wav_Class PianoG4_Mid(G4Note_Mid);
  PianoG4_Mid.Speed = .82;
  XT_Wav_Class PianoG4_End(G4Note_End);
  PianoG4_End.Speed = .82;

  XT_Wav_Class PianoA4_Beg(A4Note_Beg);
  PianoA4_Beg.Speed = .82;
  XT_Wav_Class PianoA4_Mid(A4Note_Mid);
  PianoA4_Mid.Speed = .82;
  XT_Wav_Class PianoA4_End(A4Note_End);
  PianoA4_End.Speed = .82;

  XT_Wav_Class PianoB4_Beg(B4Note_Beg);
  PianoB4_Beg.Speed = .82;
  XT_Wav_Class PianoB4_Mid(B4Note_Mid);
  PianoB4_Mid.Speed = .82;
  XT_Wav_Class PianoB4_End(B4Note_End);
  PianoB4_End.Speed = .82;

  XT_Wav_Class PianoC5_Beg(C5Note_Beg);
  PianoC5_Beg.Speed = .82;
  XT_Wav_Class PianoC5_Mid(C5Note_Mid);
  PianoC5_Mid.Speed = .82;
  XT_Wav_Class PianoC5_End(C5Note_End);
  PianoC5_End.Speed = .82;

  Serial.println("Objects created.");

  // Playback

  Serial.println("Playback...");
  
  Serial.println("--- C4 ---");

  if(PianoC4_Beg.Playing==false){
    DacAudio1.Play(&PianoC4_Beg);
    PianoC4_Beg.Looped=false;
    Serial.println("*beg*");
  }
  while(PianoC4_Beg.Looped == false){
    DacAudio1.FillBuffer();
  }

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

  if(PianoC4_End.Playing==false){
    DacAudio1.Play(&PianoC4_End);
    PianoC4_End.Looped=false;
    Serial.println("*end*");
  }
  while(PianoC4_End.Looped == false){
    DacAudio1.FillBuffer();
  }

  Serial.println("--- D4 ---");

  if(PianoD4_Beg.Playing==false){
    DacAudio1.Play(&PianoD4_Beg);
    PianoD4_Beg.Looped=false;
    Serial.println("*beg*");
  }
  while(PianoD4_Beg.Looped == false){
    DacAudio1.FillBuffer();
  }

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

  if(PianoD4_End.Playing==false){
    DacAudio1.Play(&PianoD4_End);
    PianoD4_End.Looped=false;
    Serial.println("*end*");
  }
  while(PianoD4_End.Looped == false){
    DacAudio1.FillBuffer();
  }

  Serial.println("--- E4 ---");

  if(PianoE4_Beg.Playing==false){
    DacAudio1.Play(&PianoE4_Beg);
    PianoE4_Beg.Looped=false;
    Serial.println("*beg*");
  }
  while(PianoE4_Beg.Looped == false){
    DacAudio1.FillBuffer();
  }

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

  if(PianoE4_End.Playing==false){
    DacAudio1.Play(&PianoE4_End);
    PianoE4_End.Looped=false;
    Serial.println("*end*");
  }
  while(PianoE4_End.Looped == false){
    DacAudio1.FillBuffer();
  }

  Serial.println("--- F4 ---");

  if(PianoF4_Beg.Playing==false){
    DacAudio1.Play(&PianoF4_Beg);
    PianoF4_Beg.Looped=false;
    Serial.println("*beg*");
  }
  while(PianoF4_Beg.Looped == false){
    DacAudio1.FillBuffer();
  }

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

  if(PianoF4_End.Playing==false){
    DacAudio1.Play(&PianoF4_End);
    PianoF4_End.Looped=false;
    Serial.println("*end*");
  }
  while(PianoF4_End.Looped == false){
    DacAudio1.FillBuffer();
  }

  Serial.println("--- G4 ---");

  if(PianoG4_Beg.Playing==false){
    DacAudio1.Play(&PianoG4_Beg);
    PianoG4_Beg.Looped=false;
    Serial.println("*beg*");
  }
  while(PianoG4_Beg.Looped == false){
    DacAudio1.FillBuffer();
  }

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

  if(PianoG4_End.Playing==false){
    DacAudio1.Play(&PianoG4_End);
    PianoG4_End.Looped=false;
    Serial.println("*end*");
  }
  while(PianoG4_End.Looped == false){
    DacAudio1.FillBuffer();
  }

  Serial.println("--- A4 ---");

  if(PianoA4_Beg.Playing==false){
    DacAudio1.Play(&PianoA4_Beg);
    PianoA4_Beg.Looped=false;
    Serial.println("*beg*");
  }
  while(PianoA4_Beg.Looped == false){
    DacAudio1.FillBuffer();
  }

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

  if(PianoA4_End.Playing==false){
    DacAudio1.Play(&PianoA4_End);
    PianoA4_End.Looped=false;
    Serial.println("*end*");
  }
  while(PianoA4_End.Looped == false){
    DacAudio1.FillBuffer();
  }

  Serial.println("--- B4 ---");

  if(PianoB4_Beg.Playing==false){
    DacAudio1.Play(&PianoB4_Beg);
    PianoB4_Beg.Looped=false;
    Serial.println("*beg*");
  }
  while(PianoB4_Beg.Looped == false){
    DacAudio1.FillBuffer();
  }

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

  if(PianoB4_End.Playing==false){
    DacAudio1.Play(&PianoB4_End);
    PianoB4_End.Looped=false;
    Serial.println("*end*");
  }
  while(PianoB4_End.Looped == false){
    DacAudio1.FillBuffer();
  }

  Serial.println("--- C5 ---");

  if(PianoC5_Beg.Playing==false){
    DacAudio1.Play(&PianoC5_Beg);
    PianoC5_Beg.Looped=false;
    Serial.println("*beg*");
  }
  while(PianoC5_Beg.Looped == false){
    DacAudio1.FillBuffer();
  }

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

  if(PianoC5_End.Playing==false){
    DacAudio1.Play(&PianoC5_End);
    PianoC5_End.Looped=false;
    Serial.println("*end*");
  }
  while(PianoC5_End.Looped == false){
    DacAudio1.FillBuffer();
  }
  
  // End all audio
  
  Serial.println("");
  Serial.println("----------");
  Serial.println("");
  
  DacAudio1.StopAllSounds();
  Serial.println("Done.");

  // Free the allocated memory
  
  free(C4Note_Beg);
  free(C4Note_Mid);
  free(C4Note_End);
  free(D4Note_Beg);
  free(D4Note_Mid);
  free(D4Note_End);
  free(E4Note_Beg);
  free(E4Note_Mid);
  free(E4Note_End);
  free(F4Note_Beg);
  free(F4Note_Mid);
  free(F4Note_End);
  free(G4Note_Beg);
  free(G4Note_Mid);
  free(G4Note_End);
  free(A4Note_Beg);
  free(A4Note_Mid);
  free(A4Note_End);
  free(B4Note_Beg);
  free(B4Note_Mid);
  free(B4Note_End);
  free(C5Note_Beg);
  free(C5Note_Mid);
  free(C5Note_End);
}

// Default loop() function
void loop(){
  // Contains no code because of issues with objects' references
}
