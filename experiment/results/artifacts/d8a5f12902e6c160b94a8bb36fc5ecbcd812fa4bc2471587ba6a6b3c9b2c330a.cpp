#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_31\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
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
void ProgramPage1(double io_0, double io_1, double* io_2, double* io_3, double* io_4, bool* io_5, bool* io_6, bool* io_7, bool* io_8, bool* io_9, bool* io_10, bool* io_11, bool* io_12, bool* io_13, bool* io_14);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Init function of CompositeActor named "ProgramPage1" */
#line 191 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_31\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void setup();
#line 238 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_31\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void loop();
#line 25 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_31\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, double* io_2, double* io_3, double* io_4, bool* io_5, bool* io_6, bool* io_7, bool* io_8, bool* io_9, bool* io_10, bool* io_11, bool* io_12, bool* io_13, bool* io_14) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_value_0 = 0;
    double sysml_conditional_0 = 0;
    int32_t sysml_conditional_1 = 0;
    int32_t sysml_conditional_2 = 0;
    int32_t sysml_conditional_3 = 0;
    int32_t sysml_conditional_4 = 0;
    int32_t sysml_conditional_5 = 0;
    int32_t sysml_conditional_6 = 0;
    int32_t sysml_conditional_7 = 0;
    int32_t sysml_conditional_8 = 0;
    int32_t sysml_conditional_9 = 0;
    int32_t sysml_conditional_10 = 0;
    int32_t sysml_conditional_11 = 0;
    sysml_value_0 = ((double)((io_0 * 180.0)) / 1023.0);
    double sysml_value_1 = 0;
    sysml_value_1 = ((double)((io_1 * 180.0)) / 1023.0);
    double sysml_value_2 = 0;
    if (true) {
        if (((((double)((((double)(io_0 + io_1)) / 2.0) * 180.0)) / 1023.0) + 0.5) < ((double)((int32_t)((((double)((((double)(io_0 + io_1)) / 2.0) * 180.0)) / 1023.0) + 0.5)))) {
            sysml_conditional_1 = 1;
        }
        else {
            sysml_conditional_1 = 0;
        }
        sysml_conditional_0 = ((int32_t)((((double)((((double)(io_0 + io_1)) / 2.0) * 180.0)) / 1023.0) + 0.5)) - sysml_conditional_1;
    }
    else {
        sysml_conditional_0 = 0.0;
    }
    sysml_value_2 = sysml_conditional_0;
    bool sysml_value_3 = 0;
    if ((((double)((((double)(io_0 + io_1)) / 2.0) * 10.0)) / 1023.0) < ((double)((int32_t)(((double)((((double)(io_0 + io_1)) / 2.0) * 10.0)) / 1023.0)))) {
        sysml_conditional_2 = 1;
    }
    else {
        sysml_conditional_2 = 0;
    }
    sysml_value_3 = (((int32_t)(((double)((((double)(io_0 + io_1)) / 2.0) * 10.0)) / 1023.0)) - sysml_conditional_2) >= 1;
    bool sysml_value_4 = 0;
    if ((((double)((((double)(io_0 + io_1)) / 2.0) * 10.0)) / 1023.0) < ((double)((int32_t)(((double)((((double)(io_0 + io_1)) / 2.0) * 10.0)) / 1023.0)))) {
        sysml_conditional_3 = 1;
    }
    else {
        sysml_conditional_3 = 0;
    }
    sysml_value_4 = (((int32_t)(((double)((((double)(io_0 + io_1)) / 2.0) * 10.0)) / 1023.0)) - sysml_conditional_3) >= 2;
    bool sysml_value_5 = 0;
    if ((((double)((((double)(io_0 + io_1)) / 2.0) * 10.0)) / 1023.0) < ((double)((int32_t)(((double)((((double)(io_0 + io_1)) / 2.0) * 10.0)) / 1023.0)))) {
        sysml_conditional_4 = 1;
    }
    else {
        sysml_conditional_4 = 0;
    }
    sysml_value_5 = (((int32_t)(((double)((((double)(io_0 + io_1)) / 2.0) * 10.0)) / 1023.0)) - sysml_conditional_4) >= 3;
    bool sysml_value_6 = 0;
    if ((((double)((((double)(io_0 + io_1)) / 2.0) * 10.0)) / 1023.0) < ((double)((int32_t)(((double)((((double)(io_0 + io_1)) / 2.0) * 10.0)) / 1023.0)))) {
        sysml_conditional_5 = 1;
    }
    else {
        sysml_conditional_5 = 0;
    }
    sysml_value_6 = (((int32_t)(((double)((((double)(io_0 + io_1)) / 2.0) * 10.0)) / 1023.0)) - sysml_conditional_5) >= 4;
    bool sysml_value_7 = 0;
    if ((((double)((((double)(io_0 + io_1)) / 2.0) * 10.0)) / 1023.0) < ((double)((int32_t)(((double)((((double)(io_0 + io_1)) / 2.0) * 10.0)) / 1023.0)))) {
        sysml_conditional_6 = 1;
    }
    else {
        sysml_conditional_6 = 0;
    }
    sysml_value_7 = (((int32_t)(((double)((((double)(io_0 + io_1)) / 2.0) * 10.0)) / 1023.0)) - sysml_conditional_6) >= 5;
    bool sysml_value_8 = 0;
    if ((((double)((((double)(io_0 + io_1)) / 2.0) * 10.0)) / 1023.0) < ((double)((int32_t)(((double)((((double)(io_0 + io_1)) / 2.0) * 10.0)) / 1023.0)))) {
        sysml_conditional_7 = 1;
    }
    else {
        sysml_conditional_7 = 0;
    }
    sysml_value_8 = (((int32_t)(((double)((((double)(io_0 + io_1)) / 2.0) * 10.0)) / 1023.0)) - sysml_conditional_7) >= 6;
    bool sysml_value_9 = 0;
    if ((((double)((((double)(io_0 + io_1)) / 2.0) * 10.0)) / 1023.0) < ((double)((int32_t)(((double)((((double)(io_0 + io_1)) / 2.0) * 10.0)) / 1023.0)))) {
        sysml_conditional_8 = 1;
    }
    else {
        sysml_conditional_8 = 0;
    }
    sysml_value_9 = (((int32_t)(((double)((((double)(io_0 + io_1)) / 2.0) * 10.0)) / 1023.0)) - sysml_conditional_8) >= 7;
    bool sysml_value_10 = 0;
    if ((((double)((((double)(io_0 + io_1)) / 2.0) * 10.0)) / 1023.0) < ((double)((int32_t)(((double)((((double)(io_0 + io_1)) / 2.0) * 10.0)) / 1023.0)))) {
        sysml_conditional_9 = 1;
    }
    else {
        sysml_conditional_9 = 0;
    }
    sysml_value_10 = (((int32_t)(((double)((((double)(io_0 + io_1)) / 2.0) * 10.0)) / 1023.0)) - sysml_conditional_9) >= 8;
    bool sysml_value_11 = 0;
    if ((((double)((((double)(io_0 + io_1)) / 2.0) * 10.0)) / 1023.0) < ((double)((int32_t)(((double)((((double)(io_0 + io_1)) / 2.0) * 10.0)) / 1023.0)))) {
        sysml_conditional_10 = 1;
    }
    else {
        sysml_conditional_10 = 0;
    }
    sysml_value_11 = (((int32_t)(((double)((((double)(io_0 + io_1)) / 2.0) * 10.0)) / 1023.0)) - sysml_conditional_10) >= 9;
    bool sysml_value_12 = 0;
    if ((((double)((((double)(io_0 + io_1)) / 2.0) * 10.0)) / 1023.0) < ((double)((int32_t)(((double)((((double)(io_0 + io_1)) / 2.0) * 10.0)) / 1023.0)))) {
        sysml_conditional_11 = 1;
    }
    else {
        sysml_conditional_11 = 0;
    }
    sysml_value_12 = (((int32_t)(((double)((((double)(io_0 + io_1)) / 2.0) * 10.0)) / 1023.0)) - sysml_conditional_11) >= 10;
    *io_2 = sysml_value_0;
    *io_3 = sysml_value_1;
    *io_4 = sysml_value_2;
    *io_5 = sysml_value_3;
    *io_6 = sysml_value_4;
    *io_7 = sysml_value_5;
    *io_8 = sysml_value_6;
    *io_9 = sysml_value_7;
    *io_10 = sysml_value_8;
    *io_11 = sysml_value_9;
    *io_12 = sysml_value_10;
    *io_13 = sysml_value_11;
    *io_14 = sysml_value_12;
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
static uint32_t hw_sampled_io_1 = 0;
static bool hw_attempted_io_1 = false;
static double hw_value_io_2 = 0;
Servo hw_servo_io_2;
static int hw_servo_previous_io_2 = -1;
static double hw_value_io_3 = 0;
Servo hw_servo_io_3;
static int hw_servo_previous_io_3 = -1;
static double hw_value_io_4 = 0;
Servo hw_servo_io_4;
static int hw_servo_previous_io_4 = -1;
static bool hw_value_io_5 = false;
static bool hw_value_io_6 = false;
static bool hw_value_io_7 = false;
static bool hw_value_io_8 = false;
static bool hw_value_io_9 = false;
static bool hw_value_io_10 = false;
static bool hw_value_io_11 = false;
static bool hw_value_io_12 = false;
static bool hw_value_io_13 = false;
static bool hw_value_io_14 = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  hw_servo_io_2.attach(2);
  if (isfinite((double)hw_value_io_2) && hw_value_io_2 >= 0 && hw_value_io_2 <= 180) {
    int command = (int)hw_value_io_2;
    if (command != hw_servo_previous_io_2) {
      hw_servo_io_2.write(command); hw_servo_previous_io_2 = command;
    }
  }
  hw_servo_io_3.attach(3);
  if (isfinite((double)hw_value_io_3) && hw_value_io_3 >= 0 && hw_value_io_3 <= 180) {
    int command = (int)hw_value_io_3;
    if (command != hw_servo_previous_io_3) {
      hw_servo_io_3.write(command); hw_servo_previous_io_3 = command;
    }
  }
  hw_servo_io_4.attach(4);
  if (isfinite((double)hw_value_io_4) && hw_value_io_4 >= 0 && hw_value_io_4 <= 180) {
    int command = (int)hw_value_io_4;
    if (command != hw_servo_previous_io_4) {
      hw_servo_io_4.write(command); hw_servo_previous_io_4 = command;
    }
  }
  pinMode(5, OUTPUT);
  digitalWrite(5, hw_value_io_5 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  digitalWrite(6, hw_value_io_6 ? HIGH : LOW);
  pinMode(7, OUTPUT);
  digitalWrite(7, hw_value_io_7 ? HIGH : LOW);
  pinMode(8, OUTPUT);
  digitalWrite(8, hw_value_io_8 ? HIGH : LOW);
  pinMode(9, OUTPUT);
  digitalWrite(9, hw_value_io_9 ? HIGH : LOW);
  pinMode(10, OUTPUT);
  digitalWrite(10, hw_value_io_10 ? HIGH : LOW);
  pinMode(11, OUTPUT);
  digitalWrite(11, hw_value_io_11 ? HIGH : LOW);
  pinMode(12, OUTPUT);
  digitalWrite(12, hw_value_io_12 ? HIGH : LOW);
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_13 ? HIGH : LOW);
  pinMode(44, OUTPUT);
  digitalWrite(44, hw_value_io_14 ? HIGH : LOW);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 5UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (analogRead(A3) * 1.0 + 0.0);
  }
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 5UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = (analogRead(A4) * 1.0 + 0.0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_io_7, &hw_value_io_8, &hw_value_io_9, &hw_value_io_10, &hw_value_io_11, &hw_value_io_12, &hw_value_io_13, &hw_value_io_14);
  if (isfinite((double)hw_value_io_2) && hw_value_io_2 >= 0 && hw_value_io_2 <= 180) {
    int command = (int)hw_value_io_2;
    if (command != hw_servo_previous_io_2) {
      hw_servo_io_2.write(command); hw_servo_previous_io_2 = command;
    }
  }
  if (isfinite((double)hw_value_io_3) && hw_value_io_3 >= 0 && hw_value_io_3 <= 180) {
    int command = (int)hw_value_io_3;
    if (command != hw_servo_previous_io_3) {
      hw_servo_io_3.write(command); hw_servo_previous_io_3 = command;
    }
  }
  if (isfinite((double)hw_value_io_4) && hw_value_io_4 >= 0 && hw_value_io_4 <= 180) {
    int command = (int)hw_value_io_4;
    if (command != hw_servo_previous_io_4) {
      hw_servo_io_4.write(command); hw_servo_previous_io_4 = command;
    }
  }
  digitalWrite(5, hw_value_io_5 ? HIGH : LOW);
  digitalWrite(6, hw_value_io_6 ? HIGH : LOW);
  digitalWrite(7, hw_value_io_7 ? HIGH : LOW);
  digitalWrite(8, hw_value_io_8 ? HIGH : LOW);
  digitalWrite(9, hw_value_io_9 ? HIGH : LOW);
  digitalWrite(10, hw_value_io_10 ? HIGH : LOW);
  digitalWrite(11, hw_value_io_11 ? HIGH : LOW);
  digitalWrite(12, hw_value_io_12 ? HIGH : LOW);
  digitalWrite(13, hw_value_io_13 ? HIGH : LOW);
  digitalWrite(44, hw_value_io_14 ? HIGH : LOW);
}

