#include <Rotary.h>

int lastdirection = 0; //makes sure this variable is 0 to start out with// 
Rotary rotary; //makes the object "rotary"//
void detect()//declares wrapper function. rotary.detect will not work in the attachInterrupt function, so a "wrapper" function is needed//
{
rotary.detect(); //here is the library function//
  
}
void setup()
{
  Serial.begin(9600);
  
  rotary.begin(3,2,7); //Clk pin on rotary is on pin 3 of Arduino. Dt pin is on pin 2 of Arduino. Switch pin is on pin 7 of Arduino.//
  
  attachInterrupt(rotary.getPin(), detect, FALLING); //here is the attachInterrupt 
}

void loop() 
{
 lastdirection = rotary.getDirection(); //get return value from function//
 if(lastdirection != 0) //if the rotary has turned since the last execution of rotary.getDirection().//
 {
  if(lastdirection == 1)
  {
    Serial.print("clockwise\n");
  }
  else
  {
    Serial.print("counterclockwise\n"); 
  }
 
 }

delay(100); //just to slow it down a bit//
}
