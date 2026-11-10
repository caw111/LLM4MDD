#include <Arduino.h>

static const uint8_t BTN1_PIN = 2;
static const uint8_t BTN2_PIN = 3;
static const uint8_t BTN3_PIN = 4;

static const uint8_t POT1_PIN = A3;
static const uint8_t POT2_PIN = A4;
static const uint8_t POT3_PIN = A5;

// Debounce: each button press must be debounced to avoid false triggers (0.15-second press duration).
static const uint32_t DEBOUNCE_MS = 150;

static const uint8_t RGB_RED_PIN   = 5; // mega:5  -> rgb1:R
static const uint8_t RGB_GREEN_PIN = 6; // mega:6  -> rgb1:G
static const uint8_t RGB_BLUE_PIN  = 7; // mega:7  -> rgb1:B

// Buttons are wired to GND with signal to pins 2/3/4 and other side to GND.
// Provide pullups, so pressed = LOW.
static bool stablePressed[3] = {false, false, false};
static bool lastReadingPressed[3] = {false, false, false};
static uint32_t lastChangeMs[3] = {0, 0, 0};
static uint8_t channelValue[3] = {0, 0, 0}; // 0..255 for R,G,B

static inline bool readButtonPressed(uint8_t idx, uint8_t pin) {
  // pressed when pin reads LOW
  (void)idx;
  return (digitalRead(pin) == LOW);
}

static inline uint8_t readPot255(uint8_t pin) {
  // Analog range 0..1023 -> 0..255
  uint16_t v = analogRead(pin);
  return (uint8_t)((v * 255UL) / 1023UL);
}

static inline void setRGB(uint8_t r, uint8_t g, uint8_t b) {
  // Common cathode: driving pin HIGH turns channel ON (brightness via PWM)
  analogWrite(RGB_RED_PIN, r);
  analogWrite(RGB_GREEN_PIN, g);
  analogWrite(RGB_BLUE_PIN, b);
}

void setup() {
  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);
  pinMode(BTN3_PIN, INPUT_PULLUP);

  pinMode(RGB_RED_PIN, OUTPUT);
  pinMode(RGB_GREEN_PIN, OUTPUT);
  pinMode(RGB_BLUE_PIN, OUTPUT);

  // Initialization: RGB LED starts off (all channels at 0)
  channelValue[0] = 0; // R
  channelValue[1] = 0; // G
  channelValue[2] = 0; // B
  setRGB(channelValue[0], channelValue[1], channelValue[2]);

  // Initialize debouncing trackers with current readings
  lastReadingPressed[0] = readButtonPressed(0, BTN1_PIN);
  lastReadingPressed[1] = readButtonPressed(1, BTN2_PIN);
  lastReadingPressed[2] = readButtonPressed(2, BTN3_PIN);

  stablePressed[0] = lastReadingPressed[0];
  stablePressed[1] = lastReadingPressed[1];
  stablePressed[2] = lastReadingPressed[2];

  lastChangeMs[0] = millis();
  lastChangeMs[1] = millis();
  lastChangeMs[2] = millis();
}

void loop() {
  const uint8_t btnPins[3] = {BTN1_PIN, BTN2_PIN, BTN3_PIN};
  const uint8_t potPins[3] = {POT1_PIN, POT2_PIN, POT3_PIN};

  // Debounce each button, update state, and while held continuously adjust associated intensity.
  for (uint8_t i = 0; i < 3; i++) {
    bool readingPressed = readButtonPressed(i, btnPins[i]);

    if (readingPressed != lastReadingPressed[i]) {
      lastReadingPressed[i] = readingPressed;
      lastChangeMs[i] = millis();
    }

    // If the reading has remained stable for debounce duration, accept it
    if ((millis() - lastChangeMs[i]) >= DEBOUNCE_MS) {
      stablePressed[i] = readingPressed;
    }

    // Real-time adjustment while held
    if (stablePressed[i]) {
      channelValue[i] = readPot255(potPins[i]);
    }
    // State retention handled naturally: when not pressed, channelValue[i] keeps last value.
  }

  setRGB(channelValue[0], channelValue[1], channelValue[2]);
}