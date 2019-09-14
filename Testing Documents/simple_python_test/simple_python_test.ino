int number; 
void setup() {
        pinMode(13, OUTPUT); 
        Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
}

void loop()
{
        // send data only when you receive data:
        if (Serial.available() > 0) {
                // read the incoming byte:
               number = Serial.read();
               
                if(number == 100)
                {
                  digitalWrite(13, HIGH); 
                }
                
                // say what you got:
                
        }
}
 
