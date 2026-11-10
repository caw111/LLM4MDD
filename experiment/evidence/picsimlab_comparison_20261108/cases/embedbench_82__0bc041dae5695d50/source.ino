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
extern bool sysml_state_previousPressed1;
/* Global Variable define of  */
extern bool sysml_state_previousPressed2;
/* Global Variable define of  */
extern bool sysml_state_previousPressed3;
/* Global Variable define of  */
extern double sysml_state_holdPressedSeconds;
/* Global Variable define of  */
extern int32_t sysml_state_activeState;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, double* io_3, double* io_4, double* io_5, double* io_6, double* io_7);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousPressed1 = false;
/* Global Variable define of  */
bool sysml_state_previousPressed2 = false;
/* Global Variable define of  */
bool sysml_state_previousPressed3 = false;
/* Global Variable define of  */
double sysml_state_holdPressedSeconds = 0.0;
/* Global Variable define of  */
int32_t sysml_state_activeState = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousPressed1 = false;
    sysml_state_previousPressed2 = false;
    sysml_state_previousPressed3 = false;
    sysml_state_holdPressedSeconds = 0.0;
    sysml_state_activeState = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, double* io_3, double* io_4, double* io_5, double* io_6, double* io_7) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    double sysml_conditional_7 = 0;
    double sysml_conditional_8 = 0;
    double sysml_conditional_9 = 0;
    bool sysml_lazy_10 = 0;
    double sysml_conditional_11 = 0;
    double sysml_conditional_12 = 0;
    sysml_lazy_0 = io_2;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed3;
    }
    if (sysml_lazy_0) {
        sysml_state_activeState = 3;
        sysml_state_holdPressedSeconds = 0.0;
    }
    sysml_lazy_1 = io_0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_previousPressed1;
    }
    if (sysml_lazy_1) {
        sysml_state_activeState = 1;
        sysml_state_holdPressedSeconds = 0.0;
    }
    sysml_lazy_2 = io_1;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_previousPressed2;
    }
    if (sysml_lazy_2) {
        sysml_state_activeState = 2;
        sysml_state_holdPressedSeconds = 0.0;
    }
    sysml_lazy_3 = io_2;
    if (!sysml_lazy_3) {
        sysml_lazy_3 = io_0;
    }
    sysml_lazy_4 = sysml_lazy_3;
    if (!sysml_lazy_4) {
        sysml_lazy_4 = io_1;
    }
    if (!sysml_lazy_4) {
        sysml_state_holdPressedSeconds = 2.0;
    }
    sysml_lazy_5 = io_2;
    if (!sysml_lazy_5) {
        sysml_lazy_5 = io_0;
    }
    sysml_lazy_6 = sysml_lazy_5;
    if (!sysml_lazy_6) {
        sysml_lazy_6 = io_1;
    }
    if (sysml_lazy_6) {
        sysml_state_holdPressedSeconds = 2.0;
    }
    sysml_state_previousPressed1 = io_0;
    sysml_state_previousPressed2 = io_1;
    sysml_state_previousPressed3 = io_2;
    double sysml_value_0 = 0;
    if (sysml_state_activeState == 1) {
        sysml_conditional_7 = 1.0;
    }
    else {
        sysml_conditional_7 = 0.0;
    }
    sysml_value_0 = sysml_conditional_7;
    double sysml_value_1 = 0;
    if (sysml_state_activeState == 2) {
        sysml_conditional_8 = 1.0;
    }
    else {
        sysml_conditional_8 = 0.0;
    }
    sysml_value_1 = sysml_conditional_8;
    double sysml_value_2 = 0;
    if (sysml_state_activeState == 3) {
        sysml_conditional_9 = 1.0;
    }
    else {
        sysml_conditional_9 = 0.0;
    }
    sysml_value_2 = sysml_conditional_9;
    double sysml_value_3 = 0;
    sysml_lazy_10 = sysml_state_activeState == 1;
    if (!sysml_lazy_10) {
        sysml_lazy_10 = sysml_state_activeState == 2;
    }
    if (sysml_lazy_10) {
        sysml_conditional_11 = 90.0;
    }
    else {
        sysml_conditional_11 = 0.0;
    }
    sysml_value_3 = sysml_conditional_11;
    double sysml_value_4 = 0;
    if (sysml_state_activeState == 2) {
        sysml_conditional_12 = 90.0;
    }
    else {
        sysml_conditional_12 = 0.0;
    }
    sysml_value_4 = sysml_conditional_12;
    *io_3 = sysml_value_0;
    *io_4 = sysml_value_1;
    *io_5 = sysml_value_2;
    *io_6 = sysml_value_3;
    *io_7 = sysml_value_4;
}
// END COMPILED SYSML MODEL

