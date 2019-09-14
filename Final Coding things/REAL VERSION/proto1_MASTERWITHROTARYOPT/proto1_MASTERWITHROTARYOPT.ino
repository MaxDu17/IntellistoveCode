/***********************************************
 * Maximilian Du
 * Master code for prototype version three PCB
 * Started 1-21-17
 * Finished 3-20-17
 */
#include <EEPROM.h> //library to write to memory
#include <LiquidCrystal.h> //liquid crystal library
#include <max6675.h> //Thermocouple library
#include <RotaryPoll.h> //my own library
#include <Wire.h> //i2c interface

#define AVERAGE 25 //average amount for the algorithm
#define INIT 0 //Adress for the initialized state for EEPROM
#define AVERAGEDETECT 50 //average amount for the detectstart
#define DO 8 //digital output for MAX6675
#define CS 9//chip select for MAX6675
#define CLK 10//clock for MAX6675
#define ALG 1//array value for millis

//#define EMULATE 
//COMMENTED OUT BECAUSE I AM NOT EMULATING
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
int currmillis[3]; //millis array to replace delay() function
int count = 0; //how many numbers into the average
float total = 0; //raw added sums
float averageprev = 0; //last average
float averagecurr = 0; //current average
float averagediff = 0; //differences between the two
volatile int m = 1; //carrier var
int start;//begin row for lcd arrow; //current millis, used to substitute for delay()
int prevmillis[3]; //previous millis, used to substitute for delay()
int first = 1; //variable to see if this was the first data point
volatile int dir = 0; //direction that rotary encoder was last turned 
float tempprev = 0; //last temperature point
float tempcurr = 0; //current temperature point
int pos = 0; //position of cursor on menu
int setting = 0; //whether or not menu is on settings or normal
bool push = false; //if button on rotary encoder is pushed
bool idle = true; //if temperature is <150, then idle is true
int smartmode = 0; //determines whether or not smart mode is activated
int textmode = 1; //determines whether or not text mode is activated
int textbogus = 10; //10 if textmode, 0 if not text mode// 
int statusbar = 0; //determines state of top menu bar
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


int cursormenu(int start, int ed, int dir); //displays the cursor icon
int algorithm(); //second part of algorithm
int detectstart(); //first part of algorithm
void displaymenu(int statusmain, int statussmart, int statustext, int statusbar); // this is what displays the menu
void displayshell(); //consolidates all menu logic
 
LiquidCrystal lcd(12, 11, 7, 6, 5, 4); //making objects
MAX6675 thermocouple(CLK, CS, DO);
RotaryPoll rotary; 


