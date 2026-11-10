#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_027\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <Servo.h>
#include <TM1637Display.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Wiring: LDRs A0/A1, voltage A2, button D2 to GND,
// TM1637 CLK=D4/DIO=D5, buzzer D6, servo D9.
// All devices must share GND. Power the servo from a suitable supply.
const uint8_t LEFT_LIGHT_PIN  = A0;
const uint8_t RIGHT_LIGHT_PIN = A1;
const uint8_t VOLTAGE_PIN     = A2;
const uint8_t BUTTON_PIN      = 2;
const uint8_t DISPLAY_CLK_PIN = 4;
const uint8_t DISPLAY_DIO_PIN = 5;
const uint8_t BUZZER_PIN      = 6;
const uint8_t SERVO_PIN       = 9;

// Adjust these values to match the actual voltage-sensing circuit.
// Divider example: 10k from measured voltage to A2, 10k from A2 to GND.
// Never allow A2 to exceed the Arduino's analog reference voltage.
const float ADC_REFERENCE_V = 5.0f;
const float VOLTAGE_DIVIDER_RATIO = 2.0f;
const float VOLTAGE_CALIBRATION = 1.0f;

// Example voltage thresholds; configure for the actual panel/battery.
// Voltage alone estimates charge availability, not actual charge current.
const float LOW_VOLTAGE_V       = 3.50f;
const float LOW_RECOVERY_V      = 3.70f;
const float CHARGE_AVAILABLE_V  = 4.20f;
const float CHARGE_HYSTERESIS_V = 0.10f;
const unsigned long LOW_CHARGE_DELAY_MS = 30000UL;

const bool LDR_HIGHER_MEANS_BRIGHTER = true;
// Reverse this sign if the servo moves away from the brighter side.
const int8_t LEFT_SERVO_DIRECTION = -1;

const int SERVO_MIN_ANGLE = 0;
const int SERVO_MAX_ANGLE = 180;
const int SERVO_START_ANGLE = 90;
const int LIGHT_DEADBAND = 25;
const int SERVO_STEP_DEGREES = 1;

const unsigned long SAMPLE_INTERVAL_MS = 20UL;
const unsigned long TRACK_INTERVAL_MS = 100UL;
const unsigned long DISPLAY_INTERVAL_MS = 200UL;
const unsigned long STATUS_INTERVAL_MS = 2000UL;
const unsigned long BUTTON_DEBOUNCE_MS = 30UL;
const unsigned long CALIBRATION_HOLD_MS = 1500UL;
const unsigned long SERIAL_IDLE_TIMEOUT_MS = 20UL;
const unsigned long ALARM_PERIOD_MS = 1000UL;
const unsigned long ALARM_ON_MS = 200UL;
const unsigned long ALARM_MESSAGE_INTERVAL_MS = 5000UL;

Servo panelServo;
TM1637Display display(DISPLAY_CLK_PIN, DISPLAY_DIO_PIN);

enum OperatingMode { AUTOMATIC_MODE, MANUAL_MODE };
enum ChargeState { LOW_VOLTAGE, VOLTAGE_PRESENT, CHARGE_AVAILABLE };

OperatingMode mode = AUTOMATIC_MODE;
ChargeState chargeState = VOLTAGE_PRESENT;

int servoAngle = SERVO_START_ANGLE;
int lightBalanceOffset = 0;
float filteredLeft = 0.0f;
float filteredRight = 0.0f;
float filteredVoltage = 0.0f;
bool samplesInitialized = false;

bool lowVoltageTracking = false;
bool alarmActive = false;
bool buzzerOn = false;
unsigned long lowVoltageSince = 0;
unsigned long alarmStartedAt = 0;
unsigned long lastAlarmMessage = 0;

bool buttonRaw = HIGH;
bool buttonStable = HIGH;
bool buttonLongHandled = false;
unsigned long buttonChangedAt = 0;
unsigned long buttonPressedAt = 0;

unsigned long lastSample = 0;
unsigned long lastTrack = 0;
unsigned long lastDisplay = 0;
unsigned long lastStatus = 0;

char serialBuffer[64];
uint8_t serialLength = 0;
bool serialOverflow = false;
unsigned long lastSerialByteAt = 0;

