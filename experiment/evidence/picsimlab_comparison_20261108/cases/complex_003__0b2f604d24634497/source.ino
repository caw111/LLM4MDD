#include <Arduino.h>

// Arduino UNO pins (use numeric pins; D8 etc. may not be defined in some cores)
static const uint8_t TRIG_PIN = 8;   // HC-SR04 Trig  -> D8
static const uint8_t ECHO_PIN = 9;   // HC-SR04 Echo  -> D9
static const uint8_t LED_PIN  = 13;  // LED           -> D13
static const uint8_t BUZZ_PIN = 6;   // Buzzer        -> D6

static const unsigned long WELCOME_COOLDOWN_MS = 5000UL; // 若 5 秒内无人靠近则恢复待机
static const unsigned int CLOSE_DISTANCE_CM = 10;         // 10cm 内有人靠近

// Timing for "LED快速闪烁、蜂鸣器短鸣三次"
static const uint8_t  BUZZ_TONE_FREQ = 2000;   // Hz
static const uint16_t BUZZ_ON_MS     = 120;    // each short鸣 duration
static const uint16_t BUZZ_OFF_MS    = 120;    // gap between beeps
static const uint8_t  BUZZ_REPEAT    = 3;      // 三次

static const uint16_t LED_TOGGLE_ON_MS  = 60;  // fast blink interval
static const uint16_t LED_TOGGLE_OFF_MS = 60;

static bool welcomeActive = false;
static unsigned long welcomeUntilMs = 0;

static bool wasCloseInPrevCycle = false;

static bool buzzerDoing = false;
static uint8_t buzzerCount = 0;
static unsigned long buzzerNextChangeMs = 0;
static bool buzzerOnState = false;

static unsigned long lastLedToggleMs = 0;
static bool ledState = false;

static long readUltrasonicCm() {
  // Trigger 10us pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read echo pulse width
  unsigned long duration = pulseIn(ECHO_PIN, HIGH, 30000UL); // ~max 5m
  if (duration == 0) return -1;

  // HC-SR04: distance(cm) = duration(us) / 58.0
  long cm = (long)(duration / 58UL);
  return cm;
}

static void startWelcomeIfNeeded(unsigned long nowMs) {
  // Called when a close person is detected.
  // Keep welcome active for 5 seconds from the most recent detection.
  welcomeActive = true;
  welcomeUntilMs = nowMs + WELCOME_COOLDOWN_MS;

  // If just transitioning from not active to active, do effects and serial welcome once.
  if (!wasCloseInPrevCycle) {
    Serial.println("welcome");

    // Initialize buzzer short鸣三次 sequence
    buzzerDoing = true;
    buzzerCount = 0;
    buzzerOnState = true;
    buzzerNextChangeMs = nowMs + BUZZ_ON_MS;

    // Ensure LED starts blinking immediately
    ledState = true;
    digitalWrite(LED_PIN, HIGH);
    lastLedToggleMs = nowMs;
  }

  wasCloseInPrevCycle = true;
}

static void handleWelcomeEffects(unsigned long nowMs) {
  if (!welcomeActive) {
    // Ensure outputs in standby
    if (buzzerDoing) {
      buzzerDoing = false;
      buzzerOnState = false;
      noTone(BUZZ_PIN);
    }
    digitalWrite(LED_PIN, LOW);
    return;
  }

  // LED fast blinking while in welcomeActive
  if (ledState && (nowMs - lastLedToggleMs >= (unsigned long)LED_TOGGLE_ON_MS)) {
    ledState = false;
    digitalWrite(LED_PIN, LOW);
    lastLedToggleMs = nowMs;
  } else if (!ledState && (nowMs - lastLedToggleMs >= (unsigned long)LED_TOGGLE_OFF_MS)) {
    ledState = true;
    digitalWrite(LED_PIN, HIGH);
    lastLedToggleMs = nowMs;
  }

  // Buzzer short鸣三次 (stateful; runs once per entry into welcome)
  if (buzzerDoing) {
    if (nowMs >= buzzerNextChangeMs) {
      if (buzzerOnState) {
        // Turn off and schedule next
        buzzerOnState = false;
        noTone(BUZZ_PIN);
        buzzerNextChangeMs = nowMs + BUZZ_OFF_MS;
      } else {
        // Currently off; either start next beep or finish
        buzzerCount++;
        if (buzzerCount >= BUZZ_REPEAT) {
          buzzerDoing = false;
          buzzerOnState = false;
          noTone(BUZZ_PIN);
        } else {
          buzzerOnState = true;
          tone(BUZZ_PIN, BUZZ_TONE_FREQ);
          buzzerNextChangeMs = nowMs + BUZZ_ON_MS;
        }
      }
    }
  }
}

static void endWelcomeIfExpired(unsigned long nowMs) {
  if (welcomeActive && nowMs >= welcomeUntilMs) {
    welcomeActive = false;
    wasCloseInPrevCycle = false;

    // Restore standby: stop LED and buzzer
    digitalWrite(LED_PIN, LOW);
    if (buzzerDoing) {
      buzzerDoing = false;
      buzzerOnState = false;
      noTone(BUZZ_PIN);
    }
  }
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZ_PIN, OUTPUT);

  digitalWrite(TRIG_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
  noTone(BUZZ_PIN);

  Serial.begin(9600);
}

void loop() {
  unsigned long nowMs = millis();

  // End welcome after 5 seconds without close detection
  endWelcomeIfExpired(nowMs);

  // Read distance and determine close detection
  long cm = readUltrasonicCm();
  bool close = (cm > 0 && cm <= (long)CLOSE_DISTANCE_CM);

  if (close) {
    startWelcomeIfNeeded(nowMs);
  } else {
    // If not close, do not change welcomeActive here; it times out via endWelcomeIfExpired.
    if (!welcomeActive) {
      wasCloseInPrevCycle = false;
    }
  }

  if (welcomeActive) {
    handleWelcomeEffects(nowMs);
  }

  delay(60); // modest cycle to balance responsiveness and HC-SR04 trigger rate
}