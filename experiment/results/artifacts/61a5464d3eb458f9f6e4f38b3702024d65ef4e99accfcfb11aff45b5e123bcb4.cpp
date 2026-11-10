#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_9\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

static const uint8_t BTN_PIN = 12; // btn1:1.l -> mega:12
static const uint8_t LED_PINS[10] = {
  11, // A10 (bottom)
  10, // A9
  9,  // A8
  8,  // A7
  7,  // A6
  6,  // A5
  5,  // A4
  4,  // A3
  3,  // A2
  2   // A1 (top)
};

static int pressedState = LOW;
static unsigned long lastDebounceMs = 0;
static const unsigned long debounceDelayMs = 30;

#line 21 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_9\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setAllLeds(bool on);
#line 27 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_9\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 36 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_9\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 21 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_9\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setAllLeds(bool on) {
  for (uint8_t i = 0; i < 10; i++) {
    digitalWrite(LED_PINS[i], on ? HIGH : LOW);
  }
}

void setup() {
  pinMode(BTN_PIN, INPUT_PULLUP);

  for (uint8_t i = 0; i < 10; i++) {
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], LOW); // all off on reset/power-up
  }
}

void loop() {
  // Button is wired to ground on the right side (btn1:2.r -> GND)
  // Using INPUT_PULLUP: pressed => LOW, released => HIGH.
  int reading = digitalRead(BTN_PIN);

  if (reading != pressedState) {
    lastDebounceMs = millis();
    pressedState = reading;
  }

  // Detect a new press event after debouncing (transition to LOW)
  if ((millis() - lastDebounceMs) > debounceDelayMs) {
    static int prevStableReading = HIGH;
    int stableReading = reading;

    if (stableReading == LOW && prevStableReading == HIGH) {
      // Persistent sequence step across loop iterations.
      static uint8_t step = 0; // 0..5
      uint8_t litCount = (uint8_t)(step * 2);

      // When step reaches 5, litCount becomes 10 (all LEDs on),
      // next press should turn all off and restart from first two.
      // Requirements specify: Sixth Press: All LEDs turn off, then restart.
      if (step == 5) {
        setAllLeds(false);
        step = 0;
      } else {
        setAllLeds(false);
        // Light LEDs from bottom up, two at a time
        for (uint8_t i = 0; i < litCount; i++) {
          digitalWrite(LED_PINS[i], HIGH);
        }
        step++;
      }
    }

    prevStableReading = stableReading;
  }
}
