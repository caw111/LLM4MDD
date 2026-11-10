#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_117\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
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
extern bool sysml_state_led1Stable;
/* Global Variable define of  */
extern bool sysml_state_led2Stable;
/* Global Variable define of  */
extern bool sysml_state_prevBtn1;
/* Global Variable define of  */
extern bool sysml_state_prevBtn2;
/* Global Variable define of  */
extern bool sysml_state_prevBtn3;
/* Global Variable define of  */
extern int32_t sysml_state_btn1DebounceMs;
/* Global Variable define of  */
extern int32_t sysml_state_btn2DebounceMs;
/* Global Variable define of  */
extern int32_t sysml_state_btn3DebounceMs;
/* Global Variable define of  */
extern int32_t sysml_state_btn1HoldMs;
/* Global Variable define of  */
extern int32_t sysml_state_btn2HoldMs;
/* Global Variable define of  */
extern int32_t sysml_state_btn3HoldMs;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_2, bool io_3, bool io_4, int32_t io_5, bool* io_0, bool* io_1);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_led1Stable = false;
/* Global Variable define of  */
bool sysml_state_led2Stable = false;
/* Global Variable define of  */
bool sysml_state_prevBtn1 = false;
/* Global Variable define of  */
bool sysml_state_prevBtn2 = false;
/* Global Variable define of  */
bool sysml_state_prevBtn3 = false;
/* Global Variable define of  */
int32_t sysml_state_btn1DebounceMs = 0;
/* Global Variable define of  */
int32_t sysml_state_btn2DebounceMs = 0;
/* Global Variable define of  */
int32_t sysml_state_btn3DebounceMs = 0;
/* Global Variable define of  */
int32_t sysml_state_btn1HoldMs = 0;
/* Global Variable define of  */
int32_t sysml_state_btn2HoldMs = 0;
/* Global Variable define of  */
int32_t sysml_state_btn3HoldMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
#line 239 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_117\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void setup();
#line 250 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_117\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void loop();
#line 69 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_117\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_led1Stable = false;
    sysml_state_led2Stable = false;
    sysml_state_prevBtn1 = false;
    sysml_state_prevBtn2 = false;
    sysml_state_prevBtn3 = false;
    sysml_state_btn1DebounceMs = 0;
    sysml_state_btn2DebounceMs = 0;
    sysml_state_btn3DebounceMs = 0;
    sysml_state_btn1HoldMs = 0;
    sysml_state_btn2HoldMs = 0;
    sysml_state_btn3HoldMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_2, bool io_3, bool io_4, int32_t io_5, bool* io_0, bool* io_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_conditional_0 = 0;
    bool sysml_conditional_1 = 0;
    bool sysml_conditional_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_lazy_10 = 0;
    if (io_2) {
        sysml_conditional_0 = true;
    }
    else {
        sysml_conditional_0 = false;
    }
    if (sysml_conditional_0) {
        sysml_state_btn1DebounceMs = (sysml_state_btn1DebounceMs + io_5);
    }
    else {
        sysml_state_btn1DebounceMs = 0;
    }
    if (io_3) {
        sysml_conditional_1 = true;
    }
    else {
        sysml_conditional_1 = false;
    }
    if (sysml_conditional_1) {
        sysml_state_btn2DebounceMs = (sysml_state_btn2DebounceMs + io_5);
    }
    else {
        sysml_state_btn2DebounceMs = 0;
    }
    if (io_4) {
        sysml_conditional_2 = true;
    }
    else {
        sysml_conditional_2 = false;
    }
    if (sysml_conditional_2) {
        sysml_state_btn3DebounceMs = (sysml_state_btn3DebounceMs + io_5);
    }
    else {
        sysml_state_btn3DebounceMs = 0;
    }
    if ((sysml_state_btn1HoldMs >= 1)) {
        sysml_state_btn1HoldMs = (sysml_state_btn1HoldMs - io_5);
    }
    else {
        sysml_state_btn1HoldMs = 0;
    }
    if ((sysml_state_btn2HoldMs >= 1)) {
        sysml_state_btn2HoldMs = (sysml_state_btn2HoldMs - io_5);
    }
    else {
        sysml_state_btn2HoldMs = 0;
    }
    if ((sysml_state_btn3HoldMs >= 1)) {
        sysml_state_btn3HoldMs = (sysml_state_btn3HoldMs - io_5);
    }
    else {
        sysml_state_btn3HoldMs = 0;
    }
    sysml_lazy_3 = io_2;
    if (sysml_lazy_3) {
        sysml_lazy_3 = !sysml_state_prevBtn1;
    }
    sysml_lazy_4 = sysml_lazy_3;
    if (sysml_lazy_4) {
        sysml_lazy_4 = sysml_state_btn1DebounceMs >= 20;
    }
    if (sysml_lazy_4) {
        sysml_state_led1Stable = (!sysml_state_led1Stable);
        sysml_state_btn1HoldMs = 2000;
    }
    sysml_lazy_5 = io_3;
    if (sysml_lazy_5) {
        sysml_lazy_5 = !sysml_state_prevBtn2;
    }
    sysml_lazy_6 = sysml_lazy_5;
    if (sysml_lazy_6) {
        sysml_lazy_6 = sysml_state_btn2DebounceMs >= 20;
    }
    if (sysml_lazy_6) {
        sysml_state_led2Stable = (!sysml_state_led2Stable);
        sysml_state_btn2HoldMs = 2000;
    }
    sysml_lazy_7 = io_4;
    if (sysml_lazy_7) {
        sysml_lazy_7 = !sysml_state_prevBtn3;
    }
    sysml_lazy_8 = sysml_lazy_7;
    if (sysml_lazy_8) {
        sysml_lazy_8 = sysml_state_btn3DebounceMs >= 20;
    }
    if (sysml_lazy_8) {
        sysml_state_led1Stable = false;
        sysml_state_led2Stable = false;
        sysml_state_btn3HoldMs = 2000;
    }
    if ((sysml_state_btn3HoldMs >= 1)) {
        sysml_state_led1Stable = false;
        sysml_state_led2Stable = false;
    }
    sysml_lazy_9 = sysml_state_btn1HoldMs >= 1;
    if (sysml_lazy_9) {
        sysml_lazy_9 = sysml_state_btn3HoldMs >= 1;
    }
    if (sysml_lazy_9) {
        sysml_state_led1Stable = false;
    }
    sysml_lazy_10 = sysml_state_btn2HoldMs >= 1;
    if (sysml_lazy_10) {
        sysml_lazy_10 = sysml_state_btn3HoldMs >= 1;
    }
    if (sysml_lazy_10) {
        sysml_state_led2Stable = false;
    }
    sysml_state_prevBtn1 = io_2;
    sysml_state_prevBtn2 = io_3;
    sysml_state_prevBtn3 = io_4;
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_led1Stable;
    bool sysml_value_1 = 0;
    sysml_value_1 = sysml_state_led2Stable;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
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
static bool hw_value_io_1 = false;
static bool hw_value_io_2 = false;
static uint32_t hw_sampled_io_2 = 0;
static bool hw_attempted_io_2 = false;
static bool hw_value_io_3 = false;
static uint32_t hw_sampled_io_3 = 0;
static bool hw_attempted_io_3 = false;
static bool hw_value_io_4 = false;
static uint32_t hw_sampled_io_4 = 0;
static bool hw_attempted_io_4 = false;
static int32_t hw_value_io_5 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, OUTPUT);
  digitalWrite(2, hw_value_io_0 ? HIGH : LOW);
  pinMode(3, OUTPUT);
  digitalWrite(3, hw_value_io_1 ? HIGH : LOW);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_2 || (uint32_t)(hw_now - hw_sampled_io_2) >= 5UL) {
    hw_attempted_io_2 = true; hw_sampled_io_2 = hw_now;
    hw_value_io_2 = (digitalRead(4) == LOW);
  }
  if (!hw_attempted_io_3 || (uint32_t)(hw_now - hw_sampled_io_3) >= 5UL) {
    hw_attempted_io_3 = true; hw_sampled_io_3 = hw_now;
    hw_value_io_3 = (digitalRead(5) == LOW);
  }
  if (!hw_attempted_io_4 || (uint32_t)(hw_now - hw_sampled_io_4) >= 5UL) {
    hw_attempted_io_4 = true; hw_sampled_io_4 = hw_now;
    hw_value_io_4 = (digitalRead(6) == LOW);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_5 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_2, hw_value_io_3, hw_value_io_4, hw_value_io_5, &hw_value_io_0, &hw_value_io_1);
  digitalWrite(2, hw_value_io_0 ? HIGH : LOW);
  digitalWrite(3, hw_value_io_1 ? HIGH : LOW);
}

