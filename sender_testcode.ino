//Die Zeile mit der LoRa-Message muss entsprechend nach Sensornummer angepasst werden
#include <SPI.h>
#include <LoRa.h>

int potentiometerPin = A0;  // Analog Pin A0 für das Potentiometer
int VENT = 5;               // PIN 5 für das öffnen und schließen des Ventils


void setup() {
  Serial.begin(1200);
  digitalWrite(VENT, LOW);
  pinMode(VENT, OUTPUT);

  while (!Serial);
  Serial.println("LoRa Sender");
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  int potValue = analogRead(potentiometerPin); // Liest den Wert des Potentiometers
  Serial.println(potValue); // Zeigt den Wert des Potentiometers im seriellen Monitor an
  int threshold = 200; // Definieren Sie einen Schwellenwert

  if (potValue > threshold) { // Wenn der Wert über dem Schwellenwert liegt  
    digitalWrite(VENT, HIGH); // VENT am Sender einschalten
    delay(5 * 1000); // Für 5 Sekunden warten

    // Sendet Signale für den Empfänger, um die Pumpe zu steuern
    LoRa.beginPacket();
    LoRa.print("SENSOR1DRY"); // Signal für den Empfänger, je nach Sensornummer anpassen
    LoRa.endPacket();
    
    delay(5 * 1000); // Für 5 Sekunden warten
    digitalWrite(VENT, LOW); // VENT am Sender ausschalten

    delay(5 * 1000); // Für 5 Sekunden warten

    }
  delay(500); // Warte 0.5 Sekunden vor der nächsten Messung
}
