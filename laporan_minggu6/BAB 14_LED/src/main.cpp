#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define DHTPIN 27
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "sasbillpd";
const char* password = "12345678";

unsigned long previousMillis = 0;
const long interval = 5000;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println(" Terhubung ke WiFi!");
  dht.begin();
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    float h = round(dht.readHumidity());
    float t = round(dht.readTemperature());

    if (isnan(h) || isnan(t)) {
      Serial.println("Gagal membaca dari sensor DHT!");
      return;
    }

    HTTPClient http;
    String url = "http://your-ngrok-url.ngrok-free.app/api/posts";
    http.begin(url);
    http.addHeader("Content-Type", "application/json");

    String payload = "{\"nama_sensor\":\"Sensor GD\", \"nilai1\":" + String(h) + ", \"nilai2\":" + String(t) + "}";
    int httpResponseCode = http.POST(payload);

    Serial.print("Kode respons HTTP: ");
    Serial.println(httpResponseCode);
    if (httpResponseCode > 0) {
      Serial.println(http.getString());
    }

    http.end();
  }
}
