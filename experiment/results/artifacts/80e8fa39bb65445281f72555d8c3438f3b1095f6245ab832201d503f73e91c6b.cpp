#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_016\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

// Wiring (Arduino Uno)
const uint8_t CURRENT_PIN = A0;
const uint8_t TEMPERATURE_PIN = A1;  // LM35: 10 mV/degree C
const uint8_t VIBRATION_PIN = 2;     // Active LOW switch, connected to GND
const uint8_t DISPLAY_DIO_PIN = 3;
const uint8_t DISPLAY_CLK_PIN = 4;
const uint8_t FAN_PIN = 5;           // PWM to an external fan driver
const uint8_t BUZZER_PIN = 6;
const uint8_t LED_PIN = 13;

// Calibration defaults: adjust for the actual current sensor and ADC reference.
// Example current sensor: ACS712-20A, 100 mV/A, zero-current output = 2.5 V.
// Do not drive a fan/motor directly from an Arduino pin.
const float ADC_REFERENCE_V = 5.0f;
const float CURRENT_SENSITIVITY_V_PER_A = 0.100f;
float currentZeroVoltage = 2.500f;

// These are configurable application defaults, not motor nameplate ratings.
const float OVERLOAD_ON_A = 5.0f;
const float OVERLOAD_OFF_A = 4.5f;
const float OVERHEAT_ON_C = 60.0f;
const float OVERHEAT_OFF_C = 55.0f;
const float FAN_FULL_SPEED_C = 80.0f;
const uint8_t FAN_MIN_PWM = 120;

const uint32_t SAMPLE_INTERVAL_US = 1000;
const uint32_t CURRENT_WINDOW_MS = 200;
const uint32_t TEMPERATURE_INTERVAL_MS = 100;
const uint32_t VIBRATION_DEBOUNCE_MS = 15;
const uint32_t MECHANICAL_HOLD_MS = 3000;
const uint32_t DISPLAY_PAGE_MS = 2000;
const uint32_t TELEMETRY_INTERVAL_MS = 1000;
const uint32_t SERIAL_IDLE_TIMEOUT_MS = 20;

enum MotorState : uint8_t {
  NORMAL = 0,
  OVERLOAD = 1,
  OVERHEAT = 2,
  MECHANICAL_FAULT = 3
};

MotorState motorState = NORMAL;
bool overloadActive = false;
bool overheatActive = false;
bool mechanicalActive = false;

float currentRmsA = 0.0f;
float temperatureC = 0.0f;
bool temperatureInitialized = false;
uint8_t fanPwm = 0;

float currentSquareSum = 0.0f;
uint16_t currentSampleCount = 0;
uint32_t lastSampleUs = 0;
uint32_t currentWindowStartedMs = 0;
uint32_t lastTemperatureMs = 0;

bool vibrationRaw = false;
bool vibrationStable = false;
uint32_t vibrationChangedMs = 0;
uint32_t lastMechanicalActivityMs = 0;

uint32_t lastTelemetryMs = 0;
uint32_t lastDisplayMs = 0;
uint8_t displayPage = 0;

char serialBuffer[48];
uint8_t serialLength = 0;
bool serialOverflow = false;
uint32_t lastSerialByteMs = 0;

const uint8_t DIGIT_SEGMENTS[10] = {
  0x3F, 0x06, 0x5B, 0x4F, 0x66,
  0x6D, 0x7D, 0x07, 0x7F, 0x6F
};

// Dependency-free TM1637 driver. DIO is released using INPUT_PULLUP.
#line 83 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_016\\direct\\attempt_0\\sketch\\sketch.ino"
void displayDelay();
#line 87 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_016\\direct\\attempt_0\\sketch\\sketch.ino"
void displayStart();
#line 97 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_016\\direct\\attempt_0\\sketch\\sketch.ino"
void displayStop();
#line 108 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_016\\direct\\attempt_0\\sketch\\sketch.ino"
void displayWriteByte(uint8_t value);
#line 131 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_016\\direct\\attempt_0\\sketch\\sketch.ino"
void displaySegments(const uint8_t segments[4]);
#line 148 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_016\\direct\\attempt_0\\sketch\\sketch.ino"
void updateDisplay();
#line 178 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_016\\direct\\attempt_0\\sketch\\sketch.ino"
void printStateName();
#line 187 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_016\\direct\\attempt_0\\sketch\\sketch.ino"
void printTelemetry();
#line 204 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_016\\direct\\attempt_0\\sketch\\sketch.ino"
void processSerialCommand();
#line 245 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_016\\direct\\attempt_0\\sketch\\sketch.ino"
void serviceSerial(uint32_t now);
#line 269 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_016\\direct\\attempt_0\\sketch\\sketch.ino"
void sampleSensors(uint32_t now);
#line 315 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_016\\direct\\attempt_0\\sketch\\sketch.ino"
void updateVibration(uint32_t now);
#line 337 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_016\\direct\\attempt_0\\sketch\\sketch.ino"
void updateStateAndOutputs(uint32_t now);
#line 406 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_016\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 441 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_016\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 83 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_016\\direct\\attempt_0\\sketch\\sketch.ino"
void displayDelay() {
  delayMicroseconds(4);
}

