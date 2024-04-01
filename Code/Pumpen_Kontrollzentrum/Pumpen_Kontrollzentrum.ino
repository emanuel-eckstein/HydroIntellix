#include <SPI.h>
#include <LoRa.h>
#include <SD.h> // SD-Karten-Bibliothek hinzufügen

int LED = 4; // PIN 4 für die LED am Empfänger
const int chipSelect = 10; // Chip-Select-Pin für die SD-Karte, anpassen je nach Schaltung

void setup() {
  Serial.begin(1200);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  
  while (!Serial);
  Serial.println("LoRa Receiver");

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  
  // Initialisierung der SD-Karte
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // Schleife stoppt, wenn keine SD-Karte gefunden wurde
    while (1);
  }
  Serial.println("SD card initialized.");
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String receivedMsg = "";
    while (LoRa.available()) {
      receivedMsg += (char)LoRa.read();
    }
    
    // Hier überprüfen wir, ob die Nachricht mit "S" beginnt und eine Zahl enthält
    if (receivedMsg.startsWith("S") && receivedMsg.substring(1).toInt() > 0) {
      digitalWrite(LED, HIGH); // LED am Empfänger einschalten
      delay(45UL * 60 * 1000); // Für 5 Sekunden leuchten lassen
      digitalWrite(LED, LOW); // LED am Empfänger ausschalten
      
      // Öffnen der Datei auf der SD-Karte zum Schreiben
      File dataFile = SD.open("data.txt", FILE_WRITE);
      
      // Wenn die Datei geöffnet ist, schreiben wir die Daten hinein
      if (dataFile) {
        dataFile.println(receivedMsg);
        dataFile.close();
        Serial.println("Data written to SD card.");
      }
      else {
        Serial.println("Error opening data.txt");
      }
    }
  }
  // Keine zusätzliche Verzögerung hier, da die Verzögerung im Sender gesteuert wird
}
