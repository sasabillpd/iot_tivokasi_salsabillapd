#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"


#define DHTPIN 27
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "Wokwi-GUEST";  // Ganti dengan WiFi Anda
const char* password = "";  // Ganti dengan password WiFi Anda

unsigned long previousMillis = 0;
const long interval = 5000;  // Interval 5 detik (5000 ms)

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Terhubung ke WiFi!");
  dht.begin();
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    float h = round(dht.readHumidity());
    float t = round(dht.readTemperature());
    if (isnan(h) || isnan(t)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }

    HTTPClient http;
    String url = "http://[YOUR_NGROK_URL]/api/posts"; // Ganti dengan URL ngrok yang benar
    http.begin(url);
    http.addHeader("Content-Type", "application/json");

    String payload = "{\"nama_sensor\":\"Sensor GD\", \"nilai1\":" + String(h) + ", \"nilai2\":" + String(t) + "}";
    int httpResponseCode = http.POST(payload);
    Serial.println("HTTP Response Code: " + String(httpResponseCode));

    http.end();
  }
}
