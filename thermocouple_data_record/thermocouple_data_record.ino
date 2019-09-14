#include "max6675.h"
#define DO 8
#define CS 9
#define CLK 10
MAX6675 thermocouple(CLK, CS, DO);
void setup() {
Serial.begin(9600); 

}

void loop() {
  Serial.print(thermocouple.readFahrenheit());
  Serial.print("\n"); 
  delay(1000); 

}
