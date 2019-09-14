 #include <SoftwareSerial.h>   
#define txPin 9
#define rxPin 10    
#define trgPin 11 
#define PRINT 1
#define NOPRINT 0
#define INCOMINGTEXT 2
SoftwareSerial mySerial = SoftwareSerial(rxPin, txPin); 
char textraw[500]; 
char index[10];
int counter = 0; 
void readIndex(); 
void printArr(); 
void sendreadcommand();
void deletesub();
void setup() 
{
mySerial.begin(19200); 
Serial.begin(9600); 
Serial.print("begin"); 

delay(3000);
mySerial.listen(); 
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
        break; 
       }
      else if(textraw[counter-1] == ',' && textraw[counter-2] == '"' && textraw[counter -3] == 'M')
       {
        readIndex(); 
        sendreadcommand(); 
        break; 
       }
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
  char textmessage[100];  
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
  Serial.print(textmessage); 
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

  



