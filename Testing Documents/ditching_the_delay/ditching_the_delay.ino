unsigned long milliscurr;
unsigned long millisprev; 
int ledstate = LOW; 
void setup() {
  pinMode(13, OUTPUT); 
  milliscurr = millisprev = millis(); 

}

void loop() {
 if(milliscurr - millisprev >1000)
 {
  if(ledstate == LOW)
  {
    ledstate = HIGH;
    digitalWrite(13, ledstate); 
  }
  else
  {
    ledstate = LOW;
    digitalWrite(13, ledstate); 
  }
  millisprev = millis();
 }
 milliscurr = millis(); 

}
