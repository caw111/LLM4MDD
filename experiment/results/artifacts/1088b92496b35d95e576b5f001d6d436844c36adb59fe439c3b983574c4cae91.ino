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
void ProgramPage1(double* io_0, double* io_1, double* io_2, double* io_3, double* io_4, double* io_5, bool* io_6_segment_0, bool* io_6_segment_1, bool* io_6_segment_2, bool* io_6_segment_3, bool* io_6_segment_4, bool* io_6_segment_5, bool* io_6_segment_6, bool* io_6_segment_7);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double* io_0, double* io_1, double* io_2, double* io_3, double* io_4, double* io_5, bool* io_6_segment_0, bool* io_6_segment_1, bool* io_6_segment_2, bool* io_6_segment_3, bool* io_6_segment_4, bool* io_6_segment_5, bool* io_6_segment_6, bool* io_6_segment_7) {
    unsigned phase=millis()/2000UL%6;
    bool red=phase>=1 && phase<=3;
    bool green=phase>=2 && phase<=4;
    bool blue=phase>=3;
    *io_0=red ? 180 : 0;
    *io_1=green ? 180 : 0;
    *io_2=blue ? 180 : 0;
    *io_3=red ? 255 : 0;
    *io_4=green ? 255 : 0;
    *io_5=blue ? 255 : 0;
    int count=((int)red+(int)green+(int)blue)*8/3;
    *io_6_segment_0=count>0;
    *io_6_segment_1=count>1;
    *io_6_segment_2=count>2;
    *io_6_segment_3=count>3;
    *io_6_segment_4=count>4;
    *io_6_segment_5=count>5;
    *io_6_segment_6=count>6;
    *io_6_segment_7=count>7;

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
static double hw_value_io_2 = 0;
Servo hw_servo_io_2;
static int hw_servo_previous_io_2 = -1;
static double hw_value_io_3 = 0;
static double hw_value_io_4 = 0;
static double hw_value_io_5 = 0;
static bool hw_value_io_6_segment_0 = false;
static bool hw_value_io_6_segment_1 = false;
static bool hw_value_io_6_segment_2 = false;
static bool hw_value_io_6_segment_3 = false;
static bool hw_value_io_6_segment_4 = false;
static bool hw_value_io_6_segment_5 = false;
static bool hw_value_io_6_segment_6 = false;
static bool hw_value_io_6_segment_7 = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  hw_servo_io_0.attach(5);
  if (isfinite((double)hw_value_io_0) && hw_value_io_0 >= 0 && hw_value_io_0 <= 180) {
    int command = (int)hw_value_io_0;
    if (command != hw_servo_previous_io_0) {
      hw_servo_io_0.write(command); hw_servo_previous_io_0 = command;
    }
  }
  hw_servo_io_1.attach(6);
  if (isfinite((double)hw_value_io_1) && hw_value_io_1 >= 0 && hw_value_io_1 <= 180) {
    int command = (int)hw_value_io_1;
    if (command != hw_servo_previous_io_1) {
      hw_servo_io_1.write(command); hw_servo_previous_io_1 = command;
    }
  }
  hw_servo_io_2.attach(7);
  if (isfinite((double)hw_value_io_2) && hw_value_io_2 >= 0 && hw_value_io_2 <= 180) {
    int command = (int)hw_value_io_2;
    if (command != hw_servo_previous_io_2) {
      hw_servo_io_2.write(command); hw_servo_previous_io_2 = command;
    }
  }
  pinMode(2, OUTPUT);
  analogWrite(2, (int)constrain((hw_value_io_3 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(3, OUTPUT);
  analogWrite(3, (int)constrain((hw_value_io_4 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(4, OUTPUT);
  analogWrite(4, (int)constrain((hw_value_io_5 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(8, OUTPUT);
  digitalWrite(8, hw_value_io_6_segment_0 ? HIGH : LOW);
  pinMode(9, OUTPUT);
  digitalWrite(9, hw_value_io_6_segment_1 ? HIGH : LOW);
  pinMode(10, OUTPUT);
  digitalWrite(10, hw_value_io_6_segment_2 ? HIGH : LOW);
  pinMode(11, OUTPUT);
  digitalWrite(11, hw_value_io_6_segment_3 ? HIGH : LOW);
  pinMode(12, OUTPUT);
  digitalWrite(12, hw_value_io_6_segment_4 ? HIGH : LOW);
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_6_segment_5 ? HIGH : LOW);
  pinMode(44, OUTPUT);
  digitalWrite(44, hw_value_io_6_segment_6 ? HIGH : LOW);
  pinMode(45, OUTPUT);
  digitalWrite(45, hw_value_io_6_segment_7 ? HIGH : LOW);
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
  ProgramPage1(&hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6_segment_0, &hw_value_io_6_segment_1, &hw_value_io_6_segment_2, &hw_value_io_6_segment_3, &hw_value_io_6_segment_4, &hw_value_io_6_segment_5, &hw_value_io_6_segment_6, &hw_value_io_6_segment_7);
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
  if (isfinite((double)hw_value_io_2) && hw_value_io_2 >= 0 && hw_value_io_2 <= 180) {
    int command = (int)hw_value_io_2;
    if (command != hw_servo_previous_io_2) {
      hw_servo_io_2.write(command); hw_servo_previous_io_2 = command;
    }
  }
  analogWrite(2, (int)constrain((hw_value_io_3 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(3, (int)constrain((hw_value_io_4 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(4, (int)constrain((hw_value_io_5 * 1.0 + 0.0), 0.0, 255.0));
  digitalWrite(8, hw_value_io_6_segment_0 ? HIGH : LOW);
  digitalWrite(9, hw_value_io_6_segment_1 ? HIGH : LOW);
  digitalWrite(10, hw_value_io_6_segment_2 ? HIGH : LOW);
  digitalWrite(11, hw_value_io_6_segment_3 ? HIGH : LOW);
  digitalWrite(12, hw_value_io_6_segment_4 ? HIGH : LOW);
  digitalWrite(13, hw_value_io_6_segment_5 ? HIGH : LOW);
  digitalWrite(44, hw_value_io_6_segment_6 ? HIGH : LOW);
  digitalWrite(45, hw_value_io_6_segment_7 ? HIGH : LOW);
}
