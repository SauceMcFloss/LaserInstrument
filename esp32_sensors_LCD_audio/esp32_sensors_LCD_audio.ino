#include <LiquidCrystal_I2C.h>

#include "XT_DAC_Audio.h"
#include <SD.h>
#include <SPI.h>
#include <stdlib.h>

int threshold = 150;

int LED_BUILTIN = 2;
int diode = 1; //GPIO pin 1

int sensorClow = A0; // SVP
int sensorD = A3; // SVN
int sensorE = A6; // IO 34
int sensorF = A7; // IO 35
int sensorG = A4; // IO 32
int sensorA = A5; // IO 33
int sensorB = A16; // IO 14
int sensorChigh = A14; // IO 13

int lcdColumns = 16;
int lcdRows = 2;

// set LCD address, number of col and rows
LiquidCrystal_I2C lcd(0x3F, lcdColumns, lcdRows); 
byte note[8] = { 0b00000, 0b00100, 0b00110, 0b00101, 0b00100, 0b01100, 0b01100, 0b00000 };
byte doubleNote[8] = { 0b01000, 0b01100, 0b01010, 0b01001, 0b01001, 0b11001, 0b11011, 0b00011 }; 
byte sharpAccidental[8] = { 0b00010, 0b01011, 0b01110, 0b11010, 0b01011, 0b01110, 0b11010, 0b01000 };
byte flat[8] = { B00000, B10000, B10000, B10000, B11100, B10100, B11000, B10000 }; 


// SD Card Stuff
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



void setup() {
  Serial.begin(9600);           //  setup serial
  pinMode (LED_BUILTIN, OUTPUT);
  pinMode (diode, OUTPUT);
  
  pinMode (sensorClow, INPUT);
  pinMode (sensorD, INPUT);
  pinMode (sensorE, INPUT);
  pinMode (sensorF, INPUT);
  pinMode (sensorG, INPUT);
  pinMode (sensorA, INPUT);
  pinMode (sensorB, INPUT);
  pinMode (sensorChigh, INPUT);

  // initialize LCD and backlight
  lcd.init();                     
  lcd.backlight();
  lcd.createChar(0, note);
  lcd.createChar(1, doubleNote);
  lcd.createChar(2, sharpAccidental);
  lcd.createChar(3, flat);

  // print main screen
  lcd.setCursor(0, 0);
  lcd.print("Laser Test:");
  lcd.setCursor(0, 1);
  lcd.print("c d e f g a b c");

  // SD Setup
  Serial.println("Mounting card...");
  if(!SD.begin())
  {
     Serial.println("Card mount failed.");
     return;
  }
  Serial.println("Card mount succeeded.");

  // Default settings
  PrepareAllNotes_Flute();
}


