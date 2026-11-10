#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_025\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>
#include <stdlib.h>
#include <string.h>

// Wiring:
// LM35 OUT -> A1; buttons -> D2/D3/D4 and GND (internal pull-ups).
// Buzzer -> D6; TM1637 CLK -> D7, DIO -> D5; LED -> D13.
//
// Controls:
// D2 short press: select the next setting (temperature/time for each stage).
// D2 hold 1.2 s: start; while running, hold again to abort.
// D3 / D4: increase / decrease the selected setting.
// D3 / D4 hold: increase / decrease by 10 C or 60 seconds.
//
// Display:
// First digit = stage (1=preheat, 2=cook, 3=keep warm).
// Last three digits = remaining seconds while running.
// During configuration, last three digits show the selected setting;
// the first decimal point identifies temperature rather than time.
//
// Each stage waits until its target is reached, then counts down its
// configured duration. Countdown continues if temperature subsequently falls.
// No heater/relay was specified: this system monitors and prompts only.
//
// Serial commands, terminated by CR/LF or 20 ms of idle:
// HELP, STATUS, START, STOP, SET <stage 1..3> <temperature 1..150> <seconds 1..999>

const uint8_t LM35_PIN = A1;
const uint8_t SELECT_PIN = 2;
const uint8_t UP_PIN = 3;
const uint8_t DOWN_PIN = 4;
const uint8_t BUZZER_PIN = 6;
const uint8_t TM_CLK_PIN = 7;
const uint8_t TM_DIO_PIN = 5;
const uint8_t LED_PIN = 13;

const float ADC_REFERENCE_VOLTS = 5.0f; // Calibrate to measured UNO AVcc.
const float TEMPERATURE_OFFSET_C = 0.0f;
const uint32_t DEBOUNCE_MS = 30;
const uint32_t LONG_PRESS_MS = 1200;
const uint32_t TEMPERATURE_SAMPLE_MS = 100;
const uint32_t LOG_INTERVAL_MS = 5000;
const uint32_t SERIAL_IDLE_MS = 20;

TM1637Display display(TM_CLK_PIN, TM_DIO_PIN);

struct Stage {
  uint16_t targetC;
  uint16_t durationSeconds;
};

Stage stages[3] = {
  {60, 120},
  {95, 600},
  {50, 300}
};

enum SystemState : uint8_t {
  CONFIGURING,
  WAITING_FOR_TARGET,
  COUNTING_DOWN,
  FINISHED
};

SystemState state = CONFIGURING;
uint8_t currentStage = 0;
uint8_t selectedSetting = 0; // stage * 2 + (0: temperature, 1: duration)
float temperatureC = 0.0f;
uint32_t countdownStartedAt = 0;
uint32_t lastTemperatureSample = 0;
uint32_t lastProcessLog = 0;
uint32_t lastDisplayUpdate = 0;
bool displayDirty = true;

struct Button {
  uint8_t pin;
  bool rawPressed;
  bool stablePressed;
  bool longReported;
  uint32_t rawChangedAt;
  uint32_t pressedAt;
};

Button buttons[3] = {
  {SELECT_PIN, false, false, false, 0, 0},
  {UP_PIN, false, false, false, 0, 0},
  {DOWN_PIN, false, false, false, 0, 0}
};

uint16_t pendingBeeps = 0;
bool buzzerOn = false;
bool buzzerGap = false;
uint32_t buzzerChangedAt = 0;

char serialBuffer[80];
uint8_t serialLength = 0;
bool serialOverflow = false;
uint32_t lastSerialByteAt = 0;

