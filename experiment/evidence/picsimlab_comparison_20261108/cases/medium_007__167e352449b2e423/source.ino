#include <Arduino.h>
#include <TM1637Display.h>

// Pin assignments (as required)
static const uint8_t TRIG_PIN = D8;  // HC-SR04 TRIG
static const uint8_t ECHO_PIN = D9;  // HC-SR04 ECHO

static const uint8_t TM_CLK_PIN = D4; // TM1637 CLK
static const uint8_t TM_DIO_PIN = D5; // TM1637 DIO

// Display: show integer centimeters, blanks on invalid/timeout/range error.
TM1637Display display(TM_CLK_PIN, TM_DIO_PIN);

// Timing requirements
static const uint32_t ECHO_TIMEOUT_US = 25000UL;   // per requirement
static const uint32_t MIN_START_GAP_MS = 60UL;    // sample start interval 60..160ms
static const uint32_t MAX_START_GAP_MS = 160UL;

// Quantization / acceptance: show integer cm
// Convert: distance_cm = echo_us / 58 (integer quantization).
static const int16_t VALID_CM_MIN = 2;   // 2..400cm valid range
static const int16_t VALID_CM_MAX = 400; // 400cm valid range

// For scheduling / responsiveness within 250ms:
static uint32_t nextStartMs = 0;

// Internal display state
static bool hasValid = false;
static int16_t lastCm = 0;

// Convert echo microseconds to distance in cm (integer quantization)
static inline int16_t echoUsToCm(uint32_t echo_us) {
  return (int16_t)(echo_us / 58UL);
}

// Update display: blank for invalid else print integer cm
static void updateDisplay() {
  if (!hasValid) {
    // Blank display (no digits). TM1637 supports clearing segments:
    // clear() clears all digits.
    display.clear();
    return;
  }

  // Show integer cm with no leading zeros.
  // 4 digits display is enough for 400.
  // encode as number; keep behavior simple and consistent.
  // Set without leading zeros by letting library handle formatting.
  display.showNumberDec((int)lastCm, false, 4, 0);
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Initialize TM1637
  display.setBrightness(0x0f); // default bright, can be adjusted if desired

  // Initial display: blank
  hasValid = false;
  lastCm = 0;
  display.clear();

  // Schedule first measurement soon after start (within required timing envelope).
  uint32_t now = millis();
  nextStartMs = now + MIN_START_GAP_MS;
}

void loop() {
  uint32_t nowMs = millis();

  // Enforce sample start interval between 60..160ms.
  // Choose a variable interval within that range while keeping behavior continuous.
  if ((int32_t)(nowMs - nextStartMs) < 0) {
    // Not time yet
    return;
  }

  // Prepare next interval immediately so measurement+display time is included naturally.
  // Ensure that overall change reflects within 250ms; worst-case here is:
  // - measurement wait includes 25000us timeout + normal echo time (~few ms)
  // - display transfer is short
  // - interval is <=160ms
  // This is within 250ms as required.
  uint32_t intervalMs = MIN_START_GAP_MS;
  // Deterministic-ish variation without external RNG.
  // Uses micros() LSB to vary within allowed range.
  intervalMs += (uint32_t)(micros() & 0x7F); // 0..127
  if (intervalMs > MAX_START_GAP_MS) intervalMs = MAX_START_GAP_MS;
  nextStartMs = nowMs + intervalMs;

  // Trigger measurement (HC-SR04 standard: 10us HIGH pulse)
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Measure echo pulse width; must have 25000us timeout per requirement.
  // pulseIn returns:
  //  - duration in microseconds (unsigned long) if pulse detected
  //  - 0 if timeout
  unsigned long echo_us = pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);

  // Determine validity:
  // - no echo (echo_us==0) => invalid => display blank
  // - convert to cm using integer division by 58
  // - if out of 2..400 => invalid => display blank
  // - must not treat timeout as 0cm
  if (echo_us == 0UL) {
    hasValid = false;
  } else {
    int16_t cm = echoUsToCm(echo_us);
    if (cm < VALID_CM_MIN || cm > VALID_CM_MAX) {
      hasValid = false;
    } else {
      hasValid = true;
      lastCm = cm;
    }
  }

  // Update display after each measurement.
  updateDisplay();
}