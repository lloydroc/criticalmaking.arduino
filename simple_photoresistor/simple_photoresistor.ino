#define thresh 600 // If our analogRead is less than this we will blink

void setup() {
  pinMode(LED_BUILTIN,OUTPUT); // On board LED in Arduino Micro is 13
}

void loop() {
  int sensorValue = analogRead(A0); // read the voltage from the sensor on A0
  Serial.println(sensorValue,DEC);  // print the value
  digitalWrite(LED_BUILTIN,sensorValue<500); // Light LED if sensorValue is under thresh, else dark 
  delay(1000); // sleep for 1 second
}
