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
extern int32_t sysml_state_phaseElapsedMs;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double* io_0, double* io_1, double* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6_segment_0, bool* io_6_segment_1, bool* io_6_segment_2, bool* io_6_segment_3, bool* io_6_segment_4, bool* io_6_segment_5, bool* io_6_segment_6, bool* io_6_segment_7);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_mode = 0;
/* Global Variable define of  */
int32_t sysml_state_phaseElapsedMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_mode = 0;
    sysml_state_phaseElapsedMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double* io_0, double* io_1, double* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6_segment_0, bool* io_6_segment_1, bool* io_6_segment_2, bool* io_6_segment_3, bool* io_6_segment_4, bool* io_6_segment_5, bool* io_6_segment_6, bool* io_6_segment_7) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_conditional_0 = 0;
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
    bool sysml_conditional_12 = 0;
    bool sysml_conditional_13 = 0;
    bool sysml_conditional_14 = 0;
    bool sysml_conditional_15 = 0;
    bool sysml_conditional_16 = 0;
    bool sysml_conditional_17 = 0;
    bool sysml_conditional_18 = 0;
    bool sysml_conditional_19 = 0;
    bool sysml_conditional_20 = 0;
    bool sysml_conditional_21 = 0;
    bool sysml_conditional_22 = 0;
    if ((sysml_state_mode == 0)) {
        sysml_state_mode = 1;
    }
    else {
        if ((sysml_state_mode == 1)) {
            sysml_state_mode = 2;
        }
        else {
            if ((sysml_state_mode == 2)) {
                sysml_state_mode = 3;
            }
            else {
                if ((sysml_state_mode == 3)) {
                    sysml_state_mode = 4;
                }
                else {
                    sysml_state_mode = 1;
                }
            }
        }
    }
    sysml_state_phaseElapsedMs = 0;
    double sysml_value_0 = 0;
    if (sysml_state_mode == 0) {
        sysml_conditional_0 = 0.0;
    }
    else {
        if (sysml_state_mode == 1) {
            sysml_conditional_1 = 60.0;
        }
        else {
            if (sysml_state_mode == 2) {
                sysml_conditional_2 = 0.0;
            }
            else {
                if (sysml_state_mode == 3) {
                    sysml_conditional_3 = 0.0;
                }
                else {
                    sysml_conditional_3 = 0.0;
                }
                sysml_conditional_2 = sysml_conditional_3;
            }
            sysml_conditional_1 = sysml_conditional_2;
        }
        sysml_conditional_0 = sysml_conditional_1;
    }
    sysml_value_0 = sysml_conditional_0;
    double sysml_value_1 = 0;
    if (sysml_state_mode == 0) {
        sysml_conditional_4 = 0.0;
    }
    else {
        if (sysml_state_mode == 1) {
            sysml_conditional_5 = 0.0;
        }
        else {
            if (sysml_state_mode == 2) {
                sysml_conditional_6 = 120.0;
            }
            else {
                if (sysml_state_mode == 3) {
                    sysml_conditional_7 = 0.0;
                }
                else {
                    sysml_conditional_7 = 0.0;
                }
                sysml_conditional_6 = sysml_conditional_7;
            }
            sysml_conditional_5 = sysml_conditional_6;
        }
        sysml_conditional_4 = sysml_conditional_5;
    }
    sysml_value_1 = sysml_conditional_4;
    double sysml_value_2 = 0;
    if (sysml_state_mode == 0) {
        sysml_conditional_8 = 0.0;
    }
    else {
        if (sysml_state_mode == 1) {
            sysml_conditional_9 = 0.0;
        }
        else {
            if (sysml_state_mode == 2) {
                sysml_conditional_10 = 0.0;
            }
            else {
                if (sysml_state_mode == 3) {
                    sysml_conditional_11 = 180.0;
                }
                else {
                    sysml_conditional_11 = 0.0;
                }
                sysml_conditional_10 = sysml_conditional_11;
            }
            sysml_conditional_9 = sysml_conditional_10;
        }
        sysml_conditional_8 = sysml_conditional_9;
    }
    sysml_value_2 = sysml_conditional_8;
    bool sysml_value_3 = 0;
    if (sysml_state_mode == 1) {
        sysml_conditional_12 = true;
    }
    else {
        sysml_conditional_12 = false;
    }
    sysml_value_3 = sysml_conditional_12;
    bool sysml_value_4 = 0;
    if (sysml_state_mode == 2) {
        sysml_conditional_13 = true;
    }
    else {
        sysml_conditional_13 = false;
    }
    sysml_value_4 = sysml_conditional_13;
    bool sysml_value_5 = 0;
    if (sysml_state_mode == 3) {
        sysml_conditional_14 = true;
    }
    else {
        sysml_conditional_14 = false;
    }
    sysml_value_5 = sysml_conditional_14;
    bool sysml_value_6 = 0;
    if (sysml_state_mode == 1) {
        sysml_conditional_15 = true;
    }
    else {
        sysml_conditional_15 = false;
    }
    sysml_value_6 = sysml_conditional_15;
    bool sysml_value_7 = 0;
    if (sysml_state_mode == 1) {
        sysml_conditional_16 = true;
    }
    else {
        sysml_conditional_16 = false;
    }
    sysml_value_7 = sysml_conditional_16;
    bool sysml_value_8 = 0;
    if (sysml_state_mode == 2) {
        sysml_conditional_17 = true;
    }
    else {
        sysml_conditional_17 = false;
    }
    sysml_value_8 = sysml_conditional_17;
    bool sysml_value_9 = 0;
    if (sysml_state_mode == 2) {
        sysml_conditional_18 = true;
    }
    else {
        sysml_conditional_18 = false;
    }
    sysml_value_9 = sysml_conditional_18;
    bool sysml_value_10 = 0;
    if (sysml_state_mode == 2) {
        sysml_conditional_19 = true;
    }
    else {
        sysml_conditional_19 = false;
    }
    sysml_value_10 = sysml_conditional_19;
    bool sysml_value_11 = 0;
    if (sysml_state_mode == 3) {
        sysml_conditional_20 = true;
    }
    else {
        sysml_conditional_20 = false;
    }
    sysml_value_11 = sysml_conditional_20;
    bool sysml_value_12 = 0;
    if (sysml_state_mode == 3) {
        sysml_conditional_21 = true;
    }
    else {
        sysml_conditional_21 = false;
    }
    sysml_value_12 = sysml_conditional_21;
    bool sysml_value_13 = 0;
    if (sysml_state_mode == 3) {
        sysml_conditional_22 = true;
    }
    else {
        sysml_conditional_22 = false;
    }
    sysml_value_13 = sysml_conditional_22;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
    *io_4 = sysml_value_4;
    *io_5 = sysml_value_5;
    *io_6_segment_0 = sysml_value_6;
    *io_6_segment_1 = sysml_value_7;
    *io_6_segment_2 = sysml_value_8;
    *io_6_segment_3 = sysml_value_9;
    *io_6_segment_4 = sysml_value_10;
    *io_6_segment_5 = sysml_value_11;
    *io_6_segment_6 = sysml_value_12;
    *io_6_segment_7 = sysml_value_13;
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
static bool hw_value_io_3 = false;
static bool hw_value_io_4 = false;
static bool hw_value_io_5 = false;
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
  digitalWrite(2, hw_value_io_3 ? HIGH : LOW);
  pinMode(3, OUTPUT);
  digitalWrite(3, hw_value_io_4 ? HIGH : LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, hw_value_io_5 ? HIGH : LOW);
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
  if (hw_started && hw_elapsed < 10UL) return;
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
  digitalWrite(2, hw_value_io_3 ? HIGH : LOW);
  digitalWrite(3, hw_value_io_4 ? HIGH : LOW);
  digitalWrite(4, hw_value_io_5 ? HIGH : LOW);
  digitalWrite(8, hw_value_io_6_segment_0 ? HIGH : LOW);
  digitalWrite(9, hw_value_io_6_segment_1 ? HIGH : LOW);
  digitalWrite(10, hw_value_io_6_segment_2 ? HIGH : LOW);
  digitalWrite(11, hw_value_io_6_segment_3 ? HIGH : LOW);
  digitalWrite(12, hw_value_io_6_segment_4 ? HIGH : LOW);
  digitalWrite(13, hw_value_io_6_segment_5 ? HIGH : LOW);
  digitalWrite(44, hw_value_io_6_segment_6 ? HIGH : LOW);
  digitalWrite(45, hw_value_io_6_segment_7 ? HIGH : LOW);
}
