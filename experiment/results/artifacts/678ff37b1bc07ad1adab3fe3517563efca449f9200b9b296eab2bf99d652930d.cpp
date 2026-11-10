#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_123\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
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
extern int32_t sysml_state_counter;
/* Global Variable define of  */
extern int32_t sysml_state_overflow;
/* Global Variable define of  */
extern bool sysml_state_previousPressed;
/* Global Variable define of  */
extern bool sysml_state_debounceStable;
/* Global Variable define of  */
extern int32_t sysml_state_debounceCounterMs;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool* io_1, bool* io_2, bool* io_3_segment_0, bool* io_3_segment_1, bool* io_3_segment_2, bool* io_3_segment_3, bool* io_3_segment_4, bool* io_3_segment_5, bool* io_3_segment_6, bool* io_3_segment_7, bool* io_3_segment_8, bool* io_3_segment_9);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_counter = 0;
/* Global Variable define of  */
int32_t sysml_state_overflow = 0;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
bool sysml_state_debounceStable = false;
/* Global Variable define of  */
int32_t sysml_state_debounceCounterMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
#line 169 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_123\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void setup();
#line 198 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_123\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void loop();
#line 45 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_123\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_counter = 0;
    sysml_state_overflow = 0;
    sysml_state_previousPressed = false;
    sysml_state_debounceStable = false;
    sysml_state_debounceCounterMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool* io_1, bool* io_2, bool* io_3_segment_0, bool* io_3_segment_1, bool* io_3_segment_2, bool* io_3_segment_3, bool* io_3_segment_4, bool* io_3_segment_5, bool* io_3_segment_6, bool* io_3_segment_7, bool* io_3_segment_8, bool* io_3_segment_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    if (io_0) {
        if (sysml_state_previousPressed) {
            sysml_state_debounceCounterMs = (sysml_state_debounceCounterMs + 5);
        }
        else {
            sysml_state_debounceCounterMs = 0;
        }
        if ((sysml_state_debounceCounterMs >= 150)) {
            sysml_state_debounceStable = true;
        }
    }
    else {
        sysml_state_debounceStable = false;
        sysml_state_debounceCounterMs = 0;
    }
    sysml_lazy_0 = sysml_state_debounceStable;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_0;
    }
    sysml_lazy_1 = sysml_lazy_0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_1) {
        if ((sysml_state_counter >= 10)) {
            sysml_state_counter = 0;
            sysml_state_overflow = (sysml_state_overflow + 1);
        }
        else {
            sysml_state_counter = (sysml_state_counter + 1);
        }
        sysml_state_debounceStable = false;
        sysml_state_debounceCounterMs = 0;
    }
    sysml_state_previousPressed = io_0;
    if ((sysml_state_overflow == 3)) {
        sysml_state_overflow = 0;
    }
    bool sysml_value_0 = 0;
    sysml_lazy_2 = sysml_state_overflow == 1;
    if (!sysml_lazy_2) {
        sysml_lazy_2 = sysml_state_overflow == 3;
    }
    sysml_value_0 = sysml_lazy_2;
    bool sysml_value_1 = 0;
    sysml_lazy_3 = sysml_state_overflow == 2;
    if (!sysml_lazy_3) {
        sysml_lazy_3 = sysml_state_overflow == 3;
    }
    sysml_value_1 = sysml_lazy_3;
    bool sysml_value_2 = 0;
    sysml_value_2 = (sysml_state_counter >= 1);
    bool sysml_value_3 = 0;
    sysml_value_3 = (sysml_state_counter >= 2);
    bool sysml_value_4 = 0;
    sysml_value_4 = (sysml_state_counter >= 3);
    bool sysml_value_5 = 0;
    sysml_value_5 = (sysml_state_counter >= 4);
    bool sysml_value_6 = 0;
    sysml_value_6 = (sysml_state_counter >= 5);
    bool sysml_value_7 = 0;
    sysml_value_7 = (sysml_state_counter >= 6);
    bool sysml_value_8 = 0;
    sysml_value_8 = (sysml_state_counter >= 7);
    bool sysml_value_9 = 0;
    sysml_value_9 = (sysml_state_counter >= 8);
    bool sysml_value_10 = 0;
    sysml_value_10 = (sysml_state_counter >= 9);
    bool sysml_value_11 = 0;
    sysml_value_11 = (sysml_state_counter >= 10);
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
    *io_3_segment_0 = sysml_value_2;
    *io_3_segment_1 = sysml_value_3;
    *io_3_segment_2 = sysml_value_4;
    *io_3_segment_3 = sysml_value_5;
    *io_3_segment_4 = sysml_value_6;
    *io_3_segment_5 = sysml_value_7;
    *io_3_segment_6 = sysml_value_8;
    *io_3_segment_7 = sysml_value_9;
    *io_3_segment_8 = sysml_value_10;
    *io_3_segment_9 = sysml_value_11;
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
static bool hw_value_io_2 = false;
static bool hw_value_io_3_segment_0 = false;
static bool hw_value_io_3_segment_1 = false;
static bool hw_value_io_3_segment_2 = false;
static bool hw_value_io_3_segment_3 = false;
static bool hw_value_io_3_segment_4 = false;
static bool hw_value_io_3_segment_5 = false;
static bool hw_value_io_3_segment_6 = false;
static bool hw_value_io_3_segment_7 = false;
static bool hw_value_io_3_segment_8 = false;
static bool hw_value_io_3_segment_9 = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(4, INPUT_PULLUP);
  pinMode(2, OUTPUT);
  digitalWrite(2, hw_value_io_1 ? HIGH : LOW);
  pinMode(3, OUTPUT);
  digitalWrite(3, hw_value_io_2 ? HIGH : LOW);
  pinMode(5, OUTPUT);
  digitalWrite(5, hw_value_io_3_segment_0 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  digitalWrite(6, hw_value_io_3_segment_1 ? HIGH : LOW);
  pinMode(7, OUTPUT);
  digitalWrite(7, hw_value_io_3_segment_2 ? HIGH : LOW);
  pinMode(8, OUTPUT);
  digitalWrite(8, hw_value_io_3_segment_3 ? HIGH : LOW);
  pinMode(9, OUTPUT);
  digitalWrite(9, hw_value_io_3_segment_4 ? HIGH : LOW);
  pinMode(10, OUTPUT);
  digitalWrite(10, hw_value_io_3_segment_5 ? HIGH : LOW);
  pinMode(11, OUTPUT);
  digitalWrite(11, hw_value_io_3_segment_6 ? HIGH : LOW);
  pinMode(12, OUTPUT);
  digitalWrite(12, hw_value_io_3_segment_7 ? HIGH : LOW);
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_3_segment_8 ? HIGH : LOW);
  pinMode(44, OUTPUT);
  digitalWrite(44, hw_value_io_3_segment_9 ? HIGH : LOW);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 5UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (digitalRead(4) == LOW);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3_segment_0, &hw_value_io_3_segment_1, &hw_value_io_3_segment_2, &hw_value_io_3_segment_3, &hw_value_io_3_segment_4, &hw_value_io_3_segment_5, &hw_value_io_3_segment_6, &hw_value_io_3_segment_7, &hw_value_io_3_segment_8, &hw_value_io_3_segment_9);
  digitalWrite(2, hw_value_io_1 ? HIGH : LOW);
  digitalWrite(3, hw_value_io_2 ? HIGH : LOW);
  digitalWrite(5, hw_value_io_3_segment_0 ? HIGH : LOW);
  digitalWrite(6, hw_value_io_3_segment_1 ? HIGH : LOW);
  digitalWrite(7, hw_value_io_3_segment_2 ? HIGH : LOW);
  digitalWrite(8, hw_value_io_3_segment_3 ? HIGH : LOW);
  digitalWrite(9, hw_value_io_3_segment_4 ? HIGH : LOW);
  digitalWrite(10, hw_value_io_3_segment_5 ? HIGH : LOW);
  digitalWrite(11, hw_value_io_3_segment_6 ? HIGH : LOW);
  digitalWrite(12, hw_value_io_3_segment_7 ? HIGH : LOW);
  digitalWrite(13, hw_value_io_3_segment_8 ? HIGH : LOW);
  digitalWrite(44, hw_value_io_3_segment_9 ? HIGH : LOW);
}

