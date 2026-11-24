// Temperature alert: read a DHT22 every 2 s, drive an LED from the reading.
// Board: ESP32 DevKit   Sensor: DHT22 on GPIO4   Output: LED on GPIO18
#include <DHT.h>

const int DHT_PIN = 4;
const int LED_PIN = 18;

const unsigned long READ_INTERVAL_MS = 2000;  // DHT22 needs >= 2 s between reads
const float TEMP_ON_C  = 30.0;                // LED turns ON above this
const float TEMP_OFF_C = 28.0;                // LED turns OFF below this (hysteresis)

DHT dht(DHT_PIN, DHT22);
unsigned long lastRead = 0;
bool ledOn = false;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  dht.begin();
  Serial.println("Temperature alert started");
}

void loop() {
  if (millis() - lastRead < READ_INTERVAL_MS) return;
  lastRead = millis();

  float t = dht.readTemperature();  // degrees Celsius
  if (isnan(t)) {
    Serial.println("Sensor read failed - keeping previous LED state");
    return;
  }

  // Hysteresis stops the LED flickering when temp hovers near the threshold
  if (!ledOn && t > TEMP_ON_C)  ledOn = true;
  if (ledOn  && t < TEMP_OFF_C) ledOn = false;
  digitalWrite(LED_PIN, ledOn ? HIGH : LOW);

  Serial.printf("Temp: %.1f C  ->  LED %s\n", t, ledOn ? "ON" : "OFF");
}
