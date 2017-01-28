/* A simple example which uses the Elegoo shock switch module, 
   mercury switch module, tilt switch module and knock switch module
   to blink an LED when they change. A 10k Ohm Pull-Up resistor needs
   to be connected from your switches output to +5V. See Lesson 5
   of the Elegoo 37 Kit Tutorial for the wiring as well as the lecture.
 */

void setup() {
  pinMode(A0,INPUT);
  pinMode(LED_BUILTIN,OUTPUT);
}

void loop() {
  if(digitalRead(A0)==0) {
    digitalWrite(LED_BUILTIN,1);
    Serial.println(1,DEC);
    delay(1000);
  } else {
    digitalWrite(LED_BUILTIN,0);
  }
}
