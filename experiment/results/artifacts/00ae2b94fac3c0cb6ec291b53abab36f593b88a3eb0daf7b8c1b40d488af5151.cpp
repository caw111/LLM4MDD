#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_024\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
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
extern int32_t sysml_state_sampleCount;
/* Global Variable define of  */
extern bool sysml_state_buzzerOn;
/* Global Variable define of  */
extern bool sysml_state_beepPending;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, double io_1, double io_2, bool* io_3, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, int32_t* serial_0_value_2);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
int32_t sysml_state_sampleCount = 0;
/* Global Variable define of  */
bool sysml_state_buzzerOn = false;
/* Global Variable define of  */
bool sysml_state_beepPending = false;
/* Init function of CompositeActor named "ProgramPage1" */
#line 188 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_024\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 198 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_024\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 41 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_024\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousPressed = false;
    sysml_state_sampleCount = 0;
    sysml_state_buzzerOn = false;
    sysml_state_beepPending = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, double io_1, double io_2, bool* io_3, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, int32_t* serial_0_value_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_conditional_3 = 0;
    bool sysml_conditional_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_conditional_6 = 0;
    bool sysml_lazy_7 = 0;
    double sysml_conditional_8 = 0;
    bool sysml_lazy_9 = 0;
    double sysml_conditional_10 = 0;
    double sysml_conditional_11 = 0;
    bool sysml_lazy_12 = 0;
    int32_t sysml_conditional_13 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        sysml_state_sampleCount = (sysml_state_sampleCount + 1);
        if ((sysml_state_sampleCount - 4) == 0) {
            sysml_conditional_1 = true;
        }
        else {
            sysml_conditional_1 = false;
        }
        sysml_state_beepPending = sysml_conditional_1;
        sysml_lazy_2 = sysml_state_buzzerOn;
        if (!sysml_lazy_2) {
            if ((sysml_state_sampleCount - 4) == 0) {
                sysml_conditional_3 = true;
            }
            else {
                sysml_conditional_3 = false;
            }
            sysml_lazy_2 = sysml_conditional_3;
        }
        sysml_state_buzzerOn = sysml_lazy_2;
    }
    sysml_state_previousPressed = io_0;
    if (sysml_state_beepPending) {
        sysml_state_buzzerOn = false;
        sysml_state_beepPending = false;
    }
    bool sysml_value_0 = 0;
    if (io_0) {
        sysml_conditional_4 = sysml_state_buzzerOn;
    }
    else {
        sysml_conditional_4 = false;
    }
    sysml_value_0 = sysml_conditional_4;
    bool sysml_value_1 = 0;
    sysml_lazy_5 = io_0;
    if (sysml_lazy_5) {
        sysml_lazy_5 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_5) {
        sysml_conditional_6 = true;
    }
    else {
        sysml_conditional_6 = false;
    }
    sysml_value_1 = sysml_conditional_6;
    double sysml_value_2 = 0;
    sysml_lazy_7 = io_0;
    if (sysml_lazy_7) {
        sysml_lazy_7 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_7) {
        sysml_conditional_8 = io_1;
    }
    else {
        sysml_conditional_8 = 0.0;
    }
    sysml_value_2 = sysml_conditional_8;
    double sysml_value_3 = 0;
    sysml_lazy_9 = io_0;
    if (sysml_lazy_9) {
        sysml_lazy_9 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_9) {
        if (io_2 >= 0.0) {
            sysml_conditional_11 = io_2 * 0.1;
        }
        else {
            sysml_conditional_11 = 0.0;
        }
        sysml_conditional_10 = sysml_conditional_11;
    }
    else {
        sysml_conditional_10 = 0.0;
    }
    sysml_value_3 = sysml_conditional_10;
    int32_t sysml_value_4 = 0;
    sysml_lazy_12 = io_0;
    if (sysml_lazy_12) {
        sysml_lazy_12 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_12) {
        sysml_conditional_13 = sysml_state_sampleCount;
    }
    else {
        sysml_conditional_13 = 0;
    }
    sysml_value_4 = sysml_conditional_13;
    *io_3 = sysml_value_0;
    *serial_0_trigger = sysml_value_1;
    *serial_0_value_0 = sysml_value_2;
    *serial_0_value_1 = sysml_value_3;
    *serial_0_value_2 = sysml_value_4;
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
static bool hw_value_io_0 = false;
static uint32_t hw_sampled_io_0 = 0;
static bool hw_attempted_io_0 = false;
static double hw_value_io_1 = 0;
static uint32_t hw_sampled_io_1 = 0;
static bool hw_attempted_io_1 = false;
static double hw_value_io_2 = 0;
static uint32_t hw_sampled_io_2 = 0;
static bool hw_attempted_io_2 = false;
static bool hw_value_io_3 = false;
static bool hw_value_serial_0_trigger = false;
static double hw_value_serial_0_value_0 = 0;
static double hw_value_serial_0_value_1 = 0;
static int32_t hw_value_serial_0_value_2 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(6, OUTPUT);
  digitalWrite(6, hw_value_io_3 ? HIGH : LOW);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 5UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (digitalRead(2) == LOW);
  }
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 5UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = (analogRead(A0) * 1.0 + 0.0);
  }
  if (!hw_attempted_io_2 || (uint32_t)(hw_now - hw_sampled_io_2) >= 5UL) {
    hw_attempted_io_2 = true; hw_sampled_io_2 = hw_now;
    hw_value_io_2 = (analogRead(A1) * 1.0 + 0.0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, &hw_value_io_3, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0, &hw_value_serial_0_value_1, &hw_value_serial_0_value_2);
  digitalWrite(6, hw_value_io_3 ? HIGH : LOW);
  if (hw_value_serial_0_trigger) { Serial.print("light"); Serial.println(hw_value_serial_0_value_0); }
  if (hw_value_serial_0_trigger) { Serial.print("temp"); Serial.println(hw_value_serial_0_value_1); }
  if (hw_value_serial_0_trigger) { Serial.print("count"); Serial.println(hw_value_serial_0_value_2); }
}

