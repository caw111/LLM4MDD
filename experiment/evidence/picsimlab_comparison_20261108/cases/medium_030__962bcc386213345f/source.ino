#include <Arduino.h>
#include <TM1637Display.h>

constexpr uint8_t CLK_PIN = 4;
constexpr uint8_t DIO_PIN = 5;
constexpr uint8_t BUTTON_PIN = 2;

constexpr unsigned long DEBOUNCE_MS = 30;
constexpr unsigned long SECOND_MS = 1000;

TM1637Display display(CLK_PIN, DIO_PIN);

uint16_t secondsCount = 0;
unsigned long lastSecondMs = 0;
unsigned long buttonChangedMs = 0;

uint8_t lastButtonReading = HIGH;
uint8_t stableButtonState = HIGH;

void setup() {
  // Connect the reset button between D2 and GND.
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  display.setBrightness(7);
  display.showNumberDec(0, false);

  lastSecondMs = millis();
  buttonChangedMs = lastSecondMs;
  lastButtonReading = digitalRead(BUTTON_PIN);
}

void loop() {
  const unsigned long now = millis();
  const uint8_t reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonReading) {
    lastButtonReading = reading;
    buttonChangedMs = now;
  }

  if (reading != stableButtonState &&
      now - buttonChangedMs >= DEBOUNCE_MS) {
    stableButtonState = reading;

    if (stableButtonState == LOW) {
      secondsCount = 0;
      lastSecondMs = now;
      display.showNumberDec(0, false);
    }
  }

  const unsigned long elapsedSeconds = (now - lastSecondMs) / SECOND_MS;
  if (elapsedSeconds > 0) {
    lastSecondMs += elapsedSeconds * SECOND_MS;

    // Wrap at 10000 to fit the four-digit display.
    secondsCount =
        static_cast<uint16_t>((secondsCount + elapsedSeconds) % 10000UL);
    display.showNumberDec(secondsCount, false);
  }
}