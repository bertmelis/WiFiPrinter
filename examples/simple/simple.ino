#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiPrinter.h>

const char* ssid = "........";
const char* password = "........";

void setup() {
  Serial.begin(74880);
  delay(5000);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }

  WiFiPrinter.begin();

  Serial.print("Ready for connections on ");
  Serial.print(WiFi.localIP());
  Serial.print(" - port 23\n");
}

void loop() {
  static unsigned long lastMillis = 0;
  if (millis() - lastMillis > 60 * 1000UL) {
    lastMillis = millis();
    Serial.printf("%lu- current free heap: %lu\n", lastMillis, ESP.getFreeHeap());
    WiFiPrinter.printf("%lu- current free heap: %lu\n", lastMillis, ESP.getFreeHeap());
  }
  WiFiPrinter.handle();
}
