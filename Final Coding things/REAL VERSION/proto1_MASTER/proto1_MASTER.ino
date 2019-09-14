#include <EEPROM.h> //library to write to memory
#include <LiquidCrystal.h>
#include "max6675.h"
#include <Rotary.h> //my own library//
#include <Wire.h> //i2c interface

#define AVERAGE 25
#define INIT 0
#define AVERAGEDETECT 50
#define DO 8
#define CS 9
#define CLK 10
#define ALG 1

//#define EMULATE 
#ifdef EMULATE
  #define INPUTMET  Serial.read()
  #define RETURNVAL Serial.println(tempcurr)
  #define SERIALBEG if(Serial.available() >0) {
  #define SERIALEND }
  #define PAUSE 50
#else
  #define INPUTMET thermocouple.readFahrenheit() 
  #define RETURNVAL  //ignore this
  #define PAUSE 1000
  #define SERIALBEG 
  #define SERIALEND
#endif
int stat = 0; //decides whether alg is in "rise" or "cruise" stage
int runningx = 1; //decides if pot has boiled dry

int ed; //end row for lcd arrow
int currmillis[3];
int count = 0; //how many numbers into the average
float total = 0; //raw added sums
float averageprev = 0; //last average
float averagecurr = 0; //current average
float averagediff = 0; //differences between the two
volatile int m = 1; //carrier var
int start;//begin row for lcd arrow; //current millis, used to substitute for delay()
int prevmillis[3]; //previous millis, used to substitute for delay()
int first = 1; 
volatile int dir = 0; 
float tempprev = 0; 
float tempcurr = 0; 
int pos = 0; 
int setting = 0; 
bool push = false; 
bool idle = true; 
int smartmode = 0; 
int textmode = 1; 
int textbogus = 10; //10 if textmode, 0 if not text mode// 
int statusbar = 0; 
byte arrow[8] = 
{
  0b10000,
  0b11000,
  0b11100,
  0b11110,
  0b11110,
  0b11100,
  0b11000,
  0b10000
}; // arrow byte map

void detect(); //function declarations
int cursormenu(int start, int ed, int dir);
int algorithm(); 
int detectstart(); 
void displaymenu(int statusmain, int statussmart, int statustext, int statusbar);
void displayshell(); 
 
LiquidCrystal lcd(12, 11, 7, 6, 5, 4); //making objects
MAX6675 thermocouple(CLK, CS, DO);
Rotary rotary; 


void setup() {
    
     Wire.begin(); 
     Serial.begin(9600); 
     lcd.begin(20, 4);
     rotary.begin(3,2,13);
     attachInterrupt(rotary.getPin(), detect, FALLING); //constantly monitors the rotary pin
     lcd.setCursor(0,0);
     lcd.print("Please wait while");
     lcd.setCursor(0,1);
     lcd.print("we start the module");
     delay(12000);
     lcd.clear(); 
     
     
     prevmillis[ALG] = millis(); 
     start = 1;
     ed = 3;
     lcd.createChar(0, arrow);
    delay(100);
    lcd.clear(); 
    lcd.setCursor(0,start); 
    lcd.write((uint8_t)0); 

     if(EEPROM.read(INIT) == 0)
     {
      char carrier; 
      char c; 
      int i = 0; 
      lcd.setCursor(0,0);
      lcd.print("Welcome to");
      lcd.setCursor(0,1);
      lcd.print("Intellistove");
      delay(2000); 
      lcd.clear(); 
      lcd.setCursor(0,0);
      lcd.print("Text the code to the");
      lcd.setCursor(0,1);
      lcd.print("number below");
      delay(1000); 
      Wire.beginTransmission(1); 
      Wire.write(101); 
      Wire.endTransmission(); 
      delay(100); 
      Wire.requestFrom(1, 11); 
      while(Wire.available())
      {
       c = Wire.read(); 
       lcd.setCursor(i,2); 
       lcd.print(c);
       i++; 
      }
      i = 0; 
      lcd.setCursor(0,3);
      lcd.print("MAXDU17");  
      while(carrier != 'G')
      {
        delay(1000); 
        Wire.beginTransmission(1); 
        Wire.write(202); 
        Wire.endTransmission(); 
        delay(100); 
        Wire.requestFrom(1,1); 
         while(Wire.available())
        {
         carrier = Wire.read(); 
        }
      }
      lcd.clear();
      lcd.print("recieved code, and");
      lcd.setCursor(0,1);
      lcd.print("your number is"); 
      lcd.setCursor(0,2);
      lcd.print("written to memory"); 
      delay(5000);
      lcd.clear(); 
      lcd.print("now text a pairing");
      lcd.setCursor(0,1);
      lcd.print("phrase less than 10"); 
      lcd.setCursor(0,2);
      lcd.print("characters to number"); 
      Wire.beginTransmission(1); 
      Wire.write(101); 
      Wire.endTransmission(); 
      delay(100); 
      Wire.requestFrom(1, 11); 
      while(Wire.available())
      {
       c = Wire.read(); 
       lcd.setCursor(i,3); 
       lcd.print(c);
       i++; 
      }
      while(carrier != 'M')
      {
        delay(2500); 
        Wire.beginTransmission(1); 
        Wire.write(213); 
        Wire.endTransmission(); 
        delay(100); 
        Wire.requestFrom(1,1); 
         while(Wire.available())
        {
         carrier = Wire.read(); 
        }
      }
      lcd.clear();
      lcd.print("success!"); 
      lcd.setCursor(0,1);
      lcd.print("your stove is ready");
      lcd.setCursor(0,2);
      lcd.print("for use"); 
      EEPROM.write(INIT,1); 
      delay(5000); 
      lcd.clear(); 
     }
     
  delay(1000); 
  Wire.beginTransmission(1); 
  Wire.write(2); //turns on stove
  Wire.endTransmission();

}

