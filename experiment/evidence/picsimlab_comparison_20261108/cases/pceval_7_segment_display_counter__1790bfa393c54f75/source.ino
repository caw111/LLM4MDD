#include <Arduino.h>
#include <stdint.h>

static const uint8_t PIN_SEG_0 = 7;  // F
static const uint8_t PIN_SEG_1 = 9;  // A
static const uint8_t PIN_SEG_2 = 5;  // G
static const uint8_t PIN_SEG_3 = 8;  // B
static const uint8_t PIN_SEG_4 = 6;  // D
static const uint8_t PIN_SEG_5 = 3;  // C
static const uint8_t PIN_SEG_6 = 4;  // E
static const uint8_t PIN_SEG_7 = 2;  // DP

// Common anode COM.1 -> 5V in wiring; we do not drive COM.1.

static inline bool model_io_1_segment_0(int32_t elapsedSeconds_i32) {
  // (1<=t<2) OR (3<=t<4) => false else true
  bool cond = ((elapsedSeconds_i32 >= 1 && elapsedSeconds_i32 < 2) ||
               (elapsedSeconds_i32 >= 3 && elapsedSeconds_i32 < 4));
  return cond ? false : true;
}
static inline bool model_io_1_segment_1(int32_t elapsedSeconds_i32) {
  // (1<=t<2) OR (2<=t<3) OR (3<=t<4) => true else false
  bool cond = ((elapsedSeconds_i32 >= 1 && elapsedSeconds_i32 < 2) ||
               (elapsedSeconds_i32 >= 2 && elapsedSeconds_i32 < 3) ||
               (elapsedSeconds_i32 >= 3 && elapsedSeconds_i32 < 4));
  return cond ? true : false;
}
static inline bool model_io_1_segment_2(int32_t elapsedSeconds_i32) {
  // (1<=t<2) OR (2<=t<3) => true else false
  bool cond = ((elapsedSeconds_i32 >= 1 && elapsedSeconds_i32 < 2) ||
               (elapsedSeconds_i32 >= 2 && elapsedSeconds_i32 < 3));
  return cond ? true : false;
}
static inline bool model_io_1_segment_3(int32_t elapsedSeconds_i32) {
  // (2<=t<3) OR (3<=t<4) => true else false
  bool cond = ((elapsedSeconds_i32 >= 2 && elapsedSeconds_i32 < 3) ||
               (elapsedSeconds_i32 >= 3 && elapsedSeconds_i32 < 4));
  return cond ? true : false;
}
static inline bool model_io_1_segment_4(int32_t elapsedSeconds_i32) {
  // (1<=t<2) OR (2<=t<3) OR (3<=t<4) => true else false
  bool cond = ((elapsedSeconds_i32 >= 1 && elapsedSeconds_i32 < 2) ||
               (elapsedSeconds_i32 >= 2 && elapsedSeconds_i32 < 3) ||
               (elapsedSeconds_i32 >= 3 && elapsedSeconds_i32 < 4));
  return cond ? true : false;
}
static inline bool model_io_1_segment_5(int32_t elapsedSeconds_i32) {
  // (1<=t<2) OR (2<=t<3) OR (3<=t<4) => true else false
  bool cond = ((elapsedSeconds_i32 >= 1 && elapsedSeconds_i32 < 2) ||
               (elapsedSeconds_i32 >= 2 && elapsedSeconds_i32 < 3) ||
               (elapsedSeconds_i32 >= 3 && elapsedSeconds_i32 < 4));
  return cond ? true : false;
}
static inline bool model_io_1_segment_6(int32_t elapsedSeconds_i32) {
  // (2<=t<3) OR (3<=t<4) => false else true
  bool cond = ((elapsedSeconds_i32 >= 2 && elapsedSeconds_i32 < 3) ||
               (elapsedSeconds_i32 >= 3 && elapsedSeconds_i32 < 4));
  return cond ? false : true;
}
static inline bool model_io_1_segment_7(int32_t elapsedSeconds_i32) {
  // (3<=t<4) => false else true
  bool cond = (elapsedSeconds_i32 >= 3 && elapsedSeconds_i32 < 4);
  return cond ? false : true;
}

static inline uint8_t as_level_from_model_bool(bool model_trueMeansSegmentOn, bool commonAnode = true) {
  // Frozen contract notes:
  // common-anode display, segments illuminate when segment line is driven LOW.
  // SysML maps model ON(true) to electrical active level via polarity inversion.
  // Therefore we drive:
  // - model true (segment ON) => electrical LOW
  // - model false (segment OFF) => electrical HIGH
  // Equivalent: electrical_level = !model_trueMeansSegmentOn
  // (because LOW for true, HIGH for false)
  (void)commonAnode;
  return model_trueMeansSegmentOn ? LOW : HIGH;
}

int32_t elapsedSeconds_i32 = 0;

static uint32_t lastStepMs = 0;

