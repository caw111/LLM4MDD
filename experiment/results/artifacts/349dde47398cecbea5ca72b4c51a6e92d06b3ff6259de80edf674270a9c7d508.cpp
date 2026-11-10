#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_013\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
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
extern int32_t sysml_state_accumulatedMs;
/* Global Variable define of  */
extern bool sysml_state_lastPrintedAtOrAfterTick;
/* Global Variable define of  */
extern bool sysml_state_buttonPrev;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, int32_t io_2, bool* serial_0_trigger, double* serial_0_value_0, bool* serial_0_value_1);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_accumulatedMs = 0;
/* Global Variable define of  */
bool sysml_state_lastPrintedAtOrAfterTick = false;
/* Global Variable define of  */
bool sysml_state_buttonPrev = false;
/* Init function of CompositeActor named "ProgramPage1" */
#line 86 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_013\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 93 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_013\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_013\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_accumulatedMs = 0;
    sysml_state_lastPrintedAtOrAfterTick = false;
    sysml_state_buttonPrev = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, int32_t io_2, bool* serial_0_trigger, double* serial_0_value_0, bool* serial_0_value_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    sysml_state_accumulatedMs = (sysml_state_accumulatedMs + io_2);
    if ((sysml_state_accumulatedMs >= 1000)) {
        sysml_state_accumulatedMs = (sysml_state_accumulatedMs - 1000);
        sysml_state_lastPrintedAtOrAfterTick = true;
    }
    else {
        sysml_state_lastPrintedAtOrAfterTick = false;
    }
    sysml_state_buttonPrev = io_1;
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_lastPrintedAtOrAfterTick;
    double sysml_value_1 = 0;
    sysml_value_1 = io_0;
    bool sysml_value_2 = 0;
    sysml_value_2 = io_1;
    *serial_0_trigger = sysml_value_0;
    *serial_0_value_0 = sysml_value_1;
    *serial_0_value_1 = sysml_value_2;
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
static bool hw_value_io_1 = false;
static uint32_t hw_sampled_io_1 = 0;
static bool hw_attempted_io_1 = false;
static int32_t hw_value_io_2 = 0;
static bool hw_value_serial_0_trigger = false;
static double hw_value_serial_0_value_0 = 0;
static bool hw_value_serial_0_value_1 = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(2, INPUT);
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
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 10UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = (digitalRead(2) == HIGH);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_2 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0, &hw_value_serial_0_value_1);
  if (hw_value_serial_0_trigger) { Serial.print("A0_pot_value"); Serial.println(hw_value_serial_0_value_0); }
  if (hw_value_serial_0_trigger) { Serial.print("D2_button_state"); Serial.println(hw_value_serial_0_value_1); }
}

