#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_002\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
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
extern bool sysml_state_modeAuto;
/* Global Variable define of  */
extern bool sysml_state_previousPressed;
/* Global Variable define of  */
extern double sysml_state_manualLevel;
/* Global Variable define of  */
extern double sysml_state_autoSpeed;
/* Global Variable define of  */
extern double sysml_state_temperatureC;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, bool io_3, double io_5, double* io_2, int32_t* io_4);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_modeAuto = false;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
double sysml_state_manualLevel = 0.0;
/* Global Variable define of  */
double sysml_state_autoSpeed = 0.0;
/* Global Variable define of  */
double sysml_state_temperatureC = 0.0;
/* Init function of CompositeActor named "ProgramPage1" */
#line 145 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_002\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 161 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_002\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 45 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_002\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_modeAuto = false;
    sysml_state_previousPressed = false;
    sysml_state_manualLevel = 0.0;
    sysml_state_autoSpeed = 0.0;
    sysml_state_temperatureC = 0.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, bool io_3, double io_5, double* io_2, int32_t* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_conditional_0 = 0;
    double sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    double sysml_conditional_3 = 0;
    int32_t sysml_conditional_4 = 0;
    int32_t sysml_conditional_5 = 0;
    int32_t sysml_conditional_6 = 0;
    sysml_state_temperatureC = ((double)((io_0 * 100.0)) / 1023.0);
    sysml_state_manualLevel = ((double)((io_1 * 255.0)) / 1023.0);
    if (sysml_state_temperatureC <= 20.0) {
        sysml_conditional_0 = 0.0;
    }
    else {
        if (sysml_state_temperatureC >= 50.0) {
            sysml_conditional_1 = 255.0;
        }
        else {
            sysml_conditional_1 = ((double)((sysml_state_temperatureC - 20.0) * 255.0)) / 30.0;
        }
        sysml_conditional_0 = sysml_conditional_1;
    }
    sysml_state_autoSpeed = sysml_conditional_0;
    sysml_lazy_2 = io_3;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_2) {
        sysml_state_modeAuto = (!sysml_state_modeAuto);
    }
    sysml_state_previousPressed = io_3;
    double sysml_value_0 = 0;
    if (sysml_state_modeAuto) {
        sysml_conditional_3 = sysml_state_autoSpeed;
    }
    else {
        sysml_conditional_3 = sysml_state_manualLevel;
    }
    sysml_value_0 = sysml_conditional_3;
    int32_t sysml_value_1 = 0;
    if (sysml_state_modeAuto) {
        if (sysml_state_temperatureC < ((double)((int32_t)sysml_state_temperatureC))) {
            sysml_conditional_5 = 1;
        }
        else {
            sysml_conditional_5 = 0;
        }
        sysml_conditional_4 = ((int32_t)sysml_state_temperatureC) - sysml_conditional_5;
    }
    else {
        if ((((double)sysml_state_manualLevel) / 10.0) < ((double)((int32_t)(((double)sysml_state_manualLevel) / 10.0)))) {
            sysml_conditional_6 = 1;
        }
        else {
            sysml_conditional_6 = 0;
        }
        sysml_conditional_4 = ((int32_t)(((double)sysml_state_manualLevel) / 10.0)) - sysml_conditional_6;
    }
    sysml_value_1 = sysml_conditional_4;
    *io_2 = sysml_value_0;
    *io_4 = sysml_value_1;
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
static uint32_t hw_sampled_io_0 = 0;
static bool hw_attempted_io_0 = false;
static double hw_value_io_1 = 0;
static uint32_t hw_sampled_io_1 = 0;
static bool hw_attempted_io_1 = false;
static double hw_value_io_2 = 0;
static bool hw_value_io_3 = false;
static uint32_t hw_sampled_io_3 = 0;
static bool hw_attempted_io_3 = false;
static int32_t hw_value_io_4 = 0;
TM1637Display hw_display_io_4(4, 3);
static int32_t hw_display_previous_io_4 = INT32_MIN;
static double hw_value_io_5 = 0;
static uint32_t hw_sampled_io_5 = 0;
static bool hw_attempted_io_5 = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(A1, INPUT);
  pinMode(A0, INPUT);
  pinMode(5, OUTPUT);
  analogWrite(5, (int)constrain((hw_value_io_2 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(2, INPUT_PULLUP);
  hw_display_io_4.setBrightness(7, true);
  if (hw_value_io_4 != hw_display_previous_io_4) {
    if (hw_value_io_4 < -999 || hw_value_io_4 > 9999) hw_display_io_4.clear();
    else hw_display_io_4.showNumberDec((int)hw_value_io_4, false, 4, 0);
    hw_display_previous_io_4 = hw_value_io_4;
  }
  pinMode(A2, INPUT);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 10UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (analogRead(A1) * 1.0 + 0.0);
  }
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 10UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = (analogRead(A0) * 1.0 + 0.0);
  }
  if (!hw_attempted_io_3 || (uint32_t)(hw_now - hw_sampled_io_3) >= 10UL) {
    hw_attempted_io_3 = true; hw_sampled_io_3 = hw_now;
    hw_value_io_3 = (digitalRead(2) == HIGH);
  }
  if (!hw_attempted_io_5 || (uint32_t)(hw_now - hw_sampled_io_5) >= 10UL) {
    hw_attempted_io_5 = true; hw_sampled_io_5 = hw_now;
    hw_value_io_5 = (analogRead(A2) * 1.0 + 0.0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_3, hw_value_io_5, &hw_value_io_2, &hw_value_io_4);
  analogWrite(5, (int)constrain((hw_value_io_2 * 1.0 + 0.0), 0.0, 255.0));
  if (hw_value_io_4 != hw_display_previous_io_4) {
    if (hw_value_io_4 < -999 || hw_value_io_4 > 9999) hw_display_io_4.clear();
    else hw_display_io_4.showNumberDec((int)hw_value_io_4, false, 4, 0);
    hw_display_previous_io_4 = hw_value_io_4;
  }
}

