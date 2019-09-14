#define INIT 1
#include <LiquidCrystal.h> 
#include <EEPROM.h>
#include <Wire.h> 
LiquidCrystal lcd(12, 11, 7, 6, 5, 4); //making objects
void setup() {
       
    char c; 
    int i = 0; 
     Wire.begin(); 
     Serial.begin(9600); 
     lcd.begin(20, 4);
     lcd.setCursor(0,0);
     lcd.print("Please wait while");
     lcd.setCursor(0,1);
     lcd.print("we start the module");
     delay(10000);    
     lcd.clear();  
     if(EEPROM.read(INIT) == 0)
     {
      //EEPROM.write(INIT) = 1; 
      lcd.setCursor(0,0);
      lcd.print("Welcome to");
      lcd.setCursor(0,1);
      lcd.print("Intellistove");
      delay(2000); 
      lcd.clear(); 
      lcd.setCursor(0,0);
      lcd.print("Text the code to the");
      lcd.setCursor(0,1);
      lcd.print("number below");
      delay(1000); 
      Wire.beginTransmission(1); 
      Wire.write(101); 
      Wire.endTransmission(); 
      delay(100); 
      Wire.requestFrom(1, 11); 
      while(Wire.available())
      {
       c = Wire.read(); 
       Serial.print(c); 
       lcd.setCursor(i,2); 
       lcd.print(c);
       i++; 
      }
      lcd.setCursor(0,3);
      lcd.print("MAXDU17"); 
    
     }
}

void loop() {


}
