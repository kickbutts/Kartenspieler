/*
 * Pin–Verbindungen
 * --------------------------------------------------------------------------------------------------------
 *             MFRC522      ESP8266   ESP32
 *             Reader/PCD   
 * Signal      Pin          Pin/GPIO   Pin
 * --------------------------------------------------------------------------------------------------------
 * RST/Reset   RST          D3/0       22
 * SPI SS      SDA(SS)      D8/15      21
 * SPI MOSI    MOSI         D7/13      23
 * SPI MISO    MISO         D6/12      19 
 * SPI SCK     SCK          D5/14      18
 */ 

#include <SPI.h>
#include <MFRC522.h>

#ifdef ESP8266 //Konfiguration für den ESP8266
#include <ESP8266WiFi.h>
#define RST_PIN   0  
#define SS_PIN    15  
#elif defined ESP32 //Konfiguration für den ESP32
#include <WiFi.h>
#define RST_PIN   22     
#define SS_PIN    21  
#endif
#include <PubSubClient.h>

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Instanz des MFRC522 erzeugen

//Wifi Setup
const char* ssid     = ""; //SSID einfügen
const char* password = ""; //Kennwort einfügen

//MQTT Setup
const char* mqttServer = ""; //IP Adresse des MQTT Server 
const int mqttPort = 1883; //Port des MQTT Server 
const char* mqttUser = ""; //Benutzername des MQTT Servers 
const char* mqttPassword = ""; //MQTT Passwort 
 
const char* mqttTopic = ""; //MQTT Topic 
const char* mqttClientName = ""; //MQTT Client Name


void setup() {
  // Diese Funktion wird einmalig beim Start ausgeführt
  Serial.begin(115200);  // Serielle Kommunikation mit dem PC initialisieren
  Serial.println("NFC-Leser");
  SPI.begin();         // Initialisiere SPI Kommunikation
  mfrc522.PCD_Init();  // Initialisiere MFRC522 Lesemodul
}
  WiFiClient client;
  PubSubClient mqttclient(client);


String printHex(byte *buffer, byte bufferSize) {
  String id = "";
  for (byte i = 0; i < bufferSize; i++) {
    id += buffer[i] < 0x10 ? "0" : "";
    id += String(buffer[i], HEX);
  }
  return id;
}

void loop() {
  // Diese Funktion wird in einer Endlosschleife ausgeführt.
  // Nur wenn einfolgreich eine Karte gelesen werden konnte, wird die IF Anweisung ausgeführt
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial() ) {
    Serial.print("Gelesene UID: ");
    for (byte i = 0; i < mfrc522.uid.size; i++) {

      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      Serial.print(mfrc522.uid.uidByte[i], HEX);
    }
    Serial.println();
    Serial.println("Die gelesene UID ist: " + printHex(mfrc522.uid.uidByte, mfrc522.uid.size));
    String CardID=printHex(mfrc522.uid.uidByte, mfrc522.uid.size);
    Serial.println();

 //Verbindung mit dem Wlan herstellen
  // Verbindung öffnen

    Serial.println();
    Serial.println();
    Serial.print("Verbinde mit ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WLAN verbunden");
    Serial.println("IP-Addresse: ");
    Serial.println(WiFi.localIP());

    // Sendung mqtt Nachricht

    mqttclient.setServer(mqttServer, mqttPort);
    while (!client.connected()) {
      Serial.println("Verbinde mit MQTT...");

    if (mqttclient.connect(mqttClientName, mqttUser, mqttPassword )) {

      Serial.println("verbunden");

    } else {

      Serial.print("MQTT-Verbindung fehlgeschlagen. Status: ");
      Serial.println(mqttclient.state());
      delay(2000);

    }

    }
    mqttclient.publish(mqttTopic, (char*) CardID.c_str());
    Serial.println(F("\n**MQTT-Nachricht gesendet**\n"));

    mfrc522.PICC_HaltA();
    delay(1000);
  }
}
