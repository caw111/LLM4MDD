#include <Arduino.h>
#include <TM1637Display.h>

static const uint8_t PIN_BUTTON = 2;   // D2
static const uint8_t PIN_LED13   = 13;  // D13

static const uint8_t TM1637_CLK = 4;    // D4
static const uint8_t TM1637_DIO = 5;    // D5

// Basic TM1637 wiring/behavior: show count on 7-seg
TM1637Display display(TM1637_CLK, TM1637_DIO);

// Counter + parity LED switching
volatile long counter = 0;

const unsigned long DEBOUNCE_MS = 50;
bool lastButtonLevel = HIGH; // using INPUT_PULLUP; released = HIGH
unsigned long lastDebounceTime = 0;

bool buttonPressedEvent = false;

void setup() {
  pinMode(PIN_BUTTON, INPUT_PULLUP); // D2 button to GND
  pinMode(PIN_LED13, OUTPUT);

  // Initialize TM1637
  display.setBrightness(0x0f, false); // max-ish brightness, no brightness control flicker

  // Initialize display and LED based on initial counter
  // For parity: odd -> LED ON, even -> LED OFF
  digitalWrite(PIN_LED13, (counter & 0x01) ? HIGH : LOW);

  // Show initial count
  // TM1637Display::showNumberDec prints up to 4 digits; wrap handled by modulo display.
  display.showNumberDec((int)(counter % 10000), false, 4, 0);
}

void loop() {
  // Read button with debounce; count increments once per press
  bool level = digitalRead(PIN_BUTTON);

  if (level != lastButtonLevel) {
    lastDebounceTime = millis();
    lastButtonLevel = level;
  }

  if ((millis() - lastDebounceTime) > DEBOUNCE_MS) {
    // Detect press event: transition to LOW (button pressed)
    static bool stableLevel = HIGH;
    if (level != stableLevel) {
      stableLevel = level;
      if (stableLevel == LOW) {
        buttonPressedEvent = true;
      }
    }
  }

  if (buttonPressedEvent) {
    buttonPressedEvent = false;

    // Increment counter
    counter++;

    // Update LED according to parity (odd/even)
    digitalWrite(PIN_LED13, (counter & 0x01) ? HIGH : LOW);

    // Update TM1637 display with counter (wrap to 4 digits)
    int shown = (int)(counter % 10000L);
    if (shown < 0) shown += 10000;
    display.showNumberDec(shown, false, 4, 0);
  }
}