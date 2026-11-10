#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision36\\cases\\pceval_clock\\arduino\\sketch\\sketch.ino"
#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>

// 4-digit common-anode 7-segment display.
const uint8_t DIGIT_PINS[4] = {2, 3, 4, 5};
const uint8_t SEGMENT_PINS[7] = {6, 7, 8, 9, 10, 11, 12};
const uint8_t COLON_PIN = 13;
RTC_DS1307 rtc;

// Bits 0 through 6 correspond to segments A through G.
// A set bit means that segment must be driven LOW.
const uint8_t NUMERAL_MASKS[10] = {
  0x3F,  // 0
  0x06,  // 1
  0x5B,  // 2
  0x4F,  // 3
  0x66,  // 4
  0x6D,  // 5
  0x7D,  // 6
  0x07,  // 7
  0x7F,  // 8
  0x6F   // 9
};

uint8_t displayMasks[4] = {0x40, 0x40, 0x40, 0x40};
uint8_t nextDigit = 0;

const unsigned long DIGIT_INTERVAL_US = 2000;
const unsigned long RTC_POLL_INTERVAL_MS = 250;

unsigned long lastDigitUpdate = 0;
unsigned long lastRtcPoll = 0;

#line 35 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision36\\cases\\pceval_clock\\arduino\\sketch\\sketch.ino"
void disableDigits();
#line 41 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision36\\cases\\pceval_clock\\arduino\\sketch\\sketch.ino"
bool readRtcTime(uint8_t &hours, uint8_t &minutes);
#line 49 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision36\\cases\\pceval_clock\\arduino\\sketch\\sketch.ino"
void updateTimeDisplay();
#line 67 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision36\\cases\\pceval_clock\\arduino\\sketch\\sketch.ino"
void refreshDisplay();
#line 89 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision36\\cases\\pceval_clock\\arduino\\sketch\\sketch.ino"
void setup();
#line 114 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision36\\cases\\pceval_clock\\arduino\\sketch\\sketch.ino"
void loop();
#line 35 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision36\\cases\\pceval_clock\\arduino\\sketch\\sketch.ino"
void disableDigits() {
  for (uint8_t i = 0; i < 4; ++i) {
    digitalWrite(DIGIT_PINS[i], LOW);
  }
}

bool readRtcTime(uint8_t &hours, uint8_t &minutes) {
  if (!rtc.begin()) return false;
  DateTime current=rtc.now();
  hours=current.hour();
  minutes=current.minute();
  return current.isValid();
}

void updateTimeDisplay() {
  uint8_t hours;
  uint8_t minutes;

  if (readRtcTime(hours, minutes)) {
    displayMasks[0] = NUMERAL_MASKS[hours / 10];
    displayMasks[1] = NUMERAL_MASKS[hours % 10];
    displayMasks[2] = NUMERAL_MASKS[minutes / 10];
    displayMasks[3] = NUMERAL_MASKS[minutes % 10];
  } else {
    // Do not present an invented time when the RTC is unavailable.
    // Subsequent polls automatically recover when it becomes available.
    for (uint8_t i = 0; i < 4; ++i) {
      displayMasks[i] = 0x40;  // Dash
    }
  }
}

void refreshDisplay() {
  const unsigned long now = micros();
  if (now - lastDigitUpdate < DIGIT_INTERVAL_US) {
    return;
  }
  lastDigitUpdate = now;

  // Blank all digits before changing segments to prevent ghosting.
  disableDigits();

  const uint8_t mask = displayMasks[nextDigit];
  for (uint8_t segment = 0; segment < 7; ++segment) {
    digitalWrite(
      SEGMENT_PINS[segment],
      (mask & (uint8_t(1) << segment)) ? LOW : HIGH
    );
  }

  digitalWrite(DIGIT_PINS[nextDigit], HIGH);
  nextDigit = (nextDigit + 1) % 4;
}

void setup() {
  // On the Uno, Wire uses SDA=A4 and SCL=A5.
  Wire.begin();
  Wire.setClock(100000);

#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000UL, true);
#endif

  // Check and initialize the RTC while preserving its configured time.
  updateTimeDisplay();

  for (uint8_t pin = 2; pin <= 13; ++pin) {
    // Digits inactive LOW; segments and colon initially inactive HIGH.
    digitalWrite(pin, pin <= 5 ? LOW : HIGH);
    pinMode(pin, OUTPUT);
  }

  // COM is wired to 5V, so the colon is continuously active LOW.
  digitalWrite(COLON_PIN, LOW);

  lastRtcPoll = millis();
  lastDigitUpdate = micros() - DIGIT_INTERVAL_US;
}

void loop() {
  const unsigned long now = millis();

  if (now - lastRtcPoll >= RTC_POLL_INTERVAL_MS) {
    lastRtcPoll = now;

    // Avoid leaving a single digit illuminated during an I2C transaction.
    disableDigits();
    updateTimeDisplay();
    lastDigitUpdate = micros() - DIGIT_INTERVAL_US;
  }

  refreshDisplay();
}
