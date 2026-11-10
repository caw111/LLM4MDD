#include <Arduino.h>

//
// Pin assignments (UNO):
// D8  = HC-SR04 Trig
// D9  = HC-SR04 Echo
// D13 = LED
// D6  = Buzzer (tone)
//
static const uint8_t TRIG_PIN   = 8;
static const uint8_t ECHO_PIN   = 9;
static const uint8_t LED_PIN    = 13;
static const uint8_t BUZZER_PIN = 6;

static const uint32_t SERIAL_BAUD = 9600;
static const uint16_t IDLE_TIMEOUT_MS = 20; // preserved (not used)

static const uint16_t NEAR_DISTANCE_CM = 10;   // within 10cm
static const uint32_t ALERT_RECOVER_MS = 5000; // recover after 5s without near

// Timing/behavior (preserved)
static const uint8_t  BEEP_COUNT     = 3;
static const uint16_t BEEP_ON_MS     = 150;  // short鸣一次时长
static const uint16_t BEEP_OFF_MS    = 120;  // 间隔
static const uint16_t LED_FAST_ON_MS  = 80;
static const uint16_t LED_FAST_OFF_MS = 80;

// Ultrasonic measurement (preserved)
static const uint16_t MAX_ECHO_WAIT_US = 30000; // ~5m max (approx)
static const uint8_t  SOUND_SPEED_CM_PER_US = 0; // unused placeholder (kept)

static inline long readUltrasonicCM() {
  // Trigger a 10us pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Measure echo pulse width
  unsigned long duration = pulseIn(ECHO_PIN, HIGH, MAX_ECHO_WAIT_US);
  if (duration == 0) return -1; // no reading

  // Convert time to distance (cm):
  // distance ≈ duration(us) / 58
  long cm = (long)(duration / 58UL);
  return cm;
}

static inline void buzzShortBeepNTimes(uint8_t n) {
  // Typical frequency (kept as in previous sketch)
  const uint16_t freq = 2000;

  for (uint8_t i = 0; i < n; i++) {
    tone(BUZZER_PIN, freq);
    delay(BEEP_ON_MS);
    noTone(BUZZER_PIN);
    if (i + 1 < n) delay(BEEP_OFF_MS);
  }
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);
  pinMode(ECHO_PIN, INPUT);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  pinMode(BUZZER_PIN, OUTPUT);
  noTone(BUZZER_PIN);

  Serial.begin(SERIAL_BAUD);
}

void loop() {
  static bool alerted = false;
  static uint32_t lastNearMillis = 0;

  long cm = readUltrasonicCM();
  bool near = (cm >= 0 && cm <= (long)NEAR_DISTANCE_CM);

  uint32_t now = millis();

  // When detected within 10cm, enter/renew timer
  if (near) {
    lastNearMillis = now;

    if (!alerted) {
      alerted = true;

      // welcome on serial
      Serial.println("welcome");

      // LED fast blink and buzzer short鸣 three times.
      // The buzzer should happen immediately on detection; LED continues flashing.
      buzzShortBeepNTimes(BEEP_COUNT);
    }
  }

  if (alerted) {
    if ((now - lastNearMillis) >= ALERT_RECOVER_MS) {
      // Restore standby
      alerted = false;
      digitalWrite(LED_PIN, LOW);
      noTone(BUZZER_PIN);
    } else {
      // Fast blinking LED while near within timeout window.
      static uint32_t ledToggleAt = 0;
      static bool ledState = false;

      uint16_t interval = ledState ? LED_FAST_OFF_MS : LED_FAST_ON_MS;
      if ((now - ledToggleAt) >= interval) {
        ledState = !ledState;
        digitalWrite(LED_PIN, ledState ? HIGH : LOW);
        ledToggleAt = now;
      }
    }
  } else {
    // Standby: ensure outputs are off
    digitalWrite(LED_PIN, LOW);
    noTone(BUZZER_PIN);
  }
}