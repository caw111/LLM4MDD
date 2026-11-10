#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_011\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// Arduino Uno wiring.
constexpr uint8_t TEMP_PIN       = A1;  // LM35
constexpr uint8_t SMOKE_PIN      = A0;  // MQ-2 analog output
constexpr uint8_t FLAME_PIN      = 2;
constexpr uint8_t BUZZER_PIN     = 6;
constexpr uint8_t RED_LED_PIN    = 13;
constexpr uint8_t GREEN_LED_PIN  = 12;
constexpr uint8_t DISPLAY_CLK    = 4;
constexpr uint8_t DISPLAY_DIO    = 5;

// Most flame modules assert LOW when a flame is detected.
constexpr uint8_t FLAME_ACTIVE_LEVEL = LOW;

// Calibration: LM35 produces 10 mV/degree C.
// Adjust ADC_REFERENCE_V to the measured Uno supply voltage if necessary.
// MQ-2 thresholds must be calibrated for the actual sensor and environment.
constexpr float ADC_REFERENCE_V = 5.0f;
constexpr float TEMP_OFFSET_C = 0.0f;
constexpr float TEMP_ATTENTION_C = 40.0f;
constexpr float TEMP_CLEAR_C = 38.0f;
constexpr uint16_t SMOKE_ATTENTION_ADC = 350;
constexpr uint16_t SMOKE_CLEAR_ADC = 320;

constexpr unsigned long SAMPLE_INTERVAL_MS = 100;
constexpr unsigned long SERIAL_INTERVAL_MS = 1000;
constexpr unsigned long DISPLAY_PAGE_MS = 2000;
constexpr unsigned long ATTENTION_BLINK_MS = 500;
constexpr unsigned long ALARM_ON_MS = 500;
constexpr unsigned long ALARM_OFF_MS = 500;
constexpr unsigned long SERIAL_IDLE_TIMEOUT_MS = 20;
constexpr unsigned int ALARM_FREQUENCY_HZ = 2400;

enum WarningLevel : uint8_t {
  NORMAL = 0,
  ATTENTION = 1,
  ALARM = 2
};

TM1637Display display(DISPLAY_CLK, DISPLAY_DIO);

WarningLevel level = NORMAL;
float temperatureC = 0.0f;
uint16_t smokeADC = 0;
bool temperatureExceeded = false;
bool smokeExceeded = false;
bool flameDetected = false;
bool buzzerOn = false;

unsigned long lastSampleMs = 0;
unsigned long lastSerialMs = 0;
unsigned long levelStartedMs = 0;
bool reportPending = true;

#line 57 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_011\\direct\\attempt_0\\sketch\\sketch.ino"
uint16_t readAveragedADC(uint8_t pin);
#line 67 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_011\\direct\\attempt_0\\sketch\\sketch.ino"
void sampleSensors();
#line 88 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_011\\direct\\attempt_0\\sketch\\sketch.ino"
void updateLevel(unsigned long now);
#line 106 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_011\\direct\\attempt_0\\sketch\\sketch.ino"
void updateOutputs(unsigned long now);
#line 136 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_011\\direct\\attempt_0\\sketch\\sketch.ino"
void updateDisplay(unsigned long now);
#line 169 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_011\\direct\\attempt_0\\sketch\\sketch.ino"
void reportStatus(unsigned long now);
#line 195 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_011\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 227 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_011\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 57 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_011\\direct\\attempt_0\\sketch\\sketch.ino"
uint16_t readAveragedADC(uint8_t pin) {
  // Discard one conversion after changing ADC channels.
  analogRead(pin);
  uint32_t sum = 0;
  for (uint8_t i = 0; i < 8; ++i) {
    sum += analogRead(pin);
  }
  return static_cast<uint16_t>((sum + 4) / 8);
}

void sampleSensors() {
  const uint16_t temperatureADC = readAveragedADC(TEMP_PIN);
  temperatureC =
      temperatureADC * (ADC_REFERENCE_V / 1023.0f) * 100.0f +
      TEMP_OFFSET_C;
  smokeADC = readAveragedADC(SMOKE_PIN);

  // Hysteresis prevents repeated level changes near a threshold.
  if (temperatureC >= TEMP_ATTENTION_C) {
    temperatureExceeded = true;
  } else if (temperatureC <= TEMP_CLEAR_C) {
    temperatureExceeded = false;
  }

  if (smokeADC >= SMOKE_ATTENTION_ADC) {
    smokeExceeded = true;
  } else if (smokeADC <= SMOKE_CLEAR_ADC) {
    smokeExceeded = false;
  }
}