// LLM4MDD target-board: arduino:avr:mega
#include <Servo.h>
#include <Arduino.h>
#include <math.h>
#if defined(ARDUINO) && !defined(ARDUINO_AVR_MEGA2560)
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
static bool hw_value_io_2 = false;
static uint32_t hw_sampled_io_2 = 0;
static bool hw_attempted_io_2 = false;
static double hw_value_io_3 = 0;
static double hw_value_io_4 = 0;
static double hw_value_io_5 = 0;
static double hw_value_io_6 = 0;
Servo hw_servo_io_6;
static int hw_servo_previous_io_6 = -1;
static double hw_value_io_7 = 0;
Servo hw_servo_io_7;
static int hw_servo_previous_io_7 = -1;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, OUTPUT);
  analogWrite(5, (int)constrain((hw_value_io_3 * 1 + 0), 0.0, 255.0));
  pinMode(6, OUTPUT);
  analogWrite(6, (int)constrain((hw_value_io_4 * 1 + 0), 0.0, 255.0));
  pinMode(7, OUTPUT);
  analogWrite(7, (int)constrain((hw_value_io_5 * 1 + 0), 0.0, 255.0));
  hw_servo_io_6.attach(8);
  if (isfinite((double)hw_value_io_6) && hw_value_io_6 >= 0 && hw_value_io_6 <= 180) {
    int command = (int)hw_value_io_6;
    if (command != hw_servo_previous_io_6) {
      hw_servo_io_6.write(command); hw_servo_previous_io_6 = command;
    }
  }
  hw_servo_io_7.attach(9);
  if (isfinite((double)hw_value_io_7) && hw_value_io_7 >= 0 && hw_value_io_7 <= 180) {
    int command = (int)hw_value_io_7;
    if (command != hw_servo_previous_io_7) {
      hw_servo_io_7.write(command); hw_servo_previous_io_7 = command;
    }
  }
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 10UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (digitalRead(2) == LOW);
  }
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 10UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = (digitalRead(3) == LOW);
  }
  if (!hw_attempted_io_2 || (uint32_t)(hw_now - hw_sampled_io_2) >= 10UL) {
    hw_attempted_io_2 = true; hw_sampled_io_2 = hw_now;
    hw_value_io_2 = (digitalRead(4) == LOW);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_io_7);
  analogWrite(5, (int)constrain((hw_value_io_3 * 1 + 0), 0.0, 255.0));
  analogWrite(6, (int)constrain((hw_value_io_4 * 1 + 0), 0.0, 255.0));
  analogWrite(7, (int)constrain((hw_value_io_5 * 1 + 0), 0.0, 255.0));
  if (isfinite((double)hw_value_io_6) && hw_value_io_6 >= 0 && hw_value_io_6 <= 180) {
    int command = (int)hw_value_io_6;
    if (command != hw_servo_previous_io_6) {
      hw_servo_io_6.write(command); hw_servo_previous_io_6 = command;
    }
  }
  if (isfinite((double)hw_value_io_7) && hw_value_io_7 >= 0 && hw_value_io_7 <= 180) {
    int command = (int)hw_value_io_7;
    if (command != hw_servo_previous_io_7) {
      hw_servo_io_7.write(command); hw_servo_previous_io_7 = command;
    }
  }
}
