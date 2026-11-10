#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_026\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <stdint.h>

// Resolved wiring: the PWM lamp is on D6 (authoritative remap of D9).
const uint8_t LAMP_PIN = 6;
const uint8_t PIR_A_PIN = 2;
const uint8_t PIR_B_PIN = 3;
const uint8_t LIGHT_SENSOR_PIN = A0;
const uint8_t TM_CLK_PIN = 4;
const uint8_t TM_DIO_PIN = 5;

// Set false if the installed light-sensor divider produces lower readings
// as ambient illumination increases.
const bool LIGHT_READING_INCREASES_WITH_BRIGHTNESS = true;
const uint16_t DARK_ON_THRESHOLD = 350;
const uint16_t BRIGHT_OFF_THRESHOLD = 450;

const uint32_t LIGHT_SAMPLE_MS = 20;
const uint32_t PIR_DEBOUNCE_MS = 50;
const uint32_t LIGHT_HOLD_MS = 30000UL;
const uint32_t DIRECTION_WINDOW_MS = 15000UL;
const uint32_t BOTH_CLEAR_MS = 500;
const uint32_t FADE_STEP_MS = 10;
const uint8_t FADE_STEP = 5;
const uint8_t LAMP_BRIGHTNESS = 230;
const uint32_t SERIAL_REPORT_MS = 10000UL;
const uint32_t DAY_MS = 86400000UL;

// Without an RTC, "today" means successive 24-hour periods since startup.
// Counts and accumulated illumination time are RAM-based and reset on reboot.
uint32_t todayCount = 0;
uint32_t countAToB = 0;
uint32_t countBToA = 0;
uint32_t dayStartedAt = 0;

uint64_t totalLitMs = 0;
uint32_t lastAccountingAt = 0;
uint32_t lastReportAt = 0;
uint32_t lastMotionAt = 0;
bool motionSeen = false;

uint8_t lampPWM = 0;
uint32_t lastFadeAt = 0;

int32_t lightFilterQ8 = 0;
uint16_t lightReading = 0;
uint32_t lastLightSampleAt = 0;
bool ambientDark = false;

struct PirInput {
  uint8_t pin;
  bool raw;
  bool active;
  bool rose;
  uint32_t rawChangedAt;

  void begin(uint32_t now) {
    pinMode(pin, INPUT);
    raw = digitalRead(pin) == HIGH;
    active = false;
    rose = false;
    rawChangedAt = now;
  }

  void update(uint32_t now) {
    rose = false;
    bool sample = digitalRead(pin) == HIGH;

    if (sample != raw) {
      raw = sample;
      rawChangedAt = now;
    }

    if (active != raw &&
        (uint32_t)(now - rawChangedAt) >= PIR_DEBOUNCE_MS) {
      active = raw;
      rose = active;
    }
  }
};

PirInput pirA = {PIR_A_PIN, false, false, false, 0};
PirInput pirB = {PIR_B_PIN, false, false, false, 0};

enum PassageState : uint8_t {
  READY,
  A_FIRST,
  B_FIRST,
  WAIT_FOR_CLEAR
};

PassageState passageState = READY;
uint32_t firstDetectionAt = 0;
bool clearTimerRunning = false;
uint32_t clearStartedAt = 0;

