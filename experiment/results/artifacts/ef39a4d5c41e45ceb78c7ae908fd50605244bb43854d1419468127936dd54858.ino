#include <Arduino.h>
#include <TM1637Display.h>

// TM1637 pins (authoritative pin assignments)
static const uint8_t TM1637_CLK = 4; // D4
static const uint8_t TM1637_DIO = 5; // D5

// Button pin (authoritative pin assignment)
static const uint8_t BUTTON_PIN = 2; // D2

// Potentiometer analog input (authoritative pin assignment)
static const uint8_t POT_PIN = A1;

// Display
TM1637Display display(TM1637_CLK, TM1637_DIO);

// Button handling
static bool lastButtonState = HIGH; // INPUT_PULLUP: HIGH when not pressed, LOW when pressed
static unsigned long lastDebounceMs = 0;
static const unsigned long debounceDelayMs = 30;

static bool show8888 = false;

static uint16_t mapToDisplayRange(int raw)
{
  // Convert 0..1023 to 0..9999 (no decimals)
  if (raw < 0) raw = 0;
  if (raw > 1023) raw = 1023;
  return (uint16_t)((long)raw * 9999L / 1023L);
}

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // TM1637 display brightness: safe default
  display.setBrightness(7, true);

  show8888 = false;

  lastButtonState = digitalRead(BUTTON_PIN);

  // Show potentiometer value immediately
  int raw = analogRead(POT_PIN);
  uint16_t value = mapToDisplayRange(raw);
  display.showNumberDecEx(value, 0, true);
}

void loop() {
  // Read and debounce button
  bool buttonState = digitalRead(BUTTON_PIN);

  if (buttonState != lastButtonState) {
    lastDebounceMs = millis();
    lastButtonState = buttonState;
  }

  // After debounce, determine if pressed
  static bool stableButtonPressed = false;
  if ((millis() - lastDebounceMs) > debounceDelayMs) {
    bool pressed = (buttonState == LOW); // INPUT_PULLUP: LOW when pressed
    if (pressed != stableButtonPressed) {
      stableButtonPressed = pressed;
      show8888 = pressed; // pressed => show 8888, otherwise show pot value
    }
  }

  if (show8888) {
    display.showNumberDecEx(8888, 0, true);
  } else {
    int raw = analogRead(POT_PIN);
    uint16_t value = mapToDisplayRange(raw);
    display.showNumberDecEx(value, 0, true);
  }
}