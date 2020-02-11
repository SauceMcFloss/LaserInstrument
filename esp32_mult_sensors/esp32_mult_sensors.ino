int LED_BUILTIN = 2;
int diode = 23; //GPIO pin 22

int sensor1 = A0; // anolog pin 0
int sensor2 = A3; // anolog pin 3
int sensor3 = A6; // anolog pin 6
int sensor4 = A7; // anolog pin 7

void setup() {
  Serial.begin(9600);           //  setup serial
  pinMode (LED_BUILTIN, OUTPUT);
  pinMode (diode, OUTPUT);
  
  pinMode (sensor1, INPUT);
  pinMode (sensor2, INPUT);
  pinMode (sensor3, INPUT);
  pinMode (sensor4, INPUT);
}


void loop() {
  //digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(diode, HIGH);
  
  int sen1 = analogRead(sensor1);  // read the input pin
  int sen2 = analogRead(sensor2);  // read the input pin
  int sen3 = analogRead(sensor3);  // read the input pin
  int sen4 = analogRead(sensor4);  // read the input pin
  

  if(sen1 > 150)
  {
    Serial.print("Sensor 1! ");
    Serial.println(sen1);
  }

  else if(sen2 > 150)
  {
    Serial.print("Sensor 2! ");
    Serial.println(sen2);
  }

  else if(sen3 > 150)
  {
    Serial.print("Sensor 3! ");
    Serial.println(sen3);
  }

  else if(sen4 > 150) 
  {
    Serial.print("Sensor 4! " );
    Serial.println(sen4);
  }

  else
  {
    Serial.println("Searching...");
  }


}
