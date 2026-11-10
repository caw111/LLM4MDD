#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_019\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// -------- Pin definitions --------
// NOTE: On Arduino AVR cores, D8/D9... are not valid identifiers.
// Use numeric pin numbers as wired: D4->4, D5->5, D6->6, D8->8, D9->9, D10->10, D11->11, D12->12, D13->13.

// Vehicle direction (HC-SR04)
static const uint8_t VEH_TRIG = 8;   // D8
static const uint8_t VEH_ECHO = 9;   // D9

// Pedestrian direction (HC-SR04)
static const uint8_t PED_TRIG = 10;  // D10
static const uint8_t PED_ECHO = 11;  // D11

// LEDs
static const uint8_t LED_RED   = 12; // D12
static const uint8_t LED_GREEN = 13; // D13

// Buzzer
static const uint8_t BUZZER_PIN = 6;  // D6

// TM1637
static const uint8_t TM1637_CLK = 4; // D4
static const uint8_t TM1637_DIO = 5; // D5
TM1637Display tm1637(TM1637_CLK, TM1637_DIO);

// -------- Serial --------
static const uint32_t SERIAL_BAUD = 9600;

// -------- Ultrasonic measuring --------
static const uint16_t MAX_DISTANCE_CM = 400;    // safety cap
static const uint16_t SAMPLE_INTERVAL_MS = 50;  // sensor sampling interval
static const uint16_t PING_TIMEOUT_US = 25000;  // ~4m round trip (guard)

// Use threshold(s) to decide "approaching simultaneously".
// Requirement does not specify exact thresholds; keep reasonable defaults.
static const uint16_t DANGER_DISTANCE_CM = 80;   // proximity threshold
static const uint16_t DANGER_LEVEL_1_CM = 80;
static const uint16_t DANGER_LEVEL_2_CM = 60;
static const uint16_t DANGER_LEVEL_3_CM = 40;

// -------- Timing/behavior --------
static const uint16_t RED_BLINK_PERIOD_MS = 500; // red blink speed in danger mode
static const uint16_t RED_ON_MS = 250;            // blink duty
static const uint16_t BUZZER_BEEP_MS = 300;       // beep duration (periodic)

// -------- State --------
static unsigned long lastSampleMs = 0;

static bool lastDangerState = false;
static unsigned long dangerStartMs = 0;
static unsigned long lastDangerDurationMs = 0; // "recent last dangerous event duration"

static unsigned long lastRedToggleMs = 0;
static bool redLedOn = false;

static unsigned long lastBuzzerToggleMs = 0;
static bool buzzerOn = false;

static int dangerLevel = 0; // 0=no danger, 1..3 = risk level

// -------- Helpers --------
#line 64 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_019\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void setRed(bool on);
#line 65 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_019\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void setGreen(bool on);
#line 66 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_019\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void setBuzzer(bool on);
#line 68 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_019\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static uint16_t readUltrasonicCm(uint8_t trigPin, uint8_t echoPin);
#line 94 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_019\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static int computeDangerLevel(uint16_t vehicleCm, uint16_t pedCm);
#line 103 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_019\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void setup();
#line 133 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_019\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void loop();
#line 64 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_019\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void setRed(bool on) { digitalWrite(LED_RED, on ? HIGH : LOW); }
static void setGreen(bool on) { digitalWrite(LED_GREEN, on ? HIGH : LOW); }
static void setBuzzer(bool on) { digitalWrite(BUZZER_PIN, on ? HIGH : LOW); }

static uint16_t readUltrasonicCm(uint8_t trigPin, uint8_t echoPin) {
  // Trigger
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure echo pulse width
  unsigned long start = micros();
  while (digitalRead(echoPin) == LOW) {
    if (micros() - start > PING_TIMEOUT_US) return MAX_DISTANCE_CM;
  }
  start = micros();
  while (digitalRead(echoPin) == HIGH) {
    if (micros() - start > PING_TIMEOUT_US) return MAX_DISTANCE_CM;
  }
  unsigned long durationUs = micros() - start + 0; // pulse width

  // Convert: distance(cm) = duration(us) / 58
  // For better resilience, ignore implausible values.
  unsigned long cm = durationUs / 58UL;
  if (cm > MAX_DISTANCE_CM) cm = MAX_DISTANCE_CM;
  return (uint16_t)cm;
}

