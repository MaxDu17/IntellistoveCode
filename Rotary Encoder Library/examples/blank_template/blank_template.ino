#include <Rotary.h>
int dir = 0; 
Rotary rotary; 
void detect()
{
  rotary.detect(); 
  
}
void setup()
{
  
  rotary.begin(foo, foo, foo); //put something here in the order of pin# of clk, dt, and sw.//
  attachInterrupt(rotary.getPin(), detect, FALLING);
}

void loop() 
{
 dir = rotary.getDirection(); 
 //do stuff here// 
}
