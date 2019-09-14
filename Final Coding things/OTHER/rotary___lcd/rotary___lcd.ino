 #include <LiquidCrystal.h>
 #include <Rotary.h>
LiquidCrystal lcd(12, 11, 7, 6, 5, 4);
Rotary rotary;

int lastdirection = 0; 
void detect()
{
rotary.detect();
}
void setup() 
{
  rotary.begin(3,2,7);
  attachInterrupt(rotary.getPin(), detect, FALLING); 
 lcd.begin(20, 4);
 
 delay(100); 
}

void loop() 
{ 
 lastdirection = rotary.getDirection();


 if(lastdirection != 0) //if the rotary has turned since the last execution of rotary.getDirection().//
 {
  if(lastdirection == 1)
  {
    lcd.print("clockwise");
    delay(500); 
    lcd.clear(); 
  }
  else
  {
    lcd.print("counterclockwise");
    delay(500); 
    lcd.clear();  
  }
 
 }
 delay(100); 
  
 
}
 
 


