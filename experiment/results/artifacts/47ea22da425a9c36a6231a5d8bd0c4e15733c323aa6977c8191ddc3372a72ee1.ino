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
extern int32_t sysml_state_accumMs;
/* Global Variable define of  */
extern int32_t sysml_state_displayNumber;
/* Global Variable define of  */
extern bool sysml_state_ledPhase;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_0, int32_t* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7, bool* io_8, bool* io_9, bool* io_10, bool* io_11, bool* io_12, bool* io_13, bool* io_14);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_accumMs = 0;
/* Global Variable define of  */
int32_t sysml_state_displayNumber = 0;
/* Global Variable define of  */
bool sysml_state_ledPhase = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_accumMs = 0;
    sysml_state_displayNumber = 0;
    sysml_state_ledPhase = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_0, int32_t* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7, bool* io_8, bool* io_9, bool* io_10, bool* io_11, bool* io_12, bool* io_13, bool* io_14) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int32_t sysml_conditional_0 = 0;
    int32_t sysml_conditional_1 = 0;
    bool sysml_conditional_2 = 0;
    int32_t sysml_conditional_3 = 0;
    bool sysml_conditional_4 = 0;
    int32_t sysml_conditional_5 = 0;
    bool sysml_conditional_6 = 0;
    int32_t sysml_conditional_7 = 0;
    bool sysml_conditional_8 = 0;
    int32_t sysml_conditional_9 = 0;
    bool sysml_conditional_10 = 0;
    int32_t sysml_conditional_11 = 0;
    bool sysml_conditional_12 = 0;
    int32_t sysml_conditional_13 = 0;
    bool sysml_conditional_14 = 0;
    int32_t sysml_conditional_15 = 0;
    bool sysml_conditional_16 = 0;
    int32_t sysml_conditional_17 = 0;
    bool sysml_conditional_18 = 0;
    int32_t sysml_conditional_19 = 0;
    bool sysml_conditional_20 = 0;
    bool sysml_conditional_21 = 0;
    sysml_state_accumMs = (sysml_state_accumMs + io_0);
    if ((sysml_state_accumMs >= 2000)) {
        sysml_state_accumMs = (sysml_state_accumMs - 2000);
        if (sysml_state_displayNumber >= 99) {
            sysml_conditional_0 = 0;
        }
        else {
            sysml_conditional_0 = sysml_state_displayNumber + 1;
        }
        sysml_state_displayNumber = sysml_conditional_0;
        sysml_state_ledPhase = (!sysml_state_ledPhase);
    }
    int32_t sysml_value_0 = 0;
    sysml_value_0 = sysml_state_displayNumber;
    bool sysml_value_1 = 0;
    if ((((double)sysml_state_displayNumber) / 10) < ((double)((int32_t)(((double)sysml_state_displayNumber) / 10)))) {
        sysml_conditional_1 = 1;
    }
    else {
        sysml_conditional_1 = 0;
    }
    if (((((int32_t)(((double)sysml_state_displayNumber) / 10)) - sysml_conditional_1) + 1) >= 1) {
        sysml_conditional_2 = true;
    }
    else {
        sysml_conditional_2 = false;
    }
    sysml_value_1 = sysml_conditional_2;
    bool sysml_value_2 = 0;
    if ((((double)sysml_state_displayNumber) / 10) < ((double)((int32_t)(((double)sysml_state_displayNumber) / 10)))) {
        sysml_conditional_3 = 1;
    }
    else {
        sysml_conditional_3 = 0;
    }
    if (((((int32_t)(((double)sysml_state_displayNumber) / 10)) - sysml_conditional_3) + 1) >= 2) {
        sysml_conditional_4 = true;
    }
    else {
        sysml_conditional_4 = false;
    }
    sysml_value_2 = sysml_conditional_4;
    bool sysml_value_3 = 0;
    if ((((double)sysml_state_displayNumber) / 10) < ((double)((int32_t)(((double)sysml_state_displayNumber) / 10)))) {
        sysml_conditional_5 = 1;
    }
    else {
        sysml_conditional_5 = 0;
    }
    if (((((int32_t)(((double)sysml_state_displayNumber) / 10)) - sysml_conditional_5) + 1) >= 3) {
        sysml_conditional_6 = true;
    }
    else {
        sysml_conditional_6 = false;
    }
    sysml_value_3 = sysml_conditional_6;
    bool sysml_value_4 = 0;
    if ((((double)sysml_state_displayNumber) / 10) < ((double)((int32_t)(((double)sysml_state_displayNumber) / 10)))) {
        sysml_conditional_7 = 1;
    }
    else {
        sysml_conditional_7 = 0;
    }
    if (((((int32_t)(((double)sysml_state_displayNumber) / 10)) - sysml_conditional_7) + 1) >= 4) {
        sysml_conditional_8 = true;
    }
    else {
        sysml_conditional_8 = false;
    }
    sysml_value_4 = sysml_conditional_8;
    bool sysml_value_5 = 0;
    if ((((double)sysml_state_displayNumber) / 10) < ((double)((int32_t)(((double)sysml_state_displayNumber) / 10)))) {
        sysml_conditional_9 = 1;
    }
    else {
        sysml_conditional_9 = 0;
    }
    if (((((int32_t)(((double)sysml_state_displayNumber) / 10)) - sysml_conditional_9) + 1) >= 5) {
        sysml_conditional_10 = true;
    }
    else {
        sysml_conditional_10 = false;
    }
    sysml_value_5 = sysml_conditional_10;
    bool sysml_value_6 = 0;
    if ((((double)sysml_state_displayNumber) / 10) < ((double)((int32_t)(((double)sysml_state_displayNumber) / 10)))) {
        sysml_conditional_11 = 1;
    }
    else {
        sysml_conditional_11 = 0;
    }
    if (((((int32_t)(((double)sysml_state_displayNumber) / 10)) - sysml_conditional_11) + 1) >= 6) {
        sysml_conditional_12 = true;
    }
    else {
        sysml_conditional_12 = false;
    }
    sysml_value_6 = sysml_conditional_12;
    bool sysml_value_7 = 0;
    if ((((double)sysml_state_displayNumber) / 10) < ((double)((int32_t)(((double)sysml_state_displayNumber) / 10)))) {
        sysml_conditional_13 = 1;
    }
    else {
        sysml_conditional_13 = 0;
    }
    if (((((int32_t)(((double)sysml_state_displayNumber) / 10)) - sysml_conditional_13) + 1) >= 7) {
        sysml_conditional_14 = true;
    }
    else {
        sysml_conditional_14 = false;
    }
    sysml_value_7 = sysml_conditional_14;
    bool sysml_value_8 = 0;
    if ((((double)sysml_state_displayNumber) / 10) < ((double)((int32_t)(((double)sysml_state_displayNumber) / 10)))) {
        sysml_conditional_15 = 1;
    }
    else {
        sysml_conditional_15 = 0;
    }
    if (((((int32_t)(((double)sysml_state_displayNumber) / 10)) - sysml_conditional_15) + 1) >= 8) {
        sysml_conditional_16 = true;
    }
    else {
        sysml_conditional_16 = false;
    }
    sysml_value_8 = sysml_conditional_16;
    bool sysml_value_9 = 0;
    if ((((double)sysml_state_displayNumber) / 10) < ((double)((int32_t)(((double)sysml_state_displayNumber) / 10)))) {
        sysml_conditional_17 = 1;
    }
    else {
        sysml_conditional_17 = 0;
    }
    if (((((int32_t)(((double)sysml_state_displayNumber) / 10)) - sysml_conditional_17) + 1) >= 9) {
        sysml_conditional_18 = true;
    }
    else {
        sysml_conditional_18 = false;
    }
    sysml_value_9 = sysml_conditional_18;
    bool sysml_value_10 = 0;
    if ((((double)sysml_state_displayNumber) / 10) < ((double)((int32_t)(((double)sysml_state_displayNumber) / 10)))) {
        sysml_conditional_19 = 1;
    }
    else {
        sysml_conditional_19 = 0;
    }
    if (((((int32_t)(((double)sysml_state_displayNumber) / 10)) - sysml_conditional_19) + 1) >= 10) {
        sysml_conditional_20 = true;
    }
    else {
        sysml_conditional_20 = false;
    }
    sysml_value_10 = sysml_conditional_20;
    bool sysml_value_11 = 0;
    sysml_value_11 = sysml_state_ledPhase;
    bool sysml_value_12 = 0;
    if (sysml_state_ledPhase) {
        sysml_conditional_21 = false;
    }
    else {
        sysml_conditional_21 = true;
    }
    sysml_value_12 = sysml_conditional_21;
    bool sysml_value_13 = 0;
    sysml_value_13 = false;
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
    *io_3 = sysml_value_2;
    *io_4 = sysml_value_3;
    *io_5 = sysml_value_4;
    *io_6 = sysml_value_5;
    *io_7 = sysml_value_6;
    *io_8 = sysml_value_7;
    *io_9 = sysml_value_8;
    *io_10 = sysml_value_9;
    *io_11 = sysml_value_10;
    *io_12 = sysml_value_11;
    *io_13 = sysml_value_12;
    *io_14 = sysml_value_13;
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
static int32_t hw_value_io_0 = 0;
static int32_t hw_value_io_1 = 0;
static void hw_shift595_io_1(int32_t value) {
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
static int32_t hw_shift595_previous_io_1 = INT32_MIN;
static bool hw_value_io_2 = false;
static bool hw_value_io_3 = false;
static bool hw_value_io_4 = false;
static bool hw_value_io_5 = false;
static bool hw_value_io_6 = false;
static bool hw_value_io_7 = false;
static bool hw_value_io_8 = false;
static bool hw_value_io_9 = false;
static bool hw_value_io_10 = false;
static bool hw_value_io_11 = false;
static bool hw_value_io_12 = false;
static bool hw_value_io_13 = false;
static bool hw_value_io_14 = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(A0, OUTPUT); digitalWrite(A0, LOW);
  pinMode(A2, OUTPUT); digitalWrite(A2, LOW);
  pinMode(A1, OUTPUT); digitalWrite(A1, LOW);
  if (hw_value_io_1 != hw_shift595_previous_io_1) {
    hw_shift595_io_1(hw_value_io_1); hw_shift595_previous_io_1 = hw_value_io_1;
  }
  pinMode(A10, OUTPUT);
  digitalWrite(A10, hw_value_io_2 ? HIGH : LOW);
  pinMode(2, OUTPUT);
  digitalWrite(2, hw_value_io_3 ? HIGH : LOW);
  pinMode(3, OUTPUT);
  digitalWrite(3, hw_value_io_4 ? HIGH : LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, hw_value_io_5 ? HIGH : LOW);
  pinMode(5, OUTPUT);
  digitalWrite(5, hw_value_io_6 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  digitalWrite(6, hw_value_io_7 ? HIGH : LOW);
  pinMode(7, OUTPUT);
  digitalWrite(7, hw_value_io_8 ? HIGH : LOW);
  pinMode(8, OUTPUT);
  digitalWrite(8, hw_value_io_9 ? HIGH : LOW);
  pinMode(9, OUTPUT);
  digitalWrite(9, hw_value_io_10 ? HIGH : LOW);
  pinMode(10, OUTPUT);
  digitalWrite(10, hw_value_io_11 ? HIGH : LOW);
  pinMode(11, OUTPUT);
  digitalWrite(11, hw_value_io_12 ? HIGH : LOW);
  pinMode(12, OUTPUT);
  digitalWrite(12, hw_value_io_13 ? HIGH : LOW);
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_14 ? HIGH : LOW);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_0 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_io_7, &hw_value_io_8, &hw_value_io_9, &hw_value_io_10, &hw_value_io_11, &hw_value_io_12, &hw_value_io_13, &hw_value_io_14);
  if (hw_value_io_1 != hw_shift595_previous_io_1) {
    hw_shift595_io_1(hw_value_io_1); hw_shift595_previous_io_1 = hw_value_io_1;
  }
  digitalWrite(A10, hw_value_io_2 ? HIGH : LOW);
  digitalWrite(2, hw_value_io_3 ? HIGH : LOW);
  digitalWrite(3, hw_value_io_4 ? HIGH : LOW);
  digitalWrite(4, hw_value_io_5 ? HIGH : LOW);
  digitalWrite(5, hw_value_io_6 ? HIGH : LOW);
  digitalWrite(6, hw_value_io_7 ? HIGH : LOW);
  digitalWrite(7, hw_value_io_8 ? HIGH : LOW);
  digitalWrite(8, hw_value_io_9 ? HIGH : LOW);
  digitalWrite(9, hw_value_io_10 ? HIGH : LOW);
  digitalWrite(10, hw_value_io_11 ? HIGH : LOW);
  digitalWrite(11, hw_value_io_12 ? HIGH : LOW);
  digitalWrite(12, hw_value_io_13 ? HIGH : LOW);
  digitalWrite(13, hw_value_io_14 ? HIGH : LOW);
}
