#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_clock\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Wire.h>
#include <RTClib.h>

static const uint8_t DIG1_PIN = 2;
static const uint8_t DIG2_PIN = 3;
static const uint8_t DIG3_PIN = 4;
static const uint8_t DIG4_PIN = 5;

static const uint8_t SEG_A_PIN = 6;
static const uint8_t SEG_B_PIN = 7;
static const uint8_t SEG_C_PIN = 8;
static const uint8_t SEG_D_PIN = 9;
static const uint8_t SEG_E_PIN = 10;
static const uint8_t SEG_F_PIN = 11;
static const uint8_t SEG_G_PIN = 12;
static const uint8_t COLON_PIN = 13;

// Common anode: segment turns ON when pin is LOW.
// Digit activates when digit pins are HIGH.
static const uint8_t digitPins[4] = {DIG1_PIN, DIG2_PIN, DIG3_PIN, DIG4_PIN};
static const uint8_t segPins[7] = {SEG_A_PIN, SEG_B_PIN, SEG_C_PIN, SEG_D_PIN, SEG_E_PIN, SEG_F_PIN, SEG_G_PIN};

// Segment patterns for 0-9 (A,B,C,D,E,F,G). 1=OFF (HIGH), 0=ON (LOW).
static const uint8_t segOnOffForNumber[10][7] = {
  // 0
  {0,0,0,0,0,0,1},
  // 1
  {1,0,0,1,1,1,1},
  // 2
  {0,0,1,0,0,1,0},
  // 3
  {0,0,0,0,1,1,0},
  // 4
  {1,0,0,1,1,0,0},
  // 5
  {0,1,0,0,1,0,0},
  // 6
  {0,1,0,0,0,0,0},
  // 7
  {0,0,0,1,1,1,1},
  // 8
  {0,0,0,0,0,0,0},
  // 9
  {0,0,0,0,1,0,0}
};

#line 47 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_clock\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setAllDigitsInactive();
#line 51 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_clock\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setColon(bool on);
#line 56 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_clock\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setSegmentsForNumber(int n);
#line 68 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_clock\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void showDigitAt(uint8_t position, int n);
#line 75 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_clock\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void displayHHMM(uint8_t h, uint8_t m);
#line 99 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_clock\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 124 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_clock\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 47 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_clock\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static inline void setAllDigitsInactive() {
  for (uint8_t i = 0; i < 4; i++) digitalWrite(digitPins[i], LOW);
}

static inline void setColon(bool on) {
  // Colon LED: common anode display; so ON requires LOW.
  digitalWrite(COLON_PIN, on ? LOW : HIGH);
}

static inline void setSegmentsForNumber(int n) {
  if (n < 0 || n > 9) {
    // Turn all segments off (HIGH).
    for (uint8_t s = 0; s < 7; s++) digitalWrite(segPins[s], HIGH);
    return;
  }

  for (uint8_t s = 0; s < 7; s++) {
    digitalWrite(segPins[s], segOnOffForNumber[n][s] ? HIGH : LOW);
  }
}

static inline void showDigitAt(uint8_t position, int n) {
  // position: 0..3 corresponds to DIG1..DIG4
  setAllDigitsInactive();
  setSegmentsForNumber(n);
  digitalWrite(digitPins[position], HIGH);
}

static inline void displayHHMM(uint8_t h, uint8_t m) {
  uint8_t digits[4];
  digits[0] = (h / 10) % 10;
  digits[1] = h % 10;
  digits[2] = (m / 10) % 10;
  digits[3] = m % 10;

  // Simple multiplexing
  // Keep colon on; colon pin is independent of digit enable.
  setColon(true);

  // Turn on each digit briefly
  // (delays small to maintain brightness)
  for (uint8_t pos = 0; pos < 4; pos++) {
    showDigitAt(pos, digits[pos]);
    delay(2);
  }

  // Leave digits off afterward (will be re-activated next call)
  setAllDigitsInactive();
}

RTC_DS1307 rtc;

void setup() {
  Wire.begin();

  // Pin modes: 2 ~ 13 as OUTPUT
  for (uint8_t p = 2; p <= 13; p++) pinMode(p, OUTPUT);

  // Colon led on at power-up
  setColon(true);

  // Initialize display pins to safe (digits inactive, segments off).
  setAllDigitsInactive();
  for (uint8_t s = 0; s < 7; s++) digitalWrite(segPins[s], HIGH);

  // 1. Check if RTC module is available and initialize it
  if (!rtc.begin()) {
    // If RTC not found, just keep display colon and loop (no further behavior specified).
    return;
  }

  // If RTC lost power, set it? (Circuit description doesn't request setting;
  // keep behavior minimal: only initialize if available.)
  // However, DS1307 on many platforms retains time; no explicit set requested.
  // We'll avoid writing the clock.
}

void loop() {
  // 4. Continuously retrieve the current time from the RTC
  DateTime now = rtc.now();

  // 5. Display current time in format "HH:MM"
  displayHHMM((uint8_t)now.hour(), (uint8_t)now.minute());
}
