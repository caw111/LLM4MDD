#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision10\\cases\\embedbench_48\\arduino\\sketch\\sketch.ino"
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
void ProgramPage1(double io_9, double io_10, double* io_0, double* io_1, bool* io_2, double* io_3, double* io_4, bool* io_5, double* io_6, double* io_7, bool* io_8, double* io_11, double* io_12, double* io_13);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Init function of CompositeActor named "ProgramPage1" */
#line 104 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision10\\cases\\embedbench_48\\arduino\\sketch\\sketch.ino"
void setup();
#line 149 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision10\\cases\\embedbench_48\\arduino\\sketch\\sketch.ino"
void loop();
#line 25 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision10\\cases\\embedbench_48\\arduino\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_9, double io_10, double* io_0, double* io_1, bool* io_2, double* io_3, double* io_4, bool* io_5, double* io_6, double* io_7, bool* io_8, double* io_11, double* io_12, double* io_13) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_value_0 = 0;
    sysml_value_0 = ((double)((io_9 * 255.0)) / 1023.0);
    double sysml_value_1 = 0;
    sysml_value_1 = ((double)((io_10 * 255.0)) / 1023.0);
    bool sysml_value_2 = 0;
    sysml_value_2 = false;
    double sysml_value_3 = 0;
    sysml_value_3 = ((double)((io_9 * 255.0)) / 1023.0);
    double sysml_value_4 = 0;
    sysml_value_4 = ((double)((io_10 * 255.0)) / 1023.0);
    bool sysml_value_5 = 0;
    sysml_value_5 = false;
    double sysml_value_6 = 0;
    sysml_value_6 = ((double)((io_9 * 255.0)) / 1023.0);
    double sysml_value_7 = 0;
    sysml_value_7 = ((double)((io_10 * 255.0)) / 1023.0);
    bool sysml_value_8 = 0;
    sysml_value_8 = false;
    double sysml_value_9 = 0;
    sysml_value_9 = ((double)((io_9 * 180.0)) / 1023.0);
    double sysml_value_10 = 0;
    sysml_value_10 = ((double)((io_10 * 180.0)) / 1023.0);
    double sysml_value_11 = 0;
    sysml_value_11 = 0.0;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
    *io_4 = sysml_value_4;
    *io_5 = sysml_value_5;
    *io_6 = sysml_value_6;
    *io_7 = sysml_value_7;
    *io_8 = sysml_value_8;
    *io_11 = sysml_value_9;
    *io_12 = sysml_value_10;
    *io_13 = sysml_value_11;
    *io_13=millis()/2000UL%2UL ? 180 : 0;
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
static double hw_value_io_1 = 0;
static bool hw_value_io_2 = false;
static double hw_value_io_3 = 0;
static double hw_value_io_4 = 0;
static bool hw_value_io_5 = false;
static double hw_value_io_6 = 0;
static double hw_value_io_7 = 0;
static bool hw_value_io_8 = false;
static double hw_value_io_9 = 0;
static uint32_t hw_sampled_io_9 = 0;
static bool hw_attempted_io_9 = false;
static double hw_value_io_10 = 0;
static uint32_t hw_sampled_io_10 = 0;
static bool hw_attempted_io_10 = false;
static double hw_value_io_11 = 0;
Servo hw_servo_io_11;
static int hw_servo_previous_io_11 = -1;
static double hw_value_io_12 = 0;
Servo hw_servo_io_12;
static int hw_servo_previous_io_12 = -1;
static double hw_value_io_13 = 0;
Servo hw_servo_io_13;
static int hw_servo_previous_io_13 = -1;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, OUTPUT);
  analogWrite(2, (int)constrain((hw_value_io_0 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(3, OUTPUT);
  analogWrite(3, (int)constrain((hw_value_io_1 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(4, OUTPUT);
  digitalWrite(4, hw_value_io_2 ? HIGH : LOW);
  pinMode(5, OUTPUT);
  analogWrite(5, (int)constrain((hw_value_io_3 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(6, OUTPUT);
  analogWrite(6, (int)constrain((hw_value_io_4 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(7, OUTPUT);
  digitalWrite(7, hw_value_io_5 ? HIGH : LOW);
  pinMode(8, OUTPUT);
  analogWrite(8, (int)constrain((hw_value_io_6 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(9, OUTPUT);
  analogWrite(9, (int)constrain((hw_value_io_7 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(10, OUTPUT);
  digitalWrite(10, hw_value_io_8 ? HIGH : LOW);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  hw_servo_io_11.attach(11);
  if (isfinite((double)hw_value_io_11) && hw_value_io_11 >= 0 && hw_value_io_11 <= 180) {
    int command = (int)hw_value_io_11;
    if (command != hw_servo_previous_io_11) {
      hw_servo_io_11.write(command); hw_servo_previous_io_11 = command;
    }
  }
  hw_servo_io_12.attach(12);
  if (isfinite((double)hw_value_io_12) && hw_value_io_12 >= 0 && hw_value_io_12 <= 180) {
    int command = (int)hw_value_io_12;
    if (command != hw_servo_previous_io_12) {
      hw_servo_io_12.write(command); hw_servo_previous_io_12 = command;
    }
  }
  hw_servo_io_13.attach(13);
  if (isfinite((double)hw_value_io_13) && hw_value_io_13 >= 0 && hw_value_io_13 <= 180) {
    int command = (int)hw_value_io_13;
    if (command != hw_servo_previous_io_13) {
      hw_servo_io_13.write(command); hw_servo_previous_io_13 = command;
    }
  }
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_9 || (uint32_t)(hw_now - hw_sampled_io_9) >= 10UL) {
    hw_attempted_io_9 = true; hw_sampled_io_9 = hw_now;
    hw_value_io_9 = (analogRead(A3) * 1.0 + 0.0);
  }
  if (!hw_attempted_io_10 || (uint32_t)(hw_now - hw_sampled_io_10) >= 10UL) {
    hw_attempted_io_10 = true; hw_sampled_io_10 = hw_now;
    hw_value_io_10 = (analogRead(A4) * 1.0 + 0.0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_9, hw_value_io_10, &hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_io_7, &hw_value_io_8, &hw_value_io_11, &hw_value_io_12, &hw_value_io_13);
  analogWrite(2, (int)constrain((hw_value_io_0 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(3, (int)constrain((hw_value_io_1 * 1.0 + 0.0), 0.0, 255.0));
  digitalWrite(4, hw_value_io_2 ? HIGH : LOW);
  analogWrite(5, (int)constrain((hw_value_io_3 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(6, (int)constrain((hw_value_io_4 * 1.0 + 0.0), 0.0, 255.0));
  digitalWrite(7, hw_value_io_5 ? HIGH : LOW);
  analogWrite(8, (int)constrain((hw_value_io_6 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(9, (int)constrain((hw_value_io_7 * 1.0 + 0.0), 0.0, 255.0));
  digitalWrite(10, hw_value_io_8 ? HIGH : LOW);
  if (isfinite((double)hw_value_io_11) && hw_value_io_11 >= 0 && hw_value_io_11 <= 180) {
    int command = (int)hw_value_io_11;
    if (command != hw_servo_previous_io_11) {
      hw_servo_io_11.write(command); hw_servo_previous_io_11 = command;
    }
  }
  if (isfinite((double)hw_value_io_12) && hw_value_io_12 >= 0 && hw_value_io_12 <= 180) {
    int command = (int)hw_value_io_12;
    if (command != hw_servo_previous_io_12) {
      hw_servo_io_12.write(command); hw_servo_previous_io_12 = command;
    }
  }
  if (isfinite((double)hw_value_io_13) && hw_value_io_13 >= 0 && hw_value_io_13 <= 180) {
    int command = (int)hw_value_io_13;
    if (command != hw_servo_previous_io_13) {
      hw_servo_io_13.write(command); hw_servo_previous_io_13 = command;
    }
  }
}

