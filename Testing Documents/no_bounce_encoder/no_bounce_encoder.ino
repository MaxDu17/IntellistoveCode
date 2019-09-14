 #include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 7, 6, 5, 4);
int clk = 0;
int dl = 0;
int turn = 123; 
int curr= 0; 
int prev = 0; 

volatile boolean up; 
void detect()
{
  turn = 1; 
  up = (digitalRead(3) == digitalRead(2)); // if both things match, up is true//
}

void setup() 
{
 pinMode(3, INPUT); // clk//
 pinMode(2, INPUT); // do //
 //attachInterrupt(0, detect, FALLING);  
 lcd.begin(20, 4);
 Serial.begin(9600);  
 prev = millis();
 delay(100); 
 attachInterrupt(0, detect, FALLING);
}

void loop() 
{ 

  
   
  clk = digitalRead(3);
  dl = digitalRead(2); 
 
  if (turn == 1)
  {  
      turn = 0;
      
      if(up)
      {
        
        curr = millis(); 
        if (curr - prev > 100)
        {
          Serial.print("counter\n"); 
          prev = millis(); 
        }
        else
        {
          //Serial.print("bounced\n");
        }
      
      }
     
    
      else if (!up) // if up is false, it means its turned clockwise, since the phase is offset to the left//
      {
        curr = millis(); 
        if (curr - prev > 100)
        {
          Serial.print("clock\n"); 
          prev = millis(); 
        }
        else
        {
          //Serial.print("bounced\n");
        }
        
    
      }
  }
}
 
 


