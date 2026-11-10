#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_117\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
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
extern bool sysml_state_led1State;
/* Global Variable define of  */
extern bool sysml_state_led2State;
/* Global Variable define of  */
extern bool sysml_state_prevBtn1;
/* Global Variable define of  */
extern bool sysml_state_prevBtn2;
/* Global Variable define of  */
extern bool sysml_state_prevBtn3;
/* Global Variable define of  */
extern int32_t sysml_state_debounceBtn1Ms;
/* Global Variable define of  */
extern int32_t sysml_state_debounceBtn2Ms;
/* Global Variable define of  */
extern int32_t sysml_state_debounceBtn3Ms;
/* Global Variable define of  */
extern bool sysml_state_stableBtn1;
/* Global Variable define of  */
extern bool sysml_state_stableBtn2;
/* Global Variable define of  */
extern bool sysml_state_stableBtn3;
/* Global Variable define of  */
extern int32_t sysml_state_holdBtn1RemainingMs;
/* Global Variable define of  */
extern int32_t sysml_state_holdBtn2RemainingMs;
/* Global Variable define of  */
extern int32_t sysml_state_holdBtn3RemainingMs;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_2, bool io_3, bool io_4, int32_t io_5, bool* io_0, bool* io_1);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_led1State = false;
/* Global Variable define of  */
bool sysml_state_led2State = false;
/* Global Variable define of  */
bool sysml_state_prevBtn1 = false;
/* Global Variable define of  */
bool sysml_state_prevBtn2 = false;
/* Global Variable define of  */
bool sysml_state_prevBtn3 = false;
/* Global Variable define of  */
int32_t sysml_state_debounceBtn1Ms = 0;
/* Global Variable define of  */
int32_t sysml_state_debounceBtn2Ms = 0;
/* Global Variable define of  */
int32_t sysml_state_debounceBtn3Ms = 0;
/* Global Variable define of  */
bool sysml_state_stableBtn1 = false;
/* Global Variable define of  */
bool sysml_state_stableBtn2 = false;
/* Global Variable define of  */
bool sysml_state_stableBtn3 = false;
/* Global Variable define of  */
int32_t sysml_state_holdBtn1RemainingMs = 0;
/* Global Variable define of  */
int32_t sysml_state_holdBtn2RemainingMs = 0;
/* Global Variable define of  */
int32_t sysml_state_holdBtn3RemainingMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
#line 263 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_117\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 274 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_117\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 81 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_117\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_led1State = false;
    sysml_state_led2State = false;
    sysml_state_prevBtn1 = false;
    sysml_state_prevBtn2 = false;
    sysml_state_prevBtn3 = false;
    sysml_state_debounceBtn1Ms = 0;
    sysml_state_debounceBtn2Ms = 0;
    sysml_state_debounceBtn3Ms = 0;
    sysml_state_stableBtn1 = false;
    sysml_state_stableBtn2 = false;
    sysml_state_stableBtn3 = false;
    sysml_state_holdBtn1RemainingMs = 0;
    sysml_state_holdBtn2RemainingMs = 0;
    sysml_state_holdBtn3RemainingMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_2, bool io_3, bool io_4, int32_t io_5, bool* io_0, bool* io_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int32_t sysml_conditional_0 = 0;
    bool sysml_conditional_1 = 0;
    int32_t sysml_conditional_2 = 0;
    bool sysml_conditional_3 = 0;
    int32_t sysml_conditional_4 = 0;
    bool sysml_conditional_5 = 0;
    int32_t sysml_conditional_6 = 0;
    int32_t sysml_conditional_7 = 0;
    int32_t sysml_conditional_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_lazy_10 = 0;
    bool sysml_lazy_11 = 0;
    bool sysml_lazy_12 = 0;
    bool sysml_lazy_13 = 0;
    if (io_2) {
        sysml_conditional_0 = sysml_state_debounceBtn1Ms + io_5;
    }
    else {
        sysml_conditional_0 = 0;
    }
    sysml_state_debounceBtn1Ms = sysml_conditional_0;
    if (io_2) {
        sysml_conditional_1 = sysml_state_debounceBtn1Ms >= 20;
    }
    else {
        sysml_conditional_1 = false;
    }
    sysml_state_stableBtn1 = sysml_conditional_1;
    if (io_3) {
        sysml_conditional_2 = sysml_state_debounceBtn2Ms + io_5;
    }
    else {
        sysml_conditional_2 = 0;
    }
    sysml_state_debounceBtn2Ms = sysml_conditional_2;
    if (io_3) {
        sysml_conditional_3 = sysml_state_debounceBtn2Ms >= 20;
    }
    else {
        sysml_conditional_3 = false;
    }
    sysml_state_stableBtn2 = sysml_conditional_3;
    if (io_4) {
        sysml_conditional_4 = sysml_state_debounceBtn3Ms + io_5;
    }
    else {
        sysml_conditional_4 = 0;
    }
    sysml_state_debounceBtn3Ms = sysml_conditional_4;
    if (io_4) {
        sysml_conditional_5 = sysml_state_debounceBtn3Ms >= 20;
    }
    else {
        sysml_conditional_5 = false;
    }
    sysml_state_stableBtn3 = sysml_conditional_5;
    if (sysml_state_holdBtn1RemainingMs > 0) {
        sysml_conditional_6 = sysml_state_holdBtn1RemainingMs - io_5;
    }
    else {
        sysml_conditional_6 = 0;
    }
    sysml_state_holdBtn1RemainingMs = sysml_conditional_6;
    if (sysml_state_holdBtn2RemainingMs > 0) {
        sysml_conditional_7 = sysml_state_holdBtn2RemainingMs - io_5;
    }
    else {
        sysml_conditional_7 = 0;
    }
    sysml_state_holdBtn2RemainingMs = sysml_conditional_7;
    if (sysml_state_holdBtn3RemainingMs > 0) {
        sysml_conditional_8 = sysml_state_holdBtn3RemainingMs - io_5;
    }
    else {
        sysml_conditional_8 = 0;
    }
    sysml_state_holdBtn3RemainingMs = sysml_conditional_8;
    sysml_lazy_9 = sysml_state_stableBtn3;
    if (sysml_lazy_9) {
        sysml_lazy_9 = !sysml_state_prevBtn3;
    }
    if (sysml_lazy_9) {
        sysml_state_led1State = false;
        sysml_state_led2State = false;
        sysml_state_holdBtn3RemainingMs = 2000;
    }
    sysml_lazy_10 = !(sysml_state_holdBtn3RemainingMs > 0);
    if (sysml_lazy_10) {
        sysml_lazy_10 = sysml_state_stableBtn1;
    }
    sysml_lazy_11 = sysml_lazy_10;
    if (sysml_lazy_11) {
        sysml_lazy_11 = !sysml_state_prevBtn1;
    }
    if (sysml_lazy_11) {
        sysml_state_led1State = (!sysml_state_led1State);
        sysml_state_holdBtn1RemainingMs = 2000;
    }
    sysml_lazy_12 = !(sysml_state_holdBtn3RemainingMs > 0);
    if (sysml_lazy_12) {
        sysml_lazy_12 = sysml_state_stableBtn2;
    }
    sysml_lazy_13 = sysml_lazy_12;
    if (sysml_lazy_13) {
        sysml_lazy_13 = !sysml_state_prevBtn2;
    }
    if (sysml_lazy_13) {
        sysml_state_led2State = (!sysml_state_led2State);
        sysml_state_holdBtn2RemainingMs = 2000;
    }
    if ((sysml_state_holdBtn3RemainingMs > 0)) {
        sysml_state_led1State = false;
        sysml_state_led2State = false;
    }
    if ((sysml_state_prevBtn1 != sysml_state_stableBtn1)) {
        sysml_state_prevBtn1 = sysml_state_stableBtn1;
    }
    else {
        sysml_state_prevBtn1 = sysml_state_stableBtn1;
    }
    if ((sysml_state_prevBtn2 != sysml_state_stableBtn2)) {
        sysml_state_prevBtn2 = sysml_state_stableBtn2;
    }
    else {
        sysml_state_prevBtn2 = sysml_state_stableBtn2;
    }
    if ((sysml_state_prevBtn3 != sysml_state_stableBtn3)) {
        sysml_state_prevBtn3 = sysml_state_stableBtn3;
    }
    else {
        sysml_state_prevBtn3 = sysml_state_stableBtn3;
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_led1State;
    bool sysml_value_1 = 0;
    sysml_value_1 = sysml_state_led2State;
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
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_2 || (uint32_t)(hw_now - hw_sampled_io_2) >= 10UL) {
    hw_attempted_io_2 = true; hw_sampled_io_2 = hw_now;
    hw_value_io_2 = (digitalRead(4) == HIGH);
  }
  if (!hw_attempted_io_3 || (uint32_t)(hw_now - hw_sampled_io_3) >= 10UL) {
    hw_attempted_io_3 = true; hw_sampled_io_3 = hw_now;
    hw_value_io_3 = (digitalRead(5) == HIGH);
  }
  if (!hw_attempted_io_4 || (uint32_t)(hw_now - hw_sampled_io_4) >= 10UL) {
    hw_attempted_io_4 = true; hw_sampled_io_4 = hw_now;
    hw_value_io_4 = (digitalRead(6) == HIGH);
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

