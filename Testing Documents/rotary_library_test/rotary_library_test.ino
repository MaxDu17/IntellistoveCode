#include <Rotary.h>
int dir = 0; 
bool push = false; 
Rotary rotary; 
void detect()
{
rotary.detect(); 
  
}
void setup()
{
  Serial.begin(9600);
  rotary.begin(3,2,7);
  attachInterrupt(rotary.getPin(), detect, FALLING);
}

void loop() 
{
 dir = rotary.getDirection(); 
 push = rotary.isPushed(); 
 if(dir != 0)
 {
  if(dir == 1)
  {
    Serial.print("clockwise\n");
  }
  else
  {
    Serial.print("counterclockwise\n"); 
  }
 
 }

if(push == true)
{
Serial.print("pushed\n"); 
}

delay(100); 
}
