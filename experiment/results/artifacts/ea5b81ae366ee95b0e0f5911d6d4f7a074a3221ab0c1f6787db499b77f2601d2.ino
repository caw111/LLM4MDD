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
extern int32_t sysml_state_servoStep;
/* Global Variable define of  */
extern bool sysml_state_prevPressed;
/* Global Variable define of  */
extern int32_t sysml_state_stateHoldMs;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, double* io_1, double* io_2, double* io_3);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_servoStep = 1;
/* Global Variable define of  */
bool sysml_state_prevPressed = false;
/* Global Variable define of  */
int32_t sysml_state_stateHoldMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_servoStep = 1;
    sysml_state_prevPressed = false;
    sysml_state_stateHoldMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, double* io_1, double* io_2, double* io_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int32_t sysml_conditional_1 = 0;
    int32_t sysml_conditional_2 = 0;
    int32_t sysml_conditional_3 = 0;
    int32_t sysml_conditional_4 = 0;
    double sysml_conditional_5 = 0;
    double sysml_conditional_6 = 0;
    double sysml_conditional_7 = 0;
    double sysml_conditional_8 = 0;
    double sysml_conditional_9 = 0;
    double sysml_conditional_10 = 0;
    double sysml_conditional_11 = 0;
    double sysml_conditional_12 = 0;
    double sysml_conditional_13 = 0;
    double sysml_conditional_14 = 0;
    double sysml_conditional_15 = 0;
    double sysml_conditional_16 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_prevPressed;
    }
    if (sysml_lazy_0) {
        if (sysml_state_servoStep == 1) {
            sysml_conditional_1 = 2;
        }
        else {
            if (sysml_state_servoStep == 2) {
                sysml_conditional_2 = 3;
            }
            else {
                if (sysml_state_servoStep == 3) {
                    sysml_conditional_3 = 4;
                }
                else {
                    if (sysml_state_servoStep == 4) {
                        sysml_conditional_4 = 5;
                    }
                    else {
                        sysml_conditional_4 = 1;
                    }
                    sysml_conditional_3 = sysml_conditional_4;
                }
                sysml_conditional_2 = sysml_conditional_3;
            }
            sysml_conditional_1 = sysml_conditional_2;
        }
        sysml_state_servoStep = sysml_conditional_1;
        sysml_state_stateHoldMs = 0;
    }
    sysml_state_prevPressed = io_0;
    double sysml_value_0 = 0;
    if (sysml_state_servoStep == 1) {
        sysml_conditional_5 = 0.0;
    }
    else {
        if (sysml_state_servoStep == 2) {
            sysml_conditional_6 = 90.0;
        }
        else {
            if (sysml_state_servoStep == 3) {
                sysml_conditional_7 = 180.0;
            }
            else {
                if (sysml_state_servoStep == 4) {
                    sysml_conditional_8 = 45.0;
                }
                else {
                    sysml_conditional_8 = 0.0;
                }
                sysml_conditional_7 = sysml_conditional_8;
            }
            sysml_conditional_6 = sysml_conditional_7;
        }
        sysml_conditional_5 = sysml_conditional_6;
    }
    sysml_value_0 = sysml_conditional_5;
    double sysml_value_1 = 0;
    if (sysml_state_servoStep == 1) {
        sysml_conditional_9 = 0.0;
    }
    else {
        if (sysml_state_servoStep == 2) {
            sysml_conditional_10 = 45.0;
        }
        else {
            if (sysml_state_servoStep == 3) {
                sysml_conditional_11 = 90.0;
            }
            else {
                if (sysml_state_servoStep == 4) {
                    sysml_conditional_12 = 135.0;
                }
                else {
                    sysml_conditional_12 = 0.0;
                }
                sysml_conditional_11 = sysml_conditional_12;
            }
            sysml_conditional_10 = sysml_conditional_11;
        }
        sysml_conditional_9 = sysml_conditional_10;
    }
    sysml_value_1 = sysml_conditional_9;
    double sysml_value_2 = 0;
    if (sysml_state_servoStep == 1) {
        sysml_conditional_13 = 0.0;
    }
    else {
        if (sysml_state_servoStep == 2) {
            sysml_conditional_14 = 135.0;
        }
        else {
            if (sysml_state_servoStep == 3) {
                sysml_conditional_15 = 0.0;
            }
            else {
                if (sysml_state_servoStep == 4) {
                    sysml_conditional_16 = 90.0;
                }
                else {
                    sysml_conditional_16 = 0.0;
                }
                sysml_conditional_15 = sysml_conditional_16;
            }
            sysml_conditional_14 = sysml_conditional_15;
        }
        sysml_conditional_13 = sysml_conditional_14;
    }
    sysml_value_2 = sysml_conditional_13;
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
    *io_3 = sysml_value_2;
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
static uint32_t hw_sampled_io_0 = 0;
static bool hw_attempted_io_0 = false;
static double hw_value_io_1 = 0;
Servo hw_servo_io_1;
static int hw_servo_previous_io_1 = -1;
static double hw_value_io_2 = 0;
Servo hw_servo_io_2;
static int hw_servo_previous_io_2 = -1;
static double hw_value_io_3 = 0;
Servo hw_servo_io_3;
static int hw_servo_previous_io_3 = -1;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, INPUT);
  hw_servo_io_1.attach(3);
  if (isfinite((double)hw_value_io_1) && hw_value_io_1 >= 0 && hw_value_io_1 <= 180) {
    int command = (int)hw_value_io_1;
    if (command != hw_servo_previous_io_1) {
      hw_servo_io_1.write(command); hw_servo_previous_io_1 = command;
    }
  }
  hw_servo_io_2.attach(4);
  if (isfinite((double)hw_value_io_2) && hw_value_io_2 >= 0 && hw_value_io_2 <= 180) {
    int command = (int)hw_value_io_2;
    if (command != hw_servo_previous_io_2) {
      hw_servo_io_2.write(command); hw_servo_previous_io_2 = command;
    }
  }
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
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 5UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (digitalRead(2) == HIGH);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3);
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
  if (isfinite((double)hw_value_io_3) && hw_value_io_3 >= 0 && hw_value_io_3 <= 180) {
    int command = (int)hw_value_io_3;
    if (command != hw_servo_previous_io_3) {
      hw_servo_io_3.write(command); hw_servo_previous_io_3 = command;
    }
  }
}
