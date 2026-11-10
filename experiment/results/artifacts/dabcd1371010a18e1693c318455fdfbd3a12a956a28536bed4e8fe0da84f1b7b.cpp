#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_13\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
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
extern int32_t sysml_state_seed;
/* Global Variable define of  */
extern int32_t sysml_state_ledCount;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_4, bool* io_0, bool* io_1, bool* io_2, int32_t* io_3);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_accumulatedMs = 0;
/* Global Variable define of  */
int32_t sysml_state_seed = 1;
/* Global Variable define of  */
int32_t sysml_state_ledCount = 0;
/* Init function of CompositeActor named "ProgramPage1" */
#line 235 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_13\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
static void hw_shift595_io_3(int32_t value);
#line 257 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_13\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 273 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_13\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_13\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_accumulatedMs = 0;
    sysml_state_seed = 1;
    sysml_state_ledCount = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_4, bool* io_0, bool* io_1, bool* io_2, int32_t* io_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int32_t sysml_conditional_0 = 0;
    int32_t sysml_arithmetic_1 = 0;
    int32_t sysml_conditional_2 = 0;
    int32_t sysml_arithmetic_3 = 0;
    int32_t sysml_conditional_4 = 0;
    int32_t sysml_arithmetic_5 = 0;
    int32_t sysml_conditional_6 = 0;
    int32_t sysml_conditional_7 = 0;
    int32_t sysml_arithmetic_8 = 0;
    int32_t sysml_conditional_9 = 0;
    int32_t sysml_arithmetic_10 = 0;
    int32_t sysml_conditional_11 = 0;
    int32_t sysml_arithmetic_12 = 0;
    int32_t sysml_conditional_13 = 0;
    int32_t sysml_conditional_14 = 0;
    int32_t sysml_arithmetic_15 = 0;
    int32_t sysml_conditional_16 = 0;
    int32_t sysml_arithmetic_17 = 0;
    int32_t sysml_conditional_18 = 0;
    int32_t sysml_arithmetic_19 = 0;
    int32_t sysml_conditional_20 = 0;
    int32_t sysml_conditional_21 = 0;
    int32_t sysml_arithmetic_22 = 0;
    int32_t sysml_conditional_23 = 0;
    int32_t sysml_arithmetic_24 = 0;
    int32_t sysml_conditional_25 = 0;
    int32_t sysml_arithmetic_26 = 0;
    int32_t sysml_conditional_27 = 0;
    int32_t sysml_arithmetic_28 = 0;
    int32_t sysml_conditional_29 = 0;
    int32_t sysml_arithmetic_30 = 0;
    int32_t sysml_conditional_31 = 0;
    int32_t sysml_arithmetic_32 = 0;
    sysml_state_accumulatedMs = (sysml_state_accumulatedMs + io_4);
    if ((sysml_state_accumulatedMs >= 2000)) {
        sysml_state_accumulatedMs = (sysml_state_accumulatedMs - 2000);
        sysml_state_seed = ((sysml_state_seed * 1103515245) + 12345);
        if ((((double)sysml_state_seed) / 4) < ((double)((int32_t)(((double)sysml_state_seed) / 4)))) {
            sysml_conditional_0 = 1;
        }
        else {
            sysml_conditional_0 = 0;
        }
        sysml_arithmetic_1 = ((int32_t)(((double)sysml_state_seed) / 4)) - sysml_conditional_0;
        if ((((double)sysml_state_seed) / 4) < ((double)((int32_t)(((double)sysml_state_seed) / 4)))) {
            sysml_conditional_2 = 1;
        }
        else {
            sysml_conditional_2 = 0;
        }
        sysml_arithmetic_3 = ((int32_t)(((double)sysml_state_seed) / 4)) - sysml_conditional_2;
        if ((((double)sysml_state_seed) / 4) < ((double)((int32_t)(((double)sysml_state_seed) / 4)))) {
            sysml_conditional_4 = 1;
        }
        else {
            sysml_conditional_4 = 0;
        }
        sysml_arithmetic_5 = ((int32_t)(((double)sysml_state_seed) / 4)) - sysml_conditional_4;
        if ((((double)(sysml_state_seed - (4 * sysml_arithmetic_3))) / 1) < ((double)((int32_t)(((double)(sysml_state_seed - (4 * sysml_arithmetic_5))) / 1)))) {
            sysml_conditional_6 = 1;
        }
        else {
            sysml_conditional_6 = 0;
        }
        if ((((double)sysml_state_seed) / 4) < ((double)((int32_t)(((double)sysml_state_seed) / 4)))) {
            sysml_conditional_7 = 1;
        }
        else {
            sysml_conditional_7 = 0;
        }
        sysml_arithmetic_8 = ((int32_t)(((double)sysml_state_seed) / 4)) - sysml_conditional_7;
        if ((((double)sysml_state_seed) / 4) < ((double)((int32_t)(((double)sysml_state_seed) / 4)))) {
            sysml_conditional_9 = 1;
        }
        else {
            sysml_conditional_9 = 0;
        }
        sysml_arithmetic_10 = ((int32_t)(((double)sysml_state_seed) / 4)) - sysml_conditional_9;
        if ((((double)sysml_state_seed) / 4) < ((double)((int32_t)(((double)sysml_state_seed) / 4)))) {
            sysml_conditional_11 = 1;
        }
        else {
            sysml_conditional_11 = 0;
        }
        sysml_arithmetic_12 = ((int32_t)(((double)sysml_state_seed) / 4)) - sysml_conditional_11;
        if ((((double)(sysml_state_seed - (4 * sysml_arithmetic_10))) / 1) < ((double)((int32_t)(((double)(sysml_state_seed - (4 * sysml_arithmetic_12))) / 1)))) {
            sysml_conditional_13 = 1;
        }
        else {
            sysml_conditional_13 = 0;
        }
        if ((((double)sysml_state_seed) / 4) < ((double)((int32_t)(((double)sysml_state_seed) / 4)))) {
            sysml_conditional_14 = 1;
        }
        else {
            sysml_conditional_14 = 0;
        }
        sysml_arithmetic_15 = ((int32_t)(((double)sysml_state_seed) / 4)) - sysml_conditional_14;
        if ((((double)sysml_state_seed) / 4) < ((double)((int32_t)(((double)sysml_state_seed) / 4)))) {
            sysml_conditional_16 = 1;
        }
        else {
            sysml_conditional_16 = 0;
        }
        sysml_arithmetic_17 = ((int32_t)(((double)sysml_state_seed) / 4)) - sysml_conditional_16;
        if ((((double)sysml_state_seed) / 4) < ((double)((int32_t)(((double)sysml_state_seed) / 4)))) {
            sysml_conditional_18 = 1;
        }
        else {
            sysml_conditional_18 = 0;
        }
        sysml_arithmetic_19 = ((int32_t)(((double)sysml_state_seed) / 4)) - sysml_conditional_18;
        if ((((double)(sysml_state_seed - (4 * sysml_arithmetic_17))) / 1) < ((double)((int32_t)(((double)(sysml_state_seed - (4 * sysml_arithmetic_19))) / 1)))) {
            sysml_conditional_20 = 1;
        }
        else {
            sysml_conditional_20 = 0;
        }
        if ((((double)(((int32_t)(((double)(sysml_state_seed - (4 * sysml_arithmetic_8))) / 1)) - sysml_conditional_13)) / 2) < ((double)((int32_t)(((double)(((int32_t)(((double)(sysml_state_seed - (4 * sysml_arithmetic_15))) / 1)) - sysml_conditional_20)) / 2)))) {
            sysml_conditional_21 = 1;
        }
        else {
            sysml_conditional_21 = 0;
        }
        sysml_arithmetic_22 = ((int32_t)(((double)(((int32_t)(((double)(sysml_state_seed - (4 * sysml_arithmetic_1))) / 1)) - sysml_conditional_6)) / 2)) - sysml_conditional_21;
        if ((((double)sysml_state_seed) / 4) < ((double)((int32_t)(((double)sysml_state_seed) / 4)))) {
            sysml_conditional_23 = 1;
        }
        else {
            sysml_conditional_23 = 0;
        }
        sysml_arithmetic_24 = ((int32_t)(((double)sysml_state_seed) / 4)) - sysml_conditional_23;
        if ((((double)sysml_state_seed) / 4) < ((double)((int32_t)(((double)sysml_state_seed) / 4)))) {
            sysml_conditional_25 = 1;
        }
        else {
            sysml_conditional_25 = 0;
        }
        sysml_arithmetic_26 = ((int32_t)(((double)sysml_state_seed) / 4)) - sysml_conditional_25;
        if ((((double)sysml_state_seed) / 4) < ((double)((int32_t)(((double)sysml_state_seed) / 4)))) {
            sysml_conditional_27 = 1;
        }
        else {
            sysml_conditional_27 = 0;
        }
        sysml_arithmetic_28 = ((int32_t)(((double)sysml_state_seed) / 4)) - sysml_conditional_27;
        if ((((double)sysml_state_seed) / 4) < ((double)((int32_t)(((double)sysml_state_seed) / 4)))) {
            sysml_conditional_29 = 1;
        }
        else {
            sysml_conditional_29 = 0;
        }
        sysml_arithmetic_30 = ((int32_t)(((double)sysml_state_seed) / 4)) - sysml_conditional_29;
        if ((((double)(sysml_state_seed - (4 * sysml_arithmetic_28))) / 2) < ((double)((int32_t)(((double)(sysml_state_seed - (4 * sysml_arithmetic_30))) / 2)))) {
            sysml_conditional_31 = 1;
        }
        else {
            sysml_conditional_31 = 0;
        }
        sysml_arithmetic_32 = ((int32_t)(((double)(sysml_state_seed - (4 * sysml_arithmetic_26))) / 2)) - sysml_conditional_31;
        sysml_state_ledCount = ((2 * sysml_arithmetic_22) + (sysml_state_seed - (4 * sysml_arithmetic_24))) - (2 * sysml_arithmetic_32);
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = (sysml_state_ledCount >= 1);
    bool sysml_value_1 = 0;
    sysml_value_1 = (sysml_state_ledCount >= 2);
    bool sysml_value_2 = 0;
    sysml_value_2 = (sysml_state_ledCount >= 3);
    int32_t sysml_value_3 = 0;
    sysml_value_3 = sysml_state_ledCount;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
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
static int32_t hw_value_io_4 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, OUTPUT);
  digitalWrite(2, hw_value_io_0 ? HIGH : LOW);
  pinMode(3, OUTPUT);
  digitalWrite(3, hw_value_io_1 ? HIGH : LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, hw_value_io_2 ? HIGH : LOW);
  pinMode(A0, OUTPUT); digitalWrite(A0, LOW);
  pinMode(A2, OUTPUT); digitalWrite(A2, LOW);
  pinMode(A1, OUTPUT); digitalWrite(A1, LOW);
  if (hw_value_io_3 != hw_shift595_previous_io_3) {
    hw_shift595_io_3(hw_value_io_3); hw_shift595_previous_io_3 = hw_value_io_3;
  }
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_4 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_4, &hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3);
  digitalWrite(2, hw_value_io_0 ? HIGH : LOW);
  digitalWrite(3, hw_value_io_1 ? HIGH : LOW);
  digitalWrite(4, hw_value_io_2 ? HIGH : LOW);
  if (hw_value_io_3 != hw_shift595_previous_io_3) {
    hw_shift595_io_3(hw_value_io_3); hw_shift595_previous_io_3 = hw_value_io_3;
  }
}

