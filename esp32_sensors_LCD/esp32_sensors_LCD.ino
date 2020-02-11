#include <LiquidCrystal_I2C.h>

int LED_BUILTIN = 2;
int diode = 23; //GPIO pin 22

int sensorC = A0; // anolog pin 0
int sensorD = A3; // anolog pin 3
int sensorE = A6; // anolog pin 6
int sensorF = A7; // anolog pin 7
int lcdColumns = 16;
int lcdRows = 2;

// set LCD address, number of col and rows
LiquidCrystal_I2C lcd(0x3F, lcdColumns, lcdRows); 
byte note[8] = { 0b00000, 0b00100, 0b00110, 0b00101, 0b00100, 0b01100, 0b01100, 0b00000 };
byte doubleNote[8] = { 0b01000, 0b01100, 0b01010, 0b01001, 0b01001, 0b11001, 0b11011, 0b00011 }; 
byte sharpAccidental[8] = { 0b00010, 0b01011, 0b01110, 0b11010, 0b01011, 0b01110, 0b11010, 0b01000 };
byte flat[8] = { B00000, B10000, B10000, B10000, B11100, B10100, B11000, B10000 }; 

void setup() {
  Serial.begin(9600);           //  setup serial
  pinMode (LED_BUILTIN, OUTPUT);
  pinMode (diode, OUTPUT);
  
  pinMode (sensorC, INPUT);
  pinMode (sensorD, INPUT);
  pinMode (sensorE, INPUT);
  pinMode (sensorF, INPUT);

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
}


void loop() {
  //digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(diode, HIGH);
  
  int senC = analogRead(sensorC);  // read the input pin
  int senD = analogRead(sensorD);  // read the input pin
  int senE = analogRead(sensorE);  // read the input pin
  int senF = analogRead(sensorF);  // read the input pin
  

  if(senC > 150)
  {
    Serial.print("Sensor 1! ");
    Serial.println(senC);
    lcd.setCursor(0,1);
    lcd.write(1);
    lcd.print(" d e f g a b c");
  }

  else if(senD > 150)
  {
    Serial.print("Sensor 2! ");
    Serial.println(senD);
    lcd.setCursor(2,1);
    lcd.write(1);
    lcd.print(" e f g a b c");
  }

  else if(senE > 150)
  {
    Serial.print("Sensor 3! ");
    Serial.println(senE);
    lcd.setCursor(4,1);
    lcd.write(1);
    lcd.print(" f g a b c");
  }

  else if(senF > 150) 
  {
    Serial.print("Sensor 4! " );
    Serial.println(senF);
    lcd.setCursor(6,1);
    lcd.write(1);
    lcd.print(" g a b c");
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