void setup() {
  // Per requirements: set 2~9 pins as output.
  // D2..D9 inclusive
  pinMode(PIN_SEG_7, OUTPUT); // D2
  pinMode(PIN_SEG_5, OUTPUT); // D3
  pinMode(PIN_SEG_6, OUTPUT); // D4
  pinMode(PIN_SEG_2, OUTPUT); // D5
  pinMode(PIN_SEG_4, OUTPUT); // D6
  pinMode(PIN_SEG_0, OUTPUT); // D7
  pinMode(PIN_SEG_3, OUTPUT); // D8
  pinMode(PIN_SEG_1, OUTPUT); // D9

  // Initialize to model initial outputs at elapsedSeconds_i32=0.
  // Model:
  // seg0 true (since else branch true)
  // seg1 false
  // seg2 false
  // seg3 false
  // seg4 false
  // seg5 false
  // seg6 true
  // seg7 true
  // Drive with common-anode polarity inversion.
  bool s0 = model_io_1_segment_0(elapsedSeconds_i32);
  bool s1 = model_io_1_segment_1(elapsedSeconds_i32);
  bool s2 = model_io_1_segment_2(elapsedSeconds_i32);
  bool s3 = model_io_1_segment_3(elapsedSeconds_i32);
  bool s4 = model_io_1_segment_4(elapsedSeconds_i32);
  bool s5 = model_io_1_segment_5(elapsedSeconds_i32);
  bool s6 = model_io_1_segment_6(elapsedSeconds_i32);
  bool s7 = model_io_1_segment_7(elapsedSeconds_i32);

  digitalWrite(PIN_SEG_0, as_level_from_model_bool(s0));
  digitalWrite(PIN_SEG_1, as_level_from_model_bool(s1));
  digitalWrite(PIN_SEG_2, as_level_from_model_bool(s2));
  digitalWrite(PIN_SEG_3, as_level_from_model_bool(s3));
  digitalWrite(PIN_SEG_4, as_level_from_model_bool(s4));
  digitalWrite(PIN_SEG_5, as_level_from_model_bool(s5));
  digitalWrite(PIN_SEG_6, as_level_from_model_bool(s6));
  digitalWrite(PIN_SEG_7, as_level_from_model_bool(s7));

  lastStepMs = millis();
}

void loop() {
  const uint32_t control_period_ms = 5;

  uint32_t now = millis();
  uint32_t elapsedSinceLast = now - lastStepMs;

  if (elapsedSinceLast < control_period_ms) {
    return; // keep strict 5ms control period
  }

  // Number of 5ms steps passed (deterministic accumulation of io_0 deltas).
  // Contract uses io_0 = delta_since_previous_step. Apply ticks one-by-one to preserve semantics.
  uint32_t steps = elapsedSinceLast / control_period_ms;
  if (steps == 0) steps = 1;

  for (uint32_t i = 0; i < steps; i++) {
    uint32_t stepDeltaMs = control_period_ms;

    // perform action tick:
    // elapsedSeconds_i32 := elapsedSeconds_i32 + floor(io_0 / 1000.0)
    // For io_0 in {5,10,...}, floor division yields 0 until accumulated reaches >=1000ms.
    elapsedSeconds_i32 = elapsedSeconds_i32 + (int32_t)(stepDeltaMs / 1000.0); // since floor(io_0/1000)
    // then if elapsedSeconds_i32 < 0 then set to 0
    if (elapsedSeconds_i32 < 0) {
      elapsedSeconds_i32 = 0;
    }
  }

  lastStepMs += steps * control_period_ms;

  // Outputs (combinational from elapsedSeconds_i32)
  bool seg0 = model_io_1_segment_0(elapsedSeconds_i32);
  bool seg1 = model_io_1_segment_1(elapsedSeconds_i32);
  bool seg2 = model_io_1_segment_2(elapsedSeconds_i32);
  bool seg3 = model_io_1_segment_3(elapsedSeconds_i32);
  bool seg4 = model_io_1_segment_4(elapsedSeconds_i32);
  bool seg5 = model_io_1_segment_5(elapsedSeconds_i32);
  bool seg6 = model_io_1_segment_6(elapsedSeconds_i32);
  bool seg7 = model_io_1_segment_7(elapsedSeconds_i32);

  // Drive pins with electrical levels for common anode:
  // model true => LOW (segment ON)
  // model false => HIGH (segment OFF)
  digitalWrite(PIN_SEG_0, as_level_from_model_bool(seg0));
  digitalWrite(PIN_SEG_1, as_level_from_model_bool(seg1));
  digitalWrite(PIN_SEG_2, as_level_from_model_bool(seg2));
  digitalWrite(PIN_SEG_3, as_level_from_model_bool(seg3));
  digitalWrite(PIN_SEG_4, as_level_from_model_bool(seg4));
  digitalWrite(PIN_SEG_5, as_level_from_model_bool(seg5));
  digitalWrite(PIN_SEG_6, as_level_from_model_bool(seg6));
  digitalWrite(PIN_SEG_7, as_level_from_model_bool(seg7));
}