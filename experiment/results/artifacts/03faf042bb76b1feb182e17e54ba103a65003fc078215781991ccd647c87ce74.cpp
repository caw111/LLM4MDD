#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_118\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
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
extern bool sysml_state_previousBtn1;
/* Global Variable define of  */
extern bool sysml_state_previousBtn2;
/* Global Variable define of  */
extern bool sysml_state_movingLed;
/* Global Variable define of  */
extern double sysml_state_movingUntilAngle;
/* Global Variable define of  */
extern bool sysml_state_movingHoldActive;
/* Global Variable define of  */
extern int32_t sysml_state_movingHoldMsRemaining;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_1, bool io_2, bool* io_0, double* io_3);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_servoAngle = 90.0;
/* Global Variable define of  */
bool sysml_state_previousBtn1 = false;
/* Global Variable define of  */
bool sysml_state_previousBtn2 = false;
/* Global Variable define of  */
bool sysml_state_movingLed = false;
/* Global Variable define of  */
double sysml_state_movingUntilAngle = 90.0;
/* Global Variable define of  */
bool sysml_state_movingHoldActive = false;
/* Global Variable define of  */
int32_t sysml_state_movingHoldMsRemaining = 0;
/* Init function of CompositeActor named "ProgramPage1" */
#line 190 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_118\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 205 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_118\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 53 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_118\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_servoAngle = 90.0;
    sysml_state_previousBtn1 = false;
    sysml_state_previousBtn2 = false;
    sysml_state_movingLed = false;
    sysml_state_movingUntilAngle = 90.0;
    sysml_state_movingHoldActive = false;
    sysml_state_movingHoldMsRemaining = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_1, bool io_2, bool* io_0, double* io_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    double sysml_conditional_1 = 0;
    double sysml_conditional_2 = 0;
    bool sysml_conditional_3 = 0;
    int32_t sysml_conditional_4 = 0;
    bool sysml_lazy_5 = 0;
    double sysml_conditional_6 = 0;
    double sysml_conditional_7 = 0;
    bool sysml_conditional_8 = 0;
    int32_t sysml_conditional_9 = 0;
    bool sysml_lazy_10 = 0;
    sysml_state_previousBtn1 = io_1;
    sysml_state_previousBtn2 = io_2;
    sysml_state_movingLed = false;
    sysml_state_movingHoldActive = sysml_state_movingHoldActive;
    sysml_lazy_0 = io_1;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousBtn1;
    }
    if (sysml_lazy_0) {
        if (sysml_state_servoAngle <= 150.0) {
            sysml_conditional_1 = sysml_state_servoAngle + 30.0;
        }
        else {
            sysml_conditional_1 = sysml_state_servoAngle;
        }
        sysml_state_servoAngle = sysml_conditional_1;
        if (sysml_state_servoAngle <= 150.0) {
            sysml_conditional_2 = sysml_state_servoAngle + 30.0;
        }
        else {
            sysml_conditional_2 = sysml_state_servoAngle;
        }
        sysml_state_movingUntilAngle = sysml_conditional_2;
        if (sysml_state_servoAngle <= 150.0) {
            sysml_conditional_3 = true;
        }
        else {
            sysml_conditional_3 = false;
        }
        sysml_state_movingHoldActive = sysml_conditional_3;
        if (sysml_state_servoAngle <= 150.0) {
            sysml_conditional_4 = 2000;
        }
        else {
            sysml_conditional_4 = 0;
        }
        sysml_state_movingHoldMsRemaining = sysml_conditional_4;
    }
    sysml_lazy_5 = io_2;
    if (sysml_lazy_5) {
        sysml_lazy_5 = !sysml_state_previousBtn2;
    }
    if (sysml_lazy_5) {
        if (sysml_state_servoAngle >= 30.0) {
            sysml_conditional_6 = sysml_state_servoAngle - 30.0;
        }
        else {
            sysml_conditional_6 = sysml_state_servoAngle;
        }
        sysml_state_servoAngle = sysml_conditional_6;
        if (sysml_state_servoAngle >= 30.0) {
            sysml_conditional_7 = sysml_state_servoAngle - 30.0;
        }
        else {
            sysml_conditional_7 = sysml_state_servoAngle;
        }
        sysml_state_movingUntilAngle = sysml_conditional_7;
        if (sysml_state_servoAngle >= 30.0) {
            sysml_conditional_8 = true;
        }
        else {
            sysml_conditional_8 = false;
        }
        sysml_state_movingHoldActive = sysml_conditional_8;
        if (sysml_state_servoAngle >= 30.0) {
            sysml_conditional_9 = 2000;
        }
        else {
            sysml_conditional_9 = 0;
        }
        sysml_state_movingHoldMsRemaining = sysml_conditional_9;
    }
    if (sysml_state_movingHoldActive) {
        sysml_state_movingLed = true;
    }
    sysml_lazy_10 = sysml_state_movingHoldActive;
    if (sysml_lazy_10) {
        sysml_lazy_10 = sysml_state_servoAngle == sysml_state_movingUntilAngle;
    }
    if (sysml_lazy_10) {
        sysml_state_movingLed = false;
        sysml_state_movingHoldActive = false;
        sysml_state_movingHoldMsRemaining = 0;
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_movingLed;
    double sysml_value_1 = 0;
    sysml_value_1 = sysml_state_servoAngle;
    *io_0 = sysml_value_0;
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
static bool hw_value_io_0 = false;
static bool hw_value_io_1 = false;
static uint32_t hw_sampled_io_1 = 0;
static bool hw_attempted_io_1 = false;
static bool hw_value_io_2 = false;
static uint32_t hw_sampled_io_2 = 0;
static bool hw_attempted_io_2 = false;
static double hw_value_io_3 = 90;
Servo hw_servo_io_3;
static int hw_servo_previous_io_3 = -1;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, OUTPUT);
  digitalWrite(2, hw_value_io_0 ? HIGH : LOW);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  hw_servo_io_3.attach(5);
  if (isfinite((double)hw_value_io_3) && hw_value_io_3 >= 0 && hw_value_io_3 <= 180) {
    int command = (int)hw_value_io_3;
    if (command != hw_servo_previous_io_3) {
      hw_servo_io_3.write(command); hw_servo_previous_io_3 = command;
    }
  }
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 5UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = (digitalRead(3) == HIGH);
  }
  if (!hw_attempted_io_2 || (uint32_t)(hw_now - hw_sampled_io_2) >= 5UL) {
    hw_attempted_io_2 = true; hw_sampled_io_2 = hw_now;
    hw_value_io_2 = (digitalRead(4) == HIGH);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_1, hw_value_io_2, &hw_value_io_0, &hw_value_io_3);
  digitalWrite(2, hw_value_io_0 ? HIGH : LOW);
  if (isfinite((double)hw_value_io_3) && hw_value_io_3 >= 0 && hw_value_io_3 <= 180) {
    int command = (int)hw_value_io_3;
    if (command != hw_servo_previous_io_3) {
      hw_servo_io_3.write(command); hw_servo_previous_io_3 = command;
    }
  }
}

