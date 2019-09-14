// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  if(Serial.available() > 0){
    char inByte = Serial.read(); 
    Serial.println(inByte);
    delay(10);                       
  }
}
