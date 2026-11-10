#ifdef ARDUINO
#include <Arduino.h>
#endif
// BEGIN COMPILED SYSML MODEL
#include <stdint.h>
#include <stdbool.h>
#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern int32_t sysml_state_readyElapsedMs;
/* Global Variable define of  */
extern int32_t sysml_state_gameElapsedMs;
/* Global Variable define of  */
extern int32_t sysml_state_roundIndex;
/* Global Variable define of  */
extern int32_t sysml_state_ledPhase;
/* Global Variable define of  */
extern int32_t sysml_state_score;
/* Global Variable define of  */
extern bool sysml_state_previousStopPressed;
/* Global Variable define of  */
extern bool sysml_state_previousBackPressed;
/* Global Variable define of  */
extern bool sysml_state_previousForwardPressed;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_3, bool io_4, bool io_5, bool* io_0, bool* io_1, bool* io_2, int32_t* io_6, bool* serial_0_trigger, int32_t* serial_0_value_0);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_readyElapsedMs = 0;
/* Global Variable define of  */
int32_t sysml_state_gameElapsedMs = 0;
/* Global Variable define of  */
int32_t sysml_state_roundIndex = 0;
/* Global Variable define of  */
int32_t sysml_state_ledPhase = 0;
/* Global Variable define of  */
int32_t sysml_state_score = 0;
/* Global Variable define of  */
bool sysml_state_previousStopPressed = false;
/* Global Variable define of  */
bool sysml_state_previousBackPressed = false;
/* Global Variable define of  */
bool sysml_state_previousForwardPressed = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_readyElapsedMs = 0;
    sysml_state_gameElapsedMs = 0;
    sysml_state_roundIndex = 0;
    sysml_state_ledPhase = 0;
    sysml_state_score = 0;
    sysml_state_previousStopPressed = false;
    sysml_state_previousBackPressed = false;
    sysml_state_previousForwardPressed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_3, bool io_4, bool io_5, bool* io_0, bool* io_1, bool* io_2, int32_t* io_6, bool* serial_0_trigger, int32_t* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    if (true) {
        sysml_state_readyElapsedMs = sysml_state_readyElapsedMs;
        sysml_state_gameElapsedMs = sysml_state_gameElapsedMs;
        sysml_state_roundIndex = sysml_state_roundIndex;
        sysml_state_ledPhase = sysml_state_ledPhase;
        sysml_state_score = sysml_state_score;
    }
    else {
        sysml_state_readyElapsedMs = sysml_state_readyElapsedMs;
    }
    sysml_lazy_0 = io_3;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousStopPressed;
    }
    if (sysml_lazy_0) {
        sysml_state_previousStopPressed = true;
    }
    if ((!io_3)) {
        sysml_state_previousStopPressed = false;
    }
    sysml_lazy_1 = io_4;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_previousBackPressed;
    }
    if (sysml_lazy_1) {
        sysml_state_previousBackPressed = true;
    }
    if ((!io_4)) {
        sysml_state_previousBackPressed = false;
    }
    sysml_lazy_2 = io_5;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_previousForwardPressed;
    }
    if (sysml_lazy_2) {
        sysml_state_previousForwardPressed = true;
    }
    if ((!io_5)) {
        sysml_state_previousForwardPressed = false;
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = false;
    bool sysml_value_1 = 0;
    sysml_value_1 = false;
    bool sysml_value_2 = 0;
    sysml_value_2 = false;
    int32_t sysml_value_3 = 0;
    sysml_value_3 = 0;
    bool sysml_value_4 = 0;
    sysml_value_4 = false;
    int32_t sysml_value_5 = 0;
    sysml_value_5 = 0;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_6 = sysml_value_3;
    *serial_0_trigger = sysml_value_4;
    *serial_0_value_0 = sysml_value_5;
}
// END COMPILED SYSML MODEL

