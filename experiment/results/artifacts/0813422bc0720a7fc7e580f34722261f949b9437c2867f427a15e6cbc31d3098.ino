#include <Arduino.h>

static const uint8_t BTN_PIN = 2;     // btn1
static const uint8_t POT1_PIN = A3;   // pot1 SIG -> A3
static const uint8_t POT2_PIN = A4;   // pot2 SIG -> A4

// bargraph1 segments A1..A10 connected to Mega pins 3..12
static const uint8_t BAR_LED_PINS[10] = {3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

// Debounce/valid-press rule: state change lasting >= 0.15 s
static const unsigned long VALID_PRESS_MS = 150;

enum ActivePot : uint8_t { USE_POT1 = 0, USE_POT2 = 1 };

void allOff() {
  for (uint8_t i = 0; i < 10; i++) digitalWrite(BAR_LED_PINS[i], LOW);
}

void setup() {
  // Ensure LEDs are off at startup/reset
  for (uint8_t i = 0; i < 10; i++) {
    pinMode(BAR_LED_PINS[i], OUTPUT);
    digitalWrite(BAR_LED_PINS[i], LOW);
  }

  // Button input (wiring provides active-low: green button side to pin 2)
  pinMode(BTN_PIN, INPUT_PULLUP);

  // Potentiometers are analog inputs
  pinMode(POT1_PIN, INPUT);
  pinMode(POT2_PIN, INPUT);

  allOff();
}

static int mapToLeds0to10(int analogVal) {
  // analogVal: 0..1023 mapped to 0..10 inclusive
  // Boundary conditions: 0 -> 0 LEDs, 10 -> all 10 LEDs
  // Use rounding to reduce bias.
  long scaled = (long)analogVal * 10L + 511L; // +511 for rounding (approx half of 1023)
  int leds = (int)(scaled / 1023L);
  if (leds < 0) leds = 0;
  if (leds > 10) leds = 10;
  return leds;
}

void displayFromPot(uint8_t activePot) {
  int raw = (activePot == USE_POT1) ? analogRead(POT1_PIN) : analogRead(POT2_PIN);
  int ledsOn = mapToLeds0to10(raw); // 0..10

  for (uint8_t i = 0; i < 10; i++) {
    digitalWrite(BAR_LED_PINS[i], (i < (uint8_t)ledsOn) ? HIGH : LOW);
  }
}

void loop() {
  static ActivePot active = USE_POT1; // first valid press selects pot1; start with none? We'll start at pot1, then first press toggles to pot2.
  // However requirement says: First press selects pot1, second selects pot2, etc.
  // To preserve behavior: start with pot2 inactive state; after first valid press, set active to pot1.
  // We'll implement with an "uninitialized" state using a static flag.
  static bool hasSelected = false;

  // Track button stable state duration to qualify as a valid press (state change lasting >= 150ms).
  static int lastRaw = HIGH;
  static int stableState = HIGH;
  static unsigned long stableSince = 0;

  int raw = digitalRead(BTN_PIN);

  unsigned long now = millis();
  if (raw != lastRaw) {
    lastRaw = raw; // reset transient changes
  }

  if (raw != stableState) {
    // Potential state change started
    stableState = raw;
    stableSince = now;
  }

  // Check if current stableState has been held long enough
  if ((now - stableSince) >= VALID_PRESS_MS) {
    // We need to generate an event only once per stable change.
    // Use a latch to avoid retriggering while held stable.
    static int lastHandledStable = HIGH;
    if (stableState != lastHandledStable) {
      lastHandledStable = stableState;

      // Define "valid press": button state change lasting >= 0.15s.
      // With INPUT_PULLUP, pressing makes the state go LOW; releasing makes it HIGH.
      // Requirement describes "each valid press", so toggle on the press (LOW) event only.
      if (stableState == LOW) {
        if (!hasSelected) {
          // First press selects pot1
          active = USE_POT1;
          hasSelected = true;
        } else {
          // Subsequent presses toggle between pot1 and pot2
          active = (active == USE_POT1) ? USE_POT2 : USE_POT1;
        }
      }
    }
  }

  // Continuous display update from current active potentiometer.
  // If no valid press has occurred yet, keep all LEDs off (requirement #1).
  if (!hasSelected) {
    allOff();
  } else {
    displayFromPot((uint8_t)active);
  }
}