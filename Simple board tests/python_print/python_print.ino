#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 7, 6, 5, 4);
int incomingByte; 
void setup() {
  Serial.begin(9600); 
  lcd.begin(20,4); 
  pinMode(13, OUTPUT); 

}

void loop() {
  
        if (Serial.available() > 0) {
         
         lcd.print(Serial.read());
         
         delay(5);
         lcd.clear(); 
         Serial.println(50); 
                /*
                incomingByte = Serial.read();
                incomingByte = incomingByte * 2; 
                lcd.print(incomingByte); 
               // delay(50); 
                lcd.clear();
                Serial.print(5); */
        }
}
