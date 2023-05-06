// Copyright 2023
// Author: Mehdi Bouzhuenda
/*******************************************************
**PROGRAM NAME -The program utilizes PHP, ESP32CAM, and RDM6300 to send QR code and RFID data logs to a database."

**PROGRAMMER: - MEHDI BOUZGUENDA - https://github.com/mehdibouzguenda


**DATE: - Started 02/06/2023
       

**BUGS: -  I am currently in the process of optimizing this program!


**DESCRIPTION - This program incorporates two primary libraries that were sourced from GitHub. (MODIFIED)
    * https://github.com/alvarowolfx/ESP32QRCodeReader
    * https://github.com/arduino12/rdm6300 (NEW LIBRARY )
    - Also We need to add esp to our Arduino IDE(the editor that i used).
      1- Open up your IDE then go to “File -> Preferences” or simply hit “Ctrl + comma”
      2- Paste https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json  in additional board manager URLs.
      3- Open the Boards Manager. Go to Tools > Board > Boards Manager… 
      4- Search for ESP32 and press install button for the “ESP32 by Espressif Systems“:
    - DONT FORGET to modify your SSID and PASSWORD IN THIS CODE.
    (I tested this program by running it on a local server using XAMPP.)
    - create database name: “QR_RFID_DATA_LOG”
    - import the database QR_RFID_DATA_LOG.sql
*******************************************************/

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ESP32QRCodeReader.h>
#include <rdm6300.h>

const char *ssid = "Orange-E58F";      //WIFI NAME OR HOTSPOT //Orange-E58F
const char *password = "L2RhgnaDTDn";  //WIFI PASSWORD POR MOBILE HOTSPOT PASSWORD//L2RhgnaDTDn


ESP32QRCodeReader reader(CAMERA_MODEL_AI_THINKER);
struct QRCodeData qrCodeData;
bool isConnected = false;


#define RDM6300_RX_PIN 13  // read the SoftwareSerial doc above! may need to change this pin ...
Rdm6300 rdm6300;


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
  rdm6300.begin(RDM6300_RX_PIN);

  Serial.println("\nPlace RFID tag near the rdm6300...");
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

void loop() {

	if (rdm6300.get_new_tag_id()){
    Serial.println(rdm6300.get_tag_id(), HEX);
    delay(1000);
    //sendLog(rdm6300.get_tag_id());
  }
  if (reader.receiveQrCode(&qrCodeData, 100)) {
    Serial.println("Found QRCode");
    if (qrCodeData.valid) {
      Serial.print("Payload: ");
      Serial.println((const char *)qrCodeData.payload);
      delay(1000);
      //sendLog((const char *)qrCodeData.payload);
    } else {
      Serial.print("Invalid: ");
      Serial.println((const char *)qrCodeData.payload);
    }
  }
  toggleConnStat();
  delay(100);

  digitalWrite(SUCCESS_PIN, LOW);
  digitalWrite(ERROR_PIN, LOW);
}
