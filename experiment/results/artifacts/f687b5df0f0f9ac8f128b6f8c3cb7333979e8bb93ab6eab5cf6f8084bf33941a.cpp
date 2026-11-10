#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_temperature_sensor\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

const float threshold = 30.0f;

#line 11 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_temperature_sensor\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 18 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_temperature_sensor\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 11 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_temperature_sensor\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  Serial.begin(9600);
  // Declare the threshold value
  Serial.println(String("Threshold: ") + threshold);
  dht.begin();
}

void loop() {
  // Continuously read temperature and compare against threshold
  float t = dht.readTemperature(); // in °C

  if (isnan(t)) {
    // If sensor read fails, try again on next loop iteration
    return;
  }

  if (t < threshold) {
    Serial.println("Temperature is below the threshold");
  } else if (t > threshold) {
    Serial.println("Temperature is above the threshold");
  } else {
    // Preserve compare behavior: above vs below only
    Serial.println("Temperature is above the threshold");
  }

  // DHT sensors should not be polled too quickly
  delay(2000);
}
