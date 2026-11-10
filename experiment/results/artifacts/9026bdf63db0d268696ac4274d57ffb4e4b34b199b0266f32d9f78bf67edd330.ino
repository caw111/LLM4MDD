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
extern bool sysml_state_modeManual;
/* Global Variable define of  */
extern bool sysml_state_prevBtn1Pressed;
/* Global Variable define of  */
extern bool sysml_state_prevBtn2Pressed;
/* Global Variable define of  */
extern int32_t sysml_state_debounceBtn1Remaining;
/* Global Variable define of  */
extern int32_t sysml_state_debounceBtn2Remaining;
/* Global Variable define of  */
extern bool sysml_state_stableBtn1Pressed;
/* Global Variable define of  */
extern bool sysml_state_stableBtn2Pressed;
/* Global Variable define of  */
extern int32_t sysml_state_autoPhase;
/* Global Variable define of  */
extern int32_t sysml_state_transitionElapsed;
/* Global Variable define of  */
extern bool sysml_state_toMax;
/* Global Variable define of  */
extern bool sysml_state_prevStableBtn1Pressed;
/* Global Variable define of  */
extern bool sysml_state_prevStableBtn2Pressed;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_1, bool io_2, double io_3, double io_4, double io_5, int32_t io_6, double* io_0);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_modeManual = true;
/* Global Variable define of  */
bool sysml_state_prevBtn1Pressed = false;
/* Global Variable define of  */
bool sysml_state_prevBtn2Pressed = false;
/* Global Variable define of  */
int32_t sysml_state_debounceBtn1Remaining = 0;
/* Global Variable define of  */
int32_t sysml_state_debounceBtn2Remaining = 0;
/* Global Variable define of  */
bool sysml_state_stableBtn1Pressed = false;
/* Global Variable define of  */
bool sysml_state_stableBtn2Pressed = false;
/* Global Variable define of  */
int32_t sysml_state_autoPhase = 0;
/* Global Variable define of  */
int32_t sysml_state_transitionElapsed = 0;
/* Global Variable define of  */
bool sysml_state_toMax = true;
/* Global Variable define of  */
bool sysml_state_prevStableBtn1Pressed = false;
/* Global Variable define of  */
bool sysml_state_prevStableBtn2Pressed = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_modeManual = true;
    sysml_state_prevBtn1Pressed = false;
    sysml_state_prevBtn2Pressed = false;
    sysml_state_debounceBtn1Remaining = 0;
    sysml_state_debounceBtn2Remaining = 0;
    sysml_state_stableBtn1Pressed = false;
    sysml_state_stableBtn2Pressed = false;
    sysml_state_autoPhase = 0;
    sysml_state_transitionElapsed = 0;
    sysml_state_toMax = true;
    sysml_state_prevStableBtn1Pressed = false;
    sysml_state_prevStableBtn2Pressed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_1, bool io_2, double io_3, double io_4, double io_5, int32_t io_6, double* io_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
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
    double sysml_arithmetic_19 = 0;
    double sysml_conditional_20 = 0;
    double sysml_arithmetic_21 = 0;
    double sysml_arithmetic_22 = 0;
    double sysml_conditional_23 = 0;
    double sysml_conditional_24 = 0;
    double sysml_conditional_25 = 0;
    double sysml_conditional_26 = 0;
    double sysml_arithmetic_27 = 0;
    double sysml_arithmetic_28 = 0;
    double sysml_conditional_29 = 0;
    double sysml_arithmetic_30 = 0;
    double sysml_arithmetic_31 = 0;
    if ((sysml_state_debounceBtn1Remaining > 0)) {
        sysml_state_debounceBtn1Remaining = (sysml_state_debounceBtn1Remaining - io_6);
        if (((sysml_state_debounceBtn1Remaining - io_6) <= 0)) {
            sysml_state_stableBtn1Pressed = io_1;
        }
    }
    sysml_lazy_0 = sysml_state_debounceBtn1Remaining <= 0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_1;
    }
    sysml_lazy_1 = sysml_lazy_0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_prevBtn1Pressed;
    }
    if (sysml_lazy_1) {
        sysml_state_debounceBtn1Remaining = 150;
    }
    if ((sysml_state_debounceBtn2Remaining > 0)) {
        sysml_state_debounceBtn2Remaining = (sysml_state_debounceBtn2Remaining - io_6);
        if (((sysml_state_debounceBtn2Remaining - io_6) <= 0)) {
            sysml_state_stableBtn2Pressed = io_2;
        }
    }
    sysml_lazy_2 = sysml_state_debounceBtn2Remaining <= 0;
    if (sysml_lazy_2) {
        sysml_lazy_2 = io_2;
    }
    sysml_lazy_3 = sysml_lazy_2;
    if (sysml_lazy_3) {
        sysml_lazy_3 = !sysml_state_prevBtn2Pressed;
    }
    if (sysml_lazy_3) {
        sysml_state_debounceBtn2Remaining = 150;
    }
    sysml_state_prevBtn1Pressed = io_1;
    sysml_state_prevBtn2Pressed = io_2;
    sysml_lazy_4 = sysml_state_modeManual;
    if (sysml_lazy_4) {
        sysml_lazy_4 = sysml_state_stableBtn1Pressed;
    }
    sysml_lazy_5 = sysml_lazy_4;
    if (sysml_lazy_5) {
        sysml_lazy_5 = !sysml_state_prevStableBtn1Pressed;
    }
    if (sysml_lazy_5) {
        sysml_state_modeManual = false;
        sysml_state_autoPhase = 0;
        sysml_state_transitionElapsed = 0;
        sysml_state_toMax = true;
    }
    sysml_lazy_6 = !sysml_state_modeManual;
    if (sysml_lazy_6) {
        sysml_lazy_6 = sysml_state_stableBtn2Pressed;
    }
    sysml_lazy_7 = sysml_lazy_6;
    if (sysml_lazy_7) {
        sysml_lazy_7 = !sysml_state_prevStableBtn2Pressed;
    }
    if (sysml_lazy_7) {
        sysml_state_modeManual = true;
        sysml_state_transitionElapsed = 0;
    }
    if ((!sysml_state_modeManual)) {
        sysml_state_transitionElapsed = (sysml_state_transitionElapsed + io_6);
        if ((sysml_state_transitionElapsed >= 2000)) {
            sysml_state_transitionElapsed = (sysml_state_transitionElapsed - 2000);
            sysml_state_toMax = (!sysml_state_toMax);
            sysml_state_autoPhase = (sysml_state_autoPhase + 1);
        }
    }
    sysml_state_prevStableBtn1Pressed = sysml_state_stableBtn1Pressed;
    sysml_state_prevStableBtn2Pressed = sysml_state_stableBtn2Pressed;
    double sysml_value_0 = 0;
    if (sysml_state_modeManual) {
        if (io_3 <= 0.0) {
            sysml_conditional_9 = 0.0;
        }
        else {
            if (io_3 >= 1023.0) {
                sysml_conditional_10 = 255.0;
            }
            else {
                sysml_conditional_10 = ((double)(io_3 * 255.0)) / 1023.0;
            }
            sysml_conditional_9 = sysml_conditional_10;
        }
        sysml_conditional_8 = sysml_conditional_9;
    }
    else {
        if (sysml_state_transitionElapsed >= 2000) {
            if (sysml_state_toMax) {
                if (io_5 >= 1023.0) {
                    sysml_conditional_13 = 255.0;
                }
                else {
                    sysml_conditional_13 = ((double)(io_5 * 255.0)) / 1023.0;
                }
                sysml_conditional_12 = sysml_conditional_13;
            }
            else {
                if (io_4 <= 0.0) {
                    sysml_conditional_14 = 0.0;
                }
                else {
                    if (io_4 >= 1023.0) {
                        sysml_conditional_15 = 255.0;
                    }
                    else {
                        sysml_conditional_15 = ((double)(io_4 * 255.0)) / 1023.0;
                    }
                    sysml_conditional_14 = sysml_conditional_15;
                }
                sysml_conditional_12 = sysml_conditional_14;
            }
            sysml_conditional_11 = sysml_conditional_12;
        }
        else {
            if (sysml_state_toMax) {
                if (io_4 <= 0.0) {
                    if (io_5 >= 1023.0) {
                        sysml_conditional_18 = ((double)(sysml_state_transitionElapsed * 255.0)) / 2000.0;
                    }
                    else {
                        sysml_arithmetic_19 = ((double)sysml_state_transitionElapsed) / 2000.0;
                        sysml_conditional_18 = (((double)(io_5 * 255.0)) / 1023.0) * sysml_arithmetic_19;
                    }
                    sysml_conditional_17 = sysml_conditional_18;
                }
                else {
                    if (io_5 >= 1023.0) {
                        sysml_arithmetic_21 = ((double)sysml_state_transitionElapsed) / 2000.0;
                        sysml_conditional_20 = (((double)(io_4 * 255.0)) / 1023.0) + ((255.0 - (((double)(io_4 * 255.0)) / 1023.0)) * sysml_arithmetic_21);
                    }
                    else {
                        sysml_arithmetic_22 = ((double)sysml_state_transitionElapsed) / 2000.0;
                        sysml_conditional_20 = (((double)(io_4 * 255.0)) / 1023.0) + (((((double)(io_5 * 255.0)) / 1023.0) - (((double)(io_4 * 255.0)) / 1023.0)) * sysml_arithmetic_22);
                    }
                    sysml_conditional_17 = sysml_conditional_20;
                }
                sysml_conditional_16 = sysml_conditional_17;
            }
            else {
                if (io_5 <= 0.0) {
                    if (io_4 >= 1023.0) {
                        sysml_conditional_24 = 255.0;
                    }
                    else {
                        sysml_conditional_24 = ((double)(io_4 * 255.0)) / 1023.0;
                    }
                    sysml_conditional_23 = sysml_conditional_24;
                }
                else {
                    if (io_4 <= 0.0) {
                        if (io_5 >= 1023.0) {
                            sysml_arithmetic_27 = ((double)sysml_state_transitionElapsed) / 2000.0;
                            sysml_conditional_26 = 255.0 + (((((double)(io_4 * 255.0)) / 1023.0) - 255.0) * sysml_arithmetic_27);
                        }
                        else {
                            sysml_arithmetic_28 = ((double)sysml_state_transitionElapsed) / 2000.0;
                            sysml_conditional_26 = (((double)(io_5 * 255.0)) / 1023.0) + (((((double)(io_4 * 255.0)) / 1023.0) - (((double)(io_5 * 255.0)) / 1023.0)) * sysml_arithmetic_28);
                        }
                        sysml_conditional_25 = sysml_conditional_26;
                    }
                    else {
                        if (io_5 >= 1023.0) {
                            sysml_arithmetic_30 = ((double)sysml_state_transitionElapsed) / 2000.0;
                            sysml_conditional_29 = 255.0 + (((((double)(io_4 * 255.0)) / 1023.0) - 255.0) * sysml_arithmetic_30);
                        }
                        else {
                            sysml_arithmetic_31 = ((double)sysml_state_transitionElapsed) / 2000.0;
                            sysml_conditional_29 = (((double)(io_5 * 255.0)) / 1023.0) + (((((double)(io_4 * 255.0)) / 1023.0) - (((double)(io_5 * 255.0)) / 1023.0)) * sysml_arithmetic_31);
                        }
                        sysml_conditional_25 = sysml_conditional_29;
                    }
                    sysml_conditional_23 = sysml_conditional_25;
                }
                sysml_conditional_16 = sysml_conditional_23;
            }
            sysml_conditional_11 = sysml_conditional_16;
        }
        sysml_conditional_8 = sysml_conditional_11;
    }
    sysml_value_0 = sysml_conditional_8;
    *io_0 = sysml_value_0;
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
static bool hw_value_io_1 = false;
static uint32_t hw_sampled_io_1 = 0;
static bool hw_attempted_io_1 = false;
static bool hw_value_io_2 = false;
static uint32_t hw_sampled_io_2 = 0;
static bool hw_attempted_io_2 = false;
static double hw_value_io_3 = 0;
static uint32_t hw_sampled_io_3 = 0;
static bool hw_attempted_io_3 = false;
static double hw_value_io_4 = 0;
static uint32_t hw_sampled_io_4 = 0;
static bool hw_attempted_io_4 = false;
static double hw_value_io_5 = 0;
static uint32_t hw_sampled_io_5 = 0;
static bool hw_attempted_io_5 = false;
static int32_t hw_value_io_6 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, OUTPUT);
  analogWrite(2, (int)constrain((hw_value_io_0 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 5UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = (digitalRead(3) == LOW);
  }
  if (!hw_attempted_io_2 || (uint32_t)(hw_now - hw_sampled_io_2) >= 5UL) {
    hw_attempted_io_2 = true; hw_sampled_io_2 = hw_now;
    hw_value_io_2 = (digitalRead(4) == LOW);
  }
  if (!hw_attempted_io_3 || (uint32_t)(hw_now - hw_sampled_io_3) >= 5UL) {
    hw_attempted_io_3 = true; hw_sampled_io_3 = hw_now;
    hw_value_io_3 = (analogRead(A3) * 1.0 + 0.0);
  }
  if (!hw_attempted_io_4 || (uint32_t)(hw_now - hw_sampled_io_4) >= 5UL) {
    hw_attempted_io_4 = true; hw_sampled_io_4 = hw_now;
    hw_value_io_4 = (analogRead(A4) * 1.0 + 0.0);
  }
  if (!hw_attempted_io_5 || (uint32_t)(hw_now - hw_sampled_io_5) >= 5UL) {
    hw_attempted_io_5 = true; hw_sampled_io_5 = hw_now;
    hw_value_io_5 = (analogRead(A5) * 1.0 + 0.0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_6 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_1, hw_value_io_2, hw_value_io_3, hw_value_io_4, hw_value_io_5, hw_value_io_6, &hw_value_io_0);
  analogWrite(2, (int)constrain((hw_value_io_0 * 1.0 + 0.0), 0.0, 255.0));
}
