#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_011\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const uint8_t PIN_LED = 13;
const uint8_t PIN_BUZZER = 6;

const uint8_t PIN_TRIG = 8;
const uint8_t PIN_ECHO = 9;

const uint8_t PIN_LDR = A0;

const unsigned long SERIAL_IDLE_TIMEOUT_MS = 20;

// Alarm behavior tuning (kept explicit and deterministic)
const unsigned long LED_BLINK_INTERVAL_MS = 200; // LED flash interval while alarm active
const unsigned long BUZZ_ON_INTERVAL_MS   = 300; // buzzer ON duration interval while alarm active
const unsigned long BUZZ_OFF_INTERVAL_MS  = 300; // buzzer OFF duration interval while alarm active

// Detection thresholds
const int LDR_DARK_THRESHOLD = 500;   // "environment较暗" threshold for LDR analog value (A0)
const float OBSTACLE_DISTANCE_CM = 20.0f; // trigger if object within 20cm

// Ultrasonic measurement timing
const unsigned long ULTRASONIC_PING_INTERVAL_MS = 60; // measurement cadence
const unsigned long ULTRASONIC_ECHO_TIMEOUT_US  = 30000; // ~30ms => max ~5m

static bool alarmActive = false;
static unsigned long lastPingMs = 0;

static unsigned long lastLedToggleMs = 0;
static bool ledState = false;

static unsigned long alarmCycleMs = 0;

static unsigned long lastSerialActivityMs = 0;

#line 36 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_011\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float readUltrasonicCm();
#line 56 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_011\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setAlarmOutputs(bool active);
#line 86 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_011\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void maybePrintAlarm();
#line 103 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_011\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 118 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_011\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 36 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_011\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float readUltrasonicCm() {
  // Trigger
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  // Echo
  unsigned long duration = pulseIn(PIN_ECHO, HIGH, ULTRASONIC_ECHO_TIMEOUT_US);
  if (duration == 0) {
    // No echo within timeout -> treat as far away
    return 9999.0f;
  }

  // Speed of sound: 343 m/s => 0.0343 cm/us
  // distance (cm) = duration(us) * 0.0343 / 2
  return (duration * 0.0343f) / 2.0f;
}

static void setAlarmOutputs(bool active) {
  if (!active) {
    digitalWrite(PIN_LED, LOW);
    digitalWrite(PIN_BUZZER, LOW);
    return;
  }

  // LED blink
  unsigned long now = millis();
  if (now - lastLedToggleMs >= LED_BLINK_INTERVAL_MS) {
    lastLedToggleMs = now;
    ledState = !ledState;
    digitalWrite(PIN_LED, ledState ? HIGH : LOW);
  }

  // Buzzer intermittent鸣叫
  // Cycle: BUZZ_ON_INTERVAL_MS on, BUZZ_OFF_INTERVAL_MS off
  unsigned long cycleLen = BUZZ_ON_INTERVAL_MS + BUZZ_OFF_INTERVAL_MS;
  if (cycleLen == 0) cycleLen = 1;

  unsigned long t = now - alarmCycleMs;
  unsigned long mod = t % cycleLen;

  if (mod < BUZZ_ON_INTERVAL_MS) {
    digitalWrite(PIN_BUZZER, HIGH);
  } else {
    digitalWrite(PIN_BUZZER, LOW);
  }
}

static void maybePrintAlarm() {
  // Output alarm over serial when alarm condition becomes active or persists.
  // Use line framing with idle timeout concept: print a line, then avoid spamming too fast.
  // We'll print at a moderate rate while alarm active.
  static unsigned long lastPrintMs = 0;
  unsigned long now = millis();
  if (!alarmActive) return;

  // Print roughly every 250ms to ensure visibility without excessive spam.
  if (now - lastPrintMs >= 250) {
    lastPrintMs = now;
    Serial.println(F("alarm"));
  }

  lastSerialActivityMs = now;
}

void setup() {
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  digitalWrite(PIN_LED, LOW);
  digitalWrite(PIN_BUZZER, LOW);
  digitalWrite(PIN_TRIG, LOW);

  Serial.begin(9600);
  lastSerialActivityMs = millis();
}

void loop() {
  // Read sensors continuously
  int ldrValue = analogRead(PIN_LDR);

  bool isDark = (ldrValue < LDR_DARK_THRESHOLD);

  float distanceCm = 9999.0f;
  unsigned long nowMs = millis();

  if (nowMs - lastPingMs >= ULTRASONIC_PING_INTERVAL_MS) {
    lastPingMs = nowMs;
    distanceCm = readUltrasonicCm();
  } else {
    // keep previous measurement cadence, distanceCm remains last computed by lastPingMs
    // For simplicity, use a last known value by recomputing quickly when not ready.
    // But requirement doesn't force exact cadence; we'll recompute only on cadence.
    // We'll keep the old distanceCm by storing it statically:
  }

  static float lastDistanceCm = 9999.0f;
  if (nowMs - lastPingMs <= ULTRASONIC_PING_INTERVAL_MS) {
    // On ping times, lastPingMs was updated and distanceCm is fresh
    lastDistanceCm = distanceCm;
  }
  distanceCm = lastDistanceCm;

  bool objectNear = (distanceCm <= OBSTACLE_DISTANCE_CM);
  bool shouldAlarm = (isDark && objectNear);

  if (shouldAlarm && !alarmActive) {
    alarmActive = true;
    // Initialize alarm timing so blink and buzzer start immediately
    lastLedToggleMs = nowMs;
    ledState = false;
    digitalWrite(PIN_LED, LOW);

    alarmCycleMs = nowMs;
    digitalWrite(PIN_BUZZER, HIGH); // start with buzzer ON at activation
    // Print immediate alarm line
    Serial.println(F("alarm"));
    lastSerialActivityMs = nowMs;
  } else if (!shouldAlarm && alarmActive) {
    alarmActive = false;
    // Stop outputs
    digitalWrite(PIN_LED, LOW);
    digitalWrite(PIN_BUZZER, LOW);
  }

  if (alarmActive) {
    setAlarmOutputs(true);
    maybePrintAlarm();
  } else {
    setAlarmOutputs(false);
  }

  // Respect serial idle timeout framing concept:
  // if no new serial activity within idle timeout, do nothing special.
  // (We only transmit; keeping placeholder to preserve requirement intent.)
  if (Serial.available() > 0) {
    lastSerialActivityMs = millis();
    // Drain incoming data to prevent buffer overflow; no command handling required.
    while (Serial.available() > 0) Serial.read();
  } else {
    // No RX activity; do nothing.
    (void)SERIAL_IDLE_TIMEOUT_MS;
  }
}
