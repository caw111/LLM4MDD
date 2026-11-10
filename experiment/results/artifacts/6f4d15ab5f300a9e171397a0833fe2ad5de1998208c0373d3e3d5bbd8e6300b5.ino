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
void ProgramPage1(int32_t* io_0);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t* io_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int32_t sysml_value_0 = 0;
    sysml_value_0 = 1234;
    *io_0 = sysml_value_0;
}
// END COMPILED SYSML MODEL

// LLM4MDD target-board: arduino:avr:uno
#include <TM1637Display.h>
#include <Arduino.h>
#include <math.h>
#if defined(ARDUINO) && !defined(ARDUINO_AVR_UNO)
#error "Selected Arduino board differs from the interface contract"
#endif
static uint32_t hw_last_step = 0;
static bool hw_started = false;
static int32_t hw_value_io_0 = 1234;
TM1637Display hw_display_io_0(2, 3);
static int32_t hw_display_previous_io_0 = INT32_MIN;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  hw_display_io_0.setBrightness(7, true);
  if (hw_value_io_0 != hw_display_previous_io_0) {
    if (hw_value_io_0 < -999 || hw_value_io_0 > 9999) hw_display_io_0.clear();
    else hw_display_io_0.showNumberDec((int)hw_value_io_0, false, 4, 0);
    hw_display_previous_io_0 = hw_value_io_0;
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
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(&hw_value_io_0);
  if (hw_value_io_0 != hw_display_previous_io_0) {
    if (hw_value_io_0 < -999 || hw_value_io_0 > 9999) hw_display_io_0.clear();
    else hw_display_io_0.showNumberDec((int)hw_value_io_0, false, 4, 0);
    hw_display_previous_io_0 = hw_value_io_0;
  }
}
