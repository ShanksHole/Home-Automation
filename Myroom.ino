#define BLYNK_TEMPLATE_ID "TMPL3dtggd1h1"
#define BLYNK_TEMPLATE_NAME "Home"
#define BLYNK_AUTH_TOKEN "wl6zcGhcZgovQ7UYB99wqEBWPZf55vyk"
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <WiFiManager.h> // WiFi AutoConnect Library

char auth[] = BLYNK_AUTH_TOKEN;

// Pin Mapping for ESP8266 (NodeMCU)
const int pins[] = {D1, D2};
// GPIO Mapping: D1 (GPIO5), D2 (GPIO4)
const int pinCount = 2;

void setup() {
  Serial.begin(115200);

  // Initialize all pins as OUTPUT
  for (int i = 0; i < pinCount; i++) {
    pinMode(pins[i], OUTPUT);
    digitalWrite(pins[i], LOW); // Ensure all outputs start LOW
  }

  Serial.println("ESP8266 Home Automation System Starting...");
  Serial.println("Type 'reset' in Serial Monitor to clear Wi-Fi settings.");

  // Check for user "reset" input in Serial Monitor
  long startTime = millis();
  while (millis() - startTime < 5000) { // Wait for 5 seconds
    if (Serial.available()) {
      String command = Serial.readStringUntil('\n');
      command.trim();
      if (command == "reset") {
        Serial.println("Resetting Wi-Fi settings...");
        WiFiManager wifiManager;
        wifiManager.resetSettings();
        delay(1000);
        ESP.restart();
      }
    }
  }

  // Normal Wi-Fi auto-connect
  WiFiManager wifiManager;
  wifiManager.autoConnect("ESP8266-Setup");
  Serial.println("Connected to Wi-Fi");

  Blynk.begin(auth, WiFi.SSID().c_str(), WiFi.psk().c_str());
  Serial.println("Connected to Blynk Cloud");
}

// Blynk Write Handlers
BLYNK_WRITE(V0) { togglePin(0, param.asInt()); } // D1 (GPIO5)
BLYNK_WRITE(V1) { togglePin(1, param.asInt()); } // D2 (GPIO4)

void togglePin(int index, int state) {
  digitalWrite(pins[index], state ? HIGH : LOW);
  Serial.print("Pin ");
  Serial.print(pins[index]);
  Serial.print(" (GPIO");
  Serial.print(getGPIOFromD(pins[index]));
  Serial.print(") set to ");
  Serial.println(state ? "HIGH" : "LOW");
}

// Helper function to map D pins to GPIO numbers
int getGPIOFromD(int dPin) {
  switch (dPin) {
    case D1: return 5;
    case D2: return 4;
    default: return -1;
  }
}

void loop() {
  Blynk.run();
}
