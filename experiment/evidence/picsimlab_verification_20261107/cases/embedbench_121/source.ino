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
extern double sysml_state_led1Brightness;
/* Global Variable define of  */
extern double sysml_state_led2Brightness;
/* Global Variable define of  */
extern double sysml_state_led3Brightness;
/* Global Variable define of  */
extern int32_t sysml_state_btn1StableUntil;
/* Global Variable define of  */
extern int32_t sysml_state_btn2StableUntil;
/* Global Variable define of  */
extern int32_t sysml_state_btn3StableUntil;
/* Global Variable define of  */
extern int32_t sysml_state_stableTimer;
/* Global Variable define of  */
extern bool sysml_state_previousBtn1Pressed;
/* Global Variable define of  */
extern bool sysml_state_previousBtn2Pressed;
/* Global Variable define of  */
extern bool sysml_state_previousBtn3Pressed;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_3, bool io_4, bool io_5, double io_6, double io_7, double io_8, double* io_0, double* io_1, double* io_2);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_led1Brightness = 0.0;
/* Global Variable define of  */
double sysml_state_led2Brightness = 0.0;
/* Global Variable define of  */
double sysml_state_led3Brightness = 0.0;
/* Global Variable define of  */
int32_t sysml_state_btn1StableUntil = 0;
/* Global Variable define of  */
int32_t sysml_state_btn2StableUntil = 0;
/* Global Variable define of  */
int32_t sysml_state_btn3StableUntil = 0;
/* Global Variable define of  */
int32_t sysml_state_stableTimer = 0;
/* Global Variable define of  */
bool sysml_state_previousBtn1Pressed = false;
/* Global Variable define of  */
bool sysml_state_previousBtn2Pressed = false;
/* Global Variable define of  */
bool sysml_state_previousBtn3Pressed = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_led1Brightness = 0.0;
    sysml_state_led2Brightness = 0.0;
    sysml_state_led3Brightness = 0.0;
    sysml_state_btn1StableUntil = 0;
    sysml_state_btn2StableUntil = 0;
    sysml_state_btn3StableUntil = 0;
    sysml_state_stableTimer = 0;
    sysml_state_previousBtn1Pressed = false;
    sysml_state_previousBtn2Pressed = false;
    sysml_state_previousBtn3Pressed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_3, bool io_4, bool io_5, double io_6, double io_7, double io_8, double* io_0, double* io_1, double* io_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    if ((sysml_state_stableTimer == 0)) {
        sysml_state_led1Brightness = 0.0;
        sysml_state_led2Brightness = 0.0;
        sysml_state_led3Brightness = 0.0;
        sysml_state_btn1StableUntil = 0;
        sysml_state_btn2StableUntil = 0;
        sysml_state_btn3StableUntil = 0;
    }
    sysml_state_stableTimer = (sysml_state_stableTimer + 1);
    sysml_lazy_0 = io_3;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousBtn1Pressed;
    }
    if (sysml_lazy_0) {
        sysml_state_led1Brightness = ((double)((io_6 * 255.0)) / 1023.0);
        sysml_state_btn1StableUntil = (sysml_state_stableTimer + 200);
    }
    sysml_lazy_1 = io_4;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_previousBtn2Pressed;
    }
    if (sysml_lazy_1) {
        sysml_state_led2Brightness = ((double)((io_7 * 255.0)) / 1023.0);
        sysml_state_btn2StableUntil = (sysml_state_stableTimer + 200);
    }
    sysml_lazy_2 = io_5;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_previousBtn3Pressed;
    }
    if (sysml_lazy_2) {
        sysml_state_led3Brightness = ((double)((io_8 * 255.0)) / 1023.0);
        sysml_state_btn3StableUntil = (sysml_state_stableTimer + 200);
    }
    sysml_lazy_3 = io_3;
    if (sysml_lazy_3) {
        sysml_lazy_4 = sysml_state_stableTimer >= sysml_state_btn1StableUntil;
        if (sysml_lazy_4) {
            sysml_lazy_4 = true;
        }
        sysml_lazy_3 = !sysml_lazy_4;
    }
    if (sysml_lazy_3) {
        sysml_state_led1Brightness = sysml_state_led1Brightness;
    }
    sysml_lazy_5 = io_4;
    if (sysml_lazy_5) {
        sysml_lazy_6 = sysml_state_stableTimer >= sysml_state_btn2StableUntil;
        if (sysml_lazy_6) {
            sysml_lazy_6 = true;
        }
        sysml_lazy_5 = !sysml_lazy_6;
    }
    if (sysml_lazy_5) {
        sysml_state_led2Brightness = sysml_state_led2Brightness;
    }
    sysml_lazy_7 = io_5;
    if (sysml_lazy_7) {
        sysml_lazy_8 = sysml_state_stableTimer >= sysml_state_btn3StableUntil;
        if (sysml_lazy_8) {
            sysml_lazy_8 = true;
        }
        sysml_lazy_7 = !sysml_lazy_8;
    }
    if (sysml_lazy_7) {
        sysml_state_led3Brightness = sysml_state_led3Brightness;
    }
    sysml_state_previousBtn1Pressed = io_3;
    sysml_state_previousBtn2Pressed = io_4;
    sysml_state_previousBtn3Pressed = io_5;
    double sysml_value_0 = 0;
    sysml_value_0 = sysml_state_led1Brightness;
    double sysml_value_1 = 0;
    sysml_value_1 = sysml_state_led2Brightness;
    double sysml_value_2 = 0;
    sysml_value_2 = sysml_state_led3Brightness;
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
    hw_value_io_6 = (analogRead(A3) * 1.0 + 0.0);
  }
  if (!hw_attempted_io_7 || (uint32_t)(hw_now - hw_sampled_io_7) >= 10UL) {
    hw_attempted_io_7 = true; hw_sampled_io_7 = hw_now;
    hw_value_io_7 = (analogRead(A4) * 1.0 + 0.0);
  }
  if (!hw_attempted_io_8 || (uint32_t)(hw_now - hw_sampled_io_8) >= 10UL) {
    hw_attempted_io_8 = true; hw_sampled_io_8 = hw_now;
    hw_value_io_8 = (analogRead(A5) * 1.0 + 0.0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_3, hw_value_io_4, hw_value_io_5, hw_value_io_6, hw_value_io_7, hw_value_io_8, &hw_value_io_0, &hw_value_io_1, &hw_value_io_2);
  analogWrite(2, (int)constrain((hw_value_io_0 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(3, (int)constrain((hw_value_io_1 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(4, (int)constrain((hw_value_io_2 * 1.0 + 0.0), 0.0, 255.0));
}