// Minimal TM1637 driver: no external library required.
#line 98 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_026\\direct\\attempt_0\\sketch\\sketch.ino"
void tmDelay();
#line 102 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_026\\direct\\attempt_0\\sketch\\sketch.ino"
void tmDioLow();
#line 107 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_026\\direct\\attempt_0\\sketch\\sketch.ino"
void tmDioRelease();
#line 111 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_026\\direct\\attempt_0\\sketch\\sketch.ino"
void tmStart();
#line 120 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_026\\direct\\attempt_0\\sketch\\sketch.ino"
void tmStop();
#line 130 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_026\\direct\\attempt_0\\sketch\\sketch.ino"
void tmWriteByte(uint8_t value);
#line 152 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_026\\direct\\attempt_0\\sketch\\sketch.ino"
void tmDisplay(const uint8_t segments[4]);
#line 174 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_026\\direct\\attempt_0\\sketch\\sketch.ino"
void updateDisplay(uint32_t now);
#line 223 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_026\\direct\\attempt_0\\sketch\\sketch.ino"
void printUint64(uint64_t value);
#line 234 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_026\\direct\\attempt_0\\sketch\\sketch.ino"
void reportStatus();
#line 251 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_026\\direct\\attempt_0\\sketch\\sketch.ino"
void updateAmbientLight(uint32_t now);
#line 271 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_026\\direct\\attempt_0\\sketch\\sketch.ino"
void recordPassage(bool aToB);
#line 288 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_026\\direct\\attempt_0\\sketch\\sketch.ino"
void updatePassageCounter(uint32_t now);
#line 344 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_026\\direct\\attempt_0\\sketch\\sketch.ino"
void updateLamp(uint32_t now);
#line 381 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_026\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 415 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_026\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 98 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_026\\direct\\attempt_0\\sketch\\sketch.ino"
void tmDelay() {
  delayMicroseconds(5);
}

void tmDioLow() {
  digitalWrite(TM_DIO_PIN, LOW);
  pinMode(TM_DIO_PIN, OUTPUT);
}

void tmDioRelease() {
  pinMode(TM_DIO_PIN, INPUT_PULLUP);
}

void tmStart() {
  tmDioRelease();
  digitalWrite(TM_CLK_PIN, HIGH);
  tmDelay();
  tmDioLow();
  tmDelay();
  digitalWrite(TM_CLK_PIN, LOW);
}

void tmStop() {
  digitalWrite(TM_CLK_PIN, LOW);
  tmDioLow();
  tmDelay();
  digitalWrite(TM_CLK_PIN, HIGH);
  tmDelay();
  tmDioRelease();
  tmDelay();
}

void tmWriteByte(uint8_t value) {
  for (uint8_t i = 0; i < 8; ++i) {
    digitalWrite(TM_CLK_PIN, LOW);
    if (value & 1) {
      tmDioRelease();
    } else {
      tmDioLow();
    }
    tmDelay();
    digitalWrite(TM_CLK_PIN, HIGH);
    tmDelay();
    value >>= 1;
  }

  digitalWrite(TM_CLK_PIN, LOW);
  tmDioRelease();
  tmDelay();
  digitalWrite(TM_CLK_PIN, HIGH);
  tmDelay();  // ACK clock; absence of a display must not block the system.
  digitalWrite(TM_CLK_PIN, LOW);
}

void tmDisplay(const uint8_t segments[4]) {
  tmStart();
  tmWriteByte(0x40);  // Automatic address increment.
  tmStop();

  tmStart();
  tmWriteByte(0xC0);
  for (uint8_t i = 0; i < 4; ++i) {
    tmWriteByte(segments[i]);
  }
  tmStop();

  tmStart();
  tmWriteByte(0x8B);  // Display on, moderate brightness.
  tmStop();
}

const uint8_t DIGIT_SEGMENTS[10] = {
  0x3F, 0x06, 0x5B, 0x4F, 0x66,
  0x6D, 0x7D, 0x07, 0x7F, 0x6F
};

