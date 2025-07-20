#include <dht_nonblocking.h>

#define DHT_SENSOR_TYPE DHT_TYPE_11
#define DHT_SENSOR_PIN 4

DHT_nonblocking dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);
float temperature;
float humidity;

void setup() {
  Serial.begin(9600);
}

void loop() {
  static unsigned long lastMeasurement = 0;

  if (millis() - lastMeasurement > 500000) { 
    if (dht_sensor.measure(&temperature, &humidity)) {
      unsigned long time_sec = millis() / 1000;  


      Serial.print(temperature);
      Serial.print(",");
      Serial.println(humidity);
    }
    lastMeasurement = millis();
  }
}