#line 101 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_025\\direct\\attempt_0\\sketch\\sketch.ino"
bool isRunning();
#line 105 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_025\\direct\\attempt_0\\sketch\\sketch.ino"
void printStageName(uint8_t index);
#line 113 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_025\\direct\\attempt_0\\sketch\\sketch.ino"
void logPrefix();
#line 119 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_025\\direct\\attempt_0\\sketch\\sketch.ino"
void queueBeeps(uint8_t count);
#line 123 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_025\\direct\\attempt_0\\sketch\\sketch.ino"
void serviceBuzzer(uint32_t now);
#line 147 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_025\\direct\\attempt_0\\sketch\\sketch.ino"
void readTemperature();
#line 159 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_025\\direct\\attempt_0\\sketch\\sketch.ino"
uint16_t remainingSeconds(uint32_t now);
#line 172 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_025\\direct\\attempt_0\\sketch\\sketch.ino"
void printSettings();
#line 186 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_025\\direct\\attempt_0\\sketch\\sketch.ino"
void printSelectedSetting();
#line 202 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_025\\direct\\attempt_0\\sketch\\sketch.ino"
void printStatus();
#line 228 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_025\\direct\\attempt_0\\sketch\\sketch.ino"
void beginStage(uint8_t index);
#line 247 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_025\\direct\\attempt_0\\sketch\\sketch.ino"
void startProcess();
#line 263 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_025\\direct\\attempt_0\\sketch\\sketch.ino"
void stopProcess();
#line 284 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_025\\direct\\attempt_0\\sketch\\sketch.ino"
void serviceProcess(uint32_t now);
#line 337 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_025\\direct\\attempt_0\\sketch\\sketch.ino"
void changeSetting(bool increase, bool largeStep);
#line 356 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_025\\direct\\attempt_0\\sketch\\sketch.ino"
void handleButton(uint8_t index, bool longPress);
#line 376 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_025\\direct\\attempt_0\\sketch\\sketch.ino"
void serviceButtons(uint32_t now);
#line 405 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_025\\direct\\attempt_0\\sketch\\sketch.ino"
void updateDisplay(uint32_t now);
#line 435 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_025\\direct\\attempt_0\\sketch\\sketch.ino"
bool parseInteger(const char *text, long &value);
#line 442 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_025\\direct\\attempt_0\\sketch\\sketch.ino"
void printHelp();
#line 453 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_025\\direct\\attempt_0\\sketch\\sketch.ino"
void executeSerialCommand();
#line 510 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_025\\direct\\attempt_0\\sketch\\sketch.ino"
void serviceSerial();
#line 530 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_025\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 563 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_025\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 101 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_025\\direct\\attempt_0\\sketch\\sketch.ino"
bool isRunning() {
  return state == WAITING_FOR_TARGET || state == COUNTING_DOWN;
}

void printStageName(uint8_t index) {
  switch (index) {
    case 0: Serial.print(F("PREHEAT")); break;
    case 1: Serial.print(F("COOK")); break;
    default: Serial.print(F("KEEP_WARM")); break;
  }
}

void logPrefix() {
  Serial.print('[');
  Serial.print(millis());
  Serial.print(F(" ms] "));
}

void queueBeeps(uint8_t count) {
  pendingBeeps += count;
}

void serviceBuzzer(uint32_t now) {
  if (buzzerOn) {
    if ((uint32_t)(now - buzzerChangedAt) >= 100) {
      noTone(BUZZER_PIN);
      buzzerOn = false;
      buzzerGap = true;
      buzzerChangedAt = now;
    }
    return;
  }

  if (buzzerGap) {
    if ((uint32_t)(now - buzzerChangedAt) < 120) return;
    buzzerGap = false;
  }

  if (pendingBeeps > 0) {
    --pendingBeeps;
    tone(BUZZER_PIN, 2400);
    buzzerOn = true;
    buzzerChangedAt = now;
  }
}

void readTemperature() {
  // Discard one conversion, then average to reduce ADC noise.
  analogRead(LM35_PIN);
  uint32_t sum = 0;
  for (uint8_t i = 0; i < 16; ++i) {
    sum += analogRead(LM35_PIN);
  }
  const float adc = sum / 16.0f;
  temperatureC = adc * ADC_REFERENCE_VOLTS * 100.0f / 1023.0f
                 + TEMPERATURE_OFFSET_C;
}

uint16_t remainingSeconds(uint32_t now) {
  if (state == WAITING_FOR_TARGET) {
    return stages[currentStage].durationSeconds;
  }
  if (state != COUNTING_DOWN) return 0;

  const uint32_t durationMs =
      (uint32_t)stages[currentStage].durationSeconds * 1000UL;
  const uint32_t elapsed = now - countdownStartedAt;
  if (elapsed >= durationMs) return 0;
  return (uint16_t)((durationMs - elapsed + 999UL) / 1000UL);
}

void printSettings() {
  for (uint8_t i = 0; i < 3; ++i) {
    Serial.print(F("Stage "));
    Serial.print(i + 1);
    Serial.print(' ');
    printStageName(i);
    Serial.print(F(": target="));
    Serial.print(stages[i].targetC);
    Serial.print(F(" C, duration="));
    Serial.print(stages[i].durationSeconds);
    Serial.println(F(" s"));
  }
}

