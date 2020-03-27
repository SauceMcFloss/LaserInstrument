const int pin_A = 4;  
const int pin_B = 15;  
const int pin_but = 27;

long int counter = 0;

unsigned char encoder_A;
unsigned char encoder_B;
unsigned char encoder_A_prev=0;

void IRAM_ATTR rot_interrupt() {
  encoder_A = digitalRead(pin_A);    // Read encoder pins
  encoder_B = digitalRead(pin_B);   
  if((!encoder_A) && (encoder_A_prev)){
    // A has gone from high to low 
    if(encoder_B) {
      // B is high so clockwise
      // increase the brightness, dont go over 255
      Serial.println("Clockwise");        
    }   
    else {
      // B is low so counter-clockwise      
      // decrease the brightness, dont go below 0
      Serial.println("Counter-Clockwise");          
    }   
  }   
  encoder_A_prev = encoder_A;     // Store value of A for next time  
}

void setup() {
  Serial.begin(115200);
  
  pinMode(pin_A, INPUT_PULLUP);
  pinMode(pin_B, INPUT_PULLUP);
  pinMode(pin_but, INPUT_PULLUP);

  attachInterrupt(pin_A, rot_interrupt, CHANGE);
  attachInterrupt(pin_B, rot_interrupt, CHANGE);
}

void loop() {
  Serial.println(digitalRead(pin_but));
  delay(100);
}