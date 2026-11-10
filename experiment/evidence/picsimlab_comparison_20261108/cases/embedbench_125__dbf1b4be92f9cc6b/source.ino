#include <Arduino.h>

static const uint8_t LED1_PIN = 2;
static const uint8_t LED2_PIN = 3;
static const uint8_t LED3_PIN = 4;

static const uint8_t BTN_PIN  = 5;  // btn1:1.l -> mega:5, btn1:2.r -> GND

// RGB1
static const uint8_t RGB1_R_PIN = 6;
static const uint8_t RGB1_G_PIN = 7;
static const uint8_t RGB1_B_PIN = 8;

// RGB2
static const uint8_t RGB2_R_PIN = 9;
static const uint8_t RGB2_G_PIN = 10;
static const uint8_t RGB2_B_PIN = 11;

// RGB3
static const uint8_t RGB3_R_PIN = 12;
static const uint8_t RGB3_G_PIN = 13;
static const uint8_t RGB3_B_PIN = 44;

enum Mode : uint8_t { MODE_1 = 1, MODE_2 = 2, MODE_3 = 3, MODE_4 = 4 };

static Mode currentMode = MODE_4;

static unsigned long lastModeChangeMs = 0;

// Button debouncing
static int lastBtnReading = HIGH;
static int stableBtnState = HIGH;
static unsigned long lastDebounceMs = 0;
static const unsigned long DEBOUNCE_MS = 25;

// Must be maintained at least 2 seconds before cycling
static const unsigned long MIN_MODE_HOLD_MS = 2000;

// Smooth rainbow transition (Mode 2)
static unsigned long lastRainbowUpdateMs = 0;
static const unsigned long RAINBOW_STEP_MS = 20; // smoothness

// A phase that moves over time
static float rainbowPhase = 0.0f;

static void setSingleLEDsOff() {
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, LOW);
}

static void setAllRGBOff() {
  // Common cathode: Arduino HIGH turns LED ON (through series resistor in typical wiring)
  analogWrite(RGB1_R_PIN, 0);
  analogWrite(RGB1_G_PIN, 0);
  analogWrite(RGB1_B_PIN, 0);

  analogWrite(RGB2_R_PIN, 0);
  analogWrite(RGB2_G_PIN, 0);
  analogWrite(RGB2_B_PIN, 0);

  analogWrite(RGB3_R_PIN, 0);
  analogWrite(RGB3_G_PIN, 0);
  analogWrite(RGB3_B_PIN, 0);
}

static void rgbSetAll(uint8_t r, uint8_t g, uint8_t b) {
  analogWrite(RGB1_R_PIN, r); analogWrite(RGB1_G_PIN, g); analogWrite(RGB1_B_PIN, b);
  analogWrite(RGB2_R_PIN, r); analogWrite(RGB2_G_PIN, g); analogWrite(RGB2_B_PIN, b);
  analogWrite(RGB3_R_PIN, r); analogWrite(RGB3_G_PIN, g); analogWrite(RGB3_B_PIN, b);
}

static void rgbSetRGB1(uint8_t r, uint8_t g, uint8_t b) {
  analogWrite(RGB1_R_PIN, r);
  analogWrite(RGB1_G_PIN, g);
  analogWrite(RGB1_B_PIN, b);
}
static void rgbSetRGB2(uint8_t r, uint8_t g, uint8_t b) {
  analogWrite(RGB2_R_PIN, r);
  analogWrite(RGB2_G_PIN, g);
  analogWrite(RGB2_B_PIN, b);
}
static void rgbSetRGB3(uint8_t r, uint8_t g, uint8_t b) {
  analogWrite(RGB3_R_PIN, r);
  analogWrite(RGB3_G_PIN, g);
  analogWrite(RGB3_B_PIN, b);
}

static void applyModeIndicatorLEDs(Mode m) {
  setSingleLEDsOff();

  // Mode indicator rules:
  // Mode 1: led1 will turn on in sequence to indicate the mode.
  // Mode 3: led3 will turn on in sequence to indicate the mode.
  // Mode 2: no explicit LED mention other than "clearly indicate current mode as described above"
  // We'll light led2 for Mode 2 to make it unambiguous and "clearly indicate".
  // Mode 4: all off.
  if (m == MODE_2) {
    digitalWrite(LED2_PIN, HIGH);
  } else if (m == MODE_4) {
    // all off
  } else {
    // handled by sequence updater in their mode handlers
  }
}

static void runIndicatorSequenceForLed1() {
  // "led1 will turn on in sequence" -> we'll turn on led1 briefly, then off, then on again,
  // to indicate "sequence" clearly while keeping others off.
  setSingleLEDsOff();
  digitalWrite(LED1_PIN, HIGH);
  delay(120);
  digitalWrite(LED1_PIN, LOW);
  delay(120);
  digitalWrite(LED1_PIN, HIGH);
  delay(120);
  digitalWrite(LED1_PIN, LOW);
}