void displayStart() {
  pinMode(DISPLAY_DIO_PIN, INPUT_PULLUP);
  digitalWrite(DISPLAY_CLK_PIN, HIGH);
  displayDelay();
  pinMode(DISPLAY_DIO_PIN, OUTPUT);
  digitalWrite(DISPLAY_DIO_PIN, LOW);
  displayDelay();
  digitalWrite(DISPLAY_CLK_PIN, LOW);
}

void displayStop() {
  digitalWrite(DISPLAY_CLK_PIN, LOW);
  pinMode(DISPLAY_DIO_PIN, OUTPUT);
  digitalWrite(DISPLAY_DIO_PIN, LOW);
  displayDelay();
  digitalWrite(DISPLAY_CLK_PIN, HIGH);
  displayDelay();
  pinMode(DISPLAY_DIO_PIN, INPUT_PULLUP);
  displayDelay();
}

void displayWriteByte(uint8_t value) {
  for (uint8_t i = 0; i < 8; ++i) {
    digitalWrite(DISPLAY_CLK_PIN, LOW);
    if (value & 1) {
      pinMode(DISPLAY_DIO_PIN, INPUT_PULLUP);
    } else {
      digitalWrite(DISPLAY_DIO_PIN, LOW);
      pinMode(DISPLAY_DIO_PIN, OUTPUT);
    }
    displayDelay();
    digitalWrite(DISPLAY_CLK_PIN, HIGH);
    displayDelay();
    value >>= 1;
  }

  digitalWrite(DISPLAY_CLK_PIN, LOW);
  pinMode(DISPLAY_DIO_PIN, INPUT_PULLUP);
  displayDelay();
  digitalWrite(DISPLAY_CLK_PIN, HIGH);
  displayDelay();
  digitalWrite(DISPLAY_CLK_PIN, LOW);
}

void displaySegments(const uint8_t segments[4]) {
  displayStart();
  displayWriteByte(0x40);  // Auto-increment address
  displayStop();

  displayStart();
  displayWriteByte(0xC0);
  for (uint8_t i = 0; i < 4; ++i) {
    displayWriteByte(segments[i]);
  }
  displayStop();

  displayStart();
  displayWriteByte(0x8C);  // Display on, brightness 4/7
  displayStop();
}

void updateDisplay() {
  uint8_t segments[4] = {0, 0, 0, 0};

  if (displayPage == 2) {
    // State page: S 00 ... S 03
    segments[0] = 0x6D;  // S
    segments[2] = DIGIT_SEGMENTS[0];
    segments[3] = DIGIT_SEGMENTS[(uint8_t)motorState];
  } else {
    // Current page: A followed by deciamperes.
    // Temperature page: C followed by decidegrees Celsius.
    // Example: A012 = 1.2 A, C605 = 60.5 degrees C.
    // Decimal-point/colon support varies among four-digit TM1637 modules,
    // so the last three digits always use fixed tenths with no separator.
    float value = (displayPage == 0) ? currentRmsA : temperatureC;
    segments[0] = (displayPage == 0) ? 0x77 : 0x39;  // A / C

    if (value < 0.0f || value > 99.9f) {
      segments[1] = segments[2] = segments[3] = 0x40;  // ---
    } else {
      uint16_t tenths = (uint16_t)(value * 10.0f + 0.5f);
      segments[1] = DIGIT_SEGMENTS[(tenths / 100) % 10];
      segments[2] = DIGIT_SEGMENTS[(tenths / 10) % 10];
      segments[3] = DIGIT_SEGMENTS[tenths % 10];
    }
  }

  displaySegments(segments);
}

void printStateName() {
  switch (motorState) {
    case NORMAL:           Serial.print(F("NORMAL")); break;
    case OVERLOAD:         Serial.print(F("OVERLOAD")); break;
    case OVERHEAT:         Serial.print(F("OVERHEAT")); break;
    case MECHANICAL_FAULT:  Serial.print(F("MECHANICAL")); break;
  }
}

void printTelemetry() {
  Serial.print(F("I="));
  Serial.print(currentRmsA, 2);
  Serial.print(F("A T="));
  Serial.print(temperatureC, 1);
  Serial.print(F("C STATE="));
  printStateName();
  Serial.print(F(" FAULT=F0"));
  Serial.print((uint8_t)motorState);
  Serial.print(F(" FLAGS="));
  Serial.print(overloadActive ? 1 : 0);
  Serial.print(overheatActive ? 1 : 0);
  Serial.print(mechanicalActive ? 1 : 0);
  Serial.print(F(" FAN="));
  Serial.println(fanPwm);
}

