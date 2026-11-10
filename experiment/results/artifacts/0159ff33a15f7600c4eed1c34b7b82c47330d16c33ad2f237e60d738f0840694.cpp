#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_122\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
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
extern bool sysml_state_btn1PrevPressed;
/* Global Variable define of  */
extern bool sysml_state_btn2PrevPressed;
/* Global Variable define of  */
extern bool sysml_state_btn3PrevPressed;
/* Global Variable define of  */
extern bool sysml_state_btn1StablePressed;
/* Global Variable define of  */
extern bool sysml_state_btn2StablePressed;
/* Global Variable define of  */
extern bool sysml_state_btn3StablePressed;
/* Global Variable define of  */
extern int32_t sysml_state_btn1StableTimerMs;
/* Global Variable define of  */
extern int32_t sysml_state_btn2StableTimerMs;
/* Global Variable define of  */
extern int32_t sysml_state_btn3StableTimerMs;
/* Global Variable define of  */
extern bool sysml_state_valueBit0;
/* Global Variable define of  */
extern bool sysml_state_valueBit1;
/* Global Variable define of  */
extern bool sysml_state_valueBit2;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6_segment_0, bool* io_6_segment_1, bool* io_6_segment_2, bool* io_6_segment_3, bool* io_6_segment_4, bool* io_6_segment_5, bool* io_6_segment_6, bool* io_6_segment_7);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_btn1PrevPressed = false;
/* Global Variable define of  */
bool sysml_state_btn2PrevPressed = false;
/* Global Variable define of  */
bool sysml_state_btn3PrevPressed = false;
/* Global Variable define of  */
bool sysml_state_btn1StablePressed = false;
/* Global Variable define of  */
bool sysml_state_btn2StablePressed = false;
/* Global Variable define of  */
bool sysml_state_btn3StablePressed = false;
/* Global Variable define of  */
int32_t sysml_state_btn1StableTimerMs = 0;
/* Global Variable define of  */
int32_t sysml_state_btn2StableTimerMs = 0;
/* Global Variable define of  */
int32_t sysml_state_btn3StableTimerMs = 0;
/* Global Variable define of  */
bool sysml_state_valueBit0 = false;
/* Global Variable define of  */
bool sysml_state_valueBit1 = false;
/* Global Variable define of  */
bool sysml_state_valueBit2 = false;
/* Init function of CompositeActor named "ProgramPage1" */
#line 382 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_122\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 411 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_122\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 73 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_122\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_btn1PrevPressed = false;
    sysml_state_btn2PrevPressed = false;
    sysml_state_btn3PrevPressed = false;
    sysml_state_btn1StablePressed = false;
    sysml_state_btn2StablePressed = false;
    sysml_state_btn3StablePressed = false;
    sysml_state_btn1StableTimerMs = 0;
    sysml_state_btn2StableTimerMs = 0;
    sysml_state_btn3StableTimerMs = 0;
    sysml_state_valueBit0 = false;
    sysml_state_valueBit1 = false;
    sysml_state_valueBit2 = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6_segment_0, bool* io_6_segment_1, bool* io_6_segment_2, bool* io_6_segment_3, bool* io_6_segment_4, bool* io_6_segment_5, bool* io_6_segment_6, bool* io_6_segment_7) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_conditional_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_lazy_10 = 0;
    bool sysml_conditional_11 = 0;
    bool sysml_lazy_12 = 0;
    bool sysml_lazy_13 = 0;
    bool sysml_conditional_14 = 0;
    bool sysml_lazy_15 = 0;
    bool sysml_lazy_16 = 0;
    bool sysml_conditional_17 = 0;
    bool sysml_lazy_18 = 0;
    bool sysml_lazy_19 = 0;
    bool sysml_conditional_20 = 0;
    bool sysml_lazy_21 = 0;
    bool sysml_lazy_22 = 0;
    bool sysml_conditional_23 = 0;
    bool sysml_lazy_24 = 0;
    bool sysml_lazy_25 = 0;
    bool sysml_conditional_26 = 0;
    bool sysml_lazy_27 = 0;
    bool sysml_lazy_28 = 0;
    bool sysml_conditional_29 = 0;
    if ((!io_0)) {
        sysml_state_btn1StablePressed = false;
        sysml_state_btn1StableTimerMs = 0;
    }
    else {
        if (sysml_state_btn1StablePressed) {
        }
        else {
            sysml_state_btn1StableTimerMs = (sysml_state_btn1StableTimerMs + 5);
            if (((sysml_state_btn1StableTimerMs + 5) >= 150)) {
                sysml_state_btn1StablePressed = true;
            }
        }
    }
    if ((!io_1)) {
        sysml_state_btn2StablePressed = false;
        sysml_state_btn2StableTimerMs = 0;
    }
    else {
        if (sysml_state_btn2StablePressed) {
        }
        else {
            sysml_state_btn2StableTimerMs = (sysml_state_btn2StableTimerMs + 5);
            if (((sysml_state_btn2StableTimerMs + 5) >= 150)) {
                sysml_state_btn2StablePressed = true;
            }
        }
    }
    if ((!io_2)) {
        sysml_state_btn3StablePressed = false;
        sysml_state_btn3StableTimerMs = 0;
    }
    else {
        if (sysml_state_btn3StablePressed) {
        }
        else {
            sysml_state_btn3StableTimerMs = (sysml_state_btn3StableTimerMs + 5);
            if (((sysml_state_btn3StableTimerMs + 5) >= 150)) {
                sysml_state_btn3StablePressed = true;
            }
        }
    }
    sysml_lazy_0 = sysml_state_btn1StablePressed;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_btn1PrevPressed;
    }
    if (sysml_lazy_0) {
        if (sysml_state_valueBit0 == false) {
            sysml_conditional_1 = true;
        }
        else {
            sysml_conditional_1 = false;
        }
        sysml_state_valueBit0 = sysml_conditional_1;
    }
    sysml_lazy_2 = sysml_state_btn2StablePressed;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_btn2PrevPressed;
    }
    if (sysml_lazy_2) {
        if (sysml_state_valueBit1 == false) {
            sysml_conditional_3 = true;
        }
        else {
            sysml_conditional_3 = false;
        }
        sysml_state_valueBit1 = sysml_conditional_3;
    }
    sysml_lazy_4 = sysml_state_btn3StablePressed;
    if (sysml_lazy_4) {
        sysml_lazy_4 = !sysml_state_btn3PrevPressed;
    }
    if (sysml_lazy_4) {
        if (sysml_state_valueBit2 == false) {
            sysml_conditional_5 = true;
        }
        else {
            sysml_conditional_5 = false;
        }
        sysml_state_valueBit2 = sysml_conditional_5;
    }
    sysml_state_btn1PrevPressed = sysml_state_btn1StablePressed;
    sysml_state_btn2PrevPressed = sysml_state_btn2StablePressed;
    sysml_state_btn3PrevPressed = sysml_state_btn3StablePressed;
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_valueBit0;
    bool sysml_value_1 = 0;
    sysml_value_1 = sysml_state_valueBit1;
    bool sysml_value_2 = 0;
    sysml_value_2 = sysml_state_valueBit2;
    bool sysml_value_3 = 0;
    sysml_lazy_6 = !sysml_state_valueBit0;
    if (sysml_lazy_6) {
        sysml_lazy_6 = !sysml_state_valueBit1;
    }
    sysml_lazy_7 = sysml_lazy_6;
    if (sysml_lazy_7) {
        sysml_lazy_7 = !sysml_state_valueBit2;
    }
    if (sysml_lazy_7) {
        sysml_conditional_8 = true;
    }
    else {
        sysml_conditional_8 = false;
    }
    sysml_value_3 = sysml_conditional_8;
    bool sysml_value_4 = 0;
    sysml_lazy_9 = sysml_state_valueBit0;
    if (sysml_lazy_9) {
        sysml_lazy_9 = !sysml_state_valueBit1;
    }
    sysml_lazy_10 = sysml_lazy_9;
    if (sysml_lazy_10) {
        sysml_lazy_10 = !sysml_state_valueBit2;
    }
    if (sysml_lazy_10) {
        sysml_conditional_11 = true;
    }
    else {
        sysml_conditional_11 = false;
    }
    sysml_value_4 = sysml_conditional_11;
    bool sysml_value_5 = 0;
    sysml_lazy_12 = !sysml_state_valueBit0;
    if (sysml_lazy_12) {
        sysml_lazy_12 = sysml_state_valueBit1;
    }
    sysml_lazy_13 = sysml_lazy_12;
    if (sysml_lazy_13) {
        sysml_lazy_13 = !sysml_state_valueBit2;
    }
    if (sysml_lazy_13) {
        sysml_conditional_14 = true;
    }
    else {
        sysml_conditional_14 = false;
    }
    sysml_value_5 = sysml_conditional_14;
    bool sysml_value_6 = 0;
    sysml_lazy_15 = sysml_state_valueBit0;
    if (sysml_lazy_15) {
        sysml_lazy_15 = sysml_state_valueBit1;
    }
    sysml_lazy_16 = sysml_lazy_15;
    if (sysml_lazy_16) {
        sysml_lazy_16 = !sysml_state_valueBit2;
    }
    if (sysml_lazy_16) {
        sysml_conditional_17 = true;
    }
    else {
        sysml_conditional_17 = false;
    }
    sysml_value_6 = sysml_conditional_17;
    bool sysml_value_7 = 0;
    sysml_lazy_18 = !sysml_state_valueBit0;
    if (sysml_lazy_18) {
        sysml_lazy_18 = !sysml_state_valueBit1;
    }
    sysml_lazy_19 = sysml_lazy_18;
    if (sysml_lazy_19) {
        sysml_lazy_19 = sysml_state_valueBit2;
    }
    if (sysml_lazy_19) {
        sysml_conditional_20 = true;
    }
    else {
        sysml_conditional_20 = false;
    }
    sysml_value_7 = sysml_conditional_20;
    bool sysml_value_8 = 0;
    sysml_lazy_21 = sysml_state_valueBit0;
    if (sysml_lazy_21) {
        sysml_lazy_21 = !sysml_state_valueBit1;
    }
    sysml_lazy_22 = sysml_lazy_21;
    if (sysml_lazy_22) {
        sysml_lazy_22 = sysml_state_valueBit2;
    }
    if (sysml_lazy_22) {
        sysml_conditional_23 = true;
    }
    else {
        sysml_conditional_23 = false;
    }
    sysml_value_8 = sysml_conditional_23;
    bool sysml_value_9 = 0;
    sysml_lazy_24 = !sysml_state_valueBit0;
    if (sysml_lazy_24) {
        sysml_lazy_24 = sysml_state_valueBit1;
    }
    sysml_lazy_25 = sysml_lazy_24;
    if (sysml_lazy_25) {
        sysml_lazy_25 = sysml_state_valueBit2;
    }
    if (sysml_lazy_25) {
        sysml_conditional_26 = true;
    }
    else {
        sysml_conditional_26 = false;
    }
    sysml_value_9 = sysml_conditional_26;
    bool sysml_value_10 = 0;
    sysml_lazy_27 = sysml_state_valueBit0;
    if (sysml_lazy_27) {
        sysml_lazy_27 = sysml_state_valueBit1;
    }
    sysml_lazy_28 = sysml_lazy_27;
    if (sysml_lazy_28) {
        sysml_lazy_28 = sysml_state_valueBit2;
    }
    if (sysml_lazy_28) {
        sysml_conditional_29 = true;
    }
    else {
        sysml_conditional_29 = false;
    }
    sysml_value_10 = sysml_conditional_29;
    *io_3 = sysml_value_0;
    *io_4 = sysml_value_1;
    *io_5 = sysml_value_2;
    *io_6_segment_0 = sysml_value_3;
    *io_6_segment_1 = sysml_value_4;
    *io_6_segment_2 = sysml_value_5;
    *io_6_segment_3 = sysml_value_6;
    *io_6_segment_4 = sysml_value_7;
    *io_6_segment_5 = sysml_value_8;
    *io_6_segment_6 = sysml_value_9;
    *io_6_segment_7 = sysml_value_10;
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
static uint32_t hw_sampled_io_2 = 0;
static bool hw_attempted_io_2 = false;
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
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
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
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 5UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (digitalRead(5) == LOW);
  }
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 5UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = (digitalRead(6) == LOW);
  }
  if (!hw_attempted_io_2 || (uint32_t)(hw_now - hw_sampled_io_2) >= 5UL) {
    hw_attempted_io_2 = true; hw_sampled_io_2 = hw_now;
    hw_value_io_2 = (digitalRead(7) == LOW);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6_segment_0, &hw_value_io_6_segment_1, &hw_value_io_6_segment_2, &hw_value_io_6_segment_3, &hw_value_io_6_segment_4, &hw_value_io_6_segment_5, &hw_value_io_6_segment_6, &hw_value_io_6_segment_7);
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

