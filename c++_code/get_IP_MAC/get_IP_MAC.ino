#include <WiFi.h>

void setup() {
    Serial.begin(115200);
    
    // Start ESP32 in AP mode
    WiFi.softAP("ESP32_AP", "12345678");
    
    // Fetch the MAC address of the AP interface
  Serial.println(WiFi.softAPIP());

     Serial.println(  WiFi.softAPmacAddress());
    // Print the MAC address
    
}

void loop() {
    // Nothing to do here
}
