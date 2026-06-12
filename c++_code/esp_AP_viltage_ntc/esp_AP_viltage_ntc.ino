#include <WiFi.h>
#include "ADS1X15.h"
#include <Arduino.h>

#define PORT 1234
#define MAX_CLIENTS 2
#define NTC_PIN 34 
#define SERIES_RESISTOR 10000  
#define NOMINAL_RESISTANCE 10000 
#define NOMINAL_TEMPERATURE 25.0 
#define BETA_COEFFICIENT 2600 
#define ADC_MAX 4095 
#define VREF 3.3  

ADS1115 ADS(0x48);
const char* ssid = "ESP32_AP";
const char* password = "12345678";

WiFiServer server(PORT);
WiFiClient clients[MAX_CLIENTS];  // Array to track multiple clients

void setup() {
  Serial.begin(115200);
 Wire.begin();
  ADS.begin();         // Initialisation du module ADS1115
  ADS.setGain(0);      // On prend le gain le plus bas (index 0), pour avoir la plus grande plage de mesure (6.144 volt)
  ADS.setMode(1);      // On indique à l'ADC qu'on fera des mesures à la demande, et non en continu (0 = CONTINUOUS, 1 = SINGLE)
  ADS.setDataRate(7);  // On spécifie la vitesse de mesure de tension qu'on souhaite, allant de 0 à 7 (7 étant le plus rapide, soit 860 échantillons par seconde)
  ADS.readADC(0); 
  WiFi.softAP(ssid, password, 1, 0, MAX_CLIENTS);  // Set max clients
  Serial.print("Access Point started: ");
  Serial.println(WiFi.softAPIP());

  server.begin();
  Serial.println("Server started. Waiting for connections...");
}

void loop() {
  // Accept new clients
   int adc_value = analogRead(NTC_PIN);  
    float voltage = adc_value * (VREF / ADC_MAX);
     float temperature = (1.0 / ((log(SERIES_RESISTOR * (VREF / voltage - 1) / NOMINAL_RESISTANCE) / BETA_COEFFICIENT) + (1.0 / (NOMINAL_TEMPERATURE + 273.15)))) - 273.15;
  int16_t difference_potentiel_A0_A1 = ADS.readADC_Differential_0_1();
    float tension_volts_A0_A1 = ADS.toVoltage(difference_potentiel_A0_A1);
   
 
  WiFiClient newClient = server.available();
  
  if (newClient) {
    bool added = false;
    for (int i = 0; i < MAX_CLIENTS; i++) {
      if (!clients[i] || !clients[i].connected()) {
        clients[i] = newClient;
        Serial.print("New client added at index ");
        Serial.println(i);
        added = true;
        break;
      }
    }
    if (!added) {
      Serial.println("Too many clients. Connection rejected.");
      newClient.stop();
    }
  }

  // Handle each connected client
  for (int i = 0; i < MAX_CLIENTS; i++) {
    if (clients[i] && clients[i].connected()) {
      // Send message
        clients[0].print("tension:");
           clients[0].println(tension_volts_A0_A1);
            clients[0].print("temperature:");
  clients[0].println(temperature);
  clients[0].println("IP");
    clients[0].println( WiFi.softAPIP());
    clients[0].println("adresse MAC");
    clients[0].println( WiFi.softAPmacAddress());

      // Receive message
      /*if (clients[i].available()) {
        char rx_buffer[128];
        int len = clients[i].readBytes(rx_buffer, sizeof(rx_buffer) - 1);
        if (len > 0) {
          rx_buffer[len] = '\0';
          Serial.print("Client ");
          Serial.print(i);
          Serial.print(" says: ");
          Serial.println(rx_buffer);

          // Echo it back
          clients[0].println(rx_buffer)*/
        
          
        }
      }
    } else {
      // Clean up disconnected clients
      if (clients[i]) {
        clients[i].stop();
        clients[i] = WiFiClient();  // Reset
        Serial.print("Client ");
        Serial.print(i);
        Serial.println(" disconnected.");
      }
    }
  }

  delay(1000);  // Loop delay
}
