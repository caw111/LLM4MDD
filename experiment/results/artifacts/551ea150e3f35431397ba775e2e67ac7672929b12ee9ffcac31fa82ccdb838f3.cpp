#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\repaired_workflow_continuation_20261016\\embedbench_101\\workflow\\attempt_0\\sketch\\sketch.ino"
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
extern bool sysml_state_ledPhase;
/* Global Variable define of  */
extern int32_t sysml_state_blinkAccumMs;
/* Global Variable define of  */
extern int32_t sysml_state_digit;
/* Global Variable define of  */
extern int32_t sysml_state_countAccumMs;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_3, bool* io_0, bool* io_1, int32_t* io_2);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_ledPhase = false;
/* Global Variable define of  */
int32_t sysml_state_blinkAccumMs = 0;
/* Global Variable define of  */
int32_t sysml_state_digit = 9;
/* Global Variable define of  */
int32_t sysml_state_countAccumMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
#line 103 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\repaired_workflow_continuation_20261016\\embedbench_101\\workflow\\attempt_0\\sketch\\sketch.ino"
static void hw_shift595_io_2(int32_t value);
#line 125 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\repaired_workflow_continuation_20261016\\embedbench_101\\workflow\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 139 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\repaired_workflow_continuation_20261016\\embedbench_101\\workflow\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 41 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\repaired_workflow_continuation_20261016\\embedbench_101\\workflow\\attempt_0\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_ledPhase = false;
    sysml_state_blinkAccumMs = 0;
    sysml_state_digit = 9;
    sysml_state_countAccumMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_3, bool* io_0, bool* io_1, int32_t* io_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int32_t sysml_conditional_0 = 0;
    bool sysml_conditional_1 = 0;
    if (true) {
        sysml_state_blinkAccumMs = (sysml_state_blinkAccumMs + io_3);
        if ((sysml_state_blinkAccumMs >= 2000)) {
            sysml_state_blinkAccumMs = (sysml_state_blinkAccumMs - 2000);
            sysml_state_ledPhase = (!sysml_state_ledPhase);
        }
        sysml_state_countAccumMs = (sysml_state_countAccumMs + io_3);
        if ((sysml_state_countAccumMs >= 4000)) {
            sysml_state_countAccumMs = (sysml_state_countAccumMs - 4000);
            if (sysml_state_digit == 0) {
                sysml_conditional_0 = 9;
            }
            else {
                sysml_conditional_0 = sysml_state_digit - 1;
            }
            sysml_state_digit = sysml_conditional_0;
            if ((sysml_state_digit == 0)) {
                sysml_state_digit = 9;
            }
        }
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_ledPhase;
    bool sysml_value_1 = 0;
    if (sysml_state_ledPhase) {
        sysml_conditional_1 = false;
    }
    else {
        sysml_conditional_1 = true;
    }
    sysml_value_1 = sysml_conditional_1;
    int32_t sysml_value_2 = 0;
    sysml_value_2 = sysml_state_digit;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
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
static int32_t hw_value_io_2 = 9;
static void hw_shift595_io_2(int32_t value) {
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
static int32_t hw_shift595_previous_io_2 = INT32_MIN;
static int32_t hw_value_io_3 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, OUTPUT);
  digitalWrite(2, hw_value_io_0 ? HIGH : LOW);
  pinMode(3, OUTPUT);
  digitalWrite(3, hw_value_io_1 ? HIGH : LOW);
  pinMode(A0, OUTPUT); digitalWrite(A0, LOW);
  pinMode(A2, OUTPUT); digitalWrite(A2, LOW);
  pinMode(A1, OUTPUT); digitalWrite(A1, LOW);
  if (hw_value_io_2 != hw_shift595_previous_io_2) {
    hw_shift595_io_2(hw_value_io_2); hw_shift595_previous_io_2 = hw_value_io_2;
  }
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_3 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_3, &hw_value_io_0, &hw_value_io_1, &hw_value_io_2);
  digitalWrite(2, hw_value_io_0 ? HIGH : LOW);
  digitalWrite(3, hw_value_io_1 ? HIGH : LOW);
  if (hw_value_io_2 != hw_shift595_previous_io_2) {
    hw_shift595_io_2(hw_value_io_2); hw_shift595_previous_io_2 = hw_value_io_2;
  }
}

