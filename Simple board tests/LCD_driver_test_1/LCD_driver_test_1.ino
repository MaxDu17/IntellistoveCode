#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 7, 6, 5, 4);

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(20, 4);
  lcd.setCursor(0, 0);
  // Print a message to the LCD.
  lcd.print("computer speaking");
  lcd.setCursor(0,1);
  lcd.print("aaaaa");
  1; 
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 3);
  // print the number of seconds since reset:
  lcd.print(millis() / 1000);
}
