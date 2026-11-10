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
void ProgramPage1(double io_0, double io_1, double* io_2, double* io_3, int32_t* io_4);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, double* io_2, double* io_3, int32_t* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_value_0 = 0;
    double sysml_conditional_0 = 0;
    double sysml_conditional_1 = 0;
    double sysml_conditional_2 = 0;
    double sysml_conditional_3 = 0;
    int32_t sysml_conditional_4 = 0;
    int32_t sysml_conditional_5 = 0;
    int32_t sysml_conditional_6 = 0;
    int32_t sysml_arithmetic_7 = 0;
    int32_t sysml_conditional_8 = 0;
    int32_t sysml_arithmetic_9 = 0;
    int32_t sysml_conditional_10 = 0;
    int32_t sysml_arithmetic_11 = 0;
    int32_t sysml_conditional_12 = 0;
    int32_t sysml_arithmetic_13 = 0;
    int32_t sysml_conditional_14 = 0;
    if (io_0 >= 0.0) {
        if (io_0 <= 99.0) {
            sysml_conditional_1 = io_0;
        }
        else {
            sysml_conditional_1 = 99.0;
        }
        sysml_conditional_0 = sysml_conditional_1;
    }
    else {
        sysml_conditional_0 = 0.0;
    }
    sysml_value_0 = sysml_conditional_0;
    double sysml_value_1 = 0;
    if (io_1 >= 0.0) {
        if (io_1 <= 99.0) {
            sysml_conditional_3 = io_1;
        }
        else {
            sysml_conditional_3 = 99.0;
        }
        sysml_conditional_2 = sysml_conditional_3;
    }
    else {
        sysml_conditional_2 = 0.0;
    }
    sysml_value_1 = sysml_conditional_2;
    int32_t sysml_value_2 = 0;
    if (io_0 >= 0.0) {
        if (io_0 <= 99.0) {
            if ((((double)io_0) / 10.0) < ((double)((int32_t)(((double)io_0) / 10.0)))) {
                sysml_conditional_6 = 1;
            }
            else {
                sysml_conditional_6 = 0;
            }
            sysml_arithmetic_7 = ((int32_t)(((double)io_0) / 10.0)) - sysml_conditional_6;
            if ((((double)io_0) / 10.0) < ((double)((int32_t)(((double)io_0) / 10.0)))) {
                sysml_conditional_8 = 1;
            }
            else {
                sysml_conditional_8 = 0;
            }
            sysml_arithmetic_9 = ((int32_t)(((double)io_0) / 10.0)) - sysml_conditional_8;
            if ((((double)io_0) / 10.0) < ((double)((int32_t)(((double)io_0) / 10.0)))) {
                sysml_conditional_10 = 1;
            }
            else {
                sysml_conditional_10 = 0;
            }
            sysml_arithmetic_11 = ((int32_t)(((double)io_0) / 10.0)) - sysml_conditional_10;
            if ((((double)io_0) / 10.0) < ((double)((int32_t)(((double)io_0) / 10.0)))) {
                sysml_conditional_12 = 1;
            }
            else {
                sysml_conditional_12 = 0;
            }
            sysml_arithmetic_13 = ((int32_t)(((double)io_0) / 10.0)) - sysml_conditional_12;
            if ((io_0 - (10.0 * sysml_arithmetic_11)) < ((double)((int32_t)(io_0 - (10.0 * sysml_arithmetic_13))))) {
                sysml_conditional_14 = 1;
            }
            else {
                sysml_conditional_14 = 0;
            }
            sysml_conditional_5 = (2 * sysml_arithmetic_7) + (((int32_t)(io_0 - (10.0 * sysml_arithmetic_9))) - sysml_conditional_14);
        }
        else {
            sysml_conditional_5 = 0;
        }
        sysml_conditional_4 = sysml_conditional_5;
    }
    else {
        sysml_conditional_4 = 0;
    }
    sysml_value_2 = sysml_conditional_4;
    *io_2 = sysml_value_0;
    *io_3 = sysml_value_1;
    *io_4 = sysml_value_2;
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
static uint32_t hw_sampled_io_0 = 0;
static bool hw_attempted_io_0 = false;
static double hw_value_io_1 = 0;
static uint32_t hw_sampled_io_1 = 0;
static bool hw_attempted_io_1 = false;
static double hw_value_io_2 = 0;
Servo hw_servo_io_2;
static int hw_servo_previous_io_2 = -1;
static double hw_value_io_3 = 0;
Servo hw_servo_io_3;
static int hw_servo_previous_io_3 = -1;
static int32_t hw_value_io_4 = 0;
static void hw_shift595_io_4(int32_t value) {
  static const uint8_t digits[10] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
  bool valid = value >= 0 && value < 100L;
  digitalWrite(A1, LOW);
  {
    uint8_t segments = valid && (true) ? digits[(value / 10L) % 10] : 0;
    uint8_t bits = 0;
    if (segments & 1) bits |= 1;
    if (segments & 2) bits |= 2;
    if (segments & 4) bits |= 4;
    if (segments & 8) bits |= 8;
    if (segments & 16) bits |= 16;
    if (segments & 32) bits |= 32;
    if (segments & 64) bits |= 64;
    bits = (uint8_t)~bits;
    shiftOut(A0, A2, MSBFIRST, bits);
  }
  {
    uint8_t segments = valid && (true) ? digits[(value / 1L) % 10] : 0;
    uint8_t bits = 0;
    if (segments & 1) bits |= 1;
    if (segments & 2) bits |= 2;
    if (segments & 4) bits |= 4;
    if (segments & 8) bits |= 8;
    if (segments & 16) bits |= 16;
    if (segments & 32) bits |= 32;
    if (segments & 64) bits |= 64;
    bits = (uint8_t)~bits;
    shiftOut(A0, A2, MSBFIRST, bits);
  }
  digitalWrite(A1, HIGH);
}
static int32_t hw_shift595_previous_io_4 = INT32_MIN;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  hw_servo_io_2.attach(2);
  if (isfinite((double)hw_value_io_2) && hw_value_io_2 >= 0 && hw_value_io_2 <= 180) {
    int command = (int)hw_value_io_2;
    if (command != hw_servo_previous_io_2) {
      hw_servo_io_2.write(command); hw_servo_previous_io_2 = command;
    }
  }
  hw_servo_io_3.attach(3);
  if (isfinite((double)hw_value_io_3) && hw_value_io_3 >= 0 && hw_value_io_3 <= 180) {
    int command = (int)hw_value_io_3;
    if (command != hw_servo_previous_io_3) {
      hw_servo_io_3.write(command); hw_servo_previous_io_3 = command;
    }
  }
  pinMode(A0, OUTPUT); digitalWrite(A0, LOW);
  pinMode(A2, OUTPUT); digitalWrite(A2, LOW);
  pinMode(A1, OUTPUT); digitalWrite(A1, LOW);
  if (hw_value_io_4 != hw_shift595_previous_io_4) {
    hw_shift595_io_4(hw_value_io_4); hw_shift595_previous_io_4 = hw_value_io_4;
  }
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 5UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (analogRead(A3) * 1.0 + 0.0);
  }
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 5UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = (analogRead(A4) * 1.0 + 0.0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4);
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
  if (hw_value_io_4 != hw_shift595_previous_io_4) {
    hw_shift595_io_4(hw_value_io_4); hw_shift595_previous_io_4 = hw_value_io_4;
  }
}
