// Test to see the voltage fluctuate as the MQ2 senses smoke

void setup() {
    Serial.begin(9600);
}

void loop() {
    float sensor_volt;
    float sensorValue;

    sensorValue = analogRead(A0);
    sensor_volt = sensorValue/1024*5.0;

    // The following will print on the Serial Monitor
    Serial.print("sensor_volt = ");
    Serial.print(sensor_volt);
    Serial.println("V");
    delay(1000);
}
