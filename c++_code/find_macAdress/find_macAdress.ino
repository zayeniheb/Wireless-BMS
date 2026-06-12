#include "WiFi.h"

void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
WiFi.mode(WIFI_MODE_STA);
delay(1000);
Serial.println(WiFi.macAddress());
Serial.println(WiFi.softAPIP());
}

void loop() {
  // put your main code here, to run repeatedly:

}
