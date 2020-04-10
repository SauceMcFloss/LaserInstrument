#include <LiquidCrystal_I2C.h>

#include "XT_DAC_Audio.h"
#include <SD.h>
#include <SPI.h>
#include <stdlib.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

// define BLE UUIDs
#define SERVICE_UUID        "03b80e5a-ede8-4b33-a751-6ce34ec4c700"
#define CHARACTERISTIC_UUID "7772e5db-3868-4112-a1a9-f2669d106bf3"

int threshold = 1;

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

// Rotary Encoder
int RotA_pin = 4;
int RotB_pin = 15;
int RotBut_pin = 27;

unsigned char RotA;
unsigned char RotB;
unsigned char RotA_prev = 0;
int InstrumentSelector = 2; // default = piano
int InstrumentPlaying = 2; // default = piano

// global BLE info and callbacks
BLECharacteristic *pCharacteristic;
bool deviceConnected = false;

uint8_t midi[] = {
   0x80,  // header
   0x80,  // timestamp (not implemented)
   0x00,  // status
   0x00,  // which note to play
   0x00   // velocity
};

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

void setup() {
  Serial.begin(9600);      //  setup serial

  // setup bluetooth low energy
  BLEDevice::init("Laser Instrument");

  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(BLEUUID(SERVICE_UUID));

  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
    BLEUUID(CHARACTERISTIC_UUID),
    BLECharacteristic::PROPERTY_READ   |
    BLECharacteristic::PROPERTY_WRITE  |
    BLECharacteristic::PROPERTY_NOTIFY |
    BLECharacteristic::PROPERTY_WRITE_NR
  );

  // Create a BLE Descriptor
  pCharacteristic->addDescriptor(new BLE2902());

  // Start the service
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->addServiceUUID(pService->getUUID());
  pAdvertising->start();

  // initialize LCD and backlight
  lcd.init();                     
  lcd.backlight();
  lcd.createChar(0, note);
  lcd.createChar(1, doubleNote);
  lcd.createChar(2, sharpAccidental);
  lcd.createChar(3, flat);

  // print main screen
  lcd.setCursor(0, 0);
  lcd.print("Startup...     ");
  Serial.print("Startup...");
  lcd.setCursor(0, 1);
  lcd.print("c d e f g a b c");

  // setup pins
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

  pinMode(RotA_pin, INPUT_PULLUP);
  pinMode(RotB_pin, INPUT_PULLUP);
  pinMode(RotBut_pin, INPUT_PULLUP);

  // setup SD
//  Serial.println("Mounting card...");
//  if(!SD.begin())
//  {
//     Serial.println("Card mount failed.");
//     return;
//  }
  Serial.println("Card mount succeeded.");

  // prepare for playing
  PrepareAllNotes_Piano(); // piano is default at startup
}