void updateDisplay(uint32_t now) {
  static uint32_t previousCount = UINT32_MAX;
  static uint32_t lastScrollAt = 0;
  static uint8_t scrollPosition = 0;
  static bool dirty = true;

  if (todayCount != previousCount) {
    previousCount = todayCount;
    scrollPosition = 0;
    lastScrollAt = now;
    dirty = true;
  }

  // Counts above four digits scroll in full instead of silently wrapping.
  if (todayCount > 9999 &&
      (uint32_t)(now - lastScrollAt) >= 500) {
    lastScrollAt = now;
    ++scrollPosition;
    dirty = true;
  }

  if (!dirty) {
    return;
  }
  dirty = false;

  char digits[11];
  ultoa((unsigned long)todayCount, digits, 10);
  uint8_t length = strlen(digits);
  uint8_t segments[4] = {0, 0, 0, 0};

  if (length <= 4) {
    for (uint8_t i = 0; i < length; ++i) {
      segments[4 - length + i] = DIGIT_SEGMENTS[digits[i] - '0'];
    }
  } else {
    uint8_t cycleLength = length + 4;
    scrollPosition %= cycleLength;
    for (uint8_t i = 0; i < 4; ++i) {
      uint8_t index = (scrollPosition + i) % cycleLength;
      if (index < length) {
        segments[i] = DIGIT_SEGMENTS[digits[index] - '0'];
      }
    }
  }

  tmDisplay(segments);
}

void printUint64(uint64_t value) {
  char buffer[21];
  uint8_t position = sizeof(buffer);
  buffer[--position] = '\0';
  do {
    buffer[--position] = '0' + value % 10;
    value /= 10;
  } while (value != 0);
  Serial.print(&buffer[position]);
}

void reportStatus() {
  Serial.print(F("today="));
  Serial.print(todayCount);
  Serial.print(F(", A_to_B="));
  Serial.print(countAToB);
  Serial.print(F(", B_to_A="));
  Serial.print(countBToA);
  Serial.print(F(", total_lit_ms="));
  printUint64(totalLitMs);
  Serial.print(F(", ambient="));
  Serial.print(lightReading);
  Serial.print(F(", dark="));
  Serial.print(ambientDark ? 1 : 0);
  Serial.print(F(", pwm="));
  Serial.println(lampPWM);
}

void updateAmbientLight(uint32_t now) {
  if ((uint32_t)(now - lastLightSampleAt) < LIGHT_SAMPLE_MS) {
    return;
  }
  lastLightSampleAt = now;

  int32_t sampleQ8 = (int32_t)analogRead(LIGHT_SENSOR_PIN) << 8;
  lightFilterQ8 += (sampleQ8 - lightFilterQ8) / 8;
  uint16_t rawFiltered = (uint16_t)(lightFilterQ8 >> 8);
  lightReading = LIGHT_READING_INCREASES_WITH_BRIGHTNESS
                   ? rawFiltered : 1023 - rawFiltered;

  // Hysteresis prevents flicker around the darkness threshold.
  if (!ambientDark && lightReading <= DARK_ON_THRESHOLD) {
    ambientDark = true;
  } else if (ambientDark && lightReading >= BRIGHT_OFF_THRESHOLD) {
    ambientDark = false;
  }
}

void recordPassage(bool aToB) {
  ++todayCount;
  if (aToB) {
    ++countAToB;
  } else {
    ++countBToA;
  }

  Serial.print(F("passage="));
  Serial.print(aToB ? F("A->B") : F("B->A"));
  Serial.print(F(", today="));
  Serial.println(todayCount);

  passageState = WAIT_FOR_CLEAR;
  clearTimerRunning = false;
}

void updatePassageCounter(uint32_t now) {
  // Two PIRs estimate one unambiguous traversal at a time.
  // Simultaneous triggers, turnbacks and unpaired triggers are not counted.
  if (passageState == WAIT_FOR_CLEAR) {
    if (!pirA.active && !pirB.active) {
      if (!clearTimerRunning) {
        clearTimerRunning = true;
        clearStartedAt = now;
      } else if ((uint32_t)(now - clearStartedAt) >= BOTH_CLEAR_MS) {
        passageState = READY;
        clearTimerRunning = false;
      }
    } else {
      clearTimerRunning = false;
    }
    return;
  }

  if ((passageState == A_FIRST || passageState == B_FIRST) &&
      (uint32_t)(now - firstDetectionAt) > DIRECTION_WINDOW_MS) {
    passageState = WAIT_FOR_CLEAR;
    clearTimerRunning = false;
    return;
  }

  switch (passageState) {
    case READY:
      if (pirA.rose && pirB.rose) {
        passageState = WAIT_FOR_CLEAR;
        clearTimerRunning = false;
      } else if (pirA.rose) {
        passageState = A_FIRST;
        firstDetectionAt = now;
      } else if (pirB.rose) {
        passageState = B_FIRST;
        firstDetectionAt = now;
      }
      break;

    case A_FIRST:
      if (pirB.rose) {
        recordPassage(true);
      }
      break;

    case B_FIRST:
      if (pirA.rose) {
        recordPassage(false);
      }
      break;

    default:
      break;
  }
}

