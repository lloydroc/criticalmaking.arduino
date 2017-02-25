# Arduino Burnt Toast Detector with LCD and Temp Sensor
We will use an SMAKN® MLX90614ESF GY-906 I2C contactless temperature sensor as well as an ML2 smoke sensor to display on the LCD with the Arduino Starter Kit.

## Getting Started
Before doing anything we will verify the ML2 is sufficient for our needs of burning toast. The sensor has 4 pins:
1. Vcc - Hook this to +5V on your Arduino
2. GND - Hook this to GND on your Arduino
3. NC - This is a "no connect" and isn't needed
4. Analog Output - This is the output of the sensor. Wire to A0 of the Arduino

Use the male-female jumper wires we have in the studio to wire up the sensor.

Open up the toast.ino file from this repository and burn it to the Arduino. Then open the Serial Monitor and watch the numbers on the monitor. Burn something and put it in front of the sensor. Watch how the numbers change. Detecting this change will be crucial to our smoke detection algorithm.

## Next Steps
Now that we can confirm the ML2 sensor can detect smoke we need to both integrate in our LCD as well as a buzzer. Look in the Arduino Project Book for examples of this.
