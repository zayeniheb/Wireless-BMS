#include <WiFi.h>

#define PORT 1234
#define MAX_CLIENTS 1

const char* ssid1 = "ESP32_AP1";
const char* password = "12345678";

WiFiServer server(PORT);
WiFiClient clients[MAX_CLIENTS];  // Array to track multiple clients

void setup() {
  Serial.begin(115200);

  WiFi.softAP(ssid1, password, 1, 0, MAX_CLIENTS);  // Set max clients
  Serial.print("Access Point started: ");
  Serial.println(WiFi.softAPIP());

  server.begin();
  Serial.println("Server started. Waiting for connections...");
}

void loop() {
  // Accept new clients
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
      
  clients[0].println("haaaw wejhyyy1");
      // Receive message
      if (clients[i].available()) {
        char rx_buffer[128];
        int len = clients[i].readBytes(rx_buffer, sizeof(rx_buffer) - 1);
        if (len > 0) {
          rx_buffer[len] = '\0';
          Serial.print("Client ");
          Serial.print(i);
          Serial.print(" says: ");
          Serial.println(rx_buffer);

          // Echo it back
          //clients[0].println(rx_buffer);
        
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