static void runIndicatorSequenceForLed3() {
  setSingleLEDsOff();
  digitalWrite(LED3_PIN, HIGH);
  delay(120);
  digitalWrite(LED3_PIN, LOW);
  delay(120);
  digitalWrite(LED3_PIN, HIGH);
  delay(120);
  digitalWrite(LED3_PIN, LOW);
}

static void handleMode1() {
  // Mode 1: RGB LEDs display red, green, blue respectively.
  // led1 indicates the mode in sequence.
  runIndicatorSequenceForLed1();

  setAllRGBOff();
  rgbSetRGB1(255, 0, 0);   // red
  rgbSetRGB2(0, 255, 0);   // green
  rgbSetRGB3(0, 0, 255);   // blue
}

static void rainbowComponentFromPhase(float phase) {
  // Not a real function; placeholder to keep structure clear.
}

static void handleMode2() {
  // Mode 2: RGB LEDs display a rainbow effect (yellow, cyan, purple).
  // Smooth transition between colors in Mode 2.
  //
  // We'll map "rainbowPhase" over time and generate smooth components:
  // Yellow:   R + G
  // Cyan:     G + B
  // Purple:   R + B
  //
  // We compute a smooth fade between these targets:
  //   For each RGB LED, rotate phase offsets to create continuous motion.
  unsigned long now = millis();
  if (now - lastRainbowUpdateMs >= RAINBOW_STEP_MS) {
    lastRainbowUpdateMs = now;

    // Advance phase at a rate that feels smooth and lasts within mode hold.
    // Phase wraps at 1.0
    rainbowPhase += 0.0035f;
    if (rainbowPhase > 1.0f) rainbowPhase -= 1.0f;
  }

  // Convert phase into weights between three color targets:
  // We use triangular waves shifted by offsets for smooth transitions.
  // weightX in [0..1]
  auto triWave = [](float x) -> float {
    // x in [0..1)
    // Triangular wave: 0->1->0
    if (x < 0.5f) return x * 2.0f;
    return (1.0f - x) * 2.0f;
  };

  // For LED1: yellow -> ...
  float p1 = rainbowPhase;          // 0
  float p1b = (rainbowPhase + 1.0f / 3.0f); // 120 deg
  float p1c = (rainbowPhase + 2.0f / 3.0f); // 240 deg

  float wYellow1 = triWave(p1);     // strongest near phase 0
  float wCyan1   = triWave(p1b);    // strongest near phase 1/3
  float wPurple1 = triWave(p1c);   // strongest near phase 2/3

  // For LED2: shift slightly for variety (still "rainbow effect")
  float p2 = rainbowPhase + 0.12f;
  float p2b = p2 + 1.0f / 3.0f;
  float p2c = p2 + 2.0f / 3.0f;
  if (p2b > 1.0f) p2b -= 1.0f;
  if (p2c > 1.0f) p2c -= 1.0f;

  float wYellow2 = triWave(p2 < 1.0f ? p2 : p2 - 1.0f);
  float wCyan2   = triWave(p2b);
  float wPurple2 = triWave(p2c);

  // For LED3: another shift
  float p3 = rainbowPhase + 0.25f;
  if (p3 > 1.0f) p3 -= 1.0f;
  float p3b = p3 + 1.0f / 3.0f;
  float p3c = p3 + 2.0f / 3.0f;
  if (p3b > 1.0f) p3b -= 1.0f;
  if (p3c > 1.0f) p3c -= 1.0f;

  float wYellow3 = triWave(p3);
  float wCyan3   = triWave(p3b);
  float wPurple3 = triWave(p3c);

  auto clamp255 = [](float v) -> uint8_t {
    if (v < 0) v = 0;
    if (v > 255) v = 255;
    return (uint8_t)(v + 0.5f);
  };

  // Targets:
  // Yellow:  (255, 255, 0)
  // Cyan:    (0, 255, 255)
  // Purple:  (255, 0, 255)
  auto blendRGB = [&](float wY, float wC, float wP, uint8_t &r, uint8_t &g, uint8_t &b) {
    // Normalize weights lightly to keep brightness consistent.
    float sum = wY + wC + wP;
    if (sum < 0.0001f) sum = 1.0f;
    float nY = wY / sum, nC = wC / sum, nP = wP / sum;

    float rf = 255.0f * (nY + nP);     // yellow R + purple R
    float gf = 255.0f * (nY + nC);     // yellow G + cyan G
    float bf = 255.0f * (nC + nP);     // cyan B + purple B

    r = clamp255(rf);
    g = clamp255(gf);
    b = clamp255(bf);
  };

  uint8_t r1, g1, b1, r2, g2, b2, r3, g3, b3;
  blendRGB(wYellow1, wCyan1, wPurple1, r1, g1, b1);
  blendRGB(wYellow2, wCyan2, wPurple2, r2, g2, b2);
  blendRGB(wYellow3, wCyan3, wPurple3, r3, g3, b3);

  rgbSetRGB1(r1, g1, b1);
  rgbSetRGB2(r2, g2, b2);
  rgbSetRGB3(r3, g3, b3);

  applyModeIndicatorLEDs(MODE_2);
}

