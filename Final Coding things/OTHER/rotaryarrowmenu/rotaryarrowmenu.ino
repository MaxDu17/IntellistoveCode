#include <LiquidCrystal.h>
#include <Rotary.h> 

LiquidCrystal lcd(12, 11, 7, 6, 5, 4);
int i = 0; 
int directionarr; 
int carrier = 0; 
byte arrow[8] = {
  0b10000,
  0b11000,
  0b11100,
  0b11110,
  0b11110,
  0b11100,
  0b11000,
  0b10000
};
void detect();
void setArrow(int row); 

Rotary rotary; 

void setup()
{
  rotary.begin(3,2,7);
  attachInterrupt(rotary.getPin(), detect, FALLING); 
  lcd.createChar(0, arrow);
  lcd.begin(20, 4);
  setArrow(0); 
}

void loop() 
{
  
 carrier = rotary.getDirection(); 
  if (carrier == 1 && i < 4)
  { 
    if(i == 3)
    {
      lcd.clear();
      setArrow(3); 
    }
    else
    {
    lcd.clear(); 
    i++;
    setArrow(i);
    }
  }
  else if (carrier == 2 && i >= 0)
  {
    if(i == 0)
    {
      lcd.clear();
      setArrow(0);
    }
    else
    {
    lcd.clear();
    i--;
    setArrow(i);
    }
    
  }
}
  
  
void detect()
{
rotary.detect();
}

void setArrow(int row)
{
  lcd.setCursor(0,row);
  lcd.write((uint8_t)0);
}

