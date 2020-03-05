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

  PrepareC4Note();

  PlayC4Note();

  // End program

  PowerDown();
}

// Default loop() function
void loop(){
  //DacAudio.FillBuffer(); // Always attempt to fill audio buffer, which plays any active sound item(s)
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

// Fill a playable note array with the associate .wav bytes
void PrepareC4Note()
{
  free(C4Note);
  long fileSize = FindMallocSize("/flute_C4_1_mezzo-forte_normal_mid.wav");
  C4Note = (unsigned char*)malloc(fileSize * sizeof(unsigned char));
  
  File file = SD.open("/flute_C4_1_mezzo-forte_normal_mid.wav");
  long counter = 0;
  while(file.available() && (counter < fileSize)){
    C4Note[counter] = (unsigned char)file.read();
    counter++;
  }
  file.close();
}

void PlayC4Note()
{
  XT_Wav_Class C4Note_Playable(C4Note);
  C4Note_Playable.Speed = .82;
  C4Note_Playable.RepeatForever = true;
  DacAudio.Play(&C4Note_Playable);

  for (int i = 0; i < 20000; i++){ // This is where we put "while the user is pressing a key"
    DacAudio.FillBuffer();
  }
}

// Called at the end of the program to safely shut down and free all memory
void PowerDown ()
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
