#include <WiFi.h>
#include <lwip/sockets.h>
#include "ADS1X15.h"

ADS1115 ADS(0x48); 
#define PORT 1234
#define BUF_SIZE 1024

const char* ssid = "ESP32_AP";          // Wi-Fi SSID
const char* password = "12345678";      // Wi-Fi Password

WiFiServer server(PORT);
#include <Arduino.h>

#define NTC_PIN 34 
#define SERIES_RESISTOR 10000  
#define NOMINAL_RESISTANCE 10000 
#define NOMINAL_TEMPERATURE 25.0 
#define BETA_COEFFICIENT 2600 
#define ADC_MAX 4095 
#define VREF 3.3 

void setup() {
    Serial.begin(115200);

    // Initialize Wi-Fi as Access Point
    WiFi.softAP(ssid, password);
    Serial.print("Access Point started: ");
    Serial.println(ssid);
  Wire.begin();
  ADS.begin();         // Initialisation du module ADS1115
  ADS.setGain(0);      // On prend le gain le plus bas (index 0), pour avoir la plus grande plage de mesure (6.144 volt)
  ADS.setMode(1);      // On indique à l'ADC qu'on fera des mesures à la demande, et non en continu (0 = CONTINUOUS, 1 = SINGLE)
  ADS.setDataRate(7);  // On spécifie la vitesse de mesure de tension qu'on souhaite, allant de 0 à 7 (7 étant le plus rapide, soit 860 échantillons par seconde)
  ADS.readADC(0);
    // Start Server
    server.begin();
    Serial.println("Server started. Waiting for connections...");
}

void loop() {
    // Check for a new client connection
    WiFiClient client = server.available();
    

    if (client) {
        Serial.println("ESP_B connected!");
      ;
        //if (tension_volts_A0_A1>=2){
        while(1){
          
           int adc_value = analogRead(NTC_PIN);  
    float voltage = adc_value * (VREF / ADC_MAX);
        
            int16_t difference_potentiel_A0_A1 = ADS.readADC_Differential_0_1();
      float tension_volts_A0_A1 = ADS.toVoltage(difference_potentiel_A0_A1);
       float temperature = (1.0 / ((log(SERIES_RESISTOR * (VREF / voltage - 1) / NOMINAL_RESISTANCE) / BETA_COEFFICIENT) + (1.0 / (NOMINAL_TEMPERATURE + 273.15)))) - 273.15;
      /*if(tension_volts_A0_A1>=2&&temperature<60){
      client.print("tension:");
      client.println(tension_volts_A0_A1);
      delay(2000);
      client.print("temperature:");
      client.println(temperature);
      delay(2000);}
      else if(tension_volts_A0_A1<2){
      client.println("batterie faible");
      delay(2000);
      client.print("temperature:");
      client.println(temperature);
      delay(2000);
      }
      else if(temperature>60){
         client.print("tension:");
      client.println(tension_volts_A0_A1);
      delay(2000);
      client.println("batterie chaude");
        delay(2000);
      }
      else{client.println("erreur");
        }}
      }}
      //else {client.println("batterie faible");}}
      
        //delay(5000);*/}

        // Read message from client
        /*char rx_buffer[128];
        int len = client.readBytes(rx_buffer, sizeof(rx_buffer) - 1);
        if (len > 0) {
            rx_buffer[len] = '\0';  // Null-terminate the received data
            Serial.print("Received: ");
            Serial.println(rx_buffer);
        } else {
            Serial.println("Receive failed.");
        }

        client.stop();  // Close connection
        Serial.println("Client disconnected.");*/
            // Wait before accepting another connection
    }}
