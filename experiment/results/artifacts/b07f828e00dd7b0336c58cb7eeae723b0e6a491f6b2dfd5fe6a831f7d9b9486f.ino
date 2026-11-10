#include <Arduino.h>

static const uint8_t PWM_LED_PIN = 9;   // D9 PWM LED
static const uint8_t BUZZER_PIN   = 6;   // D6 buzzer short鸣
static const uint8_t LIGHT_PIN    = A0;  // A0 光敏电阻

// Threshold: 当光线极强时（>800）蜂鸣器（D6）短鸣提示
static const int LIGHT_THRESHOLD = 800;

// Simple calibration/behavior parameters
// LED brightness control: 光线越强 LED 越暗
// Map A0 reading (0..1023) to PWM (255..0)
static const uint8_t PWM_MIN = 0;
static const uint8_t PWM_MAX = 255;

// Short鸣 duration (ms)
static const unsigned int BUZZ_SHORT_MS = 80;

void setup() {
  pinMode(PWM_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LIGHT_PIN, INPUT);

  digitalWrite(BUZZER_PIN, LOW);
  analogWrite(PWM_LED_PIN, PWM_MAX); // initial: assume dark -> brighter
}

void loop() {
  int lightVal = analogRead(LIGHT_PIN); // 0..1023

  // 光线越强 LED 越暗
  // Invert mapping: higher lightVal => lower PWM
  long pwm = (long)(PWM_MAX) * (1023L - lightVal) / 1023L;
  if (pwm < PWM_MIN) pwm = PWM_MIN;
  if (pwm > PWM_MAX) pwm = PWM_MAX;

  analogWrite(PWM_LED_PIN, (int)pwm);

  // 光线极强时（>800）蜂鸣器（D6）短鸣提示
  if (lightVal > LIGHT_THRESHOLD) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(BUZZ_SHORT_MS);
    digitalWrite(BUZZER_PIN, LOW);
  }

  // Keep loop responsive without changing requested behavior
  delay(10);
}