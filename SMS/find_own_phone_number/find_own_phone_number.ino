#include <SoftwareSerial.h> 
 
#define txPin 9
#define rxPin 10    
SoftwareSerial mySerial = SoftwareSerial(rxPin, txPin); 
void ownphonenumber();
void setup() 
{
 mySerial.begin(19200); 
Serial.begin(9600); 
delay(1000);
mySerial.listen(); 
 ownphonenumber(); 
}

void loop() {


}
void ownphonenumber()
{
  char ownnumber[15]; 
  char numraw[100]; 
  char c; 
  int counter = 0; 
  int i = 0; 
  char* beginvar;   
  
  mySerial.print("AT+CNUM\r"); 
  while(true)
  {
    if(mySerial.available() > 0) 
    {
      c = mySerial.read();
      numraw[counter] = c; 
      counter++; 
      if(numraw[counter-1] == '\r' && numraw[counter-2] == 'K' && numraw[counter - 3] == 'O' && numraw[counter -4] == '\n')
       {
          numraw[counter] = 0; 
          counter = 0; 
        //  Serial.print(numraw);
          
         beginvar = strstr(numraw, "\",\""); 
          while(beginvar[i+3] != '"')
          {
            ownnumber[i] = beginvar[i+3]; 
            i++; 
          }
          ownnumber[i] = 0; 
          Serial.print(i);  
          Serial.write(ownnumber); 
          break; 
          }
         
       }
     
    }  
}


