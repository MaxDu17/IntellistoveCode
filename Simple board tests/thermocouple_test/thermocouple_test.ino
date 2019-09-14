#include "max6675.h"
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 7, 6, 5, 4);

int temp = 0; 
int thermoDO = 8;
int thermoCS = 9;
int thermoCLK = 10;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);
int vccPin = 3;
int gndPin = 2;
  
void setup() {
 
 lcd.begin(20,4); 
  
}

void loop() {
  
   lcd.print(thermocouple.readFahrenheit());
   delay(1000);
   lcd.clear(); 
}