static void handleMode3() {
  // Mode 3: warm white color (equal red, green, blue values).
  // led3 indicates the mode in sequence.
  runIndicatorSequenceForLed3();

  setAllRGBOff();
  // Warm white -> equal RGB values (as per requirement)
  rgbSetAll(255, 255, 255);

  // Ensure indicator off during/after sequence; sequence already toggled.
  setSingleLEDsOff();
}

static void handleMode4() {
  // Mode 4: All LEDs off.
  setSingleLEDsOff();
  setAllRGBOff();
}

static void setMode(Mode m) {
  currentMode = m;
  lastModeChangeMs = millis();
  // Apply the mode immediately once when switching
  if (currentMode == MODE_1) {
    handleMode1();
  } else if (currentMode == MODE_2) {
    applyModeIndicatorLEDs(MODE_2);
    // Start rainbow from current phase; ensure some initial off->on transition is consistent
    // We'll set some initial colors by calling handler once.
    handleMode2();
  } else if (currentMode == MODE_3) {
    handleMode3();
  } else {
    handleMode4();
  }
}

void setup() {
  // Single-color LED pins
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);

  // Button pin: external connection to GND at btn1:2.r and signal at btn1:1.l to mega:5
  pinMode(BTN_PIN, INPUT_PULLUP);

  // RGB pins (use PWM-capable pins)
  pinMode(RGB1_R_PIN, OUTPUT);
  pinMode(RGB1_G_PIN, OUTPUT);
  pinMode(RGB1_B_PIN, OUTPUT);

  pinMode(RGB2_R_PIN, OUTPUT);
  pinMode(RGB2_G_PIN, OUTPUT);
  pinMode(RGB2_B_PIN, OUTPUT);

  pinMode(RGB3_R_PIN, OUTPUT);
  pinMode(RGB3_G_PIN, OUTPUT);
  pinMode(RGB3_B_PIN, OUTPUT);

  // Ensure everything off on boot/reset
  setSingleLEDsOff();
  setAllRGBOff();

  // Initial mode is off (Mode 4)
  currentMode = MODE_4;
  lastModeChangeMs = millis();

  lastBtnReading = digitalRead(BTN_PIN);
  stableBtnState = lastBtnReading;
  lastDebounceMs = millis();

  lastRainbowUpdateMs = millis();
  rainbowPhase = 0.0f;
}

void loop() {
  // Read and debounce button
  int reading = digitalRead(BTN_PIN);

  if (reading != lastBtnReading) {
    lastDebounceMs = millis();
    lastBtnReading = reading;
  }

  if (millis() - lastDebounceMs >= DEBOUNCE_MS) {
    if (reading != stableBtnState) {
      stableBtnState = reading;

      // With INPUT_PULLUP, pressed is LOW
      if (stableBtnState == LOW) {
        unsigned long now = millis();
        if (now - lastModeChangeMs >= MIN_MODE_HOLD_MS) {
          // Cycle modes 1->2->3->4->1...
          Mode next;
          if (currentMode == MODE_1) next = MODE_2;
          else if (currentMode == MODE_2) next = MODE_3;
          else if (currentMode == MODE_3) next = MODE_4;
          else next = MODE_1;

          setMode(next);
        }
      }
    }
  }

  // Maintain mode behavior while in each mode
  if (currentMode == MODE_2) {
    // Keep smooth rainbow transitions while Mode 2 is active
    handleMode2();
  } else {
    // For other modes, we already applied their state when switching.
    // Keep them stable without re-triggering indicator sequences.
    // Mode 1/3: ensure RGB and indicators stay as set by the last mode application.
    // However, indicator sequences include delays and end with indicators off per our implementation.
    // Requirement says "led1/led3 will turn on in sequence to indicate the mode." We did it during switching.
    // To keep RGB stable, enforce again if desired; but avoid replaying sequences.
    static Mode lastApplied = MODE_4;
    if (lastApplied != currentMode) {
      // Should not happen because setMode applies, but included for safety.
      if (currentMode == MODE_1) {
        setAllRGBOff();
        rgbSetRGB1(255, 0, 0);
        rgbSetRGB2(0, 255, 0);
        rgbSetRGB3(0, 0, 255);
      } else if (currentMode == MODE_3) {
        setAllRGBOff();
        rgbSetAll(255, 255, 255);
      } else if (currentMode == MODE_4) {
        setSingleLEDsOff();
        setAllRGBOff();
      }
      lastApplied = currentMode;
    }
  }
}