#include <Wire.h> 
void setup() {
  Wire.begin();
  delay(1000); 
  Wire.beginTransmission(1);
  Wire.write(2);
  Wire.endTransmission();
  Wire.beginTransmission(1);
  Wire.write(10);
  Wire.endTransmission(); 
}

void loop() {
  // put your main code here, to run repeatedly:

}
