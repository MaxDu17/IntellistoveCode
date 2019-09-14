char a = 'h'; 
char b = 'e'; 
char c = 'l';
char d = 'o';
String e; 
char text[15]; 
char command[30]; 
void setup() 
{
  Serial.begin(9600); 
  text[0] = '0';
  text[1] = '1';
  text[2] = '2';
  text[3] = '3';
  text[4] = '4';
  text[5] = '5';
  text[6] = 0;
   
  sprintf(command, "hi world: %s %d", text, 3); 
  Serial.write(command); 

}

void loop() 
{
  
}
