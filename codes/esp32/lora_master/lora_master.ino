//---------------------------------------- Include Library.
#include <SPI.h>
#include <LoRa.h>
#include <TinyGPSPlus.h>
#include <Arduino.h>
#include <math.h>
#include <AccelStepper.h>
TinyGPSPlus gps;
//----------------------------------------

//---------------------------------------- LoRa Pin / GPIO configuration.
#define ss 5
#define rst 4
#define dio0 2
//----------------------------------------
// Define the pins connected to the A4988 driver on the CNC shield
const int stepPin = 14;  // The step pin controls the movement
const int directionPin = 12;   // The direction pin determines the direction of movement

#define RUN 2
#define STOP 0
#define SETP360 200

byte statMotor = RUN;
int cntrStep = 0;
bool direcCw = false;
byte statEvent = 0;

int limitSwitchPin = 27;
int LimiValue = 0;
int statDirection = 0;

bool ERROE1 = true;

double bearing = 0.0;
// Create an instance of the AccelStepper library
AccelStepper stepper(AccelStepper::DRIVER, stepPin, directionPin);

//---------------------------------------- Variable declaration to hold incoming and outgoing data.
String Incoming = "";
String Incoming_send = "";
String Message = "";
String Message_esp = "";
//----------------------------------------

//---------------------------------------- LoRa data transmission configuration.
byte LocalAddress = 0x01;               //--> address of this device (Master Address).
byte Destination_ESP32_Slave_1 = 0x02;  //--> destination to send to Slave 1 (ESP32).
byte Destination_ESP32_Slave_2 = 0x03;  //--> destination to send to Slave 2 (ESP32).
byte Destination_ESP32_Slave_3 = 0x04;  //--> destination to send to Slave 3 (ESP32).
byte Destination_ESP32_Slave_4 = 0x05;  //--> destination to send to Slave 4 (ESP32).
byte Destination_ESP32_WIFI_5 = 0x06;  //--> destination to send to Slave 5 (ESP32).
//----------------------------------------

//---------------------------------------- Variable declaration for Millis/Timer.
unsigned long previousMillis_SendMSG = 0;
const long interval_SendMSG = 500;
//----------------------------------------

// Variable declaration to count slaves.
byte Slv = 0;
double lat1 = 0;
double lon1 = 0;
double lat2 = 0;
double lon2 = 0;

byte dirc = 1;
byte statSwitch = 0;

//bool statMotor = false;
long period = 0;
long cntrTime = 0;
const int OFFTIME = 30;
const int ONTIME = 1;

//________________________________________________________________________________ Subroutines for sending data (LoRa Ra-02).
void sendMessage(String Outgoing, byte Destination) {
  LoRa.beginPacket();             //--> start packet
  LoRa.write(Destination);        //--> add destination address
  LoRa.write(LocalAddress);       //--> add sender address
  LoRa.write(Outgoing.length());  //--> add payload length
  LoRa.print(Outgoing);           //--> add payload
  LoRa.endPacket();               //--> finish packet and send it
}
//________________________________________________________________________________ Subroutines for sending data (LoRa Ra-02).
void sendMessage1(String Outgoing, byte Destination) {
  LoRa.beginPacket();             //--> start packet
  LoRa.write(Destination);        //--> add destination address
  LoRa.write(LocalAddress);       //--> add sender address
  LoRa.write(Outgoing.length());  //--> add payload length
  LoRa.print(Outgoing);           //--> add payload
  LoRa.endPacket();               //--> finish packet and send it
}
//________________________________________________________________________________ Subroutines for receiving data (LoRa Ra-02).
void onReceive(int packetSize) {
  if (packetSize == 0) return;  // If there's no packet, return

  //---------------------------------------- read packet header bytes:
  int recipient = LoRa.read();        // Recipient address
  byte sender = LoRa.read();          // Sender address
  byte incomingLength = LoRa.read();  // Incoming message length
  //----------------------------------------

  // Clears Incoming variable data
  Incoming = "";

  //---------------------------------------- Get all incoming data.
  while (LoRa.available()) {
    Incoming += (char)LoRa.read();
    Serial.println(Incoming);
  }

  // Check length for error
  if (incomingLength != Incoming.length()) {
    Serial.println("error: message length does not match length");
    return; // Skip rest of function
  }

  // Checks whether the incoming data or message is for this device
  if (recipient != LocalAddress) {
    Serial.println("This message is not for me.");
    return; // Skip rest of function
  }

  // If message is for this device, or broadcast, print details
  Serial.println();
  Serial.println("Received from: 0x" + String(sender, HEX));
  Serial.println("Message length: " + String(incomingLength));
  Serial.println("Message: " + Incoming);

  Incoming_send = "Device:" + String(sender, HEX) + "," + Incoming;
  sendMessage(Incoming_send, Destination_ESP32_WIFI_5);

  // Parse and process the combined message.
  if (Incoming.startsWith("LAT:") && Incoming.indexOf(",LON:") != -1 && Incoming.indexOf(",Fire") != -1) {
    int latIndex = Incoming.indexOf("LAT:") + 4;
    int lonIndex = Incoming.indexOf(",LON:");

    // Extract latitude and longitude from the combined message
    String latitudeStr = Incoming.substring(latIndex, lonIndex);
    String longitudeStr = Incoming.substring(lonIndex + 5); // 5 is the length of ",LON:"

    // Convert latitude and longitude strings to float values
    lat1 = latitudeStr.toFloat();
    lon1 = longitudeStr.toFloat();

    statMotor = STOP;

    // Print the extracted data
    Serial.print("Lati2: ");
    Serial.print(lat1, 6);
    Serial.print(" --- Lon2: ");
    Serial.println(lon1, 6);

    Serial.print("Lati1: ");
    Serial.print(gps.location.lat(), 6); // 6 decimal places
    Serial.print(" --- Lon1: ");
    Serial.println(gps.location.lng(), 6); // 6 decimal places

  }
}

