#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_021\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

/// Frozen hardware contract:
/// - Board: Arduino Uno
/// - tone_output on D6
/// - Required behavior (ACCEPTED executable model semantics):
///   * Initial silence: io_0 = 0 immediately after startup.
///   * First tone starts at t = 1000 ms after startup, for 100 ms duration (approx; tolerance 20 ms).
///   * Then repeats every 1000 ms: start at 2000ms, 3000ms, ... for 100 ms each.
///   * Tone frequency: 1000 Hz using a passive buzzer connected to D6.
///   * No serial usage.
/// - Control_period_ms is 10 ms. We implement a 10ms tick loop and update based on elapsed time.

static const uint8_t TONE_PIN = 6;   // D6
static const unsigned long CONTROL_PERIOD_MS = 10;

static const unsigned long FIRST_START_MS = 1000;  // first start after 1000ms
static const unsigned long PERIOD_MS      = 1000;  // every 1000ms
static const unsigned long TONE_MS        = 100;   // duration 100ms
static const int TONE_HZ                  = 1000;  // 1000Hz

static int io_0 = 0; // model output

static unsigned long t0 = 0;
static unsigned long nextTick = 0;

#line 27 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_021\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void setToneOutputHz(int hz);
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_021\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 44 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_021\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 27 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_021\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void setToneOutputHz(int hz) {
  // "tone_output端口仅提供频率与无音策略"
  if (hz <= 0) {
    noTone(TONE_PIN);
  } else {
    tone(TONE_PIN, hz);
  }
  io_0 = hz;
}

void setup() {
  pinMode(TONE_PIN, OUTPUT);
  setToneOutputHz(0); // initial silence
  t0 = millis();
  nextTick = t0 + CONTROL_PERIOD_MS;
}

void loop() {
  unsigned long now = millis();

  // Run control updates at fixed 10ms steps
  while ((long)(now - nextTick) >= 0) {
    unsigned long elapsed = nextTick - t0; // time at the tick instant

    // Determine if we should be sounding at this tick instant
    // Start times: 1000ms, 2000ms, 3000ms, ...
    // Sound duration: 100ms
    // Condition: elapsed >= FIRST_START_MS and within [start, start+TONE_MS)
    int desired = 0;

    if (elapsed >= FIRST_START_MS) {
      unsigned long k = (elapsed - FIRST_START_MS) / PERIOD_MS; // cycle index
      unsigned long start = FIRST_START_MS + k * PERIOD_MS;
      unsigned long dt = elapsed - start;

      if (dt < TONE_MS) {
        desired = TONE_HZ;
      } else {
        desired = 0;
      }
    } else {
      desired = 0;
    }

    // Update outputs in the ordered controller update
    if (desired != io_0) {
      setToneOutputHz(desired);
    }

    nextTick += CONTROL_PERIOD_MS;
  }

  // Nothing else; keep running. Tone is managed by the last tick update.
}