#line 95 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_027\\direct\\attempt_0\\sketch\\sketch.ino"
void setServoAngle(int angle);
#line 100 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_027\\direct\\attempt_0\\sketch\\sketch.ino"
int lightError();
#line 108 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_027\\direct\\attempt_0\\sketch\\sketch.ino"
void printMode();
#line 113 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_027\\direct\\attempt_0\\sketch\\sketch.ino"
void setMode(OperatingMode newMode);
#line 123 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_027\\direct\\attempt_0\\sketch\\sketch.ino"
void calibrateLightBalance();
#line 139 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_027\\direct\\attempt_0\\sketch\\sketch.ino"
void printStatus();
#line 170 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_027\\direct\\attempt_0\\sketch\\sketch.ino"
void printHelp();
#line 179 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_027\\direct\\attempt_0\\sketch\\sketch.ino"
void processCommand(char *command);
#line 243 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_027\\direct\\attempt_0\\sketch\\sketch.ino"
void finishSerialCommand();
#line 254 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_027\\direct\\attempt_0\\sketch\\sketch.ino"
void updateSerial();
#line 278 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_027\\direct\\attempt_0\\sketch\\sketch.ino"
void updateButton(unsigned long now);
#line 306 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_027\\direct\\attempt_0\\sketch\\sketch.ino"
int readAnalogSettled(uint8_t pin);
#line 312 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_027\\direct\\attempt_0\\sketch\\sketch.ino"
void updateSensors(unsigned long now);
#line 338 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_027\\direct\\attempt_0\\sketch\\sketch.ino"
void updateTracking(unsigned long now);
#line 353 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_027\\direct\\attempt_0\\sketch\\sketch.ino"
void updateChargeAndAlarm(unsigned long now);
#line 407 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_027\\direct\\attempt_0\\sketch\\sketch.ino"
void updateDisplay(unsigned long now);
#line 425 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_027\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 453 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_027\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 95 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_027\\direct\\attempt_0\\sketch\\sketch.ino"
void setServoAngle(int angle) {
  servoAngle = constrain(angle, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE);
  panelServo.write(servoAngle);
}

int lightError() {
  int difference = (int)(filteredLeft - filteredRight);
  if (!LDR_HIGHER_MEANS_BRIGHTER) {
    difference = -difference;
  }
  return difference - lightBalanceOffset;
}

void printMode() {
  Serial.print(F("mode="));
  Serial.println(mode == AUTOMATIC_MODE ? F("auto") : F("manual"));
}

void setMode(OperatingMode newMode) {
  mode = newMode;
  printMode();
  if (mode == MANUAL_MODE) {
    Serial.println(F("Manual: angle N, left, right, center, calibrate"));
    Serial.println(F("Equalize LDR illumination before calibrating."));
    Serial.println(F("Hold button 1.5s to calibrate; tap to return to auto."));
  }
}

void calibrateLightBalance() {
  if (mode != MANUAL_MODE || !samplesInitialized) {
    Serial.println(F("Calibration requires manual mode."));
    return;
  }

  lightBalanceOffset = (int)(filteredLeft - filteredRight);
  if (!LDR_HIGHER_MEANS_BRIGHTER) {
    lightBalanceOffset = -lightBalanceOffset;
  }

  Serial.print(F("Light balance calibrated; offset="));
  Serial.println(lightBalanceOffset);
  Serial.println(F("Calibration is retained until reset."));
}

void printStatus() {
  Serial.print(F("mode="));
  Serial.print(mode == AUTOMATIC_MODE ? F("auto") : F("manual"));
  Serial.print(F(" angle="));
  Serial.print(servoAngle);
  Serial.print(F(" left="));
  Serial.print((int)filteredLeft);
  Serial.print(F(" right="));
  Serial.print((int)filteredRight);
  Serial.print(F(" error="));
  Serial.print(lightError());
  Serial.print(F(" voltage="));
  Serial.print(filteredVoltage, 2);
  Serial.print(F("V state="));

  switch (chargeState) {
    case LOW_VOLTAGE:
      Serial.print(F("low"));
      break;
    case CHARGE_AVAILABLE:
      Serial.print(F("charge_available"));
      break;
    default:
      Serial.print(F("voltage_present"));
      break;
  }

  Serial.print(F(" alarm="));
  Serial.println(alarmActive ? F("on") : F("off"));
}

void printHelp() {
  Serial.println(F("Commands: auto, manual, toggle, angle 0..180,"));
  Serial.println(F("left, right, center, calibrate, status, help"));
  Serial.println(F("Manual commands hold the selected servo position."));
  Serial.println(F("Button: tap toggles mode; hold in manual calibrates LDRs."));
  Serial.println(F("Display: voltage x100; middle separator if supported."));
  Serial.println(F("Charge status is voltage-based, not current confirmation."));
}

