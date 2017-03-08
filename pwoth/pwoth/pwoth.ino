#include <SPI.h>
#include <Ethernet2.h>
#include <EthernetUdp2.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PWOTH_B // Either Board A or B. Change PWOTH_A to PWOTH_B for 2nd Board

#if defined(PWOTH_A)
byte mac[] = {
  0x90, 0xA2, 0xDA, 0x10, 0xE8, 0x19
};
IPAddress ip(192, 168, 1, 100);
IPAddress remoteIp(192, 168, 1, 200);
int NUMPIXELS = 12;
char ReplyBuffer[] = "ack pwoth a: ";
#endif

#if defined (PWOTH_B)
byte mac[] = {
  0x90, 0xA2, 0xDA, 0x10, 0xE8, 0x52
};
IPAddress ip(192, 168, 1, 200);
IPAddress remoteIp(192, 168, 1, 100);
int NUMPIXELS = 8;
char ReplyBuffer[] = "ack pwoth b: ";
#endif

int c = 128;
unsigned int localPort = 8888;      // local port to listen on

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];  //buffer to hold incoming packet,

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

// Pin output for NeoPixel
#define PIN 7

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// Joystick Related
int xPin = A1;
int yPin = A0;
int buttonPin = 2;

int xPosition = 0;
int yPosition = 0;
int buttonState = 0;

void setup() {
  // start the Ethernet and UDP:
  Ethernet.begin(mac, ip);
  Udp.begin(localPort);
  Serial.begin(9600);

  // Neopixel setup
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code

  pixels.begin(); // This initializes the NeoPixel library.

  // Joystick setup
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);

  //activate pull-up resistor on the push-button pin
  pinMode(buttonPin, INPUT_PULLUP);

  pinMode(LED_BUILTIN,OUTPUT);
}


void loop() {

  // Read from the joystick
  xPosition = analogRead(xPin);
  yPosition = analogRead(yPin);
  buttonState = digitalRead(buttonPin);

  // For debugging
  /*Serial.print("X: ");
    Serial.print(xPosition);
    Serial.print(" | Y: ");
    Serial.print(yPosition);
    Serial.print(" | Button: ");
    Serial.println(buttonState);*/

  int low_thresh = 250;
  int hig_thresh = 750;
  int mid_threas = 512;

  int code = -1; // unknown code
  char replyBuffer[8];

  replyBuffer[0] = '\0';
  if (buttonState == 0) {
    code = 1;
    sprintf(replyBuffer, "1");
  } else if (xPosition < low_thresh && yPosition < low_thresh) { // these 4 are diagonol
    code = 2;
    sprintf(replyBuffer, "22");
  } else if (xPosition > hig_thresh && yPosition < low_thresh)  {
    code = 2;
    sprintf(replyBuffer, "22");
  } else if (xPosition < low_thresh && yPosition > hig_thresh)  {
    code = 3;
    sprintf(replyBuffer, "33");
  } else if (xPosition > hig_thresh && yPosition > hig_thresh)  {
    code = 3;
    sprintf(replyBuffer, "333");
  } else if (xPosition > hig_thresh) { // these are up,down, left right
    code = 4;
    sprintf(replyBuffer, "444");
  } else if (yPosition > hig_thresh) {
    code = 5;
    sprintf(replyBuffer, "55555");
  } else if (xPosition < low_thresh) {
    code = 6;
    sprintf(replyBuffer, "666666");
  } else if (yPosition < low_thresh) {
    code = 7;
    sprintf(replyBuffer, "7777777");
  }

  // Send the code to the other pwoth
  if (code > 0) {
    Serial.print("Code = ");
    Serial.println(code);
    // send a reply to the IP address and port that sent us the packet we received

    replyBuffer[code] = '\0';
    
    // receiver triggers on length of the packet
    Udp.beginPacket(remoteIp, localPort);
    Udp.write(replyBuffer,code);
    Udp.endPacket();

    Serial.print("Reply = ");
    Serial.print(replyBuffer);
    Serial.print(" len ");
    Serial.println(strlen(replyBuffer));
  } else {
    dark();
  }

  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remote = Udp.remoteIP();
    for (int i = 0; i < 4; i++) {
      Serial.print(remote[i], DEC);
      if (i < 3) {
        Serial.print(".");
      }
    }
    Serial.print(", port ");
    Serial.println(Udp.remotePort());

    // read the packet into packetBufffer
    Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    Serial.println("Contents:");
    Serial.println(packetBuffer);

    String pbStr = String(packetBuffer);
    int len = pbStr.length();

    Serial.print("length: ");
    Serial.println(len);
    // Set the colors and send a reply when we didn't get an ack
    if(len > 0 && len < 8) {
      sendcolors(len);
      // send a reply to the IP address and port that sent us the packet we received
      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      Udp.write(ReplyBuffer);
      Udp.write(packetBuffer,len);
      Udp.endPacket();
    }
  }
}

