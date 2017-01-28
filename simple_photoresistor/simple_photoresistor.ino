#define thresh 600 // If our analogRead is less than this we will blink

void setup() {
  pinMode(13,OUTPUT); // On board LED in Arduino Micro is 13
}

void loop() {
  int sensorValue = analogRead(A0); // read the voltage from the sensor on A0
  Serial.println(sensorValue,DEC);  // print the value
  digitalWrite(13,sensorValue<500); // Light LED if sensorValue is under thresh, else dark 
  delay(1000); // sleep for 1 second
}
