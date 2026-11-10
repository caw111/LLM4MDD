#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_008\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
// Control D5 PWM LED with linear triangle wave brightness:
// 0 -> 255 over 1000ms, then 255 -> 0 over 1000ms
// Full period 2000ms, continuous looping.
// Initial brightness is 0.
// Update interval <= 20ms; brightness tolerance is +/- 5 PWM counts.

const uint8_t LED_PIN = 5; // D5
const uint32_t PERIOD_MS = 2000; // full cycle: 0->255 (1000ms) + 255->0 (1000ms)

#line 10 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_008\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 15 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_008\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 10 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_008\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, 0); // initial brightness 0
}

void loop() {
  static uint32_t startMs = millis();

  uint32_t now = millis();
  uint32_t t = (now - startMs) % PERIOD_MS;

  uint16_t brightness;
  if (t <= 1000) {
    // 0..1000 ms: 0 -> 255
    brightness = (uint32_t)t * 255u / 1000u;
  } else {
    // 1000..2000 ms: 255 -> 0
    uint32_t td = t - 1000; // 0..999
    brightness = 255u - (uint32_t)td * 255u / 1000u;
  }

  if (brightness > 255) brightness = 255;

  analogWrite(LED_PIN, (uint8_t)brightness);

  // Keep update interval <= 20ms
  delay(10);
}
