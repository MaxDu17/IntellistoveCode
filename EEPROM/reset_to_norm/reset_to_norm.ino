#include <EEPROM.h>

void setup()
{
  Serial.begin(9600);
  
  EEPROM.write(0,1); 
  
}
void loop() {
  

}