void printSelectedSetting() {
  const uint8_t index = selectedSetting / 2;
  logPrefix();
  Serial.print(F("Editing stage "));
  Serial.print(index + 1);
  if ((selectedSetting & 1) == 0) {
    Serial.print(F(" target: "));
    Serial.print(stages[index].targetC);
    Serial.println(F(" C"));
  } else {
    Serial.print(F(" duration: "));
    Serial.print(stages[index].durationSeconds);
    Serial.println(F(" s"));
  }
}

void printStatus() {
  logPrefix();
  Serial.print(F("State="));
  switch (state) {
    case CONFIGURING: Serial.print(F("CONFIGURING")); break;
    case WAITING_FOR_TARGET: Serial.print(F("WAITING_FOR_TARGET")); break;
    case COUNTING_DOWN: Serial.print(F("COUNTING_DOWN")); break;
    case FINISHED: Serial.print(F("FINISHED")); break;
  }
  Serial.print(F(", temperature="));
  Serial.print(temperatureC, 1);
  Serial.print(F(" C"));
  if (isRunning()) {
    Serial.print(F(", stage="));
    Serial.print(currentStage + 1);
    Serial.print(' ');
    printStageName(currentStage);
    Serial.print(F(", target="));
    Serial.print(stages[currentStage].targetC);
    Serial.print(F(" C, remaining="));
    Serial.print(remainingSeconds(millis()));
    Serial.print(F(" s"));
  }
  Serial.println();
}

void beginStage(uint8_t index) {
  currentStage = index;
  state = WAITING_FOR_TARGET;
  displayDirty = true;

  logPrefix();
  Serial.print(F("ENTER stage "));
  Serial.print(index + 1);
  Serial.print(' ');
  printStageName(index);
  Serial.print(F("; target="));
  Serial.print(stages[index].targetC);
  Serial.print(F(" C; duration="));
  Serial.print(stages[index].durationSeconds);
  Serial.print(F(" s; current="));
  Serial.print(temperatureC, 1);
  Serial.println(F(" C; waiting for target before starting timer."));
}

void startProcess() {
  if (isRunning()) {
    Serial.println(F("ERROR: process already running."));
    return;
  }

  readTemperature();
  lastTemperatureSample = millis();
  lastProcessLog = millis();
  logPrefix();
  Serial.println(F("PROCESS START"));
  printSettings();
  queueBeeps(1);
  beginStage(0);
}

void stopProcess() {
  if (!isRunning()) {
    Serial.println(F("ERROR: no active process."));
    return;
  }

  logPrefix();
  Serial.print(F("PROCESS ABORTED at stage "));
  Serial.print(currentStage + 1);
  Serial.print(F("; remaining="));
  Serial.print(remainingSeconds(millis()));
  Serial.println(F(" s"));

  selectedSetting = currentStage * 2;
  state = CONFIGURING;
  displayDirty = true;
  digitalWrite(LED_PIN, LOW);
  queueBeeps(1);
  printSelectedSetting();
}

void serviceProcess(uint32_t now) {
  if (state == WAITING_FOR_TARGET &&
      temperatureC >= stages[currentStage].targetC) {
    state = COUNTING_DOWN;
    countdownStartedAt = now;
    displayDirty = true;
    queueBeeps(2);
    logPrefix();
    Serial.print(F("TARGET REACHED at stage "));
    Serial.print(currentStage + 1);
    Serial.print(F(": "));
    Serial.print(temperatureC, 1);
    Serial.println(F(" C; countdown started."));
  }

  if (state == COUNTING_DOWN) {
    const uint32_t durationMs =
        (uint32_t)stages[currentStage].durationSeconds * 1000UL;

    if ((uint32_t)(now - countdownStartedAt) >= durationMs) {
      logPrefix();
      Serial.print(F("STAGE "));
      Serial.print(currentStage + 1);
      Serial.print(F(" COMPLETE; temperature="));
      Serial.print(temperatureC, 1);
      Serial.println(F(" C"));
      queueBeeps(1);

      if (currentStage < 2) {
        beginStage(currentStage + 1);
      } else {
        state = FINISHED;
        displayDirty = true;
        queueBeeps(4);
        logPrefix();
        Serial.println(F("PROCESS COMPLETE: all three stages finished."));
      }
    }
  }

  if (isRunning() &&
      (uint32_t)(now - lastProcessLog) >= LOG_INTERVAL_MS) {
    lastProcessLog = now;
    printStatus();
  }

  if (state == WAITING_FOR_TARGET) {
    digitalWrite(LED_PIN, ((now / 500UL) & 1) ? HIGH : LOW);
  } else {
    digitalWrite(LED_PIN, state == COUNTING_DOWN ? HIGH : LOW);
  }
}

