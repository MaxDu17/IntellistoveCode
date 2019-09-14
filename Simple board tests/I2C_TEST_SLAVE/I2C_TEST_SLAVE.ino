#include <Wire.h>
int state = 0; 
void setup() {
  pinMode(13, OUTPUT); 
  Wire.begin(1); 
  Wire.onReceive(getvalue);
}
void getvalue(int asdf)
{
  state = Wire.read();
}

void loop() {
  if( state == 1)
  {
    digitalWrite(13, HIGH);
  }
  else
  {
    digitalWrite(13, LOW); 
  }

}
