#include <WiFi.h>

const char* ssid = "ESP32_AP";         // SSID of the ESP32 acting as AP
const char* password = "12345678";     // Password of the ESP32 AP
const char* stm32IP = "192.168.4.1";   // IP of the ESP32 AP
const int port = 1234;                 // TCP port the server is listening on
const int acs771Pin = A0;  // Pin where the ACS771 output is connected
float voltage = 0.0;
float current = 0.0;
const float sensitivity = 0.066;  // Sensitivity of ACS771 (66 mV/A for the 20A version)
const float offsetVoltage = 2.5;
WiFiClient client;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);  // Connect to ESP32 AP

  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi Connected!");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Try to connect to server if not already connected
   int sensorValue = (analogRead(acs771Pin));
   Serial.println(sensorValue);
  voltage = (sensorValue * (3.3 / 4096));
  Serial.println(voltage);
  current = ((voltage - offsetVoltage) / sensitivity);
  Serial.println(current);
  if (!client.connected()) {
    Serial.println("Connecting to ESP32 Server...");
    if (client.connect(stm32IP, port)) {
      Serial.println("Connected to ESP32 Server!");
    } else {
      Serial.println("Connection Failed!");
      delay(2000);
      return;  // Try again in next loop
    }
  }

  // If connected, send message
  client.print( "current:");
   client.print( current);
  Serial.println("Message sent!");

  delay(5000);  // Wait before sending the next message
}
