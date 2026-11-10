#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_2\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const byte PIN_RGB_R = 2;
const byte PIN_RGB_G = 3;
const byte PIN_RGB_B = 4;

// Common cathode RGB LED: an active-high pin turns the corresponding color on.
const byte PIN_K1 = 5;

// Debounce parameters
const unsigned long DEBOUNCE_MS = 50;

#line 13 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_2\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 28 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_2\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 13 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_2\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  // RGB LED pins
  pinMode(PIN_RGB_R, OUTPUT);
  pinMode(PIN_RGB_G, OUTPUT);
  pinMode(PIN_RGB_B, OUTPUT);

  // Button input with internal pull-up (wiring uses k1:1.l to mega:5 and k1:2.r to GND)
  pinMode(PIN_K1, INPUT_PULLUP);

  // Initialize with LED off
  digitalWrite(PIN_RGB_R, LOW);
  digitalWrite(PIN_RGB_G, LOW);
  digitalWrite(PIN_RGB_B, LOW);
}

void loop() {
  static bool lastStablePressed = false;
  static bool lastReadingPressed = false;
  static unsigned long lastDebounceTime = 0;

  bool readingPressed = (digitalRead(PIN_K1) == LOW); // pressed -> LOW due to pull-up

  if (readingPressed != lastReadingPressed) {
    lastDebounceTime = millis();
    lastReadingPressed = readingPressed;
  }

  if (millis() - lastDebounceTime >= DEBOUNCE_MS) {
    if (readingPressed != lastStablePressed) {
      lastStablePressed = readingPressed;

      // Count only rising edge of "press": when it becomes pressed
      if (lastStablePressed) {
        static uint8_t state = 0; // 0=off start, 1=R, 2=G, 3=B, 4=off
        state = (state + 1) % 5;

        switch (state) {
          case 1: // Red
            digitalWrite(PIN_RGB_R, HIGH);
            digitalWrite(PIN_RGB_G, LOW);
            digitalWrite(PIN_RGB_B, LOW);
            break;
          case 2: // Green
            digitalWrite(PIN_RGB_R, LOW);
            digitalWrite(PIN_RGB_G, HIGH);
            digitalWrite(PIN_RGB_B, LOW);
            break;
          case 3: // Blue
            digitalWrite(PIN_RGB_R, LOW);
            digitalWrite(PIN_RGB_G, LOW);
            digitalWrite(PIN_RGB_B, HIGH);
            break;
          case 4: // Off
          default:
            digitalWrite(PIN_RGB_R, LOW);
            digitalWrite(PIN_RGB_G, LOW);
            digitalWrite(PIN_RGB_B, LOW);
            break;
        }
      }
    }
  }
}
