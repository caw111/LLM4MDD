#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_85\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
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
extern int32_t sysml_state_mode1Index;
/* Global Variable define of  */
extern int32_t sysml_state_mode2Index;
/* Global Variable define of  */
extern bool sysml_state_rgbOff;
/* Global Variable define of  */
extern bool sysml_state_prevIo0;
/* Global Variable define of  */
extern bool sysml_state_prevIo1;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double io_2, bool* io_3, bool* io_4, bool* io_5);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_mode1Index = 0;
/* Global Variable define of  */
int32_t sysml_state_mode2Index = 0;
/* Global Variable define of  */
bool sysml_state_rgbOff = false;
/* Global Variable define of  */
bool sysml_state_prevIo0 = false;
/* Global Variable define of  */
bool sysml_state_prevIo1 = false;
/* Init function of CompositeActor named "ProgramPage1" */
#line 255 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_85\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 268 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_85\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 45 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_85\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_mode1Index = 0;
    sysml_state_mode2Index = 0;
    sysml_state_rgbOff = false;
    sysml_state_prevIo0 = false;
    sysml_state_prevIo1 = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double io_2, bool* io_3, bool* io_4, bool* io_5) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    bool sysml_conditional_9 = 0;
    bool sysml_conditional_10 = 0;
    bool sysml_conditional_11 = 0;
    bool sysml_conditional_12 = 0;
    bool sysml_conditional_13 = 0;
    bool sysml_conditional_14 = 0;
    bool sysml_conditional_15 = 0;
    bool sysml_conditional_16 = 0;
    bool sysml_conditional_17 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_prevIo0;
    }
    if (sysml_lazy_0) {
        if ((!sysml_state_rgbOff)) {
            if ((sysml_state_mode1Index == 0)) {
                sysml_state_mode1Index = 1;
            }
            else {
                if ((sysml_state_mode1Index == 1)) {
                    sysml_state_mode1Index = 2;
                }
                else {
                    sysml_state_mode1Index = 0;
                }
            }
        }
        else {
            sysml_state_mode1Index = 0;
        }
    }
    sysml_lazy_1 = io_1;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_prevIo1;
    }
    if (sysml_lazy_1) {
        if ((!sysml_state_rgbOff)) {
            if ((sysml_state_mode2Index == 0)) {
                sysml_state_mode2Index = 1;
            }
            else {
                if ((sysml_state_mode2Index == 1)) {
                    sysml_state_mode2Index = 2;
                }
                else {
                    sysml_state_mode2Index = 0;
                }
            }
        }
        else {
            sysml_state_mode2Index = 0;
        }
    }
    sysml_lazy_2 = !sysml_state_rgbOff;
    if (sysml_lazy_2) {
        sysml_lazy_2 = io_1;
    }
    if (sysml_lazy_2) {
        sysml_lazy_3 = sysml_state_mode1Index == 1;
        if (!sysml_lazy_3) {
            sysml_lazy_3 = sysml_state_mode1Index == 2;
        }
        if (sysml_lazy_3) {
        }
        else {
            if ((sysml_state_mode1Index == 0)) {
                sysml_state_rgbOff = true;
            }
        }
    }
    sysml_lazy_4 = !sysml_state_rgbOff;
    if (sysml_lazy_4) {
        sysml_lazy_4 = io_0;
    }
    if (sysml_lazy_4) {
        sysml_lazy_5 = sysml_state_mode2Index == 0;
        if (!sysml_lazy_5) {
            sysml_lazy_5 = sysml_state_mode2Index == 1;
        }
        sysml_lazy_6 = sysml_lazy_5;
        if (!sysml_lazy_6) {
            sysml_lazy_6 = sysml_state_mode2Index == 2;
        }
        if (sysml_lazy_6) {
            if ((sysml_state_mode2Index == 0)) {
                sysml_state_rgbOff = true;
            }
        }
    }
    sysml_lazy_7 = sysml_state_rgbOff;
    if (sysml_lazy_7) {
        sysml_lazy_8 = io_0;
        if (!sysml_lazy_8) {
            sysml_lazy_8 = io_1;
        }
        sysml_lazy_7 = sysml_lazy_8;
    }
    if (sysml_lazy_7) {
        sysml_state_mode1Index = 0;
        sysml_state_mode2Index = 0;
    }
    sysml_state_prevIo0 = io_0;
    sysml_state_prevIo1 = io_1;
    bool sysml_value_0 = 0;
    if (sysml_state_rgbOff) {
        sysml_conditional_9 = false;
    }
    else {
        if (sysml_state_mode2Index == 0) {
            sysml_conditional_10 = true;
        }
        else {
            if (sysml_state_mode1Index == 0) {
                sysml_conditional_11 = true;
            }
            else {
                sysml_conditional_11 = false;
            }
            sysml_conditional_10 = sysml_conditional_11;
        }
        sysml_conditional_9 = sysml_conditional_10;
    }
    sysml_value_0 = sysml_conditional_9;
    bool sysml_value_1 = 0;
    if (sysml_state_rgbOff) {
        sysml_conditional_12 = false;
    }
    else {
        if (sysml_state_mode2Index == 1) {
            sysml_conditional_13 = true;
        }
        else {
            if (sysml_state_mode1Index == 1) {
                sysml_conditional_14 = true;
            }
            else {
                sysml_conditional_14 = false;
            }
            sysml_conditional_13 = sysml_conditional_14;
        }
        sysml_conditional_12 = sysml_conditional_13;
    }
    sysml_value_1 = sysml_conditional_12;
    bool sysml_value_2 = 0;
    if (sysml_state_rgbOff) {
        sysml_conditional_15 = false;
    }
    else {
        if (sysml_state_mode2Index == 2) {
            sysml_conditional_16 = true;
        }
        else {
            if (sysml_state_mode1Index == 2) {
                sysml_conditional_17 = true;
            }
            else {
                sysml_conditional_17 = false;
            }
            sysml_conditional_16 = sysml_conditional_17;
        }
        sysml_conditional_15 = sysml_conditional_16;
    }
    sysml_value_2 = sysml_conditional_15;
    *io_3 = sysml_value_0;
    *io_4 = sysml_value_1;
    *io_5 = sysml_value_2;
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
static bool hw_value_io_3 = true;
static bool hw_value_io_4 = false;
static bool hw_value_io_5 = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(A3, INPUT);
  pinMode(4, OUTPUT);
  digitalWrite(4, hw_value_io_3 ? HIGH : LOW);
  pinMode(5, OUTPUT);
  digitalWrite(5, hw_value_io_4 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  digitalWrite(6, hw_value_io_5 ? HIGH : LOW);
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
  if (!hw_attempted_io_2 || (uint32_t)(hw_now - hw_sampled_io_2) >= 5UL) {
    hw_attempted_io_2 = true; hw_sampled_io_2 = hw_now;
    hw_value_io_2 = (analogRead(A3) * 1.0 + 0.0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5);
  digitalWrite(4, hw_value_io_3 ? HIGH : LOW);
  digitalWrite(5, hw_value_io_4 ? HIGH : LOW);
  digitalWrite(6, hw_value_io_5 ? HIGH : LOW);
}

