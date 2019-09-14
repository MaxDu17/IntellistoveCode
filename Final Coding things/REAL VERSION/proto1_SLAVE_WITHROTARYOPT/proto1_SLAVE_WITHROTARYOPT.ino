/***********************************************
 * Maximilian Du
 * Slave code for prototype version three PCB
 * Started 1-21-17
 * Finished 3-20-17
 */
#include <EEPROM.h> //library to write to memory
#include <Wire.h> //I2C library
#include <SoftwareSerial.h> //Serial communications for GSM library

#define PREFACE "UNREAD\",\"+" //just a constant used for finding a start point for GSM
#define txPin 9 //all pin declarations for the GSM
#define rxPin 10    
#define trgPin 11

SoftwareSerial mySerial = SoftwareSerial(rxPin, txPin); //initializing communication with GSM

int recieveVal = 0; //This is the holder variable for messages sent by master
char stoveStatus = 'X'; //this is the holder variable for messages sent out
char textraw[500]; //raw read in text message char array, contains a bunch of unwanted characters
char index[10]; //text message reading index
char number[20]; //number that sent text message
char ownnumber[12]; //own phone number
int counter = 0; //index value for textraw
char textmessage[50]; //refined pure text message
char cofnum[12]; //phone number written to EEPROM previously
char password[12]; //password written to EEPROM previously
bool textmode = true; //text mode status
bool waitforpwd = false; //status declaration if foreign number
bool specialpermission = false; //special permission for foreign phone
int initx = 0; //status of initialization; prepare for magic word
int xyz = 0; //another counter varible
void readIndex(); //reads where the text message is to be found
void printArr(); //filters unwanted junk from textraw
void ownphonenumber(); //finds own phone number
void sendreadcommand(); //sends command to read text message
void tolowercase(char* p); //turns an array of characters into lower case
void cleanbuffer(char* p); //cleans array of characters by assigning them all to zero

void setup() 
{
  int xyx = 0; //temp carrier var
  Serial.begin(9600); 
  pinMode(13, OUTPUT); 
  pinMode(trgPin, OUTPUT);
  mySerial.begin(19200);
  Wire.begin(1); 
  mySerial.listen(); 
  digitalWrite(trgPin,LOW);//turns on the GSM module
  delay(1000);
  digitalWrite(trgPin,HIGH);
  delay(2000);
  digitalWrite(trgPin,LOW);
  delay(9000); 
  ownphonenumber();
  Wire.onReceive(readFunc); //interrupt handlers
  Wire.onRequest(sendStatus);
  for(int i =0; i < 11; i ++)//reading from memory, storing them into arrays
  {
    cofnum[i] = EEPROM.read(i); 
  }
  for(int i =11; i < 21; i ++)
  {
    password[xyx] = EEPROM.read(i);
    xyx ++;  
  }
  cofnum[11] = 0; 
  password[xyx] = 0; 
  Serial.print(password); 
}
 
void sendStatus() //Tells master what's going on
{
  if(recieveVal == 101)//This means that its asking for own number
  {
    Wire.write(ownnumber); 
    recieveVal=0; 
  }
  else if(recieveVal == 213)//this means master is asking if password has been sent
  {
      initx = 1;
      if(strlen(textmessage) > 0)//basically if there is something in textmessage at this point, treat it like a pwd
      {
          Wire.write('M');//state its complete 
            for(int i = 11; i<=(strlen(textmessage) +11); i++)
          {
            EEPROM.write(i,textmessage[i-11]);//store this password
          }
          cleanbuffer(textmessage); //making sure that this doesn't happen on endless loop
          cleanbuffer(number); 
          recieveVal=0; 
          initx = 0;
          for(int i =11; i < 21; i ++)
           {
              password[xyz] = EEPROM.read(i);
              xyz ++;  
           }
     
      }      
    else
    {
      Wire.write('X'); //standard nothing has happened
    }
  }
  else if (recieveVal == 202)//means wait for pairing code
  {
    if( strstr(textmessage, "MAXDU17"))
    {
      for(int i = 0; i<11; i++)
      {
        EEPROM.write(i,number[i]);
      }
      Wire.write("G");//this means code has been sent
      cleanbuffer(textmessage);//infinite loop checks
      cleanbuffer(number); 
      for(int i =0; i < 11; i ++)//reading from memory, storing them into arrays
      {
        cofnum[i] = EEPROM.read(i); 
      }
    }
    else
    {
      Wire.write("X"); //standard
    }
  }
  else
  {
   Wire.write(stoveStatus);//This is what normally happens
   stoveStatus = 'X'; 
  } 
}
void readFunc(int asdf)//This just reads a byte from the master
{ 
  recieveVal =  Wire.read();
}

