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
extern int32_t sysml_state_count;
/* Global Variable define of  */
extern bool sysml_state_previousPressed;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, int32_t* io_1, double* io_2, double* io_3);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_count = 0;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_count = 0;
    sysml_state_previousPressed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, int32_t* io_1, double* io_2, double* io_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int32_t sysml_conditional_1 = 0;
    int32_t sysml_conditional_2 = 0;
    int32_t sysml_arithmetic_3 = 0;
    double sysml_conditional_4 = 0;
    int32_t sysml_conditional_5 = 0;
    int32_t sysml_arithmetic_6 = 0;
    double sysml_conditional_7 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        if (sysml_state_count >= 9) {
            sysml_conditional_1 = 0;
        }
        else {
            sysml_conditional_1 = sysml_state_count + 1;
        }
        sysml_state_count = sysml_conditional_1;
    }
    sysml_state_previousPressed = io_0;
    int32_t sysml_value_0 = 0;
    sysml_value_0 = sysml_state_count;
    double sysml_value_1 = 0;
    if ((((double)sysml_state_count) / 2) < ((double)((int32_t)(((double)sysml_state_count) / 2)))) {
        sysml_conditional_2 = 1;
    }
    else {
        sysml_conditional_2 = 0;
    }
    sysml_arithmetic_3 = ((int32_t)(((double)sysml_state_count) / 2)) - sysml_conditional_2;
    if ((sysml_state_count - (2 * sysml_arithmetic_3)) == 0) {
        sysml_conditional_4 = 0.0;
    }
    else {
        sysml_conditional_4 = 90.0;
    }
    sysml_value_1 = sysml_conditional_4;
    double sysml_value_2 = 0;
    if ((((double)sysml_state_count) / 2) < ((double)((int32_t)(((double)sysml_state_count) / 2)))) {
        sysml_conditional_5 = 1;
    }
    else {
        sysml_conditional_5 = 0;
    }
    sysml_arithmetic_6 = ((int32_t)(((double)sysml_state_count) / 2)) - sysml_conditional_5;
    if ((sysml_state_count - (2 * sysml_arithmetic_6)) == 0) {
        sysml_conditional_7 = 0.0;
    }
    else {
        sysml_conditional_7 = 90.0;
    }
    sysml_value_2 = sysml_conditional_7;
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
static int32_t hw_value_io_1 = 0;
static void hw_shift595_io_1(int32_t value) {
  static const uint8_t digits[10] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
  bool valid = value >= 0 && value < 10L;
  digitalWrite(A1, LOW);
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
static int32_t hw_shift595_previous_io_1 = INT32_MIN;
static double hw_value_io_2 = 0;
Servo hw_servo_io_2;
static int hw_servo_previous_io_2 = -1;
static double hw_value_io_3 = 0;
Servo hw_servo_io_3;
static int hw_servo_previous_io_3 = -1;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, INPUT_PULLUP);
  pinMode(A0, OUTPUT); digitalWrite(A0, LOW);
  pinMode(A2, OUTPUT); digitalWrite(A2, LOW);
  pinMode(A1, OUTPUT); digitalWrite(A1, LOW);
  if (hw_value_io_1 != hw_shift595_previous_io_1) {
    hw_shift595_io_1(hw_value_io_1); hw_shift595_previous_io_1 = hw_value_io_1;
  }
  hw_servo_io_2.attach(3);
  if (isfinite((double)hw_value_io_2) && hw_value_io_2 >= 0 && hw_value_io_2 <= 180) {
    int command = (int)hw_value_io_2;
    if (command != hw_servo_previous_io_2) {
      hw_servo_io_2.write(command); hw_servo_previous_io_2 = command;
    }
  }
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
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 5UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (digitalRead(2) == LOW);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3);
  if (hw_value_io_1 != hw_shift595_previous_io_1) {
    hw_shift595_io_1(hw_value_io_1); hw_shift595_previous_io_1 = hw_value_io_1;
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