// LLM4MDD target-board: arduino:avr:mega
#include <Arduino.h>
#include <math.h>
#if defined(ARDUINO) && !defined(ARDUINO_AVR_MEGA2560)
#error "Selected Arduino board differs from the interface contract"
#endif
static uint32_t hw_last_step = 0;
static bool hw_started = false;
static bool hw_value_io_0 = false;
static bool hw_value_io_1 = false;
static bool hw_value_io_2 = false;
static bool hw_value_io_3 = false;
static uint32_t hw_sampled_io_3 = 0;
static bool hw_attempted_io_3 = false;
static bool hw_value_io_4 = false;
static uint32_t hw_sampled_io_4 = 0;
static bool hw_attempted_io_4 = false;
static bool hw_value_io_5 = false;
static uint32_t hw_sampled_io_5 = 0;
static bool hw_attempted_io_5 = false;
static int32_t hw_value_io_6 = 0;
static void hw_shift595_io_6(int32_t value) {
  static const uint8_t digits[10] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
  bool valid = value >= 0 && value < 10L;
  digitalWrite(A1, LOW);
  {
    uint8_t segments = valid && (true) ? digits[(value / 1L) % 10] : 0;
    uint8_t bits = 0;
    if (segments & 1) bits |= 1;
    if (segments & 2) bits |= 2;
    if (segments & 4) bits |= 4;
    if (segments & 8) bits |= 8;
    if (segments & 16) bits |= 16;
    if (segments & 32) bits |= 32;
    if (segments & 64) bits |= 64;
    bits = (uint8_t)~bits;
    shiftOut(A0, A2, MSBFIRST, bits);
  }
  digitalWrite(A1, HIGH);
}
static int32_t hw_shift595_previous_io_6 = INT32_MIN;
static bool hw_value_serial_0_trigger = false;
static int32_t hw_value_serial_0_value_0 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  digitalWrite(2, hw_value_io_0 ? HIGH : LOW);
  pinMode(3, OUTPUT);
  digitalWrite(3, hw_value_io_1 ? HIGH : LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, hw_value_io_2 ? HIGH : LOW);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(A0, OUTPUT); digitalWrite(A0, LOW);
  pinMode(A2, OUTPUT); digitalWrite(A2, LOW);
  pinMode(A1, OUTPUT); digitalWrite(A1, LOW);
  if (hw_value_io_6 != hw_shift595_previous_io_6) {
    hw_shift595_io_6(hw_value_io_6); hw_shift595_previous_io_6 = hw_value_io_6;
  }
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_3 || (uint32_t)(hw_now - hw_sampled_io_3) >= 5UL) {
    hw_attempted_io_3 = true; hw_sampled_io_3 = hw_now;
    hw_value_io_3 = (digitalRead(5) == LOW);
  }
  if (!hw_attempted_io_4 || (uint32_t)(hw_now - hw_sampled_io_4) >= 5UL) {
    hw_attempted_io_4 = true; hw_sampled_io_4 = hw_now;
    hw_value_io_4 = (digitalRead(6) == LOW);
  }
  if (!hw_attempted_io_5 || (uint32_t)(hw_now - hw_sampled_io_5) >= 5UL) {
    hw_attempted_io_5 = true; hw_sampled_io_5 = hw_now;
    hw_value_io_5 = (digitalRead(7) == LOW);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_3, hw_value_io_4, hw_value_io_5, &hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_6, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0);
  digitalWrite(2, hw_value_io_0 ? HIGH : LOW);
  digitalWrite(3, hw_value_io_1 ? HIGH : LOW);
  digitalWrite(4, hw_value_io_2 ? HIGH : LOW);
  if (hw_value_io_6 != hw_shift595_previous_io_6) {
    hw_shift595_io_6(hw_value_io_6); hw_shift595_previous_io_6 = hw_value_io_6;
  }
  if (hw_value_serial_0_trigger) { Serial.print("N"); Serial.println(hw_value_serial_0_value_0); }
}
