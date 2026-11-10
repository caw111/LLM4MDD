#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_36\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
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
extern int32_t sysml_state_mappedTens;
/* Global Variable define of  */
extern int32_t sysml_state_mappedUnits;
/* Global Variable define of  */
extern int32_t sysml_state_sumDigits;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, int32_t* io_2);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_mappedTens = 0;
/* Global Variable define of  */
int32_t sysml_state_mappedUnits = 0;
/* Global Variable define of  */
int32_t sysml_state_sumDigits = 0;
/* Init function of CompositeActor named "ProgramPage1" */
#line 118 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_36\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
static void hw_shift595_io_2(int32_t value);
#line 139 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_36\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void setup();
#line 151 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_36\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void loop();
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_36\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_mappedTens = 0;
    sysml_state_mappedUnits = 0;
    sysml_state_sumDigits = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, int32_t* io_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int32_t sysml_conditional_0 = 0;
    int32_t sysml_conditional_1 = 0;
    int32_t sysml_conditional_2 = 0;
    int32_t sysml_arithmetic_3 = 0;
    int32_t sysml_conditional_4 = 0;
    int32_t sysml_arithmetic_5 = 0;
    int32_t sysml_conditional_6 = 0;
    int32_t sysml_arithmetic_7 = 0;
    int32_t sysml_conditional_8 = 0;
    if ((((double)(io_0 * 9.0)) / 1023.0) < ((double)((int32_t)(((double)(io_0 * 9.0)) / 1023.0)))) {
        sysml_conditional_0 = 1;
    }
    else {
        sysml_conditional_0 = 0;
    }
    sysml_state_mappedTens = ((int32_t)(((double)(io_0 * 9.0)) / 1023.0)) - sysml_conditional_0;
    if ((((double)(io_1 * 9.0)) / 1023.0) < ((double)((int32_t)(((double)(io_1 * 9.0)) / 1023.0)))) {
        sysml_conditional_1 = 1;
    }
    else {
        sysml_conditional_1 = 0;
    }
    sysml_state_mappedUnits = ((int32_t)(((double)(io_1 * 9.0)) / 1023.0)) - sysml_conditional_1;
    if ((((double)(sysml_state_mappedTens + sysml_state_mappedUnits)) / 10.0) < ((double)((int32_t)(((double)(sysml_state_mappedTens + sysml_state_mappedUnits)) / 10.0)))) {
        sysml_conditional_2 = 1;
    }
    else {
        sysml_conditional_2 = 0;
    }
    sysml_arithmetic_3 = ((int32_t)(((double)(sysml_state_mappedTens + sysml_state_mappedUnits)) / 10.0)) - sysml_conditional_2;
    if ((((double)(sysml_state_mappedTens + sysml_state_mappedUnits)) / 10.0) < ((double)((int32_t)(((double)(sysml_state_mappedTens + sysml_state_mappedUnits)) / 10.0)))) {
        sysml_conditional_4 = 1;
    }
    else {
        sysml_conditional_4 = 0;
    }
    sysml_arithmetic_5 = ((int32_t)(((double)(sysml_state_mappedTens + sysml_state_mappedUnits)) / 10.0)) - sysml_conditional_4;
    if ((((double)(sysml_state_mappedTens + sysml_state_mappedUnits)) / 10.0) < ((double)((int32_t)(((double)(sysml_state_mappedTens + sysml_state_mappedUnits)) / 10.0)))) {
        sysml_conditional_6 = 1;
    }
    else {
        sysml_conditional_6 = 0;
    }
    sysml_arithmetic_7 = ((int32_t)(((double)(sysml_state_mappedTens + sysml_state_mappedUnits)) / 10.0)) - sysml_conditional_6;
    if (((sysml_state_mappedTens + sysml_state_mappedUnits) - (10.0 * sysml_arithmetic_5)) < ((double)((int32_t)((sysml_state_mappedTens + sysml_state_mappedUnits) - (10.0 * sysml_arithmetic_7))))) {
        sysml_conditional_8 = 1;
    }
    else {
        sysml_conditional_8 = 0;
    }
    sysml_state_sumDigits = ((int32_t)((sysml_state_mappedTens + sysml_state_mappedUnits) - (10.0 * sysml_arithmetic_3))) - sysml_conditional_8;
    int32_t sysml_value_0 = 0;
    sysml_value_0 = sysml_state_sumDigits;
    *io_2 = sysml_value_0;
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
static uint32_t hw_sampled_io_0 = 0;
static bool hw_attempted_io_0 = false;
static double hw_value_io_1 = 0;
static uint32_t hw_sampled_io_1 = 0;
static bool hw_attempted_io_1 = false;
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

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
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
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 10UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (analogRead(A3) * 1.0 + 0.0);
  }
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 10UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = (analogRead(A4) * 1.0 + 0.0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, &hw_value_io_2);
  if (hw_value_io_2 != hw_shift595_previous_io_2) {
    hw_shift595_io_2(hw_value_io_2); hw_shift595_previous_io_2 = hw_value_io_2;
  }
}

