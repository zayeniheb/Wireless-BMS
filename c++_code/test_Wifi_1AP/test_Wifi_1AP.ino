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
const char* ssid = "ESP32_AP1";
const char* password = "12345678";
const int acs771Pin = A17;  // Pin where the ACS771 output is connected
float voltage = 0.0;
float current = 0.0;
float vol=0.0;
const float sensitivity = 0.066;  // Sensitivity of ACS771 (66 mV/A for the 30A version)(V/mA)
const float offsetVoltage = 1.65;
 int num_samples=1000;
  int adc_offset;
WiFiServer server(PORT);
WiFiClient clients[MAX_CLIENTS];  // Array to track multiple clients

void setup() {
  Serial.begin(115200);
  //calibrateadc();
 
 Wire.begin();
  ADS.begin();         // Initialisation du module ADS1115
  ADS.setGain(0);      // On prend le gain le plus bas (index 0), pour avoir la plus grande plage de mesure (6.144 volt)
  ADS.setMode(1);      // On indique à l'ADC qu'on fera des mesures à la demande, et non en continu (0 = CONTINUOUS, 1 = SINGLE)
  ADS.setDataRate(7);  // On spécifie la vitesse de mesure de tension qu'on souhaite, allant de 0 à 7 (7 étant le plus rapide, soit 860 échantillons par seconde)
  ADS.readADC(0); 
  WiFi.softAP(ssid, password, 1, 0, MAX_CLIENTS); // Set max clients

  Serial.print("Access Point started: ");
  Serial.println(WiFi.softAPIP());

  server.begin();
     
  Serial.println("Server started. Waiting for connections...");
}

 

void loop() {
  
  /*int rawAdc = analogRead(acs771Pin);
float volt = rawAdc  * (3.3/ 4095.0);
float current = (volt - vol) / sensitivity;
Serial.print("raw=");
Serial.println(rawAdc);
Serial.print("vol=");
Serial.println(vol);
Serial.print("volt=");
Serial.println(volt);
Serial.print("current=");
Serial.println(current,1);
delay(1000);*/


   int adc_value = analogRead(NTC_PIN);  
    float voltage = adc_value * (VREF / ADC_MAX);
     float temperature = (1.0 / ((log(SERIES_RESISTOR * (VREF / voltage - 1) / NOMINAL_RESISTANCE) / BETA_COEFFICIENT) + (1.0 / (NOMINAL_TEMPERATURE + 273.15)))) - 273.15;
  int16_t difference_potentiel_A0_A1 = ADS.readADC_Differential_0_1();
    float tension_volts_A0_A1 = ADS.toVoltage(difference_potentiel_A0_A1);
   // int sensorValue1 = (analogRead(acs771Pin)) ;
     //int sensorValue=(sensorValue1*3)/5;
    //sensorValue-=adc_offset ;
//float vol = (sensorValue * ((3.3) / 4096));
 
  // Serial.println(sensorValue); 
    //Serial.println( vol); 
   //Serial.println(current);    
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
        clients[0].print("IP:");
    clients[0].println( WiFi.softAPIP());
    clients[0].print("adresse MAC:");
    clients[0].println( WiFi.softAPmacAddress());
        clients[0].print("tension:");
           clients[0].println(tension_volts_A0_A1);
            clients[0].print("temperature:");
  clients[0].println(temperature);
   //clients[0].print("current:");
  //clients[0].println(current);


      // Receive message
     /* if (clients[i].available()) {
        char rx_buffer[128];
        int len = clients[i].readBytes(rx_buffer, sizeof(rx_buffer) - 1);
        if (len > 0) {
          rx_buffer[len] = '\0';
          Serial.print("Client ");
          Serial.print(i);
          Serial.print(" says: ");
          Serial.println(rx_buffer);

          // Echo it back
          clients[0].println(rx_buffer)
        
          
        }*/
    }
     else {
      // Clean up disconnected clients
      if (clients[i]) {
        clients[i].stop();
        clients[i] = WiFiClient();  // Reset
        Serial.print("Client ");
        Serial.print(i);
        Serial.println(" disconnected.");
      }
    }
  

  delay(1000);  // Loop delay
}
/*void calibrateadc() {
  Serial.println("Calibrating ADC (zero current baseline)...");
  
  long adc_sum = 0;
  for (int i = 0; i < num_samples; i++) {
    int adc = analogRead(acs771Pin); 
    adc_sum += adc;
    delay(5); // shorter delay for faster calibration
  }

  adc_offset = adc_sum / num_samples;
  Serial.print("Calibrated ADC Offset: ");
  Serial.println(adc_offset);
  vol =  adc_offset * (3.3/ 4095.0);
   Serial.print("vol ADC Offset: ");
  Serial.println(vol);*/

    }
