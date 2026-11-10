#include <Arduino.h>

static const uint8_t BTN_PIN = 2;

static const uint8_t POT1_PIN = A3;
static const uint8_t POT2_PIN = A4;

// Bargraph1 wiring: A1..A10 are LED anodes; C1..C10 are common cathodes (already tied to GND in circuit)
static const uint8_t BAR_LED_PINS[10] = {
  3, 4, 5, 6, 7, 8, 9, 10, 11, 12
};

static const uint16_t DEBOUNCE_MS = 150;

static bool activeIsPot1 = true;

static uint16_t readAndMapTo10() {
  // Analog range 0..1023 mapped to 0..10 LEDs
  uint16_t v = analogRead(activeIsPot1 ? POT1_PIN : POT2_PIN);

  // Map to 0..10 where boundary conditions are guaranteed:
  // v=0 => 0, v=1023 => 10
  // Use long to avoid overflow and ensure end-point correctness.
  uint16_t leds = (uint32_t)v * 10UL / 1023UL;
  return leds; // 0..10
}

static void setBargraph(uint16_t ledsLit) {
  // ledsLit in [0..10]
  // If 0 => all off; if 10 => all on.
  for (uint8_t i = 0; i < 10; i++) {
    digitalWrite(BAR_LED_PINS[i], (i < ledsLit) ? HIGH : LOW);
  }
}

void setup() {
  // Ensure all bargraph LEDs are off at startup/reset
  for (uint8_t i = 0; i < 10; i++) {
    pinMode(BAR_LED_PINS[i], OUTPUT);
    digitalWrite(BAR_LED_PINS[i], LOW);
  }

  // Button input
  pinMode(BTN_PIN, INPUT_PULLUP);

  // Initial active input can be either; requirements only specify toggling order on presses.
  // We'll start with pot1 active, and bargraph will show pot1 value continuously afterward.
  activeIsPot1 = true;

  // Optional: ensure display starts off regardless of potentiometer reading per requirement #1.
  // We keep it off until the continuous update below runs in loop().
  for (uint8_t i = 0; i < 10; i++) digitalWrite(BAR_LED_PINS[i], LOW);
}

void loop() {
  static bool lastRaw = digitalRead(BTN_PIN);
  static bool stableState = lastRaw;
  static unsigned long stableStart = millis();

  // Debounce / valid press detection:
  // Button uses INPUT_PULLUP: pressed = LOW, released = HIGH.
  // Valid press defined as a button state change lasting at least 0.15s.
  bool raw = digitalRead(BTN_PIN);

  if (raw != lastRaw) {
    // Raw changed; restart timing for stability check
    stableStart = millis();
    lastRaw = raw;
  }

  if ((millis() - stableStart) >= DEBOUNCE_MS && raw != stableState) {
    stableState = raw;

    // Consider a "valid press" at the transition into pressed state (LOW)
    if (stableState == LOW) {
      // Toggle active input: first press selects pot1, second selects pot2, etc.
      // Starting assumption activeIsPot1=true, so toggle on each valid press.
      activeIsPot1 = !activeIsPot1;
    }
  }

  // Continuous display update based on currently active potentiometer
  uint16_t ledsLit = readAndMapTo10();
  setBargraph(ledsLit);
}