#include <Arduino.h>
#include <TM1637Display.h>

static const uint8_t BTN_PIN = 2;     // D2
static const uint8_t CLK_PIN = 4;     // D4
static const uint8_t DIO_PIN = 5;     // D5

// TM1637 data: most common is brightness 0..7 (or 0..7 depending on library)
static const uint8_t TM1637_BRIGHTNESS = 7;

// Debounce/edge detection parameters
static const unsigned long DEBOUNCE_MS = 30;

TM1637Display display(CLK_PIN, DIO_PIN);

static unsigned long secondsCounter = 0;

static bool lastBtnStable = HIGH;       // assuming pull-up
static bool lastBtnReading = HIGH;
static unsigned long lastDebounceTime = 0;

void setup() {
  pinMode(BTN_PIN, INPUT_PULLUP);

  display.setBrightness(TM1637_BRIGHTNESS);

  // Initialize display to 0000 (or 0) for a clean start
  display.showNumberDecEx(0, 0b01000000, true); // no leading zeros behavior may vary by library; keep simple
}

void loop() {
  // --- Button debounce and edge detect (press = LOW) ---
  bool reading = digitalRead(BTN_PIN);

  if (reading != lastBtnReading) {
    lastDebounceTime = millis();
    lastBtnReading = reading;
  }

  if ((millis() - lastDebounceTime) > DEBOUNCE_MS) {
    if (reading != lastBtnStable) {
      lastBtnStable = reading;

      // On button press (LOW): clear counter
      if (lastBtnStable == LOW) {
        secondsCounter = 0;
      }
    }
  }

  // --- Timer: increment each second ---
  static unsigned long lastTickMs = 0;
  unsigned long now = millis();

  if (now - lastTickMs >= 1000UL) {
    // Handle potential drift by incrementing with modulo
    unsigned long ticks = (now - lastTickMs) / 1000UL;
    lastTickMs += ticks * 1000UL;

    secondsCounter += ticks;
  }

  // --- Display current seconds ---
  // Show up to 4 digits; TM1637Display truncates higher values.
  // Keep behavior consistent: show decimal without special colon.
  display.showNumberDecEx(secondsCounter % 10000UL, 0, true);
}