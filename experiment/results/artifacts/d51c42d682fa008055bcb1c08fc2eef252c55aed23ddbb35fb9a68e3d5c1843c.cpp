#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_027\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <DHT.h>

// DHT11 on D7
constexpr uint8_t DHT_PIN = 7;
constexpr uint8_t DHT_TYPE = DHT11;

DHT dht(DHT_PIN, DHT_TYPE);

#line 9 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_027\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 14 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_027\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 9 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_027\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  static unsigned long lastReadMs = 0;
  const unsigned long intervalMs = 5000;

  unsigned long now = millis();
  if (now - lastReadMs >= intervalMs) {
    lastReadMs = now;

    float tempC = dht.readTemperature(); // Celsius
    if (isnan(tempC)) {
      Serial.println("NaN");
    } else {
      Serial.println(tempC);
    }
  }
}
