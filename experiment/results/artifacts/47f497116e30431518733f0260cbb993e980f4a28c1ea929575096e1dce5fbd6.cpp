#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_001\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const uint8_t PWM_LED_PIN = 9;     // D9 PWM LED
const uint8_t BUZZER_PIN   = 6;     // D6 buzzer (short鸣提示)
const uint8_t SENSOR_PIN   = A0;    // A0 light sensor (LDR)

const int THRESHOLD_STRONG_LIGHT = 800; // >800 时提示

// Minimal PWM response: 光线越强 LED 越暗
// We'll map ADC (0..1023) -> PWM (255..0)
#line 11 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_001\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t lightToLedPWM(int adc);
#line 18 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_001\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 27 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_001\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 11 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_001\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static inline uint8_t lightToLedPWM(int adc) {
  adc = constrain(adc, 0, 1023);
  // 光线越强 -> LED 越暗
  long pwm = map(adc, 0, 1023, 255, 0);
  return (uint8_t)constrain(pwm, 0, 255);
}

void setup() {
  pinMode(PWM_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(SENSOR_PIN, INPUT);

  analogWrite(PWM_LED_PIN, 255); // default bright
  digitalWrite(BUZZER_PIN, LOW);
}

void loop() {
  int adc = analogRead(SENSOR_PIN);

  uint8_t ledPWM = lightToLedPWM(adc);
  analogWrite(PWM_LED_PIN, ledPWM);

  // 当光线极强时（>800）蜂鸣器（D6）短鸣提示
  if (adc > THRESHOLD_STRONG_LIGHT) {
    // short鸣：ON for a brief moment, then OFF
    digitalWrite(BUZZER_PIN, HIGH);
    delay(80);
    digitalWrite(BUZZER_PIN, LOW);
    delay(20);
  } else {
    digitalWrite(BUZZER_PIN, LOW);
    delay(50);
  }
}