void loop() 
{
  char c; 
  while(mySerial.available() > 0)//if GSM is trying to say something, record it
  {
        c = mySerial.read();
        textraw[counter] = c;
        counter++;
        if(textraw[counter-1] == '\r' && textraw[counter-2] == 'K' && textraw[counter - 3] == 'O' && textraw[counter -4] == '\n')//checking for specific end values
       {
        textraw[counter] = 0; 
        printArr();
        getnumber(); 
        Serial.print(textmessage); 
        break; 
       }
      else if(textraw[counter-1] == ',' && textraw[counter-2] == '"' && textraw[counter -3] == 'M')//checking for specific end values 
       {
        readIndex(); 
        sendreadcommand(); 
        break; 
       }
  }  

  if(textmode == false)//no text control when text mode is off, so just throw away any data coming in from GSM
  {
    cleanbuffer(textmessage);
    cleanbuffer(number); 
  }
  if((strcmp(number, cofnum) == 0 && textmode && initx == 0 ) || (specialpermission))//Means that if phone number is correct, or authorization has been given, listen for command
  { 
    char commandx[30]; 
    tolowercase(textmessage);
    if(strstr(textmessage, "turn on stove"))//Just sending the command to GSM
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
  else if(strcmp(number, cofnum) != 0 && textmode && strlen(number) > 2 && waitforpwd != true && initx ==0)//This is when a foreign phone needs control
  {
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
  if(waitforpwd == true)//once the password inquiry has been sent, this will be true
  {
    if(strcmp(number, cofnum) != 0 && textmode && strlen(number) > 2)//if another message has come in, and it is from the foreign phone
    {
      if(strcmp(textmessage, password) == 0)//if passwords match
      {
        char commandz[30]; 
        waitforpwd = false;
        mySerial.print("AT+CMGF=1\r"); 
        delay(1000);  
        sprintf(commandz,"AT+CMGS=\"%s\"\r", number);  
        mySerial.print(commandz);
        delay(1000);  
        mySerial.println("Accepted. Thank you. Now, your command. Please enter it here:\x1A"); 
        specialpermission = true; //grant authorization for one action
        cleanbuffer(textmessage);
        cleanbuffer(number); 
      }
      else if(strlen(textmessage) > 2 && strcmp(textmessage, password) != 0)//if passwords don't match
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
  
  if( recieveVal == 17)//Many conditionals that obey master
  {//toggles GSM module
    digitalWrite(trgPin,LOW);
    delay(1000);
    digitalWrite(trgPin,HIGH);
    delay(2000);
    digitalWrite(trgPin,LOW);
    delay(1000); 
    recieveVal = 0; 
  }
  else if( recieveVal == 2)//turns on stove
  {
    digitalWrite(13, HIGH);
    recieveVal = 0; 
  }
  else if (recieveVal == 1)//turns off stove
  {
    digitalWrite(13, LOW);
    recieveVal = 0; 
  }
  else if (recieveVal == 10)//send stove message
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
  else if (recieveVal == 66) // this restarts and wipes the EEPROM
  {
    for (int i = 0 ; i < EEPROM.length() ; i++)
    {
       EEPROM.write(i, 0);
    }
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
  else if (recieveVal == 52)//turn off text mode
  {
    textmode = false;
    recieveVal = 0; 
  }
  else if (recieveVal == 55)//turn on text mode
  {
    recieveVal = 0; 
    textmode = true;
  }
  else//don't do anything
  {
  }
  
}//end main bracket


void readIndex()//reads the index
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


}//end funct bracket

void printArr()//filters raw array
{
  int i = 0; 
  char* tempvar;//both are array pointers
  char* beginvar;  
  beginvar = strstr(textraw, "+CMGR:"); //sets one "stepladder" pointer to a line
  if(beginvar)
  {
    tempvar = strstr(beginvar, "\r"); //sets other "stepladder" pointer to a specific char
    tempvar = tempvar + 1; 
  while(tempvar[i] != '\r')//now this pointer behaves like an array
  {
    textmessage[i] = tempvar[i+1]; 
    i++; 
  }
  textmessage[i] = 0;  
  }
  if(strstr(textraw, "+CMGR"))//flushes GSM memory
  {
    mySerial.print("AT+CMGD=1,1\r"); 
  }
  counter = 0; 
}//end funct bracket


void sendreadcommand()//sends command to read text message from GSM
{
  char command[20];
  sprintf(command,"AT+CMGR=%s\r", index);  
  mySerial.print(command);
}//end funct bracket


void getnumber()//retrieve phone number from previous text
{
  int i = 0; 
  char* beginvar;  
  beginvar = strstr(textraw, PREFACE);//preface defined at top of code
  if(beginvar)
  {
    while(beginvar[i+10] != '"')//another use of an array pointer
    {
     number[i] = beginvar[i+10];
     i++; 
    }
  }
  if(strstr(textraw, "+CMGR"))//flushes GSM memory
  {
    mySerial.print("AT+CMGD=1,1\r"); 
  }
}//end funct bracket

void ownphonenumber()//get own phone number
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
      if(numraw[counter-1] == '\r' && numraw[counter-2] == 'K' && numraw[counter - 3] == 'O' && numraw[counter -4] == '\n')//looks for end values
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
}// end funct bracket


void tolowercase(char* p)//Turns an array of characters lowercase
{
  for(int i = 0; i <strlen(p); i++)
  {
    p[i] = tolower(p[i]); 
  }
}//end funct bracket


void cleanbuffer(char* p)//clears an array
{
for(int i = 0; i< strlen(p); i++)
  {
    p[i] = 0; 
  }
}//end funct bracket

  



  


