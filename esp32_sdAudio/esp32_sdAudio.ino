// Libraries
#include "XT_DAC_Audio.h"
#include <SD.h>
#include <SPI.h>
#include <stdlib.h>

// Global variables

XT_DAC_Audio_Class DacAudio(25,0); // DAC audio player assigned to pin 25 (could also support pin 26)

// Pointers to where the .wav bytes will be stored in a format playable by the XT_DAC_Audio_Class class
unsigned char* C4Note;
unsigned char* D4Note;
unsigned char* E4Note;
unsigned char* F4Note;
unsigned char* G4Note;
unsigned char* A4Note;
unsigned char* B4Note;
unsigned char* C5Note;

int SensorThreshold = 150;

int C4Sensor = A0; // anolog pin 0
int D4Sensor = A3; // anolog pin 3

// Default functions

// Default setup() function
void setup()
{
  // Debug output at 115200 baud
  
  Serial.begin(115200);
  
  // Set up sd

  Serial.println("Mounting card...");
  if(!SD.begin())
  {
     Serial.println("Card mount failed.");
     return;
  }
  Serial.println("Card mount succeeded.");

  // Test

  PrepareAllNotes_Flute();
  
  // End program

  FreeAllNotes();
}

// Default loop() function
void loop(){
  if(analogRead(C4Sensor) > SensorThreshold)
  {
    PlayNote(1, C4Sensor); // PlayNote (Note, Sensor) - Note = C4, C4Sensor = A0
  }
  if(analogRead(D4Sensor) > SensorThreshold)
  {
    PlayNote(2, D4Sensor); // PlayNote (Note, Sensor) - Note = C4, C4Sensor = A0
  }
}

// Other functions

// Read the first 8 bytes from the .wav file to determine how much space should be malloc'd for the playable array
int FindMallocSize (const char* filename)
{
  File file = SD.open(filename);
  unsigned char temp[8];
  for(int i = 0; i < 8; i++)
  {
    // Safety precautions
    if(!file.available())
    {
      return 0; // If the file is misread, improper format, or whatever... return 0 so no memory should be malloc'd
    }
    
    temp[i] = (unsigned char)file.read();
  }
  file.close();

  // Return: (data size read from .wav file) + other header bytes
  return ((temp[7] << 24 | temp[6] << 16 |temp[5] << 8 |temp[4]) + 8);
}

// Inits all notes with playable flute data
void PrepareAllNotes_Flute()
{
  PrepareNote_Flute(1);
  PrepareNote_Flute(2);
  PrepareNote_Flute(3);
  PrepareNote_Flute(4);
  PrepareNote_Flute(5);
  PrepareNote_Flute(6);
  PrepareNote_Flute(7);
  PrepareNote_Flute(8);
}
// Fill a playable note array at the selected index with the associate .wav bytes
void PrepareNote_Flute(int noteSelector)
{
  File file;
  char *filename;
  long counter = 0;

  if(noteSelector == 1)
  {
    free(C4Note);
    filename = "/flute_C4_mid.wav";
    C4Note = (unsigned char*)malloc(FindMallocSize(filename) * sizeof(unsigned char));
    file = SD.open(filename);
    while(file.available()){
      C4Note[counter] = (unsigned char)file.read();
      counter++;
    }
  }
  else if(noteSelector == 2)
  {
    free(D4Note);
    filename = "/flute_D4_mid.wav";
    D4Note = (unsigned char*)malloc(FindMallocSize(filename) * sizeof(unsigned char));
    file = SD.open(filename);
    while(file.available()){
      D4Note[counter] = (unsigned char)file.read();
      counter++;
    }
  }
  else if(noteSelector == 3)
  {
    free(E4Note);
    filename = "/flute_E4_mid.wav";
    E4Note = (unsigned char*)malloc(FindMallocSize(filename) * sizeof(unsigned char));
    file = SD.open(filename);
    while(file.available()){
      E4Note[counter] = (unsigned char)file.read();
      counter++;
    }
  }
  else if(noteSelector == 4)
  {
    free(F4Note);
    filename = "/flute_F4_mid.wav";
    F4Note = (unsigned char*)malloc(FindMallocSize(filename) * sizeof(unsigned char));
    file = SD.open(filename);
    while(file.available()){
      F4Note[counter] = (unsigned char)file.read();
      counter++;
    }
  }
  else if(noteSelector == 5)
  {
    free(G4Note);
    filename = "/flute_G4_mid.wav";
    G4Note = (unsigned char*)malloc(FindMallocSize(filename) * sizeof(unsigned char));
    file = SD.open(filename);
    while(file.available()){
      G4Note[counter] = (unsigned char)file.read();
      counter++;
    }
  }
  else if(noteSelector == 6)
  {
    free(A4Note);
    filename = "/flute_A4_mid.wav";
    A4Note = (unsigned char*)malloc(FindMallocSize(filename) * sizeof(unsigned char));
    file = SD.open(filename);
    while(file.available()){
      A4Note[counter] = (unsigned char)file.read();
      counter++;
    }
  }
  else if(noteSelector == 7)
  {
    free(B4Note);
    filename = "/flute_B4_mid.wav";
    B4Note = (unsigned char*)malloc(FindMallocSize(filename) * sizeof(unsigned char));
    file = SD.open(filename);
    while(file.available()){
      B4Note[counter] = (unsigned char)file.read();
      counter++;
    }
  }
  else if(noteSelector == 8)
  {
    free(C5Note);
    filename = "/flute_C5_mid.wav";
    C5Note = (unsigned char*)malloc(FindMallocSize(filename) * sizeof(unsigned char));
    file = SD.open(filename);
    while(file.available()){
      C5Note[counter] = (unsigned char)file.read();
      counter++;
    }
  }
  else
  {
    Serial.println("Error preparing note! Note selector value must be 1 to 8.");
  }
  
  file.close();
}

