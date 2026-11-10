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
extern int32_t sysml_state_led1Brightness;
/* Global Variable define of  */
extern int32_t sysml_state_led2Brightness;
/* Global Variable define of  */
extern int32_t sysml_state_led3Brightness;
/* Global Variable define of  */
extern bool sysml_state_prevBtn1;
/* Global Variable define of  */
extern bool sysml_state_prevBtn2;
/* Global Variable define of  */
extern bool sysml_state_prevBtn3;
/* Global Variable define of  */
extern int32_t sysml_state_hold1RemainingMs;
/* Global Variable define of  */
extern int32_t sysml_state_hold2RemainingMs;
/* Global Variable define of  */
extern int32_t sysml_state_hold3RemainingMs;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_3, bool io_4, bool io_5, double io_6, double io_7, double io_8, int32_t io_9, double* io_0, double* io_1, double* io_2);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_led1Brightness = 0;
/* Global Variable define of  */
int32_t sysml_state_led2Brightness = 0;
/* Global Variable define of  */
int32_t sysml_state_led3Brightness = 0;
/* Global Variable define of  */
bool sysml_state_prevBtn1 = false;
/* Global Variable define of  */
bool sysml_state_prevBtn2 = false;
/* Global Variable define of  */
bool sysml_state_prevBtn3 = false;
/* Global Variable define of  */
int32_t sysml_state_hold1RemainingMs = 0;
/* Global Variable define of  */
int32_t sysml_state_hold2RemainingMs = 0;
/* Global Variable define of  */
int32_t sysml_state_hold3RemainingMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_led1Brightness = 0;
    sysml_state_led2Brightness = 0;
    sysml_state_led3Brightness = 0;
    sysml_state_prevBtn1 = false;
    sysml_state_prevBtn2 = false;
    sysml_state_prevBtn3 = false;
    sysml_state_hold1RemainingMs = 0;
    sysml_state_hold2RemainingMs = 0;
    sysml_state_hold3RemainingMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_3, bool io_4, bool io_5, double io_6, double io_7, double io_8, int32_t io_9, double* io_0, double* io_1, double* io_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int32_t sysml_conditional_1 = 0;
    int32_t sysml_conditional_2 = 0;
    int32_t sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    int32_t sysml_conditional_5 = 0;
    int32_t sysml_conditional_6 = 0;
    int32_t sysml_conditional_7 = 0;
    bool sysml_lazy_8 = 0;
    int32_t sysml_conditional_9 = 0;
    int32_t sysml_conditional_10 = 0;
    int32_t sysml_conditional_11 = 0;
    int32_t sysml_conditional_12 = 0;
    int32_t sysml_conditional_13 = 0;
    int32_t sysml_conditional_14 = 0;
    if ((sysml_state_hold1RemainingMs > 0)) {
        sysml_state_hold1RemainingMs = (sysml_state_hold1RemainingMs - io_9);
    }
    if ((sysml_state_hold1RemainingMs <= 0)) {
        sysml_state_hold1RemainingMs = 0;
    }
    if ((sysml_state_hold2RemainingMs > 0)) {
        sysml_state_hold2RemainingMs = (sysml_state_hold2RemainingMs - io_9);
    }
    if ((sysml_state_hold2RemainingMs <= 0)) {
        sysml_state_hold2RemainingMs = 0;
    }
    if ((sysml_state_hold3RemainingMs > 0)) {
        sysml_state_hold3RemainingMs = (sysml_state_hold3RemainingMs - io_9);
    }
    if ((sysml_state_hold3RemainingMs <= 0)) {
        sysml_state_hold3RemainingMs = 0;
    }
    sysml_lazy_0 = io_3;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_prevBtn1;
    }
    if (sysml_lazy_0) {
        sysml_state_hold1RemainingMs = 2000;
        if (io_6 <= 0.0) {
            sysml_conditional_1 = 0;
        }
        else {
            if (io_6 >= 1023.0) {
                sysml_conditional_2 = 255;
            }
            else {
                if ((((double)(io_6 * 255.0)) / 1023.0) < ((double)((int32_t)(((double)(io_6 * 255.0)) / 1023.0)))) {
                    sysml_conditional_3 = 1;
                }
                else {
                    sysml_conditional_3 = 0;
                }
                sysml_conditional_2 = ((int32_t)(((double)(io_6 * 255.0)) / 1023.0)) - sysml_conditional_3;
            }
            sysml_conditional_1 = sysml_conditional_2;
        }
        sysml_state_led1Brightness = sysml_conditional_1;
    }
    sysml_lazy_4 = io_4;
    if (sysml_lazy_4) {
        sysml_lazy_4 = !sysml_state_prevBtn2;
    }
    if (sysml_lazy_4) {
        sysml_state_hold2RemainingMs = 2000;
        if (io_7 <= 0.0) {
            sysml_conditional_5 = 0;
        }
        else {
            if (io_7 >= 1023.0) {
                sysml_conditional_6 = 255;
            }
            else {
                if ((((double)(io_7 * 255.0)) / 1023.0) < ((double)((int32_t)(((double)(io_7 * 255.0)) / 1023.0)))) {
                    sysml_conditional_7 = 1;
                }
                else {
                    sysml_conditional_7 = 0;
                }
                sysml_conditional_6 = ((int32_t)(((double)(io_7 * 255.0)) / 1023.0)) - sysml_conditional_7;
            }
            sysml_conditional_5 = sysml_conditional_6;
        }
        sysml_state_led2Brightness = sysml_conditional_5;
    }
    sysml_lazy_8 = io_5;
    if (sysml_lazy_8) {
        sysml_lazy_8 = !sysml_state_prevBtn3;
    }
    if (sysml_lazy_8) {
        sysml_state_hold3RemainingMs = 2000;
        if (io_8 <= 0.0) {
            sysml_conditional_9 = 0;
        }
        else {
            if (io_8 >= 1023.0) {
                sysml_conditional_10 = 255;
            }
            else {
                if ((((double)(io_8 * 255.0)) / 1023.0) < ((double)((int32_t)(((double)(io_8 * 255.0)) / 1023.0)))) {
                    sysml_conditional_11 = 1;
                }
                else {
                    sysml_conditional_11 = 0;
                }
                sysml_conditional_10 = ((int32_t)(((double)(io_8 * 255.0)) / 1023.0)) - sysml_conditional_11;
            }
            sysml_conditional_9 = sysml_conditional_10;
        }
        sysml_state_led3Brightness = sysml_conditional_9;
    }
    sysml_state_prevBtn1 = io_3;
    sysml_state_prevBtn2 = io_4;
    sysml_state_prevBtn3 = io_5;
    double sysml_value_0 = 0;
    if (sysml_state_led1Brightness < ((double)((int32_t)sysml_state_led1Brightness))) {
        sysml_conditional_12 = 1;
    }
    else {
        sysml_conditional_12 = 0;
    }
    sysml_value_0 = ((int32_t)sysml_state_led1Brightness) - sysml_conditional_12;
    double sysml_value_1 = 0;
    if (sysml_state_led2Brightness < ((double)((int32_t)sysml_state_led2Brightness))) {
        sysml_conditional_13 = 1;
    }
    else {
        sysml_conditional_13 = 0;
    }
    sysml_value_1 = ((int32_t)sysml_state_led2Brightness) - sysml_conditional_13;
    double sysml_value_2 = 0;
    if (sysml_state_led3Brightness < ((double)((int32_t)sysml_state_led3Brightness))) {
        sysml_conditional_14 = 1;
    }
    else {
        sysml_conditional_14 = 0;
    }
    sysml_value_2 = ((int32_t)sysml_state_led3Brightness) - sysml_conditional_14;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
}
// END COMPILED SYSML MODEL

