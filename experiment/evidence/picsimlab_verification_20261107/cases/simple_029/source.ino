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
extern double sysml_state_io_0_clamped;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double* io_1, bool* io_2);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_io_0_clamped = 0.0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_io_0_clamped = 0.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double* io_1, bool* io_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_conditional_0 = 0;
    double sysml_conditional_1 = 0;
    double sysml_conditional_2 = 0;
    double sysml_conditional_3 = 0;
    if (io_0 < 0.0) {
        sysml_conditional_0 = 0.0;
    }
    else {
        if (io_0 > 1023.0) {
            sysml_conditional_1 = 1023.0;
        }
        else {
            sysml_conditional_1 = io_0;
        }
        sysml_conditional_0 = sysml_conditional_1;
    }
    sysml_state_io_0_clamped = sysml_conditional_0;
    double sysml_value_0 = 0;
    if (sysml_state_io_0_clamped == 0.0) {
        sysml_conditional_2 = 1023.0;
    }
    else {
        if (sysml_state_io_0_clamped == 1023.0) {
            sysml_conditional_3 = 0.0;
        }
        else {
            sysml_conditional_3 = 1023.0 - sysml_state_io_0_clamped;
        }
        sysml_conditional_2 = sysml_conditional_3;
    }
    sysml_value_0 = sysml_conditional_2;
    bool sysml_value_1 = 0;
    sysml_value_1 = (io_0 <= 512.0);
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
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
static double hw_value_io_0 = 0;
static uint32_t hw_sampled_io_0 = 0;
static bool hw_attempted_io_0 = false;
static double hw_value_io_1 = 0;
static bool hw_value_io_2 = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(A0, INPUT);
  pinMode(9, OUTPUT);
  analogWrite(9, (int)constrain((hw_value_io_1 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_2 ? HIGH : LOW);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 10UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (analogRead(A0) * 1.0 + 0.0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, &hw_value_io_1, &hw_value_io_2);
  analogWrite(9, (int)constrain((hw_value_io_1 * 1.0 + 0.0), 0.0, 255.0));
  digitalWrite(13, hw_value_io_2 ? HIGH : LOW);
}
