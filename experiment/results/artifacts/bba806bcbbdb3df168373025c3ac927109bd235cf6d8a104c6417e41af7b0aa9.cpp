#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\repaired_workflow_continuation_20261016\\embedbench_63\\workflow\\attempt_0\\sketch\\sketch.ino"
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
extern int32_t sysml_state_accumulatedMs;
/* Global Variable define of  */
extern int32_t sysml_state_currentNumber;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_5, double* io_0, double* io_1, double* io_2, int32_t* io_3, bool* io_4_segment_0, bool* io_4_segment_1, bool* io_4_segment_2, bool* io_4_segment_3, bool* io_4_segment_4, bool* io_4_segment_5, bool* io_4_segment_6, bool* io_4_segment_7, bool* io_4_segment_8, bool* io_4_segment_9, bool* io_6);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_accumulatedMs = 0;
/* Global Variable define of  */
int32_t sysml_state_currentNumber = 0;
/* Init function of CompositeActor named "ProgramPage1" */
#line 222 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\repaired_workflow_continuation_20261016\\embedbench_63\\workflow\\attempt_0\\sketch\\sketch.ino"
static void hw_shift595_io_3(int32_t value);
#line 255 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\repaired_workflow_continuation_20261016\\embedbench_63\\workflow\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 293 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\repaired_workflow_continuation_20261016\\embedbench_63\\workflow\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 33 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\repaired_workflow_continuation_20261016\\embedbench_63\\workflow\\attempt_0\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_accumulatedMs = 0;
    sysml_state_currentNumber = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_5, double* io_0, double* io_1, double* io_2, int32_t* io_3, bool* io_4_segment_0, bool* io_4_segment_1, bool* io_4_segment_2, bool* io_4_segment_3, bool* io_4_segment_4, bool* io_4_segment_5, bool* io_4_segment_6, bool* io_4_segment_7, bool* io_4_segment_8, bool* io_4_segment_9, bool* io_6) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int32_t sysml_conditional_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    double sysml_conditional_4 = 0;
    bool sysml_lazy_5 = 0;
    double sysml_conditional_6 = 0;
    bool sysml_lazy_7 = 0;
    double sysml_conditional_8 = 0;
    bool sysml_conditional_9 = 0;
    bool sysml_conditional_10 = 0;
    bool sysml_conditional_11 = 0;
    bool sysml_conditional_12 = 0;
    bool sysml_conditional_13 = 0;
    bool sysml_conditional_14 = 0;
    bool sysml_conditional_15 = 0;
    bool sysml_conditional_16 = 0;
    bool sysml_conditional_17 = 0;
    sysml_state_accumulatedMs = (sysml_state_accumulatedMs + io_5);
    if ((sysml_state_accumulatedMs >= 2000)) {
        sysml_state_accumulatedMs = (sysml_state_accumulatedMs - 2000);
        if (sysml_state_currentNumber >= 9) {
            sysml_conditional_0 = 0;
        }
        else {
            sysml_conditional_0 = sysml_state_currentNumber + 1;
        }
        sysml_state_currentNumber = sysml_conditional_0;
    }
    double sysml_value_0 = 0;
    sysml_lazy_1 = sysml_state_currentNumber == 0;
    if (!sysml_lazy_1) {
        sysml_lazy_1 = sysml_state_currentNumber == 1;
    }
    sysml_lazy_2 = sysml_lazy_1;
    if (!sysml_lazy_2) {
        sysml_lazy_2 = sysml_state_currentNumber == 2;
    }
    sysml_lazy_3 = sysml_lazy_2;
    if (!sysml_lazy_3) {
        sysml_lazy_3 = sysml_state_currentNumber == 3;
    }
    if (sysml_lazy_3) {
        sysml_conditional_4 = 1.0;
    }
    else {
        sysml_conditional_4 = 0.0;
    }
    sysml_value_0 = sysml_conditional_4;
    double sysml_value_1 = 0;
    sysml_lazy_5 = sysml_state_currentNumber >= 4;
    if (sysml_lazy_5) {
        sysml_lazy_5 = sysml_state_currentNumber <= 6;
    }
    if (sysml_lazy_5) {
        sysml_conditional_6 = 1.0;
    }
    else {
        sysml_conditional_6 = 0.0;
    }
    sysml_value_1 = sysml_conditional_6;
    double sysml_value_2 = 0;
    sysml_lazy_7 = sysml_state_currentNumber >= 7;
    if (sysml_lazy_7) {
        sysml_lazy_7 = sysml_state_currentNumber <= 9;
    }
    if (sysml_lazy_7) {
        sysml_conditional_8 = 1.0;
    }
    else {
        sysml_conditional_8 = 0.0;
    }
    sysml_value_2 = sysml_conditional_8;
    int32_t sysml_value_3 = 0;
    sysml_value_3 = sysml_state_currentNumber;
    bool sysml_value_4 = 0;
    if (sysml_state_currentNumber >= 1) {
        sysml_conditional_9 = true;
    }
    else {
        sysml_conditional_9 = false;
    }
    sysml_value_4 = sysml_conditional_9;
    bool sysml_value_5 = 0;
    if (sysml_state_currentNumber >= 2) {
        sysml_conditional_10 = true;
    }
    else {
        sysml_conditional_10 = false;
    }
    sysml_value_5 = sysml_conditional_10;
    bool sysml_value_6 = 0;
    if (sysml_state_currentNumber >= 3) {
        sysml_conditional_11 = true;
    }
    else {
        sysml_conditional_11 = false;
    }
    sysml_value_6 = sysml_conditional_11;
    bool sysml_value_7 = 0;
    if (sysml_state_currentNumber >= 4) {
        sysml_conditional_12 = true;
    }
    else {
        sysml_conditional_12 = false;
    }
    sysml_value_7 = sysml_conditional_12;
    bool sysml_value_8 = 0;
    if (sysml_state_currentNumber >= 5) {
        sysml_conditional_13 = true;
    }
    else {
        sysml_conditional_13 = false;
    }
    sysml_value_8 = sysml_conditional_13;
    bool sysml_value_9 = 0;
    if (sysml_state_currentNumber >= 6) {
        sysml_conditional_14 = true;
    }
    else {
        sysml_conditional_14 = false;
    }
    sysml_value_9 = sysml_conditional_14;
    bool sysml_value_10 = 0;
    if (sysml_state_currentNumber >= 7) {
        sysml_conditional_15 = true;
    }
    else {
        sysml_conditional_15 = false;
    }
    sysml_value_10 = sysml_conditional_15;
    bool sysml_value_11 = 0;
    if (sysml_state_currentNumber >= 8) {
        sysml_conditional_16 = true;
    }
    else {
        sysml_conditional_16 = false;
    }
    sysml_value_11 = sysml_conditional_16;
    bool sysml_value_12 = 0;
    if (sysml_state_currentNumber >= 9) {
        sysml_conditional_17 = true;
    }
    else {
        sysml_conditional_17 = false;
    }
    sysml_value_12 = sysml_conditional_17;
    bool sysml_value_13 = 0;
    sysml_value_13 = false;
    bool sysml_value_14 = 0;
    sysml_value_14 = false;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
    *io_4_segment_0 = sysml_value_4;
    *io_4_segment_1 = sysml_value_5;
    *io_4_segment_2 = sysml_value_6;
    *io_4_segment_3 = sysml_value_7;
    *io_4_segment_4 = sysml_value_8;
    *io_4_segment_5 = sysml_value_9;
    *io_4_segment_6 = sysml_value_10;
    *io_4_segment_7 = sysml_value_11;
    *io_4_segment_8 = sysml_value_12;
    *io_4_segment_9 = sysml_value_13;
    *io_6 = sysml_value_14;
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
static double hw_value_io_0 = 0;
static double hw_value_io_1 = 0;
static double hw_value_io_2 = 0;
static int32_t hw_value_io_3 = 0;
static void hw_shift595_io_3(int32_t value) {
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
static int32_t hw_shift595_previous_io_3 = INT32_MIN;
static bool hw_value_io_4_segment_0 = false;
static bool hw_value_io_4_segment_1 = false;
static bool hw_value_io_4_segment_2 = false;
static bool hw_value_io_4_segment_3 = false;
static bool hw_value_io_4_segment_4 = false;
static bool hw_value_io_4_segment_5 = false;
static bool hw_value_io_4_segment_6 = false;
static bool hw_value_io_4_segment_7 = false;
static bool hw_value_io_4_segment_8 = false;
static bool hw_value_io_4_segment_9 = false;
static int32_t hw_value_io_5 = 0;
static bool hw_value_io_6 = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, OUTPUT);
  analogWrite(2, (int)constrain((hw_value_io_0 * 1 + 0.0), 0.0, 255.0));
  pinMode(3, OUTPUT);
  analogWrite(3, (int)constrain((hw_value_io_1 * 1 + 0.0), 0.0, 255.0));
  pinMode(4, OUTPUT);
  analogWrite(4, (int)constrain((hw_value_io_2 * 1 + 0.0), 0.0, 255.0));
  pinMode(A0, OUTPUT); digitalWrite(A0, LOW);
  pinMode(A2, OUTPUT); digitalWrite(A2, LOW);
  pinMode(A1, OUTPUT); digitalWrite(A1, LOW);
  if (hw_value_io_3 != hw_shift595_previous_io_3) {
    hw_shift595_io_3(hw_value_io_3); hw_shift595_previous_io_3 = hw_value_io_3;
  }
  pinMode(5, OUTPUT);
  digitalWrite(5, hw_value_io_4_segment_0 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  digitalWrite(6, hw_value_io_4_segment_1 ? HIGH : LOW);
  pinMode(7, OUTPUT);
  digitalWrite(7, hw_value_io_4_segment_2 ? HIGH : LOW);
  pinMode(8, OUTPUT);
  digitalWrite(8, hw_value_io_4_segment_3 ? HIGH : LOW);
  pinMode(9, OUTPUT);
  digitalWrite(9, hw_value_io_4_segment_4 ? HIGH : LOW);
  pinMode(10, OUTPUT);
  digitalWrite(10, hw_value_io_4_segment_5 ? HIGH : LOW);
  pinMode(11, OUTPUT);
  digitalWrite(11, hw_value_io_4_segment_6 ? HIGH : LOW);
  pinMode(12, OUTPUT);
  digitalWrite(12, hw_value_io_4_segment_7 ? HIGH : LOW);
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_4_segment_8 ? HIGH : LOW);
  pinMode(44, OUTPUT);
  digitalWrite(44, hw_value_io_4_segment_9 ? HIGH : LOW);
  pinMode(A10, OUTPUT);
  digitalWrite(A10, hw_value_io_6 ? HIGH : LOW);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_5 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_5, &hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4_segment_0, &hw_value_io_4_segment_1, &hw_value_io_4_segment_2, &hw_value_io_4_segment_3, &hw_value_io_4_segment_4, &hw_value_io_4_segment_5, &hw_value_io_4_segment_6, &hw_value_io_4_segment_7, &hw_value_io_4_segment_8, &hw_value_io_4_segment_9, &hw_value_io_6);
  analogWrite(2, (int)constrain((hw_value_io_0 * 1 + 0.0), 0.0, 255.0));
  analogWrite(3, (int)constrain((hw_value_io_1 * 1 + 0.0), 0.0, 255.0));
  analogWrite(4, (int)constrain((hw_value_io_2 * 1 + 0.0), 0.0, 255.0));
  if (hw_value_io_3 != hw_shift595_previous_io_3) {
    hw_shift595_io_3(hw_value_io_3); hw_shift595_previous_io_3 = hw_value_io_3;
  }
  digitalWrite(5, hw_value_io_4_segment_0 ? HIGH : LOW);
  digitalWrite(6, hw_value_io_4_segment_1 ? HIGH : LOW);
  digitalWrite(7, hw_value_io_4_segment_2 ? HIGH : LOW);
  digitalWrite(8, hw_value_io_4_segment_3 ? HIGH : LOW);
  digitalWrite(9, hw_value_io_4_segment_4 ? HIGH : LOW);
  digitalWrite(10, hw_value_io_4_segment_5 ? HIGH : LOW);
  digitalWrite(11, hw_value_io_4_segment_6 ? HIGH : LOW);
  digitalWrite(12, hw_value_io_4_segment_7 ? HIGH : LOW);
  digitalWrite(13, hw_value_io_4_segment_8 ? HIGH : LOW);
  digitalWrite(44, hw_value_io_4_segment_9 ? HIGH : LOW);
  digitalWrite(A10, hw_value_io_6 ? HIGH : LOW);
}