void loop()
{   
  //digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(diode, HIGH);

  // Rotary Encoder Check
  // rotation
  RotA = digitalRead(RotA_pin);    // Read encoder pins
  RotB = digitalRead(RotB_pin);   
  
  if((!RotA) && (RotA_prev))
  {
    // A has gone from high to low 
    if(RotB) {
      // B is high so clockwise
      InstrumentSelector = (InstrumentSelector + 1) % 5;
    }   
    else {
      // B is low so counter-clockwise      
      InstrumentSelector = (InstrumentSelector - 1) % 5;
    }

    delay(100);
  }   
  RotA_prev = RotA;     // Store value of A for next time

  lcd.setCursor(0, 0);
  if(InstrumentSelector == 0){
    lcd.print("Flute");
    Serial.print("Flute");
    if(InstrumentSelector == InstrumentPlaying){
      lcd.print(":");
      Serial.print(":");
    }
    else{
      lcd.print("?");
      Serial.print("?");
    }
    lcd.print("         ");
  }
  else if(InstrumentSelector == 1){
    lcd.print("Percussion");
    Serial.print("Percussion");
    if(InstrumentSelector == InstrumentPlaying){
      lcd.print(":");
      Serial.print(":");
    }
    else{
      lcd.print("?");
      Serial.print("?");
    }
    lcd.print("    ");
  }
  else if(InstrumentSelector == 2){
    lcd.print("Piano");
    Serial.print("Piano");
    if(InstrumentSelector == InstrumentPlaying){
      lcd.print(":");
      Serial.print(":");
    }
    else{
      lcd.print("?");
      Serial.print("?");
    }
    lcd.print("         ");
  }
  else if(InstrumentSelector == 3){
    lcd.print("Trumpet");
    Serial.print("Trumpet");
    if(InstrumentSelector == InstrumentPlaying){
      lcd.print(":");
      Serial.print(":");
    }
    else{
      lcd.print("?");
      Serial.print("?");
    }
    lcd.print("       ");
  }
  else if(InstrumentSelector == 4){
    lcd.print("Violin");
    Serial.print("Violin");
    if(InstrumentSelector == InstrumentPlaying){
      lcd.print(":");
      Serial.print(":");
    }
    else{
      lcd.print("?");
      Serial.print("?");
    }
    lcd.print("        ");
  }

  // button
  if(!digitalRead(RotBut_pin)){
    if(InstrumentPlaying != InstrumentSelector)
    {
      InstrumentPlaying = InstrumentSelector;
  
      lcd.setCursor(0, 0);
      lcd.print("Preparing...   ");
      Serial.print("Preparing...");
    
      if(InstrumentSelector == 0){
        PrepareAllNotes_Flute();
      }
      else if(InstrumentSelector == 1){
        PrepareAllNotes_Percussion();
      }
      else if(InstrumentSelector == 2){
        PrepareAllNotes_Piano();
      }
      else if(InstrumentSelector == 3){
        PrepareAllNotes_Trumpet();
      }
      else if(InstrumentSelector == 4){
        PrepareAllNotes_Violin();
      } 
    }
  }
  else
  {

    if(analogRead(sensorClow) < threshold)
    {
      // For Debugging
      Serial.print("Sensor Clow! ");
      lcd.setCursor(0,1);
      lcd.write(1);
      lcd.print(" d e f g a b c");

      if (deviceConnected)
      {
        // send MIDI
        midi[2] = 0x90; // note down, channel 0
        midi[3] = 0x3c; // middle C
        midi[4] = 127;  // velocity
        pCharacteristic->setValue(midi, 5); // packet, length in bytes
        pCharacteristic->notify();
      }
      
      PlayNote(1, sensorClow); // PlayNote (Note, Sensor) - Note = CLow, CLowSensor = A0
    }
  
    if(analogRead(sensorD) < threshold)
    {
      // For Debugging
      Serial.print("Sensor D! ");
      lcd.setCursor(0,1);
      lcd.print("c ");
      lcd.write(1);
      lcd.print(" e f g a b c");

      if (deviceConnected)
      {
        // send MIDI
        midi[2] = 0x90; // note down, channel 0
        midi[3] = 0x3e; // D
        midi[4] = 127;  // velocity
        pCharacteristic->setValue(midi, 5); // packet, length in bytes
        pCharacteristic->notify();
      }
      
      PlayNote(2, sensorD); // PlayNote (Note, Sensor) - Note = D, DSensor = A3
    }
  
    if(analogRead(sensorE) < threshold)
    {
      // For Debugging
      Serial.print("Sensor E! ");
      lcd.setCursor(0,1);
      lcd.print("c d ");
      lcd.write(1);
      lcd.print(" f g a b c");

      if (deviceConnected)
      {
        // send MIDI
        midi[2] = 0x90; // note down, channel 0
        midi[3] = 0x40; // middle C
        midi[4] = 127;  // velocity
        pCharacteristic->setValue(midi, 5); // packet, length in bytes
        pCharacteristic->notify();
      }
      
      PlayNote(3, sensorE); // PlayNote (Note, Sensor) - Note = E, ESensor = A6
    }
  
    if(analogRead(sensorF) < threshold)
    {
      // For Debugging
      Serial.print("Sensor F! ");
      lcd.setCursor(0,1);
      lcd.print("c d e ");
      lcd.write(1);
      lcd.print(" g a b c");

      if (deviceConnected)
      {
        // send MIDI
        midi[2] = 0x90; // note down, channel 0
        midi[3] = 0x41; // middle C
        midi[4] = 127;  // velocity
        pCharacteristic->setValue(midi, 5); // packet, length in bytes
        pCharacteristic->notify();
      }
      
      PlayNote(4, sensorF); // PlayNote (Note, Sensor) - Note = F, FSensor = A7
    }
  
    if(analogRead(sensorG) < threshold)
    {
      // For Debugging
      Serial.print("Sensor G! ");
      lcd.setCursor(0,1);
      lcd.print("c d e f ");
      lcd.write(1);
      lcd.print(" a b c");

      if (deviceConnected)
      {
        // send MIDI
        midi[2] = 0x90; // note down, channel 0
        midi[3] = 0x43; // middle C
        midi[4] = 127;  // velocity
        pCharacteristic->setValue(midi, 5); // packet, length in bytes
        pCharacteristic->notify();
      }
      
      PlayNote(5, sensorG); // PlayNote (Note, Sensor) - Note = G, GSensor = A4
    }
  
    if(analogRead(sensorA) < threshold)
    {
      // For Debugging
      Serial.print("Sensor A! ");
      lcd.setCursor(0,1);
      lcd.print("c d e f g ");
      lcd.write(1);
      lcd.print(" b c");

      if (deviceConnected)
      {
        // send MIDI
        midi[2] = 0x90; // note down, channel 0
        midi[3] = 0x45; // middle C
        midi[4] = 127;  // velocity
        pCharacteristic->setValue(midi, 5); // packet, length in bytes
        pCharacteristic->notify();
      }
      
      PlayNote(6, sensorA); // PlayNote (Note, Sensor) - Note = A, ASensor = A5
    }
  
    if(analogRead(sensorB) < threshold)
    {
      // For Debugging
      Serial.print("Sensor B! ");
      lcd.setCursor(0,1);
      lcd.print("c d e f g a ");
      lcd.write(1);
      lcd.print(" c");

      if (deviceConnected)
      {
        // send MIDI
        midi[2] = 0x90; // note down, channel 0
        midi[3] = 0x47  ; // middle C
        midi[4] = 127;  // velocity
        pCharacteristic->setValue(midi, 5); // packet, length in bytes
        pCharacteristic->notify();
      }
      
      PlayNote(7, sensorB); // PlayNote (Note, Sensor) - Note = B, BSensor = A16
    }
  
    if(analogRead(sensorChigh) < threshold)
    {
      // For Debugging
      Serial.print("Sensor Chigh! ");
      lcd.setCursor(0,1);
      lcd.print("c d e f g a b ");
      lcd.write(1);
      lcd.print("");

      if (deviceConnected)
      {
        // send MIDI
        midi[2] = 0x90; // note down, channel 0
        midi[3] = 0x48; // middle C
        midi[4] = 127;  // velocity
        pCharacteristic->setValue(midi, 5); // packet, length in bytes
        pCharacteristic->notify();
      }
      
      PlayNote(8, sensorChigh); // PlayNote (Note, Sensor) - Note = CHigh, CHighSensor = A14
    }
  
    // Default actions...
  
    //Serial.println("Searching...");
    lcd.setCursor(0,1);
    lcd.print("c d e f g a b c");
  
    //lcd.print("                        ");
    //lcd.setCursor(0, 1);
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
    if(InstrumentPlaying != 1 && InstrumentPlaying != 2)
    {
      C4Note_Playable.RepeatForever = true;
    }
    DacAudio.Play(&C4Note_Playable);
  }
  else if(noteSelector == 2)
  {
    // For Debugging
    debugMessage = "Sensor D! ";
    //
    
    XT_Wav_Class D4Note_Playable(D4Note);
    D4Note_Playable.Speed = 1.0;
    if(InstrumentPlaying != 1 && InstrumentPlaying != 2)
    {
      D4Note_Playable.RepeatForever = true;
    }
    DacAudio.Play(&D4Note_Playable);
  }
  else if(noteSelector == 3)
  {
    // For Debugging
    debugMessage = "Sensor E! ";
    //
    
    XT_Wav_Class E4Note_Playable(E4Note);
    E4Note_Playable.Speed = 1.0;
    if(InstrumentPlaying != 1 && InstrumentPlaying != 2)
    {
      E4Note_Playable.RepeatForever = true;
    }
    DacAudio.Play(&E4Note_Playable);
  }
  else if(noteSelector == 4)
  {
    // For Debugging
    debugMessage = "Sensor F! ";
    //
    
    XT_Wav_Class F4Note_Playable(F4Note);
    F4Note_Playable.Speed = 1.0;
    if(InstrumentPlaying != 1 && InstrumentPlaying != 2)
    {
      F4Note_Playable.RepeatForever = true;
    }
    DacAudio.Play(&F4Note_Playable);
  }
  else if(noteSelector == 5)
  {
    // For Debugging
    debugMessage = "Sensor G! ";
    //
    
    XT_Wav_Class G4Note_Playable(G4Note);
    G4Note_Playable.Speed = 1.0;
    if(InstrumentPlaying != 2) // don't repeat if piano, but fine if percussion (drumroll key)
    {
      G4Note_Playable.RepeatForever = true;
    }
    DacAudio.Play(&G4Note_Playable);
  }
  else if(noteSelector == 6)
  {
    // For Debugging
    debugMessage = "Sensor A! ";
    //
    
    XT_Wav_Class A4Note_Playable(A4Note);
    A4Note_Playable.Speed = 1.0;
    if(InstrumentPlaying != 1 && InstrumentPlaying != 2)
    {
      A4Note_Playable.RepeatForever = true;
    }
    DacAudio.Play(&A4Note_Playable);
  }
  else if(noteSelector == 7)
  {
    // For Debugging
    debugMessage = "Sensor B! ";
    //
    
    XT_Wav_Class B4Note_Playable(B4Note);
    B4Note_Playable.Speed = 1.0;
    if(InstrumentPlaying != 1 && InstrumentPlaying != 2)
    {
      B4Note_Playable.RepeatForever = true;
    }
    DacAudio.Play(&B4Note_Playable);
  }
  else if(noteSelector == 8)
  {
    // For Debugging
    debugMessage = "Sensor Chigh! ";
    //
    
    XT_Wav_Class C5Note_Playable(C5Note);
    C5Note_Playable.Speed = 1.0;
    if(InstrumentPlaying != 1 && InstrumentPlaying != 2)
    {
      C5Note_Playable.RepeatForever = true;
    }
    DacAudio.Play(&C5Note_Playable);
  }
  else
  {
    // For Debugging
    debugMessage = "Error playing note! Note selector value must be 1 to 8. ";
    //
    
    Serial.println("Error playing note! Note selector value must be 1 to 8.");
  }

  while(analogRead(sensor) < threshold) // Play the loaded audio until the specified sensor stops registering user input
  {
    // For Debugging
    Serial.print(debugMessage);
    Serial.println(analogRead(sensor));
    //
    
    DacAudio.FillBuffer();
  }

  DacAudio.StopAllSounds(); // End all audio, hopefully stopping all residual whines or buzzes from the speaker
}

