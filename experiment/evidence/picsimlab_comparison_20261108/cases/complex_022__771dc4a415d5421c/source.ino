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
extern int32_t sysml_state_mode;
/* Global Variable define of  */
extern int32_t sysml_state_param;
/* Global Variable define of  */
extern double sysml_state_brightnessGate;
/* Global Variable define of  */
extern int32_t sysml_state_accumulatedMs;
/* Global Variable define of  */
extern bool sysml_state_previousPressed;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, int32_t io_5, double* io_2, bool* io_3, int32_t* io_4);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_mode = 0;
/* Global Variable define of  */
int32_t sysml_state_param = 128;
/* Global Variable define of  */
double sysml_state_brightnessGate = 0.0;
/* Global Variable define of  */
int32_t sysml_state_accumulatedMs = 0;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_mode = 0;
    sysml_state_param = 128;
    sysml_state_brightnessGate = 0.0;
    sysml_state_accumulatedMs = 0;
    sysml_state_previousPressed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, int32_t io_5, double* io_2, bool* io_3, int32_t* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int32_t sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    int32_t sysml_conditional_3 = 0;
    int32_t sysml_conditional_4 = 0;
    double sysml_conditional_5 = 0;
    double sysml_conditional_6 = 0;
    double sysml_conditional_7 = 0;
    double sysml_arithmetic_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_lazy_10 = 0;
    int32_t sysml_conditional_11 = 0;
    int32_t sysml_conditional_12 = 0;
    sysml_state_previousPressed = io_0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        if (sysml_state_mode == 2) {
            sysml_conditional_1 = 0;
        }
        else {
            sysml_conditional_1 = sysml_state_mode + 1;
        }
        sysml_state_mode = sysml_conditional_1;
    }
    sysml_lazy_2 = io_1;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !(sysml_state_mode == 2);
    }
    if (sysml_lazy_2) {
        if (sysml_state_param == 255) {
            sysml_conditional_3 = 0;
        }
        else {
            sysml_conditional_3 = sysml_state_param + 16;
        }
        sysml_state_param = sysml_conditional_3;
    }
    else {
        if (sysml_state_param == 5000) {
            sysml_conditional_4 = 500;
        }
        else {
            sysml_conditional_4 = sysml_state_param + 500;
        }
        sysml_state_param = sysml_conditional_4;
    }
    if ((sysml_state_mode == 0)) {
        sysml_state_brightnessGate = 0.0;
    }
    if ((sysml_state_mode == 1)) {
        if (sysml_state_param >= 255) {
            sysml_conditional_5 = 255.0;
        }
        else {
            sysml_conditional_5 = sysml_state_param * 1.0;
        }
        sysml_state_brightnessGate = sysml_conditional_5;
    }
    if ((sysml_state_mode == 2)) {
        sysml_state_accumulatedMs = (sysml_state_accumulatedMs + io_5);
        if (sysml_state_accumulatedMs < (sysml_state_param * 1)) {
            if (sysml_state_accumulatedMs >= (((double)sysml_state_param) / 2)) {
                sysml_conditional_7 = ((double)(sysml_state_param - (((double)(sysml_state_accumulatedMs - (((double)sysml_state_param) / 2))) / 1))) / 1;
            }
            else {
                sysml_arithmetic_8 = ((double)sysml_state_param) / 255;
                sysml_conditional_7 = ((double)sysml_state_accumulatedMs) / sysml_arithmetic_8;
            }
            sysml_conditional_6 = sysml_conditional_7;
        }
        else {
            sysml_conditional_6 = 0.0;
        }
        sysml_state_brightnessGate = sysml_conditional_6;
    }
    sysml_lazy_9 = sysml_state_mode == 2;
    if (sysml_lazy_9) {
        sysml_lazy_9 = sysml_state_accumulatedMs >= (sysml_state_param * 2);
    }
    if (sysml_lazy_9) {
        sysml_state_accumulatedMs = 0;
    }
    double sysml_value_0 = 0;
    sysml_value_0 = sysml_state_brightnessGate;
    bool sysml_value_1 = 0;
    sysml_value_1 = (sysml_state_mode != 0);
    int32_t sysml_value_2 = 0;
    sysml_lazy_10 = sysml_state_mode == 0;
    if (!sysml_lazy_10) {
        sysml_lazy_10 = sysml_state_mode == 1;
    }
    if (sysml_lazy_10) {
        sysml_conditional_11 = sysml_state_mode;
    }
    else {
        if (sysml_state_param < ((double)((int32_t)sysml_state_param))) {
            sysml_conditional_12 = 1;
        }
        else {
            sysml_conditional_12 = 0;
        }
        sysml_conditional_11 = ((int32_t)sysml_state_param) - sysml_conditional_12;
    }
    sysml_value_2 = sysml_conditional_11;
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
static bool hw_value_io_0 = false;
static uint32_t hw_sampled_io_0 = 0;
static bool hw_attempted_io_0 = false;
static bool hw_value_io_1 = false;
static uint32_t hw_sampled_io_1 = 0;
static bool hw_attempted_io_1 = false;
static double hw_value_io_2 = 0;
static bool hw_value_io_3 = false;
static int32_t hw_value_io_4 = 1;
TM1637Display hw_display_io_4(4, 5);
static int32_t hw_display_previous_io_4 = INT32_MIN;
static int32_t hw_value_io_5 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(9, OUTPUT);
  analogWrite(9, (int)constrain((hw_value_io_2 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_3 ? HIGH : LOW);
  hw_display_io_4.setBrightness(7, true);
  if (hw_value_io_4 != hw_display_previous_io_4) {
    if (hw_value_io_4 < -999 || hw_value_io_4 > 9999) hw_display_io_4.clear();
    else hw_display_io_4.showNumberDec((int)hw_value_io_4, false, 4, 0);
    hw_display_previous_io_4 = hw_value_io_4;
  }
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 5UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (digitalRead(2) == HIGH);
  }
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 5UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = (digitalRead(3) == HIGH);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_5 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_5, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4);
  analogWrite(9, (int)constrain((hw_value_io_2 * 1.0 + 0.0), 0.0, 255.0));
  digitalWrite(13, hw_value_io_3 ? HIGH : LOW);
  if (hw_value_io_4 != hw_display_previous_io_4) {
    if (hw_value_io_4 < -999 || hw_value_io_4 > 9999) hw_display_io_4.clear();
    else hw_display_io_4.showNumberDec((int)hw_value_io_4, false, 4, 0);
    hw_display_previous_io_4 = hw_value_io_4;
  }
}
