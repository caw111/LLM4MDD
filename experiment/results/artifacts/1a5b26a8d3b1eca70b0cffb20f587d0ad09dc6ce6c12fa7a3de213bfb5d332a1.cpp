#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_60\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
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
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_6, double io_7, double io_8, double* io_0, double* io_1, double* io_2, double* io_3, double* io_4, double* io_5, int32_t* io_9);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Init function of CompositeActor named "ProgramPage1" */
#line 92 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_60\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
static void hw_shift595_io_9(int32_t value);
#line 126 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_60\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void setup();
#line 151 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_60\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void loop();
#line 25 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_60\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_6, double io_7, double io_8, double* io_0, double* io_1, double* io_2, double* io_3, double* io_4, double* io_5, int32_t* io_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_value_0 = 0;
    int32_t sysml_conditional_0 = 0;
    int32_t sysml_conditional_1 = 0;
    sysml_value_0 = ((double)((io_6 * 255.0)) / 1023.0);
    double sysml_value_1 = 0;
    sysml_value_1 = ((double)((io_7 * 255.0)) / 1023.0);
    double sysml_value_2 = 0;
    sysml_value_2 = ((double)((io_8 * 255.0)) / 1023.0);
    double sysml_value_3 = 0;
    sysml_value_3 = ((double)((io_6 * 255.0)) / 1023.0);
    double sysml_value_4 = 0;
    sysml_value_4 = ((double)((io_7 * 255.0)) / 1023.0);
    double sysml_value_5 = 0;
    sysml_value_5 = ((double)((io_8 * 255.0)) / 1023.0);
    int32_t sysml_value_6 = 0;
    if (io_8 >= 0.0) {
        if ((((((double)(io_6 * 255.0)) / 1023.0) + (io_8 * 0.0)) * 0.0) < ((double)((int32_t)(((((double)(io_6 * 255.0)) / 1023.0) + (io_8 * 0.0)) * 0.0)))) {
            sysml_conditional_1 = 1;
        }
        else {
            sysml_conditional_1 = 0;
        }
        sysml_conditional_0 = ((int32_t)(((((double)(io_6 * 255.0)) / 1023.0) + (io_8 * 0.0)) * 0.0)) - sysml_conditional_1;
    }
    else {
        sysml_conditional_0 = 0;
    }
    sysml_value_6 = sysml_conditional_0;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
    *io_4 = sysml_value_4;
    *io_5 = sysml_value_5;
    *io_9 = sysml_value_6;
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
static double hw_value_io_3 = 0;
static double hw_value_io_4 = 0;
static double hw_value_io_5 = 0;
static double hw_value_io_6 = 0;
static uint32_t hw_sampled_io_6 = 0;
static bool hw_attempted_io_6 = false;
static double hw_value_io_7 = 0;
static uint32_t hw_sampled_io_7 = 0;
static bool hw_attempted_io_7 = false;
static double hw_value_io_8 = 0;
static uint32_t hw_sampled_io_8 = 0;
static bool hw_attempted_io_8 = false;
static int32_t hw_value_io_9 = 0;
static void hw_shift595_io_9(int32_t value) {
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
    uint8_t segments = valid && (value >= 10L) ? digits[(value / 10L) % 10] : 0;
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
static int32_t hw_shift595_previous_io_9 = INT32_MIN;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, OUTPUT);
  analogWrite(2, (int)constrain((hw_value_io_0 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(3, OUTPUT);
  analogWrite(3, (int)constrain((hw_value_io_1 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(4, OUTPUT);
  analogWrite(4, (int)constrain((hw_value_io_2 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(5, OUTPUT);
  analogWrite(5, (int)constrain((hw_value_io_3 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(6, OUTPUT);
  analogWrite(6, (int)constrain((hw_value_io_4 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(7, OUTPUT);
  analogWrite(7, (int)constrain((hw_value_io_5 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(A0, OUTPUT); digitalWrite(A0, LOW);
  pinMode(A2, OUTPUT); digitalWrite(A2, LOW);
  pinMode(A1, OUTPUT); digitalWrite(A1, LOW);
  if (hw_value_io_9 != hw_shift595_previous_io_9) {
    hw_shift595_io_9(hw_value_io_9); hw_shift595_previous_io_9 = hw_value_io_9;
  }
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_6 || (uint32_t)(hw_now - hw_sampled_io_6) >= 5UL) {
    hw_attempted_io_6 = true; hw_sampled_io_6 = hw_now;
    hw_value_io_6 = (analogRead(A3) * 1.0 + 0.0);
  }
  if (!hw_attempted_io_7 || (uint32_t)(hw_now - hw_sampled_io_7) >= 5UL) {
    hw_attempted_io_7 = true; hw_sampled_io_7 = hw_now;
    hw_value_io_7 = (analogRead(A4) * 1.0 + 0.0);
  }
  if (!hw_attempted_io_8 || (uint32_t)(hw_now - hw_sampled_io_8) >= 5UL) {
    hw_attempted_io_8 = true; hw_sampled_io_8 = hw_now;
    hw_value_io_8 = (analogRead(A5) * 1.0 + 0.0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_6, hw_value_io_7, hw_value_io_8, &hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_9);
  analogWrite(2, (int)constrain((hw_value_io_0 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(3, (int)constrain((hw_value_io_1 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(4, (int)constrain((hw_value_io_2 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(5, (int)constrain((hw_value_io_3 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(6, (int)constrain((hw_value_io_4 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(7, (int)constrain((hw_value_io_5 * 1.0 + 0.0), 0.0, 255.0));
  if (hw_value_io_9 != hw_shift595_previous_io_9) {
    hw_shift595_io_9(hw_value_io_9); hw_shift595_previous_io_9 = hw_value_io_9;
  }
}

