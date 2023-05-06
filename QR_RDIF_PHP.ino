// Copyright 2023
// Author: Mehdi Bouzhuenda
/*******************************************************
**PROGRAM NAME -The program utilizes PHP, ESP32CAM, and RDM6300 to send QR code and RFID data logs to a database."

**PROGRAMMER: - MEHDI BOUZGUENDA - https://github.com/mehdibouzguenda


**DATE: - Started 02/06/2023
       

**BUGS: -  I am currently in the process of optimizing this program, 
          but there is still an outstanding issue with the RFID functionality. 
          While it is currently working, it is not performing as expected and requires further attention.

**DESCRIPTION:
    - This program incorporates a key library that has been sourced from GitHub.
      * https://github.com/alvarowolfx/ESP32QRCodeReader
    - Also We need to add esp to our Arduino IDE(the editor that i used).
      1- Open up your IDE then go to “File -> Preferences” or simply hit “Ctrl + comma”
      2- Paste https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json  in additional board manager URLs.
      3- Open the Boards Manager. Go to Tools > Board > Boards Manager… 
      4- Search for ESP32 and press install button for the “ESP32 by Espressif Systems“:
    - DONT FORGET to modify your SSID and PASSWORD IN THIS CODE.
    (I tested this program by running it on a local server using XAMPP.)
    - create database name: “QR_RFID_DATA_LOG”
    - import the database QR_RFID_DATA_LOG.sql
    -While uploading, I encountered an issue with the SoftwareSerial library. To resolve this, 
     I installed the ESP SoftwareSerial library, which can be found in the 'lib' folder of this repository.

*******************************************************/

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ESP32QRCodeReader.h>
#include <SoftwareSerial.h>


const char *ssid = "WIFI VDSL PRO";      //WIFI NAME OR HOTSPOT //Orange-E58F
const char *password = "Eventizer2023";  //WIFI PASSWORD POR MOBILE HOTSPOT PASSWORD//L2RhgnaDTDn


ESP32QRCodeReader reader(CAMERA_MODEL_AI_THINKER);
struct QRCodeData qrCodeData;
bool isConnected = false;



SoftwareSerial RFID(14, 15);  // RX and TX

String text;

//indicators
#define ERROR_PIN 12
#define SUCCESS_PIN 4
#define CONN_PIN 5

void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();

  WiFi.mode(WIFI_OFF);
  delay(1000);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  reader.setup();
  //reader.setDebug(true);
  Serial.println("Setup QRCode Reader");

  reader.begin();
  Serial.println("Begin QR Code reader");

  delay(1000);

  RFID.begin(115200);
  Serial.println("Bring your RFID Card Closer...");
}

void sendLog(String cardid) {

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String postData = "cardid=" + String(cardid) + "&action=insertRecord";
    http.begin("http://192.168.1.81/RFIDandPHP/process.php");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    int httpCode = http.POST(postData);
    String payload = http.getString();
    Serial.println(httpCode);
    Serial.println(payload);

    if (payload.equals("success")) {
      digitalWrite(SUCCESS_PIN, HIGH);
      Serial.println("DONE!!");
    } else {
      digitalWrite(ERROR_PIN, HIGH);
      Serial.println("non");
    }

    http.end();
  }
}

void toggleConnStat() {
  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(CONN_PIN, HIGH);
  } else {
    digitalWrite(CONN_PIN, LOW);
  }
}
char c;
void loop() {
  while (RFID.available() > 0) {
    delay(5);
    c = RFID.read();
    text += c;
    Serial.println("text");
  }
  if (text.length() > 20) {
    text = text.substring(1, 11);
    Serial.println("Card ID : " + text);
    sendLog(text);
  }
  if (reader.receiveQrCode(&qrCodeData, 100)) {
    Serial.println("Found QRCode");
    if (qrCodeData.valid) {
      Serial.print("Payload: ");
      Serial.println((const char *)qrCodeData.payload);
      sendLog((const char *)qrCodeData.payload);
    } else {
      Serial.print("Invalid: ");
      Serial.println((const char *)qrCodeData.payload);
    }
  }
  toggleConnStat();
  delay(1000);

  digitalWrite(SUCCESS_PIN, LOW);
  digitalWrite(ERROR_PIN, LOW);
}
