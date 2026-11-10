#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_001\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

constexpr uint8_t DHT_PIN = 7;
constexpr uint8_t TRIG_PIN = 8;
constexpr uint8_t ECHO_PIN = 9;
constexpr uint8_t BUZZER_PIN = 6;
constexpr uint8_t DISPLAY_CLK = 4;
constexpr uint8_t DISPLAY_DIO = 5;

// The final, authoritative serial configuration specifies 9600 baud.
constexpr unsigned long SERIAL_BAUD = 9600;

constexpr uint32_t DHT_INTERVAL_MS = 2100;
constexpr uint32_t DHT_START_LOW_MS = 18;
constexpr uint32_t DHT_RESPONSE_TIMEOUT_US = 6000;
constexpr uint32_t DISTANCE_INTERVAL_MS = 100;
constexpr uint32_t ECHO_TIMEOUT_US = 25000;
constexpr uint32_t PAGE_INTERVAL_MS = 2000;
constexpr uint32_t ALARM_REPORT_INTERVAL_MS = 1000;

TM1637Display display(DISPLAY_CLK, DISPLAY_DIO);

float temperatureC = 0.0f;
float distanceCm = 0.0f;
bool temperatureValid = false;
bool distanceValid = false;

enum DhtCaptureState : uint8_t {
  DHT_DISABLED,
  DHT_WAIT_RESPONSE_LOW,
  DHT_WAIT_RESPONSE_HIGH,
  DHT_WAIT_RESPONSE_END,
  DHT_WAIT_BIT_HIGH,
  DHT_WAIT_BIT_LOW,
  DHT_DONE,
  DHT_ERROR
};

volatile uint8_t dhtCaptureState = DHT_DISABLED;
volatile uint8_t dhtBytes[5] = {};
volatile uint8_t dhtBitCount = 0;
volatile uint32_t dhtHighStartedUs = 0;

enum DhtTaskState : uint8_t {
  DHT_IDLE,
  DHT_START_LOW,
  DHT_CAPTURING
};

DhtTaskState dhtTaskState = DHT_IDLE;
uint32_t lastDhtStartMs = 0;
uint32_t dhtLowStartedMs = 0;
uint32_t dhtReleasedUs = 0;
bool dhtHasStarted = false;

enum EchoState : uint8_t {
  ECHO_IDLE,
  ECHO_WAIT_RISE,
  ECHO_WAIT_FALL,
  ECHO_DONE
};

volatile uint8_t echoState = ECHO_IDLE;
volatile uint32_t echoRiseUs = 0;
volatile uint32_t echoWidthUs = 0;
volatile uint32_t echoFallUs = 0;

bool distancePending = false;
bool distanceHasStarted = false;
uint32_t lastDistanceStartMs = 0;
uint32_t echoWaitStartedUs = 0;

bool showTemperature = true;
uint32_t pageStartedMs = 0;
bool displayInitialized = false;
bool displayedValid = false;
bool displayedTemperaturePage = true;
int16_t displayedInteger = 0;

constexpr uint8_t REASON_TEMP = 0x01;
constexpr uint8_t REASON_DISTANCE = 0x02;

uint8_t alarmReasons = 0;
uint32_t alarmStartedMs = 0;
uint32_t lastAlarmReportMs = 0;
bool buzzerOn = false;

// D7 is PD7 / PCINT23 on the Uno.
#line 162 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_001\\direct\\attempt_0\\sketch\\sketch.ino"
void disableDhtCapture();
#line 169 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_001\\direct\\attempt_0\\sketch\\sketch.ino"
void beginDhtSample(uint32_t nowMs);
#line 183 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_001\\direct\\attempt_0\\sketch\\sketch.ino"
void releaseDhtBus();
#line 202 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_001\\direct\\attempt_0\\sketch\\sketch.ino"
void serviceDht();
#line 257 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_001\\direct\\attempt_0\\sketch\\sketch.ino"
void beginDistanceSample(uint32_t nowMs);
#line 277 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_001\\direct\\attempt_0\\sketch\\sketch.ino"
void serviceDistance();
#line 322 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_001\\direct\\attempt_0\\sketch\\sketch.ino"
void reportAlarm(uint8_t reasons);
#line 342 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_001\\direct\\attempt_0\\sketch\\sketch.ino"
void setBuzzer(bool enabled);
#line 356 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_001\\direct\\attempt_0\\sketch\\sketch.ino"
void serviceAlarm();
#line 393 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_001\\direct\\attempt_0\\sketch\\sketch.ino"
int16_t roundedInteger(float value);
#line 397 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_001\\direct\\attempt_0\\sketch\\sketch.ino"
void serviceDisplay();
#line 426 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_001\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 457 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_001\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 92 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_001\\direct\\attempt_0\\sketch\\sketch.ino"
ISR(PCINT2_vect) {
  const bool high = (PIND & _BV(PD7)) != 0;
  const uint32_t nowUs = micros();

  switch (dhtCaptureState) {
    case DHT_WAIT_RESPONSE_LOW:
      if (!high) {
        dhtCaptureState = DHT_WAIT_RESPONSE_HIGH;
      }
      break;

    case DHT_WAIT_RESPONSE_HIGH:
      if (high) {
        dhtCaptureState = DHT_WAIT_RESPONSE_END;
      }
      break;

    case DHT_WAIT_RESPONSE_END:
      if (!high) {
        dhtCaptureState = DHT_WAIT_BIT_HIGH;
      }
      break;

    case DHT_WAIT_BIT_HIGH:
      if (high) {
        dhtHighStartedUs = nowUs;
        dhtCaptureState = DHT_WAIT_BIT_LOW;
      }
      break;

    case DHT_WAIT_BIT_LOW:
      if (!high) {
        const uint32_t widthUs = nowUs - dhtHighStartedUs;

        if (widthUs < 10 || widthUs > 110) {
          dhtCaptureState = DHT_ERROR;
          break;
        }

        const uint8_t byteIndex = dhtBitCount >> 3;
        dhtBytes[byteIndex] =
            static_cast<uint8_t>((dhtBytes[byteIndex] << 1) |
                                 (widthUs > 50 ? 1 : 0));

        ++dhtBitCount;
        dhtCaptureState =
            (dhtBitCount == 40) ? DHT_DONE : DHT_WAIT_BIT_HIGH;
      }
      break;

    default:
      break;
  }
}