void processCommand(char *command) {
  while (*command && isspace((unsigned char)*command)) {
    ++command;
  }

  size_t length = strlen(command);
  while (length && isspace((unsigned char)command[length - 1])) {
    command[--length] = '\0';
  }

  for (size_t i = 0; i < length; ++i) {
    command[i] = (char)tolower((unsigned char)command[i]);
  }

  if (!length) return;

  if (strcmp(command, "auto") == 0) {
    setMode(AUTOMATIC_MODE);
  } else if (strcmp(command, "manual") == 0) {
    setMode(MANUAL_MODE);
  } else if (strcmp(command, "toggle") == 0) {
    setMode(mode == AUTOMATIC_MODE ? MANUAL_MODE : AUTOMATIC_MODE);
  } else if (strcmp(command, "status") == 0) {
    printStatus();
  } else if (strcmp(command, "help") == 0) {
    printHelp();
  } else if (strcmp(command, "calibrate") == 0) {
    calibrateLightBalance();
  } else if (strncmp(command, "angle ", 6) == 0) {
    if (mode != MANUAL_MODE) {
      Serial.println(F("Enter manual mode first."));
      return;
    }

    char *end;
    long angle = strtol(command + 6, &end, 10);
    if (end == command + 6 || *end != '\0' ||
        angle < SERVO_MIN_ANGLE || angle > SERVO_MAX_ANGLE) {
      Serial.println(F("Invalid angle; use angle 0..180."));
      return;
    }
    setServoAngle((int)angle);
    printStatus();
  } else if (strcmp(command, "left") == 0 ||
             strcmp(command, "right") == 0 ||
             strcmp(command, "center") == 0) {
    if (mode != MANUAL_MODE) {
      Serial.println(F("Enter manual mode first."));
      return;
    }

    if (strcmp(command, "center") == 0) {
      setServoAngle(SERVO_START_ANGLE);
    } else {
      int direction = strcmp(command, "left") == 0
                        ? LEFT_SERVO_DIRECTION : -LEFT_SERVO_DIRECTION;
      setServoAngle(servoAngle + direction * 5);
    }
    printStatus();
  } else {
    Serial.println(F("Unknown command; send help."));
  }
}

void finishSerialCommand() {
  if (serialOverflow) {
    Serial.println(F("Command too long; discarded."));
  } else if (serialLength) {
    serialBuffer[serialLength] = '\0';
    processCommand(serialBuffer);
  }
  serialLength = 0;
  serialOverflow = false;
}

void updateSerial() {
  // Limit work per pass so continuous serial input cannot starve control.
  uint8_t budget = 32;
  while (Serial.available() > 0 && budget--) {
    char c = (char)Serial.read();
    lastSerialByteAt = millis();

    if (c == '\r' || c == '\n') {
      finishSerialCommand();
    } else if (!serialOverflow) {
      if (serialLength < sizeof(serialBuffer) - 1) {
        serialBuffer[serialLength++] = c;
      } else {
        serialOverflow = true;
      }
    }
  }

  if ((serialLength || serialOverflow) && Serial.available() == 0 &&
      (unsigned long)(millis() - lastSerialByteAt) >= SERIAL_IDLE_TIMEOUT_MS) {
    finishSerialCommand();
  }
}

void updateButton(unsigned long now) {
  bool raw = digitalRead(BUTTON_PIN);

  if (raw != buttonRaw) {
    buttonRaw = raw;
    buttonChangedAt = now;
  }

  if (buttonStable != buttonRaw &&
      (unsigned long)(now - buttonChangedAt) >= BUTTON_DEBOUNCE_MS) {
    buttonStable = buttonRaw;

    if (buttonStable == LOW) {
      buttonPressedAt = now;
      buttonLongHandled = false;
    } else if (!buttonLongHandled) {
      setMode(mode == AUTOMATIC_MODE ? MANUAL_MODE : AUTOMATIC_MODE);
    }
  }

  if (buttonStable == LOW && buttonRaw == LOW &&
      !buttonLongHandled && mode == MANUAL_MODE &&
      (unsigned long)(now - buttonPressedAt) >= CALIBRATION_HOLD_MS) {
    calibrateLightBalance();
    buttonLongHandled = true;
  }
}

int readAnalogSettled(uint8_t pin) {
  // Discard the first conversion after switching ADC channels.
  analogRead(pin);
  return analogRead(pin);
}

void updateSensors(unsigned long now) {
  if (samplesInitialized &&
      (unsigned long)(now - lastSample) < SAMPLE_INTERVAL_MS) {
    return;
  }
  lastSample = now;

  float left = readAnalogSettled(LEFT_LIGHT_PIN);
  float right = readAnalogSettled(RIGHT_LIGHT_PIN);
  float voltage = readAnalogSettled(VOLTAGE_PIN) *
                  (ADC_REFERENCE_V / 1023.0f) *
                  VOLTAGE_DIVIDER_RATIO * VOLTAGE_CALIBRATION;

  if (!samplesInitialized) {
    filteredLeft = left;
    filteredRight = right;
    filteredVoltage = voltage;
    samplesInitialized = true;
  } else {
    const float alpha = 0.15f;
    filteredLeft += alpha * (left - filteredLeft);
    filteredRight += alpha * (right - filteredRight);
    filteredVoltage += alpha * (voltage - filteredVoltage);
  }
}

