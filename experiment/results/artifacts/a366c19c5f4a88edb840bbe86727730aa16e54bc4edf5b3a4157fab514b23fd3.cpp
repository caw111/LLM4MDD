#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_011\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

static const uint8_t PWM_PIN = 9;     // D9 PWM LED
static const uint32_t BAUD = 9600;    // Serial baud rate
static const uint16_t IDLE_TIMEOUT_MS = 20;

static uint8_t currentBrightness = 0;

#line 9 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_011\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 16 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_011\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 9 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_011\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(PWM_PIN, OUTPUT);
  analogWrite(PWM_PIN, currentBrightness);

  Serial.begin(BAUD);
}

void loop() {
  static unsigned long lastRxMs = 0;

  // Read one integer (0..255) from serial. Requirement: serial input a number to set PWM brightness.
  // Accept digits possibly preceded/followed by whitespace, terminated by newline or any non-digit.
  // If the input is malformed, ignore it (do not change output).
  if (Serial.available() > 0) {
    lastRxMs = millis();

    // Read the incoming line (line_or_idle). If idle happens, line may be partial;
    // in that case, parsing will still succeed if digits are present.
    String s;
    while (Serial.available() > 0) {
      char c = (char)Serial.read();
      // Stop on newline; otherwise keep collecting for idle-framing parsing.
      if (c == '\n' || c == '\r') break;
      s += c;
      // Avoid unbounded growth in case of continuous garbage.
      if (s.length() > 16) break;
    }

    // Trim and parse as integer.
    s.trim();
    if (s.length() > 0) {
      // Parse only if it looks like an integer (handles possible leading '+' or '-')
      bool negative = false;
      size_t i = 0;
      if (s[0] == '+') i = 1;
      else if (s[0] == '-') {
        negative = true;
        i = 1;
      }

      bool hasDigits = false;
      long value = 0;
      for (; i < s.length(); i++) {
        char c = s[i];
        if (c >= '0' && c <= '9') {
          hasDigits = true;
          value = value * 10 + (c - '0');
          if (value > 10000) break; // safety clamp for parsing overflow-ish inputs
        } else {
          // If non-digit appears, stop parsing.
          break;
        }
      }

      if (hasDigits) {
        long signedValue = negative ? -value : value;
        if (signedValue >= 0 && signedValue <= 255) {
          currentBrightness = (uint8_t)signedValue;
          analogWrite(PWM_PIN, currentBrightness);
        }
      }
    }
  } else {
    // idle_timeout_ms handling: if serial goes idle, nothing special required for this behavior,
    // but we keep lastRxMs to respect configuration intent.
    (void)lastRxMs;
    // No other behavior required.
  }
}