void loop() {

   Wire.requestFrom(1, 1);  
   dir = rotary.getDirection();
   pos = cursormenu(start,ed,dir);
   push = rotary.isPushed(); 
   Serial.print(pos); 
   displayshell();  
  if (Wire.available()) 
  { 
      char c = Wire.read();
      switch (c) 
      {
        case 'T':
            lcd.clear();
            
            while(dir ==0)
            {
               dir = rotary.getDirection();  
               displaymenu(0,0,1,1);
            }
           lcd.clear(); 
          break;
        case 'V':
          lcd.clear();
          displaymenu(0,0,1,2); 
          delay(2500);
          lcd.clear();  
          break; 
        default: 
        break;
    }
  }
  if(smartmode == 1)
  {
      if(stat == 0 && runningx ==1 )
      {
        stat = detectstart(); 
      }
    
     if(stat == 1 && runningx == 1)
     {
      runningx = algorithm(); 
     } 
  }

  else if (smartmode == 0)
  {


      if(currmillis[2] - prevmillis[2] >= PAUSE)
         {
                prevmillis[2] = millis(); 
                currmillis[2] = millis(); 
                tempcurr = INPUTMET; 
         }
      else
      {
        currmillis[2] = millis(); 
      }
  
  }
  
}

int algorithm()
{ 

 if(count < AVERAGE)
       {
        if(currmillis[ALG] - prevmillis[ALG] >= PAUSE)
         {
          // if(Serial.available() > 0)
           // {
                prevmillis[ALG] = millis(); 
                currmillis[ALG] = millis(); 
                tempcurr = INPUTMET;
                //tempcurr = tempcurr *3; 
                
                if(tempcurr - tempprev > 15 || tempcurr - tempprev < -15)
                 { 
                   
                    RETURNVAL;
                 }
                 else
                 {
                   total = total + tempcurr;
                   tempprev = tempcurr; 
                   count++; 
                   RETURNVAL;
                 }
         
            //}
         }  
         else
         {
          currmillis[ALG] = millis(); 
         }
     
       }     
       
  
   else
   {     
   
        averagecurr = total/AVERAGE;
        averagediff = averagecurr - averageprev;
          if(averagediff >6.5)
          {
              smartmode = 2; 
              statusbar = 3;
               
              Wire.beginTransmission(1); 
              Wire.write(1); 
              Wire.endTransmission(); 
              delay(100); 
              Wire.beginTransmission(1); 
              Wire.write(textbogus); 
              Wire.endTransmission(); 
              delay(3000); 
               lcd.clear();
              return 0; 
          } 
        count = 0; 
        total = 0; 
        averageprev = averagecurr;  
        averagecurr = 0; 
        
   }
  
return 1; 

 
  
}
     
  
int detectstart()
{ 
    if(count <= AVERAGEDETECT)
       {
        if(currmillis[ALG] - prevmillis[ALG] >= PAUSE)
         {
          // if(Serial.available() > 0)
            //{
              
               prevmillis[ALG] = millis(); 
               currmillis[ALG] = millis(); 
               tempcurr = INPUTMET; 
               //tempcurr = tempcurr * 3; 
               if(tempcurr > 150)
               {
                idle = false; 
               }
               else
               {
                idle = true; 
               }
              if(tempcurr - tempprev > 15 || tempcurr - tempprev < -15)
               {
                  if(first == 0)
                   {
                      //lcd.print("X"); 
                      RETURNVAL; 
                   }
                   else
                   {
                      total = total + tempcurr;
                      count++; 
                      tempprev = tempcurr; 
                      RETURNVAL;
                   }
               }
               else
               {
               total = total + tempcurr;
               tempprev = tempcurr; 
               count++; 
               RETURNVAL;
                }
            //}
         }
         else
         {
          currmillis[ALG] = millis(); 
         }
         
       }
  
   else
   {     
   
        averagecurr = (total/AVERAGEDETECT);
        averagediff = (averagecurr) - (averageprev);

          if(averagediff <1 && averagecurr > 350)
          {
              //lcd.print("now we start"); 
              averageprev = averagecurr;  
              averagecurr = 0;
              count = 0; 
              total = 0;
              return 1; 
          } 
        count = 0; 
        total = 0; 
        averageprev = averagecurr;  
        averagecurr = 0;
        first = 0; 
        
   }
  
return 0; 

 
  
}
int cursormenu(int start, int ed, int xdir)
{
  if (xdir == 1 && m < 4)
  { 
    if(m == ed)
    {
      //lcd.clear();
       lcd.setCursor(0,ed);
       lcd.write((uint8_t)0);
       return ed; 
    }
    else
    {
    lcd.clear(); 
    m++;
    lcd.setCursor(0,m);
    lcd.write((uint8_t)0);
    return m;
    }
  }
  else if (xdir == 2 && m >= 0)
  {
    if(m == start)
    {
     
     lcd.setCursor(0,start);
     lcd.write((uint8_t)0);
     return start; 
    }
    else
    {
    lcd.clear();
    m--;
    lcd.setCursor(0,m);
    lcd.write((uint8_t)0);
    return m; 
    }
  }
  else
  {
    return m;
  }
    
  
}

