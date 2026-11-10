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
extern int32_t sysml_state_currentNumber;
/* Global Variable define of  */
extern int32_t sysml_state_accumulatedMs;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_4, bool* io_0, bool* io_1, int32_t* io_2, bool* io_3_segment_0, bool* io_3_segment_1, bool* io_3_segment_2, bool* io_3_segment_3, bool* io_3_segment_4, bool* io_3_segment_5, bool* io_3_segment_6, bool* io_3_segment_7, bool* io_3_segment_8, bool* io_3_segment_9);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_currentNumber = 0;
/* Global Variable define of  */
int32_t sysml_state_accumulatedMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_currentNumber = 0;
    sysml_state_accumulatedMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_4, bool* io_0, bool* io_1, int32_t* io_2, bool* io_3_segment_0, bool* io_3_segment_1, bool* io_3_segment_2, bool* io_3_segment_3, bool* io_3_segment_4, bool* io_3_segment_5, bool* io_3_segment_6, bool* io_3_segment_7, bool* io_3_segment_8, bool* io_3_segment_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int32_t sysml_conditional_0 = 0;
    int32_t sysml_conditional_1 = 0;
    int32_t sysml_arithmetic_2 = 0;
    bool sysml_conditional_3 = 0;
    int32_t sysml_conditional_4 = 0;
    int32_t sysml_arithmetic_5 = 0;
    bool sysml_conditional_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_conditional_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_conditional_10 = 0;
    bool sysml_lazy_11 = 0;
    bool sysml_conditional_12 = 0;
    bool sysml_lazy_13 = 0;
    bool sysml_conditional_14 = 0;
    bool sysml_lazy_15 = 0;
    bool sysml_conditional_16 = 0;
    bool sysml_lazy_17 = 0;
    bool sysml_conditional_18 = 0;
    bool sysml_lazy_19 = 0;
    bool sysml_conditional_20 = 0;
    bool sysml_lazy_21 = 0;
    bool sysml_conditional_22 = 0;
    bool sysml_lazy_23 = 0;
    bool sysml_conditional_24 = 0;
    bool sysml_lazy_25 = 0;
    bool sysml_conditional_26 = 0;
    if ((sysml_state_accumulatedMs >= 2000)) {
        sysml_state_accumulatedMs = (sysml_state_accumulatedMs - 2000);
        if (sysml_state_currentNumber == 9) {
            sysml_conditional_0 = 0;
        }
        else {
            sysml_conditional_0 = sysml_state_currentNumber + 1;
        }
        sysml_state_currentNumber = sysml_conditional_0;
    }
    sysml_state_accumulatedMs = (sysml_state_accumulatedMs + io_4);
    bool sysml_value_0 = 0;
    if ((((double)sysml_state_currentNumber) / 2) < ((double)((int32_t)(((double)sysml_state_currentNumber) / 2)))) {
        sysml_conditional_1 = 1;
    }
    else {
        sysml_conditional_1 = 0;
    }
    sysml_arithmetic_2 = ((int32_t)(((double)sysml_state_currentNumber) / 2)) - sysml_conditional_1;
    if (sysml_state_currentNumber == (2 * sysml_arithmetic_2)) {
        sysml_conditional_3 = true;
    }
    else {
        sysml_conditional_3 = false;
    }
    sysml_value_0 = sysml_conditional_3;
    bool sysml_value_1 = 0;
    if ((((double)sysml_state_currentNumber) / 2) < ((double)((int32_t)(((double)sysml_state_currentNumber) / 2)))) {
        sysml_conditional_4 = 1;
    }
    else {
        sysml_conditional_4 = 0;
    }
    sysml_arithmetic_5 = ((int32_t)(((double)sysml_state_currentNumber) / 2)) - sysml_conditional_4;
    if (sysml_state_currentNumber != (2 * sysml_arithmetic_5)) {
        sysml_conditional_6 = true;
    }
    else {
        sysml_conditional_6 = false;
    }
    sysml_value_1 = sysml_conditional_6;
    int32_t sysml_value_2 = 0;
    sysml_value_2 = sysml_state_currentNumber;
    bool sysml_value_3 = 0;
    sysml_lazy_7 = sysml_state_currentNumber >= 1;
    if (sysml_lazy_7) {
        sysml_lazy_7 = sysml_state_currentNumber <= 1;
    }
    if (sysml_lazy_7) {
        sysml_conditional_8 = true;
    }
    else {
        sysml_conditional_8 = false;
    }
    sysml_value_3 = sysml_conditional_8;
    bool sysml_value_4 = 0;
    sysml_lazy_9 = sysml_state_currentNumber >= 2;
    if (sysml_lazy_9) {
        sysml_lazy_9 = sysml_state_currentNumber <= 2;
    }
    if (sysml_lazy_9) {
        sysml_conditional_10 = true;
    }
    else {
        sysml_conditional_10 = false;
    }
    sysml_value_4 = sysml_conditional_10;
    bool sysml_value_5 = 0;
    sysml_lazy_11 = sysml_state_currentNumber >= 3;
    if (sysml_lazy_11) {
        sysml_lazy_11 = sysml_state_currentNumber <= 3;
    }
    if (sysml_lazy_11) {
        sysml_conditional_12 = true;
    }
    else {
        sysml_conditional_12 = false;
    }
    sysml_value_5 = sysml_conditional_12;
    bool sysml_value_6 = 0;
    sysml_lazy_13 = sysml_state_currentNumber >= 4;
    if (sysml_lazy_13) {
        sysml_lazy_13 = sysml_state_currentNumber <= 4;
    }
    if (sysml_lazy_13) {
        sysml_conditional_14 = true;
    }
    else {
        sysml_conditional_14 = false;
    }
    sysml_value_6 = sysml_conditional_14;
    bool sysml_value_7 = 0;
    sysml_lazy_15 = sysml_state_currentNumber >= 5;
    if (sysml_lazy_15) {
        sysml_lazy_15 = sysml_state_currentNumber <= 5;
    }
    if (sysml_lazy_15) {
        sysml_conditional_16 = true;
    }
    else {
        sysml_conditional_16 = false;
    }
    sysml_value_7 = sysml_conditional_16;
    bool sysml_value_8 = 0;
    sysml_lazy_17 = sysml_state_currentNumber >= 6;
    if (sysml_lazy_17) {
        sysml_lazy_17 = sysml_state_currentNumber <= 6;
    }
    if (sysml_lazy_17) {
        sysml_conditional_18 = true;
    }
    else {
        sysml_conditional_18 = false;
    }
    sysml_value_8 = sysml_conditional_18;
    bool sysml_value_9 = 0;
    sysml_lazy_19 = sysml_state_currentNumber >= 7;
    if (sysml_lazy_19) {
        sysml_lazy_19 = sysml_state_currentNumber <= 7;
    }
    if (sysml_lazy_19) {
        sysml_conditional_20 = true;
    }
    else {
        sysml_conditional_20 = false;
    }
    sysml_value_9 = sysml_conditional_20;
    bool sysml_value_10 = 0;
    sysml_lazy_21 = sysml_state_currentNumber >= 8;
    if (sysml_lazy_21) {
        sysml_lazy_21 = sysml_state_currentNumber <= 8;
    }
    if (sysml_lazy_21) {
        sysml_conditional_22 = true;
    }
    else {
        sysml_conditional_22 = false;
    }
    sysml_value_10 = sysml_conditional_22;
    bool sysml_value_11 = 0;
    sysml_lazy_23 = sysml_state_currentNumber >= 9;
    if (sysml_lazy_23) {
        sysml_lazy_23 = sysml_state_currentNumber <= 9;
    }
    if (sysml_lazy_23) {
        sysml_conditional_24 = true;
    }
    else {
        sysml_conditional_24 = false;
    }
    sysml_value_11 = sysml_conditional_24;
    bool sysml_value_12 = 0;
    sysml_lazy_25 = sysml_state_currentNumber >= 10;
    if (sysml_lazy_25) {
        sysml_lazy_25 = sysml_state_currentNumber <= 10;
    }
    if (sysml_lazy_25) {
        sysml_conditional_26 = true;
    }
    else {
        sysml_conditional_26 = false;
    }
    sysml_value_12 = sysml_conditional_26;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3_segment_0 = sysml_value_3;
    *io_3_segment_1 = sysml_value_4;
    *io_3_segment_2 = sysml_value_5;
    *io_3_segment_3 = sysml_value_6;
    *io_3_segment_4 = sysml_value_7;
    *io_3_segment_5 = sysml_value_8;
    *io_3_segment_6 = sysml_value_9;
    *io_3_segment_7 = sysml_value_10;
    *io_3_segment_8 = sysml_value_11;
    *io_3_segment_9 = sysml_value_12;
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
static int32_t hw_value_io_2 = 0;
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
static bool hw_value_io_3_segment_0 = false;
static bool hw_value_io_3_segment_1 = false;
static bool hw_value_io_3_segment_2 = false;
static bool hw_value_io_3_segment_3 = false;
static bool hw_value_io_3_segment_4 = false;
static bool hw_value_io_3_segment_5 = false;
static bool hw_value_io_3_segment_6 = false;
static bool hw_value_io_3_segment_7 = false;
static bool hw_value_io_3_segment_8 = false;
static bool hw_value_io_3_segment_9 = false;
static int32_t hw_value_io_4 = 0;

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
  pinMode(4, OUTPUT);
  digitalWrite(4, hw_value_io_3_segment_0 ? HIGH : LOW);
  pinMode(5, OUTPUT);
  digitalWrite(5, hw_value_io_3_segment_1 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  digitalWrite(6, hw_value_io_3_segment_2 ? HIGH : LOW);
  pinMode(7, OUTPUT);
  digitalWrite(7, hw_value_io_3_segment_3 ? HIGH : LOW);
  pinMode(8, OUTPUT);
  digitalWrite(8, hw_value_io_3_segment_4 ? HIGH : LOW);
  pinMode(9, OUTPUT);
  digitalWrite(9, hw_value_io_3_segment_5 ? HIGH : LOW);
  pinMode(10, OUTPUT);
  digitalWrite(10, hw_value_io_3_segment_6 ? HIGH : LOW);
  pinMode(11, OUTPUT);
  digitalWrite(11, hw_value_io_3_segment_7 ? HIGH : LOW);
  pinMode(12, OUTPUT);
  digitalWrite(12, hw_value_io_3_segment_8 ? HIGH : LOW);
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_3_segment_9 ? HIGH : LOW);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_4 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_4, &hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3_segment_0, &hw_value_io_3_segment_1, &hw_value_io_3_segment_2, &hw_value_io_3_segment_3, &hw_value_io_3_segment_4, &hw_value_io_3_segment_5, &hw_value_io_3_segment_6, &hw_value_io_3_segment_7, &hw_value_io_3_segment_8, &hw_value_io_3_segment_9);
  digitalWrite(2, hw_value_io_0 ? HIGH : LOW);
  digitalWrite(3, hw_value_io_1 ? HIGH : LOW);
  if (hw_value_io_2 != hw_shift595_previous_io_2) {
    hw_shift595_io_2(hw_value_io_2); hw_shift595_previous_io_2 = hw_value_io_2;
  }
  digitalWrite(4, hw_value_io_3_segment_0 ? HIGH : LOW);
  digitalWrite(5, hw_value_io_3_segment_1 ? HIGH : LOW);
  digitalWrite(6, hw_value_io_3_segment_2 ? HIGH : LOW);
  digitalWrite(7, hw_value_io_3_segment_3 ? HIGH : LOW);
  digitalWrite(8, hw_value_io_3_segment_4 ? HIGH : LOW);
  digitalWrite(9, hw_value_io_3_segment_5 ? HIGH : LOW);
  digitalWrite(10, hw_value_io_3_segment_6 ? HIGH : LOW);
  digitalWrite(11, hw_value_io_3_segment_7 ? HIGH : LOW);
  digitalWrite(12, hw_value_io_3_segment_8 ? HIGH : LOW);
  digitalWrite(13, hw_value_io_3_segment_9 ? HIGH : LOW);
}
