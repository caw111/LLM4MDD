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
extern double sysml_state_prevAngle;
/* Global Variable define of  */
extern int32_t sysml_state_holdMsCounter;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_2, bool* io_0, bool* io_1, double* io_3);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_prevAngle = 0.0;
/* Global Variable define of  */
int32_t sysml_state_holdMsCounter = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_prevAngle = 0.0;
    sysml_state_holdMsCounter = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_2, bool* io_0, bool* io_1, double* io_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_conditional_0 = 0;
    double sysml_conditional_1 = 0;
    double sysml_conditional_2 = 0;
    double sysml_conditional_3 = 0;
    double sysml_conditional_4 = 0;
    double sysml_conditional_5 = 0;
    double sysml_conditional_6 = 0;
    double sysml_conditional_7 = 0;
    sysml_state_holdMsCounter = 0;
    if (io_2 >= 0.0) {
        if (io_2 <= 1023.0) {
            sysml_conditional_1 = ((double)(io_2 * 180.0)) / 1023.0;
        }
        else {
            if (io_2 > 1023.0) {
                sysml_conditional_2 = 180.0;
            }
            else {
                sysml_conditional_2 = 0.0;
            }
            sysml_conditional_1 = sysml_conditional_2;
        }
        sysml_conditional_0 = sysml_conditional_1;
    }
    else {
        if (io_2 < 0.0) {
            sysml_conditional_3 = 0.0;
        }
        else {
            sysml_conditional_3 = ((double)(io_2 * 180.0)) / 1023.0;
        }
        sysml_conditional_0 = sysml_conditional_3;
    }
    sysml_state_prevAngle = sysml_conditional_0;
    double sysml_value_0 = 0;
    if (io_2 >= 0.0) {
        if (io_2 <= 1023.0) {
            sysml_conditional_5 = ((double)(io_2 * 180.0)) / 1023.0;
        }
        else {
            if (io_2 > 1023.0) {
                sysml_conditional_6 = 180.0;
            }
            else {
                sysml_conditional_6 = 0.0;
            }
            sysml_conditional_5 = sysml_conditional_6;
        }
        sysml_conditional_4 = sysml_conditional_5;
    }
    else {
        if (io_2 < 0.0) {
            sysml_conditional_7 = 0.0;
        }
        else {
            sysml_conditional_7 = ((double)(io_2 * 180.0)) / 1023.0;
        }
        sysml_conditional_4 = sysml_conditional_7;
    }
    sysml_value_0 = sysml_conditional_4;
    bool sysml_value_1 = 0;
    sysml_value_1 = false;
    bool sysml_value_2 = 0;
    sysml_value_2 = false;
    *io_0 = sysml_value_1;
    *io_1 = sysml_value_2;
    *io_3 = sysml_value_0;
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
static double hw_value_io_2 = 0;
static uint32_t hw_sampled_io_2 = 0;
static bool hw_attempted_io_2 = false;
static double hw_value_io_3 = 0;
Servo hw_servo_io_3;
static int hw_servo_previous_io_3 = -1;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, OUTPUT);
  digitalWrite(2, hw_value_io_0 ? HIGH : LOW);
  pinMode(3, OUTPUT);
  digitalWrite(3, hw_value_io_1 ? HIGH : LOW);
  pinMode(A3, INPUT);
  hw_servo_io_3.attach(4);
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
  if (!hw_attempted_io_2 || (uint32_t)(hw_now - hw_sampled_io_2) >= 5UL) {
    hw_attempted_io_2 = true; hw_sampled_io_2 = hw_now;
    hw_value_io_2 = (analogRead(A3) * 1.0 + 0.0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_2, &hw_value_io_0, &hw_value_io_1, &hw_value_io_3);
  digitalWrite(2, hw_value_io_0 ? HIGH : LOW);
  digitalWrite(3, hw_value_io_1 ? HIGH : LOW);
  if (isfinite((double)hw_value_io_3) && hw_value_io_3 >= 0 && hw_value_io_3 <= 180) {
    int command = (int)hw_value_io_3;
    if (command != hw_servo_previous_io_3) {
      hw_servo_io_3.write(command); hw_servo_previous_io_3 = command;
    }
  }
}