//________________________________________________________________________________ VOID SETUP
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial2.begin(9600);
  //---------------------------------------- Settings and start Lora Ra-02.
  LoRa.setPins(ss, rst, dio0);
  pinMode(stepPin, OUTPUT);
  pinMode(directionPin, OUTPUT);
  pinMode(limitSwitchPin, INPUT_PULLUP);

  // Set the maximum speed and acceleration
  stepper.setMaxSpeed(100);  // Adjust this value as needed
  stepper.setAcceleration(200);  // Adjust this value as needed

  Serial.println("Start LoRa init...");
  if (!LoRa.begin(433E6)) {             // initialize ratio at 915 or 433 MHz
    Serial.println("LoRa init failed. Check your connections.");
    while (true);                       // if failed, do nothing
  }
  Serial.println("LoRa init succeeded.");
}
//________________________________________________________________________________ VOID LOOP
void loop() {
  stepper_code();
  while (ERROE1) {
    unsigned long currentMillis_SendMSG = millis();

    if (currentMillis_SendMSG - previousMillis_SendMSG >= interval_SendMSG) {
      previousMillis_SendMSG = currentMillis_SendMSG;

      Slv++;
      if (Slv > 4) Slv = 1;

      Message_esp = "SDS" + String(Slv);

      //::::::::::::::::: Condition for sending message / command data to Slave 1 (ESP32 Slave 1).
      if (Slv == 1) {
        Serial.println();
        Serial.print("Send message to ESP32 Slave " + String(Slv));
        Serial.println(" : " + Message_esp);
        sendMessage(Message_esp, Destination_ESP32_Slave_1);
      }
      //motorRotate();
      //::::::::::::::::: Condition for sending message / command data to Slave 2 (ESP32 Slave 2).
      if (Slv == 2) {
        Serial.println();
        Serial.print("Send message to ESP32 Slave " + String(Slv));
        Serial.println(" : " + Message_esp);
        sendMessage(Message_esp, Destination_ESP32_Slave_2);
      }
      //motorRotate();
      //::::::::::::::::: Condition for sending message / command data to Slave 3 (ESP32 Slave 3).
      if (Slv == 3) {
        Serial.println();
        Serial.print("Send message to ESP32 Slave " + String(Slv));
        Serial.println(" : " + Message_esp);
        sendMessage(Message_esp, Destination_ESP32_Slave_3);
      }
      //motorRotate();
      //::::::::::::::::: Condition for sending message / command data to Slave 4 (ESP32 Slave 4).
      if (Slv == 4) {
        Serial.println();
        Serial.print("Send message to ESP32 Slave " + String(Slv));
        Serial.println(" : " + Message_esp);
        sendMessage(Message_esp, Destination_ESP32_Slave_4);

        ERROE1 = false;
      }
    }

    //---------------------------------------// parse for a packet, and call onReceive with the result:
    onReceive(LoRa.parsePacket());
    //---------------------------------------//
  }

  raspi();
  stepper_code();
  while (Serial2.available() > 0) {
    if (gps.encode(Serial2.read())) {
      // Check if we have new valid GPS data
      if (gps.location.isValid()) {
        lat2 = gps.location.lat(), 6;
        lon2 = gps.location.lng(), 6;

      }
    }
  }
  stepper_code();
}

void raspi() {//----------------------------------------------------------------------------
  String receivedData_pi = Serial.readStringUntil('\n');
  if (receivedData_pi != NULL) {
    Serial.print("Received: ");
    Serial.println(receivedData_pi);
    sendMessage1(receivedData_pi, Destination_ESP32_WIFI_5);
  } else {
    printf("No data received.\n");
  }
  ERROE1 = true;
}

void stepper_code() {
  limitSwich();
  if (millis() > (cntrTime + period)) {
    cntrTime = millis();
    if (statMotor) {
      statMotor = false;
      motorDrive(statMotor);
      period = OFFTIME;
    } else {
      statMotor = true;
      motorDrive(statMotor);
      period = ONTIME;
    }
  }
}

byte statSw = 0;

void limitSwich() {
  LimiValue = digitalRead(limitSwitchPin);
  if (LimiValue == 1) {
    if (statSw == 0) return;
    statSw = 0;
    if (digitalRead(directionPin) == LOW) {
      digitalWrite(directionPin, HIGH);
      Serial.print("!"); Serial.println(digitalRead(directionPin));
    } else {
      digitalWrite(directionPin, LOW);
      Serial.print("#"); Serial.println(digitalRead(directionPin));
    }
  } else {
    if (statSw == 1) return;
    statSw = 1;
  }
}
void motorDrive(bool isOn) {
  if (isOn) {
    digitalWrite(stepPin, HIGH);
  }
  else {
    digitalWrite(stepPin, LOW);
  }
}