// Free all the memory malloc'd by Note data
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

  lcd.setCursor(0, 0);
  lcd.print("Flute:         ");
  Serial.print("Flute:");
}
// Fill a playable note array at the selected index with the associate .wav bytes
void PrepareNote_Flute(int noteSelector)
{
  File file;
  char *filename = "//";
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

// Inits all notes with playable percussion data
void PrepareAllNotes_Percussion()
{
  PrepareNote_Percussion(1);
  PrepareNote_Percussion(2);
  PrepareNote_Percussion(3);
  PrepareNote_Percussion(4);
  PrepareNote_Percussion(5);
  PrepareNote_Percussion(6);
  PrepareNote_Percussion(7);
  PrepareNote_Percussion(8);

  lcd.setCursor(0, 0);
  lcd.print("Percussion:    ");
  Serial.print("Percussion:");
}
// Fill a playable note array at the selected index with the associate .wav bytes
void PrepareNote_Percussion(int noteSelector)
{
  File file;
  char *filename = "//";
  long counter = 0;

  if(noteSelector == 1)
  {
    free(C4Note);
    filename = "/percussion_C4_mid.wav";
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
    filename = "/percussion_D4_mid.wav";
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
    filename = "/percussion_E4_mid.wav";
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
    filename = "/percussion_F4_mid.wav";
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
    filename = "/percussion_G4_mid.wav";
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
    filename = "/percussion_A4_mid.wav";
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
    filename = "/percussion_B4_mid.wav";
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
    filename = "/percussion_C5_mid.wav";
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

// Inits all notes with playable piano data
void PrepareAllNotes_Piano()
{
  PrepareNote_Piano(1);
  PrepareNote_Piano(2);
  PrepareNote_Piano(3);
  PrepareNote_Piano(4);
  PrepareNote_Piano(5);
  PrepareNote_Piano(6);
  PrepareNote_Piano(7);
  PrepareNote_Piano(8);

  lcd.setCursor(0, 0);
  lcd.print("Piano:         ");
  Serial.print("Piano:");
}
// Fill a playable note array at the selected index with the associate .wav bytes
void PrepareNote_Piano(int noteSelector)
{
  File file;
  char *filename = "//";
  long counter = 0;

  if(noteSelector == 1)
  {
    free(C4Note);
    filename = "/piano_C4_mid.wav";
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
    filename = "/piano_D4_mid.wav";
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
    filename = "/piano_E4_mid.wav";
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
    filename = "/piano_F4_mid.wav";
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
    filename = "/piano_G4_mid.wav";
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
    filename = "/piano_A4_mid.wav";
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
    filename = "/piano_B4_mid.wav";
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
    filename = "/piano_C5_mid.wav";
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
  lcd.println("HERE1");
  PrepareNote_Trumpet(1);
  lcd.println("HERE2");
  PrepareNote_Trumpet(2);
  PrepareNote_Trumpet(3);
  PrepareNote_Trumpet(4);
  PrepareNote_Trumpet(5);
  PrepareNote_Trumpet(6);
  PrepareNote_Trumpet(7);
  PrepareNote_Trumpet(8);

  lcd.setCursor(0, 0);
  lcd.print("Trumpet:       ");
  Serial.print("Trumpet:");
}
// Fill a playable note array at the selected index with the associate .wav bytes
void PrepareNote_Trumpet(int noteSelector)
{
  File file;
  char *filename = "//";
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

// Inits all notes with playable violin data
void PrepareAllNotes_Violin()
{
  PrepareNote_Violin(1);
  PrepareNote_Violin(2);
  PrepareNote_Violin(3);
  PrepareNote_Violin(4);
  PrepareNote_Violin(5);
  PrepareNote_Violin(6);
  PrepareNote_Violin(7);
  PrepareNote_Violin(8);

  lcd.setCursor(0, 0);
  lcd.print("Violin:        ");
  Serial.print("Violin:");
}
// Fill a playable note array at the selected index with the associate .wav bytes
void PrepareNote_Violin(int noteSelector)
{
  File file;
  char *filename = "//";
  long counter = 0;

  if(noteSelector == 1)
  {
    free(C4Note);
    filename = "/violin_C4_mid.wav";
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
    filename = "/violin_D4_mid.wav";
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
    filename = "/violin_E4_mid.wav";
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
    filename = "/violin_F4_mid.wav";
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
    filename = "/violin_G4_mid.wav";
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
    filename = "/violin_A4_mid.wav";
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
    filename = "/violin_B4_mid.wav";
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
    filename = "/violin_C5_mid.wav";
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