void loop() {
  
  //digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(diode, HIGH);

  if(analogRead(sensorClow) > threshold)
  {
    // For Debugging
    Serial.print("Sensor Clow! ");
    lcd.setCursor(0,1);
    lcd.write(1);
    lcd.print(" d e f g a b c");
    //
    
    PlayNote(1, sensorClow); // PlayNote (Note, Sensor) - Note = CLow, CLowSensor = A0
  }

  if(analogRead(sensorD) > threshold)
  {
    // For Debugging
    Serial.print("Sensor D! ");
    lcd.setCursor(2,1);
    lcd.write(1);
    lcd.print(" e f g a b c");
    //
    
    PlayNote(2, sensorD); // PlayNote (Note, Sensor) - Note = D, DSensor = A3
  }

  if(analogRead(sensorE) > threshold)
  {
    // For Debugging
    Serial.print("Sensor E! ");
    lcd.setCursor(3,1);
    lcd.write(1);
    lcd.print(" f g a b c");
    //
    
    PlayNote(3, sensorE); // PlayNote (Note, Sensor) - Note = E, ESensor = A6
  }

  if(analogRead(sensorF) > threshold)
  {
    // For Debugging
    Serial.print("Sensor F! ");
    lcd.setCursor(6,1);
    lcd.write(1);
    lcd.print(" g a b c");
    //
    
    PlayNote(4, sensorF); // PlayNote (Note, Sensor) - Note = F, FSensor = A7
  }

  if(analogRead(sensorG) > threshold)
  {
    // For Debugging
    Serial.print("Sensor G! ");
    lcd.setCursor(8,1);
    lcd.write(1);
    lcd.print(" a b c");
    //
    
    PlayNote(5, sensorG); // PlayNote (Note, Sensor) - Note = G, GSensor = A4
  }

  if(analogRead(sensorA) > threshold)
  {
    // For Debugging
    Serial.print("Sensor A! ");
    lcd.setCursor(10,1);
    lcd.write(1);
    lcd.print(" b c");
    //
    
    PlayNote(6, sensorA); // PlayNote (Note, Sensor) - Note = A, ASensor = A5
  }

  if(analogRead(sensorB) > threshold)
  {
    // For Debugging
    Serial.print("Sensor B! ");
    lcd.setCursor(12,1);
    lcd.write(1);
    lcd.print(" c");
    //
    
    PlayNote(7, sensorB); // PlayNote (Note, Sensor) - Note = B, BSensor = A16
  }

  if(analogRead(sensorChigh) > threshold)
  {
    // For Debugging
    Serial.print("Sensor Chigh! ");
    lcd.setCursor(14,1);
    lcd.write(1);
    lcd.print("");
    //
    
    PlayNote(1, sensorChigh); // PlayNote (Note, Sensor) - Note = CHigh, CHighSensor = A14
  }

  // Default actions...

  Serial.println("Searching...");
  lcd.setCursor(0,1);
  lcd.print("c d e f g a b c");

  //lcd.print("                        ");
  //lcd.setCursor(0, 1);
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
  // For Debugging
  char* debugMessage;
  //
  
  if(noteSelector == 1)
  {
    // For Debugging
    debugMessage = "Sensor Clow! ";
    //
    
    XT_Wav_Class C4Note_Playable(C4Note);
    C4Note_Playable.Speed = 1.0;
    C4Note_Playable.RepeatForever = true;
    DacAudio.Play(&C4Note_Playable);
  }
  else if(noteSelector == 2)
  {
    // For Debugging
    debugMessage = "Sensor D! ";
    //
    
    XT_Wav_Class D4Note_Playable(D4Note);
    D4Note_Playable.Speed = 1.0;
    D4Note_Playable.RepeatForever = true;
    DacAudio.Play(&D4Note_Playable);
  }
  else if(noteSelector == 3)
  {
    // For Debugging
    debugMessage = "Sensor E! ";
    //
    
    XT_Wav_Class E4Note_Playable(E4Note);
    E4Note_Playable.Speed = 1.0;
    E4Note_Playable.RepeatForever = true;
    DacAudio.Play(&E4Note_Playable);
  }
  else if(noteSelector == 4)
  {
    // For Debugging
    debugMessage = "Sensor F! ";
    //
    
    XT_Wav_Class F4Note_Playable(F4Note);
    F4Note_Playable.Speed = 1.0;
    F4Note_Playable.RepeatForever = true;
    DacAudio.Play(&F4Note_Playable);
  }
  else if(noteSelector == 5)
  {
    // For Debugging
    debugMessage = "Sensor G! ";
    //
    
    XT_Wav_Class G4Note_Playable(G4Note);
    G4Note_Playable.Speed = 1.0;
    G4Note_Playable.RepeatForever = true;
    DacAudio.Play(&G4Note_Playable);
  }
  else if(noteSelector == 6)
  {
    // For Debugging
    debugMessage = "Sensor A! ";
    //
    
    XT_Wav_Class A4Note_Playable(A4Note);
    A4Note_Playable.Speed = 1.0;
    A4Note_Playable.RepeatForever = true;
    DacAudio.Play(&A4Note_Playable);
  }
  else if(noteSelector == 7)
  {
    // For Debugging
    debugMessage = "Sensor B! ";
    //
    
    XT_Wav_Class B4Note_Playable(B4Note);
    B4Note_Playable.Speed = 1.0;
    B4Note_Playable.RepeatForever = true;
    DacAudio.Play(&B4Note_Playable);
  }
  else if(noteSelector == 8)
  {
    // For Debugging
    debugMessage = "Sensor Chigh! ";
    //
    
    XT_Wav_Class C5Note_Playable(C5Note);
    C5Note_Playable.Speed = 1.0;
    C5Note_Playable.RepeatForever = true;
    DacAudio.Play(&C5Note_Playable);
  }
  else
  {
    // For Debugging
    debugMessage = "Error playing note! Note selector value must be 1 to 8. ";
    //
    
    Serial.println("Error playing note! Note selector value must be 1 to 8.");
  }

  while(analogRead(sensor) > threshold) // Play the loaded audio until the specified sensor stops registering user input
  {
    // For Debugging
    Serial.print(debugMessage);
    Serial.println(analogRead(sensor));
    //
    
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
