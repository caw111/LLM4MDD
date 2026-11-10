#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_015\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
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
extern bool sysml_state_previousPressed;
/* Global Variable define of  */
extern bool sysml_state_modeForced;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, double io_4, bool io_5, bool* io_2, double* io_3);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
bool sysml_state_modeForced = false;
/* Init function of CompositeActor named "ProgramPage1" */
#line 121 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_015\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 134 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_015\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 33 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_015\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousPressed = false;
    sysml_state_modeForced = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, double io_4, bool io_5, bool* io_2, double* io_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    double sysml_conditional_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    double sysml_conditional_7 = 0;
    sysml_lazy_0 = io_1;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        sysml_state_modeForced = (!sysml_state_modeForced);
    }
    sysml_state_previousPressed = io_1;
    bool sysml_value_0 = 0;
    if (sysml_state_modeForced) {
        sysml_conditional_1 = true;
    }
    else {
        sysml_lazy_2 = io_0 >= 0.0;
        if (sysml_lazy_2) {
            sysml_lazy_2 = io_5;
        }
        sysml_lazy_3 = sysml_lazy_2;
        if (sysml_lazy_3) {
            sysml_lazy_3 = io_4 > 0.0;
        }
        sysml_conditional_1 = sysml_lazy_3;
    }
    sysml_value_0 = sysml_conditional_1;
    double sysml_value_1 = 0;
    if (sysml_state_modeForced) {
        sysml_conditional_4 = 255.0;
    }
    else {
        sysml_lazy_5 = io_0 >= 0.0;
        if (sysml_lazy_5) {
            sysml_lazy_5 = io_5;
        }
        sysml_lazy_6 = sysml_lazy_5;
        if (sysml_lazy_6) {
            sysml_lazy_6 = io_4 > 0.0;
        }
        if (sysml_lazy_6) {
            sysml_conditional_7 = 255.0;
        }
        else {
            sysml_conditional_7 = 0.0;
        }
        sysml_conditional_4 = sysml_conditional_7;
    }
    sysml_value_1 = sysml_conditional_4;
    *io_2 = sysml_value_0;
    *io_3 = sysml_value_1;
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
static bool hw_value_io_2 = false;
static double hw_value_io_3 = 0;
static double hw_value_io_4 = 0;
static bool hw_value_io_5 = false;
static uint32_t hw_ultrasonic_sampled_0 = 0;
static bool hw_ultrasonic_attempted_0 = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(A0, INPUT);
  pinMode(2, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_2 ? HIGH : LOW);
  pinMode(9, OUTPUT);
  analogWrite(9, (int)constrain((hw_value_io_3 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(8, OUTPUT);
  digitalWrite(8, LOW);
  pinMode(3, INPUT);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 5UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (analogRead(A0) * 1.0 + 0.0);
  }
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 5UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = (digitalRead(2) == HIGH);
  }
  uint32_t hw_ultrasonic_now_0 = (uint32_t)millis();
  if (!hw_ultrasonic_attempted_0 || (uint32_t)(hw_ultrasonic_now_0 - hw_ultrasonic_sampled_0) >= 75UL) {
    hw_ultrasonic_attempted_0 = true; hw_ultrasonic_sampled_0 = hw_ultrasonic_now_0;
    digitalWrite(8, LOW); delayMicroseconds(2);
    digitalWrite(8, HIGH); delayMicroseconds(10); digitalWrite(8, LOW);
    unsigned long hw_ultrasonic_pulse_0 = pulseIn(3, HIGH, 30000UL);
    hw_value_io_5 = (hw_ultrasonic_pulse_0 >= 116UL && hw_ultrasonic_pulse_0 <= 23200UL);
    if (hw_value_io_5) hw_value_io_4 = hw_ultrasonic_pulse_0 / 58.0;
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_4, hw_value_io_5, &hw_value_io_2, &hw_value_io_3);
  digitalWrite(13, hw_value_io_2 ? HIGH : LOW);
  analogWrite(9, (int)constrain((hw_value_io_3 * 1.0 + 0.0), 0.0, 255.0));
}

