//---------------------------------------- Include Library.
#include <SPI.h>
#include <LoRa.h>
#include <TinyGPSPlus.h>
TinyGPSPlus gps;
//----------------------------------------

//---------------------------------------- LoRa Pin / GPIO configuration.
#define ss 5
#define rst 4
#define dio0 2
//----------------------------------------
const int FireSensorPin = 32; // You can change this to the pin you are using
const int soundSensorPin = 34; // You can change this to the pin you are using

unsigned long previousMillis = 0;  // Initialize a variable to store the previous millis value
const unsigned long interval = 1000;  // Define the interval in milliseconds (1 second)


int soundValue = 0;
int fireValue = 0;
int soundError = 0;
int fireError = 0;

unsigned int output;
int Decibels;

//---------------------------------------- Variable declaration to hold incoming and outgoing data.
String Incoming = "";
String Message = "";
//----------------------------------------


byte LocalAddress = 0x02;          //--> address of this device (Slave 1).
//byte LocalAddress = 0x03;        //--> address of this device (Slave 2).
//byte LocalAddress = 0x04;        //--> address of this device (Slave 2).
//byte LocalAddress = 0x05;        //--> address of this device (Slave 2).

byte Destination_Master = 0x01; //--> destination to send to Master (ESP32).
//----------------------------------------


//________________________________________________________________________________ Subroutines for sending data (LoRa Ra-02).
void sendMessage(String Outgoing, byte Destination) {
  LoRa.beginPacket();             //--> start packet
  LoRa.write(Destination);        //--> add destination address
  LoRa.write(LocalAddress);       //--> add sender address
  LoRa.write(Outgoing.length());  //--> add payload length
  LoRa.print(Outgoing);           //--> add payload
  LoRa.endPacket();               //--> finish packet and send it
}
//________________________________________________________________________________

//________________________________________________________________________________ Subroutines for receiving data (LoRa Ra-02).
void onReceive(int packetSize) {
  if (packetSize == 0) return;  //--> if there's no packet, return

  //---------------------------------------- read packet header bytes:
  int recipient = LoRa.read();        //--> recipient address
  byte sender = LoRa.read();          //--> sender address
  byte incomingLength = LoRa.read();  //--> incoming msg length
  //----------------------------------------

  // Clears Incoming variable data.
  Incoming = "";

  //---------------------------------------- Get all incoming data.
  while (LoRa.available()) {
    Incoming += (char)LoRa.read();
  }
  //----------------------------------------

  //---------------------------------------- Check length for error.
  if (incomingLength != Incoming.length()) {
    Serial.println("error: message length does not match length");
    return; //--> skip rest of function
  }
  //----------------------------------------

  //---------------------------------------- Checks whether the incoming data or message for this device.
  if (recipient != LocalAddress) {
    Serial.println("This message is not for me.");
    return; //--> skip rest of function
  }
  //----------------------------------------

  //---------------------------------------- if message is for this device, or broadcast, print details:
  Serial.println();
  Serial.println("Received from: 0x" + String(sender, HEX));
  Serial.println("Message length: " + String(incomingLength));
  Serial.println("Message: " + Incoming);


  // Calls the Processing_incoming_data() subroutine.
  Processing_incoming_data();
}
//________________________________________________________________________________

//________________________________________________________________________________ Subroutines to process data from incoming messages, then send messages to the Master.
void Processing_incoming_data() {
  if (Incoming == "SDS1") {
    // Get latitude and longitude
    String latitudeStr = String(gps.location.lat(), 6);
    String longitudeStr = String(gps.location.lng(), 6);

    if (fireValue == HIGH) {

      // Create a single message containing both latitude and longitude
      Message = "LAT:" + latitudeStr + ",LON:" + longitudeStr + ",Fire";
      sendMessage(Message, Destination_Master);
      fireError = 1;

    }
    if (soundValue > 100) {
      Message = "LAT:" + latitudeStr + ",LON:" + longitudeStr + ",SOU:" + soundValue;
      sendMessage(Message, Destination_Master);
      soundError = 1;

    }
    Serial.println();
    Serial.print("Send message to Master : ");
    Serial.println(Message);



  }
}


//________________________________________________________________________________ VOID SETUP
void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  Serial2.begin(9600);

  pinMode(FireSensorPin, INPUT);
  pinMode(soundSensorPin, INPUT);
  //---------------------------------------- Settings and start Lora Ra-02.
  LoRa.setPins(ss, rst, dio0);

  Serial.println();
  Serial.println("Start LoRa init...");
  if (!LoRa.begin(433E6)) {             // initialize ratio at 915 or 433 MHz
    Serial.println("LoRa init failed. Check your connections.");
    while (true);                       // if failed, do nothing
  }
  Serial.println("LoRa init succeeded.");
  //----------------------------------------
}
//________________________________________________________________________________

//________________________________________________________________________________ VOID LOOP
void loop() {
  sound();
  soundValue = Decibels;
  //soundValue = analogRead(soundSensorPin);
  fireValue = digitalRead(FireSensorPin);
  //--------------------------------GPS Location----------------------------------

  while (Serial2.available() > 0)
    if (gps.encode(Serial2.read()))
      displayInfo();
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while (true);
  }
  //------------------------------------------------------------------

  if ( fireValue == HIGH) {
    while (fireError == 0) {
      onReceive(LoRa.parsePacket());
    }
  } else if (fireValue == LOW) {
    fireError = 0;

  }
  if (soundValue > 100) {
    while (soundError == 0) {
      onReceive(LoRa.parsePacket());
    }
  } else if (soundValue < 100) {
    soundError = 0;
  }
}

void displayInfo()
{
  Serial.print(F("Location: "));
  if (gps.location.isValid()) {
    Serial.print("Lat: ");
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print("Lng: ");
    Serial.print(gps.location.lng(), 6);
    Serial.println();
  }
  else
  {
    Serial.println(F("INVALID"));
  }
}
void sound() {
  unsigned long start_time = millis();
  float PeakToPeak = 0;
  unsigned int maximum_signal = 0;  //minimum value
  unsigned int minimum_signal = 4095;  //maximum value
  while (millis() - start_time < 50)
  {
    output = analogRead(soundSensorPin);
    if (output < 4095)
    {
      if (output > maximum_signal)
      {
        maximum_signal = output;
      }
      else if (output < minimum_signal)
      {
        minimum_signal = output;
      }
    }
  }
  PeakToPeak = maximum_signal - minimum_signal;
  //Serial.println(PeakToPeak);
  Decibels = map(PeakToPeak, 50, 500, 49.5, 90);
  delay(100);
}
