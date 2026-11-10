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
extern int32_t sysml_state_tMs;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_1, double* io_0);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_tMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_tMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_1, double* io_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int32_t sysml_conditional_0 = 0;
    double sysml_conditional_1 = 0;
    if ((sysml_state_tMs + io_1) >= 2000) {
        sysml_conditional_0 = (sysml_state_tMs + io_1) - 2000;
    }
    else {
        sysml_conditional_0 = sysml_state_tMs + io_1;
    }
    sysml_state_tMs = sysml_conditional_0;
    double sysml_value_0 = 0;
    if (sysml_state_tMs < 1000) {
        sysml_conditional_1 = (((double)sysml_state_tMs) / 1000.0) * 255.0;
    }
    else {
        sysml_conditional_1 = (((double)(2000 - sysml_state_tMs)) / 1000.0) * 255.0;
    }
    sysml_value_0 = sysml_conditional_1;
    *io_0 = sysml_value_0;
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
static int32_t hw_value_io_1 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(5, OUTPUT);
  analogWrite(5, (int)constrain((hw_value_io_0 * 1.0 + 0.0), 0.0, 255.0));
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_1 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_1, &hw_value_io_0);
  analogWrite(5, (int)constrain((hw_value_io_0 * 1.0 + 0.0), 0.0, 255.0));
}
