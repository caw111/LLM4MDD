// Arduino Mega 2560 sketch (as specified)
// Wiring preserved per provided circuit:
// LED led1: anode A -> mega pin 2, cathode C -> GND
// Buttons: btn1 -> mega pin 3 (left), right to GND; btn2 -> mega pin 4 (left), right to GND
// Pots: pot1 -> A3, pot2 -> A4, pot3 -> A5; VCC -> 5V, GND -> GND

// Button pins
const uint8_t BTN1_PIN = 3;
const uint8_t BTN2_PIN = 4;

// LED pin
const uint8_t LED_PIN = 2;

// Pot pins
const uint8_t POT1_PIN = A3;
const uint8_t POT2_PIN = A4;
const uint8_t POT3_PIN = A5;

// Debounce
const unsigned long DEBOUNCE_DELAY_MS = 150;

// State persistence (at least 2 seconds before switching)
const unsigned long MIN_MODE_TIME_MS = 2000;

// Brightness transition timing in Auto mode
const unsigned long TRANSITION_TIME_MS = 2000;

enum Mode {
  MANUAL = 0,
  AUTO = 1
};

Mode mode = MANUAL;

unsigned long lastModeSwitchMs = 0;

// Debounce tracking
bool btn1StablePressed = false;
bool btn2StablePressed = false;

bool btn1LastReading = false;
bool btn2LastReading = false;

unsigned long btn1LastChangeMs = 0;
unsigned long btn2LastChangeMs = 0;

// Auto mode transition tracking
int autoMinB = 0;
int autoMaxB = 255;

unsigned long autoTransitionStartMs = 0;
int autoStartB = 0;

int readPotMapped255(uint8_t pin) {
  int raw = analogRead(pin); // 0..1023
  if (raw < 0) raw = 0;
  if (raw > 1023) raw = 1023;
  return (raw * 255) / 1023;
}

void applyManualBrightness() {
  int b = readPotMapped255(POT1_PIN);
  analogWrite(LED_PIN, b);
}

void startAutoTransitionIfNeeded(unsigned long nowMs, bool forceNewRange) {
  if (forceNewRange) {
    // Sample current range endpoints (pot2=min, pot3=max)
    autoMinB = readPotMapped255(POT2_PIN);
    autoMaxB = readPotMapped255(POT3_PIN);
    if (autoMinB > autoMaxB) {
      int t = autoMinB;
      autoMinB = autoMaxB;
      autoMaxB = t;
    }

    autoTransitionStartMs = nowMs;
    autoStartB = analogRead(A0); // dummy to silence warnings; will be overwritten below
    // Set start brightness to current LED PWM (approximate by using last computed value is better)
    // We'll set it to the value we were at at transition start by recomputing from time fraction.
  } else if (nowMs - autoTransitionStartMs >= TRANSITION_TIME_MS) {
    // Start the next transition to opposite endpoint
    // Determine current brightness at end of last transition:
    // Since transition time is exactly 2s, at boundary we can just set to the opposite end.
    bool goingUp = (autoStartB == autoMinB);
    autoMinB = autoMinB; // keep current sampled range for this transition
    autoMaxB = autoMaxB;

    autoStartB = goingUp ? autoMaxB : autoMinB;
    autoTransitionStartMs = nowMs;
  }
}

void updateAutoBrightness(unsigned long nowMs) {
  // Refresh range endpoints continuously or only at boundaries?
  // Requirement: "minimum brightness is set by pot2, maximum by pot3"
  // We'll sample at start of each transition to preserve consistent behavior.
  if (nowMs == autoTransitionStartMs) {
    // first call handled by setup mode start
  }

  // If first transition, initialize
  if (nowMs - autoTransitionStartMs >= TRANSITION_TIME_MS || autoTransitionStartMs == 0) {
    // Start new transition with updated range endpoints from pots
    autoMinB = readPotMapped255(POT2_PIN);
    autoMaxB = readPotMapped255(POT3_PIN);
    if (autoMinB > autoMaxB) {
      int t = autoMinB;
      autoMinB = autoMaxB;
      autoMaxB = t;
    }

    // Determine where we are at the moment this boundary occurs
    // The previous transition should have ended at one endpoint.
    // Use whether we were previously starting from min to decide next direction.
    // If autoStartB equals min, we were going up; otherwise going down.
    bool lastWasStartingFromMin = (autoStartB == autoMinB) ? true : false;

    // If we can't infer due to range change, we still set a coherent start:
    // choose start endpoint based on last known autoStartB relative to new endpoints.
    // If autoStartB is closer to min, start from min; else start from max.
    int diffToMin = (autoStartB >= 0) ? abs(autoStartB - autoMinB) : 0;
    int diffToMax = (autoStartB >= 0) ? abs(autoStartB - autoMaxB) : 255;
    bool startFromMin = (diffToMin <= diffToMax);

    autoStartB = startFromMin ? autoMinB : autoMaxB;
    autoTransitionStartMs = nowMs;
  }

  unsigned long elapsed = nowMs - autoTransitionStartMs;
  if (elapsed > TRANSITION_TIME_MS) elapsed = TRANSITION_TIME_MS;

  // Determine direction based on start endpoint
  int fromB = autoStartB;
  int toB = (fromB == autoMinB) ? autoMaxB : autoMinB;

  // Smooth linear transition between endpoints
  // brightness = from + (to-from) * elapsed/TRANSITION
  long delta = (long)toB - (long)fromB;
  int b = (int)((long)fromB + (delta * (long)elapsed) / (long)TRANSITION_TIME_MS);

  // Ensure within range
  if (b < 0) b = 0;
  if (b > 255) b = 255;

  analogWrite(LED_PIN, b);
}

