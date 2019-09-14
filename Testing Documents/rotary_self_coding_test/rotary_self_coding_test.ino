int clk = 0;
int dl = 0;
int turn = 123; 

volatile boolean up; 

void detect()
{
  turn = 1; 
  up = (digitalRead(3) == digitalRead(2)); // if both things match, up is true//
}

void setup() {
  pinMode(3, INPUT); // clk//
  pinMode(2, INPUT); // do //
  Serial.begin(9600); 
 attachInterrupt(0, detect, FALLING);  
}

void loop() {
  clk = digitalRead(3);
  dl = digitalRead(2); 
 
  if (turn == 1)
  {
    //Serial.print("turned\n");
    if(up)
    {
      Serial.print("counter\n");
    }
    else if (!up) // if up is false, it means its turned clockwise, since the phase is offset to the left//
    {
      Serial.print("clock\n");
    }
    turn = 123; 
    
  }
  
  
}