void updateLevel(unsigned long now) {
  // Read flame every loop so it does not wait for the analog sampling timer.
  flameDetected = (digitalRead(FLAME_PIN) == FLAME_ACTIVE_LEVEL);

  WarningLevel nextLevel = NORMAL;
  if (flameDetected || (temperatureExceeded && smokeExceeded)) {
    nextLevel = ALARM;
  } else if (temperatureExceeded || smokeExceeded) {
    nextLevel = ATTENTION;
  }

  if (nextLevel != level) {
    level = nextLevel;
    levelStartedMs = now;
    reportPending = true;
  }
}

void updateOutputs(unsigned long now) {
  const unsigned long elapsed = now - levelStartedMs;

  digitalWrite(GREEN_LED_PIN, level == NORMAL ? HIGH : LOW);

  if (level == ALARM) {
    digitalWrite(RED_LED_PIN, HIGH);
  } else if (level == ATTENTION) {
    digitalWrite(RED_LED_PIN,
                 ((elapsed / ATTENTION_BLINK_MS) % 2 == 0) ? HIGH : LOW);
  } else {
    digitalWrite(RED_LED_PIN, LOW);
  }

  // Continue intermittent sound for as long as the alarm level remains active.
  const bool shouldSound =
      level == ALARM &&
      (elapsed % (ALARM_ON_MS + ALARM_OFF_MS)) < ALARM_ON_MS;

  if (shouldSound != buzzerOn) {
    buzzerOn = shouldSound;
    if (buzzerOn) {
      tone(BUZZER_PIN, ALARM_FREQUENCY_HZ);
    } else {
      noTone(BUZZER_PIN);
      digitalWrite(BUZZER_PIN, LOW);
    }
  }
}

void updateDisplay(unsigned long now) {
  // Four-digit layout: L.VVV
  // L is level 0/1/2; the dot is a separator, not a decimal point.
  // Alternate every 2 seconds:
  //   temperature page: VVV = rounded temperature in degrees C
  //   smoke page:       VVV = MQ-2 ADC full-scale percentage, NOT gas ppm
  // A single-dot separator identifies temperature; both center dots
  // identify smoke on TM1637 modules fitted with a center colon.
  const bool smokePage = ((now / DISPLAY_PAGE_MS) % 2) != 0;
  int keyValue;

  if (smokePage) {
    keyValue = static_cast<int>(
        (static_cast<uint32_t>(smokeADC) * 100UL + 511UL) / 1023UL);
  } else {
    keyValue = static_cast<int>(temperatureC + 0.5f);
  }

  keyValue = constrain(keyValue, 0, 999);

  uint8_t segments[4];
  segments[0] = display.encodeDigit(static_cast<uint8_t>(level)) | 0x80;
  segments[1] = display.encodeDigit(keyValue / 100);
  segments[2] = display.encodeDigit((keyValue / 10) % 10);
  segments[3] = display.encodeDigit(keyValue % 10);

  // Digit 1's decimal-point bit drives the center colon on colon modules.
  if (smokePage) {
    segments[1] |= 0x80;
  }
  display.setSegments(segments);
}

void reportStatus(unsigned long now) {
  if (!reportPending && now - lastSerialMs < SERIAL_INTERVAL_MS) {
    return;
  }

  lastSerialMs = now;
  reportPending = false;

  if (level == ALARM) {
    Serial.println(F("fire alert"));
  }

  Serial.print(F("level="));
  Serial.print(static_cast<uint8_t>(level));
  Serial.print(F(" temperature_C="));
  Serial.print(temperatureC, 1);
  Serial.print(F(" smoke_ADC="));
  Serial.print(smokeADC);
  Serial.print(F(" temperature_exceeded="));
  Serial.print(temperatureExceeded ? 1 : 0);
  Serial.print(F(" smoke_exceeded="));
  Serial.print(smokeExceeded ? 1 : 0);
  Serial.print(F(" flame="));
  Serial.println(flameDetected ? 1 : 0);
}

void setup() {
  pinMode(TEMP_PIN, INPUT);
  pinMode(SMOKE_PIN, INPUT);
  pinMode(FLAME_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);

  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, LOW);
  noTone(BUZZER_PIN);

  analogReference(DEFAULT);

  Serial.begin(9600);
  Serial.setTimeout(SERIAL_IDLE_TIMEOUT_MS);
  // Transmitted records are newline-framed; no serial commands are required.

  display.setBrightness(5, true);
  display.clear();

  sampleSensors();
  const unsigned long now = millis();
  lastSampleMs = now;
  levelStartedMs = now;
  updateLevel(now);
  updateOutputs(now);
  updateDisplay(now);
  reportStatus(now);
}

void loop() {
  const unsigned long now = millis();

  if (now - lastSampleMs >= SAMPLE_INTERVAL_MS) {
    lastSampleMs = now;
    sampleSensors();
  }

  updateLevel(now);
  updateOutputs(now);

  // Limit display traffic while preserving responsive alarm handling.
  static unsigned long lastDisplayMs = 0;
  if (now - lastDisplayMs >= 100) {
    lastDisplayMs = now;
    updateDisplay(now);
  }

  reportStatus(now);
}
