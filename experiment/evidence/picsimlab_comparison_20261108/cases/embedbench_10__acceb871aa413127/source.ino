// Arduino Mega 2560 sketch (Wokwi): LED bar graph controlled by slide potentiometer + pushbutton.
// Wiring preserved per provided circuit.
// Bargraph pins: A1..A10 -> Mega digital 2..11 (A1=2, A10=11)
// Pot: SIG -> Mega A3, VCC -> 5V, GND -> GND
// Button: left terminal -> Mega pin 12, right terminal -> GND

const uint8_t barPins[10] = { // Index 0 -> A1 (pin 2), Index 9 -> A10 (pin 11)
  2, 3, 4, 5, 6, 7, 8, 9, 10, 11
};

const uint8_t potPin = A3;

// Button on pin 12, with other side to GND.
// Since the button wiring uses "1.l" to Mega:12 and "2.r" to GND,
// we use INPUT_PULLUP and treat "pressed" as LOW.
const uint8_t btnPin = 12;

const int potMin = 0;
const int potMax = 1023;

bool reverseOrder = false;
int lastLedCount = -1;

static int mapPotToLedCount(int potValue) {
  if (potValue < potMin) potValue = potMin;
  if (potValue > potMax) potValue = potMax;

  // Convert 0..1023 to 0..10
  // Ensures increasing pot increases number of lit LEDs.
  long count = (long)potValue * 10L / 1023L;
  if (count < 0) count = 0;
  if (count > 10) count = 10;
  return (int)count;
}

static void clearAll() {
  for (uint8_t i = 0; i < 10; i++) {
    digitalWrite(barPins[i], LOW);
  }
}

static void setBarByCount(int ledCount) {
  // ledCount: 0..10
  clearAll();
  if (ledCount <= 0) return;

  // Original sequence: start lighting from A1 (pin 2) with pin: A10 last.
  // Reversed sequence: start lighting from A10 (pin 11) with pin A1 last.
  if (!reverseOrder) {
    for (int i = 0; i < ledCount; i++) {
      digitalWrite(barPins[i], HIGH);
    }
  } else {
    // Reverse order: light from the last LED (A10) backwards.
    for (int k = 0; k < ledCount; k++) {
      int idx = 9 - k; // A10 index 9
      digitalWrite(barPins[idx], HIGH);
    }
  }
}

void setup() {
  for (uint8_t i = 0; i < 10; i++) {
    pinMode(barPins[i], OUTPUT);
    digitalWrite(barPins[i], LOW); // Off on power-up/reset
  }

  pinMode(btnPin, INPUT_PULLUP);
}

void loop() {
  // Button edge detect with basic debouncing
  static bool lastBtnPressed = false;
  static unsigned long lastDebounceMs = 0;

  bool btnPressed = (digitalRead(btnPin) == LOW);

  if (btnPressed != lastBtnPressed) {
    lastDebounceMs = millis();
    lastBtnPressed = btnPressed;
  }

  static bool stablePrev = false;
  // Only toggle on a stable press transition
  if (millis() - lastDebounceMs > 30) {
    bool stablePressed = btnPressed;
    if (stablePressed && !stablePrev) {
      reverseOrder = !reverseOrder;
    }
    stablePrev = stablePressed;
  }

  int potValue = analogRead(potPin);
  int ledCount = mapPotToLedCount(potValue);

  if (ledCount != lastLedCount) {
    setBarByCount(ledCount);
    lastLedCount = ledCount;
  }

  // No need for delay; keep responsive
}