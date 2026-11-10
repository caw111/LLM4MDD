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
extern bool sysml_state_automaticMode;
/* Global Variable define of  */
extern bool sysml_state_previousAutoPressed;
/* Global Variable define of  */
extern bool sysml_state_previousManualPressed;
/* Global Variable define of  */
extern double sysml_state_rampPhaseMs;
/* Global Variable define of  */
extern double sysml_state_directionDurationMs;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double io_2, int32_t io_4, double* io_3);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_automaticMode = true;
/* Global Variable define of  */
bool sysml_state_previousAutoPressed = false;
/* Global Variable define of  */
bool sysml_state_previousManualPressed = false;
/* Global Variable define of  */
double sysml_state_rampPhaseMs = 0.0;
/* Global Variable define of  */
double sysml_state_directionDurationMs = 2000.0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_automaticMode = true;
    sysml_state_previousAutoPressed = false;
    sysml_state_previousManualPressed = false;
    sysml_state_rampPhaseMs = 0.0;
    sysml_state_directionDurationMs = 2000.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double io_2, int32_t io_4, double* io_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_arithmetic_0 = 0;
    double sysml_arithmetic_1 = 0;
    double sysml_arithmetic_2 = 0;
    int32_t sysml_conditional_3 = 0;
    int32_t sysml_arithmetic_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    double sysml_conditional_7 = 0;
    double sysml_conditional_8 = 0;
    double sysml_arithmetic_9 = 0;
    double sysml_conditional_10 = 0;
    double sysml_conditional_11 = 0;
    if (sysml_state_automaticMode) {
        sysml_state_rampPhaseMs = (sysml_state_rampPhaseMs + io_4);
        sysml_arithmetic_0 = 2.0 * sysml_state_directionDurationMs;
        sysml_arithmetic_1 = 2.0 * sysml_state_directionDurationMs;
        sysml_arithmetic_2 = 2.0 * sysml_state_directionDurationMs;
        if ((((double)sysml_state_rampPhaseMs) / sysml_arithmetic_1) < ((double)((int32_t)(((double)sysml_state_rampPhaseMs) / sysml_arithmetic_2)))) {
            sysml_conditional_3 = 1;
        }
        else {
            sysml_conditional_3 = 0;
        }
        sysml_arithmetic_4 = ((int32_t)(((double)sysml_state_rampPhaseMs) / sysml_arithmetic_0)) - sysml_conditional_3;
        sysml_state_rampPhaseMs = sysml_state_rampPhaseMs - ((2.0 * sysml_state_directionDurationMs) * sysml_arithmetic_4);
    }
    sysml_lazy_5 = io_0;
    if (sysml_lazy_5) {
        sysml_lazy_5 = !sysml_state_previousAutoPressed;
    }
    if (sysml_lazy_5) {
        sysml_state_automaticMode = true;
        sysml_state_rampPhaseMs = 0.0;
    }
    else {
        sysml_lazy_6 = io_1;
        if (sysml_lazy_6) {
            sysml_lazy_6 = !sysml_state_previousManualPressed;
        }
        if (sysml_lazy_6) {
            sysml_state_automaticMode = false;
        }
    }
    sysml_state_previousAutoPressed = io_0;
    sysml_state_previousManualPressed = io_1;
    double sysml_value_0 = 0;
    if (sysml_state_automaticMode) {
        if (sysml_state_rampPhaseMs <= sysml_state_directionDurationMs) {
            sysml_conditional_8 = ((double)(255.0 * sysml_state_rampPhaseMs)) / sysml_state_directionDurationMs;
        }
        else {
            sysml_arithmetic_9 = (2.0 * sysml_state_directionDurationMs) - sysml_state_rampPhaseMs;
            sysml_conditional_8 = ((double)(255.0 * sysml_arithmetic_9)) / sysml_state_directionDurationMs;
        }
        sysml_conditional_7 = sysml_conditional_8;
    }
    else {
        if (io_2 < 0.0) {
            sysml_conditional_10 = 0.0;
        }
        else {
            if (io_2 > 1023.0) {
                sysml_conditional_11 = 255.0;
            }
            else {
                sysml_conditional_11 = ((double)(io_2 * 255.0)) / 1023.0;
            }
            sysml_conditional_10 = sysml_conditional_11;
        }
        sysml_conditional_7 = sysml_conditional_10;
    }
    sysml_value_0 = sysml_conditional_7;
    *io_3 = sysml_value_0;
}
// END COMPILED SYSML MODEL

// LLM4MDD target-board: arduino:avr:uno
#include <Arduino.h>
#include <math.h>
#if defined(ARDUINO) && !defined(ARDUINO_AVR_UNO)
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
static int32_t hw_value_io_4 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(A0, INPUT);
  pinMode(9, OUTPUT);
  analogWrite(9, (int)constrain((hw_value_io_3 * 1.0 + 0.0), 0.0, 255.0));
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 5UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (digitalRead(2) == LOW);
  }
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 5UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = (digitalRead(3) == LOW);
  }
  if (!hw_attempted_io_2 || (uint32_t)(hw_now - hw_sampled_io_2) >= 5UL) {
    hw_attempted_io_2 = true; hw_sampled_io_2 = hw_now;
    hw_value_io_2 = (analogRead(A0) * 1.0 + 0.0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_4 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, hw_value_io_4, &hw_value_io_3);
  analogWrite(9, (int)constrain((hw_value_io_3 * 1.0 + 0.0), 0.0, 255.0));
}
