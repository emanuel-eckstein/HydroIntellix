//In diesem Code werden die Pumpenrelais vorerst durch LED's simuliert. Es empfielt sich, ein Relais stets mit einem Transistor zu schalten, 
//da auch die Relaisströme zu hoch für einen Arduino sein können. Dieser Testcode ist für die Nachrichten SENSOR{1/2/3}DRY konfiguriert. 
//Hier erfolgt keine Aufzeichnung der Messungen der Sensoren
#include <SPI.h>
#include <LoRa.h>
int LED = 4; // PIN 4 für die LED am Empfänger

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
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String receivedMsg = "";
    while (LoRa.available()) {
      receivedMsg += (char)LoRa.read();
    }
    if (receivedMsg == "S1ON" || receivedMsg == "S2ON" || receivedMsg == "S3ON") {
      digitalWrite(LED, HIGH); // LED am Empfänger einschalten
      delay(5000); // Für 5 Sekunden leuchten lassen
      digitalWrite(LED, LOW); // LED am Empfänger ausschalten
    }
  }
  // Keine zusätzliche Verzögerung hier, da die Verzögerung im Sender gesteuert wird
}
