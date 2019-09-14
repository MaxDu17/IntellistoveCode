#include <Rotary.h>

bool push = false; //sets the push variable to false to start out with//
Rotary rotary; 
void detect()
{
rotary.detect(); 
  
}
void setup()
{
  Serial.begin(9600);
  rotary.begin(3,2,7); //clk is on pin 3, dt is on pin 2, sw is on pin 7. 
  attachInterrupt(rotary.getPin(), detect, FALLING); //attachInterrupt function implemented outside of library//
}

void loop() 
{
 push = rotary.isPushed(); 

if(push == true)
{
Serial.print("pushed\n"); 
}

delay(100); 
}
