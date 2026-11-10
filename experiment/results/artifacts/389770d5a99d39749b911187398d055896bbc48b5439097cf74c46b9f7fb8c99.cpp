#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_011\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

static const uint8_t PWM_PIN = 9;      // D9 PWM LED
static const uint32_t BAUD = 9600;

#line 6 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_011\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 15 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_011\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 6 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_011\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  Serial.begin(BAUD);

  pinMode(PWM_PIN, OUTPUT);

  // Ensure deterministic initial state
  analogWrite(PWM_PIN, 0);
}

void loop() {
  static bool inNumber = false;
  static uint16_t value = 0;

  // Parse an integer from serial input in the range 0..255.
  // Accept line-oriented input; also handle idle_timeout-like behavior
  // by finalizing number parsing when no more digits arrive for a short time.
  static unsigned long lastByteTime = 0;
  const unsigned long idleTimeoutMs = 20;

  while (Serial.available() > 0) {
    lastByteTime = millis();

    int c = Serial.read();
    if (c < 0) break;

    // Treat newline/carriage return as end-of-number separators
    if (c == '\n' || c == '\r') {
      if (inNumber) {
        if (value > 255) value = 255;
        analogWrite(PWM_PIN, (uint8_t)value);
      }
      inNumber = false;
      value = 0;
      continue;
    }

    // Accept digits; build the number
    if (c >= '0' && c <= '9') {
      inNumber = true;
      value = (uint16_t)(value * 10u + (uint16_t)(c - '0'));
      continue;
    }

    // Ignore other characters; if we were building a number, finalize on separator-ish characters
    if (inNumber) {
      if (value > 255) value = 255;
      analogWrite(PWM_PIN, (uint8_t)value);
    }
    inNumber = false;
    value = 0;
  }

  // Idle timeout finalization to emulate "line_or_idle" behavior
  if (inNumber && (millis() - lastByteTime) > idleTimeoutMs) {
    if (value > 255) value = 255;
    analogWrite(PWM_PIN, (uint8_t)value);
    inNumber = false;
    value = 0;
  }
}