void updateLamp(uint32_t now) {
  // Refresh continuously while either PIR remains asserted, not only on edges.
  if (pirA.active || pirB.active) {
    lastMotionAt = now;
    motionSeen = true;
  }

  bool motionHoldActive =
      motionSeen && (uint32_t)(now - lastMotionAt) < LIGHT_HOLD_MS;
  uint8_t target = ambientDark && motionHoldActive ? LAMP_BRIGHTNESS : 0;

  // Bright ambient light overrides the hold timer and fade immediately.
  if (!ambientDark) {
    lampPWM = 0;
    analogWrite(LAMP_PIN, 0);
    lastFadeAt = now;
    return;
  }

  uint32_t steps = (uint32_t)(now - lastFadeAt) / FADE_STEP_MS;
  if (steps == 0) {
    return;
  }
  lastFadeAt += steps * FADE_STEP_MS;

  uint16_t change = steps >= 51 ? 255 : steps * FADE_STEP;
  if (lampPWM < target) {
    uint16_t next = lampPWM + change;
    lampPWM = next > target ? target : (uint8_t)next;
  } else if (lampPWM > target) {
    lampPWM = change >= lampPWM - target
                ? target : (uint8_t)(lampPWM - change);
  }

  analogWrite(LAMP_PIN, lampPWM);
}

void setup() {
  pinMode(LAMP_PIN, OUTPUT);
  analogWrite(LAMP_PIN, 0);
  pinMode(LIGHT_SENSOR_PIN, INPUT);

  pinMode(TM_CLK_PIN, OUTPUT);
  digitalWrite(TM_CLK_PIN, HIGH);
  tmDioRelease();

  Serial.begin(9600);

  uint32_t sum = 0;
  for (uint8_t i = 0; i < 16; ++i) {
    sum += analogRead(LIGHT_SENSOR_PIN);
  }
  uint16_t initialLight = sum / 16;
  lightFilterQ8 = (int32_t)initialLight << 8;
  lightReading = LIGHT_READING_INCREASES_WITH_BRIGHTNESS
                   ? initialLight : 1023 - initialLight;
  ambientDark = lightReading <= DARK_ON_THRESHOLD;

  uint32_t now = millis();
  pirA.begin(now);
  pirB.begin(now);
  dayStartedAt = now;
  lastAccountingAt = now;
  lastReportAt = now;
  lastLightSampleAt = now;
  lastFadeAt = now;

  updateDisplay(now);
  Serial.println(F("Ready; day=24h since boot; PWM=D6; total_lit_ms counts PWM>0."));
}

void loop() {
  uint32_t now = millis();

  // Account for the actual time the previous PWM output was nonzero.
  uint32_t elapsed = (uint32_t)(now - lastAccountingAt);
  if (lampPWM != 0) {
    totalLitMs += elapsed;
  }
  lastAccountingAt = now;

  if ((uint32_t)(now - dayStartedAt) >= DAY_MS) {
    dayStartedAt += DAY_MS;
    todayCount = 0;
    countAToB = 0;
    countBToA = 0;
    Serial.println(F("New 24h day; daily passage counters reset."));
  }

  pirA.update(now);
  pirB.update(now);
  updateAmbientLight(now);
  updatePassageCounter(now);
  updateLamp(now);
  updateDisplay(now);

  if ((uint32_t)(now - lastReportAt) >= SERIAL_REPORT_MS) {
    lastReportAt = now;
    reportStatus();
  }
}
