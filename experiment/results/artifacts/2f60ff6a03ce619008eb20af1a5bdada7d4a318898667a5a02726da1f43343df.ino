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
extern double sysml_state_prevPot;
/* Global Variable define of  */
extern bool sysml_state_servoHold1;
/* Global Variable define of  */
extern bool sysml_state_servoHold2;
/* Global Variable define of  */
extern bool sysml_state_servoHold3;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_6, double* io_0, double* io_1, double* io_2, double* io_3, double* io_4, double* io_5, double* io_7, double* io_8, double* io_9);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_prevPot = 0.0;
/* Global Variable define of  */
bool sysml_state_servoHold1 = false;
/* Global Variable define of  */
bool sysml_state_servoHold2 = false;
/* Global Variable define of  */
bool sysml_state_servoHold3 = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_prevPot = 0.0;
    sysml_state_servoHold1 = false;
    sysml_state_servoHold2 = false;
    sysml_state_servoHold3 = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_6, double* io_0, double* io_1, double* io_2, double* io_3, double* io_4, double* io_5, double* io_7, double* io_8, double* io_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_conditional_0 = 0;
    bool sysml_lazy_1 = 0;
    double sysml_conditional_2 = 0;
    double sysml_conditional_3 = 0;
    double sysml_conditional_4 = 0;
    double sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    double sysml_conditional_7 = 0;
    double sysml_conditional_8 = 0;
    double sysml_conditional_9 = 0;
    sysml_state_prevPot = io_6;
    sysml_state_servoHold1 = false;
    sysml_state_servoHold2 = false;
    sysml_state_servoHold3 = false;
    double sysml_value_0 = 0;
    if (io_6 <= 341.0) {
        sysml_conditional_0 = ((double)(io_6 * 255.0)) / 341.0;
    }
    else {
        sysml_conditional_0 = 0.0;
    }
    sysml_value_0 = sysml_conditional_0;
    double sysml_value_1 = 0;
    sysml_lazy_1 = io_6 >= 342.0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = io_6 <= 682.0;
    }
    if (sysml_lazy_1) {
        sysml_conditional_2 = ((double)((io_6 - 342.0) * 255.0)) / 340.0;
    }
    else {
        if (io_6 < 342.0) {
            sysml_conditional_3 = 0.0;
        }
        else {
            sysml_conditional_3 = 0.0;
        }
        sysml_conditional_2 = sysml_conditional_3;
    }
    sysml_value_1 = sysml_conditional_2;
    double sysml_value_2 = 0;
    if (io_6 >= 683.0) {
        sysml_conditional_4 = ((double)((io_6 - 683.0) * 255.0)) / 340.0;
    }
    else {
        sysml_conditional_4 = 0.0;
    }
    sysml_value_2 = sysml_conditional_4;
    double sysml_value_3 = 0;
    if (io_6 <= 341.0) {
        sysml_conditional_5 = ((double)(io_6 * 255.0)) / 341.0;
    }
    else {
        sysml_conditional_5 = 0.0;
    }
    sysml_value_3 = sysml_conditional_5;
    double sysml_value_4 = 0;
    sysml_lazy_6 = io_6 >= 342.0;
    if (sysml_lazy_6) {
        sysml_lazy_6 = io_6 <= 682.0;
    }
    if (sysml_lazy_6) {
        sysml_conditional_7 = ((double)((io_6 - 342.0) * 255.0)) / 340.0;
    }
    else {
        if (io_6 < 342.0) {
            sysml_conditional_8 = 0.0;
        }
        else {
            sysml_conditional_8 = 0.0;
        }
        sysml_conditional_7 = sysml_conditional_8;
    }
    sysml_value_4 = sysml_conditional_7;
    double sysml_value_5 = 0;
    if (io_6 >= 683.0) {
        sysml_conditional_9 = ((double)((io_6 - 683.0) * 255.0)) / 340.0;
    }
    else {
        sysml_conditional_9 = 0.0;
    }
    sysml_value_5 = sysml_conditional_9;
    double sysml_value_6 = 0;
    sysml_value_6 = ((double)((io_6 * 180.0)) / 1023.0);
    double sysml_value_7 = 0;
    sysml_value_7 = ((double)((io_6 * 180.0)) / 1023.0);
    double sysml_value_8 = 0;
    sysml_value_8 = ((double)((io_6 * 180.0)) / 1023.0);
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
    *io_4 = sysml_value_4;
    *io_5 = sysml_value_5;
    *io_7 = sysml_value_6;
    *io_8 = sysml_value_7;
    *io_9 = sysml_value_8;
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
static double hw_value_io_2 = 0;
static double hw_value_io_3 = 0;
static double hw_value_io_4 = 0;
static double hw_value_io_5 = 0;
static double hw_value_io_6 = 0;
static uint32_t hw_sampled_io_6 = 0;
static bool hw_attempted_io_6 = false;
static double hw_value_io_7 = 0;
Servo hw_servo_io_7;
static int hw_servo_previous_io_7 = -1;
static double hw_value_io_8 = 0;
Servo hw_servo_io_8;
static int hw_servo_previous_io_8 = -1;
static double hw_value_io_9 = 0;
Servo hw_servo_io_9;
static int hw_servo_previous_io_9 = -1;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, OUTPUT);
  analogWrite(2, (int)constrain((hw_value_io_0 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(3, OUTPUT);
  analogWrite(3, (int)constrain((hw_value_io_1 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(4, OUTPUT);
  analogWrite(4, (int)constrain((hw_value_io_2 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(5, OUTPUT);
  analogWrite(5, (int)constrain((hw_value_io_3 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(6, OUTPUT);
  analogWrite(6, (int)constrain((hw_value_io_4 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(7, OUTPUT);
  analogWrite(7, (int)constrain((hw_value_io_5 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(A3, INPUT);
  hw_servo_io_7.attach(8);
  if (isfinite((double)hw_value_io_7) && hw_value_io_7 >= 0 && hw_value_io_7 <= 180) {
    int command = (int)hw_value_io_7;
    if (command != hw_servo_previous_io_7) {
      hw_servo_io_7.write(command); hw_servo_previous_io_7 = command;
    }
  }
  hw_servo_io_8.attach(9);
  if (isfinite((double)hw_value_io_8) && hw_value_io_8 >= 0 && hw_value_io_8 <= 180) {
    int command = (int)hw_value_io_8;
    if (command != hw_servo_previous_io_8) {
      hw_servo_io_8.write(command); hw_servo_previous_io_8 = command;
    }
  }
  hw_servo_io_9.attach(10);
  if (isfinite((double)hw_value_io_9) && hw_value_io_9 >= 0 && hw_value_io_9 <= 180) {
    int command = (int)hw_value_io_9;
    if (command != hw_servo_previous_io_9) {
      hw_servo_io_9.write(command); hw_servo_previous_io_9 = command;
    }
  }
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_6 || (uint32_t)(hw_now - hw_sampled_io_6) >= 5UL) {
    hw_attempted_io_6 = true; hw_sampled_io_6 = hw_now;
    hw_value_io_6 = (analogRead(A3) * 1.0 + 0.0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_6, &hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_7, &hw_value_io_8, &hw_value_io_9);
  analogWrite(2, (int)constrain((hw_value_io_0 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(3, (int)constrain((hw_value_io_1 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(4, (int)constrain((hw_value_io_2 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(5, (int)constrain((hw_value_io_3 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(6, (int)constrain((hw_value_io_4 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(7, (int)constrain((hw_value_io_5 * 1.0 + 0.0), 0.0, 255.0));
  if (isfinite((double)hw_value_io_7) && hw_value_io_7 >= 0 && hw_value_io_7 <= 180) {
    int command = (int)hw_value_io_7;
    if (command != hw_servo_previous_io_7) {
      hw_servo_io_7.write(command); hw_servo_previous_io_7 = command;
    }
  }
  if (isfinite((double)hw_value_io_8) && hw_value_io_8 >= 0 && hw_value_io_8 <= 180) {
    int command = (int)hw_value_io_8;
    if (command != hw_servo_previous_io_8) {
      hw_servo_io_8.write(command); hw_servo_previous_io_8 = command;
    }
  }
  if (isfinite((double)hw_value_io_9) && hw_value_io_9 >= 0 && hw_value_io_9 <= 180) {
    int command = (int)hw_value_io_9;
    if (command != hw_servo_previous_io_9) {
      hw_servo_io_9.write(command); hw_servo_previous_io_9 = command;
    }
  }
}
