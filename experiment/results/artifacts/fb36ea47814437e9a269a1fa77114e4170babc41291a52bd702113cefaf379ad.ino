#include <Arduino.h>
#include <TM1637Display.h>

// Wiring:
// TM1637 CLK -> D4, DIO -> D5
// D2: increase selected stage by 60 seconds (maximum 99:59)
// D3: select the next stage for configuration
// D7: start / pause / resume; after completion, start a new run
// Buttons connect their input pin to GND and use INPUT_PULLUP.
// D6: passive buzzer; D13: completion LED.
//
// Configuration is available before starting and after completion.
// During running or paused states, stage durations cannot be changed.
// Actual stage times count active time only, excluding pauses.
// Stage completion starts the next stage immediately; alarms never block.

constexpr uint8_t PIN_INCREASE = 2;
constexpr uint8_t PIN_SELECT   = 3;
constexpr uint8_t PIN_CLK      = 4;
constexpr uint8_t PIN_DIO      = 5;
constexpr uint8_t PIN_BUZZER   = 6;
constexpr uint8_t PIN_START    = 7;
constexpr uint8_t PIN_LED      = 13;

constexpr uint8_t STAGE_COUNT = 3;
constexpr uint16_t MAX_SECONDS = 99U * 60U + 59U;
constexpr uint16_t INCREMENT_SECONDS = 60;
constexpr uint32_t DEBOUNCE_MS = 30;
constexpr uint32_t BEEP_ON_MS = 180;
constexpr uint32_t BEEP_GAP_MS = 180;
constexpr uint32_t ALARM_GAP_MS = 600;
constexpr unsigned int BUZZER_HZ = 2200;

TM1637Display display(PIN_CLK, PIN_DIO);

class Button {
public:
  explicit Button(uint8_t pin) : pin_(pin) {}

  void begin() {
    pinMode(pin_, INPUT_PULLUP);
    raw_ = stable_ = digitalRead(pin_);
    changedAt_ = millis();
  }

  bool pressed(uint32_t now) {
    const uint8_t reading = digitalRead(pin_);
    if (reading != raw_) {
      raw_ = reading;
      changedAt_ = now;
    }

    if (raw_ != stable_ &&
        static_cast<uint32_t>(now - changedAt_) >= DEBOUNCE_MS) {
      stable_ = raw_;
      return stable_ == LOW;
    }
    return false;
  }

private:
  uint8_t pin_;
  uint8_t raw_ = HIGH;
  uint8_t stable_ = HIGH;
  uint32_t changedAt_ = 0;
};

Button increaseButton(PIN_INCREASE);
Button selectButton(PIN_SELECT);
Button startButton(PIN_START);

enum ProcessState : uint8_t {
  CONFIGURING,
  RUNNING,
  PAUSED,
  COMPLETE
};

ProcessState processState = CONFIGURING;
uint16_t stageSeconds[STAGE_COUNT] = {60, 60, 60};
uint32_t actualMilliseconds[STAGE_COUNT] = {0, 0, 0};
uint8_t selectedStage = 0;
uint8_t currentStage = 0;
uint32_t lastAccountingAt = 0;

// Each completed stage queues its own group of 1, 2, or 3 beeps.
constexpr uint8_t ALARM_QUEUE_SIZE = 8;
uint8_t alarmQueue[ALARM_QUEUE_SIZE];
uint8_t alarmHead = 0;
uint8_t alarmTail = 0;
uint8_t alarmCount = 0;

enum AlarmState : uint8_t {
  ALARM_IDLE,
  ALARM_ON,
  ALARM_BETWEEN_BEEPS,
  ALARM_BETWEEN_GROUPS
};

AlarmState alarmState = ALARM_IDLE;
uint8_t beepsRemaining = 0;
uint32_t alarmChangedAt = 0;

void queueAlarm(uint8_t beepCount) {
  // With a minimum stage duration of 60 seconds and an alarm duration
  // below two seconds, this queue cannot fill during normal operation.
  if (alarmCount < ALARM_QUEUE_SIZE) {
    alarmQueue[alarmTail] = beepCount;
    alarmTail = (alarmTail + 1) % ALARM_QUEUE_SIZE;
    ++alarmCount;
  }
}

void beginBeep(uint32_t now) {
  tone(PIN_BUZZER, BUZZER_HZ);
  alarmChangedAt = now;
  alarmState = ALARM_ON;
}

void updateAlarm(uint32_t now) {
  switch (alarmState) {
    case ALARM_IDLE:
      if (alarmCount != 0) {
        beepsRemaining = alarmQueue[alarmHead];
        alarmHead = (alarmHead + 1) % ALARM_QUEUE_SIZE;
        --alarmCount;
        beginBeep(now);
      }
      break;

    case ALARM_ON:
      if (static_cast<uint32_t>(now - alarmChangedAt) >= BEEP_ON_MS) {
        noTone(PIN_BUZZER);
        --beepsRemaining;
        alarmChangedAt = now;
        alarmState = beepsRemaining
                         ? ALARM_BETWEEN_BEEPS
                         : ALARM_BETWEEN_GROUPS;
      }
      break;

    case ALARM_BETWEEN_BEEPS:
      if (static_cast<uint32_t>(now - alarmChangedAt) >= BEEP_GAP_MS) {
        beginBeep(now);
      }
      break;

    case ALARM_BETWEEN_GROUPS:
      if (static_cast<uint32_t>(now - alarmChangedAt) >= ALARM_GAP_MS) {
        alarmState = ALARM_IDLE;
      }
      break;
  }
}

