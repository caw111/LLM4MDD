#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_3\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
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
extern bool sysml_state_modeCycling;
/* Global Variable define of  */
extern bool sysml_state_modeOff;
/* Global Variable define of  */
extern int32_t sysml_state_currentColorIndex;
/* Global Variable define of  */
extern bool sysml_state_previousK1;
/* Global Variable define of  */
extern int32_t sysml_state_accumulatedMs;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_3, bool io_4, bool io_5, int32_t io_6, double* io_0, double* io_1, double* io_2);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_modeCycling = true;
/* Global Variable define of  */
bool sysml_state_modeOff = false;
/* Global Variable define of  */
int32_t sysml_state_currentColorIndex = 0;
/* Global Variable define of  */
bool sysml_state_previousK1 = false;
/* Global Variable define of  */
int32_t sysml_state_accumulatedMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
#line 229 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_3\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void setup();
#line 242 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_3\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void loop();
#line 45 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_3\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_modeCycling = true;
    sysml_state_modeOff = false;
    sysml_state_currentColorIndex = 0;
    sysml_state_previousK1 = false;
    sysml_state_accumulatedMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_3, bool io_4, bool io_5, int32_t io_6, double* io_0, double* io_1, double* io_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    int32_t sysml_conditional_4 = 0;
    double sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    double sysml_conditional_7 = 0;
    bool sysml_lazy_8 = 0;
    double sysml_conditional_9 = 0;
    double sysml_conditional_10 = 0;
    bool sysml_lazy_11 = 0;
    double sysml_conditional_12 = 0;
    bool sysml_lazy_13 = 0;
    double sysml_conditional_14 = 0;
    double sysml_conditional_15 = 0;
    bool sysml_lazy_16 = 0;
    double sysml_conditional_17 = 0;
    bool sysml_lazy_18 = 0;
    double sysml_conditional_19 = 0;
    if (io_5) {
        sysml_state_modeOff = true;
        sysml_state_modeCycling = false;
    }
    sysml_lazy_0 = !io_5;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_4;
    }
    sysml_lazy_1 = sysml_lazy_0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_modeOff;
    }
    if (sysml_lazy_1) {
        sysml_state_modeCycling = true;
        sysml_state_modeOff = false;
    }
    sysml_lazy_2 = io_3;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_previousK1;
    }
    if (sysml_lazy_2) {
        sysml_state_modeCycling = false;
    }
    sysml_lazy_3 = sysml_state_modeCycling;
    if (sysml_lazy_3) {
        sysml_lazy_3 = !sysml_state_modeOff;
    }
    if (sysml_lazy_3) {
        sysml_state_accumulatedMs = (sysml_state_accumulatedMs + io_6);
        if ((sysml_state_accumulatedMs >= 2000)) {
            sysml_state_accumulatedMs = (sysml_state_accumulatedMs - 2000);
            if (sysml_state_currentColorIndex == 2) {
                sysml_conditional_4 = 0;
            }
            else {
                sysml_conditional_4 = sysml_state_currentColorIndex + 1;
            }
            sysml_state_currentColorIndex = sysml_conditional_4;
        }
    }
    sysml_state_previousK1 = io_3;
    double sysml_value_0 = 0;
    if (sysml_state_modeOff) {
        sysml_conditional_5 = 0.0;
    }
    else {
        sysml_lazy_6 = sysml_state_modeCycling;
        if (sysml_lazy_6) {
            sysml_lazy_6 = sysml_state_currentColorIndex == 0;
        }
        if (sysml_lazy_6) {
            sysml_conditional_7 = 255.0;
        }
        else {
            sysml_lazy_8 = !sysml_state_modeCycling;
            if (sysml_lazy_8) {
                sysml_lazy_8 = sysml_state_currentColorIndex == 0;
            }
            if (sysml_lazy_8) {
                sysml_conditional_9 = 255.0;
            }
            else {
                sysml_conditional_9 = 0.0;
            }
            sysml_conditional_7 = sysml_conditional_9;
        }
        sysml_conditional_5 = sysml_conditional_7;
    }
    sysml_value_0 = sysml_conditional_5;
    double sysml_value_1 = 0;
    if (sysml_state_modeOff) {
        sysml_conditional_10 = 0.0;
    }
    else {
        sysml_lazy_11 = sysml_state_modeCycling;
        if (sysml_lazy_11) {
            sysml_lazy_11 = sysml_state_currentColorIndex == 1;
        }
        if (sysml_lazy_11) {
            sysml_conditional_12 = 255.0;
        }
        else {
            sysml_lazy_13 = !sysml_state_modeCycling;
            if (sysml_lazy_13) {
                sysml_lazy_13 = sysml_state_currentColorIndex == 1;
            }
            if (sysml_lazy_13) {
                sysml_conditional_14 = 255.0;
            }
            else {
                sysml_conditional_14 = 0.0;
            }
            sysml_conditional_12 = sysml_conditional_14;
        }
        sysml_conditional_10 = sysml_conditional_12;
    }
    sysml_value_1 = sysml_conditional_10;
    double sysml_value_2 = 0;
    if (sysml_state_modeOff) {
        sysml_conditional_15 = 0.0;
    }
    else {
        sysml_lazy_16 = sysml_state_modeCycling;
        if (sysml_lazy_16) {
            sysml_lazy_16 = sysml_state_currentColorIndex == 2;
        }
        if (sysml_lazy_16) {
            sysml_conditional_17 = 255.0;
        }
        else {
            sysml_lazy_18 = !sysml_state_modeCycling;
            if (sysml_lazy_18) {
                sysml_lazy_18 = sysml_state_currentColorIndex == 2;
            }
            if (sysml_lazy_18) {
                sysml_conditional_19 = 255.0;
            }
            else {
                sysml_conditional_19 = 0.0;
            }
            sysml_conditional_17 = sysml_conditional_19;
        }
        sysml_conditional_15 = sysml_conditional_17;
    }
    sysml_value_2 = sysml_conditional_15;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
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
static bool hw_value_io_3 = false;
static uint32_t hw_sampled_io_3 = 0;
static bool hw_attempted_io_3 = false;
static bool hw_value_io_4 = false;
static uint32_t hw_sampled_io_4 = 0;
static bool hw_attempted_io_4 = false;
static bool hw_value_io_5 = false;
static uint32_t hw_sampled_io_5 = 0;
static bool hw_attempted_io_5 = false;
static int32_t hw_value_io_6 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, OUTPUT);
  analogWrite(2, (int)constrain((hw_value_io_0 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(3, OUTPUT);
  analogWrite(3, (int)constrain((hw_value_io_1 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(4, OUTPUT);
  analogWrite(4, (int)constrain((hw_value_io_2 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_3 || (uint32_t)(hw_now - hw_sampled_io_3) >= 5UL) {
    hw_attempted_io_3 = true; hw_sampled_io_3 = hw_now;
    hw_value_io_3 = (digitalRead(5) == LOW);
  }
  if (!hw_attempted_io_4 || (uint32_t)(hw_now - hw_sampled_io_4) >= 5UL) {
    hw_attempted_io_4 = true; hw_sampled_io_4 = hw_now;
    hw_value_io_4 = (digitalRead(6) == LOW);
  }
  if (!hw_attempted_io_5 || (uint32_t)(hw_now - hw_sampled_io_5) >= 5UL) {
    hw_attempted_io_5 = true; hw_sampled_io_5 = hw_now;
    hw_value_io_5 = (digitalRead(7) == LOW);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_6 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_3, hw_value_io_4, hw_value_io_5, hw_value_io_6, &hw_value_io_0, &hw_value_io_1, &hw_value_io_2);
  analogWrite(2, (int)constrain((hw_value_io_0 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(3, (int)constrain((hw_value_io_1 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(4, (int)constrain((hw_value_io_2 * 1.0 + 0.0), 0.0, 255.0));
}

