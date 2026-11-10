#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_019\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

#define VEH_TRIG_PIN 8
#define VEH_ECHO_PIN 9
#define PED_TRIG_PIN 10
#define PED_ECHO_PIN 11

#define LED_RED_PIN 12
#define LED_GREEN_PIN 13
#define BUZZER_PIN 6

#define TM1637_CLK_PIN 4
#define TM1637_DIO_PIN 5

// -------------------- User-tunable behavior --------------------
static const uint16_t DANGER_LEVEL_1_DIST_CM = 80;   // nearer => higher level
static const uint16_t DANGER_LEVEL_2_DIST_CM = 55;
static const uint16_t DANGER_LEVEL_3_DIST_CM = 35;

// Safety distance thresholds for "simultaneously approaching"
static const uint16_t SAFE_PED_DIST_CM = 120;         // pedestrian must be within this to count
static const uint16_t SAFE_VEH_DIST_CM = 120;         // vehicle must be within this to count
static const uint32_t SIMULTANEOUS_WINDOW_MS = 1200;  // both "near" within this window => danger
static const uint32_t MEASUREMENT_INTERVAL_MS = 100;

// Distance sensor timing
static const uint32_t ULTRASONIC_TIMEOUT_US = 30000UL; // ~5m max

// Buzzer behavior
static const uint16_t BUZZER_TONE_HZ = 2000;
static const uint32_t BUZZER_ON_MS = 250;
static const uint32_t BUZZER_OFF_MS = 250;

// Red blink behavior
static const uint32_t RED_BLINK_ON_MS = 250;
static const uint32_t RED_BLINK_OFF_MS = 250;

// -------------------- Globals --------------------
TM1637Display tm1637(TM1637_CLK_PIN, TM1637_DIO_PIN);

static int lastDangerLevel = 0;

static bool dangerActive = false;
static uint32_t dangerStartMs = 0;
static uint32_t lastDangerDurationMs = 0;

static uint32_t pedNearSinceMs = 0;
static uint32_t vehNearSinceMs = 0;

static bool redState = false;
static uint32_t redLastToggleMs = 0;

static bool buzzerStateOn = false;
static uint32_t buzzerLastToggleMs = 0;

static uint32_t lastMeasureMs = 0;

#line 59 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_019\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint32_t nowMs();
#line 63 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_019\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float measureDistanceCm(uint8_t trigPin, uint8_t echoPin);
#line 92 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_019\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool isNearWithin(uint16_t distCmThreshold, float measuredCm);
#line 97 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_019\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static int computeDangerLevel(float pedCm, float vehCm);
#line 111 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_019\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setRedLed(bool on);
#line 115 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_019\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setGreenLed(bool on);
#line 119 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_019\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateIndicatorsForDanger(bool active, int dangerLevel);
#line 182 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_019\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 216 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_019\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 59 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_019\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint32_t nowMs() {
  return millis();
}

static float measureDistanceCm(uint8_t trigPin, uint8_t echoPin) {
  // Trigger pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure echo pulse width with timeout
  uint32_t startMicros = micros();
  // Wait for echo HIGH
  while (digitalRead(echoPin) == LOW) {
    if ((micros() - startMicros) > ULTRASONIC_TIMEOUT_US) return -1.0f;
  }
  uint32_t echoStart = micros();

  // Wait for echo LOW
  while (digitalRead(echoPin) == HIGH) {
    if ((micros() - echoStart) > ULTRASONIC_TIMEOUT_US) return -1.0f;
  }
  uint32_t echoEnd = micros();
  uint32_t pulseWidthUs = echoEnd - echoStart;

  // Convert to distance (cm). Speed of sound ~ 343 m/s.
  // distance = (pulseWidth / 2) * 0.0343 cm/us
  float distanceCm = (pulseWidthUs * 0.0343f) / 2.0f;
  return distanceCm;
}

static bool isNearWithin(uint16_t distCmThreshold, float measuredCm) {
  if (measuredCm < 0) return false;
  return (measuredCm <= (float)distCmThreshold);
}

static int computeDangerLevel(float pedCm, float vehCm) {
  // Use the minimum of the two distances as "severity"
  float d = pedCm;
  if (vehCm >= 0 && (d < 0 || vehCm < d)) d = vehCm;

  if (d < 0) return 1;

  // Higher danger for closer distance
  if (d <= (float)DANGER_LEVEL_3_DIST_CM) return 3;
  if (d <= (float)DANGER_LEVEL_2_DIST_CM) return 2;
  if (d <= (float)DANGER_LEVEL_1_DIST_CM) return 1;
  return 0;
}

static void setRedLed(bool on) {
  digitalWrite(LED_RED_PIN, on ? HIGH : LOW);
}

static void setGreenLed(bool on) {
  digitalWrite(LED_GREEN_PIN, on ? HIGH : LOW);
}

