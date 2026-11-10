#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_003\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const uint8_t TRIG_PIN = D8;   // HC-SR04 Trig
const uint8_t ECHO_PIN = D9;   // HC-SR04 Echo
const uint8_t LED_PIN  = D13;  // LED
const uint8_t BUZZ_PIN = D6;   // Buzzer

const unsigned long WELCOME_COOLDOWN_MS = 5000UL; // 若 5 秒内无人靠近则恢复待机
const unsigned int CLOSE_DISTANCE_CM = 10;        // 10cm 内有人靠近

// Timing for "LED快速闪烁、蜂鸣器短鸣三次"
const unsigned long BUZZ_PATTERN_START_MS = 0;     // used internally
const uint8_t BUZZ_TONE_FREQ = 2000;              // Hz (arbitrary but suitable for buzzer)
const uint16_t BUZZ_ON_MS = 120;                  // each short鸣 duration
const uint16_t BUZZ_OFF_MS = 120;                // gap between beeps
const uint8_t BUZZ_REPEAT = 3;                   // 三次

const uint16_t LED_TOGGLE_ON_MS = 60;            // fast blink interval
const uint16_t LED_TOGGLE_OFF_MS = 60;

static bool welcomeActive = false;
static unsigned long welcomeUntilMs = 0;

static bool wasCloseInPrevCycle = false;

static bool buzzerDoing = false;
static uint8_t buzzerCount = 0;
static unsigned long buzzerNextChangeMs = 0;
static bool buzzerOnState = false;

static unsigned long lastLedToggleMs = 0;
static bool ledState = false;

#line 34 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_003\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static long readUltrasonicCm();
#line 51 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_003\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void startWelcomeIfNeeded(unsigned long nowMs);
#line 76 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_003\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void handleWelcomeEffects(unsigned long nowMs);
#line 124 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_003\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void endWelcomeIfExpired(unsigned long nowMs);
#line 139 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_003\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 152 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_003\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 34 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_003\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
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
    // Should not normally happen; ensure outputs in standby
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

  // Buzzer short鸣三次 (do pattern once per entry into welcome; keep stateful)
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
    // But do not clear wasCloseInPrevCycle immediately to avoid retriggering welcome on transient gaps.
    // The next "close after previously not close" will still only trigger if not currently marked.
    // If welcome is active, keep effects. If standby, we'll mark wasCloseInPrevCycle false when expired.
    if (!welcomeActive) {
      wasCloseInPrevCycle = false;
    }
  }

  if (welcomeActive) {
    handleWelcomeEffects(nowMs);
  } else {
    // Standby outputs already handled by endWelcomeIfExpired
  }

  delay(60); // modest cycle to balance responsiveness and HC-SR04 trigger rate
}
