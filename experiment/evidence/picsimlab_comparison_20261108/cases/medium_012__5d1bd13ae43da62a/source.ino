#include <Arduino.h>

static const uint8_t LED_PIN = 9;          // D9 PWM LED
static const unsigned long SERIAL_BAUD = 9600;
static const uint16_t IDLE_TIMEOUT_MS = 20;

static uint16_t incomingValue = 0;
static bool hasDigits = false;
static unsigned long lastRxMs = 0;

static void applyBrightness(uint16_t v) {
  // Requirements: Serial input 0~255 controls D9 PWM LED brightness and is echoed back.
  // Clamp defensively to 0..255.
  if (v > 255) v = 255;
  analogWrite(LED_PIN, (uint8_t)v);
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, 0);

  Serial.begin(SERIAL_BAUD);
  lastRxMs = millis();
}

void loop() {
  // Read all available bytes from serial; accept either line-based numbers or idle-terminated tokens.
  // Echo back resolved value (after parsing) and update PWM.
  while (Serial.available() > 0) {
    int c = Serial.read();
    lastRxMs = millis();

    if (c < 0) break;
    if (c == '\r' || c == '\n') {
      if (hasDigits) {
        applyBrightness(incomingValue);
        Serial.println(incomingValue); // echo back
      }
      // reset token state
      incomingValue = 0;
      hasDigits = false;
      continue;
    }

    // Accept digits only
    if (c >= '0' && c <= '9') {
      hasDigits = true;
      incomingValue = (uint16_t)(incomingValue * 10u + (uint16_t)(c - '0'));
      // Keep it bounded to 0..255 to preserve defined behavior.
      if (incomingValue > 255) incomingValue = 255;
    } else if (c == ' ' || c == '\t') {
      // Ignore whitespace inside token; token ends on newline or idle timeout.
    } else {
      // Ignore other characters (do not change behavior requirements).
    }
  }

  // Idle timeout terminates a token even without newline.
  if (hasDigits && (millis() - lastRxMs) >= IDLE_TIMEOUT_MS) {
    applyBrightness(incomingValue);
    Serial.println(incomingValue); // echo back
    incomingValue = 0;
    hasDigits = false;
  }
}