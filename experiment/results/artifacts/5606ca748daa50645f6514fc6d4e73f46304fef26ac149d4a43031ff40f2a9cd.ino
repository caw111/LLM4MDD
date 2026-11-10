#include <Arduino.h>

constexpr uint8_t FIRST_PIN = 2;
constexpr uint8_t LAST_PIN = 12;
constexpr uint8_t LED_COUNT = 10;
constexpr unsigned long STEP_DELAY_MS = 2000UL;

void allLedsOff() {
  for (uint8_t pin = FIRST_PIN; pin <= LAST_PIN; ++pin) {
    digitalWrite(pin, LOW);
  }
}

void displayLed(uint8_t ledNumber) {
  allLedsOff();

  // Bargraph1 LED i uses pin i + 1.
  // The same pin drives bargraph2 LED i - 1 for i >= 2.
  // At i == 1, only pin 2 is HIGH, so bargraph2 is entirely off.
  digitalWrite(FIRST_PIN + ledNumber - 1, HIGH);

  // Pin 12 (bargraph2 LED 10) remains LOW: the required offset
  // only uses bargraph2 LEDs 1 through 9.
}

void setup() {
  for (uint8_t pin = FIRST_PIN; pin <= LAST_PIN; ++pin) {
    digitalWrite(pin, LOW);
    pinMode(pin, OUTPUT);
  }

  // Provide an observable all-off initialization interval.
  delay(STEP_DELAY_MS);
}

void loop() {
  // Left to right: bargraph1 LEDs 1 through 10.
  for (uint8_t led = 1; led <= LED_COUNT; ++led) {
    displayLed(led);
    delay(STEP_DELAY_MS);
  }

  // Right to left: explicitly start at LED 10 and finish at LED 1.
  for (int8_t led = LED_COUNT; led >= 1; --led) {
    displayLed(static_cast<uint8_t>(led));
    delay(STEP_DELAY_MS);
  }
}