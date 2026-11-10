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
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double* io_1, double* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7, bool* io_8, bool* io_9, bool* io_10, bool* io_11, bool* io_12);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double* io_1, double* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7, bool* io_8, bool* io_9, bool* io_10, bool* io_11, bool* io_12) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_value_0 = 0;
    int32_t sysml_conditional_0 = 0;
    int32_t sysml_conditional_1 = 0;
    int32_t sysml_conditional_2 = 0;
    int32_t sysml_conditional_3 = 0;
    int32_t sysml_conditional_4 = 0;
    int32_t sysml_conditional_5 = 0;
    int32_t sysml_conditional_6 = 0;
    int32_t sysml_conditional_7 = 0;
    int32_t sysml_conditional_8 = 0;
    int32_t sysml_conditional_9 = 0;
    sysml_value_0 = ((double)((io_0 * 180.0)) / 1023.0);
    double sysml_value_1 = 0;
    sysml_value_1 = (180.0 - ((double)((io_0 * 180.0)) / 1023.0));
    bool sysml_value_2 = 0;
    if ((((double)(io_0 * 10.0)) / 1023.0) < ((double)((int32_t)(((double)(io_0 * 10.0)) / 1023.0)))) {
        sysml_conditional_0 = 1;
    }
    else {
        sysml_conditional_0 = 0;
    }
    sysml_value_2 = (((int32_t)(((double)(io_0 * 10.0)) / 1023.0)) - sysml_conditional_0) >= 1;
    bool sysml_value_3 = 0;
    if ((((double)(io_0 * 10.0)) / 1023.0) < ((double)((int32_t)(((double)(io_0 * 10.0)) / 1023.0)))) {
        sysml_conditional_1 = 1;
    }
    else {
        sysml_conditional_1 = 0;
    }
    sysml_value_3 = (((int32_t)(((double)(io_0 * 10.0)) / 1023.0)) - sysml_conditional_1) >= 2;
    bool sysml_value_4 = 0;
    if ((((double)(io_0 * 10.0)) / 1023.0) < ((double)((int32_t)(((double)(io_0 * 10.0)) / 1023.0)))) {
        sysml_conditional_2 = 1;
    }
    else {
        sysml_conditional_2 = 0;
    }
    sysml_value_4 = (((int32_t)(((double)(io_0 * 10.0)) / 1023.0)) - sysml_conditional_2) >= 3;
    bool sysml_value_5 = 0;
    if ((((double)(io_0 * 10.0)) / 1023.0) < ((double)((int32_t)(((double)(io_0 * 10.0)) / 1023.0)))) {
        sysml_conditional_3 = 1;
    }
    else {
        sysml_conditional_3 = 0;
    }
    sysml_value_5 = (((int32_t)(((double)(io_0 * 10.0)) / 1023.0)) - sysml_conditional_3) >= 4;
    bool sysml_value_6 = 0;
    if ((((double)(io_0 * 10.0)) / 1023.0) < ((double)((int32_t)(((double)(io_0 * 10.0)) / 1023.0)))) {
        sysml_conditional_4 = 1;
    }
    else {
        sysml_conditional_4 = 0;
    }
    sysml_value_6 = (((int32_t)(((double)(io_0 * 10.0)) / 1023.0)) - sysml_conditional_4) >= 5;
    bool sysml_value_7 = 0;
    if ((((double)(io_0 * 10.0)) / 1023.0) < ((double)((int32_t)(((double)(io_0 * 10.0)) / 1023.0)))) {
        sysml_conditional_5 = 1;
    }
    else {
        sysml_conditional_5 = 0;
    }
    sysml_value_7 = (((int32_t)(((double)(io_0 * 10.0)) / 1023.0)) - sysml_conditional_5) >= 6;
    bool sysml_value_8 = 0;
    if ((((double)(io_0 * 10.0)) / 1023.0) < ((double)((int32_t)(((double)(io_0 * 10.0)) / 1023.0)))) {
        sysml_conditional_6 = 1;
    }
    else {
        sysml_conditional_6 = 0;
    }
    sysml_value_8 = (((int32_t)(((double)(io_0 * 10.0)) / 1023.0)) - sysml_conditional_6) >= 7;
    bool sysml_value_9 = 0;
    if ((((double)(io_0 * 10.0)) / 1023.0) < ((double)((int32_t)(((double)(io_0 * 10.0)) / 1023.0)))) {
        sysml_conditional_7 = 1;
    }
    else {
        sysml_conditional_7 = 0;
    }
    sysml_value_9 = (((int32_t)(((double)(io_0 * 10.0)) / 1023.0)) - sysml_conditional_7) >= 8;
    bool sysml_value_10 = 0;
    if ((((double)(io_0 * 10.0)) / 1023.0) < ((double)((int32_t)(((double)(io_0 * 10.0)) / 1023.0)))) {
        sysml_conditional_8 = 1;
    }
    else {
        sysml_conditional_8 = 0;
    }
    sysml_value_10 = (((int32_t)(((double)(io_0 * 10.0)) / 1023.0)) - sysml_conditional_8) >= 9;
    bool sysml_value_11 = 0;
    if ((((double)(io_0 * 10.0)) / 1023.0) < ((double)((int32_t)(((double)(io_0 * 10.0)) / 1023.0)))) {
        sysml_conditional_9 = 1;
    }
    else {
        sysml_conditional_9 = 0;
    }
    sysml_value_11 = (((int32_t)(((double)(io_0 * 10.0)) / 1023.0)) - sysml_conditional_9) >= 10;
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
    *io_3 = sysml_value_2;
    *io_4 = sysml_value_3;
    *io_5 = sysml_value_4;
    *io_6 = sysml_value_5;
    *io_7 = sysml_value_6;
    *io_8 = sysml_value_7;
    *io_9 = sysml_value_8;
    *io_10 = sysml_value_9;
    *io_11 = sysml_value_10;
    *io_12 = sysml_value_11;
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
static uint32_t hw_sampled_io_0 = 0;
static bool hw_attempted_io_0 = false;
static double hw_value_io_1 = 0;
Servo hw_servo_io_1;
static int hw_servo_previous_io_1 = -1;
static double hw_value_io_2 = 0;
Servo hw_servo_io_2;
static int hw_servo_previous_io_2 = -1;
static bool hw_value_io_3 = false;
static bool hw_value_io_4 = false;
static bool hw_value_io_5 = false;
static bool hw_value_io_6 = false;
static bool hw_value_io_7 = false;
static bool hw_value_io_8 = false;
static bool hw_value_io_9 = false;
static bool hw_value_io_10 = false;
static bool hw_value_io_11 = false;
static bool hw_value_io_12 = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(A3, INPUT);
  hw_servo_io_1.attach(2);
  if (isfinite((double)hw_value_io_1) && hw_value_io_1 >= 0 && hw_value_io_1 <= 180) {
    int command = (int)hw_value_io_1;
    if (command != hw_servo_previous_io_1) {
      hw_servo_io_1.write(command); hw_servo_previous_io_1 = command;
    }
  }
  hw_servo_io_2.attach(3);
  if (isfinite((double)hw_value_io_2) && hw_value_io_2 >= 0 && hw_value_io_2 <= 180) {
    int command = (int)hw_value_io_2;
    if (command != hw_servo_previous_io_2) {
      hw_servo_io_2.write(command); hw_servo_previous_io_2 = command;
    }
  }
  pinMode(4, OUTPUT);
  digitalWrite(4, hw_value_io_3 ? HIGH : LOW);
  pinMode(5, OUTPUT);
  digitalWrite(5, hw_value_io_4 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  digitalWrite(6, hw_value_io_5 ? HIGH : LOW);
  pinMode(7, OUTPUT);
  digitalWrite(7, hw_value_io_6 ? HIGH : LOW);
  pinMode(8, OUTPUT);
  digitalWrite(8, hw_value_io_7 ? HIGH : LOW);
  pinMode(9, OUTPUT);
  digitalWrite(9, hw_value_io_8 ? HIGH : LOW);
  pinMode(10, OUTPUT);
  digitalWrite(10, hw_value_io_9 ? HIGH : LOW);
  pinMode(11, OUTPUT);
  digitalWrite(11, hw_value_io_10 ? HIGH : LOW);
  pinMode(12, OUTPUT);
  digitalWrite(12, hw_value_io_11 ? HIGH : LOW);
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_12 ? HIGH : LOW);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 5UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (analogRead(A3) * 1 + 0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_io_7, &hw_value_io_8, &hw_value_io_9, &hw_value_io_10, &hw_value_io_11, &hw_value_io_12);
  if (isfinite((double)hw_value_io_1) && hw_value_io_1 >= 0 && hw_value_io_1 <= 180) {
    int command = (int)hw_value_io_1;
    if (command != hw_servo_previous_io_1) {
      hw_servo_io_1.write(command); hw_servo_previous_io_1 = command;
    }
  }
  if (isfinite((double)hw_value_io_2) && hw_value_io_2 >= 0 && hw_value_io_2 <= 180) {
    int command = (int)hw_value_io_2;
    if (command != hw_servo_previous_io_2) {
      hw_servo_io_2.write(command); hw_servo_previous_io_2 = command;
    }
  }
  digitalWrite(4, hw_value_io_3 ? HIGH : LOW);
  digitalWrite(5, hw_value_io_4 ? HIGH : LOW);
  digitalWrite(6, hw_value_io_5 ? HIGH : LOW);
  digitalWrite(7, hw_value_io_6 ? HIGH : LOW);
  digitalWrite(8, hw_value_io_7 ? HIGH : LOW);
  digitalWrite(9, hw_value_io_8 ? HIGH : LOW);
  digitalWrite(10, hw_value_io_9 ? HIGH : LOW);
  digitalWrite(11, hw_value_io_10 ? HIGH : LOW);
  digitalWrite(12, hw_value_io_11 ? HIGH : LOW);
  digitalWrite(13, hw_value_io_12 ? HIGH : LOW);
}
