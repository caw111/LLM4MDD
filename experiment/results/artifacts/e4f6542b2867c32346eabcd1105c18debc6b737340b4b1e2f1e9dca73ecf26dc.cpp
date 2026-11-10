#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_64\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
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
extern int32_t sysml_state_colorIndex;
/* Global Variable define of  */
extern bool sysml_state_debouncedBtn1;
/* Global Variable define of  */
extern bool sysml_state_debouncedBtn2;
/* Global Variable define of  */
extern bool sysml_state_previousDebouncedBtn1;
/* Global Variable define of  */
extern bool sysml_state_previousDebouncedBtn2;
/* Global Variable define of  */
extern int32_t sysml_state_stableCounter1;
/* Global Variable define of  */
extern int32_t sysml_state_stableCounter2;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool* io_2, bool* io_3, bool* io_4);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_colorIndex = 0;
/* Global Variable define of  */
bool sysml_state_debouncedBtn1 = false;
/* Global Variable define of  */
bool sysml_state_debouncedBtn2 = false;
/* Global Variable define of  */
bool sysml_state_previousDebouncedBtn1 = false;
/* Global Variable define of  */
bool sysml_state_previousDebouncedBtn2 = false;
/* Global Variable define of  */
int32_t sysml_state_stableCounter1 = 0;
/* Global Variable define of  */
int32_t sysml_state_stableCounter2 = 0;
/* Init function of CompositeActor named "ProgramPage1" */
#line 260 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_64\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 272 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_64\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 53 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_64\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_colorIndex = 0;
    sysml_state_debouncedBtn1 = false;
    sysml_state_debouncedBtn2 = false;
    sysml_state_previousDebouncedBtn1 = false;
    sysml_state_previousDebouncedBtn2 = false;
    sysml_state_stableCounter1 = 0;
    sysml_state_stableCounter2 = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool* io_2, bool* io_3, bool* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    int32_t sysml_conditional_5 = 0;
    int32_t sysml_conditional_6 = 0;
    bool sysml_lazy_7 = 0;
    int32_t sysml_conditional_8 = 0;
    int32_t sysml_conditional_9 = 0;
    bool sysml_lazy_10 = 0;
    bool sysml_lazy_11 = 0;
    bool sysml_conditional_12 = 0;
    bool sysml_lazy_13 = 0;
    bool sysml_lazy_14 = 0;
    bool sysml_lazy_15 = 0;
    bool sysml_conditional_16 = 0;
    bool sysml_lazy_17 = 0;
    bool sysml_lazy_18 = 0;
    bool sysml_lazy_19 = 0;
    bool sysml_conditional_20 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_debouncedBtn1;
    }
    if (sysml_lazy_0) {
        sysml_state_stableCounter1 = (sysml_state_stableCounter1 + 1);
        if (((sysml_state_stableCounter1 + 1) >= 2)) {
            sysml_state_debouncedBtn1 = true;
        }
    }
    else {
        sysml_lazy_1 = !io_0;
        if (sysml_lazy_1) {
            sysml_lazy_1 = sysml_state_debouncedBtn1;
        }
        if (sysml_lazy_1) {
            sysml_state_stableCounter1 = (sysml_state_stableCounter1 + 1);
            if (((sysml_state_stableCounter1 + 1) >= 2)) {
                sysml_state_debouncedBtn1 = false;
            }
        }
        else {
            sysml_state_stableCounter1 = 0;
        }
    }
    sysml_lazy_2 = io_1;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_debouncedBtn2;
    }
    if (sysml_lazy_2) {
        sysml_state_stableCounter2 = (sysml_state_stableCounter2 + 1);
        if (((sysml_state_stableCounter2 + 1) >= 2)) {
            sysml_state_debouncedBtn2 = true;
        }
    }
    else {
        sysml_lazy_3 = !io_1;
        if (sysml_lazy_3) {
            sysml_lazy_3 = sysml_state_debouncedBtn2;
        }
        if (sysml_lazy_3) {
            sysml_state_stableCounter2 = (sysml_state_stableCounter2 + 1);
            if (((sysml_state_stableCounter2 + 1) >= 2)) {
                sysml_state_debouncedBtn2 = false;
            }
        }
        else {
            sysml_state_stableCounter2 = 0;
        }
    }
    sysml_lazy_4 = sysml_state_debouncedBtn1;
    if (sysml_lazy_4) {
        sysml_lazy_4 = !sysml_state_previousDebouncedBtn1;
    }
    if (sysml_lazy_4) {
        if (sysml_state_colorIndex == 7) {
            sysml_conditional_5 = 0;
        }
        else {
            if (sysml_state_colorIndex == 0) {
                sysml_conditional_6 = 1;
            }
            else {
                sysml_conditional_6 = sysml_state_colorIndex + 1;
            }
            sysml_conditional_5 = sysml_conditional_6;
        }
        sysml_state_colorIndex = sysml_conditional_5;
    }
    sysml_lazy_7 = sysml_state_debouncedBtn2;
    if (sysml_lazy_7) {
        sysml_lazy_7 = !sysml_state_previousDebouncedBtn2;
    }
    if (sysml_lazy_7) {
        if (sysml_state_colorIndex == 1) {
            sysml_conditional_8 = 0;
        }
        else {
            if (sysml_state_colorIndex == 0) {
                sysml_conditional_9 = 7;
            }
            else {
                sysml_conditional_9 = sysml_state_colorIndex - 1;
            }
            sysml_conditional_8 = sysml_conditional_9;
        }
        sysml_state_colorIndex = sysml_conditional_8;
    }
    sysml_state_previousDebouncedBtn1 = sysml_state_debouncedBtn1;
    sysml_state_previousDebouncedBtn2 = sysml_state_debouncedBtn2;
    if ((sysml_state_colorIndex == 8)) {
        sysml_state_colorIndex = 0;
    }
    bool sysml_value_0 = 0;
    sysml_lazy_10 = sysml_state_colorIndex == 1;
    if (!sysml_lazy_10) {
        sysml_lazy_10 = sysml_state_colorIndex == 5;
    }
    sysml_lazy_11 = sysml_lazy_10;
    if (!sysml_lazy_11) {
        sysml_lazy_11 = sysml_state_colorIndex == 7;
    }
    if (sysml_lazy_11) {
        sysml_conditional_12 = true;
    }
    else {
        sysml_conditional_12 = false;
    }
    sysml_value_0 = sysml_conditional_12;
    bool sysml_value_1 = 0;
    sysml_lazy_13 = sysml_state_colorIndex == 2;
    if (!sysml_lazy_13) {
        sysml_lazy_13 = sysml_state_colorIndex == 4;
    }
    sysml_lazy_14 = sysml_lazy_13;
    if (!sysml_lazy_14) {
        sysml_lazy_14 = sysml_state_colorIndex == 5;
    }
    sysml_lazy_15 = sysml_lazy_14;
    if (!sysml_lazy_15) {
        sysml_lazy_15 = sysml_state_colorIndex == 7;
    }
    if (sysml_lazy_15) {
        sysml_conditional_16 = true;
    }
    else {
        sysml_conditional_16 = false;
    }
    sysml_value_1 = sysml_conditional_16;
    bool sysml_value_2 = 0;
    sysml_lazy_17 = sysml_state_colorIndex == 3;
    if (!sysml_lazy_17) {
        sysml_lazy_17 = sysml_state_colorIndex == 4;
    }
    sysml_lazy_18 = sysml_lazy_17;
    if (!sysml_lazy_18) {
        sysml_lazy_18 = sysml_state_colorIndex == 6;
    }
    sysml_lazy_19 = sysml_lazy_18;
    if (!sysml_lazy_19) {
        sysml_lazy_19 = sysml_state_colorIndex == 7;
    }
    if (sysml_lazy_19) {
        sysml_conditional_20 = true;
    }
    else {
        sysml_conditional_20 = false;
    }
    sysml_value_2 = sysml_conditional_20;
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
static bool hw_value_io_2 = false;
static bool hw_value_io_3 = false;
static bool hw_value_io_4 = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, OUTPUT);
  digitalWrite(4, hw_value_io_2 ? HIGH : LOW);
  pinMode(5, OUTPUT);
  digitalWrite(5, hw_value_io_3 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  digitalWrite(6, hw_value_io_4 ? HIGH : LOW);
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
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 5UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = (digitalRead(3) == HIGH);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4);
  digitalWrite(4, hw_value_io_2 ? HIGH : LOW);
  digitalWrite(5, hw_value_io_3 ? HIGH : LOW);
  digitalWrite(6, hw_value_io_4 ? HIGH : LOW);
}

