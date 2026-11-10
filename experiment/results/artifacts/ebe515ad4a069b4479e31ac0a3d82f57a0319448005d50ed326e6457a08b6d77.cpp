#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_84\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
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
extern int32_t sysml_state_mode;
/* Global Variable define of  */
extern bool sysml_state_previousBtn1;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, double io_2, bool io_3, bool io_4, double* io_5, double* io_6, double* io_7);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_mode = 1;
/* Global Variable define of  */
bool sysml_state_previousBtn1 = false;
/* Init function of CompositeActor named "ProgramPage1" */
#line 149 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_84\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 164 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_84\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 33 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_84\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_mode = 1;
    sysml_state_previousBtn1 = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, double io_2, bool io_3, bool io_4, double* io_5, double* io_6, double* io_7) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int32_t sysml_conditional_1 = 0;
    int32_t sysml_conditional_2 = 0;
    int32_t sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    double sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    double sysml_conditional_7 = 0;
    bool sysml_lazy_8 = 0;
    double sysml_conditional_9 = 0;
    if (io_4) {
        sysml_state_mode = 1;
    }
    sysml_lazy_0 = io_3;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousBtn1;
    }
    if (sysml_lazy_0) {
        if (sysml_state_mode == 1) {
            sysml_conditional_1 = 2;
        }
        else {
            if (sysml_state_mode == 2) {
                sysml_conditional_2 = 3;
            }
            else {
                if (sysml_state_mode == 3) {
                    sysml_conditional_3 = 4;
                }
                else {
                    sysml_conditional_3 = 1;
                }
                sysml_conditional_2 = sysml_conditional_3;
            }
            sysml_conditional_1 = sysml_conditional_2;
        }
        sysml_state_mode = sysml_conditional_1;
    }
    sysml_state_previousBtn1 = io_3;
    double sysml_value_0 = 0;
    sysml_lazy_4 = sysml_state_mode == 1;
    if (!sysml_lazy_4) {
        sysml_lazy_4 = sysml_state_mode == 4;
    }
    if (sysml_lazy_4) {
        sysml_conditional_5 = io_0;
    }
    else {
        sysml_conditional_5 = 0.0;
    }
    sysml_value_0 = sysml_conditional_5;
    double sysml_value_1 = 0;
    sysml_lazy_6 = sysml_state_mode == 2;
    if (!sysml_lazy_6) {
        sysml_lazy_6 = sysml_state_mode == 4;
    }
    if (sysml_lazy_6) {
        sysml_conditional_7 = io_1;
    }
    else {
        sysml_conditional_7 = 0.0;
    }
    sysml_value_1 = sysml_conditional_7;
    double sysml_value_2 = 0;
    sysml_lazy_8 = sysml_state_mode == 3;
    if (!sysml_lazy_8) {
        sysml_lazy_8 = sysml_state_mode == 4;
    }
    if (sysml_lazy_8) {
        sysml_conditional_9 = io_2;
    }
    else {
        sysml_conditional_9 = 0.0;
    }
    sysml_value_2 = sysml_conditional_9;
    *io_5 = sysml_value_0;
    *io_6 = sysml_value_1;
    *io_7 = sysml_value_2;
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
static double hw_value_io_2 = 0;
static uint32_t hw_sampled_io_2 = 0;
static bool hw_attempted_io_2 = false;
static bool hw_value_io_3 = false;
static uint32_t hw_sampled_io_3 = 0;
static bool hw_attempted_io_3 = false;
static bool hw_value_io_4 = false;
static uint32_t hw_sampled_io_4 = 0;
static bool hw_attempted_io_4 = false;
static double hw_value_io_5 = 0;
static double hw_value_io_6 = 0;
static double hw_value_io_7 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, OUTPUT);
  analogWrite(4, (int)constrain((hw_value_io_5 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(5, OUTPUT);
  analogWrite(5, (int)constrain((hw_value_io_6 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(6, OUTPUT);
  analogWrite(6, (int)constrain((hw_value_io_7 * 1.0 + 0.0), 0.0, 255.0));
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 5UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (analogRead(A3) * 1.0 + 0.0);
  }
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 5UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = (analogRead(A4) * 1.0 + 0.0);
  }
  if (!hw_attempted_io_2 || (uint32_t)(hw_now - hw_sampled_io_2) >= 5UL) {
    hw_attempted_io_2 = true; hw_sampled_io_2 = hw_now;
    hw_value_io_2 = (analogRead(A5) * 1.0 + 0.0);
  }
  if (!hw_attempted_io_3 || (uint32_t)(hw_now - hw_sampled_io_3) >= 5UL) {
    hw_attempted_io_3 = true; hw_sampled_io_3 = hw_now;
    hw_value_io_3 = (digitalRead(2) == HIGH);
  }
  if (!hw_attempted_io_4 || (uint32_t)(hw_now - hw_sampled_io_4) >= 5UL) {
    hw_attempted_io_4 = true; hw_sampled_io_4 = hw_now;
    hw_value_io_4 = (digitalRead(3) == HIGH);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, hw_value_io_3, hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_io_7);
  analogWrite(4, (int)constrain((hw_value_io_5 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(5, (int)constrain((hw_value_io_6 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(6, (int)constrain((hw_value_io_7 * 1.0 + 0.0), 0.0, 255.0));
}

