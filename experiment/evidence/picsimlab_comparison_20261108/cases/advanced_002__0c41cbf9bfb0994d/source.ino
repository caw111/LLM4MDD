#include <Arduino.h>
#include <TM1637Display.h>

// Arduino Uno 接线（按钮接 D2 与 GND，使用内部上拉）。
const uint8_t ENTRY_TRIG_PIN = 8;
const uint8_t ENTRY_ECHO_PIN = 9;
const uint8_t BAY_TRIG_PIN   = 11;
const uint8_t BAY_ECHO_PIN   = 12;
const uint8_t DISPLAY_CLK_PIN = 4;
const uint8_t DISPLAY_DIO_PIN = 5;
const uint8_t RED_LED_PIN   = 13;
const uint8_t GREEN_LED_PIN = 10;
const uint8_t BUZZER_PIN    = 6;
const uint8_t BUTTON_PIN    = 2;

TM1637Display display(DISPLAY_CLK_PIN, DISPLAY_DIO_PIN);

// 所有距离阈值以毫米表示。
const uint16_t ENTRY_GUIDE_MM = 1000;
const uint16_t OCCUPIED_ON_MM = 1000;
const uint16_t OCCUPIED_OFF_MM = 1200;
const uint16_t WARNING_MM = 500;
const uint16_t CLOSE_MM = 200;
const uint16_t VERY_CLOSE_MM = 100;
const uint16_t EMERGENCY_ON_MM = 50;
const uint16_t EMERGENCY_OFF_MM = 60;

const unsigned long ECHO_TIMEOUT_US = 25000UL;
const unsigned long SENSOR_INTERVAL_MS = 60UL; // 交替触发，避免串扰
const unsigned long SENSOR_STALE_MS = 600UL;
const unsigned long OCCUPY_CONFIRM_MS = 500UL;
const unsigned long RELEASE_CONFIRM_MS = 1500UL;
const unsigned long NO_ECHO_RELEASE_MS = 3000UL;
const unsigned long DEBOUNCE_MS = 35UL;
const unsigned long DISPLAY_INTERVAL_MS = 100UL;

struct DistanceSensor {
  uint8_t trigPin;
  uint8_t echoPin;
  uint16_t mm;
  bool valid;
  bool hasValidReading;
  unsigned long lastValidMs;
};

DistanceSensor entrySensor = {
  ENTRY_TRIG_PIN, ENTRY_ECHO_PIN, 0, false, false, 0
};
DistanceSensor baySensor = {
  BAY_TRIG_PIN, BAY_ECHO_PIN, 0, false, false, 0
};

bool occupied = false;
bool emergencyAlarm = false;
bool occupancyCandidate = false;
bool occupancyCandidateActive = false;
unsigned long occupancyCandidateSince = 0;

// 计数为累计确认占用次数；上电时检测到已停车也计一次。
// 不写 EEPROM，避免频繁写入；断电后从零重新计数。
uint32_t occupancyCount = 0;

// 0：车位距离及状态；1：入口距离及状态；2：累计占用次数。
uint8_t displayMode = 0;

bool buttonRaw = HIGH;
bool buttonStable = HIGH;
unsigned long buttonChangedMs = 0;
unsigned long lastSensorMs = 0;
unsigned long lastDisplayMs = 0;
bool readEntryNext = true;
bool forceDisplayUpdate = true;
uint16_t activeToneHz = 0;

bool isFresh(const DistanceSensor &sensor, unsigned long now) {
  return sensor.valid && sensor.hasValidReading &&
         (unsigned long)(now - sensor.lastValidMs) <= SENSOR_STALE_MS;
}

