#include <EEPROM.h>

void setup()
{
  Serial.begin(9600);
  
  for (int i = 0 ; i < 20 ; i++) {
    Serial.print(char(EEPROM.read(i)));
    //Serial.print(i);
    Serial.print("\n"); 
  }
}
void loop() {
  

}
