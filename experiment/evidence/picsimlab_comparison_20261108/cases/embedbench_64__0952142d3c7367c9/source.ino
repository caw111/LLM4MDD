#include <Arduino.h>

static const uint8_t BTN1_PIN = 2;   // btn1
static const uint8_t BTN2_PIN = 3;   // btn2

static const uint8_t RGB_R_PIN = 4;   // rgb1:R
static const uint8_t RGB_G_PIN = 5;   // rgb1:G
static const uint8_t RGB_B_PIN = 6;   // rgb1:B
// rgb1:COM -> GND (common cathode)

static const uint16_t DEBOUNCE_MS = 50;

// Color sequence: Off → Red → Green → Blue → Yellow → Cyan → Magenta → White → Off
enum ColorIndex : uint8_t {
  OFF = 0,
  RED,
  GREEN,
  BLUE,
  YELLOW,
  CYAN,
  MAGENTA,
  WHITE,
  OFF_AGAIN
};

struct RGB {
  uint8_t r, g, b; // 0..255 (common cathode: HIGH turns ON)
};

static const RGB COLORS[] = {
  {0,   0,   0},    // Off
  {255, 0,   0},    // Red
  {0,   255, 0},    // Green
  {0,   0,   255},  // Blue
  {255, 255, 0},    // Yellow
  {0,   255, 255},  // Cyan
  {255, 0,   255},  // Magenta
  {255, 255, 255},  // White
  {0,   0,   0}     // Off (loop back)
};

static volatile uint8_t currentIndex = OFF;

static uint8_t readButtonPressedState(uint8_t pin, bool &stableNow) {
  // Not used; kept minimal.
  (void)pin;
  (void)stableNow;
  return 0;
}

static bool buttonsReadAsPressed(bool rawLevel, bool activeLow) {
  return activeLow ? !rawLevel : rawLevel;
}

static void applyColor(uint8_t idx) {
  const RGB &c = COLORS[idx];
  // Common cathode: COM to GND, so drive R/G/B HIGH to turn ON.
  digitalWrite(RGB_R_PIN, c.r ? HIGH : LOW);
  digitalWrite(RGB_G_PIN, c.g ? HIGH : LOW);
  digitalWrite(RGB_B_PIN, c.b ? HIGH : LOW);
}

static bool debounceButton(uint8_t pin, bool activeLow, uint16_t debounceMs,
                            bool &lastStablePressed, uint32_t &lastDebounceTime) {
  // Returns true if a debounced "pressed event" occurred (transition to pressed).
  static bool lastRaw = false;

  bool rawLevel = digitalRead(pin);
  bool rawPressed = buttonsReadAsPressed(rawLevel, activeLow);

  if (rawPressed != lastRaw) {
    lastDebounceTime = millis();
    lastRaw = rawPressed;
  }

  if ((millis() - lastDebounceTime) >= debounceMs) {
    if (rawPressed != lastStablePressed) {
      lastStablePressed = rawPressed;
      if (lastStablePressed) {
        return true; // pressed event
      }
    }
  }
  return false;
}

void setup() {
  pinMode(BTN1_PIN, INPUT_PULLUP); // wiring uses GND on one side and signal on input => active LOW
  pinMode(BTN2_PIN, INPUT_PULLUP); // wiring uses GND on one side and signal on input => active LOW

  pinMode(RGB_R_PIN, OUTPUT);
  pinMode(RGB_G_PIN, OUTPUT);
  pinMode(RGB_B_PIN, OUTPUT);

  currentIndex = OFF; // initialization: off on power/reset
  applyColor(currentIndex);
}

void loop() {
  static bool lastStableBtn1Pressed = false;
  static bool lastStableBtn2Pressed = false;
  static uint32_t lastDebounceBtn1 = 0;
  static uint32_t lastDebounceBtn2 = 0;

  const bool btnActiveLow = true;

  bool btn1PressedEvent = debounceButton(BTN1_PIN, btnActiveLow, DEBOUNCE_MS,
                                         lastStableBtn1Pressed, lastDebounceBtn1);
  bool btn2PressedEvent = debounceButton(BTN2_PIN, btnActiveLow, DEBOUNCE_MS,
                                         lastStableBtn2Pressed, lastDebounceBtn2);

  if (btn1PressedEvent) {
    // Advance to next color
    currentIndex = (currentIndex + 1) % (sizeof(COLORS) / sizeof(COLORS[0]));
    applyColor(currentIndex); // update immediately after valid press
  } else if (btn2PressedEvent) {
    // Return to previous color
    if (currentIndex == 0) {
      currentIndex = (sizeof(COLORS) / sizeof(COLORS[0])) - 1;
    } else {
      currentIndex = currentIndex - 1;
    }
    applyColor(currentIndex); // update immediately after valid press
  }
}