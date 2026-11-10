#include <Arduino.h>
#include <TM1637Display.h>

// Wiring: vibration D2, button D3, TM1637 CLK D4 / DIO D5,
// buzzer D6, water sensor A0, LED D13.
// Install the "TM1637Display" library before compiling.
constexpr uint8_t VIBRATION_PIN = 2;
constexpr uint8_t BUTTON_PIN = 3;
constexpr uint8_t DISPLAY_CLK_PIN = 4;
constexpr uint8_t DISPLAY_DIO_PIN = 5;
constexpr uint8_t BUZZER_PIN = 6;
constexpr uint8_t LED_PIN = 13;
constexpr uint8_t WATER_PIN = A0;

// Change to LOW if the vibration module has an active-low output.
constexpr uint8_t VIBRATION_ACTIVE_LEVEL = HIGH;

// Water readings are assumed to increase with water level.
// Calibrate these thresholds for the installed sensor.
constexpr uint16_t WATER_PRESENT_THRESHOLD = 450;
constexpr uint16_t WATER_EMPTY_THRESHOLD = 300;
constexpr uint16_t WATER_SATURATION_THRESHOLD = 1020;

constexpr unsigned long WATER_SAMPLE_MS = 50;
constexpr unsigned long WATER_EMPTY_CONFIRM_MS = 5000;
constexpr unsigned long VIBRATION_RECENT_MS = 4000;
constexpr unsigned long END_QUIET_MS = 25000;
constexpr unsigned long FAULT_QUIET_MS = 120000;
constexpr unsigned long BUTTON_DEBOUNCE_MS = 35;
constexpr unsigned long LED_BLINK_MS = 300;
constexpr unsigned long BEEP_PERIOD_MS = 1000;
constexpr unsigned long BEEP_ON_MS = 350;
constexpr unsigned int BEEP_FREQUENCY_HZ = 2200;

enum Stage : uint8_t {
  STANDBY = 0,
  FILLING = 1,
  WASHING = 2,
  SPINNING = 3,
  FINISHED = 4
};

TM1637Display display(DISPLAY_CLK_PIN, DISPLAY_DIO_PIN);

volatile bool vibrationPending = false;

Stage stage = STANDBY;
bool cycleActive = false;
bool washedOrSpun = false;
bool alarmActive = false;
bool buzzerOn = false;
bool faultReported = false;
bool waterPresent = false;
bool emptyTiming = false;
bool restartArmed = true;

uint16_t rawWater = 0;
uint16_t filteredWater = 0;

unsigned long lastWaterSample = 0;
unsigned long lastVibration = 0;
unsigned long emptySince = 0;
unsigned long alarmStarted = 0;

uint8_t buttonRaw = HIGH;
uint8_t buttonStable = HIGH;
unsigned long buttonChangedAt = 0;

void vibrationISR() {
  vibrationPending = true;
}

void showStage(Stage next) {
  if (stage != next) {
    stage = next;
    display.showNumberDec(static_cast<int>(stage), false);
  }
}

void stopAlarm() {
  alarmActive = false;
  buzzerOn = false;
  noTone(BUZZER_PIN);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
}

void startCycle(unsigned long now, Stage initialStage) {
  stopAlarm();
  cycleActive = true;
  washedOrSpun = (initialStage == WASHING || initialStage == SPINNING);
  faultReported = false;
  lastVibration = now;
  restartArmed = false;
  showStage(initialStage);
}

void finishCycle(unsigned long now) {
  cycleActive = false;
  alarmActive = true;
  alarmStarted = now;
  faultReported = false;
  restartArmed = false;
  showStage(FINISHED);
}

bool buttonPressed(unsigned long now) {
  const uint8_t reading = digitalRead(BUTTON_PIN);

  if (reading != buttonRaw) {
    buttonRaw = reading;
    buttonChangedAt = now;
  }

  if (buttonStable != buttonRaw &&
      now - buttonChangedAt >= BUTTON_DEBOUNCE_MS) {
    buttonStable = buttonRaw;
    return buttonStable == LOW;
  }

  return false;
}

void updateWater(unsigned long now) {
  if (now - lastWaterSample < WATER_SAMPLE_MS) {
    return;
  }
  lastWaterSample = now;

  rawWater = analogRead(WATER_PIN);
  filteredWater =
      (static_cast<uint32_t>(filteredWater) * 7UL + rawWater) / 8UL;

  if (!waterPresent && filteredWater >= WATER_PRESENT_THRESHOLD) {
    waterPresent = true;
  } else if (waterPresent && filteredWater <= WATER_EMPTY_THRESHOLD) {
    waterPresent = false;
  }

  // Require a continuously low reading before declaring the drum empty.
  if (filteredWater <= WATER_EMPTY_THRESHOLD) {
    if (!emptyTiming) {
      emptyTiming = true;
      emptySince = now;
    }
  } else {
    emptyTiming = false;
  }
}