void detect()
{
rotary.detect();
}

void displaymenu(int statusmain, int statussmart, int statustext, int statusbar)
{
 const char* bar[] = {"", "Text Message Shutoff", "Text Message Turn On", "STOVE BOILED DRY"}; 
    if(statusbar == 0)
    {
      lcd.setCursor(2,0);
      lcd.print(int(tempcurr));
      if(idle == true)
      {
        lcd.setCursor(9,0);
        lcd.print("Idle"); 
      }
      else if(stat == 0 && idle == false)
      {
        lcd.setCursor(9,0);
        lcd.print("Heat"); 
      }
      else if (stat == 1)
      {
        lcd.setCursor(9,0);
        lcd.print("Armed"); 
      }
    }
   else if(statusbar < 4)
   {
    lcd.setCursor(0,0);
    lcd.print(bar[statusbar]); 
   }
      
      
  
  if(statusmain ==0 )
  {
    switch (statussmart)
    {
      case 0:
        lcd.setCursor(1,1);
        lcd.print("Start Smart Mode");
        break;
      case 1:
        lcd.setCursor(1,1);
        lcd.print("Stop Smart Mode");
        break; 
      case 2: 
        lcd.setCursor(1,1);
        lcd.print("Reset");
        break; 
    }
      switch (statustext)
    {
      case 0:
        lcd.setCursor(1,2);
        lcd.print("Turn On Text Mode");
        break;
      case 1:
        lcd.setCursor(1,2);
        lcd.print("Turn Off Text Mode");
        break; 
    }
    lcd.setCursor(1,3);
    lcd.print("Settings"); 
  }
  else
  {
    lcd.setCursor(1,1);
    lcd.print("Sleep. (Turn wake)"); 
    lcd.setCursor(1,2);
    lcd.print("Factory Reset"); 
    lcd.setCursor(1,3);
    lcd.print("Return to Main Menu");
  }
     
}

void displayshell()
{
   if(setting == 0)
   {
    //pos = cursormenu(start,ed,dir);
    displaymenu(0,smartmode,textmode,statusbar);
    if(pos == 3 && push == 1)
    {
      lcd.clear(); 
      setting = 1; 
      m = 1;  
    }
     if(pos == 2 && push == 1)
    {
       switch(textmode)
       {
        case 0:
          textmode = 1;
          lcd.clear(); 
          pos = cursormenu(start,ed,dir);
          textbogus = 10; 
           Wire.beginTransmission(1); 
           Wire.write(55); 
           Wire.endTransmission(); 
          break;
        case 1:
          textmode = 0;
          lcd.clear(); 
          pos = cursormenu(start,ed,dir);
          textbogus = 0; 
           Wire.beginTransmission(1); 
           Wire.write(52); 
           Wire.endTransmission(); 
            Wire.beginTransmission(1); 
                Wire.write(2); //turns on stove
                Wire.endTransmission();
          break; 
       }

    }
     if(pos == 1 && push == 1)
    {
      switch(smartmode)
      {
        case 0:
          smartmode = 1;
          lcd.clear(); 
          break;
        case 1:
          smartmode = 0;
                 Wire.beginTransmission(1); 
                Wire.write(2); //turns on stove
                Wire.endTransmission();
          lcd.clear(); 
          break;
         case 2:
         
          smartmode = 0;
          stat = 0;
          runningx = 1; 
          count = 0; 
          total = 0; 
          statusbar = 0; 
          lcd.clear(); 
          break; 
      }
    }
    
   }
   else if (setting == 1)
   {
      
      displaymenu(1,0,0,0);
       if(pos == 3 && push == 1)
      {
        lcd.clear(); 
        setting = 0; 
        m = 1;
      }
      if(pos == 1 && push == 1)
      {
        lcd.clear(); 
        lcd.noDisplay(); 
        while(dir == 0)
        {
          dir = rotary.getDirection(); 
        }
        lcd.display(); 
        m = 1; 
     }
      if(pos == 2 && push == 1)
      {
        EEPROM.write(0,0); 
        Wire.beginTransmission(1); 
        Wire.write(66); 
        Wire.endTransmission(); 
        lcd.clear();
        lcd.print("Please Wait"); 
        delay(5000); 
        asm ( " jmp 0"); 
     }
   }
   
}