// D9 is PB1 / PCINT1 on the Uno.
ISR(PCINT0_vect) {
  const bool high = (PINB & _BV(PB1)) != 0;
  const uint32_t nowUs = micros();

  if (echoState == ECHO_WAIT_RISE && high) {
    echoRiseUs = nowUs;
    echoState = ECHO_WAIT_FALL;
  } else if (echoState == ECHO_WAIT_FALL && !high) {
    echoWidthUs = nowUs - echoRiseUs;
    echoFallUs = nowUs;
    echoState = ECHO_DONE;
  }
}

void disableDhtCapture() {
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    PCMSK2 &= ~_BV(PCINT23);
    dhtCaptureState = DHT_DISABLED;
  }
}

void beginDhtSample(uint32_t nowMs) {
  disableDhtCapture();

  digitalWrite(DHT_PIN, LOW);
  pinMode(DHT_PIN, OUTPUT);

  lastDhtStartMs = nowMs;
  dhtLowStartedMs = nowMs;
  dhtHasStarted = true;
  dhtTaskState = DHT_START_LOW;

  // Retain the preceding measurement while the new acquisition is pending.
}

void releaseDhtBus() {
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    for (uint8_t i = 0; i < 5; ++i) {
      dhtBytes[i] = 0;
    }

    dhtBitCount = 0;
    dhtCaptureState = DHT_WAIT_RESPONSE_LOW;
    PCIFR = _BV(PCIF2);
    PCMSK2 |= _BV(PCINT23);
    PCICR |= _BV(PCIE2);

    dhtReleasedUs = micros();
    pinMode(DHT_PIN, INPUT_PULLUP);
  }

  dhtTaskState = DHT_CAPTURING;
}

void serviceDht() {
  const uint32_t nowMs = millis();

  if (dhtTaskState == DHT_IDLE) {
    if (!dhtHasStarted ||
        static_cast<uint32_t>(nowMs - lastDhtStartMs) >= DHT_INTERVAL_MS) {
      beginDhtSample(nowMs);
    }
    return;
  }

  if (dhtTaskState == DHT_START_LOW) {
    if (static_cast<uint32_t>(nowMs - dhtLowStartedMs) >=
        DHT_START_LOW_MS) {
      releaseDhtBus();
    }
    return;
  }

  const uint8_t state = dhtCaptureState;

  if (state == DHT_DONE) {
    uint8_t data[5];

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
      PCMSK2 &= ~_BV(PCINT23);
      for (uint8_t i = 0; i < 5; ++i) {
        data[i] = dhtBytes[i];
      }
      dhtCaptureState = DHT_DISABLED;
    }

    const uint8_t checksum =
        static_cast<uint8_t>(data[0] + data[1] + data[2] + data[3]);

    temperatureValid = (checksum == data[4]);
    if (temperatureValid) {
      float value = static_cast<float>(data[2] & 0x7F) +
                    static_cast<float>(data[3]) * 0.1f;
      if (data[2] & 0x80) {
        value = -value;
      }
      temperatureC = value;
    }

    dhtTaskState = DHT_IDLE;
  } else if (state == DHT_ERROR ||
             static_cast<uint32_t>(micros() - dhtReleasedUs) >=
                 DHT_RESPONSE_TIMEOUT_US) {
    disableDhtCapture();
    temperatureValid = false;
    dhtTaskState = DHT_IDLE;
  }
}

void beginDistanceSample(uint32_t nowMs) {
  digitalWrite(TRIG_PIN, LOW);

  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    echoState = ECHO_WAIT_RISE;
    echoWidthUs = 0;
    echoFallUs = 0;
    PCIFR = _BV(PCIF0);
  }

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  echoWaitStartedUs = micros();
  lastDistanceStartMs = nowMs;
  distanceHasStarted = true;
  distancePending = true;
}

