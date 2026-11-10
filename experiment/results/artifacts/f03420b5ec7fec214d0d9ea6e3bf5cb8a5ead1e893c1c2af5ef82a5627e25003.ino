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
extern bool sysml_state_manualMode;
/* Global Variable define of  */
extern int32_t sysml_state_frozenDisplayValue;
/* Global Variable define of  */
extern double sysml_state_frozenBrightnessValue;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_2, double* io_1, bool* io_3, int32_t* io_4);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
bool sysml_state_manualMode = false;
/* Global Variable define of  */
int32_t sysml_state_frozenDisplayValue = 0;
/* Global Variable define of  */
double sysml_state_frozenBrightnessValue = 0.0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousPressed = false;
    sysml_state_manualMode = false;
    sysml_state_frozenDisplayValue = 0;
    sysml_state_frozenBrightnessValue = 0.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_2, double* io_1, bool* io_3, int32_t* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int32_t sysml_conditional_1 = 0;
    int32_t sysml_conditional_2 = 0;
    int32_t sysml_conditional_3 = 0;
    double sysml_conditional_4 = 0;
    double sysml_conditional_5 = 0;
    double sysml_conditional_6 = 0;
    double sysml_conditional_7 = 0;
    double sysml_conditional_8 = 0;
    double sysml_conditional_9 = 0;
    double sysml_conditional_10 = 0;
    double sysml_conditional_11 = 0;
    int32_t sysml_conditional_12 = 0;
    int32_t sysml_conditional_13 = 0;
    sysml_lazy_0 = io_2;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        sysml_state_manualMode = (!sysml_state_manualMode);
        if (sysml_state_manualMode) {
            if (io_0 < ((double)((int32_t)io_0))) {
                sysml_conditional_2 = 1;
            }
            else {
                sysml_conditional_2 = 0;
            }
            sysml_conditional_1 = ((int32_t)io_0) - sysml_conditional_2;
        }
        else {
            if (io_0 < ((double)((int32_t)io_0))) {
                sysml_conditional_3 = 1;
            }
            else {
                sysml_conditional_3 = 0;
            }
            sysml_conditional_1 = ((int32_t)io_0) - sysml_conditional_3;
        }
        sysml_state_frozenDisplayValue = sysml_conditional_1;
        if (sysml_state_manualMode) {
            if (io_0 >= 1023.0) {
                sysml_conditional_5 = 255.0;
            }
            else {
                if (io_0 <= 0.0) {
                    sysml_conditional_6 = 0.0;
                }
                else {
                    sysml_conditional_6 = ((double)(io_0 * 255.0)) / 1023.0;
                }
                sysml_conditional_5 = sysml_conditional_6;
            }
            sysml_conditional_4 = sysml_conditional_5;
        }
        else {
            if (io_0 >= 1023.0) {
                sysml_conditional_7 = 255.0;
            }
            else {
                if (io_0 <= 0.0) {
                    sysml_conditional_8 = 0.0;
                }
                else {
                    sysml_conditional_8 = ((double)(io_0 * 255.0)) / 1023.0;
                }
                sysml_conditional_7 = sysml_conditional_8;
            }
            sysml_conditional_4 = sysml_conditional_7;
        }
        sysml_state_frozenBrightnessValue = sysml_conditional_4;
        sysml_state_previousPressed = true;
    }
    else {
        sysml_state_previousPressed = io_2;
    }
    double sysml_value_0 = 0;
    if (sysml_state_manualMode) {
        sysml_conditional_9 = sysml_state_frozenBrightnessValue;
    }
    else {
        if (io_0 >= 1023.0) {
            sysml_conditional_10 = 255.0;
        }
        else {
            if (io_0 <= 0.0) {
                sysml_conditional_11 = 0.0;
            }
            else {
                sysml_conditional_11 = ((double)(io_0 * 255.0)) / 1023.0;
            }
            sysml_conditional_10 = sysml_conditional_11;
        }
        sysml_conditional_9 = sysml_conditional_10;
    }
    sysml_value_0 = sysml_conditional_9;
    bool sysml_value_1 = 0;
    sysml_value_1 = sysml_state_manualMode;
    int32_t sysml_value_2 = 0;
    if (sysml_state_manualMode) {
        sysml_conditional_12 = sysml_state_frozenDisplayValue;
    }
    else {
        if (io_0 < ((double)((int32_t)io_0))) {
            sysml_conditional_13 = 1;
        }
        else {
            sysml_conditional_13 = 0;
        }
        sysml_conditional_12 = ((int32_t)io_0) - sysml_conditional_13;
    }
    sysml_value_2 = sysml_conditional_12;
    *io_1 = sysml_value_0;
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
static uint32_t hw_sampled_io_0 = 0;
static bool hw_attempted_io_0 = false;
static double hw_value_io_1 = 0;
static bool hw_value_io_2 = false;
static uint32_t hw_sampled_io_2 = 0;
static bool hw_attempted_io_2 = false;
static bool hw_value_io_3 = false;
static int32_t hw_value_io_4 = 0;
TM1637Display hw_display_io_4(4, 5);
static int32_t hw_display_previous_io_4 = INT32_MIN;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(A0, INPUT);
  pinMode(9, OUTPUT);
  analogWrite(9, (int)constrain((hw_value_io_1 * 255 + 0), 0.0, 255.0));
  pinMode(2, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_3 ? HIGH : LOW);
  hw_display_io_4.setBrightness(5, true);
  if (hw_value_io_4 != hw_display_previous_io_4) {
    if (hw_value_io_4 < -999 || hw_value_io_4 > 9999) hw_display_io_4.clear();
    else hw_display_io_4.showNumberDec((int)hw_value_io_4, false, 4, 0);
    hw_display_previous_io_4 = hw_value_io_4;
  }
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 10UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (analogRead(A0) * 1 + 0.0);
  }
  if (!hw_attempted_io_2 || (uint32_t)(hw_now - hw_sampled_io_2) >= 10UL) {
    hw_attempted_io_2 = true; hw_sampled_io_2 = hw_now;
    hw_value_io_2 = (digitalRead(2) == LOW);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_2, &hw_value_io_1, &hw_value_io_3, &hw_value_io_4);
  analogWrite(9, (int)constrain((hw_value_io_1 * 255 + 0), 0.0, 255.0));
  digitalWrite(13, hw_value_io_3 ? HIGH : LOW);
  if (hw_value_io_4 != hw_display_previous_io_4) {
    if (hw_value_io_4 < -999 || hw_value_io_4 > 9999) hw_display_io_4.clear();
    else hw_display_io_4.showNumberDec((int)hw_value_io_4, false, 4, 0);
    hw_display_previous_io_4 = hw_value_io_4;
  }
}
