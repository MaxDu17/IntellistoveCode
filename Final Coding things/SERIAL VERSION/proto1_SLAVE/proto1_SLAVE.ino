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
char ownnumber[12]; 
int counter = 0;
char textmessage[50]; 
char cofnum[12];  
char password[12]; 
void readIndex(); 
void printArr(); 
void ownphonenumber(); 
void sendreadcommand();
void deletesub();
void tolowercase(char* p); 
void cleanbuffer(char* p);
bool textmode = true; 
bool waitforpwd = false; 
bool specialpermission = false; 
int initx = 0; 
void setup() 
{
  int xyx = 0; 
 Serial.begin(9600); 
 //Serial.print("123456"); 
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
  //Serial.print(ownnumber); 
  Wire.onReceive(readFunc); 
  Wire.onRequest(sendStatus);
  for(int i =0; i < 11; i ++)
  {
    cofnum[i] = EEPROM.read(i); 
  }
  for(int i =11; i < 21; i ++)
  {
    password[xyx] = EEPROM.read(i);
    xyx ++;  
  }
  cofnum[11] = 0; 
 //Serial.print(cofnum);
// Serial.print(password);  
}
 
void sendStatus() 
{
  if(recieveVal == 101)
  {
    Wire.write(ownnumber); 
    recieveVal=0; 
  }
  else if(recieveVal == 213)
  {
      initx = 1;
      if(strlen(textmessage) > 0)
      {
          Serial.print(textmessage);
          Wire.write('M'); 
            for(int i = 11; i<=(strlen(textmessage) +11); i++)
          {
            EEPROM.write(i,textmessage[i-11]);
          }
          cleanbuffer(textmessage); 
          cleanbuffer(number); 
          recieveVal=0; 
          initx = 0;
      }      
    else
    {
      Wire.write('X'); 
    }
  }
  else if (recieveVal == 202)
  {
    if( strstr(textmessage, "MAXDU17"))
    {
      for(int i = 0; i<11; i++)
      {
        EEPROM.write(i,number[i]);
      }
      Wire.write("G");
      cleanbuffer(textmessage);
      cleanbuffer(number); 
    }
    else
    {
      Wire.write("X"); 
    }
  }
  else
  {
   Wire.write(stoveStatus);
   stoveStatus = 'X'; 
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
          Serial.print(textmessage); 
          break; 
         }
        else if(textraw[counter-1] == ',' && textraw[counter-2] == '"' && textraw[counter -3] == 'M')
         {
          readIndex(); 
          sendreadcommand(); 
          break; 
         }
    }  

    if(textmode == false)
    {
      cleanbuffer(textmessage);
      cleanbuffer(number); 
    }
    if((strcmp(number, cofnum) == 0 && textmode) || (specialpermission))
    { 
      char commandx[30]; 
      tolowercase(textmessage);
      if(strstr(textmessage, "turn on stove"))
      {
        digitalWrite(13, HIGH);  
        mySerial.print("AT+CMGF=1\r"); 
        delay(1000);  
        sprintf(commandx,"AT+CMGS=\"%s\"\r", number);  
        mySerial.print(commandx);
        delay(1000);  
        mySerial.println("turned on.\x1A"); 
        stoveStatus = 'V'; 
        cleanbuffer(textmessage); 
        cleanbuffer(commandx); 
        specialpermission = false;
      }
      else if(strstr(textmessage, "turn off stove"))
      {
        digitalWrite(13, LOW);  
        mySerial.print("AT+CMGF=1\r"); 
        delay(1000);  
        sprintf(commandx,"AT+CMGS=\"%s\"\r", number);  
        mySerial.print(commandx);
        delay(1000);  
        mySerial.println("turned off.\x1A"); 
        stoveStatus = 'T';
        cleanbuffer(textmessage); 
        cleanbuffer(commandx);
        specialpermission = false;  
      }
      cleanbuffer(number); 
    }
    else if(strcmp(number, cofnum) != 0 && textmode && strlen(number) > 2 && waitforpwd != true && initx ==0)
    {
      Serial.print(number); 
      char commandz[30]; 
        mySerial.print("AT+CMGF=1\r"); 
        delay(1000);  
         sprintf(commandz,"AT+CMGS=\"%s\"\r", number);  
        mySerial.print(commandz);
        delay(1000);  
        mySerial.println("Password?\x1A"); 
        waitforpwd = true; 
        cleanbuffer(number); 
        
    }
    if(waitforpwd)
    {
      if(strcmp(number, cofnum) != 0 && textmode && strlen(number) > 2)
      {
        if(strcmp(textmessage, password) == 0)
        {
          char commandz[30]; 
           waitforpwd = false;
            mySerial.print("AT+CMGF=1\r"); 
            delay(1000);  
             sprintf(commandz,"AT+CMGS=\"%s\"\r", number);  
            mySerial.print(commandz);
            delay(1000);  
            mySerial.println("Accepted. Thank you. Now, your command. Please enter it here:\x1A"); 
            specialpermission = true; 
            cleanbuffer(textmessage);
            cleanbuffer(number); 
        }
        else if(strlen(textmessage) > 2 && strcmp(textmessage, password) != 0)
        {
          char commanda[30]; 
           mySerial.print("AT+CMGF=1\r"); 
            delay(1000);  
             sprintf(commanda,"AT+CMGS=\"%s\"\r", number);  
            mySerial.print(commanda);
            delay(1000);  
            mySerial.println("Denyed, please try again.\x1A"); 
          cleanbuffer(textmessage); 
        }
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
       char commandb[30]; 
       mySerial.print("AT+CMGF=1\r"); 
            delay(1000);  
             sprintf(commandb,"AT+CMGS=\"%s\"\r", cofnum);  
            mySerial.print(commandb);
            delay(1000);  
            mySerial.println("Your pot has boiled dry and I have turned it off for you.\x1A"); 
      recieveVal = 0; 
    }
    else if (recieveVal == 66)
    {
      for (int i = 0 ; i < EEPROM.length() ; i++)
      {
         EEPROM.write(i, 0);
         digitalWrite(trgPin,LOW);
         delay(1000);
         digitalWrite(trgPin,HIGH);
         delay(2000);
         digitalWrite(trgPin,LOW);
         delay(3000); 
         //recieveVal = 0;
         Serial.print("Restart");  
         asm ( " jmp 0"); 
         
         
      }
      
    }
    else if (recieveVal == 52)
    {
      textmode = false;
      recieveVal = 0; 
    }
    else if (recieveVal == 55)
    {
      recieveVal = 0; 
      textmode = true;
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
void tolowercase(char* p)
{
  for(int i = 0; i <strlen(p); i++)
  {
    p[i] = tolower(p[i]); 
  }
}
void cleanbuffer(char* p)
{
for(int i = 0; i< strlen(p); i++)
  {
    p[i] = 0; 
  }
}

  



  


