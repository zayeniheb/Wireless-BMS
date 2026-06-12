#include <WiFi.h>

void setup() {
  Serial.begin(115200);

  // Initialize Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(); // This is optional, it can be skipped

  // Wait for the Wi-Fi module to initialize
  delay(1000);

  // Get and print the MAC address
  String macAddress = WiFi.macAddress();
  Serial.print("MAC Address: ");
  Serial.println(macAddress);
}

void loop() {
  // Do nothing here
}
