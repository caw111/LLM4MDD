#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_75\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
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
extern bool sysml_state_previousPressed;
/* Global Variable define of  */
extern int32_t sysml_state_pressedStableMs;
/* Global Variable define of  */
extern bool sysml_state_activeSourcePot1;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, double io_1, double io_2, int32_t io_3, bool* io_4_segment_0, bool* io_4_segment_1, bool* io_4_segment_2, bool* io_4_segment_3, bool* io_4_segment_4, bool* io_4_segment_5, bool* io_4_segment_6, bool* io_4_segment_7, bool* io_4_segment_8, bool* io_4_segment_9);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
int32_t sysml_state_pressedStableMs = 0;
/* Global Variable define of  */
bool sysml_state_activeSourcePot1 = true;
/* Init function of CompositeActor named "ProgramPage1" */
#line 256 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_75\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void setup();
#line 283 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_75\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void loop();
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_75\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousPressed = false;
    sysml_state_pressedStableMs = 0;
    sysml_state_activeSourcePot1 = true;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, double io_1, double io_2, int32_t io_3, bool* io_4_segment_0, bool* io_4_segment_1, bool* io_4_segment_2, bool* io_4_segment_3, bool* io_4_segment_4, bool* io_4_segment_5, bool* io_4_segment_6, bool* io_4_segment_7, bool* io_4_segment_8, bool* io_4_segment_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_conditional_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    bool sysml_conditional_9 = 0;
    bool sysml_lazy_10 = 0;
    bool sysml_conditional_11 = 0;
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
    if (io_0) {
        sysml_state_pressedStableMs = (sysml_state_pressedStableMs + io_3);
    }
    else {
        sysml_state_pressedStableMs = 0;
    }
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    sysml_lazy_1 = sysml_lazy_0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = sysml_state_pressedStableMs >= 150;
    }
    if (sysml_lazy_1) {
        sysml_state_activeSourcePot1 = (!sysml_state_activeSourcePot1);
    }
    sysml_state_previousPressed = io_0;
    bool sysml_value_0 = 0;
    sysml_lazy_2 = sysml_state_activeSourcePot1;
    if (sysml_lazy_2) {
        sysml_lazy_2 = io_1 > 0.0;
    }
    if (sysml_lazy_2) {
        sysml_lazy_4 = io_1 >= 102.3;
        if (sysml_lazy_4) {
            sysml_lazy_4 = true;
        }
        sysml_conditional_3 = sysml_lazy_4;
    }
    else {
        sysml_lazy_5 = !sysml_state_activeSourcePot1;
        if (sysml_lazy_5) {
            sysml_lazy_5 = io_2 > 0.0;
        }
        if (sysml_lazy_5) {
            sysml_lazy_7 = io_2 >= 102.3;
            if (sysml_lazy_7) {
                sysml_lazy_7 = true;
            }
            sysml_conditional_6 = sysml_lazy_7;
        }
        else {
            sysml_lazy_8 = sysml_state_activeSourcePot1;
            if (sysml_lazy_8) {
                sysml_lazy_8 = io_1 == 0.0;
            }
            if (sysml_lazy_8) {
                sysml_conditional_9 = false;
            }
            else {
                sysml_lazy_10 = !sysml_state_activeSourcePot1;
                if (sysml_lazy_10) {
                    sysml_lazy_10 = io_2 == 0.0;
                }
                if (sysml_lazy_10) {
                    sysml_conditional_11 = false;
                }
                else {
                    if (sysml_state_activeSourcePot1) {
                        sysml_conditional_12 = io_1 >= 102.3;
                    }
                    else {
                        sysml_conditional_12 = io_2 >= 102.3;
                    }
                    sysml_conditional_11 = sysml_conditional_12;
                }
                sysml_conditional_9 = sysml_conditional_11;
            }
            sysml_conditional_6 = sysml_conditional_9;
        }
        sysml_conditional_3 = sysml_conditional_6;
    }
    sysml_value_0 = sysml_conditional_3;
    bool sysml_value_1 = 0;
    if (sysml_state_activeSourcePot1) {
        sysml_conditional_13 = io_1 >= 204.6;
    }
    else {
        sysml_conditional_13 = io_2 >= 204.6;
    }
    sysml_value_1 = sysml_conditional_13;
    bool sysml_value_2 = 0;
    if (sysml_state_activeSourcePot1) {
        sysml_conditional_14 = io_1 >= 307.0;
    }
    else {
        sysml_conditional_14 = io_2 >= 307.0;
    }
    sysml_value_2 = sysml_conditional_14;
    bool sysml_value_3 = 0;
    if (sysml_state_activeSourcePot1) {
        sysml_conditional_15 = io_1 >= 409.2;
    }
    else {
        sysml_conditional_15 = io_2 >= 409.2;
    }
    sysml_value_3 = sysml_conditional_15;
    bool sysml_value_4 = 0;
    if (sysml_state_activeSourcePot1) {
        sysml_conditional_16 = io_1 >= 511.5;
    }
    else {
        sysml_conditional_16 = io_2 >= 511.5;
    }
    sysml_value_4 = sysml_conditional_16;
    bool sysml_value_5 = 0;
    if (sysml_state_activeSourcePot1) {
        sysml_conditional_17 = io_1 >= 613.8;
    }
    else {
        sysml_conditional_17 = io_2 >= 613.8;
    }
    sysml_value_5 = sysml_conditional_17;
    bool sysml_value_6 = 0;
    if (sysml_state_activeSourcePot1) {
        sysml_conditional_18 = io_1 >= 716.1;
    }
    else {
        sysml_conditional_18 = io_2 >= 716.1;
    }
    sysml_value_6 = sysml_conditional_18;
    bool sysml_value_7 = 0;
    if (sysml_state_activeSourcePot1) {
        sysml_conditional_19 = io_1 >= 818.4;
    }
    else {
        sysml_conditional_19 = io_2 >= 818.4;
    }
    sysml_value_7 = sysml_conditional_19;
    bool sysml_value_8 = 0;
    if (sysml_state_activeSourcePot1) {
        sysml_conditional_20 = io_1 >= 920.7;
    }
    else {
        sysml_conditional_20 = io_2 >= 920.7;
    }
    sysml_value_8 = sysml_conditional_20;
    bool sysml_value_9 = 0;
    if (sysml_state_activeSourcePot1) {
        sysml_conditional_21 = io_1 >= 1023.0;
    }
    else {
        sysml_conditional_21 = io_2 >= 1023.0;
    }
    sysml_value_9 = sysml_conditional_21;
    *io_4_segment_0 = sysml_value_0;
    *io_4_segment_1 = sysml_value_1;
    *io_4_segment_2 = sysml_value_2;
    *io_4_segment_3 = sysml_value_3;
    *io_4_segment_4 = sysml_value_4;
    *io_4_segment_5 = sysml_value_5;
    *io_4_segment_6 = sysml_value_6;
    *io_4_segment_7 = sysml_value_7;
    *io_4_segment_8 = sysml_value_8;
    *io_4_segment_9 = sysml_value_9;
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
static double hw_value_io_1 = 0;
static uint32_t hw_sampled_io_1 = 0;
static bool hw_attempted_io_1 = false;
static double hw_value_io_2 = 0;
static uint32_t hw_sampled_io_2 = 0;
static bool hw_attempted_io_2 = false;
static int32_t hw_value_io_3 = 0;
static bool hw_value_io_4_segment_0 = false;
static bool hw_value_io_4_segment_1 = false;
static bool hw_value_io_4_segment_2 = false;
static bool hw_value_io_4_segment_3 = false;
static bool hw_value_io_4_segment_4 = false;
static bool hw_value_io_4_segment_5 = false;
static bool hw_value_io_4_segment_6 = false;
static bool hw_value_io_4_segment_7 = false;
static bool hw_value_io_4_segment_8 = false;
static bool hw_value_io_4_segment_9 = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, INPUT_PULLUP);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(3, OUTPUT);
  digitalWrite(3, hw_value_io_4_segment_0 ? HIGH : LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, hw_value_io_4_segment_1 ? HIGH : LOW);
  pinMode(5, OUTPUT);
  digitalWrite(5, hw_value_io_4_segment_2 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  digitalWrite(6, hw_value_io_4_segment_3 ? HIGH : LOW);
  pinMode(7, OUTPUT);
  digitalWrite(7, hw_value_io_4_segment_4 ? HIGH : LOW);
  pinMode(8, OUTPUT);
  digitalWrite(8, hw_value_io_4_segment_5 ? HIGH : LOW);
  pinMode(9, OUTPUT);
  digitalWrite(9, hw_value_io_4_segment_6 ? HIGH : LOW);
  pinMode(10, OUTPUT);
  digitalWrite(10, hw_value_io_4_segment_7 ? HIGH : LOW);
  pinMode(11, OUTPUT);
  digitalWrite(11, hw_value_io_4_segment_8 ? HIGH : LOW);
  pinMode(12, OUTPUT);
  digitalWrite(12, hw_value_io_4_segment_9 ? HIGH : LOW);
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
    hw_value_io_1 = (analogRead(A3) * 1 + 0.0);
  }
  if (!hw_attempted_io_2 || (uint32_t)(hw_now - hw_sampled_io_2) >= 5UL) {
    hw_attempted_io_2 = true; hw_sampled_io_2 = hw_now;
    hw_value_io_2 = (analogRead(A4) * 1 + 0.0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_3 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, hw_value_io_3, &hw_value_io_4_segment_0, &hw_value_io_4_segment_1, &hw_value_io_4_segment_2, &hw_value_io_4_segment_3, &hw_value_io_4_segment_4, &hw_value_io_4_segment_5, &hw_value_io_4_segment_6, &hw_value_io_4_segment_7, &hw_value_io_4_segment_8, &hw_value_io_4_segment_9);
  digitalWrite(3, hw_value_io_4_segment_0 ? HIGH : LOW);
  digitalWrite(4, hw_value_io_4_segment_1 ? HIGH : LOW);
  digitalWrite(5, hw_value_io_4_segment_2 ? HIGH : LOW);
  digitalWrite(6, hw_value_io_4_segment_3 ? HIGH : LOW);
  digitalWrite(7, hw_value_io_4_segment_4 ? HIGH : LOW);
  digitalWrite(8, hw_value_io_4_segment_5 ? HIGH : LOW);
  digitalWrite(9, hw_value_io_4_segment_6 ? HIGH : LOW);
  digitalWrite(10, hw_value_io_4_segment_7 ? HIGH : LOW);
  digitalWrite(11, hw_value_io_4_segment_8 ? HIGH : LOW);
  digitalWrite(12, hw_value_io_4_segment_9 ? HIGH : LOW);
}

