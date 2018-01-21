/*
IAGODUINO :
SIMPLE SEND/RECEIVE FOR A PRESSURE SENSOR AND AN LED ON MKR1000
*/


#include <SPI.h>
#include <WiFi101.h>
#include <WiFiUdp.h>

#define sensorPin A1
#define ledPin 2


//___________________IP ADDRESS OF THE MAC RUNNING IAGOTCHI __________________
char IAGOTCHI_IP[] = "192.168.1.31";

//___________________CHANGE THIS FOR SERIAL DEBUG __________________
boolean  useSerial = 0;

//___________________DONT CHANGE ANYTHING BELOW THIS LINE __________________
int status = WL_IDLE_STATUS;
char ssid[] = "GroupeDunes"; //  your network SSID (name)
char pass[] = "decetendroit";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

unsigned int localPort = 2390;      // local port to listen on

unsigned int IAGOTCHI_port = 2391; //distant port to send on

char packetBuffer[255]; //buffer to hold incoming packet
int valueToSend;
char  ReplyBuffer[3];       // a string to send back

WiFiUDP Udp;

//________________________SETUP
void setup() {
  pinMode(sensorPin, INPUT);
  pinMode(ledPin, OUTPUT);
  //Initialize serial and wait for port to open:
  if (useSerial)
  {
    Serial.begin(9600);
    while (!Serial) {
      ; // wait for serial port to connect. Needed for native USB port only
    }
  }
  InitWifi();
  Udp.begin(localPort);
  //___________BLINK LED TO SAY HELLO
  for (int i = 0; i < 10; i++)
  {
    digitalWrite (ledPin, HIGH);
    delay(50);
    digitalWrite (ledPin, LOW);
    delay(50);
  }
}


//________________________LOOP
void loop() {

  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    int len = Udp.read(packetBuffer, 255);
    if (len > 0) packetBuffer[len] = 0;
    Serial.println(packetBuffer);
    analogWrite(ledPin, atoi(packetBuffer));
  }
  Udp.beginPacket(IAGOTCHI_IP, IAGOTCHI_port);
  valueToSend = map(analogRead(sensorPin), 0, 800, 0, 100);
  itoa(valueToSend, ReplyBuffer, 10);
  Udp.write(ReplyBuffer);
  Udp.endPacket();
  delay(1);
}



