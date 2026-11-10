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
extern int32_t sysml_state_ledIndex;
/* Global Variable define of  */
extern int32_t sysml_state_holdMs;
/* Global Variable define of  */
extern bool sysml_state_previousPressed;
/* Global Variable define of  */
extern bool sysml_state_pressArmed;
/* Global Variable define of  */
extern bool sysml_state_pressedQualified;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, double* io_1, double* io_2, double* io_3);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_ledIndex = 7;
/* Global Variable define of  */
int32_t sysml_state_holdMs = 0;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
bool sysml_state_pressArmed = false;
/* Global Variable define of  */
bool sysml_state_pressedQualified = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_ledIndex = 7;
    sysml_state_holdMs = 0;
    sysml_state_previousPressed = false;
    sysml_state_pressArmed = false;
    sysml_state_pressedQualified = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, double* io_1, double* io_2, double* io_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    int32_t sysml_conditional_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    double sysml_conditional_9 = 0;
    bool sysml_lazy_10 = 0;
    bool sysml_lazy_11 = 0;
    bool sysml_lazy_12 = 0;
    double sysml_conditional_13 = 0;
    bool sysml_lazy_14 = 0;
    bool sysml_lazy_15 = 0;
    bool sysml_lazy_16 = 0;
    double sysml_conditional_17 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_pressArmed;
    }
    if (sysml_lazy_0) {
        sysml_state_holdMs = 0;
        sysml_state_pressArmed = true;
    }
    sysml_lazy_1 = io_0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = sysml_state_pressArmed;
    }
    if (sysml_lazy_1) {
        sysml_state_holdMs = (sysml_state_holdMs + 5);
    }
    sysml_lazy_2 = !io_0;
    if (sysml_lazy_2) {
        sysml_lazy_2 = sysml_state_pressArmed;
    }
    if (sysml_lazy_2) {
        sysml_state_pressedQualified = (sysml_state_holdMs >= 150);
        sysml_state_pressArmed = false;
        sysml_state_holdMs = 0;
    }
    sysml_lazy_3 = sysml_state_pressedQualified;
    if (sysml_lazy_3) {
        sysml_lazy_3 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_3) {
        if (sysml_state_ledIndex == 7) {
            sysml_conditional_4 = 0;
        }
        else {
            sysml_conditional_4 = sysml_state_ledIndex + 1;
        }
        sysml_state_ledIndex = sysml_conditional_4;
        sysml_state_pressedQualified = false;
    }
    sysml_lazy_5 = sysml_state_pressedQualified;
    if (sysml_lazy_5) {
        sysml_lazy_5 = sysml_state_previousPressed;
    }
    if (sysml_lazy_5) {
        sysml_state_pressedQualified = false;
    }
    sysml_state_previousPressed = io_0;
    double sysml_value_0 = 0;
    sysml_lazy_6 = sysml_state_ledIndex == 1;
    if (!sysml_lazy_6) {
        sysml_lazy_6 = sysml_state_ledIndex == 3;
    }
    sysml_lazy_7 = sysml_lazy_6;
    if (!sysml_lazy_7) {
        sysml_lazy_7 = sysml_state_ledIndex == 5;
    }
    sysml_lazy_8 = sysml_lazy_7;
    if (!sysml_lazy_8) {
        sysml_lazy_8 = sysml_state_ledIndex == 6;
    }
    if (sysml_lazy_8) {
        sysml_conditional_9 = 255.0;
    }
    else {
        sysml_conditional_9 = 0.0;
    }
    sysml_value_0 = sysml_conditional_9;
    double sysml_value_1 = 0;
    sysml_lazy_10 = sysml_state_ledIndex == 1;
    if (!sysml_lazy_10) {
        sysml_lazy_10 = sysml_state_ledIndex == 3;
    }
    sysml_lazy_11 = sysml_lazy_10;
    if (!sysml_lazy_11) {
        sysml_lazy_11 = sysml_state_ledIndex == 4;
    }
    sysml_lazy_12 = sysml_lazy_11;
    if (!sysml_lazy_12) {
        sysml_lazy_12 = sysml_state_ledIndex == 6;
    }
    if (sysml_lazy_12) {
        sysml_conditional_13 = 255.0;
    }
    else {
        sysml_conditional_13 = 0.0;
    }
    sysml_value_1 = sysml_conditional_13;
    double sysml_value_2 = 0;
    sysml_lazy_14 = sysml_state_ledIndex == 2;
    if (!sysml_lazy_14) {
        sysml_lazy_14 = sysml_state_ledIndex == 4;
    }
    sysml_lazy_15 = sysml_lazy_14;
    if (!sysml_lazy_15) {
        sysml_lazy_15 = sysml_state_ledIndex == 5;
    }
    sysml_lazy_16 = sysml_lazy_15;
    if (!sysml_lazy_16) {
        sysml_lazy_16 = sysml_state_ledIndex == 6;
    }
    if (sysml_lazy_16) {
        sysml_conditional_17 = 255.0;
    }
    else {
        sysml_conditional_17 = 0.0;
    }
    sysml_value_2 = sysml_conditional_17;
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
static bool hw_value_io_0 = false;
static uint32_t hw_sampled_io_0 = 0;
static bool hw_attempted_io_0 = false;
static double hw_value_io_1 = 0;
static double hw_value_io_2 = 0;
static double hw_value_io_3 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, INPUT);
  pinMode(3, OUTPUT);
  analogWrite(3, (int)constrain((hw_value_io_1 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(4, OUTPUT);
  analogWrite(4, (int)constrain((hw_value_io_2 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(5, OUTPUT);
  analogWrite(5, (int)constrain((hw_value_io_3 * 1.0 + 0.0), 0.0, 255.0));
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 5UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (digitalRead(2) == HIGH);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3);
  analogWrite(3, (int)constrain((hw_value_io_1 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(4, (int)constrain((hw_value_io_2 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(5, (int)constrain((hw_value_io_3 * 1.0 + 0.0), 0.0, 255.0));
}