void processSerialCommand() {
  serialBuffer[serialLength] = '\0';

  if (serialOverflow) {
    Serial.println(F("ERR command too long"));
  } else if (serialLength > 0) {
    for (uint8_t i = 0; i < serialLength; ++i) {
      if (serialBuffer[i] >= 'a' && serialBuffer[i] <= 'z') {
        serialBuffer[i] -= ('a' - 'A');
      }
    }

    if (strcmp(serialBuffer, "STATUS") == 0) {
      printTelemetry();
    } else if (strcmp(serialBuffer, "HELP") == 0) {
      Serial.println(F("STATUS | OFFSET <millivolts> | HELP"));
      Serial.println(F("OFFSET: measured zero-current voltage; motor OFF."));
    } else if (strncmp(serialBuffer, "OFFSET ", 7) == 0) {
      char *end;
      float millivolts = (float)strtod(serialBuffer + 7, &end);
      while (*end == ' ') ++end;
      if (end != serialBuffer + 7 && *end == '\0' &&
          millivolts >= 100.0f &&
          millivolts <= ADC_REFERENCE_V * 1000.0f - 100.0f) {
        currentZeroVoltage = millivolts / 1000.0f;
        currentSquareSum = 0.0f;
        currentSampleCount = 0;
        currentWindowStartedMs = millis();
        Serial.println(F("OK OFFSET (RAM only)"));
      } else {
        Serial.println(F("ERR invalid offset"));
      }
    } else {
      Serial.println(F("ERR unknown command; use HELP"));
    }
  }

  serialLength = 0;
  serialOverflow = false;
}

void serviceSerial(uint32_t now) {
  // Bound work per pass so serial traffic cannot starve monitoring.
  uint8_t budget = 24;
  while (Serial.available() && budget--) {
    char c = (char)Serial.read();
    lastSerialByteMs = millis();

    if (c == '\r' || c == '\n') {
      if (serialLength || serialOverflow) processSerialCommand();
    } else if (!serialOverflow) {
      if (serialLength < sizeof(serialBuffer) - 1) {
        serialBuffer[serialLength++] = c;
      } else {
        serialOverflow = true;
      }
    }
  }

  if ((serialLength || serialOverflow) && !Serial.available() &&
      (uint32_t)(now - lastSerialByteMs) >= SERIAL_IDLE_TIMEOUT_MS) {
    processSerialCommand();
  }
}

void sampleSensors(uint32_t now) {
  uint32_t nowUs = micros();
  if ((uint32_t)(nowUs - lastSampleUs) >= SAMPLE_INTERVAL_US) {
    lastSampleUs = nowUs;

    // Discard the first conversion after switching ADC channels.
    analogRead(CURRENT_PIN);
    int raw = analogRead(CURRENT_PIN);
    float voltage = raw * (ADC_REFERENCE_V / 1023.0f);
    float amps = (voltage - currentZeroVoltage) /
                 CURRENT_SENSITIVITY_V_PER_A;
    currentSquareSum += amps * amps;
    ++currentSampleCount;
  }

  if ((uint32_t)(now - currentWindowStartedMs) >= CURRENT_WINDOW_MS) {
    if (currentSampleCount) {
      // RMS around calibrated zero measures both AC and DC load current.
      currentRmsA = sqrtf(currentSquareSum / currentSampleCount);
    }
    currentSquareSum = 0.0f;
    currentSampleCount = 0;
    currentWindowStartedMs = now;
  }

  if (!temperatureInitialized ||
      (uint32_t)(now - lastTemperatureMs) >= TEMPERATURE_INTERVAL_MS) {
    lastTemperatureMs = now;
    analogRead(TEMPERATURE_PIN);
    uint16_t sum = 0;
    for (uint8_t i = 0; i < 8; ++i) {
      sum += analogRead(TEMPERATURE_PIN);
    }

    float measuredC = (sum / 8.0f) *
                      (ADC_REFERENCE_V / 1023.0f) * 100.0f;

    if (!temperatureInitialized) {
      temperatureC = measuredC;
      temperatureInitialized = true;
    } else {
      temperatureC += 0.25f * (measuredC - temperatureC);
    }
  }
}

void updateVibration(uint32_t now) {
  bool active = (digitalRead(VIBRATION_PIN) == LOW);

  if (active != vibrationRaw) {
    vibrationRaw = active;
    vibrationChangedMs = now;
  }

  if ((uint32_t)(now - vibrationChangedMs) >= VIBRATION_DEBOUNCE_MS) {
    vibrationStable = vibrationRaw;
  }

  if (vibrationStable) {
    mechanicalActive = true;
    lastMechanicalActivityMs = now;
  } else if (mechanicalActive &&
             (uint32_t)(now - lastMechanicalActivityMs) >=
                 MECHANICAL_HOLD_MS) {
    mechanicalActive = false;
  }
}

