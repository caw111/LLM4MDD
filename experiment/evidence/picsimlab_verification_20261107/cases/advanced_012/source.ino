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
extern bool sysml_state_autoMode;
/* Global Variable define of  */
extern int32_t sysml_state_manualAnglePercent;
/* Global Variable define of  */
extern bool sysml_state_prevBtn2;
/* Global Variable define of  */
extern bool sysml_state_prevBtn3;
/* Global Variable define of  */
extern int32_t sysml_state_curAnglePercent;
/* Global Variable define of  */
extern bool sysml_state_lightMode;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, bool io_2, bool io_3, int32_t io_7, bool* io_4, double* io_5, int32_t* io_6);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_autoMode = true;
/* Global Variable define of  */
int32_t sysml_state_manualAnglePercent = 0;
/* Global Variable define of  */
bool sysml_state_prevBtn2 = false;
/* Global Variable define of  */
bool sysml_state_prevBtn3 = false;
/* Global Variable define of  */
int32_t sysml_state_curAnglePercent = 0;
/* Global Variable define of  */
bool sysml_state_lightMode = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_autoMode = true;
    sysml_state_manualAnglePercent = 0;
    sysml_state_prevBtn2 = false;
    sysml_state_prevBtn3 = false;
    sysml_state_curAnglePercent = 0;
    sysml_state_lightMode = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, bool io_2, bool io_3, int32_t io_7, bool* io_4, double* io_5, int32_t* io_6) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    int32_t sysml_conditional_3 = 0;
    double sysml_conditional_4 = 0;
    double sysml_conditional_5 = 0;
    int32_t sysml_conditional_6 = 0;
    int32_t sysml_conditional_7 = 0;
    if (sysml_state_autoMode) {
        if ((io_1 >= 30.0)) {
            sysml_state_curAnglePercent = 0;
            sysml_state_lightMode = false;
        }
        else {
            if ((io_0 <= 400.0)) {
                sysml_state_curAnglePercent = 100;
                sysml_state_lightMode = true;
            }
            else {
                sysml_state_curAnglePercent = 0;
                sysml_state_lightMode = false;
            }
        }
    }
    if ((!sysml_state_autoMode)) {
        sysml_state_curAnglePercent = sysml_state_manualAnglePercent;
    }
    sysml_lazy_0 = io_2;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_prevBtn2;
    }
    if (sysml_lazy_0) {
        if (sysml_state_autoMode) {
            sysml_conditional_1 = false;
        }
        else {
            sysml_conditional_1 = true;
        }
        sysml_state_autoMode = sysml_conditional_1;
    }
    sysml_lazy_2 = io_3;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_prevBtn3;
    }
    if (sysml_lazy_2) {
        if ((!sysml_state_autoMode)) {
            if (sysml_state_manualAnglePercent == 100) {
                sysml_conditional_3 = 0;
            }
            else {
                sysml_conditional_3 = sysml_state_manualAnglePercent + 10;
            }
            sysml_state_manualAnglePercent = sysml_conditional_3;
        }
    }
    sysml_state_prevBtn2 = io_2;
    sysml_state_prevBtn3 = io_3;
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_lightMode;
    double sysml_value_1 = 0;
    if (sysml_state_curAnglePercent >= 0) {
        if (sysml_state_curAnglePercent <= 100) {
            sysml_conditional_5 = sysml_state_curAnglePercent;
        }
        else {
            sysml_conditional_5 = 100.0;
        }
        sysml_conditional_4 = sysml_conditional_5;
    }
    else {
        sysml_conditional_4 = 0.0;
    }
    sysml_value_1 = sysml_conditional_4;
    int32_t sysml_value_2 = 0;
    if (sysml_state_curAnglePercent <= 100) {
        if (sysml_state_curAnglePercent >= 0) {
            sysml_conditional_7 = sysml_state_curAnglePercent;
        }
        else {
            sysml_conditional_7 = 0;
        }
        sysml_conditional_6 = sysml_conditional_7;
    }
    else {
        sysml_conditional_6 = 100;
    }
    sysml_value_2 = sysml_conditional_6;
    *io_4 = sysml_value_0;
    *io_5 = sysml_value_1;
    *io_6 = sysml_value_2;
}
// END COMPILED SYSML MODEL

// LLM4MDD target-board: arduino:avr:uno
#include <Servo.h>
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
static bool hw_value_io_2 = false;
static uint32_t hw_sampled_io_2 = 0;
static bool hw_attempted_io_2 = false;
static bool hw_value_io_3 = false;
static uint32_t hw_sampled_io_3 = 0;
static bool hw_attempted_io_3 = false;
static bool hw_value_io_4 = false;
static double hw_value_io_5 = 0;
Servo hw_servo_io_5;
static int hw_servo_previous_io_5 = -1;
static int32_t hw_value_io_6 = 0;
TM1637Display hw_display_io_6(4, 5);
static int32_t hw_display_previous_io_6 = INT32_MIN;
static int32_t hw_value_io_7 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(10, OUTPUT);
  digitalWrite(10, hw_value_io_4 ? HIGH : LOW);
  hw_servo_io_5.attach(9);
  if (isfinite((double)hw_value_io_5) && hw_value_io_5 >= 0 && hw_value_io_5 <= 180) {
    int command = (int)hw_value_io_5;
    if (command != hw_servo_previous_io_5) {
      hw_servo_io_5.write(command); hw_servo_previous_io_5 = command;
    }
  }
  hw_display_io_6.setBrightness(7, true);
  if (hw_value_io_6 != hw_display_previous_io_6) {
    if (hw_value_io_6 < -999 || hw_value_io_6 > 9999) hw_display_io_6.clear();
    else hw_display_io_6.showNumberDec((int)hw_value_io_6, false, 4, 0);
    hw_display_previous_io_6 = hw_value_io_6;
  }
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
  if (!hw_attempted_io_2 || (uint32_t)(hw_now - hw_sampled_io_2) >= 10UL) {
    hw_attempted_io_2 = true; hw_sampled_io_2 = hw_now;
    hw_value_io_2 = (digitalRead(2) == LOW);
  }
  if (!hw_attempted_io_3 || (uint32_t)(hw_now - hw_sampled_io_3) >= 10UL) {
    hw_attempted_io_3 = true; hw_sampled_io_3 = hw_now;
    hw_value_io_3 = (digitalRead(3) == LOW);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_7 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, hw_value_io_3, hw_value_io_7, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6);
  digitalWrite(10, hw_value_io_4 ? HIGH : LOW);
  if (isfinite((double)hw_value_io_5) && hw_value_io_5 >= 0 && hw_value_io_5 <= 180) {
    int command = (int)hw_value_io_5;
    if (command != hw_servo_previous_io_5) {
      hw_servo_io_5.write(command); hw_servo_previous_io_5 = command;
    }
  }
  if (hw_value_io_6 != hw_display_previous_io_6) {
    if (hw_value_io_6 < -999 || hw_value_io_6 > 9999) hw_display_io_6.clear();
    else hw_display_io_6.showNumberDec((int)hw_value_io_6, false, 4, 0);
    hw_display_previous_io_6 = hw_value_io_6;
  }
}
