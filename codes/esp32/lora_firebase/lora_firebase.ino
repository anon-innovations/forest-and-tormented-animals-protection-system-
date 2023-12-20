#include <LoRa.h>
#include <SPI.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <FirebaseESP32.h>

#define ss 5
#define rst 4
#define dio0 2

HTTPClient HttpClient;

// WiFi credentials
#define FIREBASE_HOST "Your Fire Host"
#define FIREBASE_AUTH "Fire Auth"
const char* SSID = "Your SSID";
const char* PASSWORD = "Your SSID Password";

String Incoming = "";

byte LocalAddress = 0x06;

double lat1 = 0;
double lon1 = 0;

FirebaseData firebaseData;
FirebaseJson json;


void wifi() {
  WiFi.begin(SSID, PASSWORD);
  WiFi.setSleep(false);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

void setup() {
  Serial.begin(115200);

  wifi();

  LoRa.setPins(ss, rst, dio0);

  Serial.println("Start LoRa init...");
  if (!LoRa.begin(433E6)) {             // initialize ratio at 915 or 433 MHz
    Serial.println("LoRa init failed. Check your connections.");
    while (true);                       // if failed, do nothing
  }
  Serial.println("LoRa init succeeded.");

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  Serial.println("Firebase init succeeded.");
}

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

  //-----------------------------------------Fire------------------------------------------------------------------------------------------

  if (Incoming.startsWith("Device:") && Incoming.indexOf("LAT:") && Incoming.indexOf(",LON:") != -1 && Incoming.indexOf(",Fire") != -1) {
    int devIndex = Incoming.startsWith("Device:") + 6;
    int latIndex = Incoming.indexOf(",LAT:");
    int lonIndex = Incoming.indexOf(",LON:");

    // Extract latitude and longitude from the combined message
    String devStr = Incoming.substring(devIndex, latIndex);
    String latitudeStr = Incoming.substring(latIndex + 5 , lonIndex);
    String longitudeStr = Incoming.substring(lonIndex + 5);

    // Convert latitude and longitude strings to float values
    lat1 = latitudeStr.toFloat();
    lon1 = longitudeStr.toFloat();

    // Print the extracted data
    Serial.print("Lati2: ");
    Serial.print(lat1, 6);
    Serial.print(" --- Lon2: ");
    Serial.println(lon1, 6);

    String info = "fire";
    String dev = devStr;
    int devInt = dev.toInt();

    String completePathlat = "dev" + dev + "/latitude";
    String completePathlon = "dev" + dev + "/longitude";
    String completePathDet = "dev" + dev + "/Device";

    Firebase.setFloat(firebaseData, completePathlat, lat1);
    Firebase.setFloat(firebaseData, completePathlon , lon1);
    Firebase.setInt(firebaseData, completePathDet , devInt);

    Firebase.setFloat(firebaseData, "com/latitude", lat1);
    Firebase.setFloat(firebaseData, "com/longitude" , lon1);
    Firebase.setString(firebaseData, "com/Detection" , info);
    Firebase.setInt(firebaseData, "com/Device" , devInt);

    //-----------------set History------------------------------------
    Firebase.pushFloat(firebaseData, "history/latitude", lat1);
    Firebase.pushFloat(firebaseData, "history/longitude" , lon1);
    Firebase.pushString(firebaseData, "history/Detection" , info);
    Firebase.pushInt(firebaseData, "history/Device" , devInt);

    Serial.println("Fire data send to firebase");

    String detected = "Fire detected in device " + dev;

    sendNotification("echoSentry Notification", detected);
  }

  //--------------------------------------------------Sound----------------------------------------------------------------------------

  if (Incoming.startsWith("Device:") && Incoming.indexOf(",LAT:") != -1 && Incoming.indexOf(",LON:") != -1 && Incoming.indexOf(",SOU") != -1) {
    int devIndex = Incoming.startsWith("Device:") + 6;
    int latIndex = Incoming.indexOf(",LAT:");
    int lonIndex = Incoming.indexOf(",LON:");
    int souIndex = Incoming.indexOf(",SOU:");


    // Extract latitude and longitude from the combined message
    String devStr = Incoming.substring(devIndex, latIndex);
    String latitudeStr = Incoming.substring(latIndex + 5 , lonIndex);
    String longitudeStr = Incoming.substring(lonIndex + 5, souIndex); // 5 is the length of ",LON:"
    String souStr = Incoming.substring(souIndex + 5);
    Serial.println(devStr);
    Serial.println(latitudeStr);
    Serial.println(longitudeStr);
    Serial.println(souStr);

    // Convert latitude and longitude strings to float values
    lat1 = latitudeStr.toFloat();
    lon1 = longitudeStr.toFloat();

    // Print the extracted data
    Serial.print("Lati2: ");
    Serial.print(lat1, 6);
    Serial.print(" --- Lon2: ");
    Serial.println(lon1, 6);

    String info = "Sound";
    String dev = devStr;
    int devInt = dev.toInt();


    String completePathlat = "dev" + dev + "/latitude";
    String completePathlon = "dev" + dev + "/longitude";
    String completePathDet = "dev" + dev + "/Device";

    Firebase.setFloat(firebaseData, completePathlat, lat1);
    Firebase.setFloat(firebaseData, completePathlon , lon1);
    Firebase.setInt(firebaseData, completePathDet , devInt);

    Firebase.setFloat(firebaseData, "com/latitude", lat1);
    Firebase.setFloat(firebaseData, "com/longitude" , lon1);
    Firebase.setString(firebaseData, "com/Detection" , info);
    Firebase.setInt(firebaseData, "com/Device" , devInt);

    //-----------------set History------------------------------------
    Firebase.pushFloat(firebaseData, "latitude", lat1);
    Firebase.pushFloat(firebaseData, "longitude" , lon1);
    Firebase.pushString(firebaseData, "Detection" , info);
    Firebase.pushInt(firebaseData, "Device" , devInt);

    Serial.println("Fire data send to firebase");

    String detected = "Gunshot detected in device " + dev;

    sendNotification("echoSentry Notification", detected);
  }

  //-------------------------------------Raspi---------------------------------------------------

  if (Incoming.startsWith("Received:") && Incoming.indexOf(",COUNT:") != -1) {
    int detIndex = Incoming.indexOf("Received:") + 9;
    int couIndex = Incoming.indexOf(",COUNT:");

    // Extract latitude and longitude from the combined message
    String detStr = Incoming.substring(detIndex, couIndex);
    String couStr = Incoming.substring(couIndex + 7);
    String info;

    int detInt = detStr.toInt();

    if (detInt == 1) {
      info = "elephant";
    } else if (detInt == 2) {
      info = "fire";
    } else if (detInt == 3) {
      info = "weapon";
    } else if (detInt == 4) {
      info = "bear";
    } else if (detInt == 5) {
      info = "leopard";
    }
    Serial.println(info);
    Serial.println(couStr);

    int couInt = couStr.toInt();

    Firebase.setString(firebaseData, "/com/Detection" , info);
    Firebase.setInt(firebaseData, "/com/Count" , couInt);

    //-----------------set History------------------------------------
    Firebase.pushString(firebaseData, "history/Detection" , info);
    Firebase.pushInt(firebaseData, "history/Count" , couInt);

    Serial.println("Detection data send to firebase");

    String msg = couStr + " " + info + " Detected";

    sendNotification("echoSentry Notification", msg );
  }
}

void loop() {
  onReceive(LoRa.parsePacket());
}


void sendNotification(String title, String body) {
  String DEVICE_FCM_KEY = "Your FCM Key";
  String FCM_API_KEY = "FCM API";
  String FCM_URL = "https://fcm.googleapis.com/fcm/send";

  //Check the current connection status
  if ((WiFi.status() == WL_CONNECTED)) {
    //Specify the URL
    HttpClient.begin(FCM_URL);

    // Set headers
    HttpClient.addHeader("Authorization", FCM_API_KEY);
    HttpClient.addHeader("Content-Type", "application/json");

    // Data string
    String data = "{\"registration_ids\": [\"" + DEVICE_FCM_KEY + "\"], \"notification\": {\"body\":\"" + body + "\", \"title\":\"" + title + "\"}}";

    //Make the request
    int httpCode = HttpClient.POST(data);

    //Check for the returning code
    if (httpCode == HTTP_CODE_OK) {
      Serial.println("Notification Sent To The Phone");
    } else {
      Serial.println("Error on sending notification");
      Serial.println(httpCode);
      Serial.println(HttpClient.getString());
    }
    HttpClient.end();  //Free the resources
  }
}
