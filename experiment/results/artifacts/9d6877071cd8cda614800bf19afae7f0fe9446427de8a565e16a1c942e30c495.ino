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
extern int32_t sysml_state_phaseMs;
/* Global Variable define of  */
extern bool sysml_state_ledOn;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_1, bool* io_0);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_phaseMs = 0;
/* Global Variable define of  */
bool sysml_state_ledOn = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_phaseMs = 0;
    sysml_state_ledOn = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_1, bool* io_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    sysml_state_phaseMs = (sysml_state_phaseMs + io_1);
    if (sysml_state_ledOn) {
        if ((sysml_state_phaseMs >= 1000)) {
            sysml_state_phaseMs = (sysml_state_phaseMs - 1000);
            sysml_state_ledOn = false;
        }
    }
    if ((!sysml_state_ledOn)) {
        if ((sysml_state_phaseMs >= 2000)) {
            sysml_state_phaseMs = (sysml_state_phaseMs - 2000);
            sysml_state_ledOn = true;
        }
    }
    sysml_lazy_0 = sysml_state_ledOn;
    if (sysml_lazy_0) {
        sysml_lazy_0 = sysml_state_phaseMs >= 3000;
    }
    if (sysml_lazy_0) {
        sysml_state_phaseMs = (sysml_state_phaseMs - 3000);
        sysml_state_ledOn = false;
    }
    sysml_lazy_1 = !sysml_state_ledOn;
    if (sysml_lazy_1) {
        sysml_lazy_1 = sysml_state_phaseMs >= 3000;
    }
    if (sysml_lazy_1) {
        sysml_state_phaseMs = (sysml_state_phaseMs - 3000);
        sysml_state_ledOn = true;
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_ledOn;
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
static bool hw_value_io_0 = false;
static int32_t hw_value_io_1 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, OUTPUT);
  digitalWrite(2, hw_value_io_0 ? HIGH : LOW);
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
  digitalWrite(2, hw_value_io_0 ? HIGH : LOW);
}
