#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_21\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
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
extern int32_t sysml_state_stepIndex;
/* Global Variable define of  */
extern int32_t sysml_state_remainingMs;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_3, double* io_0, double* io_1, double* io_2);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_stepIndex = 0;
/* Global Variable define of  */
int32_t sysml_state_remainingMs = 2000;
/* Init function of CompositeActor named "ProgramPage1" */
#line 190 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_21\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 215 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_21\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 33 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_21\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_stepIndex = 0;
    sysml_state_remainingMs = 2000;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_3, double* io_0, double* io_1, double* io_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int32_t sysml_conditional_0 = 0;
    double sysml_conditional_1 = 0;
    double sysml_conditional_2 = 0;
    double sysml_conditional_3 = 0;
    double sysml_conditional_4 = 0;
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
    sysml_state_remainingMs = (sysml_state_remainingMs - io_3);
    if ((sysml_state_remainingMs <= 0)) {
        if (sysml_state_stepIndex == 5) {
            sysml_conditional_0 = 0;
        }
        else {
            sysml_conditional_0 = sysml_state_stepIndex + 1;
        }
        sysml_state_stepIndex = sysml_conditional_0;
        sysml_state_remainingMs = 2000;
    }
    double sysml_value_0 = 0;
    if (sysml_state_stepIndex == 0) {
        sysml_conditional_1 = 0.0;
    }
    else {
        if (sysml_state_stepIndex == 1) {
            sysml_conditional_2 = 0.0;
        }
        else {
            if (sysml_state_stepIndex == 2) {
                sysml_conditional_3 = 45.0;
            }
            else {
                if (sysml_state_stepIndex == 3) {
                    sysml_conditional_4 = 90.0;
                }
                else {
                    if (sysml_state_stepIndex == 4) {
                        sysml_conditional_5 = 135.0;
                    }
                    else {
                        sysml_conditional_5 = 180.0;
                    }
                    sysml_conditional_4 = sysml_conditional_5;
                }
                sysml_conditional_3 = sysml_conditional_4;
            }
            sysml_conditional_2 = sysml_conditional_3;
        }
        sysml_conditional_1 = sysml_conditional_2;
    }
    sysml_value_0 = sysml_conditional_1;
    double sysml_value_1 = 0;
    if (sysml_state_stepIndex == 0) {
        sysml_conditional_6 = 0.0;
    }
    else {
        if (sysml_state_stepIndex == 1) {
            sysml_conditional_7 = 45.0;
        }
        else {
            if (sysml_state_stepIndex == 2) {
                sysml_conditional_8 = 90.0;
            }
            else {
                if (sysml_state_stepIndex == 3) {
                    sysml_conditional_9 = 135.0;
                }
                else {
                    if (sysml_state_stepIndex == 4) {
                        sysml_conditional_10 = 180.0;
                    }
                    else {
                        sysml_conditional_10 = 0.0;
                    }
                    sysml_conditional_9 = sysml_conditional_10;
                }
                sysml_conditional_8 = sysml_conditional_9;
            }
            sysml_conditional_7 = sysml_conditional_8;
        }
        sysml_conditional_6 = sysml_conditional_7;
    }
    sysml_value_1 = sysml_conditional_6;
    double sysml_value_2 = 0;
    if (sysml_state_stepIndex == 0) {
        sysml_conditional_11 = 0.0;
    }
    else {
        if (sysml_state_stepIndex == 1) {
            sysml_conditional_12 = 90.0;
        }
        else {
            if (sysml_state_stepIndex == 2) {
                sysml_conditional_13 = 135.0;
            }
            else {
                if (sysml_state_stepIndex == 3) {
                    sysml_conditional_14 = 180.0;
                }
                else {
                    if (sysml_state_stepIndex == 4) {
                        sysml_conditional_15 = 0.0;
                    }
                    else {
                        sysml_conditional_15 = 45.0;
                    }
                    sysml_conditional_14 = sysml_conditional_15;
                }
                sysml_conditional_13 = sysml_conditional_14;
            }
            sysml_conditional_12 = sysml_conditional_13;
        }
        sysml_conditional_11 = sysml_conditional_12;
    }
    sysml_value_2 = sysml_conditional_11;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
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
static int32_t hw_value_io_3 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  hw_servo_io_0.attach(2);
  if (isfinite((double)hw_value_io_0) && hw_value_io_0 >= 0 && hw_value_io_0 <= 180) {
    int command = (int)hw_value_io_0;
    if (command != hw_servo_previous_io_0) {
      hw_servo_io_0.write(command); hw_servo_previous_io_0 = command;
    }
  }
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
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_3 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_3, &hw_value_io_0, &hw_value_io_1, &hw_value_io_2);
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
}

