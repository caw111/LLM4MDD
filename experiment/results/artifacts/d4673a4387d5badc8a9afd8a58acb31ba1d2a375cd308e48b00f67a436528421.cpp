#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_18\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
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
extern int32_t sysml_state_phaseIndex;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_9, double* io_0, double* io_1, double* io_2, double* io_3, double* io_4, double* io_5, double* io_6, double* io_7, double* io_8);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_accumulatedMs = 0;
/* Global Variable define of  */
int32_t sysml_state_phaseIndex = 0;
/* Init function of CompositeActor named "ProgramPage1" */
#line 235 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_18\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 257 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_18\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 33 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_18\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_accumulatedMs = 0;
    sysml_state_phaseIndex = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_9, double* io_0, double* io_1, double* io_2, double* io_3, double* io_4, double* io_5, double* io_6, double* io_7, double* io_8) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int32_t sysml_conditional_0 = 0;
    int32_t sysml_conditional_1 = 0;
    double sysml_conditional_2 = 0;
    double sysml_conditional_3 = 0;
    double sysml_conditional_4 = 0;
    double sysml_conditional_5 = 0;
    double sysml_conditional_6 = 0;
    double sysml_conditional_7 = 0;
    double sysml_conditional_8 = 0;
    double sysml_conditional_9 = 0;
    double sysml_conditional_10 = 0;
    double sysml_conditional_11 = 0;
    double sysml_conditional_12 = 0;
    double sysml_conditional_13 = 0;
    double sysml_conditional_14 = 0;
    double sysml_conditional_15 = 0;
    double sysml_conditional_16 = 0;
    double sysml_conditional_17 = 0;
    double sysml_conditional_18 = 0;
    double sysml_conditional_19 = 0;
    sysml_state_accumulatedMs = (sysml_state_accumulatedMs + io_9);
    if ((sysml_state_accumulatedMs >= 2000)) {
        sysml_state_accumulatedMs = (sysml_state_accumulatedMs - 2000);
        if (sysml_state_phaseIndex == 0) {
            sysml_conditional_0 = 1;
        }
        else {
            if (sysml_state_phaseIndex == 1) {
                sysml_conditional_1 = 2;
            }
            else {
                sysml_conditional_1 = 0;
            }
            sysml_conditional_0 = sysml_conditional_1;
        }
        sysml_state_phaseIndex = sysml_conditional_0;
    }
    double sysml_value_0 = 0;
    if (sysml_state_phaseIndex == 0) {
        sysml_conditional_2 = 255.0;
    }
    else {
        if (sysml_state_phaseIndex == 1) {
            sysml_conditional_3 = 0.0;
        }
        else {
            sysml_conditional_3 = 0.0;
        }
        sysml_conditional_2 = sysml_conditional_3;
    }
    sysml_value_0 = sysml_conditional_2;
    double sysml_value_1 = 0;
    if (sysml_state_phaseIndex == 0) {
        sysml_conditional_4 = 0.0;
    }
    else {
        if (sysml_state_phaseIndex == 1) {
            sysml_conditional_5 = 255.0;
        }
        else {
            sysml_conditional_5 = 0.0;
        }
        sysml_conditional_4 = sysml_conditional_5;
    }
    sysml_value_1 = sysml_conditional_4;
    double sysml_value_2 = 0;
    if (sysml_state_phaseIndex == 0) {
        sysml_conditional_6 = 0.0;
    }
    else {
        if (sysml_state_phaseIndex == 1) {
            sysml_conditional_7 = 0.0;
        }
        else {
            sysml_conditional_7 = 255.0;
        }
        sysml_conditional_6 = sysml_conditional_7;
    }
    sysml_value_2 = sysml_conditional_6;
    double sysml_value_3 = 0;
    if (sysml_state_phaseIndex == 0) {
        sysml_conditional_8 = 0.0;
    }
    else {
        if (sysml_state_phaseIndex == 1) {
            sysml_conditional_9 = 255.0;
        }
        else {
            sysml_conditional_9 = 0.0;
        }
        sysml_conditional_8 = sysml_conditional_9;
    }
    sysml_value_3 = sysml_conditional_8;
    double sysml_value_4 = 0;
    if (sysml_state_phaseIndex == 0) {
        sysml_conditional_10 = 0.0;
    }
    else {
        if (sysml_state_phaseIndex == 1) {
            sysml_conditional_11 = 0.0;
        }
        else {
            sysml_conditional_11 = 255.0;
        }
        sysml_conditional_10 = sysml_conditional_11;
    }
    sysml_value_4 = sysml_conditional_10;
    double sysml_value_5 = 0;
    if (sysml_state_phaseIndex == 0) {
        sysml_conditional_12 = 255.0;
    }
    else {
        if (sysml_state_phaseIndex == 1) {
            sysml_conditional_13 = 0.0;
        }
        else {
            sysml_conditional_13 = 0.0;
        }
        sysml_conditional_12 = sysml_conditional_13;
    }
    sysml_value_5 = sysml_conditional_12;
    double sysml_value_6 = 0;
    if (sysml_state_phaseIndex == 0) {
        sysml_conditional_14 = 0.0;
    }
    else {
        if (sysml_state_phaseIndex == 1) {
            sysml_conditional_15 = 0.0;
        }
        else {
            sysml_conditional_15 = 255.0;
        }
        sysml_conditional_14 = sysml_conditional_15;
    }
    sysml_value_6 = sysml_conditional_14;
    double sysml_value_7 = 0;
    if (sysml_state_phaseIndex == 0) {
        sysml_conditional_16 = 255.0;
    }
    else {
        if (sysml_state_phaseIndex == 1) {
            sysml_conditional_17 = 0.0;
        }
        else {
            sysml_conditional_17 = 0.0;
        }
        sysml_conditional_16 = sysml_conditional_17;
    }
    sysml_value_7 = sysml_conditional_16;
    double sysml_value_8 = 0;
    if (sysml_state_phaseIndex == 0) {
        sysml_conditional_18 = 0.0;
    }
    else {
        if (sysml_state_phaseIndex == 1) {
            sysml_conditional_19 = 255.0;
        }
        else {
            sysml_conditional_19 = 0.0;
        }
        sysml_conditional_18 = sysml_conditional_19;
    }
    sysml_value_8 = sysml_conditional_18;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
    *io_4 = sysml_value_4;
    *io_5 = sysml_value_5;
    *io_6 = sysml_value_6;
    *io_7 = sysml_value_7;
    *io_8 = sysml_value_8;
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
static int32_t hw_value_io_9 = 0;

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
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_9 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_9, &hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_io_7, &hw_value_io_8);
  analogWrite(2, (int)constrain((hw_value_io_0 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(3, (int)constrain((hw_value_io_1 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(4, (int)constrain((hw_value_io_2 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(5, (int)constrain((hw_value_io_3 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(6, (int)constrain((hw_value_io_4 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(7, (int)constrain((hw_value_io_5 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(8, (int)constrain((hw_value_io_6 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(9, (int)constrain((hw_value_io_7 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(10, (int)constrain((hw_value_io_8 * 1.0 + 0.0), 0.0, 255.0));
}