bool debounceButton(uint8_t pin, bool &stablePressed, bool &lastReading, unsigned long &lastChangeMs, unsigned long nowMs) {
  bool reading = (digitalRead(pin) == HIGH);

  if (reading != lastReading) {
    lastReading = reading;
    lastChangeMs = nowMs;
  }

  if ((nowMs - lastChangeMs) >= DEBOUNCE_DELAY_MS) {
    if (reading != stablePressed) {
      stablePressed = reading;
      return true; // state changed after debounce
    }
  }
  return false;
}

bool canSwitchModes(unsigned long nowMs) {
  return (nowMs - lastModeSwitchMs) >= MIN_MODE_TIME_MS;
}

void switchToMode(Mode newMode, unsigned long nowMs) {
  mode = newMode;
  lastModeSwitchMs = nowMs;

  if (mode == MANUAL) {
    // Ensure LED updates immediately in Manual mode
    applyManualBrightness();
  } else {
    // Initialize Auto transition timing; sample new endpoints
    autoMinB = readPotMapped255(POT2_PIN);
    autoMaxB = readPotMapped255(POT3_PIN);
    if (autoMinB > autoMaxB) {
      int t = autoMinB;
      autoMinB = autoMaxB;
      autoMaxB = t;
    }

    // Start from current LED value if possible; otherwise choose min
    // We'll choose min by default to keep direction consistent.
    autoStartB = autoMinB;
    autoTransitionStartMs = nowMs;
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);

  // Buttons use internal pullups because wiring shows switch to GND.
  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);

  // Pots
  analogReference(DEFAULT);

  // Per requirement: LED off at power on/reset and start in Manual Mode
  analogWrite(LED_PIN, 0);

  mode = MANUAL;
  lastModeSwitchMs = millis();

  // Initialize debounce variables
  unsigned long nowMs = millis();

  btn1LastReading = (digitalRead(BTN1_PIN) == HIGH);
  btn2LastReading = (digitalRead(BTN2_PIN) == HIGH);

  btn1StablePressed = btn1LastReading;
  btn2StablePressed = btn2LastReading;

  btn1LastChangeMs = nowMs;
  btn2LastChangeMs = nowMs;

  // Auto variables init
  autoMinB = readPotMapped255(POT2_PIN);
  autoMaxB = readPotMapped255(POT3_PIN);
  if (autoMinB > autoMaxB) {
    int t = autoMinB;
    autoMinB = autoMaxB;
    autoMaxB = t;
  }
  autoStartB = autoMinB;
  autoTransitionStartMs = nowMs;
}

void loop() {
  unsigned long nowMs = millis();

  // Debounce both buttons
  debounceButton(BTN1_PIN, btn1StablePressed, btn1LastReading, btn1LastChangeMs, nowMs);
  debounceButton(BTN2_PIN, btn2StablePressed, btn2LastReading, btn2LastChangeMs, nowMs);

  // Button actions:
  // btn1 press -> Manual -> Auto
  // btn2 press -> Auto -> Manual
  // "Pressing" interpreted as stable HIGH state (with INPUT_PULLUP, press = LOW).
  // We currently treat stablePressed as reading==HIGH, so pressed is LOW.
  bool btn1Pressed = (digitalRead(BTN1_PIN) == LOW);
  bool btn2Pressed = (digitalRead(BTN2_PIN) == LOW);

  if (canSwitchModes(nowMs)) {
    if (mode == MANUAL) {
      if (btn1Pressed) {
        switchToMode(AUTO, nowMs);
      }
    } else { // AUTO
      if (btn2Pressed) {
        switchToMode(MANUAL, nowMs);
      }
    }
  }

  // Mode behavior
  if (mode == MANUAL) {
    applyManualBrightness();
    // Static brightness; just keep updating based on pot1.
    delay(10); // small loop pacing, does not affect required timing
  } else {
    updateAutoBrightness(nowMs);
    // No extra delay to keep smooth transitions
  }
}