void changeSetting(bool increase, bool largeStep) {
  if (isRunning()) return;
  state = CONFIGURING;

  const uint8_t index = selectedSetting / 2;
  const bool editingTime = (selectedSetting & 1) != 0;
  const int step = largeStep ? (editingTime ? 60 : 10) : 1;
  const int maximum = editingTime ? 999 : 150;
  uint16_t &setting = editingTime
      ? stages[index].durationSeconds : stages[index].targetC;
  int value = (int)setting + (increase ? step : -step);
  if (value < 1) value = 1;
  if (value > maximum) value = maximum;
  setting = (uint16_t)value;

  displayDirty = true;
  printSelectedSetting();
}

void handleButton(uint8_t index, bool longPress) {
  if (index == 0) {
    if (longPress) {
      if (isRunning()) stopProcess();
      else startProcess();
    } else if (!isRunning()) {
      if (state == FINISHED) {
        state = CONFIGURING;
        selectedSetting = 0;
      } else {
        selectedSetting = (selectedSetting + 1) % 6;
      }
      displayDirty = true;
      printSelectedSetting();
    }
  } else {
    changeSetting(index == 1, longPress);
  }
}

void serviceButtons(uint32_t now) {
  for (uint8_t i = 0; i < 3; ++i) {
    Button &b = buttons[i];
    const bool pressed = digitalRead(b.pin) == LOW;

    if (pressed != b.rawPressed) {
      b.rawPressed = pressed;
      b.rawChangedAt = now;
    }

    if (b.rawPressed != b.stablePressed &&
        (uint32_t)(now - b.rawChangedAt) >= DEBOUNCE_MS) {
      b.stablePressed = b.rawPressed;
      if (b.stablePressed) {
        b.pressedAt = now;
        b.longReported = false;
      } else if (!b.longReported) {
        handleButton(i, false);
      }
    }

    if (b.stablePressed && b.rawPressed && !b.longReported &&
        (uint32_t)(now - b.pressedAt) >= LONG_PRESS_MS) {
      b.longReported = true;
      handleButton(i, true);
    }
  }
}

void updateDisplay(uint32_t now) {
  if (!displayDirty &&
      (uint32_t)(now - lastDisplayUpdate) < 100) return;

  lastDisplayUpdate = now;
  displayDirty = false;
  uint8_t segments[4];
  uint8_t stageNumber = 0;
  uint16_t value = 0;
  bool temperatureSetting = false;

  if (isRunning()) {
    stageNumber = currentStage + 1;
    value = remainingSeconds(now);
  } else if (state == CONFIGURING) {
    const uint8_t index = selectedSetting / 2;
    stageNumber = index + 1;
    temperatureSetting = (selectedSetting & 1) == 0;
    value = temperatureSetting
        ? stages[index].targetC : stages[index].durationSeconds;
  }

  segments[0] = display.encodeDigit(stageNumber);
  if (temperatureSetting) segments[0] |= 0x80;
  segments[1] = display.encodeDigit((value / 100) % 10);
  segments[2] = display.encodeDigit((value / 10) % 10);
  segments[3] = display.encodeDigit(value % 10);
  display.setSegments(segments);
}

bool parseInteger(const char *text, long &value) {
  if (text == NULL || *text == '\0') return false;
  char *end;
  value = strtol(text, &end, 10);
  return *end == '\0';
}

void printHelp() {
  Serial.println(F("D2 short: select setting; D2 hold: start/abort."));
  Serial.println(F("D3/D4: +/-1; hold: +/-10 C or +/-60 s."));
  Serial.println(F("Serial: HELP | STATUS | START | STOP"));
  Serial.println(F("SET <stage 1..3> <temperature 1..150> <seconds 1..999>"));
  Serial.println(F("Commands end with CR/LF or 20 ms idle."));
  Serial.println(F("Display: stage + 3 digits; decimal point = temperature edit."));
  Serial.println(F("LED: blinking=waiting; solid=countdown; off=idle/finished."));
  Serial.println(F("Monitoring only: no heater output is connected."));
}

