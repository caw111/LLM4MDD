#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_85\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
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
extern int32_t sysml_state_mode1Index;
/* Global Variable define of  */
extern int32_t sysml_state_mode2Index;
/* Global Variable define of  */
extern bool sysml_state_previousPressedBtn1;
/* Global Variable define of  */
extern bool sysml_state_previousPressedBtn2;
/* Global Variable define of  */
extern bool sysml_state_isSecondMode;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double io_2, double* io_3, double* io_4, double* io_5);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_mode1Index = 0;
/* Global Variable define of  */
int32_t sysml_state_mode2Index = 0;
/* Global Variable define of  */
bool sysml_state_previousPressedBtn1 = false;
/* Global Variable define of  */
bool sysml_state_previousPressedBtn2 = false;
/* Global Variable define of  */
bool sysml_state_isSecondMode = false;
/* Init function of CompositeActor named "ProgramPage1" */
#line 334 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_85\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 347 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_85\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 45 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_85\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_mode1Index = 0;
    sysml_state_mode2Index = 0;
    sysml_state_previousPressedBtn1 = false;
    sysml_state_previousPressedBtn2 = false;
    sysml_state_isSecondMode = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double io_2, double* io_3, double* io_4, double* io_5) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int32_t sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    int32_t sysml_conditional_3 = 0;
    double sysml_conditional_4 = 0;
    double sysml_conditional_5 = 0;
    int32_t sysml_conditional_6 = 0;
    double sysml_conditional_7 = 0;
    double sysml_conditional_8 = 0;
    int32_t sysml_conditional_9 = 0;
    double sysml_conditional_10 = 0;
    int32_t sysml_conditional_11 = 0;
    double sysml_conditional_12 = 0;
    double sysml_conditional_13 = 0;
    double sysml_conditional_14 = 0;
    double sysml_conditional_15 = 0;
    int32_t sysml_conditional_16 = 0;
    double sysml_conditional_17 = 0;
    int32_t sysml_conditional_18 = 0;
    double sysml_conditional_19 = 0;
    double sysml_conditional_20 = 0;
    double sysml_conditional_21 = 0;
    int32_t sysml_conditional_22 = 0;
    double sysml_conditional_23 = 0;
    double sysml_conditional_24 = 0;
    double sysml_conditional_25 = 0;
    double sysml_conditional_26 = 0;
    double sysml_conditional_27 = 0;
    int32_t sysml_conditional_28 = 0;
    double sysml_conditional_29 = 0;
    double sysml_conditional_30 = 0;
    double sysml_conditional_31 = 0;
    int32_t sysml_conditional_32 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressedBtn1;
    }
    if (sysml_lazy_0) {
        if (sysml_state_isSecondMode) {
            sysml_state_isSecondMode = false;
        }
        else {
            if (sysml_state_mode1Index == 2) {
                sysml_conditional_1 = 0;
            }
            else {
                sysml_conditional_1 = sysml_state_mode1Index + 1;
            }
            sysml_state_mode1Index = sysml_conditional_1;
        }
    }
    sysml_lazy_2 = io_1;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_previousPressedBtn2;
    }
    if (sysml_lazy_2) {
        if (sysml_state_isSecondMode) {
            if (sysml_state_mode2Index == 2) {
                sysml_conditional_3 = 0;
            }
            else {
                sysml_conditional_3 = sysml_state_mode2Index + 1;
            }
            sysml_state_mode2Index = sysml_conditional_3;
        }
        else {
            sysml_state_isSecondMode = true;
        }
    }
    sysml_state_previousPressedBtn1 = io_0;
    sysml_state_previousPressedBtn2 = io_1;
    double sysml_value_0 = 0;
    if (sysml_state_isSecondMode) {
        if (sysml_state_mode2Index == 0) {
            if ((((double)(io_2 * 255.0)) / 1023.0) < ((double)((int32_t)(((double)(io_2 * 255.0)) / 1023.0)))) {
                sysml_conditional_6 = 1;
            }
            else {
                sysml_conditional_6 = 0;
            }
            sysml_conditional_5 = ((int32_t)(((double)(io_2 * 255.0)) / 1023.0)) - sysml_conditional_6;
        }
        else {
            if (sysml_state_mode2Index == 1) {
                sysml_conditional_7 = 0.0;
            }
            else {
                if (sysml_state_mode2Index == 2) {
                    if ((((double)(io_2 * 255.0)) / 1023.0) < ((double)((int32_t)(((double)(io_2 * 255.0)) / 1023.0)))) {
                        sysml_conditional_9 = 1;
                    }
                    else {
                        sysml_conditional_9 = 0;
                    }
                    sysml_conditional_8 = ((int32_t)(((double)(io_2 * 255.0)) / 1023.0)) - sysml_conditional_9;
                }
                else {
                    sysml_conditional_8 = 0.0;
                }
                sysml_conditional_7 = sysml_conditional_8;
            }
            sysml_conditional_5 = sysml_conditional_7;
        }
        sysml_conditional_4 = sysml_conditional_5;
    }
    else {
        if (sysml_state_mode1Index == 0) {
            if ((((double)(io_2 * 255.0)) / 1023.0) < ((double)((int32_t)(((double)(io_2 * 255.0)) / 1023.0)))) {
                sysml_conditional_11 = 1;
            }
            else {
                sysml_conditional_11 = 0;
            }
            sysml_conditional_10 = ((int32_t)(((double)(io_2 * 255.0)) / 1023.0)) - sysml_conditional_11;
        }
        else {
            if (sysml_state_mode1Index == 1) {
                sysml_conditional_12 = 0.0;
            }
            else {
                if (sysml_state_mode1Index == 2) {
                    sysml_conditional_13 = 0.0;
                }
                else {
                    sysml_conditional_13 = 0.0;
                }
                sysml_conditional_12 = sysml_conditional_13;
            }
            sysml_conditional_10 = sysml_conditional_12;
        }
        sysml_conditional_4 = sysml_conditional_10;
    }
    sysml_value_0 = sysml_conditional_4;
    double sysml_value_1 = 0;
    if (sysml_state_isSecondMode) {
        if (sysml_state_mode2Index == 0) {
            if ((((double)(io_2 * 255.0)) / 1023.0) < ((double)((int32_t)(((double)(io_2 * 255.0)) / 1023.0)))) {
                sysml_conditional_16 = 1;
            }
            else {
                sysml_conditional_16 = 0;
            }
            sysml_conditional_15 = ((int32_t)(((double)(io_2 * 255.0)) / 1023.0)) - sysml_conditional_16;
        }
        else {
            if (sysml_state_mode2Index == 1) {
                if ((((double)(io_2 * 255.0)) / 1023.0) < ((double)((int32_t)(((double)(io_2 * 255.0)) / 1023.0)))) {
                    sysml_conditional_18 = 1;
                }
                else {
                    sysml_conditional_18 = 0;
                }
                sysml_conditional_17 = ((int32_t)(((double)(io_2 * 255.0)) / 1023.0)) - sysml_conditional_18;
            }
            else {
                if (sysml_state_mode2Index == 2) {
                    sysml_conditional_19 = 0.0;
                }
                else {
                    sysml_conditional_19 = 0.0;
                }
                sysml_conditional_17 = sysml_conditional_19;
            }
            sysml_conditional_15 = sysml_conditional_17;
        }
        sysml_conditional_14 = sysml_conditional_15;
    }
    else {
        if (sysml_state_mode1Index == 0) {
            sysml_conditional_20 = 0.0;
        }
        else {
            if (sysml_state_mode1Index == 1) {
                if ((((double)(io_2 * 255.0)) / 1023.0) < ((double)((int32_t)(((double)(io_2 * 255.0)) / 1023.0)))) {
                    sysml_conditional_22 = 1;
                }
                else {
                    sysml_conditional_22 = 0;
                }
                sysml_conditional_21 = ((int32_t)(((double)(io_2 * 255.0)) / 1023.0)) - sysml_conditional_22;
            }
            else {
                if (sysml_state_mode1Index == 2) {
                    sysml_conditional_23 = 0.0;
                }
                else {
                    sysml_conditional_23 = 0.0;
                }
                sysml_conditional_21 = sysml_conditional_23;
            }
            sysml_conditional_20 = sysml_conditional_21;
        }
        sysml_conditional_14 = sysml_conditional_20;
    }
    sysml_value_1 = sysml_conditional_14;
    double sysml_value_2 = 0;
    if (sysml_state_isSecondMode) {
        if (sysml_state_mode2Index == 0) {
            sysml_conditional_25 = 0.0;
        }
        else {
            if (sysml_state_mode2Index == 1) {
                sysml_conditional_26 = 0.0;
            }
            else {
                if (sysml_state_mode2Index == 2) {
                    if ((((double)(io_2 * 255.0)) / 1023.0) < ((double)((int32_t)(((double)(io_2 * 255.0)) / 1023.0)))) {
                        sysml_conditional_28 = 1;
                    }
                    else {
                        sysml_conditional_28 = 0;
                    }
                    sysml_conditional_27 = ((int32_t)(((double)(io_2 * 255.0)) / 1023.0)) - sysml_conditional_28;
                }
                else {
                    sysml_conditional_27 = 0.0;
                }
                sysml_conditional_26 = sysml_conditional_27;
            }
            sysml_conditional_25 = sysml_conditional_26;
        }
        sysml_conditional_24 = sysml_conditional_25;
    }
    else {
        if (sysml_state_mode1Index == 0) {
            sysml_conditional_29 = 0.0;
        }
        else {
            if (sysml_state_mode1Index == 1) {
                sysml_conditional_30 = 0.0;
            }
            else {
                if (sysml_state_mode1Index == 2) {
                    if ((((double)(io_2 * 255.0)) / 1023.0) < ((double)((int32_t)(((double)(io_2 * 255.0)) / 1023.0)))) {
                        sysml_conditional_32 = 1;
                    }
                    else {
                        sysml_conditional_32 = 0;
                    }
                    sysml_conditional_31 = ((int32_t)(((double)(io_2 * 255.0)) / 1023.0)) - sysml_conditional_32;
                }
                else {
                    sysml_conditional_31 = 0.0;
                }
                sysml_conditional_30 = sysml_conditional_31;
            }
            sysml_conditional_29 = sysml_conditional_30;
        }
        sysml_conditional_24 = sysml_conditional_29;
    }
    sysml_value_2 = sysml_conditional_24;
    *io_3 = sysml_value_0;
    *io_4 = sysml_value_1;
    *io_5 = sysml_value_2;
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
static double hw_value_io_2 = 0;
static uint32_t hw_sampled_io_2 = 0;
static bool hw_attempted_io_2 = false;
static double hw_value_io_3 = 0;
static double hw_value_io_4 = 0;
static double hw_value_io_5 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(A3, INPUT);
  pinMode(4, OUTPUT);
  analogWrite(4, (int)constrain((hw_value_io_3 * 1 + 0), 0.0, 255.0));
  pinMode(5, OUTPUT);
  analogWrite(5, (int)constrain((hw_value_io_4 * 1 + 0), 0.0, 255.0));
  pinMode(6, OUTPUT);
  analogWrite(6, (int)constrain((hw_value_io_5 * 1 + 0), 0.0, 255.0));
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 10UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (digitalRead(2) == LOW);
  }
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 10UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = (digitalRead(3) == LOW);
  }
  if (!hw_attempted_io_2 || (uint32_t)(hw_now - hw_sampled_io_2) >= 10UL) {
    hw_attempted_io_2 = true; hw_sampled_io_2 = hw_now;
    hw_value_io_2 = (analogRead(A3) * 1 + 0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5);
  analogWrite(4, (int)constrain((hw_value_io_3 * 1 + 0), 0.0, 255.0));
  analogWrite(5, (int)constrain((hw_value_io_4 * 1 + 0), 0.0, 255.0));
  analogWrite(6, (int)constrain((hw_value_io_5 * 1 + 0), 0.0, 255.0));
}