void updateStateAndOutputs(uint32_t now) {
  if (!overloadActive && currentRmsA >= OVERLOAD_ON_A) {
    overloadActive = true;
  } else if (overloadActive && currentRmsA <= OVERLOAD_OFF_A) {
    overloadActive = false;
  }

  if (!overheatActive && temperatureC >= OVERHEAT_ON_C) {
    overheatActive = true;
  } else if (overheatActive && temperatureC <= OVERHEAT_OFF_C) {
    overheatActive = false;
  }

  MotorState nextState = mechanicalActive ? MECHANICAL_FAULT :
                         overheatActive ? OVERHEAT :
                         overloadActive ? OVERLOAD : NORMAL;
  bool stateChanged = (nextState != motorState);
  motorState = nextState;

  // Cooling remains active even when mechanical alarm has display priority.
  if (overheatActive) {
    float fraction = (temperatureC - OVERHEAT_OFF_C) /
                     (FAN_FULL_SPEED_C - OVERHEAT_OFF_C);
    fraction = constrain(fraction, 0.0f, 1.0f);
    fanPwm = FAN_MIN_PWM +
             (uint8_t)((255 - FAN_MIN_PWM) * fraction);
  } else {
    fanPwm = 0;
  }
  analogWrite(FAN_PIN, fanPwm);

  bool beep = false;
  uint16_t frequency = 0;
  switch (motorState) {
    case MECHANICAL_FAULT:
      beep = (now % 300UL) < 150UL;
      frequency = 2400;
      digitalWrite(LED_PIN, (now % 200UL) < 100UL);
      break;
    case OVERHEAT:
      beep = (now % 1000UL) < 250UL;
      frequency = 1600;
      digitalWrite(LED_PIN, (now % 500UL) < 250UL);
      break;
    case OVERLOAD:
      beep = (now % 1500UL) < 100UL;
      frequency = 1000;
      digitalWrite(LED_PIN, (now % 1000UL) < 500UL);
      break;
    default:
      digitalWrite(LED_PIN, HIGH);
      break;
  }

  static uint16_t activeTone = 0;
  uint16_t requestedTone = beep ? frequency : 0;
  if (requestedTone != activeTone) {
    if (requestedTone) tone(BUZZER_PIN, requestedTone);
    else noTone(BUZZER_PIN);
    activeTone = requestedTone;
  }

  if (stateChanged) {
    // Immediate event report includes the priority fault code.
    printTelemetry();
    updateDisplay();
  }
}

void setup() {
  pinMode(CURRENT_PIN, INPUT);
  pinMode(TEMPERATURE_PIN, INPUT);
  pinMode(VIBRATION_PIN, INPUT_PULLUP);
  pinMode(FAN_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(DISPLAY_CLK_PIN, OUTPUT);
  pinMode(DISPLAY_DIO_PIN, INPUT_PULLUP);

  analogReference(DEFAULT);
  analogWrite(FAN_PIN, 0);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_PIN, HIGH);
  digitalWrite(DISPLAY_CLK_PIN, HIGH);

  Serial.begin(9600);

  uint32_t now = millis();
  currentWindowStartedMs = now;
  vibrationChangedMs = now;
  vibrationRaw = (digitalRead(VIBRATION_PIN) == LOW);
  lastDisplayMs = now;
  lastTelemetryMs = now;
  lastSampleUs = micros();

  sampleSensors(now);
  updateStateAndOutputs(now);
  updateDisplay();

  Serial.println(F("Motor monitor ready: 0=NORMAL 1=OVERLOAD 2=OVERHEAT 3=MECHANICAL"));
  Serial.println(F("Display: Axxx=0.1A Cxxx=0.1C Sxx=state; HELP for commands."));
  printTelemetry();
}

void loop() {
  uint32_t now = millis();

  sampleSensors(now);
  updateVibration(now);
  updateStateAndOutputs(now);

  if ((uint32_t)(now - lastDisplayMs) >= DISPLAY_PAGE_MS) {
    lastDisplayMs = now;
    displayPage = (displayPage + 1) % 3;
    updateDisplay();
  } else {
    static uint32_t lastRefreshMs = 0;
    if ((uint32_t)(now - lastRefreshMs) >= 250UL) {
      lastRefreshMs = now;
      updateDisplay();
    }
  }

  if ((uint32_t)(now - lastTelemetryMs) >= TELEMETRY_INTERVAL_MS) {
    lastTelemetryMs = now;
    printTelemetry();
  }

  serviceSerial(millis());
}