// Inits all notes with playable trumpet data
void PrepareAllNotes_Trumpet()
{
  PrepareNote_Trumpet(1);
  PrepareNote_Trumpet(2);
  PrepareNote_Trumpet(3);
  PrepareNote_Trumpet(4);
  PrepareNote_Trumpet(5);
  PrepareNote_Trumpet(6);
  PrepareNote_Trumpet(7);
  PrepareNote_Trumpet(8);
}
// Fill a playable note array at the selected index with the associate .wav bytes
void PrepareNote_Trumpet(int noteSelector)
{
  File file;
  char *filename;
  long counter = 0;

  if(noteSelector == 1)
  {
    free(C4Note);
    filename = "/trumpet_C4_mid.wav";
    C4Note = (unsigned char*)malloc(FindMallocSize(filename) * sizeof(unsigned char));
    file = SD.open(filename);
    while(file.available()){
      C4Note[counter] = (unsigned char)file.read();
      counter++;
    }
  }
  else if(noteSelector == 2)
  {
    free(D4Note);
    filename = "/trumpet_D4_mid.wav";
    D4Note = (unsigned char*)malloc(FindMallocSize(filename) * sizeof(unsigned char));
    file = SD.open(filename);
    while(file.available()){
      D4Note[counter] = (unsigned char)file.read();
      counter++;
    }
  }
  else if(noteSelector == 3)
  {
    free(E4Note);
    filename = "/trumpet_E4_mid.wav";
    E4Note = (unsigned char*)malloc(FindMallocSize(filename) * sizeof(unsigned char));
    file = SD.open(filename);
    while(file.available()){
      E4Note[counter] = (unsigned char)file.read();
      counter++;
    }
  }
  else if(noteSelector == 4)
  {
    free(F4Note);
    filename = "/trumpet_F4_mid.wav";
    F4Note = (unsigned char*)malloc(FindMallocSize(filename) * sizeof(unsigned char));
    file = SD.open(filename);
    while(file.available()){
      F4Note[counter] = (unsigned char)file.read();
      counter++;
    }
  }
  else if(noteSelector == 5)
  {
    free(G4Note);
    filename = "/trumpet_G4_mid.wav";
    G4Note = (unsigned char*)malloc(FindMallocSize(filename) * sizeof(unsigned char));
    file = SD.open(filename);
    while(file.available()){
      G4Note[counter] = (unsigned char)file.read();
      counter++;
    }
  }
  else if(noteSelector == 6)
  {
    free(A4Note);
    filename = "/trumpet_A4_mid.wav";
    A4Note = (unsigned char*)malloc(FindMallocSize(filename) * sizeof(unsigned char));
    file = SD.open(filename);
    while(file.available()){
      A4Note[counter] = (unsigned char)file.read();
      counter++;
    }
  }
  else if(noteSelector == 7)
  {
    free(B4Note);
    filename = "/trumpet_B4_mid.wav";
    B4Note = (unsigned char*)malloc(FindMallocSize(filename) * sizeof(unsigned char));
    file = SD.open(filename);
    while(file.available()){
      B4Note[counter] = (unsigned char)file.read();
      counter++;
    }
  }
  else if(noteSelector == 8)
  {
    free(C5Note);
    filename = "/trumpet_C5_mid.wav";
    C5Note = (unsigned char*)malloc(FindMallocSize(filename) * sizeof(unsigned char));
    file = SD.open(filename);
    while(file.available()){
      C5Note[counter] = (unsigned char)file.read();
      counter++;
    }
  }
  else
  {
    Serial.println("Error preparing note! Note selector value must be 1 to 8.");
  }
  
  file.close();
}

