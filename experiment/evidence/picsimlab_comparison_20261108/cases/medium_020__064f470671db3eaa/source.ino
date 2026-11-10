#include <Arduino.h>
#include <TM1637Display.h>

// ---- Pin assignments (authoritative) ----
static const uint8_t PIN_POT   = A0;  // analog_input
static const uint8_t PIN_BTN   = 2;   // digital_input (D2)
static const uint8_t PIN_TM_CLK = 4;  // TM1637 CLK (D4)
static const uint8_t PIN_TM_DIO = 5;  // TM1637 DIO (D5)

// ---- Counting behavior ----
static long counterValue = 0;
static unsigned long lastCountMs = 0;
static const unsigned long COUNT_PERIOD_MS = 250;

// ---- TM1637 ----
TM1637Display display(PIN_TM_CLK, PIN_TM_DIO);

// ---- Button / mode switching ----
enum DisplayMode : uint8_t {
  MODE_COUNT = 0,
  MODE_POT   = 1,
  MODE_1234  = 2
};

static volatile bool buttonPressedFlag = false;
static DisplayMode mode = MODE_COUNT;
static unsigned long lastButtonEdgeMs = 0;

// Basic debouncing for button edges
static const unsigned long DEBOUNCE_MS = 30;

// Button press ISR: just set a flag, real work in loop()
void onButtonISR() {
  buttonPressedFlag = true;
}

static void handleButtonPress() {
  unsigned long now = millis();
  if (now - lastButtonEdgeMs < DEBOUNCE_MS) return;
  lastButtonEdgeMs = now;

  // Cycle modes on each button press: count -> potentiometer value -> 1234 -> ...
  switch (mode) {
    case MODE_COUNT: mode = MODE_POT;   break;
    case MODE_POT:   mode = MODE_1234; break;
    case MODE_1234:  mode = MODE_COUNT; break;
    default:         mode = MODE_COUNT; break;
  }
}

// Convert potentiometer analog reading to a 4-digit display value.
// Requirement says: display "电位器值" (potentiometer value).
// Display raw 0..1023 on 4 digits (right-aligned, no leading zeros).
static uint16_t readPotValue() {
  return (uint16_t)analogRead(PIN_POT); // 0..1023
}

// Update display based on current mode
static void updateDisplay() {
  if (mode == MODE_COUNT) {
    long v = counterValue;
    if (v < 0) v = 0;
    if (v > 9999) v = 9999;
    display.showNumberDec((int)v, false, 4, 0); // right-aligned, no leading zeros
  } else if (mode == MODE_POT) {
    uint16_t pot = readPotValue(); // 0..1023
    display.showNumberDec((int)pot, false, 4, 0); // right-aligned, no leading zeros
  } else { // MODE_1234
    display.showNumberDec(1234, false, 4, 0);
  }
}

void setup() {
  pinMode(PIN_BTN, INPUT_PULLUP);

  display.setBrightness(0x0f, true); // max brightness

  // Edge-triggered interrupt for D2 (INT0 on UNO).
  attachInterrupt(digitalPinToInterrupt(PIN_BTN), onButtonISR, FALLING);
}

void loop() {
  // Handle button (mode switch)
  if (buttonPressedFlag) {
    noInterrupts();
    buttonPressedFlag = false;
    interrupts();
    handleButtonPress();
  }

  // Update count periodically (keeps consistent "计数" behavior)
  unsigned long now = millis();
  if (now - lastCountMs >= COUNT_PERIOD_MS) {
    lastCountMs = now;
    counterValue++;
  }

  // Refresh display
  updateDisplay();
}