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
extern bool sysml_state_prevBtn1Pressed;
/* Global Variable define of  */
extern bool sysml_state_prevBtn2Pressed;
/* Global Variable define of  */
extern bool sysml_state_prevBtn3Pressed;
/* Global Variable define of  */
extern bool sysml_state_btn1Stable;
/* Global Variable define of  */
extern bool sysml_state_btn2Stable;
/* Global Variable define of  */
extern bool sysml_state_btn3Stable;
/* Global Variable define of  */
extern int32_t sysml_state_btn1ConfirmCounter;
/* Global Variable define of  */
extern int32_t sysml_state_btn2ConfirmCounter;
/* Global Variable define of  */
extern int32_t sysml_state_btn3ConfirmCounter;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, int32_t* io_3, bool* io_4_segment_0, bool* io_4_segment_1, bool* io_4_segment_2, bool* io_4_segment_3, bool* io_4_segment_4, bool* io_4_segment_5, bool* io_4_segment_6, bool* io_4_segment_7, bool* io_4_segment_8, bool* io_4_segment_9);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_currentNumber = 0;
/* Global Variable define of  */
bool sysml_state_prevBtn1Pressed = false;
/* Global Variable define of  */
bool sysml_state_prevBtn2Pressed = false;
/* Global Variable define of  */
bool sysml_state_prevBtn3Pressed = false;
/* Global Variable define of  */
bool sysml_state_btn1Stable = false;
/* Global Variable define of  */
bool sysml_state_btn2Stable = false;
/* Global Variable define of  */
bool sysml_state_btn3Stable = false;
/* Global Variable define of  */
int32_t sysml_state_btn1ConfirmCounter = 0;
/* Global Variable define of  */
int32_t sysml_state_btn2ConfirmCounter = 0;
/* Global Variable define of  */
int32_t sysml_state_btn3ConfirmCounter = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_currentNumber = 0;
    sysml_state_prevBtn1Pressed = false;
    sysml_state_prevBtn2Pressed = false;
    sysml_state_prevBtn3Pressed = false;
    sysml_state_btn1Stable = false;
    sysml_state_btn2Stable = false;
    sysml_state_btn3Stable = false;
    sysml_state_btn1ConfirmCounter = 0;
    sysml_state_btn2ConfirmCounter = 0;
    sysml_state_btn3ConfirmCounter = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, int32_t* io_3, bool* io_4_segment_0, bool* io_4_segment_1, bool* io_4_segment_2, bool* io_4_segment_3, bool* io_4_segment_4, bool* io_4_segment_5, bool* io_4_segment_6, bool* io_4_segment_7, bool* io_4_segment_8, bool* io_4_segment_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    bool sysml_lazy_9 = 0;
    int32_t sysml_conditional_10 = 0;
    bool sysml_lazy_11 = 0;
    bool sysml_lazy_12 = 0;
    int32_t sysml_conditional_13 = 0;
    int32_t sysml_conditional_14 = 0;
    int32_t sysml_arithmetic_15 = 0;
    bool sysml_lazy_16 = 0;
    int32_t sysml_conditional_17 = 0;
    bool sysml_conditional_18 = 0;
    int32_t sysml_conditional_19 = 0;
    bool sysml_conditional_20 = 0;
    int32_t sysml_conditional_21 = 0;
    bool sysml_conditional_22 = 0;
    int32_t sysml_conditional_23 = 0;
    bool sysml_conditional_24 = 0;
    int32_t sysml_conditional_25 = 0;
    bool sysml_conditional_26 = 0;
    int32_t sysml_conditional_27 = 0;
    bool sysml_conditional_28 = 0;
    int32_t sysml_conditional_29 = 0;
    bool sysml_conditional_30 = 0;
    int32_t sysml_conditional_31 = 0;
    bool sysml_conditional_32 = 0;
    int32_t sysml_conditional_33 = 0;
    bool sysml_conditional_34 = 0;
    int32_t sysml_conditional_35 = 0;
    bool sysml_conditional_36 = 0;
    sysml_lazy_0 = io_2;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_prevBtn3Pressed;
    }
    if (sysml_lazy_0) {
        sysml_state_btn3ConfirmCounter = 1;
    }
    if ((!io_2)) {
        sysml_state_btn3ConfirmCounter = 0;
        sysml_state_btn3Stable = false;
    }
    sysml_lazy_1 = io_2;
    if (sysml_lazy_1) {
        sysml_lazy_1 = sysml_state_btn3ConfirmCounter < 3;
    }
    if (sysml_lazy_1) {
        sysml_state_btn3ConfirmCounter = (sysml_state_btn3ConfirmCounter + 1);
    }
    sysml_lazy_2 = io_2;
    if (sysml_lazy_2) {
        sysml_lazy_2 = sysml_state_btn3ConfirmCounter >= 3;
    }
    if (sysml_lazy_2) {
        sysml_state_btn3Stable = true;
    }
    if (sysml_state_btn3Stable) {
        sysml_state_currentNumber = 0;
    }
    sysml_lazy_3 = io_0;
    if (sysml_lazy_3) {
        sysml_lazy_3 = !sysml_state_prevBtn1Pressed;
    }
    if (sysml_lazy_3) {
        sysml_state_btn1ConfirmCounter = 1;
    }
    if ((!io_0)) {
        sysml_state_btn1ConfirmCounter = 0;
        sysml_state_btn1Stable = false;
    }
    sysml_lazy_4 = io_0;
    if (sysml_lazy_4) {
        sysml_lazy_4 = sysml_state_btn1ConfirmCounter < 3;
    }
    if (sysml_lazy_4) {
        sysml_state_btn1ConfirmCounter = (sysml_state_btn1ConfirmCounter + 1);
    }
    sysml_lazy_5 = io_0;
    if (sysml_lazy_5) {
        sysml_lazy_5 = sysml_state_btn1ConfirmCounter >= 3;
    }
    if (sysml_lazy_5) {
        sysml_state_btn1Stable = true;
    }
    sysml_lazy_6 = io_1;
    if (sysml_lazy_6) {
        sysml_lazy_6 = !sysml_state_prevBtn2Pressed;
    }
    if (sysml_lazy_6) {
        sysml_state_btn2ConfirmCounter = 1;
    }
    if ((!io_1)) {
        sysml_state_btn2ConfirmCounter = 0;
        sysml_state_btn2Stable = false;
    }
    sysml_lazy_7 = io_1;
    if (sysml_lazy_7) {
        sysml_lazy_7 = sysml_state_btn2ConfirmCounter < 3;
    }
    if (sysml_lazy_7) {
        sysml_state_btn2ConfirmCounter = (sysml_state_btn2ConfirmCounter + 1);
    }
    sysml_lazy_8 = io_1;
    if (sysml_lazy_8) {
        sysml_lazy_8 = sysml_state_btn2ConfirmCounter >= 3;
    }
    if (sysml_lazy_8) {
        sysml_state_btn2Stable = true;
    }
    sysml_lazy_9 = !sysml_state_btn3Stable;
    if (sysml_lazy_9) {
        sysml_lazy_9 = sysml_state_btn1Stable;
    }
    if (sysml_lazy_9) {
        if (sysml_state_currentNumber >= 99) {
            sysml_conditional_10 = 0;
        }
        else {
            sysml_conditional_10 = sysml_state_currentNumber + 1;
        }
        sysml_state_currentNumber = sysml_conditional_10;
    }
    sysml_lazy_11 = !sysml_state_btn3Stable;
    if (sysml_lazy_11) {
        sysml_lazy_11 = !sysml_state_btn1Stable;
    }
    sysml_lazy_12 = sysml_lazy_11;
    if (sysml_lazy_12) {
        sysml_lazy_12 = sysml_state_btn2Stable;
    }
    if (sysml_lazy_12) {
        if (sysml_state_currentNumber >= 95) {
            sysml_conditional_13 = 0;
        }
        else {
            sysml_conditional_13 = sysml_state_currentNumber + 5;
        }
        sysml_state_currentNumber = sysml_conditional_13;
    }
    sysml_state_prevBtn1Pressed = io_0;
    sysml_state_prevBtn2Pressed = io_1;
    sysml_state_prevBtn3Pressed = io_2;
    int32_t sysml_value_0 = 0;
    sysml_value_0 = sysml_state_currentNumber;
    bool sysml_value_1 = 0;
    if ((((double)sysml_state_currentNumber) / 10) < ((double)((int32_t)(((double)sysml_state_currentNumber) / 10)))) {
        sysml_conditional_14 = 1;
    }
    else {
        sysml_conditional_14 = 0;
    }
    sysml_arithmetic_15 = ((int32_t)(((double)sysml_state_currentNumber) / 10)) - sysml_conditional_14;
    sysml_lazy_16 = sysml_state_currentNumber == (10 * sysml_arithmetic_15);
    if (sysml_lazy_16) {
        if ((((double)sysml_state_currentNumber) / 10) < ((double)((int32_t)(((double)sysml_state_currentNumber) / 10)))) {
            sysml_conditional_17 = 1;
        }
        else {
            sysml_conditional_17 = 0;
        }
        sysml_lazy_16 = (((int32_t)(((double)sysml_state_currentNumber) / 10)) - sysml_conditional_17) >= 1;
    }
    if (sysml_lazy_16) {
        sysml_conditional_18 = true;
    }
    else {
        sysml_conditional_18 = false;
    }
    sysml_value_1 = sysml_conditional_18;
    bool sysml_value_2 = 0;
    if ((((double)sysml_state_currentNumber) / 10) < ((double)((int32_t)(((double)sysml_state_currentNumber) / 10)))) {
        sysml_conditional_19 = 1;
    }
    else {
        sysml_conditional_19 = 0;
    }
    if ((((int32_t)(((double)sysml_state_currentNumber) / 10)) - sysml_conditional_19) >= 2) {
        sysml_conditional_20 = true;
    }
    else {
        sysml_conditional_20 = false;
    }
    sysml_value_2 = sysml_conditional_20;
    bool sysml_value_3 = 0;
    if ((((double)sysml_state_currentNumber) / 10) < ((double)((int32_t)(((double)sysml_state_currentNumber) / 10)))) {
        sysml_conditional_21 = 1;
    }
    else {
        sysml_conditional_21 = 0;
    }
    if ((((int32_t)(((double)sysml_state_currentNumber) / 10)) - sysml_conditional_21) >= 3) {
        sysml_conditional_22 = true;
    }
    else {
        sysml_conditional_22 = false;
    }
    sysml_value_3 = sysml_conditional_22;
    bool sysml_value_4 = 0;
    if ((((double)sysml_state_currentNumber) / 10) < ((double)((int32_t)(((double)sysml_state_currentNumber) / 10)))) {
        sysml_conditional_23 = 1;
    }
    else {
        sysml_conditional_23 = 0;
    }
    if ((((int32_t)(((double)sysml_state_currentNumber) / 10)) - sysml_conditional_23) >= 4) {
        sysml_conditional_24 = true;
    }
    else {
        sysml_conditional_24 = false;
    }
    sysml_value_4 = sysml_conditional_24;
    bool sysml_value_5 = 0;
    if ((((double)sysml_state_currentNumber) / 10) < ((double)((int32_t)(((double)sysml_state_currentNumber) / 10)))) {
        sysml_conditional_25 = 1;
    }
    else {
        sysml_conditional_25 = 0;
    }
    if ((((int32_t)(((double)sysml_state_currentNumber) / 10)) - sysml_conditional_25) >= 5) {
        sysml_conditional_26 = true;
    }
    else {
        sysml_conditional_26 = false;
    }
    sysml_value_5 = sysml_conditional_26;
    bool sysml_value_6 = 0;
    if ((((double)sysml_state_currentNumber) / 10) < ((double)((int32_t)(((double)sysml_state_currentNumber) / 10)))) {
        sysml_conditional_27 = 1;
    }
    else {
        sysml_conditional_27 = 0;
    }
    if ((((int32_t)(((double)sysml_state_currentNumber) / 10)) - sysml_conditional_27) >= 6) {
        sysml_conditional_28 = true;
    }
    else {
        sysml_conditional_28 = false;
    }
    sysml_value_6 = sysml_conditional_28;
    bool sysml_value_7 = 0;
    if ((((double)sysml_state_currentNumber) / 10) < ((double)((int32_t)(((double)sysml_state_currentNumber) / 10)))) {
        sysml_conditional_29 = 1;
    }
    else {
        sysml_conditional_29 = 0;
    }
    if ((((int32_t)(((double)sysml_state_currentNumber) / 10)) - sysml_conditional_29) >= 7) {
        sysml_conditional_30 = true;
    }
    else {
        sysml_conditional_30 = false;
    }
    sysml_value_7 = sysml_conditional_30;
    bool sysml_value_8 = 0;
    if ((((double)sysml_state_currentNumber) / 10) < ((double)((int32_t)(((double)sysml_state_currentNumber) / 10)))) {
        sysml_conditional_31 = 1;
    }
    else {
        sysml_conditional_31 = 0;
    }
    if ((((int32_t)(((double)sysml_state_currentNumber) / 10)) - sysml_conditional_31) >= 8) {
        sysml_conditional_32 = true;
    }
    else {
        sysml_conditional_32 = false;
    }
    sysml_value_8 = sysml_conditional_32;
    bool sysml_value_9 = 0;
    if ((((double)sysml_state_currentNumber) / 10) < ((double)((int32_t)(((double)sysml_state_currentNumber) / 10)))) {
        sysml_conditional_33 = 1;
    }
    else {
        sysml_conditional_33 = 0;
    }
    if ((((int32_t)(((double)sysml_state_currentNumber) / 10)) - sysml_conditional_33) >= 9) {
        sysml_conditional_34 = true;
    }
    else {
        sysml_conditional_34 = false;
    }
    sysml_value_9 = sysml_conditional_34;
    bool sysml_value_10 = 0;
    if ((((double)sysml_state_currentNumber) / 10) < ((double)((int32_t)(((double)sysml_state_currentNumber) / 10)))) {
        sysml_conditional_35 = 1;
    }
    else {
        sysml_conditional_35 = 0;
    }
    if ((((int32_t)(((double)sysml_state_currentNumber) / 10)) - sysml_conditional_35) >= 10) {
        sysml_conditional_36 = true;
    }
    else {
        sysml_conditional_36 = false;
    }
    sysml_value_10 = sysml_conditional_36;
    *io_3 = sysml_value_0;
    *io_4_segment_0 = sysml_value_1;
    *io_4_segment_1 = sysml_value_2;
    *io_4_segment_2 = sysml_value_3;
    *io_4_segment_3 = sysml_value_4;
    *io_4_segment_4 = sysml_value_5;
    *io_4_segment_5 = sysml_value_6;
    *io_4_segment_6 = sysml_value_7;
    *io_4_segment_7 = sysml_value_8;
    *io_4_segment_8 = sysml_value_9;
    *io_4_segment_9 = sysml_value_10;
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
static uint32_t hw_sampled_io_0 = 0;
static bool hw_attempted_io_0 = false;
static bool hw_value_io_1 = false;
static uint32_t hw_sampled_io_1 = 0;
static bool hw_attempted_io_1 = false;
static bool hw_value_io_2 = false;
static uint32_t hw_sampled_io_2 = 0;
static bool hw_attempted_io_2 = false;
static int32_t hw_value_io_3 = 0;
static void hw_shift595_io_3(int32_t value) {
  static const uint8_t digits[10] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
  bool valid = value >= 0 && value < 100L;
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
  {
    uint8_t segments = valid && (true) ? digits[(value / 10L) % 10] : 0;
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

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
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
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 5UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (digitalRead(2) == LOW);
  }
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 5UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = (digitalRead(3) == LOW);
  }
  if (!hw_attempted_io_2 || (uint32_t)(hw_now - hw_sampled_io_2) >= 5UL) {
    hw_attempted_io_2 = true; hw_sampled_io_2 = hw_now;
    hw_value_io_2 = (digitalRead(4) == LOW);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, &hw_value_io_3, &hw_value_io_4_segment_0, &hw_value_io_4_segment_1, &hw_value_io_4_segment_2, &hw_value_io_4_segment_3, &hw_value_io_4_segment_4, &hw_value_io_4_segment_5, &hw_value_io_4_segment_6, &hw_value_io_4_segment_7, &hw_value_io_4_segment_8, &hw_value_io_4_segment_9);
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
}
