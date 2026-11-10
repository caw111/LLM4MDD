#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_021\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

static const uint8_t BUZZER_PIN = 6;      // D6
static const uint16_t T_INTERVAL_MS = 1000;
static const uint16_t T_BEEP_MS = 100;
static const uint16_t T_START_DELAY_MS = 1000;

static const uint16_t TONE_FREQ_HZ = 1000;

#line 10 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_021\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 20 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_021\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 10 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_021\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(BUZZER_PIN, OUTPUT);

  // Initial silent state
  noTone(BUZZER_PIN);

  // Ensure known state at boot
  digitalWrite(BUZZER_PIN, LOW);
}

void loop() {
  static bool started = false;
  static unsigned long startMs = 0;
  unsigned long now = millis();

  if (!started) {
    started = true;
    startMs = now;
  }

  unsigned long elapsed = now - startMs;

  if (elapsed < T_START_DELAY_MS) {
    noTone(BUZZER_PIN);
    return;
  }

  unsigned long cycle = elapsed / T_INTERVAL_MS;
  unsigned long within = elapsed - (cycle * (unsigned long)T_INTERVAL_MS);

  if (within < T_BEEP_MS) {
    tone(BUZZER_PIN, TONE_FREQ_HZ);   // Passive buzzer, 1000 Hz
  } else {
    noTone(BUZZER_PIN);
  }
}
