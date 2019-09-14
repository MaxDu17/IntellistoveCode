#include "max6675.h"
#include <LiquidCrystal.h>
#include <Rotary.h>
#include <Wire.h>

int thermoDO = 8;
int thermoCS = 9;
int thermoCLK = 10;
int RS = 12;
int E = 11;
int D4 = 7;
int D5 = 6;
int D6 = 5;
int D7 = 4;
float temp = 0; 

LiquidCrystal lcd(RS, E, D4, D5, D6, D7);
MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);
Rotary rotary;


void detect()
{
rotary.detect(); 
}

void setup() 

  lcd.begin(20, 4);
  rotary.begin(3,2,13);
  Wire.begin(); 
  attachInterrupt(rotary.getPin(), detect, FALLING);

}
  

void loop() 
{
  // put your main code here, to run repeatedly:

}