static void updateIndicatorsForDanger(bool active, int dangerLevel) {
  // Red blinking + buzzer alarm + TM1637 display danger level
  if (active) {
    // Display danger level
    // Show "dL" style? We'll show the number as "L" without extra letters.
    // If your TM1637 wiring expects 4-digit segment only, this prints the level number.
    // Level 1..3 => show it; level 0 => show 0.
    tm1637.showNumberDec(dangerLevel, false, 4, 0);

    // Red blink
    if (redLastToggleMs == 0) {
      redLastToggleMs = nowMs();
      redState = false;
      setRedLed(false);
    } else {
      uint32_t t = nowMs();
      uint32_t interval = redState ? RED_BLINK_OFF_MS : RED_BLINK_ON_MS;
      if (t - redLastToggleMs >= interval) {
        redLastToggleMs = t;
        redState = !redState;
        setRedLed(redState);
      }
    }

    // Buzzer alarm (tone toggling)
    if (buzzerLastToggleMs == 0) {
      buzzerLastToggleMs = nowMs();
      buzzerStateOn = false;
      noTone(BUZZER_PIN);
    } else {
      uint32_t t = nowMs();
      uint32_t interval = buzzerStateOn ? BUZZER_OFF_MS : BUZZER_ON_MS;
      if (t - buzzerLastToggleMs >= interval) {
        buzzerLastToggleMs = t;
        buzzerStateOn = !buzzerStateOn;
        if (buzzerStateOn) {
          tone(BUZZER_PIN, BUZZER_TONE_HZ);
        } else {
          noTone(BUZZER_PIN);
        }
      }
    }

    // Green off when danger active
    setGreenLed(false);

  } else {
    // No danger: green steady on, red off, buzzer off, show 0
    tm1637.showNumberDec(0, false, 4, 0);

    setRedLed(false);
    setGreenLed(true);

    noTone(BUZZER_PIN);

    // Reset timing toggles
    redLastToggleMs = 0;
    buzzerLastToggleMs = 0;
    redState = false;
    buzzerStateOn = false;
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(VEH_TRIG_PIN, OUTPUT);
  pinMode(VEH_ECHO_PIN, INPUT);
  pinMode(PED_TRIG_PIN, OUTPUT);
  pinMode(PED_ECHO_PIN, INPUT);

  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(VEH_TRIG_PIN, LOW);
  digitalWrite(PED_TRIG_PIN, LOW);

  setGreenLed(true);
  setRedLed(false);
  noTone(BUZZER_PIN);

  tm1637.setBrightness(0x0f, true);
  tm1637.showNumberDec(0, false, 4, 0);

  // Ensure globals initialized
  pedNearSinceMs = 0;
  vehNearSinceMs = 0;
  dangerActive = false;
  dangerStartMs = 0;
  lastDangerDurationMs = 0;
  lastDangerLevel = 0;
  lastMeasureMs = 0;
  redLastToggleMs = 0;
  buzzerLastToggleMs = 0;
}

void loop() {
  uint32_t t = nowMs();

  if (t - lastMeasureMs >= MEASUREMENT_INTERVAL_MS) {
    lastMeasureMs = t;

    // Measure distances
    float vehCm = measureDistanceCm(VEH_TRIG_PIN, VEH_ECHO_PIN);
    float pedCm = measureDistanceCm(PED_TRIG_PIN, PED_ECHO_PIN);

    bool pedNear = isNearWithin(SAFE_PED_DIST_CM, pedCm);
    bool vehNear = isNearWithin(SAFE_VEH_DIST_CM, vehCm);

    // Update "near since" windows
    if (pedNear) {
      if (pedNearSinceMs == 0) pedNearSinceMs = t;
    } else {
      pedNearSinceMs = 0;
    }

    if (vehNear) {
      if (vehNearSinceMs == 0) vehNearSinceMs = t;
    } else {
      vehNearSinceMs = 0;
    }

    // Determine if both are simultaneously approaching
    bool simultaneous =
      (pedNearSinceMs != 0 && vehNearSinceMs != 0 &&
       ( (pedNearSinceMs > vehNearSinceMs) ? (pedNearSinceMs - vehNearSinceMs) : (vehNearSinceMs - pedNearSinceMs) )
         <= SIMULTANEOUS_WINDOW_MS);

    // If danger becomes active
    if (simultaneous) {
      if (!dangerActive) {
        dangerActive = true;
        dangerStartMs = t;
        redLastToggleMs = 0;
        buzzerLastToggleMs = 0;
        pedNearSinceMs = pedNearSinceMs; // keep
        vehNearSinceMs = vehNearSinceMs; // keep
      }

      int lvl = computeDangerLevel(pedCm, vehCm);
      if (lvl <= 0) lvl = 1; // Ensure "danger" implies at least level 1
      lastDangerLevel = lvl;

      updateIndicatorsForDanger(true, lvl);
    } else {
      // No simultaneous danger
      if (dangerActive) {
        // Danger just ended: compute duration and output last event duration
        lastDangerDurationMs = t - dangerStartMs;
        dangerActive = false;

        // Output the most recent dangerous event duration via Serial
        // Requirement: 串口输出最近一次危险事件持续时间。
        Serial.println(lastDangerDurationMs);
      }

      updateIndicatorsForDanger(false, 0);

      // Keep near windows cleared only by above logic; do nothing else
    }
  }

  // Maintain indicators even without new measurements (handled inside updateIndicatorsForDanger during blink/tone toggles)
  // But if dangerActive persists and measurements are sparse, tone/red toggles depend on the update call.
  // Therefore, call a lightweight indicator update when dangerActive.
  if (dangerActive) {
    // Re-assert danger outputs (including time-based toggles) using lastDangerLevel
    updateIndicatorsForDanger(true, lastDangerLevel);
  }
}
