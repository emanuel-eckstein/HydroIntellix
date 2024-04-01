//Code für das HydoIntellix Gerät
//benötigte Bibliotheken
#include <SPI.h>
#include <LoRa.h>

// Analoger Pin A0 für den Sensor
int sensorPin = A0;  

// PIN 5 für das öffnen und schließen des Ventils
int VENT = 5;               



void setup() {
  Serial.begin(1200);
  digitalWrite(VENT, LOW);
  pinMode(VENT, OUTPUT);

  //hiermit kann über den Seriellen Monitor der Arduino IDE
  //ein Problem mit dem LoRa-Modul festgestellt werden
  Serial.println("LoRa Sender");
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
  }
}

void loop() {
  int potValue = analogRead(sensorPin); // Liest den Wert des Sensors
  // Zeigt den Wert des Sensors auf dem seriellen Monitor an
  Serial.println(potValue); 
  // Definierter Grenzwert
  int threshold = 200;
  
  //SX muss je nach Zone angepasst werden
  String message = "S1" + String(potValue);
  // jedes Messergebnis wird an das Pumpen-Kontrollzentrum gesendet 
  // und dort aufgezeichnet
  LoRa.beginPacket();
  LoRa.print(message);
  LoRa.endPacket(); 

  // Wenn der Wert unter dem Grenzwert liegt wird das Ventil geöffnet
  if (potValue < threshold) {   
    digitalWrite(VENT, HIGH); // öffnet das Ventil
    // 5 Sekunden Verzögerung, damit sich das Ventil vollständig 
    // öffnen kann
    delay(5 * 1000);          

    // Sendet Signale an das Pumpen-Kontrollzentrum, um die Pumpe zu 
    // starten. SX anpassen
    LoRa.beginPacket();
    LoRa.print("S1ON");
    LoRa.endPacket();
    
    delay(46UL * 60 * 1000); // Das Ventil steht für eine Bewässerungsperiode (46min) offen
    
    digitalWrite(VENT, LOW); // Das Ventil schließt sich dann wieder

    delay(1000); //Puffer

    }
  // Warte eine Sekunde statt 10 Minuten vor der 
  // nächsten Messung
  delay(10UL * 60 * 1000); 
}