void reportConfiguration() {
  Serial.print(F("selected stage "));
  Serial.print(selectedStage + 1);
  Serial.print(F(": "));
  Serial.print(stageSeconds[selectedStage]);
  Serial.println(F(" seconds"));
}

void reportCompletion() {
  Serial.println(F("process complete"));
  for (uint8_t i = 0; i < STAGE_COUNT; ++i) {
    Serial.print(F("stage "));
    Serial.print(i + 1);
    Serial.print(F(" actual elapsed: "));
    Serial.print(actualMilliseconds[i]);
    Serial.println(F(" ms (active time, pauses excluded)"));
  }
}

void startProcess(uint32_t now) {
  for (uint8_t i = 0; i < STAGE_COUNT; ++i) {
    actualMilliseconds[i] = 0;
  }
  currentStage = 0;
  lastAccountingAt = now;
  processState = RUNNING;
  digitalWrite(PIN_LED, LOW);
  Serial.println(F("process started"));
  Serial.println(F("stage 1 started"));
}

void updateProcess(uint32_t now) {
  if (processState != RUNNING) {
    return;
  }

  const uint32_t elapsed = now - lastAccountingAt;
  lastAccountingAt = now;
  actualMilliseconds[currentStage] += elapsed;

  const uint32_t targetMilliseconds =
      static_cast<uint32_t>(stageSeconds[currentStage]) * 1000UL;

  if (actualMilliseconds[currentStage] < targetMilliseconds) {
    return;
  }

  // Preserve the measured elapsed time, including loop timing resolution.
  queueAlarm(currentStage + 1);

  Serial.print(F("stage "));
  Serial.print(currentStage + 1);
  Serial.println(F(" complete"));

  ++currentStage;
  if (currentStage == STAGE_COUNT) {
    processState = COMPLETE;
    digitalWrite(PIN_LED, HIGH);
    reportCompletion();
  } else {
    // The next stage starts immediately, independently of the buzzer.
    Serial.print(F("stage "));
    Serial.print(currentStage + 1);
    Serial.println(F(" started"));
  }
}

void enterConfiguration() {
  processState = CONFIGURING;
  digitalWrite(PIN_LED, LOW);
}

void updateDisplay(uint32_t now) {
  static uint16_t previousNumber = 0xFFFF;
  static uint8_t previousDots = 0xFF;

  uint16_t seconds;
  uint8_t dots = 0x40;  // Center colon.

  if (processState == CONFIGURING) {
    seconds = stageSeconds[selectedStage];
  } else if (processState == COMPLETE) {
    seconds = 0;
  } else {
    const uint32_t target =
        static_cast<uint32_t>(stageSeconds[currentStage]) * 1000UL;
    const uint32_t elapsed = actualMilliseconds[currentStage];
    const uint32_t remaining = elapsed < target ? target - elapsed : 0;
    seconds = static_cast<uint16_t>((remaining + 999UL) / 1000UL);

    if (processState == RUNNING) {
      dots = ((now / 500UL) & 1U) ? 0x40 : 0;
    } else {
      dots = ((now / 1000UL) & 1U) ? 0x40 : 0;
    }
  }

  const uint16_t number = (seconds / 60U) * 100U + seconds % 60U;
  if (number != previousNumber || dots != previousDots) {
    display.showNumberDecEx(number, dots, true);
    previousNumber = number;
    previousDots = dots;
  }
}

void setup() {
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);
  pinMode(PIN_BUZZER, OUTPUT);
  digitalWrite(PIN_BUZZER, LOW);

  increaseButton.begin();
  selectButton.begin();
  startButton.begin();

  Serial.begin(9600);
  display.setBrightness(5, true);
  display.clear();

  Serial.println(F("Three-stage countdown ready"));
  Serial.println(F("D2: +60s, D3: select stage, D7: start/pause/resume"));
  reportConfiguration();
  updateDisplay(millis());
}

void loop() {
  const uint32_t now = millis();

  // Account for running time before handling a pause press.
  updateProcess(now);

  const bool increasePressed = increaseButton.pressed(now);
  const bool selectPressed = selectButton.pressed(now);
  const bool startPressed = startButton.pressed(now);

  if (processState == CONFIGURING || processState == COMPLETE) {
    if (selectPressed) {
      enterConfiguration();
      selectedStage = (selectedStage + 1) % STAGE_COUNT;
      reportConfiguration();
    }

    if (increasePressed) {
      enterConfiguration();
      uint32_t increased =
          static_cast<uint32_t>(stageSeconds[selectedStage]) +
          INCREMENT_SECONDS;
      stageSeconds[selectedStage] =
          increased > MAX_SECONDS ? MAX_SECONDS
                                  : static_cast<uint16_t>(increased);
      reportConfiguration();
    }
  }

  if (startPressed) {
    switch (processState) {
      case CONFIGURING:
      case COMPLETE:
        startProcess(now);
        break;

      case RUNNING:
        processState = PAUSED;
        Serial.println(F("process paused"));
        break;

      case PAUSED:
        lastAccountingAt = now;
        processState = RUNNING;
        Serial.println(F("process resumed"));
        break;
    }
  }

  updateAlarm(now);
  updateDisplay(now);
}