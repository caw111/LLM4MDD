#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_23\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
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
void ProgramPage1(double io_0, double* io_1, double* io_2, double* io_3);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Init function of CompositeActor named "ProgramPage1" */
#line 183 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_23\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void setup();
#line 194 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_23\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void loop();
#line 25 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_23\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double* io_1, double* io_2, double* io_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_value_0 = 0;
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    double sysml_conditional_7 = 0;
    bool sysml_lazy_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_lazy_10 = 0;
    bool sysml_lazy_11 = 0;
    bool sysml_lazy_12 = 0;
    bool sysml_lazy_13 = 0;
    bool sysml_lazy_14 = 0;
    double sysml_conditional_15 = 0;
    bool sysml_lazy_16 = 0;
    bool sysml_lazy_17 = 0;
    bool sysml_lazy_18 = 0;
    bool sysml_lazy_19 = 0;
    bool sysml_lazy_20 = 0;
    bool sysml_lazy_21 = 0;
    bool sysml_lazy_22 = 0;
    double sysml_conditional_23 = 0;
    sysml_lazy_0 = io_0 >= 128.0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_0 <= 255.0;
    }
    sysml_lazy_1 = sysml_lazy_0;
    if (!sysml_lazy_1) {
        sysml_lazy_2 = io_0 >= 512.0;
        if (sysml_lazy_2) {
            sysml_lazy_2 = io_0 <= 639.0;
        }
        sysml_lazy_1 = sysml_lazy_2;
    }
    sysml_lazy_3 = sysml_lazy_1;
    if (!sysml_lazy_3) {
        sysml_lazy_4 = io_0 >= 640.0;
        if (sysml_lazy_4) {
            sysml_lazy_4 = io_0 <= 767.0;
        }
        sysml_lazy_3 = sysml_lazy_4;
    }
    sysml_lazy_5 = sysml_lazy_3;
    if (!sysml_lazy_5) {
        sysml_lazy_6 = io_0 >= 896.0;
        if (sysml_lazy_6) {
            sysml_lazy_6 = io_0 <= 1023.0;
        }
        sysml_lazy_5 = sysml_lazy_6;
    }
    if (sysml_lazy_5) {
        sysml_conditional_7 = 255.0;
    }
    else {
        sysml_conditional_7 = 0.0;
    }
    sysml_value_0 = sysml_conditional_7;
    double sysml_value_1 = 0;
    sysml_lazy_8 = io_0 >= 256.0;
    if (sysml_lazy_8) {
        sysml_lazy_8 = io_0 <= 383.0;
    }
    sysml_lazy_9 = sysml_lazy_8;
    if (!sysml_lazy_9) {
        sysml_lazy_10 = io_0 >= 512.0;
        if (sysml_lazy_10) {
            sysml_lazy_10 = io_0 <= 639.0;
        }
        sysml_lazy_9 = sysml_lazy_10;
    }
    sysml_lazy_11 = sysml_lazy_9;
    if (!sysml_lazy_11) {
        sysml_lazy_12 = io_0 >= 768.0;
        if (sysml_lazy_12) {
            sysml_lazy_12 = io_0 <= 895.0;
        }
        sysml_lazy_11 = sysml_lazy_12;
    }
    sysml_lazy_13 = sysml_lazy_11;
    if (!sysml_lazy_13) {
        sysml_lazy_14 = io_0 >= 896.0;
        if (sysml_lazy_14) {
            sysml_lazy_14 = io_0 <= 1023.0;
        }
        sysml_lazy_13 = sysml_lazy_14;
    }
    if (sysml_lazy_13) {
        sysml_conditional_15 = 255.0;
    }
    else {
        sysml_conditional_15 = 0.0;
    }
    sysml_value_1 = sysml_conditional_15;
    double sysml_value_2 = 0;
    sysml_lazy_16 = io_0 >= 384.0;
    if (sysml_lazy_16) {
        sysml_lazy_16 = io_0 <= 511.0;
    }
    sysml_lazy_17 = sysml_lazy_16;
    if (!sysml_lazy_17) {
        sysml_lazy_18 = io_0 >= 640.0;
        if (sysml_lazy_18) {
            sysml_lazy_18 = io_0 <= 767.0;
        }
        sysml_lazy_17 = sysml_lazy_18;
    }
    sysml_lazy_19 = sysml_lazy_17;
    if (!sysml_lazy_19) {
        sysml_lazy_20 = io_0 >= 768.0;
        if (sysml_lazy_20) {
            sysml_lazy_20 = io_0 <= 895.0;
        }
        sysml_lazy_19 = sysml_lazy_20;
    }
    sysml_lazy_21 = sysml_lazy_19;
    if (!sysml_lazy_21) {
        sysml_lazy_22 = io_0 >= 896.0;
        if (sysml_lazy_22) {
            sysml_lazy_22 = io_0 <= 1023.0;
        }
        sysml_lazy_21 = sysml_lazy_22;
    }
    if (sysml_lazy_21) {
        sysml_conditional_23 = 255.0;
    }
    else {
        sysml_conditional_23 = 0.0;
    }
    sysml_value_2 = sysml_conditional_23;
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
    *io_3 = sysml_value_2;
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
static double hw_value_io_2 = 0;
static double hw_value_io_3 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(A3, INPUT);
  pinMode(2, OUTPUT);
  analogWrite(2, (int)constrain((hw_value_io_1 * 255 + 0), 0.0, 255.0));
  pinMode(3, OUTPUT);
  analogWrite(3, (int)constrain((hw_value_io_2 * 255 + 0), 0.0, 255.0));
  pinMode(4, OUTPUT);
  analogWrite(4, (int)constrain((hw_value_io_3 * 255 + 0), 0.0, 255.0));
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 5UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (analogRead(A3) * 1 + 0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3);
  analogWrite(2, (int)constrain((hw_value_io_1 * 255 + 0), 0.0, 255.0));
  analogWrite(3, (int)constrain((hw_value_io_2 * 255 + 0), 0.0, 255.0));
  analogWrite(4, (int)constrain((hw_value_io_3 * 255 + 0), 0.0, 255.0));
}

