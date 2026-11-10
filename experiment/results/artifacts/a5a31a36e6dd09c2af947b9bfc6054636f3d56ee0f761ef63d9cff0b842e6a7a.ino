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
extern int32_t sysml_state_mode;
/* Global Variable define of  */
extern int32_t sysml_state_stepIndex;
/* Global Variable define of  */
extern int32_t sysml_state_phaseMs;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool* io_0, bool* io_1, bool* io_2, double* io_3, double* io_4);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_mode = 0;
/* Global Variable define of  */
int32_t sysml_state_stepIndex = 0;
/* Global Variable define of  */
int32_t sysml_state_phaseMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_mode = 0;
    sysml_state_stepIndex = 0;
    sysml_state_phaseMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool* io_0, bool* io_1, bool* io_2, double* io_3, double* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    int32_t sysml_conditional_2 = 0;
    int32_t sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    int32_t sysml_conditional_5 = 0;
    int32_t sysml_conditional_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_conditional_8 = 0;
    bool sysml_conditional_9 = 0;
    bool sysml_conditional_10 = 0;
    double sysml_conditional_11 = 0;
    double sysml_conditional_12 = 0;
    double sysml_conditional_13 = 0;
    double sysml_conditional_14 = 0;
    double sysml_conditional_15 = 0;
    sysml_lazy_0 = sysml_state_mode == 0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = sysml_state_phaseMs >= 2000;
    }
    if (sysml_lazy_0) {
        sysml_state_mode = 1;
        sysml_state_phaseMs = 0;
        sysml_state_stepIndex = 0;
    }
    sysml_lazy_1 = sysml_state_mode == 1;
    if (sysml_lazy_1) {
        sysml_lazy_1 = sysml_state_phaseMs >= 2000;
    }
    if (sysml_lazy_1) {
        sysml_state_phaseMs = 0;
        if (sysml_state_stepIndex < 5) {
            sysml_conditional_2 = sysml_state_stepIndex + 1;
        }
        else {
            sysml_conditional_2 = 5;
        }
        sysml_state_stepIndex = sysml_conditional_2;
        if (sysml_state_stepIndex == 5) {
            sysml_conditional_3 = 2;
        }
        else {
            sysml_conditional_3 = 1;
        }
        sysml_state_mode = sysml_conditional_3;
    }
    sysml_lazy_4 = sysml_state_mode == 2;
    if (sysml_lazy_4) {
        sysml_lazy_4 = sysml_state_phaseMs >= 2000;
    }
    if (sysml_lazy_4) {
        sysml_state_phaseMs = 0;
        if (sysml_state_stepIndex < 5) {
            sysml_conditional_5 = sysml_state_stepIndex + 1;
        }
        else {
            sysml_conditional_5 = 5;
        }
        sysml_state_stepIndex = sysml_conditional_5;
        if (sysml_state_stepIndex == 5) {
            sysml_conditional_6 = 3;
        }
        else {
            sysml_conditional_6 = 2;
        }
        sysml_state_mode = sysml_conditional_6;
    }
    sysml_lazy_7 = sysml_state_mode == 3;
    if (sysml_lazy_7) {
        sysml_lazy_7 = sysml_state_phaseMs >= 2000;
    }
    if (sysml_lazy_7) {
        sysml_state_mode = 1;
        sysml_state_phaseMs = 0;
        sysml_state_stepIndex = 0;
    }
    bool sysml_value_0 = 0;
    if (sysml_state_mode == 1) {
        sysml_conditional_8 = true;
    }
    else {
        sysml_conditional_8 = false;
    }
    sysml_value_0 = sysml_conditional_8;
    bool sysml_value_1 = 0;
    if (sysml_state_mode == 2) {
        sysml_conditional_9 = true;
    }
    else {
        sysml_conditional_9 = false;
    }
    sysml_value_1 = sysml_conditional_9;
    bool sysml_value_2 = 0;
    if (sysml_state_mode == 3) {
        sysml_conditional_10 = true;
    }
    else {
        sysml_conditional_10 = false;
    }
    sysml_value_2 = sysml_conditional_10;
    double sysml_value_3 = 0;
    if (sysml_state_mode == 0) {
        sysml_conditional_11 = 0.0;
    }
    else {
        if (sysml_state_mode == 1) {
            sysml_conditional_12 = ((sysml_state_stepIndex + 0) * 30) + 0;
        }
        else {
            sysml_conditional_12 = 180.0;
        }
        sysml_conditional_11 = sysml_conditional_12;
    }
    sysml_value_3 = sysml_conditional_11;
    double sysml_value_4 = 0;
    if (sysml_state_mode == 0) {
        sysml_conditional_13 = 180.0;
    }
    else {
        if (sysml_state_mode == 2) {
            sysml_conditional_14 = 180.0 - ((sysml_state_stepIndex + 0) * 30);
        }
        else {
            if (sysml_state_mode == 1) {
                sysml_conditional_15 = 180.0;
            }
            else {
                sysml_conditional_15 = 0.0;
            }
            sysml_conditional_14 = sysml_conditional_15;
        }
        sysml_conditional_13 = sysml_conditional_14;
    }
    sysml_value_4 = sysml_conditional_13;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
    *io_4 = sysml_value_4;
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
Servo hw_servo_io_3;
static int hw_servo_previous_io_3 = -1;
static double hw_value_io_4 = 180;
Servo hw_servo_io_4;
static int hw_servo_previous_io_4 = -1;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, OUTPUT);
  digitalWrite(2, hw_value_io_0 ? HIGH : LOW);
  pinMode(3, OUTPUT);
  digitalWrite(3, hw_value_io_1 ? HIGH : LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, hw_value_io_2 ? HIGH : LOW);
  hw_servo_io_3.attach(5);
  if (isfinite((double)hw_value_io_3) && hw_value_io_3 >= 0 && hw_value_io_3 <= 180) {
    int command = (int)hw_value_io_3;
    if (command != hw_servo_previous_io_3) {
      hw_servo_io_3.write(command); hw_servo_previous_io_3 = command;
    }
  }
  hw_servo_io_4.attach(6);
  if (isfinite((double)hw_value_io_4) && hw_value_io_4 >= 0 && hw_value_io_4 <= 180) {
    int command = (int)hw_value_io_4;
    if (command != hw_servo_previous_io_4) {
      hw_servo_io_4.write(command); hw_servo_previous_io_4 = command;
    }
  }
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(&hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4);
  digitalWrite(2, hw_value_io_0 ? HIGH : LOW);
  digitalWrite(3, hw_value_io_1 ? HIGH : LOW);
  digitalWrite(4, hw_value_io_2 ? HIGH : LOW);
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
}
