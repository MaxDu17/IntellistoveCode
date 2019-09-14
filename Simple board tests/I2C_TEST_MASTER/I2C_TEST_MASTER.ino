#include <Wire.h>
int state = 0; 
void setup() {
  Wire.begin(); 
}

void loop() {
  Wire.beginTransmission(1); 
  Wire.write(state);
  Wire.endTransmission(); 
  if(state == 0)
  {
    state = 1;
  }
  else
  {
    state = 0; 
  }
  delay(1000); 
}