void updateAlarm(unsigned long now) {
  if (!alarmActive) {
    return;
  }

  const unsigned long elapsed = now - alarmStarted;
  digitalWrite(LED_PIN, ((elapsed / LED_BLINK_MS) % 2UL) == 0UL
                           ? HIGH
                           : LOW);

  const bool shouldBeep = (elapsed % BEEP_PERIOD_MS) < BEEP_ON_MS;
  if (shouldBeep != buzzerOn) {
    buzzerOn = shouldBeep;
    if (buzzerOn) {
      tone(BUZZER_PIN, BEEP_FREQUENCY_HZ);
    } else {
      noTone(BUZZER_PIN);
      digitalWrite(BUZZER_PIN, LOW);
    }
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(VIBRATION_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(WATER_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  stopAlarm();

  display.setBrightness(5, true);
  display.showNumberDec(0, false);

  rawWater = analogRead(WATER_PIN);
  filteredWater = rawWater;
  waterPresent = filteredWater >= WATER_PRESENT_THRESHOLD;

  const unsigned long now = millis();
  lastWaterSample = now;
  lastVibration = now;
  emptyTiming = filteredWater <= WATER_EMPTY_THRESHOLD;
  emptySince = now;

  buttonRaw = digitalRead(BUTTON_PIN);
  buttonStable = buttonRaw;
  buttonChangedAt = now;

  attachInterrupt(digitalPinToInterrupt(VIBRATION_PIN), vibrationISR,
                  VIBRATION_ACTIVE_LEVEL == HIGH ? RISING : FALLING);
}

void loop() {
  const unsigned long now = millis();

  noInterrupts();
  const bool vibrationDetected = vibrationPending;
  vibrationPending = false;
  interrupts();

  if (vibrationDetected) {
    lastVibration = now;
    faultReported = false;
  }

  updateWater(now);

  const bool emptyConfirmed =
      emptyTiming && now - emptySince >= WATER_EMPTY_CONFIRM_MS;

  const bool acknowledged = buttonPressed(now);
  if (acknowledged && alarmActive) {
    stopAlarm();
    // Keep displaying stage 4 until the next cycle starts.
  }

  // Completion remains latched until acknowledgement, even if sensors change.
  if (stage == FINISHED) {
    if (!alarmActive) {
      // Observing an empty drum rearms detection of a new cycle.
      if (emptyConfirmed) {
        restartArmed = true;
      }

      if (restartArmed && waterPresent) {
        startCycle(now, FILLING);
      } else if (restartArmed && vibrationDetected && emptyConfirmed) {
        // Also support a spin-only cycle.
        startCycle(now, SPINNING);
      }
    }

    updateAlarm(now);
    return;
  }

  if (!cycleActive) {
    if (waterPresent) {
      startCycle(now, FILLING);
    } else if (vibrationDetected && emptyConfirmed) {
      startCycle(now, SPINNING);
    }
  }

  if (cycleActive) {
    const unsigned long quietFor = now - lastVibration;
    const bool vibrating =
        vibrationDetected || quietFor < VIBRATION_RECENT_MS;

    switch (stage) {
      case FILLING:
        // Water without agitation indicates filling; do not mistake a
        // stalled filling cycle for normal completion.
        if (waterPresent && vibrating) {
          washedOrSpun = true;
          showStage(WASHING);
        } else if (emptyConfirmed && vibrating) {
          washedOrSpun = true;
          showStage(SPINNING);
        }
        break;

      case WASHING:
        if (emptyConfirmed && vibrating) {
          washedOrSpun = true;
          showStage(SPINNING);
        }
        // Quiet periods with retained water remain washing (e.g. soaking).
        break;

      case SPINNING:
        // A refill/agitation sequence may be a rinse, not a new cycle.
        if (waterPresent) {
          showStage(vibrating ? WASHING : FILLING);
        }
        break;

      default:
        break;
    }

    const bool abnormalWater =
        waterPresent || filteredWater >= WATER_SATURATION_THRESHOLD;

    if (quietFor >= FAULT_QUIET_MS && abnormalWater) {
      if (!faultReported) {
        Serial.println(F("wash fault"));
        faultReported = true;
      }
    } else {
      faultReported = false;
    }

    // Require a previously observed wash/spin, confirmed drainage, and a
    // sustained absence of vibration to avoid alarms during short pauses.
    if (washedOrSpun && emptyConfirmed && quietFor >= END_QUIET_MS) {
      finishCycle(now);
    }
  }

  updateAlarm(now);
}