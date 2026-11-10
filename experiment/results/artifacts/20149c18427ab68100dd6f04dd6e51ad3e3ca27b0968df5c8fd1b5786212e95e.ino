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
extern bool sysml_state_initialized;
/* Global Variable define of  */
extern int32_t sysml_state_displayNumber;
/* Global Variable define of  */
extern int32_t sysml_state_previousTensDigit;
/* Global Variable define of  */
extern int32_t sysml_state_previousUnitsDigit;
/* Global Variable define of  */
extern bool sysml_state_previousIncrementPressed;
/* Global Variable define of  */
extern bool sysml_state_previousDecrementPressed;
/* Global Variable define of  */
extern double sysml_state_incrementElapsed;
/* Global Variable define of  */
extern double sysml_state_decrementElapsed;
/* Global Variable define of  */
extern int32_t sysml_state_incrementSteps;
/* Global Variable define of  */
extern int32_t sysml_state_decrementSteps;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double io_2, double io_3, int32_t io_5, int32_t* io_4);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_initialized = false;
/* Global Variable define of  */
int32_t sysml_state_displayNumber = 0;
/* Global Variable define of  */
int32_t sysml_state_previousTensDigit = 0;
/* Global Variable define of  */
int32_t sysml_state_previousUnitsDigit = 0;
/* Global Variable define of  */
bool sysml_state_previousIncrementPressed = false;
/* Global Variable define of  */
bool sysml_state_previousDecrementPressed = false;
/* Global Variable define of  */
double sysml_state_incrementElapsed = 0.0;
/* Global Variable define of  */
double sysml_state_decrementElapsed = 0.0;
/* Global Variable define of  */
int32_t sysml_state_incrementSteps = 0;
/* Global Variable define of  */
int32_t sysml_state_decrementSteps = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_initialized = false;
    sysml_state_displayNumber = 0;
    sysml_state_previousTensDigit = 0;
    sysml_state_previousUnitsDigit = 0;
    sysml_state_previousIncrementPressed = false;
    sysml_state_previousDecrementPressed = false;
    sysml_state_incrementElapsed = 0.0;
    sysml_state_decrementElapsed = 0.0;
    sysml_state_incrementSteps = 0;
    sysml_state_decrementSteps = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double io_2, double io_3, int32_t io_5, int32_t* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int32_t sysml_conditional_0 = 0;
    int32_t sysml_conditional_1 = 0;
    int32_t sysml_conditional_2 = 0;
    int32_t sysml_conditional_3 = 0;
    int32_t sysml_conditional_4 = 0;
    int32_t sysml_arithmetic_5 = 0;
    int32_t sysml_conditional_6 = 0;
    int32_t sysml_conditional_7 = 0;
    int32_t sysml_arithmetic_8 = 0;
    int32_t sysml_conditional_9 = 0;
    double sysml_arithmetic_10 = 0;
    double sysml_arithmetic_11 = 0;
    double sysml_arithmetic_12 = 0;
    int32_t sysml_conditional_13 = 0;
    double sysml_arithmetic_14 = 0;
    double sysml_arithmetic_15 = 0;
    double sysml_arithmetic_16 = 0;
    double sysml_arithmetic_17 = 0;
    int32_t sysml_conditional_18 = 0;
    double sysml_arithmetic_19 = 0;
    int32_t sysml_conditional_20 = 0;
    int32_t sysml_arithmetic_21 = 0;
    int32_t sysml_conditional_22 = 0;
    int32_t sysml_conditional_23 = 0;
    int32_t sysml_conditional_24 = 0;
    int32_t sysml_conditional_25 = 0;
    if ((!sysml_state_initialized)) {
        if ((((double)(io_2 * 10.0)) / 1024.0) < ((double)((int32_t)(((double)(io_2 * 10.0)) / 1024.0)))) {
            sysml_conditional_0 = 1;
        }
        else {
            sysml_conditional_0 = 0;
        }
        if ((((double)(io_3 * 10.0)) / 1024.0) < ((double)((int32_t)(((double)(io_3 * 10.0)) / 1024.0)))) {
            sysml_conditional_1 = 1;
        }
        else {
            sysml_conditional_1 = 0;
        }
        sysml_state_displayNumber = ((((int32_t)(((double)(io_2 * 10.0)) / 1024.0)) - sysml_conditional_0) * 10) + (((int32_t)(((double)(io_3 * 10.0)) / 1024.0)) - sysml_conditional_1);
        sysml_state_initialized = true;
    }
    else {
        if ((((double)(io_2 * 10.0)) / 1024.0) < ((double)((int32_t)(((double)(io_2 * 10.0)) / 1024.0)))) {
            sysml_conditional_2 = 1;
        }
        else {
            sysml_conditional_2 = 0;
        }
        if ((((int32_t)(((double)(io_2 * 10.0)) / 1024.0)) - sysml_conditional_2) != sysml_state_previousTensDigit) {
            if ((((double)(io_2 * 10.0)) / 1024.0) < ((double)((int32_t)(((double)(io_2 * 10.0)) / 1024.0)))) {
                sysml_conditional_3 = 1;
            }
            else {
                sysml_conditional_3 = 0;
            }
            if ((((double)sysml_state_displayNumber) / 10) < ((double)((int32_t)(((double)sysml_state_displayNumber) / 10)))) {
                sysml_conditional_4 = 1;
            }
            else {
                sysml_conditional_4 = 0;
            }
            sysml_arithmetic_5 = ((int32_t)(((double)sysml_state_displayNumber) / 10)) - sysml_conditional_4;
            sysml_state_displayNumber = ((((int32_t)(((double)(io_2 * 10.0)) / 1024.0)) - sysml_conditional_3) * 10) + (sysml_state_displayNumber - (10 * sysml_arithmetic_5));
        }
        if ((((double)(io_3 * 10.0)) / 1024.0) < ((double)((int32_t)(((double)(io_3 * 10.0)) / 1024.0)))) {
            sysml_conditional_6 = 1;
        }
        else {
            sysml_conditional_6 = 0;
        }
        if ((((int32_t)(((double)(io_3 * 10.0)) / 1024.0)) - sysml_conditional_6) != sysml_state_previousUnitsDigit) {
            if ((((double)sysml_state_displayNumber) / 10) < ((double)((int32_t)(((double)sysml_state_displayNumber) / 10)))) {
                sysml_conditional_7 = 1;
            }
            else {
                sysml_conditional_7 = 0;
            }
            sysml_arithmetic_8 = ((int32_t)(((double)sysml_state_displayNumber) / 10)) - sysml_conditional_7;
            if ((((double)(io_3 * 10.0)) / 1024.0) < ((double)((int32_t)(((double)(io_3 * 10.0)) / 1024.0)))) {
                sysml_conditional_9 = 1;
            }
            else {
                sysml_conditional_9 = 0;
            }
            sysml_state_displayNumber = (10 * sysml_arithmetic_8) + (((int32_t)(((double)(io_3 * 10.0)) / 1024.0)) - sysml_conditional_9);
        }
    }
    sysml_state_incrementSteps = 0;
    sysml_state_decrementSteps = 0;
    if (io_0) {
        if ((!sysml_state_previousIncrementPressed)) {
            sysml_state_incrementSteps = 1;
            sysml_state_incrementElapsed = 0.0;
        }
        else {
            sysml_state_incrementElapsed = (sysml_state_incrementElapsed + io_5);
            sysml_arithmetic_10 = 100.0 + (((double)(io_2 * 900.0)) / 1023.0);
            sysml_arithmetic_11 = 100.0 + (((double)(io_2 * 900.0)) / 1023.0);
            sysml_arithmetic_12 = 100.0 + (((double)(io_2 * 900.0)) / 1023.0);
            if ((((double)sysml_state_incrementElapsed) / sysml_arithmetic_11) < ((double)((int32_t)(((double)sysml_state_incrementElapsed) / sysml_arithmetic_12)))) {
                sysml_conditional_13 = 1;
            }
            else {
                sysml_conditional_13 = 0;
            }
            sysml_state_incrementSteps = ((int32_t)(((double)sysml_state_incrementElapsed) / sysml_arithmetic_10)) - sysml_conditional_13;
            sysml_arithmetic_14 = 100.0 + (((double)(io_2 * 900.0)) / 1023.0);
            sysml_state_incrementElapsed = sysml_state_incrementElapsed - (sysml_state_incrementSteps * sysml_arithmetic_14);
        }
    }
    else {
        sysml_state_incrementElapsed = 0.0;
    }
    if (io_1) {
        if ((!sysml_state_previousDecrementPressed)) {
            sysml_state_decrementSteps = 1;
            sysml_state_decrementElapsed = 0.0;
        }
        else {
            sysml_state_decrementElapsed = (sysml_state_decrementElapsed + io_5);
            sysml_arithmetic_15 = 100.0 + (((double)(io_3 * 900.0)) / 1023.0);
            sysml_arithmetic_16 = 100.0 + (((double)(io_3 * 900.0)) / 1023.0);
            sysml_arithmetic_17 = 100.0 + (((double)(io_3 * 900.0)) / 1023.0);
            if ((((double)sysml_state_decrementElapsed) / sysml_arithmetic_16) < ((double)((int32_t)(((double)sysml_state_decrementElapsed) / sysml_arithmetic_17)))) {
                sysml_conditional_18 = 1;
            }
            else {
                sysml_conditional_18 = 0;
            }
            sysml_state_decrementSteps = ((int32_t)(((double)sysml_state_decrementElapsed) / sysml_arithmetic_15)) - sysml_conditional_18;
            sysml_arithmetic_19 = 100.0 + (((double)(io_3 * 900.0)) / 1023.0);
            sysml_state_decrementElapsed = sysml_state_decrementElapsed - (sysml_state_decrementSteps * sysml_arithmetic_19);
        }
    }
    else {
        sysml_state_decrementElapsed = 0.0;
    }
    sysml_state_displayNumber = ((sysml_state_displayNumber + sysml_state_incrementSteps) - sysml_state_decrementSteps);
    if ((((double)sysml_state_displayNumber) / 100) < ((double)((int32_t)(((double)sysml_state_displayNumber) / 100)))) {
        sysml_conditional_20 = 1;
    }
    else {
        sysml_conditional_20 = 0;
    }
    sysml_arithmetic_21 = ((int32_t)(((double)sysml_state_displayNumber) / 100)) - sysml_conditional_20;
    sysml_state_displayNumber = sysml_state_displayNumber - (100 * sysml_arithmetic_21);
    if ((((double)(io_2 * 10.0)) / 1024.0) < ((double)((int32_t)(((double)(io_2 * 10.0)) / 1024.0)))) {
        sysml_conditional_22 = 1;
    }
    else {
        sysml_conditional_22 = 0;
    }
    sysml_state_previousTensDigit = ((int32_t)(((double)(io_2 * 10.0)) / 1024.0)) - sysml_conditional_22;
    if ((((double)(io_3 * 10.0)) / 1024.0) < ((double)((int32_t)(((double)(io_3 * 10.0)) / 1024.0)))) {
        sysml_conditional_23 = 1;
    }
    else {
        sysml_conditional_23 = 0;
    }
    sysml_state_previousUnitsDigit = ((int32_t)(((double)(io_3 * 10.0)) / 1024.0)) - sysml_conditional_23;
    sysml_state_previousIncrementPressed = io_0;
    sysml_state_previousDecrementPressed = io_1;
    int32_t sysml_value_0 = 0;
    if ((((double)(io_2 * 10.0)) / 1024.0) < ((double)((int32_t)(((double)(io_2 * 10.0)) / 1024.0)))) {
        sysml_conditional_24 = 1;
    }
    else {
        sysml_conditional_24 = 0;
    }
    sysml_value_0 = ((int32_t)(((double)(io_2 * 10.0)) / 1024.0)) - sysml_conditional_24;
    int32_t sysml_value_1 = 0;
    if ((((double)(io_3 * 10.0)) / 1024.0) < ((double)((int32_t)(((double)(io_3 * 10.0)) / 1024.0)))) {
        sysml_conditional_25 = 1;
    }
    else {
        sysml_conditional_25 = 0;
    }
    sysml_value_1 = ((int32_t)(((double)(io_3 * 10.0)) / 1024.0)) - sysml_conditional_25;
    double sysml_value_2 = 0;
    sysml_value_2 = (100.0 + ((double)((io_2 * 900.0)) / 1023.0));
    double sysml_value_3 = 0;
    sysml_value_3 = (100.0 + ((double)((io_3 * 900.0)) / 1023.0));
    int32_t sysml_value_4 = 0;
    sysml_value_4 = sysml_state_displayNumber;
    *io_4 = sysml_value_4;
}
// END COMPILED SYSML MODEL

