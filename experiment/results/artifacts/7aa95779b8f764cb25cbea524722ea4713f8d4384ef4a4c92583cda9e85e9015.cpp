#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_023\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
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
void ProgramPage1(double io_0, bool io_1, int32_t* io_2, bool* io_3, double* io_4);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Init function of CompositeActor named "ProgramPage1" */
#line 101 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_023\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 118 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_023\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 25 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_023\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, int32_t* io_2, bool* io_3, double* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int32_t sysml_value_0 = 0;
    int32_t sysml_conditional_0 = 0;
    int32_t sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_conditional_3 = 0;
    double sysml_conditional_4 = 0;
    double sysml_conditional_5 = 0;
    if (io_1) {
        if (io_0 < ((double)((int32_t)io_0))) {
            sysml_conditional_1 = 1;
        }
        else {
            sysml_conditional_1 = 0;
        }
        sysml_conditional_0 = ((int32_t)io_0) - sysml_conditional_1;
    }
    else {
        sysml_conditional_0 = 0;
    }
    sysml_value_0 = sysml_conditional_0;
    bool sysml_value_1 = 0;
    sysml_lazy_2 = io_1;
    if (sysml_lazy_2) {
        sysml_lazy_2 = io_0 <= 30.0;
    }
    if (sysml_lazy_2) {
        sysml_conditional_3 = true;
    }
    else {
        sysml_conditional_3 = false;
    }
    sysml_value_1 = sysml_conditional_3;
    double sysml_value_2 = 0;
    if (io_1) {
        if (io_0 <= 30.0) {
            sysml_conditional_5 = ((double)(30.0 - io_0)) / 30.0;
        }
        else {
            sysml_conditional_5 = 0.0;
        }
        sysml_conditional_4 = sysml_conditional_5;
    }
    else {
        sysml_conditional_4 = 0.0;
    }
    sysml_value_2 = sysml_conditional_4;
    *io_2 = sysml_value_0;
    *io_3 = sysml_value_1;
    *io_4 = sysml_value_2;
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
static double hw_value_io_0 = 0;
static bool hw_value_io_1 = false;
static int32_t hw_value_io_2 = 0;
TM1637Display hw_display_io_2(4, 5);
static int32_t hw_display_previous_io_2 = INT32_MIN;
static bool hw_value_io_3 = false;
static double hw_value_io_4 = 0;
static uint32_t hw_ultrasonic_sampled_0 = 0;
static bool hw_ultrasonic_attempted_0 = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  hw_display_io_2.setBrightness(7, true);
  if (hw_value_io_2 != hw_display_previous_io_2) {
    if (hw_value_io_2 < -999 || hw_value_io_2 > 9999) hw_display_io_2.clear();
    else hw_display_io_2.showNumberDec((int)hw_value_io_2, false, 4, 0);
    hw_display_previous_io_2 = hw_value_io_2;
  }
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_3 ? HIGH : LOW);
  pinMode(9, OUTPUT);
  analogWrite(9, (int)constrain((hw_value_io_4 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(8, OUTPUT);
  digitalWrite(8, LOW);
  pinMode(2, INPUT);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  uint32_t hw_ultrasonic_now_0 = (uint32_t)millis();
  if (!hw_ultrasonic_attempted_0 || (uint32_t)(hw_ultrasonic_now_0 - hw_ultrasonic_sampled_0) >= 65UL) {
    hw_ultrasonic_attempted_0 = true; hw_ultrasonic_sampled_0 = hw_ultrasonic_now_0;
    digitalWrite(8, LOW); delayMicroseconds(2);
    digitalWrite(8, HIGH); delayMicroseconds(10); digitalWrite(8, LOW);
    unsigned long hw_ultrasonic_pulse_0 = pulseIn(2, HIGH, 30000UL);
    hw_value_io_1 = (hw_ultrasonic_pulse_0 >= 116UL && hw_ultrasonic_pulse_0 <= 23200UL);
    if (hw_value_io_1) hw_value_io_0 = hw_ultrasonic_pulse_0 / 58.0;
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4);
  if (hw_value_io_2 != hw_display_previous_io_2) {
    if (hw_value_io_2 < -999 || hw_value_io_2 > 9999) hw_display_io_2.clear();
    else hw_display_io_2.showNumberDec((int)hw_value_io_2, false, 4, 0);
    hw_display_previous_io_2 = hw_value_io_2;
  }
  digitalWrite(13, hw_value_io_3 ? HIGH : LOW);
  analogWrite(9, (int)constrain((hw_value_io_4 * 1.0 + 0.0), 0.0, 255.0));
}