void serviceDistance() {
  if (distancePending) {
    uint8_t state;
    uint32_t widthUs = 0;
    uint32_t completedUs = 0;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
      state = echoState;
      if (state == ECHO_DONE) {
        widthUs = echoWidthUs;
        completedUs = echoFallUs;
        echoState = ECHO_IDLE;
      }
    }

    if (state == ECHO_DONE) {
      const float measuredCm = static_cast<float>(widthUs) / 58.0f;
      distanceValid =
          static_cast<uint32_t>(completedUs - echoWaitStartedUs) <=
              ECHO_TIMEOUT_US &&
          measuredCm >= 2.0f && measuredCm <= 400.0f;

      if (distanceValid) {
        distanceCm = measuredCm;
      }
      distancePending = false;
    } else if (static_cast<uint32_t>(micros() - echoWaitStartedUs) >=
               ECHO_TIMEOUT_US) {
      ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        echoState = ECHO_IDLE;
      }
      distanceValid = false;
      distancePending = false;
    }
  }

  const uint32_t nowMs = millis();
  if (!distancePending &&
      (!distanceHasStarted ||
       static_cast<uint32_t>(nowMs - lastDistanceStartMs) >=
           DISTANCE_INTERVAL_MS)) {
    beginDistanceSample(nowMs);
  }
}

void reportAlarm(uint8_t reasons) {
  if (reasons == 0) {
    Serial.println(F("ALARM_CLEAR"));
    return;
  }

  if (reasons & REASON_TEMP) {
    Serial.print(F("ALARM_TEMP"));
  }

  if (reasons & REASON_DISTANCE) {
    if (reasons & REASON_TEMP) {
      Serial.print(' ');
    }
    Serial.print(F("ALARM_DISTANCE"));
  }

  Serial.println();
}

void setBuzzer(bool enabled) {
  if (enabled == buzzerOn) {
    return;
  }

  buzzerOn = enabled;
  if (enabled) {
    tone(BUZZER_PIN, 1000);
  } else {
    noTone(BUZZER_PIN);
    digitalWrite(BUZZER_PIN, LOW);
  }
}

void serviceAlarm() {
  const uint32_t nowMs = millis();
  uint8_t currentReasons = 0;

  // Compare the original measurements, never their displayed integers.
  if (temperatureValid && temperatureC > 32.0f) {
    currentReasons |= REASON_TEMP;
  }
  if (distanceValid && distanceCm < 15.0f) {
    currentReasons |= REASON_DISTANCE;
  }

  const bool reasonsChanged = currentReasons != alarmReasons;

  if (currentReasons != 0 && alarmReasons == 0) {
    alarmStartedMs = nowMs;
  }

  alarmReasons = currentReasons;

  if (alarmReasons != 0) {
    const uint32_t phaseMs =
        static_cast<uint32_t>(nowMs - alarmStartedMs) % 1000UL;
    setBuzzer(phaseMs < 500);
  } else {
    setBuzzer(false);
  }

  if (reasonsChanged ||
      (alarmReasons != 0 &&
       static_cast<uint32_t>(nowMs - lastAlarmReportMs) >=
           ALARM_REPORT_INTERVAL_MS)) {
    reportAlarm(alarmReasons);
    lastAlarmReportMs = nowMs;
  }
}

int16_t roundedInteger(float value) {
  return static_cast<int16_t>(value >= 0.0f ? value + 0.5f : value - 0.5f);
}

void serviceDisplay() {
  const uint32_t nowMs = millis();

  while (static_cast<uint32_t>(nowMs - pageStartedMs) >= PAGE_INTERVAL_MS) {
    pageStartedMs += PAGE_INTERVAL_MS;
    showTemperature = !showTemperature;
  }

  const bool valid = showTemperature ? temperatureValid : distanceValid;
  const float value = showTemperature ? temperatureC : distanceCm;
  const int16_t integerValue = valid ? roundedInteger(value) : 0;

  if (!displayInitialized ||
      displayedTemperaturePage != showTemperature ||
      displayedValid != valid ||
      (valid && displayedInteger != integerValue)) {
    if (valid) {
      display.showNumberDec(integerValue, false);
    } else {
      display.clear();
    }

    displayInitialized = true;
    displayedTemperaturePage = showTemperature;
    displayedValid = valid;
    displayedInteger = integerValue;
  }
}

void setup() {
  Serial.begin(SERIAL_BAUD);

  pinMode(DHT_PIN, INPUT_PULLUP);
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  noTone(BUZZER_PIN);

  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    PCMSK0 |= _BV(PCINT1);
    PCIFR = _BV(PCIF0);
    PCICR |= _BV(PCIE0);
  }

  display.setBrightness(7, true);
  display.clear();

  pageStartedMs = millis();
  showTemperature = true;
  displayedTemperaturePage = true;
  displayedValid = false;
  displayInitialized = true;

  // Both first acquisitions begin well within 200 ms of startup.
  beginDhtSample(millis());
  beginDistanceSample(millis());
}

void loop() {
  serviceDht();
  serviceDistance();
  serviceAlarm();
  serviceDisplay();
}