void setup() {
    
     Wire.begin(); 
     Serial.begin(9600); 
     lcd.begin(20, 4);
     rotary.begin(3,2,13);
     lcd.setCursor(0,0);
     lcd.print("Please wait while");
     lcd.setCursor(0,1);
     lcd.print("we start the module");//Waits for the slave GSM to connect
     delay(12000);
     lcd.clear(); 
     
     prevmillis[ALG] = millis(); 
     start = 1;
     ed = 3;
     lcd.createChar(0, arrow);
     delay(100);
     lcd.clear(); 
     lcd.setCursor(0,start); 
     lcd.write((uint8_t)0); //makes the cursor icon

     if(EEPROM.read(INIT) == 0)//if the pairing between the stove and the phone has not been done
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
      Wire.write(101); //requests for own phone number
      Wire.endTransmission(); 
      delay(100); 
      Wire.requestFrom(1, 11); //gets the 11 character phone number
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
      while(carrier != 'G')// 'G' means that the user has sent the code
      {
        delay(1000); 
        Wire.beginTransmission(1); 
        Wire.write(202); //Tells slave to prepare to recieve the code MAXDU17
        Wire.endTransmission(); 
        delay(100); 
        Wire.requestFrom(1,1); //requests status
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
      Wire.write(101); //Again, asks for own number
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
      while(carrier != 'M')// 'M' means that the password has been sent
      {
        delay(2500); 
        Wire.beginTransmission(1); 
        Wire.write(213); //Tells the slave to prepare to accept password
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
      EEPROM.write(INIT,1); //Writes status of the initialization to complete, as to not repeat it again
      delay(5000); 
      lcd.clear(); 
     }
     
  delay(1000); 
  Wire.beginTransmission(1); 
  Wire.write(2); //turns on stove
  Wire.endTransmission();

}

void loop() {
   Wire.requestFrom(1, 1);  //Always asks slave for a singlar byte status
   dir = rotary.getDirection(); //gets rotary direction
   pos = cursormenu(start,ed,dir); //gets position on screen
   Serial.print(dir); 
   push = rotary.isPushed(); //gets push status
   displayshell();  //Always executes the consolidated function to display menu
   if (Wire.available()) 
   { 
    char c = Wire.read();
    switch (c) 
      {
        case 'T':
          lcd.clear();
          while(dir ==0)
          {
            dir = rotary.getDirection();  //basically waits until user turns the rotary encoder
            displaymenu(0,0,1,1); //This means print "text message shutoff"
          }
           lcd.clear(); 
          break;
        case 'V':
          lcd.clear();
          displaymenu(0,0,1,2); //This means print text message turn on
          delay(2500);
          lcd.clear();  
          break; 
        default: 
        break;
    }
   }
  if(smartmode == 1)//If smart mode is running
  {
      if(stat == 0 && runningx ==1 )//If the flat part of the temp curve hasn't been reached
      {
        stat = detectstart(); 
      }
    
     if(stat == 1 && runningx == 1)//If the flat has been reached, and also the pot hasn't boiled dry
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
  
}//Main loop end bracket

int algorithm()
{ 

 if(count < AVERAGE)
       {
        if(currmillis[ALG] - prevmillis[ALG] >= PAUSE)
         {
          // if(Serial.available() > 0)
           // {
           //Commented out because of no emulation
                prevmillis[ALG] = millis(); 
                currmillis[ALG] = millis(); //This is to substitute for the delay function; it uses system up time to compare elapsed time
                tempcurr = INPUTMET;
                //tempcurr = tempcurr *3; 
                
                if(tempcurr - tempprev > 15 || tempcurr - tempprev < -15)
                 { 
                    RETURNVAL; //Eliminates extraneous point values
                 }
                 else
                 {
                   total = total + tempcurr;
                   tempprev = tempcurr; 
                   count++; 
                   RETURNVAL;//Either nothing or Serial.println(1), depending if emulate or not
                 }
            //}
         }  
         else
         {
          currmillis[ALG] = millis(); //If 1 second hasn't passed, then just go through and do nothing besides update current sys time
         }
       }     
   else //if the set amount of collected points have been reached
   {     
        averagecurr = total/AVERAGE;
        averagediff = averagecurr - averageprev;
        if(averagediff >6.5)
         {
          smartmode = 2; 
          statusbar = 3;
          Wire.beginTransmission(1); 
          Wire.write(1); //turns off stove
          Wire.endTransmission(); 
          delay(100); 
          Wire.beginTransmission(1); 
          Wire.write(textbogus); //either sends text message or does nothing, depending on textmode status
          Wire.endTransmission(); 
          delay(3000); 
          lcd.clear();
          return 0; 
         } 
        count = 0; //sets everything to what it should be
        total = 0; 
        averageprev = averagecurr;  
        averagecurr = 0;   
   }
return 1;//return 1 if pot hasn't boiled dry 
}//end of funct bracket
     
  
int detectstart()
{ 
    if(count <= AVERAGEDETECT)
       {
        if(currmillis[ALG] - prevmillis[ALG] >= PAUSE)
         {
          // if(Serial.available() > 0)
            //{
              //commented out because no emulation
               prevmillis[ALG] = millis(); 
               currmillis[ALG] = millis(); 
               tempcurr = INPUTMET; 
               //tempcurr = tempcurr * 3; 
               if(tempcurr > 150)
               {
                idle = false; //Checks temperature to see if stat is idle
               }
               else
               {
                idle = true; 
               }
              if(tempcurr - tempprev > 15 || tempcurr - tempprev < -15)
               {
                  if(first == 0)
                   {
                      RETURNVAL;// eliminates extraneous point values
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
               total = total + tempcurr;//adds current temperature to the total for this average set
               tempprev = tempcurr; 
               count++; 
               RETURNVAL;
                }
            //}
         }
         else
         {
          currmillis[ALG] = millis(); //if 1 second hasn't passed, then just update sys time
         }
         
       }
   else // if set amount of points are collected and summed
   {     
   
        averagecurr = (total/AVERAGEDETECT); //calculates average
        averagediff = (averagecurr) - (averageprev);//calculates differences between averages
          if(averagediff <1 && averagecurr > 350)
          {
            averageprev = averagecurr;  
            averagecurr = 0;
            count = 0; 
            total = 0;
            return 1; //returns 1 if pot has reached stable state
          } 
        count = 0; //sets everything back where it should be
        total = 0; 
        averageprev = averagecurr;  
        averagecurr = 0;
        first = 0; 
        
   }
  
return 0; //returns 0 if pot hasn't reached the stable state 
}//end of func bracket


int cursormenu(int start, int ed, int xdir) //prints out cursor
{
  if (xdir == 1 && m < 4)//basically, this algorithm is to keep the cursor in between certain bounds
  { 
      if(m == ed)
      {
        lcd.setCursor(0,ed);
        lcd.write((uint8_t)0);
        return m; 
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
        return m; 
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
    return m; //just returns the current position if there is no action
  } 
}//function end bracket

void displaymenu(int statusmain, int statussmart, int statustext, int statusbar)
{
 const char* bar[] = {"", "Text Message Shutoff", "Text Message Turn On", "STOVE BOILED DRY"}; // sets an array of strings, to avoid too many switch cases
    if(statusbar == 0)//just some menu cases
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
      
  if(statusmain ==0 )//more menu cases. No true algorithm, just a bunch of conditionals
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
    lcd.print("Sleep (Turn wake)"); 
    lcd.setCursor(1,2);
    lcd.print("Factory Reset"); 
    lcd.setCursor(1,3);
    lcd.print("Return to Main Menu");
  }
     
}

void displayshell()//This puts all display logic into a hollistic package
{
   if(setting == 0)//If the user is not in the settings menu
   {
    displaymenu(0,smartmode,textmode,statusbar);//this function displays the menu
    if(pos == 3 && push == 1)//logic goes as follows: if the button is pushed, then depending on what position the menu cursor is in, the action varies
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
           Wire.write(55); //write to slave to turn on text mode
           Wire.endTransmission(); 
           break;
        case 1:
          textmode = 0;
          lcd.clear(); 
          pos = cursormenu(start,ed,dir);
          textbogus = 0; 
          Wire.beginTransmission(1); 
          Wire.write(52); // write to slave turn off text mode
          Wire.endTransmission(); 
          Wire.beginTransmission(1); 
          Wire.write(2); //turns on stove
          Wire.endTransmission();
          break; 
       }
    }
     if(pos == 1 && push == 1)//pretty much same logic as before
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
   }//end of settings conditional
   else if (setting == 1)
   {
      displaymenu(1,0,0,0);//the 1 signifies that this is settings mode
       if(pos == 3 && push == 1)//same logic as before; see above
        {
          lcd.clear(); 
          setting = 0; 
          m = 1;
        }
      if(pos == 1 && push == 1)//this is to blank out the lcd, and rest all operations
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
        EEPROM.write(0,0); //this is the factory reset, resets initialization state to zero
        Wire.beginTransmission(1); 
        Wire.write(66); //this commands slave to wipe all EEPROM data and restart
        Wire.endTransmission(); 
        lcd.clear();
        lcd.print("Please Wait"); //waits for GSM module to turn off
        delay(9000); 
        asm ( " jmp 0"); //restart command
     }
   }
   
}//function end bracket

