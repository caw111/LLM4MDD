#include <Arduino.h>

const int BTN1_PIN = 2; // btn1
const int BTN2_PIN = 3; // btn2

const int RGB_R_PIN = 4;
const int RGB_G_PIN = 5;
const int RGB_B_PIN = 6;

const uint8_t sequenceSize = 9;

// Sequence: Off → Red → Green → Blue → Yellow → Cyan → Magenta → White → Off
// Common cathode: drive HIGH to turn ON.
struct RGB {
  uint8_t r, g, b;
};

const RGB sequence[sequenceSize] = {
  {0, 0, 0}, // Off
  {1, 0, 0}, // Red
  {0, 1, 0}, // Green
  {0, 0, 1}, // Blue
  {1, 1, 0}, // Yellow
  {0, 1, 1}, // Cyan
  {1, 0, 1}, // Magenta
  {1, 1, 1}, // White
  {0, 0, 0}  // Off
};

const unsigned long debounceMs = 50;

static bool stableBtn1 = false;
static bool stableBtn2 = false;
static bool lastReadingBtn1 = false;
static bool lastReadingBtn2 = false;
static unsigned long lastDebounceTimeBtn1 = 0;
static unsigned long lastDebounceTimeBtn2 = 0;

static uint8_t colorIndex = 0; // starts in Off

void applyColor(uint8_t idx) {
  idx %= sequenceSize;
  digitalWrite(RGB_R_PIN, sequence[idx].r ? HIGH : LOW);
  digitalWrite(RGB_G_PIN, sequence[idx].g ? HIGH : LOW);
  digitalWrite(RGB_B_PIN, sequence[idx].b ? HIGH : LOW);
}

void setup() {
  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);

  pinMode(RGB_R_PIN, OUTPUT);
  pinMode(RGB_G_PIN, OUTPUT);
  pinMode(RGB_B_PIN, OUTPUT);

  // Initialize RGB LED to Off
  applyColor(colorIndex);

  // Initialize debounce state
  lastReadingBtn1 = digitalRead(BTN1_PIN) == LOW;
  lastReadingBtn2 = digitalRead(BTN2_PIN) == LOW;
  stableBtn1 = lastReadingBtn1;
  stableBtn2 = lastReadingBtn2;
  lastDebounceTimeBtn1 = millis();
  lastDebounceTimeBtn2 = millis();
}

void loop() {
  unsigned long now = millis();

  // Read raw states (pressed == LOW due to INPUT_PULLUP)
  bool readingBtn1 = (digitalRead(BTN1_PIN) == LOW);
  bool readingBtn2 = (digitalRead(BTN2_PIN) == LOW);

  // Debounce btn1
  if (readingBtn1 != lastReadingBtn1) {
    lastDebounceTimeBtn1 = now;
    lastReadingBtn1 = readingBtn1;
  }
  if ((now - lastDebounceTimeBtn1) >= debounceMs && readingBtn1 != stableBtn1) {
    stableBtn1 = readingBtn1;
    if (stableBtn1) {
      // btn1 advances to next color
      colorIndex = (colorIndex + 1) % sequenceSize;
      applyColor(colorIndex);
    }
  }

  // Debounce btn2
  if (readingBtn2 != lastReadingBtn2) {
    lastDebounceTimeBtn2 = now;
    lastReadingBtn2 = readingBtn2;
  }
  if ((now - lastDebounceTimeBtn2) >= debounceMs && readingBtn2 != stableBtn2) {
    stableBtn2 = readingBtn2;
    if (stableBtn2) {
      // btn2 returns to previous color
      colorIndex = (colorIndex + sequenceSize - 1) % sequenceSize;
      applyColor(colorIndex);
    }
  }
}