#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_10\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// Arduino Mega 2560 wiring:
// Bar graph A10..A1 connect to digital pins 11..2, respectively.
// All bar graph cathodes connect to GND (active-HIGH LEDs).
// Potentiometer SIG connects to analog A3.
// Button connects digital pin 12 to GND.
constexpr uint8_t LED_COUNT = 10;
constexpr uint8_t LED_PINS[LED_COUNT] = {
  11, 10, 9, 8, 7, 6, 5, 4, 3, 2
};
constexpr uint8_t POT_PIN = A3;
constexpr uint8_t BUTTON_PIN = 12;
constexpr unsigned long DEBOUNCE_MS = 30;

bool reversed = false;
uint8_t lastButtonReading = HIGH;
uint8_t stableButtonState = HIGH;
unsigned long lastButtonChangeMs = 0;

#line 21 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_10\\direct\\attempt_0\\sketch\\sketch.ino"
void updateButton();
#line 41 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_10\\direct\\attempt_0\\sketch\\sketch.ino"
void displayLevel(uint8_t litCount);
#line 50 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_10\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 62 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_10\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 21 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_10\\direct\\attempt_0\\sketch\\sketch.ino"
void updateButton() {
  const unsigned long now = millis();
  const uint8_t reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonReading) {
    lastButtonReading = reading;
    lastButtonChangeMs = now;
  }

  if (reading != stableButtonState &&
      now - lastButtonChangeMs >= DEBOUNCE_MS) {
    stableButtonState = reading;

    // Toggle once per debounced press, not while held or on release.
    if (stableButtonState == LOW) {
      reversed = !reversed;
    }
  }
}

void displayLevel(uint8_t litCount) {
  for (uint8_t i = 0; i < LED_COUNT; ++i) {
    const bool on = reversed
                      ? (i >= LED_COUNT - litCount)
                      : (i < litCount);
    digitalWrite(LED_PINS[i], on ? HIGH : LOW);
  }
}

void setup() {
  // Initialize every LED off before beginning normal operation.
  for (uint8_t i = 0; i < LED_COUNT; ++i) {
    digitalWrite(LED_PINS[i], LOW);
    pinMode(LED_PINS[i], OUTPUT);
  }

  pinMode(POT_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  lastButtonChangeMs = millis();
}

void loop() {
  updateButton();

  const uint16_t potValue = analogRead(POT_PIN);

  // Map the full ADC range to 11 levels: zero through ten LEDs.
  const uint8_t litCount =
    static_cast<uint8_t>((static_cast<uint32_t>(potValue) *
                          (LED_COUNT + 1)) / 1024UL);

  displayLevel(litCount);
}