void readDistance(DistanceSensor &sensor) {
  digitalWrite(sensor.trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(sensor.trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(sensor.trigPin, LOW);

  const unsigned long duration =
      pulseIn(sensor.echoPin, HIGH, ECHO_TIMEOUT_US);

  // HC-SR04 在约 2 cm 以下无法可靠测距，极短回波不误作有效距离。
  const unsigned long distanceMm = (duration * 343UL) / 2000UL;
  if (duration != 0 && distanceMm >= 20UL && distanceMm <= 4000UL) {
    sensor.mm = (uint16_t)distanceMm;
    sensor.valid = true;
    sensor.hasValidReading = true;
    sensor.lastValidMs = millis();
  } else {
    sensor.valid = false;
  }
}

void serviceSensors(unsigned long now) {
  if ((unsigned long)(now - lastSensorMs) < SENSOR_INTERVAL_MS) {
    return;
  }
  lastSensorMs = now;

  if (readEntryNext) {
    readDistance(entrySensor);
  } else {
    readDistance(baySensor);
    if (baySensor.valid) {
      if (baySensor.mm < EMERGENCY_ON_MM) {
        emergencyAlarm = true;
      } else if (baySensor.mm >= EMERGENCY_OFF_MM) {
        emergencyAlarm = false;
      }
    }
    // 紧急报警后若回波丢失，继续报警，直到有效安全距离解除。
  }
  readEntryNext = !readEntryNext;
}

void serviceButton(unsigned long now) {
  const bool reading = digitalRead(BUTTON_PIN);
  if (reading != buttonRaw) {
    buttonRaw = reading;
    buttonChangedMs = now;
  }

  if (reading != buttonStable &&
      (unsigned long)(now - buttonChangedMs) >= DEBOUNCE_MS) {
    buttonStable = reading;
    if (buttonStable == LOW) {
      displayMode = (displayMode + 1) % 3;
      forceDisplayUpdate = true;
    }
  }
}

void serviceOccupancy(unsigned long now) {
  bool target = occupied;
  bool evidenceAvailable = false;

  if (emergencyAlarm) {
    target = true;
    evidenceAvailable = true;
  } else if (isFresh(baySensor, now)) {
    evidenceAvailable = true;
    if (!occupied && baySensor.mm <= OCCUPIED_ON_MM) {
      target = true;
    } else if (occupied && baySensor.mm >= OCCUPIED_OFF_MM) {
      target = false;
    }
  } else if (baySensor.hasValidReading &&
             (unsigned long)(now - baySensor.lastValidMs) >=
                 NO_ECHO_RELEASE_MS) {
    // 长时间无回波视为空车位；紧急报警锁存时禁止据此解除。
    target = false;
    evidenceAvailable = true;
  }

  if (!evidenceAvailable || target == occupied) {
    occupancyCandidateActive = false;
    return;
  }

  if (!occupancyCandidateActive || occupancyCandidate != target) {
    occupancyCandidate = target;
    occupancyCandidateSince = now;
    occupancyCandidateActive = true;
  }

  const unsigned long confirmation =
      target ? OCCUPY_CONFIRM_MS : RELEASE_CONFIRM_MS;

  if ((unsigned long)(now - occupancyCandidateSince) >= confirmation) {
    occupied = target;
    occupancyCandidateActive = false;
    if (occupied && occupancyCount < 0xFFFFFFFFUL) {
      ++occupancyCount;
    }
    forceDisplayUpdate = true;
  }
}

void setBuzzer(uint16_t frequencyHz) {
  if (frequencyHz == activeToneHz) {
    return;
  }

  activeToneHz = frequencyHz;
  if (frequencyHz == 0) {
    noTone(BUZZER_PIN);
    digitalWrite(BUZZER_PIN, LOW);
  } else {
    tone(BUZZER_PIN, frequencyHz);
  }
}

void serviceIndicators(unsigned long now) {
  bool red = occupied;
  bool green = false;
  uint16_t frequencyHz = 0;

  const bool entryNear =
      isFresh(entrySensor, now) && entrySensor.mm <= ENTRY_GUIDE_MM;
  const bool bayNear =
      isFresh(baySensor, now) && baySensor.mm <= OCCUPIED_ON_MM;

  if (emergencyAlarm) {
    red = true;
    green = false;
    frequencyHz = 3000; // 小于 5 cm：连续声音及常亮红灯
  } else if (bayNear) {
    const uint16_t distance = baySensor.mm;

    if (distance <= VERY_CLOSE_MM) {
      const bool pulse = (now % 180UL) < 120UL;
      red = pulse;
      green = false;
      frequencyHz = pulse ? 2600 : 0;
    } else if (distance <= CLOSE_MM) {
      const bool pulse = (now % 350UL) < 140UL;
      red = pulse;
      green = false;
      frequencyHz = pulse ? 2200 : 0;
    } else if (distance <= WARNING_MM) {
      const bool pulse = (now % 700UL) < 150UL;
      red = pulse;
      green = !pulse;
      frequencyHz = pulse ? 1700 : 0;
    } else {
      const bool pulse = (now % 1200UL) < 120UL;
      red = false;
      green = true;
      frequencyHz = pulse ? 1200 : 0;
    }
  } else if (entryNear && !occupied) {
    red = false;
    green = true; // 空车位且入口有车，绿灯引导
  }

  digitalWrite(RED_LED_PIN, red ? HIGH : LOW);
  digitalWrite(GREEN_LED_PIN, green ? HIGH : LOW);
  setBuzzer(frequencyHz);
}

void encodeThreeDigits(uint16_t number, uint8_t *segments) {
  segments[0] = number >= 100
                    ? display.encodeDigit((number / 100) % 10) : 0;
  segments[1] = number >= 10
                    ? display.encodeDigit((number / 10) % 10) : 0;
  segments[2] = display.encodeDigit(number % 10);
}

void serviceDisplay(unsigned long now) {
  if (!forceDisplayUpdate &&
      (unsigned long)(now - lastDisplayMs) < DISPLAY_INTERVAL_MS) {
    return;
  }
  lastDisplayMs = now;
  forceDisplayUpdate = false;

  const uint8_t CHAR_E = SEG_A | SEG_D | SEG_E | SEG_F | SEG_G;
  const uint8_t CHAR_P = SEG_A | SEG_B | SEG_E | SEG_F | SEG_G;
  const uint8_t CHAR_F = SEG_A | SEG_E | SEG_F | SEG_G;
  const uint8_t CHAR_C = SEG_A | SEG_D | SEG_E | SEG_F;

  uint8_t segments[4] = {0, 0, 0, 0};

  if (displayMode == 2) {
    segments[0] = CHAR_C;
    // 四位屏只能显示三位计数，超过 999 时饱和显示，不回绕。
    const uint16_t count =
        occupancyCount > 999UL ? 999 : (uint16_t)occupancyCount;
    encodeThreeDigits(count, &segments[1]);
  } else {
    const DistanceSensor &sensor =
        displayMode == 1 ? entrySensor : baySensor;

    // E=入口；P=车位占用，F=车位空闲；其后三位为厘米。
    segments[0] = displayMode == 1
                      ? CHAR_E
                      : ((occupied || emergencyAlarm) ? CHAR_P : CHAR_F);

    if (isFresh(sensor, now)) {
      const uint16_t centimeters = (sensor.mm + 5U) / 10U;
      encodeThreeDigits(centimeters, &segments[1]);
    } else {
      segments[1] = SEG_G;
      segments[2] = SEG_G;
      segments[3] = SEG_G; // 无有效回波显示 "---"，不显示虚假零距离
    }
  }

  display.setSegments(segments);
}

void setup() {
  pinMode(ENTRY_TRIG_PIN, OUTPUT);
  pinMode(ENTRY_ECHO_PIN, INPUT);
  pinMode(BAY_TRIG_PIN, OUTPUT);
  pinMode(BAY_ECHO_PIN, INPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  digitalWrite(ENTRY_TRIG_PIN, LOW);
  digitalWrite(BAY_TRIG_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  display.setBrightness(5, true);
  display.clear();

  buttonRaw = digitalRead(BUTTON_PIN);
  buttonStable = buttonRaw;
  buttonChangedMs = millis();
  lastSensorMs = millis() - SENSOR_INTERVAL_MS;
  serviceDisplay(millis());
}

void loop() {
  serviceButton(millis());
  serviceSensors(millis());

  const unsigned long now = millis();
  serviceButton(now);
  serviceOccupancy(now);
  serviceIndicators(now);
  serviceDisplay(now);
}