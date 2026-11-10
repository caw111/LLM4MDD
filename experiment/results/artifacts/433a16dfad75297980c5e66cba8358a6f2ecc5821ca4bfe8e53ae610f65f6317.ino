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
extern bool sysml_state_prevBtn1;
/* Global Variable define of  */
extern bool sysml_state_prevBtn2;
/* Global Variable define of  */
extern bool sysml_state_btn1Stable;
/* Global Variable define of  */
extern bool sysml_state_btn2Stable;
/* Global Variable define of  */
extern int32_t sysml_state_debounceCount1;
/* Global Variable define of  */
extern int32_t sysml_state_debounceCount2;
/* Global Variable define of  */
extern int32_t sysml_state_colorIndex;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double* io_2, double* io_3, double* io_4);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_prevBtn1 = false;
/* Global Variable define of  */
bool sysml_state_prevBtn2 = false;
/* Global Variable define of  */
bool sysml_state_btn1Stable = false;
/* Global Variable define of  */
bool sysml_state_btn2Stable = false;
/* Global Variable define of  */
int32_t sysml_state_debounceCount1 = 0;
/* Global Variable define of  */
int32_t sysml_state_debounceCount2 = 0;
/* Global Variable define of  */
int32_t sysml_state_colorIndex = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_prevBtn1 = false;
    sysml_state_prevBtn2 = false;
    sysml_state_btn1Stable = false;
    sysml_state_btn2Stable = false;
    sysml_state_debounceCount1 = 0;
    sysml_state_debounceCount2 = 0;
    sysml_state_colorIndex = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double* io_2, double* io_3, double* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_conditional_0 = 0;
    bool sysml_conditional_1 = 0;
    bool sysml_conditional_2 = 0;
    bool sysml_conditional_3 = 0;
    int32_t sysml_conditional_4 = 0;
    int32_t sysml_conditional_5 = 0;
    int32_t sysml_conditional_6 = 0;
    int32_t sysml_conditional_7 = 0;
    bool sysml_lazy_8 = 0;
    int32_t sysml_conditional_9 = 0;
    bool sysml_lazy_10 = 0;
    int32_t sysml_conditional_11 = 0;
    bool sysml_lazy_12 = 0;
    bool sysml_lazy_13 = 0;
    bool sysml_lazy_14 = 0;
    double sysml_conditional_15 = 0;
    double sysml_conditional_16 = 0;
    double sysml_conditional_17 = 0;
    bool sysml_lazy_18 = 0;
    bool sysml_lazy_19 = 0;
    bool sysml_lazy_20 = 0;
    double sysml_conditional_21 = 0;
    bool sysml_lazy_22 = 0;
    bool sysml_lazy_23 = 0;
    bool sysml_lazy_24 = 0;
    double sysml_conditional_25 = 0;
    if (io_0) {
        if (sysml_state_debounceCount1 >= 2) {
            sysml_conditional_1 = true;
        }
        else {
            sysml_conditional_1 = sysml_state_btn1Stable;
        }
        sysml_conditional_0 = sysml_conditional_1;
    }
    else {
        sysml_conditional_0 = false;
    }
    sysml_state_btn1Stable = sysml_conditional_0;
    if (io_1) {
        if (sysml_state_debounceCount2 >= 2) {
            sysml_conditional_3 = true;
        }
        else {
            sysml_conditional_3 = sysml_state_btn2Stable;
        }
        sysml_conditional_2 = sysml_conditional_3;
    }
    else {
        sysml_conditional_2 = false;
    }
    sysml_state_btn2Stable = sysml_conditional_2;
    if (io_0) {
        if (sysml_state_debounceCount1 >= 2) {
            sysml_conditional_5 = 2;
        }
        else {
            sysml_conditional_5 = sysml_state_debounceCount1 + 1;
        }
        sysml_conditional_4 = sysml_conditional_5;
    }
    else {
        sysml_conditional_4 = 0;
    }
    sysml_state_debounceCount1 = sysml_conditional_4;
    if (io_1) {
        if (sysml_state_debounceCount2 >= 2) {
            sysml_conditional_7 = 2;
        }
        else {
            sysml_conditional_7 = sysml_state_debounceCount2 + 1;
        }
        sysml_conditional_6 = sysml_conditional_7;
    }
    else {
        sysml_conditional_6 = 0;
    }
    sysml_state_debounceCount2 = sysml_conditional_6;
    sysml_lazy_8 = sysml_state_btn1Stable;
    if (sysml_lazy_8) {
        sysml_lazy_8 = !sysml_state_prevBtn1;
    }
    if (sysml_lazy_8) {
        if (sysml_state_colorIndex == 8) {
            sysml_conditional_9 = 0;
        }
        else {
            sysml_conditional_9 = sysml_state_colorIndex + 1;
        }
        sysml_state_colorIndex = sysml_conditional_9;
    }
    sysml_lazy_10 = sysml_state_btn2Stable;
    if (sysml_lazy_10) {
        sysml_lazy_10 = !sysml_state_prevBtn2;
    }
    if (sysml_lazy_10) {
        if (sysml_state_colorIndex == 0) {
            sysml_conditional_11 = 8;
        }
        else {
            sysml_conditional_11 = sysml_state_colorIndex - 1;
        }
        sysml_state_colorIndex = sysml_conditional_11;
    }
    sysml_state_prevBtn1 = sysml_state_btn1Stable;
    sysml_state_prevBtn2 = sysml_state_btn2Stable;
    double sysml_value_0 = 0;
    sysml_lazy_12 = sysml_state_colorIndex == 1;
    if (!sysml_lazy_12) {
        sysml_lazy_12 = sysml_state_colorIndex == 4;
    }
    sysml_lazy_13 = sysml_lazy_12;
    if (!sysml_lazy_13) {
        sysml_lazy_13 = sysml_state_colorIndex == 7;
    }
    sysml_lazy_14 = sysml_lazy_13;
    if (!sysml_lazy_14) {
        sysml_lazy_14 = sysml_state_colorIndex == 6;
    }
    if (sysml_lazy_14) {
        sysml_conditional_15 = 255.0;
    }
    else {
        if (sysml_state_colorIndex == 5) {
            sysml_conditional_16 = 0.0;
        }
        else {
            if (sysml_state_colorIndex == 3) {
                sysml_conditional_17 = 0.0;
            }
            else {
                sysml_conditional_17 = 0.0;
            }
            sysml_conditional_16 = sysml_conditional_17;
        }
        sysml_conditional_15 = sysml_conditional_16;
    }
    sysml_value_0 = sysml_conditional_15;
    double sysml_value_1 = 0;
    sysml_lazy_18 = sysml_state_colorIndex == 2;
    if (!sysml_lazy_18) {
        sysml_lazy_18 = sysml_state_colorIndex == 4;
    }
    sysml_lazy_19 = sysml_lazy_18;
    if (!sysml_lazy_19) {
        sysml_lazy_19 = sysml_state_colorIndex == 5;
    }
    sysml_lazy_20 = sysml_lazy_19;
    if (!sysml_lazy_20) {
        sysml_lazy_20 = sysml_state_colorIndex == 7;
    }
    if (sysml_lazy_20) {
        sysml_conditional_21 = 255.0;
    }
    else {
        sysml_conditional_21 = 0.0;
    }
    sysml_value_1 = sysml_conditional_21;
    double sysml_value_2 = 0;
    sysml_lazy_22 = sysml_state_colorIndex == 3;
    if (!sysml_lazy_22) {
        sysml_lazy_22 = sysml_state_colorIndex == 4;
    }
    sysml_lazy_23 = sysml_lazy_22;
    if (!sysml_lazy_23) {
        sysml_lazy_23 = sysml_state_colorIndex == 6;
    }
    sysml_lazy_24 = sysml_lazy_23;
    if (!sysml_lazy_24) {
        sysml_lazy_24 = sysml_state_colorIndex == 7;
    }
    if (sysml_lazy_24) {
        sysml_conditional_25 = 255.0;
    }
    else {
        sysml_conditional_25 = 0.0;
    }
    sysml_value_2 = sysml_conditional_25;
    *io_2 = sysml_value_0;
    *io_3 = sysml_value_1;
    *io_4 = sysml_value_2;
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
static double hw_value_io_3 = 0;
static double hw_value_io_4 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, OUTPUT);
  analogWrite(4, (int)constrain((hw_value_io_2 * 255 + 0.0), 0.0, 255.0));
  pinMode(5, OUTPUT);
  analogWrite(5, (int)constrain((hw_value_io_3 * 255 + 0.0), 0.0, 255.0));
  pinMode(6, OUTPUT);
  analogWrite(6, (int)constrain((hw_value_io_4 * 255 + 0.0), 0.0, 255.0));
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
  ProgramPage1(hw_value_io_0, hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4);
  analogWrite(4, (int)constrain((hw_value_io_2 * 255 + 0.0), 0.0, 255.0));
  analogWrite(5, (int)constrain((hw_value_io_3 * 255 + 0.0), 0.0, 255.0));
  analogWrite(6, (int)constrain((hw_value_io_4 * 255 + 0.0), 0.0, 255.0));
}