// Play the note at the selected index while user input is detected at the specified sensor
void PlayNote(int noteSelector, int sensor)
{
  if(noteSelector == 1)
  {
    XT_Wav_Class C4Note_Playable(C4Note);
    C4Note_Playable.Speed = 1.0;
    C4Note_Playable.RepeatForever = true;
    DacAudio.Play(&C4Note_Playable);
  }
  else if(noteSelector == 2)
  {
    XT_Wav_Class D4Note_Playable(D4Note);
    D4Note_Playable.Speed = 1.0;
    D4Note_Playable.RepeatForever = true;
    DacAudio.Play(&D4Note_Playable);
  }
  else if(noteSelector == 3)
  {
    XT_Wav_Class E4Note_Playable(E4Note);
    E4Note_Playable.Speed = 1.0;
    E4Note_Playable.RepeatForever = true;
    DacAudio.Play(&E4Note_Playable);
  }
  else if(noteSelector == 4)
  {
    XT_Wav_Class F4Note_Playable(F4Note);
    F4Note_Playable.Speed = 1.0;
    F4Note_Playable.RepeatForever = true;
    DacAudio.Play(&F4Note_Playable);
  }
  else if(noteSelector == 5)
  {
    XT_Wav_Class G4Note_Playable(G4Note);
    G4Note_Playable.Speed = 1.0;
    G4Note_Playable.RepeatForever = true;
    DacAudio.Play(&G4Note_Playable);
  }
  else if(noteSelector == 6)
  {
    XT_Wav_Class A4Note_Playable(A4Note);
    A4Note_Playable.Speed = 1.0;
    A4Note_Playable.RepeatForever = true;
    DacAudio.Play(&A4Note_Playable);
  }
  else if(noteSelector == 7)
  {
    XT_Wav_Class B4Note_Playable(B4Note);
    B4Note_Playable.Speed = 1.0;
    B4Note_Playable.RepeatForever = true;
    DacAudio.Play(&B4Note_Playable);
  }
  else if(noteSelector == 8)
  {
    XT_Wav_Class C5Note_Playable(C5Note);
    C5Note_Playable.Speed = 1.0;
    C5Note_Playable.RepeatForever = true;
    DacAudio.Play(&C5Note_Playable);
  }
  else
  {
    Serial.println("Error playing note! Note selector value must be 1 to 8.");
  }

  while(analogRead(sensor) > SensorThreshold) // Play the loaded audio until the specified sensor stops registering user input
  {
    DacAudio.FillBuffer();
  }

  DacAudio.StopAllSounds(); // End all audio, hopefully stopping all residual whines or buzzes from the speaker
}

// Called at the end of the program to safely shut down and free all memory
void FreeAllNotes ()
{
  free(C4Note);
  free(D4Note);
  free(E4Note);
  free(F4Note);
  free(G4Note);
  free(A4Note);
  free(B4Note);
  free(C5Note);
}
