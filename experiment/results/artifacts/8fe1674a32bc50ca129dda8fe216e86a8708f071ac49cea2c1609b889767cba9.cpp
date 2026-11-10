#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_109\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
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
void ProgramPage1(bool* io_0, bool* io_1, bool* io_2, double* io_3, double* io_4, double* io_5, double* io_6, double* io_7);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Init function of CompositeActor named "ProgramPage1" */
#line 79 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_109\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void setup();
#line 109 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_109\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void loop();
#line 25 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_109\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool* io_0, bool* io_1, bool* io_2, double* io_3, double* io_4, double* io_5, double* io_6, double* io_7) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_value_0 = 0;
    sysml_value_0 = false;
    bool sysml_value_1 = 0;
    sysml_value_1 = false;
    bool sysml_value_2 = 0;
    sysml_value_2 = false;
    double sysml_value_3 = 0;
    sysml_value_3 = 1.0;
    double sysml_value_4 = 0;
    sysml_value_4 = 0.0;
    double sysml_value_5 = 0;
    sysml_value_5 = 0.0;
    double sysml_value_6 = 0;
    sysml_value_6 = 0.0;
    double sysml_value_7 = 0;
    sysml_value_7 = 0.0;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
    *io_4 = sysml_value_4;
    *io_5 = sysml_value_5;
    *io_6 = sysml_value_6;
    *io_7 = sysml_value_7;
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
static bool hw_value_io_2 = false;
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
  pinMode(2, OUTPUT);
  digitalWrite(2, hw_value_io_0 ? HIGH : LOW);
  pinMode(3, OUTPUT);
  digitalWrite(3, hw_value_io_1 ? HIGH : LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, hw_value_io_2 ? HIGH : LOW);
  pinMode(5, OUTPUT);
  analogWrite(5, (int)constrain((hw_value_io_3 * 255 + 0), 0.0, 255.0));
  pinMode(6, OUTPUT);
  analogWrite(6, (int)constrain((hw_value_io_4 * 255 + 0), 0.0, 255.0));
  pinMode(7, OUTPUT);
  analogWrite(7, (int)constrain((hw_value_io_5 * 255 + 0), 0.0, 255.0));
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
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(&hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_io_7);
  digitalWrite(2, hw_value_io_0 ? HIGH : LOW);
  digitalWrite(3, hw_value_io_1 ? HIGH : LOW);
  digitalWrite(4, hw_value_io_2 ? HIGH : LOW);
  analogWrite(5, (int)constrain((hw_value_io_3 * 255 + 0), 0.0, 255.0));
  analogWrite(6, (int)constrain((hw_value_io_4 * 255 + 0), 0.0, 255.0));
  analogWrite(7, (int)constrain((hw_value_io_5 * 255 + 0), 0.0, 255.0));
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

