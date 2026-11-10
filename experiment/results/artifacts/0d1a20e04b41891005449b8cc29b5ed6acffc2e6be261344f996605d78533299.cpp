#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_5\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
// Arduino Mega 2560 - Button toggles LED on/off each press.
// Initial state: LED ON.

const uint8_t LED_PIN = 3;     // led1:A -> mega:3
const uint8_t BUTTON_PIN = 2;  // mega:2 -> k1:1.l

// Assumption from wiring: button provides a switch to GND (shared with mega:GND.1).

bool ledState = true;
bool lastButtonReading = HIGH; // with INPUT_PULLUP, idle is HIGH
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelayMs = 30;

#line 14 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_5\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 22 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_5\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 14 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_5\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  ledState = true;                 // LED should be ON after reset/power on
  digitalWrite(LED_PIN, HIGH);   // red LED on (active-high assumed by wiring)
}

void loop() {
  // Read and debounce
  bool reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonReading) {
    lastDebounceTime = millis();
    lastButtonReading = reading;
  }

  if ((millis() - lastDebounceTime) > debounceDelayMs) {
    // Detect a press: with INPUT_PULLUP, pressed => LOW
    static bool previouslyPressed = false;
    bool currentlyPressed = (reading == LOW);

    if (currentlyPressed && !previouslyPressed) {
      // Toggle on each press
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState ? HIGH : LOW);
    }

    previouslyPressed = currentlyPressed;
  }
}
