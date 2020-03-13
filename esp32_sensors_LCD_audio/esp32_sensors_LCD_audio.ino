#include <LiquidCrystal_I2C.h>

#include "XT_DAC_Audio.h"
#include <SD.h>
#include <SPI.h>

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
const long defaultWavLength = 210;
unsigned char ANote[defaultWavLength];
XT_DAC_Audio_Class DacAudio1(25,0);


void SetFileSize(unsigned char * list, long maximum){
  list[28] = 210;
  list[29] = 0x00;
  list[30] = 0x00;
  list[31] = 0x00;
}


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
  SetFileSize(ANote, defaultWavLength);
  
  Serial.println("---Stage 3 A---");
  counter = 0;
  while(file.available() && (counter < defaultWavLength))
  {
    ANote[counter] = (unsigned char)file.read();
    counter++;
  }
  
  file.close();
}


void loop() {
  
  //digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(diode, HIGH);
  
  int senClow = analogRead(sensorClow);  // read the input pin
  int senD = analogRead(sensorD);  // read the input pin
  int senE = analogRead(sensorE);  // read the input pin
  int senF = analogRead(sensorF);  // read the input pin
  int senG = analogRead(sensorG);  // read the input pin
  int senA = analogRead(sensorA);  // read the input pin
  int senB = analogRead(sensorB);  // read the input pin
  int senChigh = analogRead(sensorChigh);  // read the input pin
  

  if(senClow > threshold)
  {
    Serial.print("Sensor Clow! ");
    Serial.println(senClow);
    lcd.setCursor(0,1);
    lcd.write(1);
    lcd.print(" d e f g a b c");
    
    //XT_Wav_Class PianoCl(ClNote);
    while(senClow > threshold)
    {
      senClow = analogRead(sensorClow);  // read the input pin
      Serial.print("Sensor Clow! ");
      Serial.println(senClow);
    }
  }

  else if(senD > threshold)
  {
    Serial.print("Sensor D! ");
    Serial.println(senD);
    lcd.setCursor(2,1);
    lcd.write(1);
    lcd.print(" e f g a b c");
    
    //XT_Wav_Class PianoD(DNote);
    while(senD > threshold)
    {
      senD = analogRead(sensorD);  // read the input pin
      Serial.print("Sensor D! ");
      Serial.println(senD);
    }
  }

  else if(senE > threshold)
  {
    Serial.print("Sensor E! ");
    Serial.println(senE);
    lcd.setCursor(4,1);
    lcd.write(1);
    lcd.print(" f g a b c");
    
    //XT_Wav_Class PianoE(ENote);
    while(senE > threshold)
    {
      senE = analogRead(sensorE);  // read the input pin
      Serial.print("Sensor E! ");
      Serial.println(senE);
    }
  }


  else if(senF > threshold) 
  {
    Serial.print("Sensor F! " );
    Serial.println(senF);
    lcd.setCursor(6,1);
    lcd.write(1);
    lcd.print(" g a b c");
    
    //XT_Wav_Class PianoF(FNote);
    while(senF > threshold)
    {
      senF = analogRead(sensorF);  // read the input pin
      Serial.print("Sensor F! " );
      Serial.println(senF);
    }
  }

  else if(senG > threshold) 
  {
    Serial.print("Sensor G! " );
    Serial.println(senG);
    lcd.setCursor(8,1);
    lcd.write(1);
    lcd.print(" a b c");
    
    //XT_Wav_Class PianoF(FNote);
    while(senG > threshold)
    {
      senG = analogRead(sensorG);  // read the input pin
      Serial.print("Sensor G! " );
      Serial.println(senG);
    }
  }

  else if(senA > threshold) 
  {
    Serial.print("Sensor A! " );
    Serial.println(senA);
    lcd.setCursor(10,1);
    lcd.write(1);
    lcd.print(" b c");
    
    //XT_Wav_Class PianoF(FNote);
    while(senA > threshold)
    {
      senA = analogRead(sensorA);  // read the input pin
      Serial.print("Sensor A! " );
      Serial.println(senA);
    }
  }

  else if(senB > threshold) 
  {
    Serial.print("Sensor B! " );
    Serial.println(senB);
    lcd.setCursor(12,1);
    lcd.write(1);
    lcd.print(" c");

    //XT_Wav_Class PianoF(FNote);
    while(senB > threshold)
    {
      senB = analogRead(sensorB);  // read the input pin
      Serial.print("Sensor B! " );
      Serial.println(senB);
    }
  }

  else if(senChigh > threshold) 
  {
    Serial.print("Sensor Chigh! " );
    Serial.println(senChigh);
    lcd.setCursor(14,1);
    lcd.write(1);
    lcd.print("");
    
    //XT_Wav_Class PianoF(FNote);
    while(senChigh > threshold)
    {
      senChigh = analogRead(sensorChigh);  // read the input pin
      Serial.print("Sensor Chigh! " );
      Serial.println(senChigh);
    }
  }

  else
  {
    Serial.println("Searching...");
    lcd.setCursor(0,1);
    lcd.print("c d e f g a b c");
  }

  //lcd.print("                        ");
  //lcd.setCursor(0, 1);
}