void updateTracking(unsigned long now) {
  if (!samplesInitialized || mode != AUTOMATIC_MODE ||
      (unsigned long)(now - lastTrack) < TRACK_INTERVAL_MS) {
    return;
  }
  lastTrack = now;

  int error = lightError();
  if (error > LIGHT_DEADBAND) {
    setServoAngle(servoAngle + LEFT_SERVO_DIRECTION * SERVO_STEP_DEGREES);
  } else if (error < -LIGHT_DEADBAND) {
    setServoAngle(servoAngle - LEFT_SERVO_DIRECTION * SERVO_STEP_DEGREES);
  }
}

void updateChargeAndAlarm(unsigned long now) {
  if (!samplesInitialized) return;

  // Hysteresis prevents noisy voltage near the threshold from resetting
  // the sustained-low timer or repeatedly toggling the alarm.
  if (!lowVoltageTracking && filteredVoltage < LOW_VOLTAGE_V) {
    lowVoltageTracking = true;
    lowVoltageSince = now;
  } else if (lowVoltageTracking && filteredVoltage >= LOW_RECOVERY_V) {
    lowVoltageTracking = false;
  }

  if (lowVoltageTracking) {
    chargeState = LOW_VOLTAGE;
  } else if (filteredVoltage >= CHARGE_AVAILABLE_V) {
    chargeState = CHARGE_AVAILABLE;
  } else if (chargeState != CHARGE_AVAILABLE ||
             filteredVoltage < CHARGE_AVAILABLE_V - CHARGE_HYSTERESIS_V) {
    chargeState = VOLTAGE_PRESENT;
  }

  bool shouldAlarm = lowVoltageTracking &&
      (unsigned long)(now - lowVoltageSince) >= LOW_CHARGE_DELAY_MS;

  if (shouldAlarm && !alarmActive) {
    alarmActive = true;
    alarmStartedAt = now;
    lastAlarmMessage = now;
    Serial.println(F("low charge"));
  } else if (!shouldAlarm && alarmActive) {
    alarmActive = false;
    Serial.println(F("charge voltage recovered"));
  }

  if (alarmActive &&
      (unsigned long)(now - lastAlarmMessage) >= ALARM_MESSAGE_INTERVAL_MS) {
    lastAlarmMessage = now;
    Serial.println(F("low charge"));
  }

  bool shouldSound = alarmActive &&
      ((unsigned long)(now - alarmStartedAt) % ALARM_PERIOD_MS < ALARM_ON_MS);

  if (shouldSound != buzzerOn) {
    buzzerOn = shouldSound;
    if (buzzerOn) {
      tone(BUZZER_PIN, 2200);
    } else {
      noTone(BUZZER_PIN);
      digitalWrite(BUZZER_PIN, LOW);
    }
  }
}

void updateDisplay(unsigned long now) {
  if (!samplesInitialized ||
      (unsigned long)(now - lastDisplay) < DISPLAY_INTERVAL_MS) {
    return;
  }
  lastDisplay = now;

  // Four digits show centivolts: 0420 means 4.20 V.
  // On common clock-style TM1637 modules this appears as 04:20.
  long centivolts = (long)(filteredVoltage * 100.0f + 0.5f);
  if (centivolts < 0 || centivolts > 9999) {
    const uint8_t dashes[] = {0x40, 0x40, 0x40, 0x40};
    display.setSegments(dashes);
  } else {
    display.showNumberDecEx((int)centivolts, 0x40, true);
  }
}

void setup() {
  pinMode(LEFT_LIGHT_PIN, INPUT);
  pinMode(RIGHT_LIGHT_PIN, INPUT);
  pinMode(VOLTAGE_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  analogReference(DEFAULT);
  Serial.begin(9600);

  panelServo.attach(SERVO_PIN);
  setServoAngle(SERVO_START_ANGLE);

  display.setBrightness(5, true);
  display.clear();

  buttonRaw = digitalRead(BUTTON_PIN);
  buttonStable = HIGH;
  buttonChangedAt = millis();

  Serial.println(F("Solar tracker and charge-voltage monitor"));
  printHelp();
  printMode();

  updateSensors(millis());
}

void loop() {
  updateSerial();

  unsigned long now = millis();
  updateButton(now);
  updateSensors(now);
  updateTracking(now);
  updateChargeAndAlarm(now);
  updateDisplay(now);

  if ((unsigned long)(now - lastStatus) >= STATUS_INTERVAL_MS) {
    lastStatus = now;
    printStatus();
  }
}
