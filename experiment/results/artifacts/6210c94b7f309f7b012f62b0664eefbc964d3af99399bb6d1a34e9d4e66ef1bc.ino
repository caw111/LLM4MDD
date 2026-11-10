#include <Wire.h>
#include <RTClib.h>

// ----- Pin mapping (preserve wiring) -----
// Digits
static const uint8_t DIG_PINS[4] = {2, 3, 4, 5};   // DIG1..DIG4
// Segments (common anode; segment turns ON when pin reads LOW)
static const uint8_t SEG_PINS[7] = {6, 7, 8, 9, 10, 11, 12}; // A..G

static const uint8_t COLON_PIN = 13; // colon LED

// Segment order: A B C D E F G
// For 7-seg common anode:
// - Segment ON => output LOW
// - Segment OFF => output HIGH
static const uint8_t DIGIT_SEGMENTS[10][7] = {
  // A, B, C, D, E, F, G  (true = ON)
  {true,  true,  true,  true,  true,  true,  false}, // 0
  {false, true,  true,  false, false, false, false}, // 1
  {true,  true,  false, true,  true,  false, true }, // 2
  {true,  true,  true,  true,  false, false, true }, // 3
  {false, true,  true,  false, false, true,  true }, // 4
  {true,  false, true,  true,  false, true,  true }, // 5
  {true,  false, true,  true,  true,  true,  true }, // 6
  {true,  true,  true,  false, false, false, false}, // 7
  {true,  true,  true,  true,  true,  true,  true }, // 8
  {true,  true,  true,  true,  false, true,  true }  // 9
};

static const uint8_t ALL_OFF_SEG[7] = {false, false, false, false, false, false, false};

RTC_DS1307 rtc;

// Display state (updated in loop, rendered via multiplexing)
static volatile uint8_t g_digits[4] = {0, 0, 0, 0};
static volatile bool g_colon = true;

static unsigned long g_lastTimeReadMs = 0;
static unsigned long g_lastMuxUpdateUs = 0;
static uint8_t g_activeDigit = 0;

static inline void setSegmentsForDigit(uint8_t d) {
  if (d > 9) d = 0;
  for (uint8_t i = 0; i < 7; i++) {
    // common anode: ON = LOW, OFF = HIGH
    digitalWrite(SEG_PINS[i], DIGIT_SEGMENTS[d][i] ? LOW : HIGH);
  }
}

static inline void setAllSegmentsOff() {
  for (uint8_t i = 0; i < 7; i++) {
    digitalWrite(SEG_PINS[i], HIGH); // OFF for common anode
  }
}

static inline void activateDigit(uint8_t idx /*0..3*/) {
  // digits are activated when digit pins are HIGH
  for (uint8_t i = 0; i < 4; i++) {
    digitalWrite(DIG_PINS[i], (i == idx) ? HIGH : LOW);
  }
}

static void multiplexRenderStep() {
  // Multiplexing: update one digit per call
  // Turn off all digits briefly to avoid ghosting
  for (uint8_t i = 0; i < 4; i++) digitalWrite(DIG_PINS[i], LOW);

  // Set segments for the currently active digit
  uint8_t d = g_digits[g_activeDigit];
  setSegmentsForDigit(d);

  // Turn on desired digit
  activateDigit(g_activeDigit);

  // Colon is controlled separately; keep it always set
  digitalWrite(COLON_PIN, g_colon ? LOW : HIGH); // assume common anode style: ON=LOW
  // (Colon pin wiring uses same LED behavior as segments described)

  g_activeDigit = (g_activeDigit + 1) & 0x03;
}

void setup() {
  // 1. Check if RTC module is available and initialize it
  Wire.begin();

  if (!rtc.begin()) {
    // If RTC isn't available, still proceed to pin init + display (time will remain default)
  } else {
    // If running, ensure it starts; ds1307 doesn't support lostPower reset reliably in same manner,
    // but attempt typical behavior.
    if (rtc.lostPower()) {
      // Do not set time here because no requirement provided for initial time setting.
      // The simulator already has initTime configured.
    }
  }

  // 2. Set pin 2 ~ 13 mode as OUTPUT
  pinMode(COLON_PIN, OUTPUT);
  for (uint8_t i = 0; i < 4; i++) pinMode(DIG_PINS[i], OUTPUT);
  for (uint8_t i = 0; i < 7; i++) pinMode(SEG_PINS[i], OUTPUT);

  // Initialize outputs to safe state
  setAllSegmentsOff();
  for (uint8_t i = 0; i < 4; i++) digitalWrite(DIG_PINS[i], LOW);

  // 3. Turn on the colon led
  // Common anode: ON when pin reads LOW (requirement statement)
  g_colon = true;
  digitalWrite(COLON_PIN, LOW);

  // Initialize digit buffer
  for (uint8_t i = 0; i < 4; i++) g_digits[i] = 0;

  // Prime multiplex timing
  g_lastMuxUpdateUs = micros();
}

void loop() {
  // 5. Continuously retrieve the current time from the RTC
  // Keep a modest refresh rate for time reads; multiplexing continues continuously
  unsigned long nowMs = millis();
  if (nowMs - g_lastTimeReadMs >= 250) {
    g_lastTimeReadMs = nowMs;

    DateTime t = rtc.now(); // If RTC failed, this returns default (library behavior)

    // 5. Display in format "HH:MM" on 4 digits
    // digits: DIG1 DIG2 DIG3 DIG4 -> H H M M
    uint8_t hh = (uint8_t)t.hour();
    uint8_t mm = (uint8_t)t.minute();

    g_digits[0] = hh / 10;
    g_digits[1] = hh % 10;
    g_digits[2] = mm / 10;
    g_digits[3] = mm % 10;

    // 3/Expected behavior: colon stays on (requirement says turn on at power up; no blinking requirement)
    g_colon = true;
  }

  // Multiplex rendering (timed)
  unsigned long nowUs = micros();
  if (nowUs - g_lastMuxUpdateUs >= 2000) { // ~2ms per digit => ~125Hz full refresh
    g_lastMuxUpdateUs = nowUs;
    multiplexRenderStep();
  }
}