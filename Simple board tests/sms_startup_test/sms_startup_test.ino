 #include <SoftwareSerial.h>   
#define txPin 9
#define rxPin 10    
#define trgPin 11
int i = 0; //turn on when you want to send something//
SoftwareSerial mySerial = SoftwareSerial(rxPin, txPin);  

void setup() {
mySerial.begin(19200); 

  /*  pinMode(trgPin, OUTPUT);
    digitalWrite(trgPin,LOW);
    delay(1000);
    digitalWrite(trgPin,HIGH);
    delay(2000);
    digitalWrite(trgPin,LOW);
    delay(3000);*/

//delay(7000); 
}

void loop() {
 while(i == 0)
 {
  sendtext();
  i = 1;    
 }
 
}
void sendtext()
{
 mySerial.print("AT+CMGF=1\r"); 
delay(1000);  
mySerial.print("AT+CMGS=\"13157511504\"\r");  
delay(1000);  
mySerial.println("hello there. is it finally working.\x1A");  
}