// LLM4MDD target-board: arduino:avr:mega
#include <Arduino.h>
#include <math.h>
#if defined(ARDUINO) && !defined(ARDUINO_AVR_MEGA2560)
#error "Selected Arduino board differs from the interface contract"
#endif
static uint32_t hw_last_step = 0;
static bool hw_started = false;
static double hw_value_io_0 = 0;
static double hw_value_io_1 = 0;
static double hw_value_io_2 = 0;
static bool hw_value_io_3 = false;
static uint32_t hw_sampled_io_3 = 0;
static bool hw_attempted_io_3 = false;
static bool hw_value_io_4 = false;
static uint32_t hw_sampled_io_4 = 0;
static bool hw_attempted_io_4 = false;
static bool hw_value_io_5 = false;
static uint32_t hw_sampled_io_5 = 0;
static bool hw_attempted_io_5 = false;
static double hw_value_io_6 = 0;
static uint32_t hw_sampled_io_6 = 0;
static bool hw_attempted_io_6 = false;
static double hw_value_io_7 = 0;
static uint32_t hw_sampled_io_7 = 0;
static bool hw_attempted_io_7 = false;
static double hw_value_io_8 = 0;
static uint32_t hw_sampled_io_8 = 0;
static bool hw_attempted_io_8 = false;
static int32_t hw_value_io_9 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, OUTPUT);
  analogWrite(2, (int)constrain((hw_value_io_0 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(3, OUTPUT);
  analogWrite(3, (int)constrain((hw_value_io_1 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(4, OUTPUT);
  analogWrite(4, (int)constrain((hw_value_io_2 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_3 || (uint32_t)(hw_now - hw_sampled_io_3) >= 10UL) {
    hw_attempted_io_3 = true; hw_sampled_io_3 = hw_now;
    hw_value_io_3 = (digitalRead(5) == LOW);
  }
  if (!hw_attempted_io_4 || (uint32_t)(hw_now - hw_sampled_io_4) >= 10UL) {
    hw_attempted_io_4 = true; hw_sampled_io_4 = hw_now;
    hw_value_io_4 = (digitalRead(6) == LOW);
  }
  if (!hw_attempted_io_5 || (uint32_t)(hw_now - hw_sampled_io_5) >= 10UL) {
    hw_attempted_io_5 = true; hw_sampled_io_5 = hw_now;
    hw_value_io_5 = (digitalRead(7) == LOW);
  }
  if (!hw_attempted_io_6 || (uint32_t)(hw_now - hw_sampled_io_6) >= 10UL) {
    hw_attempted_io_6 = true; hw_sampled_io_6 = hw_now;
    hw_value_io_6 = (analogRead(A3) * 1 + 0.0);
  }
  if (!hw_attempted_io_7 || (uint32_t)(hw_now - hw_sampled_io_7) >= 10UL) {
    hw_attempted_io_7 = true; hw_sampled_io_7 = hw_now;
    hw_value_io_7 = (analogRead(A4) * 1 + 0.0);
  }
  if (!hw_attempted_io_8 || (uint32_t)(hw_now - hw_sampled_io_8) >= 10UL) {
    hw_attempted_io_8 = true; hw_sampled_io_8 = hw_now;
    hw_value_io_8 = (analogRead(A5) * 1 + 0.0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_9 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_3, hw_value_io_4, hw_value_io_5, hw_value_io_6, hw_value_io_7, hw_value_io_8, hw_value_io_9, &hw_value_io_0, &hw_value_io_1, &hw_value_io_2);
  analogWrite(2, (int)constrain((hw_value_io_0 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(3, (int)constrain((hw_value_io_1 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(4, (int)constrain((hw_value_io_2 * 1.0 + 0.0), 0.0, 255.0));
}
