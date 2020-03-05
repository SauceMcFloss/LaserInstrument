#include <LiquidCrystal_I2C.h>

#include "XT_DAC_Audio.h"
#include <SD.h>
#include <SPI.h>

int LED_BUILTIN = 2;
int diode = 1; //GPIO pin 1

int sensorClow = A0; // anolog pin 0
int sensorD = A3; // anolog pin 3
int sensorE = A6; // anolog pin 6
int sensorF = A7; // anolog pin 7
//int sensorG = A7; // anolog pin 7
//int sensorA = A7; // anolog pin 7
//int sensorB = A7; // anolog pin 7
//int sensorChigh = A7; // anolog pin 7

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
  //pinMode (sensorG, INPUT);
  //pinMode (sensorA, INPUT);
  //pinMode (sensorB, INPUT);
  //pinMode (sensorChigh, INPUT);

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
  //int senG = analogRead(sensorG);  // read the input pin
  //int senA = analogRead(sensorA);  // read the input pin
  //int senB = analogRead(sensorB);  // read the input pin
  //int senChigh = analogRead(sensorChigh);  // read the input pin
  

  if(senClow > 150)
  {
    Serial.print("Sensor Clow! ");
    Serial.println(senClow);
    lcd.setCursor(2,1);
    lcd.write(1);
    lcd.print(" e f g a b c");

    
    //XT_Wav_Class PianoCl(ClNote);
    while(senClow > 150)
    {
      /*
      DacAudio1.FillBuffer();
      if(PianoCl.Playing==false)
      {
        DacAudio1.Play(&PianoCl);
        Serial.println("*noteCl*");
      }
      */

      senClow = analogRead(sensorClow);  // read the input pin
      Serial.print("Sensor Clow! ");
      Serial.println(senClow);
    }
  }

  else if(senD > 150)
  {
    Serial.print("Sensor D! ");
    Serial.println(senD);
    lcd.setCursor(2,1);
    lcd.write(1);
    lcd.print(" e f g a b c");

    
    //XT_Wav_Class PianoD(DNote);
    while(senD > 150)
    {
      /*
      DacAudio1.FillBuffer();
      if(PianoD.Playing==false)
      {
        DacAudio1.Play(&PianoD);
        Serial.println("*noteD*");
      }
      */

      senD = analogRead(sensorD);  // read the input pin
      Serial.print("Sensor D! ");
      Serial.println(senD);
    }
  }

  else if(senE > 150)
  {
    Serial.print("Sensor E! ");
    Serial.println(senE);
    lcd.setCursor(4,1);
    lcd.write(1);
    lcd.print(" f g a b c");

    
    //XT_Wav_Class PianoE(ENote);
    while(senE > 150)
    {
      /*
      DacAudio1.FillBuffer();
      if(PianoE.Playing==false)
      {
        DacAudio1.Play(&PianoE);
        Serial.println("*noteE*");
      }
      */

      senE = analogRead(sensorE);  // read the input pin
      Serial.print("Sensor E! ");
      Serial.println(senE);
    }
  }


  else if(senF > 150) 
  {
    Serial.print("Sensor F! " );
    Serial.println(senF);
    lcd.setCursor(6,1);
    lcd.write(1);
    lcd.print(" g a b c");

    
    //XT_Wav_Class PianoF(FNote);
    while(senF > 150)
    {
      /*
      DacAudio1.FillBuffer();
      if(PianoF.Playing==false)
      {
        DacAudio1.Play(&PianoF);
        Serial.println("*noteF*");
      }
      */

      senF = analogRead(sensorF);  // read the input pin
      Serial.print("Sensor F! " );
      Serial.println(senF);
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