// LLM4MDD target-board: arduino:avr:mega
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
static bool hw_value_io_1 = false;
static uint32_t hw_sampled_io_1 = 0;
static bool hw_attempted_io_1 = false;
static double hw_value_io_2 = 0;
static uint32_t hw_sampled_io_2 = 0;
static bool hw_attempted_io_2 = false;
static double hw_value_io_3 = 0;
static uint32_t hw_sampled_io_3 = 0;
static bool hw_attempted_io_3 = false;
static int32_t hw_value_io_4 = 0;
static void hw_shift595_io_4(int32_t value) {
  static const uint8_t digits[10] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
  bool valid = value >= 0 && value < 100L;
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
  digitalWrite(A1, HIGH);
}
static int32_t hw_shift595_previous_io_4 = INT32_MIN;
static int32_t hw_value_io_5 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
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
    hw_value_io_0 = (digitalRead(2) == LOW);
  }
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 5UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = (digitalRead(3) == LOW);
  }
  if (!hw_attempted_io_2 || (uint32_t)(hw_now - hw_sampled_io_2) >= 5UL) {
    hw_attempted_io_2 = true; hw_sampled_io_2 = hw_now;
    hw_value_io_2 = (analogRead(A3) * 1.0 + 0.0);
  }
  if (!hw_attempted_io_3 || (uint32_t)(hw_now - hw_sampled_io_3) >= 5UL) {
    hw_attempted_io_3 = true; hw_sampled_io_3 = hw_now;
    hw_value_io_3 = (analogRead(A4) * 1.0 + 0.0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_5 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, hw_value_io_3, hw_value_io_5, &hw_value_io_4);
  if (hw_value_io_4 != hw_shift595_previous_io_4) {
    hw_shift595_io_4(hw_value_io_4); hw_shift595_previous_io_4 = hw_value_io_4;
  }
}
