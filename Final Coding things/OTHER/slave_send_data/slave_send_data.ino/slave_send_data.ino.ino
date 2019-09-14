#include <EEPROM.h> //library to write to memory
#include <Wire.h>
#include <SoftwareSerial.h>

#define PREFACE "UNREAD\",\"+"
#define txPin 9
#define rxPin 10    
#define trgPin 11

SoftwareSerial mySerial = SoftwareSerial(rxPin, txPin);

int recieveVal = 0; 
char stoveStatus = 'X';
char textraw[500]; 
char index[10];
char number[20]; 
char ownnumber[11]; 
int counter = 0;
char textmessage[100];   
void readIndex(); 
void printArr(); 
void ownphonenumber(); 
void sendreadcommand();
void deletesub();
void setup() 
{
 Serial.begin(9600); 
 Serial.print("123456"); 
  pinMode(13, OUTPUT); 
  pinMode(trgPin, OUTPUT);
  mySerial.begin(19200);
  Wire.begin(1); 
  mySerial.listen(); 
  digitalWrite(trgPin,LOW);
  delay(1000);
  digitalWrite(trgPin,HIGH);
  delay(2000);
  digitalWrite(trgPin,LOW);
  delay(9000); 
  ownphonenumber(); 
  
  Wire.onReceive(readFunc); 
  Wire.onRequest(sendStatus);
}
 
void sendStatus() 
{
  if(recieveVal == 101)
  {
    Serial.print(ownnumber);
    Wire.write(ownnumber); 
    recieveVal=0; 
  }
  else if (recieveVal == 202)
  {
    if( strstr(textmessage, "MAXDU17"))
    {
      Wire.write("G");
      for(int i = 1;  
    }
    else
    {
      Wire.write("X"); 
    }
  }
  else
  {
   Wire.write(stoveStatus);
  } 
}
void readFunc(int asdf)
{
  Serial.print("recieved"); 
  recieveVal =  Wire.read();
  Serial.print(recieveVal);
}

  void loop() 
  {
    
    char c; 
    while(mySerial.available() > 0)
    {
          c = mySerial.read();
          textraw[counter] = c;
          counter++;
          if(textraw[counter-1] == '\r' && textraw[counter-2] == 'K' && textraw[counter - 3] == 'O' && textraw[counter -4] == '\n')
         {
          textraw[counter] = 0; 
          printArr();
          getnumber(); 
          break; 
         }
        else if(textraw[counter-1] == ',' && textraw[counter-2] == '"' && textraw[counter -3] == 'M')
         {
          readIndex(); 
          sendreadcommand(); 
          break; 
         }
    }  

   
    if( recieveVal == 17)
    {
      digitalWrite(trgPin,LOW);
      delay(1000);
      digitalWrite(trgPin,HIGH);
      delay(2000);
      digitalWrite(trgPin,LOW);
      delay(1000); 
      recieveVal = 0; 
    }
    else if( recieveVal == 2)
    {
      digitalWrite(13, HIGH);
      recieveVal = 0; 
    }
    else if (recieveVal == 1)
    {
      digitalWrite(13, LOW);
      recieveVal = 0; 
    }
    else if (recieveVal == 10)
    {
      mySerial.print("AT+CMGF=1\r"); 
      delay(1000);  
      mySerial.print("AT+CMGS=\"13157511504\"\r");  
      delay(1000);  
      mySerial.println("Hello, your pot has boiled dry and I have turned it off for you.\x1A"); 
      recieveVal = 0; 
    }
    else
    {
    }

  

  }
  void readIndex()
{
    int indexcount = 0; 
    char x; 
    while(true)
    {
        x = mySerial.read(); 
        if (x != -1)
        {
          index[indexcount] = x; 
          indexcount ++; 
        }
        
        if(x == '\n') break;
    }
    index[indexcount] = 0; 


}

void printArr()
{

  int i = 0; 
  char* tempvar;
  char* beginvar;  
  beginvar = strstr(textraw, "+CMGR:"); 
  if(beginvar)
  {
    tempvar = strstr(beginvar, "\r"); 
    tempvar = tempvar + 1; 
  while(tempvar[i] != '\r')
  {
    textmessage[i] = tempvar[i+1]; 
    i++; 
  }
  textmessage[i] = 0;  
  }
  
  if(strstr(textraw, "+CMGR"))
  {
    mySerial.print("AT+CMGD=1,1\r"); 
  }
  
   
  counter = 0; 
  
}
void sendreadcommand()
{
  char command[20];

  sprintf(command,"AT+CMGR=%s\r", index);  
  mySerial.print(command);
  
}
void getnumber()
{
  int i = 0; 
  char* beginvar;  
  beginvar = strstr(textraw, PREFACE);
  if(beginvar)
  {
    while(beginvar[i+10] != '"')
    {
     number[i] = beginvar[i+10];
     i++; 
    }
  }
  
  if(strstr(textraw, "+CMGR"))
  {
    mySerial.print("AT+CMGD=1,1\r"); 
  }
}
void ownphonenumber()
{
  
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
          beginvar = strstr(numraw, "\",\""); 
          while(beginvar[i+3] != '"')
          {
            ownnumber[i] = beginvar[i+3]; 
            i++; 
          }
          ownnumber[i] = 0;  
          break; 
        }
         
       }
     
    }  
}


  



  


