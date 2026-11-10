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
extern double sysml_state_servoAngle;
/* Global Variable define of  */
extern bool sysml_state_previousBtn1Pressed;
/* Global Variable define of  */
extern bool sysml_state_previousBtn2Pressed;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double* io_2, bool* io_3);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_servoAngle = 90.0;
/* Global Variable define of  */
bool sysml_state_previousBtn1Pressed = false;
/* Global Variable define of  */
bool sysml_state_previousBtn2Pressed = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_servoAngle = 90.0;
    sysml_state_previousBtn1Pressed = false;
    sysml_state_previousBtn2Pressed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double* io_2, bool* io_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    double sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    double sysml_conditional_3 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousBtn1Pressed;
    }
    if (sysml_lazy_0) {
        if (sysml_state_servoAngle >= 150.0) {
            sysml_conditional_1 = sysml_state_servoAngle;
        }
        else {
            sysml_conditional_1 = sysml_state_servoAngle + 30.0;
        }
        sysml_state_servoAngle = sysml_conditional_1;
    }
    sysml_lazy_2 = io_1;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_previousBtn2Pressed;
    }
    if (sysml_lazy_2) {
        if (sysml_state_servoAngle <= 30.0) {
            sysml_conditional_3 = sysml_state_servoAngle;
        }
        else {
            sysml_conditional_3 = sysml_state_servoAngle - 30.0;
        }
        sysml_state_servoAngle = sysml_conditional_3;
    }
    sysml_state_previousBtn1Pressed = io_0;
    sysml_state_previousBtn2Pressed = io_1;
    double sysml_value_0 = 0;
    sysml_value_0 = sysml_state_servoAngle;
    bool sysml_value_1 = 0;
    sysml_value_1 = false;
    *io_2 = sysml_value_0;
    *io_3 = sysml_value_1;
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
static bool hw_value_io_0 = true;
static uint32_t hw_sampled_io_0 = 0;
static bool hw_attempted_io_0 = false;
static bool hw_value_io_1 = true;
static uint32_t hw_sampled_io_1 = 0;
static bool hw_attempted_io_1 = false;
static double hw_value_io_2 = 90;
Servo hw_servo_io_2;
static int hw_servo_previous_io_2 = -1;
static bool hw_value_io_3 = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  hw_servo_io_2.attach(5);
  if (isfinite((double)hw_value_io_2) && hw_value_io_2 >= 0 && hw_value_io_2 <= 180) {
    int command = (int)hw_value_io_2;
    if (command != hw_servo_previous_io_2) {
      hw_servo_io_2.write(command); hw_servo_previous_io_2 = command;
    }
  }
  pinMode(2, OUTPUT);
  digitalWrite(2, hw_value_io_3 ? HIGH : LOW);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 5UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (digitalRead(3) == LOW);
  }
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 5UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = (digitalRead(4) == LOW);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, &hw_value_io_2, &hw_value_io_3);
  if (isfinite((double)hw_value_io_2) && hw_value_io_2 >= 0 && hw_value_io_2 <= 180) {
    int command = (int)hw_value_io_2;
    if (command != hw_servo_previous_io_2) {
      hw_servo_io_2.write(command); hw_servo_previous_io_2 = command;
    }
  }
  digitalWrite(2, hw_value_io_3 ? HIGH : LOW);
}
