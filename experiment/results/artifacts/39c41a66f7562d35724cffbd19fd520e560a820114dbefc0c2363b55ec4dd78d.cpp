#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_51\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
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
extern int32_t sysml_state_phase;
/* Global Variable define of  */
extern int32_t sysml_state_phaseAccumMs;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double* io_0, double* io_1, double* io_2, double* io_3, double* io_4, double* io_5, double* io_6, double* io_7, double* io_8, bool* io_9_segment_0, bool* io_9_segment_1, bool* io_9_segment_2, bool* io_9_segment_3, bool* io_9_segment_4);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_phase = 1;
/* Global Variable define of  */
int32_t sysml_state_phaseAccumMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
#line 271 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_51\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void setup();
#line 303 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_51\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void loop();
#line 33 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_51\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_phase = 1;
    sysml_state_phaseAccumMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double* io_0, double* io_1, double* io_2, double* io_3, double* io_4, double* io_5, double* io_6, double* io_7, double* io_8, bool* io_9_segment_0, bool* io_9_segment_1, bool* io_9_segment_2, bool* io_9_segment_3, bool* io_9_segment_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int32_t sysml_conditional_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    double sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    double sysml_conditional_6 = 0;
    bool sysml_lazy_7 = 0;
    double sysml_conditional_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_lazy_10 = 0;
    double sysml_conditional_11 = 0;
    bool sysml_lazy_12 = 0;
    bool sysml_lazy_13 = 0;
    double sysml_conditional_14 = 0;
    double sysml_conditional_15 = 0;
    double sysml_conditional_16 = 0;
    double sysml_conditional_17 = 0;
    bool sysml_lazy_18 = 0;
    double sysml_conditional_19 = 0;
    bool sysml_conditional_20 = 0;
    bool sysml_conditional_21 = 0;
    bool sysml_conditional_22 = 0;
    bool sysml_conditional_23 = 0;
    bool sysml_conditional_24 = 0;
    if ((sysml_state_phaseAccumMs >= 2000)) {
        if (sysml_state_phase == 5) {
            sysml_conditional_0 = 1;
        }
        else {
            sysml_conditional_0 = sysml_state_phase + 1;
        }
        sysml_state_phase = sysml_conditional_0;
        sysml_state_phaseAccumMs = 0;
    }
    else {
        sysml_state_phaseAccumMs = (sysml_state_phaseAccumMs + 10);
    }
    double sysml_value_0 = 0;
    sysml_lazy_1 = sysml_state_phase == 1;
    if (!sysml_lazy_1) {
        sysml_lazy_1 = sysml_state_phase == 4;
    }
    sysml_lazy_2 = sysml_lazy_1;
    if (!sysml_lazy_2) {
        sysml_lazy_2 = sysml_state_phase == 5;
    }
    if (sysml_lazy_2) {
        sysml_conditional_3 = 255.0;
    }
    else {
        sysml_conditional_3 = 0.0;
    }
    sysml_value_0 = sysml_conditional_3;
    double sysml_value_1 = 0;
    sysml_lazy_4 = sysml_state_phase == 2;
    if (!sysml_lazy_4) {
        sysml_lazy_4 = sysml_state_phase == 4;
    }
    sysml_lazy_5 = sysml_lazy_4;
    if (!sysml_lazy_5) {
        sysml_lazy_5 = sysml_state_phase == 5;
    }
    if (sysml_lazy_5) {
        sysml_conditional_6 = 255.0;
    }
    else {
        sysml_conditional_6 = 0.0;
    }
    sysml_value_1 = sysml_conditional_6;
    double sysml_value_2 = 0;
    sysml_lazy_7 = sysml_state_phase == 3;
    if (!sysml_lazy_7) {
        sysml_lazy_7 = sysml_state_phase == 5;
    }
    if (sysml_lazy_7) {
        sysml_conditional_8 = 255.0;
    }
    else {
        sysml_conditional_8 = 0.0;
    }
    sysml_value_2 = sysml_conditional_8;
    double sysml_value_3 = 0;
    sysml_lazy_9 = sysml_state_phase == 2;
    if (!sysml_lazy_9) {
        sysml_lazy_9 = sysml_state_phase == 4;
    }
    sysml_lazy_10 = sysml_lazy_9;
    if (!sysml_lazy_10) {
        sysml_lazy_10 = sysml_state_phase == 5;
    }
    if (sysml_lazy_10) {
        sysml_conditional_11 = 255.0;
    }
    else {
        sysml_conditional_11 = 0.0;
    }
    sysml_value_3 = sysml_conditional_11;
    double sysml_value_4 = 0;
    sysml_lazy_12 = sysml_state_phase == 2;
    if (!sysml_lazy_12) {
        sysml_lazy_12 = sysml_state_phase == 4;
    }
    sysml_lazy_13 = sysml_lazy_12;
    if (!sysml_lazy_13) {
        sysml_lazy_13 = sysml_state_phase == 5;
    }
    if (sysml_lazy_13) {
        sysml_conditional_14 = 255.0;
    }
    else {
        sysml_conditional_14 = 0.0;
    }
    sysml_value_4 = sysml_conditional_14;
    double sysml_value_5 = 0;
    if (sysml_state_phase == 5) {
        sysml_conditional_15 = 255.0;
    }
    else {
        sysml_conditional_15 = 0.0;
    }
    sysml_value_5 = sysml_conditional_15;
    double sysml_value_6 = 0;
    if (sysml_state_phase == 5) {
        sysml_conditional_16 = 255.0;
    }
    else {
        sysml_conditional_16 = 0.0;
    }
    sysml_value_6 = sysml_conditional_16;
    double sysml_value_7 = 0;
    if (sysml_state_phase == 5) {
        sysml_conditional_17 = 255.0;
    }
    else {
        sysml_conditional_17 = 0.0;
    }
    sysml_value_7 = sysml_conditional_17;
    double sysml_value_8 = 0;
    sysml_lazy_18 = sysml_state_phase == 3;
    if (!sysml_lazy_18) {
        sysml_lazy_18 = sysml_state_phase == 5;
    }
    if (sysml_lazy_18) {
        sysml_conditional_19 = 255.0;
    }
    else {
        sysml_conditional_19 = 0.0;
    }
    sysml_value_8 = sysml_conditional_19;
    bool sysml_value_9 = 0;
    if (sysml_state_phase >= 1) {
        sysml_conditional_20 = true;
    }
    else {
        sysml_conditional_20 = false;
    }
    sysml_value_9 = sysml_conditional_20;
    bool sysml_value_10 = 0;
    if (sysml_state_phase >= 2) {
        sysml_conditional_21 = true;
    }
    else {
        sysml_conditional_21 = false;
    }
    sysml_value_10 = sysml_conditional_21;
    bool sysml_value_11 = 0;
    if (sysml_state_phase >= 3) {
        sysml_conditional_22 = true;
    }
    else {
        sysml_conditional_22 = false;
    }
    sysml_value_11 = sysml_conditional_22;
    bool sysml_value_12 = 0;
    if (sysml_state_phase >= 4) {
        sysml_conditional_23 = true;
    }
    else {
        sysml_conditional_23 = false;
    }
    sysml_value_12 = sysml_conditional_23;
    bool sysml_value_13 = 0;
    if (sysml_state_phase >= 5) {
        sysml_conditional_24 = true;
    }
    else {
        sysml_conditional_24 = false;
    }
    sysml_value_13 = sysml_conditional_24;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
    *io_4 = sysml_value_4;
    *io_5 = sysml_value_5;
    *io_6 = sysml_value_6;
    *io_7 = sysml_value_7;
    *io_8 = sysml_value_8;
    *io_9_segment_0 = sysml_value_9;
    *io_9_segment_1 = sysml_value_10;
    *io_9_segment_2 = sysml_value_11;
    *io_9_segment_3 = sysml_value_12;
    *io_9_segment_4 = sysml_value_13;
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
static double hw_value_io_7 = 0;
static double hw_value_io_8 = 0;
static bool hw_value_io_9_segment_0 = false;
static bool hw_value_io_9_segment_1 = false;
static bool hw_value_io_9_segment_2 = false;
static bool hw_value_io_9_segment_3 = false;
static bool hw_value_io_9_segment_4 = false;

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
  pinMode(8, OUTPUT);
  analogWrite(8, (int)constrain((hw_value_io_6 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(9, OUTPUT);
  analogWrite(9, (int)constrain((hw_value_io_7 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(10, OUTPUT);
  analogWrite(10, (int)constrain((hw_value_io_8 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(11, OUTPUT);
  digitalWrite(11, hw_value_io_9_segment_0 ? HIGH : LOW);
  pinMode(12, OUTPUT);
  digitalWrite(12, hw_value_io_9_segment_1 ? HIGH : LOW);
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_9_segment_2 ? HIGH : LOW);
  pinMode(44, OUTPUT);
  digitalWrite(44, hw_value_io_9_segment_3 ? HIGH : LOW);
  pinMode(45, OUTPUT);
  digitalWrite(45, hw_value_io_9_segment_4 ? HIGH : LOW);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(&hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_io_7, &hw_value_io_8, &hw_value_io_9_segment_0, &hw_value_io_9_segment_1, &hw_value_io_9_segment_2, &hw_value_io_9_segment_3, &hw_value_io_9_segment_4);
  analogWrite(2, (int)constrain((hw_value_io_0 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(3, (int)constrain((hw_value_io_1 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(4, (int)constrain((hw_value_io_2 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(5, (int)constrain((hw_value_io_3 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(6, (int)constrain((hw_value_io_4 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(7, (int)constrain((hw_value_io_5 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(8, (int)constrain((hw_value_io_6 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(9, (int)constrain((hw_value_io_7 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(10, (int)constrain((hw_value_io_8 * 1.0 + 0.0), 0.0, 255.0));
  digitalWrite(11, hw_value_io_9_segment_0 ? HIGH : LOW);
  digitalWrite(12, hw_value_io_9_segment_1 ? HIGH : LOW);
  digitalWrite(13, hw_value_io_9_segment_2 ? HIGH : LOW);
  digitalWrite(44, hw_value_io_9_segment_3 ? HIGH : LOW);
  digitalWrite(45, hw_value_io_9_segment_4 ? HIGH : LOW);
}

