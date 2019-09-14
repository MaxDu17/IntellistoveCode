#include "max6675.h"
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int thermoDO = 8;
int thermoCS = 9;
int thermoCLK = 10;
float temp = 0; 

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

void setup() {
  pinMode(13, OUTPUT); 
  Serial.begin(9600);
  lcd.begin(20, 4);
  
  
  //Serial.println("MAX6675 test");
  // wait for MAX chip to stabilize
  delay(500);
}

void loop() {
 digitalWrite(13, HIGH); 
  
 temp = thermocouple.readFahrenheit();
 lcd.setCursor(0,0);
 lcd.print(temp); 
  delay(1000);
   lcd.clear(); 
 
}
