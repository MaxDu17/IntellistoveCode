#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 7, 6, 5, 4);
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


void setup()
{
  // create a new custom character
  lcd.createChar(0, arrow);
  
  // set up number of columns and rows
  lcd.begin(20, 4);

  // print the custom char to the lcd
  lcd.write((uint8_t)0);
  // why typecast? see: http://arduino.cc/forum/index.php?topic=74666.0
}
void loop()
{
  
}

