#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_012\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
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
extern bool sysml_state_running;
/* Global Variable define of  */
extern bool sysml_state_previousPressed;
/* Global Variable define of  */
extern int32_t sysml_state_blinkPeriodMs;
/* Global Variable define of  */
extern int32_t sysml_state_remainingMs;
/* Global Variable define of  */
extern int32_t sysml_state_tickPhase;
/* Global Variable define of  */
extern bool sysml_state_ledOn;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, bool* io_2, int32_t* io_3);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_running = false;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
int32_t sysml_state_blinkPeriodMs = 0;
/* Global Variable define of  */
int32_t sysml_state_remainingMs = 0;
/* Global Variable define of  */
int32_t sysml_state_tickPhase = 0;
/* Global Variable define of  */
bool sysml_state_ledOn = false;
/* Init function of CompositeActor named "ProgramPage1" */
#line 169 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_012\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 183 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_012\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 49 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_012\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_running = false;
    sysml_state_previousPressed = false;
    sysml_state_blinkPeriodMs = 0;
    sysml_state_remainingMs = 0;
    sysml_state_tickPhase = 0;
    sysml_state_ledOn = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, bool* io_2, int32_t* io_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    int32_t sysml_conditional_2 = 0;
    int32_t sysml_conditional_3 = 0;
    int32_t sysml_conditional_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    sysml_lazy_0 = io_1;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        sysml_state_running = (!sysml_state_running);
    }
    sysml_state_previousPressed = io_1;
    sysml_lazy_1 = io_0 >= 0.0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = io_0 <= 1023.0;
    }
    if (sysml_lazy_1) {
        if (((io_0 * 1000.0) - 0.0) < ((double)((int32_t)((io_0 * 1000.0) - 0.0)))) {
            sysml_conditional_2 = 1;
        }
        else {
            sysml_conditional_2 = 0;
        }
        if ((((int32_t)((io_0 * 1000.0) - 0.0)) - sysml_conditional_2) == 0) {
            sysml_conditional_3 = 1;
        }
        else {
            if ((1.0 + ((((double)io_0) / 1023.0) * 999.0)) < ((double)((int32_t)(1.0 + ((((double)io_0) / 1023.0) * 999.0))))) {
                sysml_conditional_4 = 1;
            }
            else {
                sysml_conditional_4 = 0;
            }
            sysml_conditional_3 = ((int32_t)(1.0 + ((((double)io_0) / 1023.0) * 999.0))) - sysml_conditional_4;
        }
        sysml_state_blinkPeriodMs = sysml_conditional_3;
    }
    else {
        sysml_state_blinkPeriodMs = 1;
    }
    sysml_lazy_5 = sysml_state_running;
    if (sysml_lazy_5) {
        sysml_lazy_5 = sysml_state_remainingMs == 0;
    }
    if (sysml_lazy_5) {
        sysml_state_remainingMs = sysml_state_blinkPeriodMs;
        sysml_state_tickPhase = 0;
        sysml_state_ledOn = true;
    }
    sysml_lazy_6 = !sysml_state_running;
    if (sysml_lazy_6) {
        sysml_lazy_6 = true;
    }
    if (sysml_lazy_6) {
        sysml_state_remainingMs = 0;
        sysml_state_tickPhase = 0;
        sysml_state_ledOn = false;
    }
    sysml_lazy_7 = sysml_state_running;
    if (sysml_lazy_7) {
        sysml_lazy_7 = sysml_state_remainingMs != 0;
    }
    if (sysml_lazy_7) {
        sysml_state_remainingMs = (sysml_state_remainingMs - 5);
        if ((sysml_state_remainingMs <= 0)) {
            sysml_state_ledOn = (!sysml_state_ledOn);
            sysml_state_remainingMs = sysml_state_blinkPeriodMs;
            sysml_state_tickPhase = (sysml_state_tickPhase + 1);
        }
    }
    bool sysml_value_0 = 0;
    sysml_lazy_8 = sysml_state_running;
    if (sysml_lazy_8) {
        sysml_lazy_8 = sysml_state_ledOn;
    }
    sysml_value_0 = sysml_lazy_8;
    int32_t sysml_value_1 = 0;
    sysml_value_1 = sysml_state_blinkPeriodMs;
    *io_2 = sysml_value_0;
    *io_3 = sysml_value_1;
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
static bool hw_value_io_1 = false;
static uint32_t hw_sampled_io_1 = 0;
static bool hw_attempted_io_1 = false;
static bool hw_value_io_2 = false;
static int32_t hw_value_io_3 = 0;
TM1637Display hw_display_io_3(4, 5);
static int32_t hw_display_previous_io_3 = INT32_MIN;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(A0, INPUT);
  pinMode(2, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_2 ? HIGH : LOW);
  hw_display_io_3.setBrightness(7, true);
  if (hw_value_io_3 != hw_display_previous_io_3) {
    if (hw_value_io_3 < -999 || hw_value_io_3 > 9999) hw_display_io_3.clear();
    else hw_display_io_3.showNumberDec((int)hw_value_io_3, false, 4, 0);
    hw_display_previous_io_3 = hw_value_io_3;
  }
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
    hw_value_io_1 = (digitalRead(2) == LOW);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, &hw_value_io_2, &hw_value_io_3);
  digitalWrite(13, hw_value_io_2 ? HIGH : LOW);
  if (hw_value_io_3 != hw_display_previous_io_3) {
    if (hw_value_io_3 < -999 || hw_value_io_3 > 9999) hw_display_io_3.clear();
    else hw_display_io_3.showNumberDec((int)hw_value_io_3, false, 4, 0);
    hw_display_previous_io_3 = hw_value_io_3;
  }
}

