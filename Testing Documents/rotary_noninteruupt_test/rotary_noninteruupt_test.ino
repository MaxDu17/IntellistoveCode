#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 7, 6, 5, 4);
unsigned long currentTime;
unsigned long loopTime;
const int pin_A = 3;  // pin 12
const int pin_B = 2;  // pin 11
unsigned char encoder_A;
unsigned char encoder_B;
unsigned char encoder_A_prev=0;
int i = 0; 

void setup()  {
 lcd.begin(20, 4);
 
  pinMode(pin_A, INPUT);
  pinMode(pin_B, INPUT);
  currentTime = millis();
  loopTime = currentTime; 
  Serial.begin(9600);
} 

void loop()  {
  // get the current elapsed time
  currentTime = millis();
  if(currentTime >= (loopTime + 5)){
    // 5ms since last check of encoder = 200Hz  
    encoder_A = digitalRead(pin_A);    // Read encoder pins
    encoder_B = digitalRead(pin_B);   
    if((!encoder_A) && (encoder_A_prev)){
      // A has gone from high to low 
      if(encoder_B) {
       Serial.print("counterclockwise turn\n");
      }   
      else {
       Serial.print("clockwise turn\n");

    }   
    encoder_A_prev = encoder_A;     // Store value of A for next time    
    
   
   
    loopTime = currentTime;  // Updates loopTime
  }
  }
  // Other processing can be done here
                           
}

