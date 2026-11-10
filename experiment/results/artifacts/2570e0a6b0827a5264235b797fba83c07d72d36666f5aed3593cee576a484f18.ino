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
extern bool sysml_state_previousBtn1Pressed;
/* Global Variable define of  */
extern bool sysml_state_previousBtn2Pressed;
/* Global Variable define of  */
extern int32_t sysml_state_activeMode;
/* Global Variable define of  */
extern int32_t sysml_state_remainingHoldMs;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double* io_2, double* io_3, bool* io_4, bool* io_5, bool* io_6);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousBtn1Pressed = true;
/* Global Variable define of  */
bool sysml_state_previousBtn2Pressed = true;
/* Global Variable define of  */
int32_t sysml_state_activeMode = 0;
/* Global Variable define of  */
int32_t sysml_state_remainingHoldMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousBtn1Pressed = true;
    sysml_state_previousBtn2Pressed = true;
    sysml_state_activeMode = 0;
    sysml_state_remainingHoldMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double* io_2, double* io_3, bool* io_4, bool* io_5, bool* io_6) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    double sysml_conditional_8 = 0;
    bool sysml_lazy_9 = 0;
    double sysml_conditional_10 = 0;
    bool sysml_conditional_11 = 0;
    bool sysml_conditional_12 = 0;
    bool sysml_conditional_13 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_1;
    }
    if (sysml_lazy_0) {
        sysml_state_activeMode = 3;
        sysml_state_remainingHoldMs = 2000;
        sysml_state_previousBtn1Pressed = io_0;
        sysml_state_previousBtn2Pressed = io_1;
    }
    else {
        sysml_lazy_1 = io_0;
        if (sysml_lazy_1) {
            sysml_lazy_1 = !io_1;
        }
        if (sysml_lazy_1) {
            sysml_lazy_2 = io_0;
            if (sysml_lazy_2) {
                sysml_lazy_2 = !sysml_state_previousBtn1Pressed;
            }
            if (sysml_lazy_2) {
                sysml_state_activeMode = 1;
                sysml_state_remainingHoldMs = 2000;
            }
            sysml_lazy_3 = io_0;
            if (sysml_lazy_3) {
                sysml_lazy_3 = !sysml_state_previousBtn1Pressed;
            }
            if (sysml_lazy_3) {
            }
            sysml_state_previousBtn1Pressed = io_0;
            sysml_state_previousBtn2Pressed = io_1;
        }
        else {
            sysml_lazy_4 = io_1;
            if (sysml_lazy_4) {
                sysml_lazy_4 = !io_0;
            }
            if (sysml_lazy_4) {
                sysml_lazy_5 = io_1;
                if (sysml_lazy_5) {
                    sysml_lazy_5 = !sysml_state_previousBtn2Pressed;
                }
                if (sysml_lazy_5) {
                    sysml_state_activeMode = 2;
                    sysml_state_remainingHoldMs = 2000;
                }
                sysml_state_previousBtn1Pressed = io_0;
                sysml_state_previousBtn2Pressed = io_1;
            }
            else {
                sysml_lazy_6 = sysml_state_remainingHoldMs >= 0;
                if (sysml_lazy_6) {
                    sysml_lazy_6 = sysml_state_remainingHoldMs > 0;
                }
                if (sysml_lazy_6) {
                    sysml_state_remainingHoldMs = 0;
                    sysml_state_activeMode = 0;
                }
                else {
                    sysml_state_activeMode = 0;
                }
                sysml_state_previousBtn1Pressed = io_0;
                sysml_state_previousBtn2Pressed = io_1;
            }
        }
    }
    double sysml_value_0 = 0;
    sysml_lazy_7 = sysml_state_activeMode != 0;
    if (sysml_lazy_7) {
        sysml_lazy_7 = sysml_state_activeMode == 1;
    }
    if (sysml_lazy_7) {
        sysml_conditional_8 = 90.0;
    }
    else {
        sysml_conditional_8 = 0.0;
    }
    sysml_value_0 = sysml_conditional_8;
    double sysml_value_1 = 0;
    sysml_lazy_9 = sysml_state_activeMode != 0;
    if (sysml_lazy_9) {
        sysml_lazy_9 = sysml_state_activeMode == 2;
    }
    if (sysml_lazy_9) {
        sysml_conditional_10 = 180.0;
    }
    else {
        sysml_conditional_10 = 0.0;
    }
    sysml_value_1 = sysml_conditional_10;
    bool sysml_value_2 = 0;
    if (sysml_state_activeMode == 1) {
        sysml_conditional_11 = true;
    }
    else {
        sysml_conditional_11 = false;
    }
    sysml_value_2 = sysml_conditional_11;
    bool sysml_value_3 = 0;
    if (sysml_state_activeMode == 3) {
        sysml_conditional_12 = true;
    }
    else {
        sysml_conditional_12 = false;
    }
    sysml_value_3 = sysml_conditional_12;
    bool sysml_value_4 = 0;
    if (sysml_state_activeMode == 2) {
        sysml_conditional_13 = true;
    }
    else {
        sysml_conditional_13 = false;
    }
    sysml_value_4 = sysml_conditional_13;
    *io_2 = sysml_value_0;
    *io_3 = sysml_value_1;
    *io_4 = sysml_value_2;
    *io_5 = sysml_value_3;
    *io_6 = sysml_value_4;
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
static bool hw_value_io_0 = true;
static uint32_t hw_sampled_io_0 = 0;
static bool hw_attempted_io_0 = false;
static bool hw_value_io_1 = true;
static uint32_t hw_sampled_io_1 = 0;
static bool hw_attempted_io_1 = false;
static double hw_value_io_2 = 0;
Servo hw_servo_io_2;
static int hw_servo_previous_io_2 = -1;
static double hw_value_io_3 = 0;
Servo hw_servo_io_3;
static int hw_servo_previous_io_3 = -1;
static bool hw_value_io_4 = false;
static bool hw_value_io_5 = false;
static bool hw_value_io_6 = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  hw_servo_io_2.attach(7);
  if (isfinite((double)hw_value_io_2) && hw_value_io_2 >= 0 && hw_value_io_2 <= 180) {
    int command = (int)hw_value_io_2;
    if (command != hw_servo_previous_io_2) {
      hw_servo_io_2.write(command); hw_servo_previous_io_2 = command;
    }
  }
  hw_servo_io_3.attach(8);
  if (isfinite((double)hw_value_io_3) && hw_value_io_3 >= 0 && hw_value_io_3 <= 180) {
    int command = (int)hw_value_io_3;
    if (command != hw_servo_previous_io_3) {
      hw_servo_io_3.write(command); hw_servo_previous_io_3 = command;
    }
  }
  pinMode(4, OUTPUT);
  digitalWrite(4, hw_value_io_4 ? HIGH : LOW);
  pinMode(5, OUTPUT);
  digitalWrite(5, hw_value_io_5 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  digitalWrite(6, hw_value_io_6 ? HIGH : LOW);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 5UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (digitalRead(2) == LOW);
  }
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 5UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = (digitalRead(3) == LOW);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6);
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
  digitalWrite(4, hw_value_io_4 ? HIGH : LOW);
  digitalWrite(5, hw_value_io_5 ? HIGH : LOW);
  digitalWrite(6, hw_value_io_6 ? HIGH : LOW);
}