void executeSerialCommand() {
  if (serialOverflow) {
    Serial.println(F("ERROR: serial command too long."));
  } else {
    serialBuffer[serialLength] = '\0';
    for (uint8_t i = 0; i < serialLength; ++i) {
      if (serialBuffer[i] >= 'a' && serialBuffer[i] <= 'z') {
        serialBuffer[i] -= ('a' - 'A');
      }
    }

    char *command = strtok(serialBuffer, " \t");
    if (command != NULL) {
      if (strcmp(command, "SET") == 0) {
        long index, target, duration;
        char *a = strtok(NULL, " \t");
        char *b = strtok(NULL, " \t");
        char *c = strtok(NULL, " \t");
        char *extra = strtok(NULL, " \t");

        if (!parseInteger(a, index) || !parseInteger(b, target) ||
            !parseInteger(c, duration) || extra != NULL ||
            index < 1 || index > 3 || target < 1 || target > 150 ||
            duration < 1 || duration > 999) {
          Serial.println(F("ERROR: SET <1..3> <1..150 C> <1..999 s>"));
        } else if (isRunning()) {
          Serial.println(F("ERROR: stop the process before changing settings."));
        } else {
          stages[index - 1].targetC = (uint16_t)target;
          stages[index - 1].durationSeconds = (uint16_t)duration;
          selectedSetting = (uint8_t)((index - 1) * 2);
          state = CONFIGURING;
          displayDirty = true;
          logPrefix();
          Serial.println(F("Settings updated."));
          printSettings();
        }
      } else if (strtok(NULL, " \t") != NULL) {
        Serial.println(F("ERROR: unexpected command arguments."));
      } else if (strcmp(command, "HELP") == 0) {
        printHelp();
      } else if (strcmp(command, "STATUS") == 0) {
        printStatus();
        printSettings();
      } else if (strcmp(command, "START") == 0) {
        startProcess();
      } else if (strcmp(command, "STOP") == 0) {
        stopProcess();
      } else {
        Serial.println(F("ERROR: unknown command; use HELP."));
      }
    }
  }
  serialLength = 0;
  serialOverflow = false;
}

void serviceSerial() {
  while (Serial.available() > 0) {
    const char c = (char)Serial.read();
    lastSerialByteAt = millis();

    if (c == '\r' || c == '\n') {
      if (serialLength > 0 || serialOverflow) executeSerialCommand();
    } else if (serialLength < sizeof(serialBuffer) - 1) {
      serialBuffer[serialLength++] = c;
    } else {
      serialOverflow = true;
    }
  }

  if ((serialLength > 0 || serialOverflow) &&
      (uint32_t)(millis() - lastSerialByteAt) >= SERIAL_IDLE_MS) {
    executeSerialCommand();
  }
}

void setup() {
  pinMode(LM35_PIN, INPUT);
  pinMode(SELECT_PIN, INPUT_PULLUP);
  pinMode(UP_PIN, INPUT_PULLUP);
  pinMode(DOWN_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_PIN, LOW);

  analogReference(DEFAULT);
  Serial.begin(9600);
  display.setBrightness(5, true);
  display.clear();

  const uint32_t now = millis();
  for (uint8_t i = 0; i < 3; ++i) {
    buttons[i].rawPressed = digitalRead(buttons[i].pin) == LOW;
    buttons[i].stablePressed = buttons[i].rawPressed;
    buttons[i].rawChangedAt = now;
    buttons[i].pressedAt = now;
  }

  readTemperature();
  lastTemperatureSample = millis();
  Serial.println(F("\nThree-stage cooking assistant ready."));
  printHelp();
  printSettings();
  printStatus();
  printSelectedSetting();
  updateDisplay(millis());
}

void loop() {
  serviceSerial();

  uint32_t now = millis();
  serviceButtons(now);

  now = millis();
  if ((uint32_t)(now - lastTemperatureSample) >= TEMPERATURE_SAMPLE_MS) {
    lastTemperatureSample = now;
    readTemperature();
  }

  now = millis();
  serviceProcess(now);
  serviceBuzzer(millis());
  updateDisplay(millis());
}
