# Kartenspieler
## Hörspiel-Player für Kinder verdrahtet mit Node-Red

**Repository zum c't Artikel Kartenspieler (c't 2020, Heft 25)**

Das Programm zum lesen von NFC Tags läuft auf einem ESP 8266 oder ESP32. Die Tags sind mit den jeweiligen Covern der Hörspiele bedruckt. Per NFC Reader (Modell RC522) wird die UID der NFC-Karte ausgelesen und per MQTT verschickt. NodeRed empfängt die MQTT Nachricht sucht in einem KeyValue Store die damit verknüpfte Playlist heraus und spielt diese auf einem Sonos Lautsprecher ab. Die Playlists und MP3s sind auf einem NAS im lokalen Netz gespeichert.
Es gibt eine GUI um Karten hinzuzufügen und alle Karten aus dem KeyValue Store zu löschen.

Das System ist bewusst einfach gehalten. So lässt es sich problemlos auch von Kinder die noch nicht lesen können bedienen.

Verwendete Node-Red Pakete:
* node-red-contrib-sonos-plus
* node-red-contrib-key-value-store
* node-red-contrib-tableify

Verwendete Libarys für ESP32:
* SPI.h
* MFRC522.h
* WiFi.h
* PubSubClient.h
