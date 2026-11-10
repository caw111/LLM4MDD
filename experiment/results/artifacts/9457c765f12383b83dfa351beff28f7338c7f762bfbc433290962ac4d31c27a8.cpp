#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_52\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
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
extern int32_t sysml_state_servoAngleAccumMs;
/* Global Variable define of  */
extern double sysml_state_servoAngle1;
/* Global Variable define of  */
extern double sysml_state_servoAngle2;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_8, double* io_0, double* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7, bool* io_9, bool* io_10, bool* io_11, bool* io_12, bool* io_13, bool* io_14);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_servoAngleAccumMs = 0;
/* Global Variable define of  */
double sysml_state_servoAngle1 = 0.0;
/* Global Variable define of  */
double sysml_state_servoAngle2 = 0.0;
/* Init function of CompositeActor named "ProgramPage1" */
#line 187 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_52\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 229 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_52\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_52\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_servoAngleAccumMs = 0;
    sysml_state_servoAngle1 = 0.0;
    sysml_state_servoAngle2 = 0.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_8, double* io_0, double* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7, bool* io_9, bool* io_10, bool* io_11, bool* io_12, bool* io_13, bool* io_14) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_conditional_7 = 0;
    sysml_state_servoAngleAccumMs = (sysml_state_servoAngleAccumMs + io_8);
    if ((sysml_state_servoAngleAccumMs >= 2000)) {
        sysml_state_servoAngleAccumMs = (sysml_state_servoAngleAccumMs - 2000);
        if ((sysml_state_servoAngle1 >= 180.0)) {
            sysml_state_servoAngle1 = 0.0;
            sysml_state_servoAngle2 = 0.0;
        }
        else {
            sysml_state_servoAngle1 = (sysml_state_servoAngle1 + 30.0);
            sysml_state_servoAngle2 = (sysml_state_servoAngle2 + 30.0);
        }
        if ((sysml_state_servoAngle1 >= 180.0)) {
            sysml_state_servoAngle1 = 0.0;
            sysml_state_servoAngle2 = 0.0;
        }
    }
    if ((sysml_state_servoAngle1 == 0.0)) {
        sysml_state_servoAngle2 = 0.0;
    }
    double sysml_value_0 = 0;
    sysml_value_0 = sysml_state_servoAngle1;
    double sysml_value_1 = 0;
    sysml_value_1 = sysml_state_servoAngle2;
    bool sysml_value_2 = 0;
    sysml_lazy_0 = sysml_state_servoAngle1 == 0.0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = sysml_state_servoAngle2 == 0.0;
    }
    if (sysml_lazy_0) {
        sysml_conditional_1 = false;
    }
    else {
        sysml_conditional_1 = true;
    }
    sysml_value_2 = sysml_conditional_1;
    bool sysml_value_3 = 0;
    sysml_value_3 = false;
    bool sysml_value_4 = 0;
    sysml_value_4 = false;
    bool sysml_value_5 = 0;
    sysml_lazy_2 = sysml_state_servoAngle1 == 0.0;
    if (sysml_lazy_2) {
        sysml_lazy_2 = sysml_state_servoAngle2 == 0.0;
    }
    if (sysml_lazy_2) {
        sysml_conditional_3 = false;
    }
    else {
        sysml_conditional_3 = true;
    }
    sysml_value_5 = sysml_conditional_3;
    bool sysml_value_6 = 0;
    sysml_value_6 = false;
    bool sysml_value_7 = 0;
    sysml_value_7 = true;
    bool sysml_value_8 = 0;
    sysml_lazy_4 = (((double)(sysml_state_servoAngle1 + sysml_state_servoAngle2)) / 2.0) >= 0.0;
    if (sysml_lazy_4) {
        sysml_lazy_4 = (((double)(sysml_state_servoAngle1 + sysml_state_servoAngle2)) / 2.0) <= 90.0;
    }
    if (sysml_lazy_4) {
        sysml_conditional_5 = true;
    }
    else {
        sysml_conditional_5 = false;
    }
    sysml_value_8 = sysml_conditional_5;
    bool sysml_value_9 = 0;
    sysml_lazy_6 = (((double)(sysml_state_servoAngle1 + sysml_state_servoAngle2)) / 2.0) > 0.0;
    if (sysml_lazy_6) {
        sysml_lazy_6 = (((double)(sysml_state_servoAngle1 + sysml_state_servoAngle2)) / 2.0) <= 180.0;
    }
    if (sysml_lazy_6) {
        sysml_conditional_7 = true;
    }
    else {
        sysml_conditional_7 = false;
    }
    sysml_value_9 = sysml_conditional_7;
    bool sysml_value_10 = 0;
    sysml_value_10 = false;
    bool sysml_value_11 = 0;
    sysml_value_11 = false;
    bool sysml_value_12 = 0;
    sysml_value_12 = false;
    bool sysml_value_13 = 0;
    sysml_value_13 = false;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
    *io_4 = sysml_value_4;
    *io_5 = sysml_value_5;
    *io_6 = sysml_value_6;
    *io_7 = sysml_value_7;
    *io_9 = sysml_value_8;
    *io_10 = sysml_value_9;
    *io_11 = sysml_value_10;
    *io_12 = sysml_value_11;
    *io_13 = sysml_value_12;
    *io_14 = sysml_value_13;
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
static double hw_value_io_0 = 0;
Servo hw_servo_io_0;
static int hw_servo_previous_io_0 = -1;
static double hw_value_io_1 = 0;
Servo hw_servo_io_1;
static int hw_servo_previous_io_1 = -1;
static bool hw_value_io_2 = false;
static bool hw_value_io_3 = false;
static bool hw_value_io_4 = false;
static bool hw_value_io_5 = false;
static bool hw_value_io_6 = false;
static bool hw_value_io_7 = false;
static int32_t hw_value_io_8 = 0;
static bool hw_value_io_9 = false;
static bool hw_value_io_10 = false;
static bool hw_value_io_11 = false;
static bool hw_value_io_12 = false;
static bool hw_value_io_13 = false;
static bool hw_value_io_14 = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  hw_servo_io_0.attach(8);
  if (isfinite((double)hw_value_io_0) && hw_value_io_0 >= 0 && hw_value_io_0 <= 180) {
    int command = (int)hw_value_io_0;
    if (command != hw_servo_previous_io_0) {
      hw_servo_io_0.write(command); hw_servo_previous_io_0 = command;
    }
  }
  hw_servo_io_1.attach(9);
  if (isfinite((double)hw_value_io_1) && hw_value_io_1 >= 0 && hw_value_io_1 <= 180) {
    int command = (int)hw_value_io_1;
    if (command != hw_servo_previous_io_1) {
      hw_servo_io_1.write(command); hw_servo_previous_io_1 = command;
    }
  }
  pinMode(2, OUTPUT);
  digitalWrite(2, hw_value_io_2 ? HIGH : LOW);
  pinMode(3, OUTPUT);
  digitalWrite(3, hw_value_io_3 ? HIGH : LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, hw_value_io_4 ? HIGH : LOW);
  pinMode(5, OUTPUT);
  digitalWrite(5, hw_value_io_5 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  digitalWrite(6, hw_value_io_6 ? HIGH : LOW);
  pinMode(7, OUTPUT);
  digitalWrite(7, hw_value_io_7 ? HIGH : LOW);
  pinMode(10, OUTPUT);
  digitalWrite(10, hw_value_io_9 ? HIGH : LOW);
  pinMode(11, OUTPUT);
  digitalWrite(11, hw_value_io_10 ? HIGH : LOW);
  pinMode(12, OUTPUT);
  digitalWrite(12, hw_value_io_11 ? HIGH : LOW);
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_12 ? HIGH : LOW);
  pinMode(44, OUTPUT);
  digitalWrite(44, hw_value_io_13 ? HIGH : LOW);
  pinMode(45, OUTPUT);
  digitalWrite(45, hw_value_io_14 ? HIGH : LOW);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_8 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_8, &hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_io_7, &hw_value_io_9, &hw_value_io_10, &hw_value_io_11, &hw_value_io_12, &hw_value_io_13, &hw_value_io_14);
  if (isfinite((double)hw_value_io_0) && hw_value_io_0 >= 0 && hw_value_io_0 <= 180) {
    int command = (int)hw_value_io_0;
    if (command != hw_servo_previous_io_0) {
      hw_servo_io_0.write(command); hw_servo_previous_io_0 = command;
    }
  }
  if (isfinite((double)hw_value_io_1) && hw_value_io_1 >= 0 && hw_value_io_1 <= 180) {
    int command = (int)hw_value_io_1;
    if (command != hw_servo_previous_io_1) {
      hw_servo_io_1.write(command); hw_servo_previous_io_1 = command;
    }
  }
  digitalWrite(2, hw_value_io_2 ? HIGH : LOW);
  digitalWrite(3, hw_value_io_3 ? HIGH : LOW);
  digitalWrite(4, hw_value_io_4 ? HIGH : LOW);
  digitalWrite(5, hw_value_io_5 ? HIGH : LOW);
  digitalWrite(6, hw_value_io_6 ? HIGH : LOW);
  digitalWrite(7, hw_value_io_7 ? HIGH : LOW);
  digitalWrite(10, hw_value_io_9 ? HIGH : LOW);
  digitalWrite(11, hw_value_io_10 ? HIGH : LOW);
  digitalWrite(12, hw_value_io_11 ? HIGH : LOW);
  digitalWrite(13, hw_value_io_12 ? HIGH : LOW);
  digitalWrite(44, hw_value_io_13 ? HIGH : LOW);
  digitalWrite(45, hw_value_io_14 ? HIGH : LOW);
}