void dark() {
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    pixels.show(); // This sends the updated pixel color to the hardware.
  }
}

void sendcolors(int rgb) {
  digitalWrite(LED_BUILTIN,HIGH);
  for (int i = 0; i < NUMPIXELS; i++) {
    switch (rgb) {
      case 1: // red
        pixels.setPixelColor(i, pixels.Color(c, 0, 0));
        break;
      case 2: // green
        pixels.setPixelColor(i, pixels.Color(0, c, 0));
        break;
      case 3: // red+green
        pixels.setPixelColor(i, pixels.Color(c, c, 0));
        break;
      case 4: // blue
        pixels.setPixelColor(i, pixels.Color(0, 0, c));
        break;
      case 5: // red+blue
        pixels.setPixelColor(i, pixels.Color(c, 0, c));
        break;
      case 6: // green+blue
        pixels.setPixelColor(i, pixels.Color(0, c, c));
        break;
      case 7: // red+green+blue
        pixels.setPixelColor(i, pixels.Color(c, c, c));
        break;
    }
    pixels.show(); // This sends the updated pixel color to the hardware.
  }
  digitalWrite(LED_BUILTIN,LOW);
  delay(50);
}

/* Not used
 * 
// Stuff for the pulse function
int delayval = 0; // delay between sending each pixel, slow will walk the ring..
int dir = 1;
int up = 3;
int prev = 0;
int c = 1;
#define MAX 128
void pulse(int numpulse, int rgb) {
  Serial.print("ent pulse ");
  Serial.print(numpulse);
  Serial.print(", ");
  Serial.println(rgb);

  static int prev = c;
  for (int k = 0; k < numpulse; k++) {
    for (int j = 0; j < 255; j++) {
      c = c + dir;
      for (int i = 0; i < NUMPIXELS; i++) {
        switch (rgb) {
          case 1: // red
            pixels.setPixelColor(i, pixels.Color(c, 0, 0));
            break;
          case 2: // green
            pixels.setPixelColor(i, pixels.Color(0, c, 0));
            break;
          case 3: // red+green
            pixels.setPixelColor(i, pixels.Color(c, c, 0));
            break;
          case 4: // blue
            pixels.setPixelColor(i, pixels.Color(0, 0, c));
            break;
          case 5: // red+blue
            pixels.setPixelColor(i, pixels.Color(c, 0, c));
            break;
          case 6: // green+blue
            pixels.setPixelColor(i, pixels.Color(0, c, c));
            break;
          case 7: // red+green+blue
            pixels.setPixelColor(i, pixels.Color(c, c, c));
            break;
        }
        pixels.show(); // This sends the updated pixel color to the hardware.
        delay(delayval); // Delay for a period of time (in milliseconds).
      }

      if (prev == MAX - 1 && c == MAX) {
        dir = dir - up;
      } else if (prev > c && c <= 0) {
        dir = dir + up;
      }
      delay(10);
    }
  }

  // Set the lights back to nothing
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(delayval); // Delay for a period of time (in milliseconds).
  }
}*/
