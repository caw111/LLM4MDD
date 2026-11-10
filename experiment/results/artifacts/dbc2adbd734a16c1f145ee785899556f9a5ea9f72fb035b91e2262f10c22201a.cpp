#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_020\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
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
extern double sysml_state_lightThreshold;
/* Global Variable define of  */
extern double sysml_state_tempThreshold;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, int32_t io_3, bool* io_2, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, bool* serial_1_trigger, int32_t* serial_1_value_0);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_elapsedAccumMs = 0;
/* Global Variable define of  */
double sysml_state_lightThreshold = 0.0;
/* Global Variable define of  */
double sysml_state_tempThreshold = 0.0;
/* Init function of CompositeActor named "ProgramPage1" */
#line 118 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_020\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 127 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_020\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_020\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_elapsedAccumMs = 0;
    sysml_state_lightThreshold = 0.0;
    sysml_state_tempThreshold = 0.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, int32_t io_3, bool* io_2, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, bool* serial_1_trigger, int32_t* serial_1_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    sysml_state_elapsedAccumMs = (sysml_state_elapsedAccumMs + io_3);
    if ((sysml_state_elapsedAccumMs >= 1000)) {
        sysml_state_elapsedAccumMs = (sysml_state_elapsedAccumMs - 1000);
    }
    bool sysml_value_0 = 0;
    sysml_lazy_0 = io_0 > sysml_state_lightThreshold;
    if (!sysml_lazy_0) {
        sysml_lazy_0 = io_1 > sysml_state_tempThreshold;
    }
    sysml_value_0 = sysml_lazy_0;
    bool sysml_value_1 = 0;
    sysml_lazy_1 = sysml_state_elapsedAccumMs >= 0;
    if (sysml_lazy_1) {
        sysml_lazy_2 = io_0 > sysml_state_lightThreshold;
        if (!sysml_lazy_2) {
            sysml_lazy_2 = io_1 > sysml_state_tempThreshold;
        }
        sysml_lazy_3 = sysml_lazy_2;
        if (!sysml_lazy_3) {
            sysml_lazy_3 = true;
        }
        sysml_lazy_1 = sysml_lazy_3;
    }
    sysml_value_1 = sysml_lazy_1;
    double sysml_value_2 = 0;
    sysml_value_2 = io_0;
    double sysml_value_3 = 0;
    sysml_value_3 = io_1;
    bool sysml_value_4 = 0;
    sysml_lazy_4 = io_0 > sysml_state_lightThreshold;
    if (!sysml_lazy_4) {
        sysml_lazy_4 = io_1 > sysml_state_tempThreshold;
    }
    sysml_value_4 = sysml_lazy_4;
    int32_t sysml_value_5 = 0;
    sysml_value_5 = 1;
    *io_2 = sysml_value_0;
    *serial_0_trigger = sysml_value_1;
    *serial_0_value_0 = sysml_value_2;
    *serial_0_value_1 = sysml_value_3;
    *serial_1_trigger = sysml_value_4;
    *serial_1_value_0 = sysml_value_5;
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
static uint32_t hw_sampled_io_1 = 0;
static bool hw_attempted_io_1 = false;
static bool hw_value_io_2 = false;
static int32_t hw_value_io_3 = 0;
static bool hw_value_serial_0_trigger = false;
static double hw_value_serial_0_value_0 = 0;
static double hw_value_serial_0_value_1 = 0;
static bool hw_value_serial_1_trigger = false;
static int32_t hw_value_serial_1_value_0 = 1;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
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
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 10UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = (analogRead(A1) * 1.0 + 0.0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_3 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_3, &hw_value_io_2, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0, &hw_value_serial_0_value_1, &hw_value_serial_1_trigger, &hw_value_serial_1_value_0);
  digitalWrite(13, hw_value_io_2 ? HIGH : LOW);
  if (hw_value_serial_0_trigger) { Serial.print("Light"); Serial.println(hw_value_serial_0_value_0); }
  if (hw_value_serial_0_trigger) { Serial.print("Temperature"); Serial.println(hw_value_serial_0_value_1); }
  if (hw_value_serial_1_trigger) { Serial.print("WARNING"); Serial.println(hw_value_serial_1_value_0); }
}

