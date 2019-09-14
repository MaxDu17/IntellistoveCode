#include <SoftwareSerial.h>   
#define rxPin 7  
#define txPin 8  
SoftwareSerial mySerial = SoftwareSerial(rxPin, txPin);  
void setup() {
  Serial.begin(19200);
  mySerial.begin(19200); 

}

void loop() {
  mySerial.print("AT");
 Serial.print(mySerial.read); 
  
}
