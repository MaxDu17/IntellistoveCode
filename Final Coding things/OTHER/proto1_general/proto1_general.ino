#include <LiquidCrystal.h>
#include "max6675.h"
#define AVERAGE 50
#define PAUSE 30 // actually, its 1000, but for the simulation, I will make it 5//
#define AVERAGEDET 50

int thermoDO = 10;
int thermoCS = 11;
int thermoCLK = 12;
int stat = 0; 
int runningx = 1;
int incomingByte;
int count = 0;
float total = 0; 
float averageprev = 1000;
float averagecurr = 0; 
float averagediff = 0;  

int algorithm(); 
int detectsize(); 
LiquidCrystal lcd(12, 11, 7, 6, 5, 4);
MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);


void setup() {
  pinMode(13, OUTPUT); 
  digitalWrite(13, HIGH); 
  Serial.begin(9600);
  lcd.begin(20,4); 
}

void loop() {
  if(stat == 0 && runningx ==1 )
  {
    //Serial.print("opened"); 
  stat = detectsize(); 
  //Serial.print("after"); 
  }

 
 if(stat == 1 && runningx == 1)
 {
  //Serial.print("alg"); 
  runningx = algorithm(); 
 }
 
  
}


int algorithm()
{
  if(Serial.available() != 0)
  {
    total = 0; 
    count = 0;
    while(count < AVERAGE)
        {
         count++; 
         total = total + (Serial.read() * 2); 
         delay(PAUSE); 
         Serial.println(1); 
        }
       
       
        
        averagecurr = (total/AVERAGE);
        averagediff = averagecurr - averageprev;
        //lcd.print(averageprev); 
        lcd.print(averagediff);
        delay(500);
        lcd.clear();  
        //Serial.print("\n");  
          if(averagediff > 7.5)
          {
           //digitalWrite(13, LOW); 
            lcd.print(averagediff);
            lcd.print("pot boiled dry");
            //Serial.print("\n");   
            return 0; 
            
          }
        
        averageprev = averagecurr;  
        averagecurr = 0;
         Serial.println(1); 
         return 1; 
  }
        
  
        
}
     
  
int detectsize()
{ 
  if(Serial.available() != 0)
  {   
  
    while(count < AVERAGEDET)
       {
         count++; 
         total = total + (Serial.read() * 2); 
         delay(PAUSE); 
         Serial.println(1); 
       }
  
        
        averagecurr = (total/AVERAGEDET);
       averagediff = (averagecurr) - (averageprev);
       lcd.print(averagediff);
        delay(500);
        lcd.clear();  
      // Serial.print(averagediff);
      // Serial.print("\n");  
      
          if(averagediff <0.2 && averagecurr > 350)
          {
             lcd.print("now we start"); 
             total = 0; 
             count = 0;
              averageprev = averagecurr;  
              averagecurr = 0;
              Serial.println(1); 
              return 1; 
          } 
        
        averageprev = averagecurr;  
        averagecurr = 0;
        total = 0; 
         count = 0;
       return 0; 

  }
        
 
  
}

