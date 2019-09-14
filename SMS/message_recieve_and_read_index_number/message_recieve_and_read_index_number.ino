 #include <SoftwareSerial.h>   
#define txPin 9
#define rxPin 10    
#define trgPin 11 
#define PRINT 1
#define NOPRINT 0
#define INCOMINGTEXT 2
char text[500]; 
char c; 
int stat = NOPRINT; 
int counter = 0; 
//char index[10]; 
int indexcount = 0; 
int i = 0; //turn on when you want to send something//
bool begins = true;
SoftwareSerial mySerial = SoftwareSerial(rxPin, txPin);  
void readIndex(); 
void printArray(); 
void setup() 
{
mySerial.begin(19200); 
Serial.begin(9600); 
   /* pinMode(trgPin, OUTPUT);
    digitalWrite(trgPin,LOW);
    delay(1000);
    digitalWrite(trgPin,HIGH);
    delay(2000);
    digitalWrite(trgPin,LOW);
    delay(3000);

delay(7000); */
delay(3000);
Serial.print("text1");
mySerial.print("AT+CMGR=1\r");
mySerial.listen(); 
}

void loop() 
{
   
  while(mySerial.available() > 0)
  {
        c = mySerial.read();
        text[counter] = c;
        counter++;
        if(text[counter-1] == '\r' && text[counter-2] == 'K' && text[counter - 3] == 'O' && text[counter -4] == '\n')
       {
        printArr();
        break; 
       }
      else if(text[counter-1] == ',' && text[counter-2] == '"' && text[counter -3] == 'M')
       {
        readIndex(); 
        break; 
       }
  }  

}

void readIndex()
{
    int i; 
    int indexcount = 0; 
    char index[10];
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
    
   
     Serial.print("text index:"); 
     for( i = 0; i< indexcount; i++)
     {
         Serial.write(index[i]);   
     }
      
       Serial.write("\n"); 

}

void printArr()
{
   int i; 
   for( i = 0; i< counter; i++)
   {
    Serial.write(text[i]); 
   }
   Serial.write("\n"); 
}
  