static int computeDangerLevel(uint16_t vehicleCm, uint16_t pedCm) {
  // Higher level when either is closer.
  uint16_t closest = min(vehicleCm, pedCm);
  if (closest <= DANGER_LEVEL_3_CM) return 3;
  if (closest <= DANGER_LEVEL_2_CM) return 2;
  if (closest <= DANGER_LEVEL_1_CM) return 1;
  return 0;
}

void setup() {
  // Sensor pins
  pinMode(VEH_TRIG, OUTPUT);
  pinMode(VEH_ECHO, INPUT);
  pinMode(PED_TRIG, OUTPUT);
  pinMode(PED_ECHO, INPUT);

  // LEDs
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);

  // Buzzer
  pinMode(BUZZER_PIN, OUTPUT);

  // Initialize outputs
  digitalWrite(VEH_TRIG, LOW);
  digitalWrite(PED_TRIG, LOW);
  setRed(false);
  setGreen(true);
  setBuzzer(false);

  // TM1637 initialization
  tm1637.setBrightness(0x0f);
  // Default display: 0 (safe)
  tm1637.showNumberDec(0, false, 4);

  // Serial
  Serial.begin(SERIAL_BAUD);
}

void loop() {
  unsigned long now = millis();

  if (now - lastSampleMs >= SAMPLE_INTERVAL_MS) {
    lastSampleMs = now;

    uint16_t vehCm = readUltrasonicCm(VEH_TRIG, VEH_ECHO);
    uint16_t pedCm = readUltrasonicCm(PED_TRIG, PED_ECHO);

    // "When detecting pedestrians and vehicles approaching simultaneously"
    // -> Both distances are within proximity threshold.
    bool vehicleApproaching = (vehCm <= DANGER_DISTANCE_CM);
    bool pedApproaching = (pedCm <= DANGER_DISTANCE_CM);

    bool dangerNow = vehicleApproaching && pedApproaching;
    int newLevel = dangerNow ? computeDangerLevel(vehCm, pedCm) : 0;

    if (dangerNow) {
      // Track danger event duration
      if (!lastDangerState) {
        dangerStartMs = now;
        lastDangerDurationMs = 0;
        // Reset blinking/buzzer timing
        lastRedToggleMs = now;
        redLedOn = false;
        lastBuzzerToggleMs = now;
        buzzerOn = false;
      }

      // Update danger state
      lastDangerState = true;
      dangerLevel = newLevel;

      // Red LED blinking; Green off
      setGreen(false);

      if (redLedOn) {
        if (now - lastRedToggleMs >= RED_ON_MS) {
          redLedOn = false;
          lastRedToggleMs = now;
          setRed(false);
        }
      } else {
        if (now - lastRedToggleMs >= (RED_BLINK_PERIOD_MS - RED_ON_MS)) {
          redLedOn = true;
          lastRedToggleMs = now;
          setRed(true);
        }
      }

      // Buzzer alarm (periodic beeping)
      if (buzzerOn) {
        if (now - lastBuzzerToggleMs >= BUZZER_BEEP_MS / 2) {
          buzzerOn = false;
          lastBuzzerToggleMs = now;
          setBuzzer(false);
        }
      } else {
        if (now - lastBuzzerToggleMs >= BUZZER_BEEP_MS) {
          buzzerOn = true;
          lastBuzzerToggleMs = now;
          setBuzzer(true);
        }
      }

      // Display danger level on TM1637
      // Requirement: "display dangerous grade"; use 1..3, show 0 in safe state.
      tm1637.showNumberDec(dangerLevel, false, 4);
    } else {
      // Safe mode
      if (lastDangerState) {
        // Leaving danger: compute last event duration and output via serial
        lastDangerState = false;
        lastDangerDurationMs = now - dangerStartMs;

        // Output: "最近一次危险事件持续时间"
        // Output as milliseconds, one line.
        Serial.print("LastDangerDurationMs=");
        Serial.println(lastDangerDurationMs);

        // Ensure outputs reset
        setBuzzer(false);
        redLedOn = false;
        setRed(false);
      }

      dangerLevel = 0;

      // Green constant on, Red off, Buzzer off
      setGreen(true);
      setRed(false);
      setBuzzer(false);

      // Display safe level 0
      tm1637.showNumberDec(0, false, 4);
    }
  }

  // Keep doing nothing else; all behavior handled on sampling cadence.
}
