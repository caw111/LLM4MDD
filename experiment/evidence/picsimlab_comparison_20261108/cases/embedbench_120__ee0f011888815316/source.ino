#include <Arduino.h>

static const uint8_t LED_PIN  = 2;

static const uint8_t BTN1_PIN = 3; // btn1:1.l
static const uint8_t BTN2_PIN = 4; // btn2:1.l

static const uint8_t POT1_PIN = A3; // pot1:SIG
static const uint8_t POT2_PIN = A4; // pot2:SIG
static const uint8_t POT3_PIN = A5; // pot3:SIG

// Debounce & state persistence rules
static const uint16_t DEBOUNCE_DELAY_MS = 150;
static const uint32_t MIN_STATE_HOLD_MS = 2000; // persist for at least 2 seconds

enum Mode : uint8_t { MANUAL = 0, AUTO = 1 };

static Mode mode = MANUAL;

// Button debounce state
static bool stableBtn1 = HIGH, stableBtn2 = HIGH;
static bool lastRawBtn1 = HIGH, lastRawBtn2 = HIGH;
static uint32_t btn1LastChangeMs = 0, btn2LastChangeMs = 0;

static void initButton(uint8_t pin) {
  pinMode(pin, INPUT_PULLUP);
}

static bool readDebouncedFallingEdge(uint8_t pin,
                                     bool &stableState,
                                     bool &lastRawState,
                                     uint32_t &lastChangeMs) {
  // Buttons are active-HIGH due to pullups:
  // raw HIGH = not pressed, raw LOW = pressed
  bool raw = digitalRead(pin);

  uint32_t now = millis();
  if (raw != lastRawState) {
    lastChangeMs = now;
    lastRawState = raw;
  }

  if ((now - lastChangeMs) >= DEBOUNCE_DELAY_MS) {
    if (raw != stableState) {
      stableState = raw;
      // Return true on transition to PRESSED (LOW)
      return (stableState == LOW);
    }
  }
  return false;
}

static uint8_t mapPotToBrightness(int raw) {
  raw = constrain(raw, 0, 1023);
  return (uint8_t)map(raw, 0, 1023, 0, 255);
}

static uint8_t potToMinMax(uint8_t pin) {
  int raw = analogRead(pin);
  return mapPotToBrightness(raw);
}

// Auto mode smooth transition variables
static uint8_t autoMinB = 0;
static uint8_t autoMaxB = 255;
static bool autoIncreasing = true;
static uint8_t autoCurrentB = 0;

// Use per-transition timing: each transition takes 2 seconds (rule)
static uint32_t transitionStartMs = 0;
static uint8_t transitionFromB = 0;
static uint8_t transitionToB = 255;

static void startAutoTransition(uint8_t fromB, uint8_t toB) {
  transitionFromB = fromB;
  transitionToB = toB;
  transitionStartMs = millis();
}

static void updateAutoTargetsFromPots() {
  uint8_t p2 = potToMinMax(POT2_PIN);
  uint8_t p3 = potToMinMax(POT3_PIN);

  // Minimum brightness set by pot2, maximum brightness by pot3
  autoMinB = p2;
  autoMaxB = p3;
  if (autoMinB > autoMaxB) {
    // Preserve intended behavior (min/max); swap if user wiring makes p2>p3
    uint8_t tmp = autoMinB;
    autoMinB = autoMaxB;
    autoMaxB = tmp;
  }
}

static void ensureAutoInitialized() {
  updateAutoTargetsFromPots();
  autoCurrentB = autoMinB;
  autoIncreasing = true;
  startAutoTransition(autoMinB, autoMaxB);
  analogWrite(LED_PIN, autoCurrentB);
}

void setup() {
  // LED off on power/reset (rule #1)
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, 0);

  initButton(BTN1_PIN);
  initButton(BTN2_PIN);

  // Start in Manual Mode (rule #1)
  mode = MANUAL;

  // Initialize debouncing tracking
  lastRawBtn1 = digitalRead(BTN1_PIN);
  lastRawBtn2 = digitalRead(BTN2_PIN);
  stableBtn1 = lastRawBtn1;
  stableBtn2 = lastRawBtn2;
  btn1LastChangeMs = millis();
  btn2LastChangeMs = millis();

  // Ensure manual state has no special timing
}

void loop() {
  static uint32_t modeEnteredAtMs = millis();

  // Enforce minimum 2-second persistence before switching modes again
  uint32_t now = millis();
  bool canSwitchMode = (now - modeEnteredAtMs) >= MIN_STATE_HOLD_MS;

  // Debounced press detection
  bool btn1Pressed = readDebouncedFallingEdge(BTN1_PIN, stableBtn1, lastRawBtn1, btn1LastChangeMs);
  bool btn2Pressed = readDebouncedFallingEdge(BTN2_PIN, stableBtn2, lastRawBtn2, btn2LastChangeMs);

  if (mode == MANUAL) {
    // Mode behavior: static brightness controlled by pot1 (rule #5)
    uint8_t b = mapPotToBrightness(analogRead(POT1_PIN));
    analogWrite(LED_PIN, b);
    autoCurrentB = b;

    if (canSwitchMode && btn1Pressed) {
      // Button 1 switches to Auto mode
      mode = AUTO;
      modeEnteredAtMs = millis();
      ensureAutoInitialized();
    }
  } else { // AUTO
    // If first time or if transition variables not initialized
    static bool autoInitDone = false;
    if (!autoInitDone) {
      autoInitDone = true;
      ensureAutoInitialized();
    }

    // Auto behavior: brightness cycles min<->max, smooth transition each takes 2 seconds
    // Rule #3 requires each transition taking 2 seconds.
    // Additionally, min/max are determined by pot2/pot3.
    // We'll refresh targets continuously but keep the current transition endpoints stable within a transition.
    updateAutoTargetsFromPots();

    // If targets changed enough, we update next transition endpoints; current transition continues.
    // When a transition completes, start a new one using updated pot2/pot3.
    const uint32_t TRANSITION_MS = 2000;

    uint32_t elapsed = now - transitionStartMs;
    if (elapsed >= TRANSITION_MS) {
      // End of transition
      autoCurrentB = transitionToB;

      // Flip direction and start new transition using current pot-defined endpoints
      if (transitionToB == autoMaxB) {
        // Went to max, go to min
        autoIncreasing = false;
        startAutoTransition(autoCurrentB, autoMinB);
      } else {
        // Went to min, go to max
        autoIncreasing = true;
        startAutoTransition(autoCurrentB, autoMaxB);
      }
    } else {
      // Smooth interpolation between fromB and toB
      // Linear: brightness(t) = from + (to-from)*t/T
      int from = (int)transitionFromB;
      int to = (int)transitionToB;
      long diff = (long)to - (long)from;

      // Use long math to preserve precision
      uint8_t interp = (uint8_t)(from + (diff * (long)elapsed) / (long)TRANSITION_MS);
      autoCurrentB = interp;
    }

    analogWrite(LED_PIN, autoCurrentB);

    if (canSwitchMode && btn2Pressed) {
      // Button 2 switches back to Manual mode
      mode = MANUAL;
      modeEnteredAtMs = millis();
      autoInitDone = false;

      // Ensure LED respects manual control immediately after switch (next loop iteration will update)
    }
  }
}