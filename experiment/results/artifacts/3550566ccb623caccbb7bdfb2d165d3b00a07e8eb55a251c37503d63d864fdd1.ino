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
extern int32_t sysml_state_elapsedAccumMs;
/* Global Variable define of  */
extern int32_t sysml_state_cycleAccumMs;
/* Global Variable define of  */
extern bool sysml_state_previousBecameActive;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_0, int32_t* io_1);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_elapsedAccumMs = 0;
/* Global Variable define of  */
int32_t sysml_state_cycleAccumMs = 0;
/* Global Variable define of  */
bool sysml_state_previousBecameActive = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_elapsedAccumMs = 0;
    sysml_state_cycleAccumMs = 0;
    sysml_state_previousBecameActive = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_0, int32_t* io_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    int32_t sysml_conditional_3 = 0;
    sysml_state_elapsedAccumMs = (sysml_state_elapsedAccumMs + io_0);
    sysml_state_cycleAccumMs = (sysml_state_cycleAccumMs + io_0);
    sysml_lazy_0 = sysml_state_cycleAccumMs >= 2000;
    if (!sysml_lazy_0) {
        sysml_lazy_0 = sysml_state_elapsedAccumMs >= 2000;
    }
    if (sysml_lazy_0) {
        sysml_state_cycleAccumMs = (sysml_state_cycleAccumMs - 2000);
    }
    sysml_lazy_1 = sysml_state_cycleAccumMs < 200;
    if (sysml_lazy_1) {
        sysml_lazy_1 = sysml_state_cycleAccumMs >= 0;
    }
    if (sysml_lazy_1) {
        sysml_state_previousBecameActive = true;
    }
    else {
        sysml_state_previousBecameActive = false;
    }
    int32_t sysml_value_0 = 0;
    sysml_lazy_2 = sysml_state_cycleAccumMs < 200;
    if (sysml_lazy_2) {
        sysml_lazy_2 = sysml_state_cycleAccumMs >= 0;
    }
    if (sysml_lazy_2) {
        sysml_conditional_3 = 31;
    }
    else {
        sysml_conditional_3 = 0;
    }
    sysml_value_0 = sysml_conditional_3;
    *io_1 = sysml_value_0;
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
static int32_t hw_value_io_0 = 0;
static int32_t hw_value_io_1 = 31;
static int32_t hw_tone_io_1 = -1;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(6, OUTPUT);
  int32_t hw_tone_command_io_1 = (hw_value_io_1 >= 31 && hw_value_io_1 <= 65535) ? hw_value_io_1 : 0;
  if (hw_tone_command_io_1 != hw_tone_io_1) {
    if (hw_tone_command_io_1 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_1);
    hw_tone_io_1 = hw_tone_command_io_1;
  }
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_0 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, &hw_value_io_1);
  int32_t hw_tone_command_io_1 = (hw_value_io_1 >= 31 && hw_value_io_1 <= 65535) ? hw_value_io_1 : 0;
  if (hw_tone_command_io_1 != hw_tone_io_1) {
    if (hw_tone_command_io_1 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_1);
    hw_tone_io_1 = hw_tone_command_io_1;
  }
}
