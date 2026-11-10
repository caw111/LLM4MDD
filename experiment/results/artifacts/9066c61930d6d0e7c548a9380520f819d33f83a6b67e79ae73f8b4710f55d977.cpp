#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_8\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
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
extern int32_t sysml_state_accumulatedMs;
/* Global Variable define of  */
extern bool sysml_state_toggle;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_0, bool* io_1_segment_0, bool* io_1_segment_1, bool* io_1_segment_2, bool* io_1_segment_3, bool* io_1_segment_4, bool* io_1_segment_5, bool* io_1_segment_6, bool* io_1_segment_7, bool* io_1_segment_8, bool* io_1_segment_9);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_accumulatedMs = 0;
/* Global Variable define of  */
bool sysml_state_toggle = false;
/* Init function of CompositeActor named "ProgramPage1" */
#line 169 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_8\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void setup();
#line 193 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_8\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void loop();
#line 33 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_8\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_accumulatedMs = 0;
    sysml_state_toggle = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_0, bool* io_1_segment_0, bool* io_1_segment_1, bool* io_1_segment_2, bool* io_1_segment_3, bool* io_1_segment_4, bool* io_1_segment_5, bool* io_1_segment_6, bool* io_1_segment_7, bool* io_1_segment_8, bool* io_1_segment_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_conditional_0 = 0;
    bool sysml_conditional_1 = 0;
    bool sysml_conditional_2 = 0;
    bool sysml_conditional_3 = 0;
    bool sysml_conditional_4 = 0;
    bool sysml_conditional_5 = 0;
    bool sysml_conditional_6 = 0;
    bool sysml_conditional_7 = 0;
    bool sysml_conditional_8 = 0;
    bool sysml_conditional_9 = 0;
    sysml_state_accumulatedMs = (sysml_state_accumulatedMs + io_0);
    if ((sysml_state_accumulatedMs >= 2000)) {
        sysml_state_accumulatedMs = (sysml_state_accumulatedMs - 2000);
        sysml_state_toggle = (!sysml_state_toggle);
    }
    bool sysml_value_0 = 0;
    if (sysml_state_toggle) {
        sysml_conditional_0 = false;
    }
    else {
        sysml_conditional_0 = true;
    }
    sysml_value_0 = sysml_conditional_0;
    bool sysml_value_1 = 0;
    if (sysml_state_toggle) {
        sysml_conditional_1 = true;
    }
    else {
        sysml_conditional_1 = false;
    }
    sysml_value_1 = sysml_conditional_1;
    bool sysml_value_2 = 0;
    if (sysml_state_toggle) {
        sysml_conditional_2 = false;
    }
    else {
        sysml_conditional_2 = true;
    }
    sysml_value_2 = sysml_conditional_2;
    bool sysml_value_3 = 0;
    if (sysml_state_toggle) {
        sysml_conditional_3 = true;
    }
    else {
        sysml_conditional_3 = false;
    }
    sysml_value_3 = sysml_conditional_3;
    bool sysml_value_4 = 0;
    if (sysml_state_toggle) {
        sysml_conditional_4 = false;
    }
    else {
        sysml_conditional_4 = true;
    }
    sysml_value_4 = sysml_conditional_4;
    bool sysml_value_5 = 0;
    if (sysml_state_toggle) {
        sysml_conditional_5 = true;
    }
    else {
        sysml_conditional_5 = false;
    }
    sysml_value_5 = sysml_conditional_5;
    bool sysml_value_6 = 0;
    if (sysml_state_toggle) {
        sysml_conditional_6 = false;
    }
    else {
        sysml_conditional_6 = true;
    }
    sysml_value_6 = sysml_conditional_6;
    bool sysml_value_7 = 0;
    if (sysml_state_toggle) {
        sysml_conditional_7 = true;
    }
    else {
        sysml_conditional_7 = false;
    }
    sysml_value_7 = sysml_conditional_7;
    bool sysml_value_8 = 0;
    if (sysml_state_toggle) {
        sysml_conditional_8 = false;
    }
    else {
        sysml_conditional_8 = true;
    }
    sysml_value_8 = sysml_conditional_8;
    bool sysml_value_9 = 0;
    if (sysml_state_toggle) {
        sysml_conditional_9 = true;
    }
    else {
        sysml_conditional_9 = false;
    }
    sysml_value_9 = sysml_conditional_9;
    *io_1_segment_0 = sysml_value_0;
    *io_1_segment_1 = sysml_value_1;
    *io_1_segment_2 = sysml_value_2;
    *io_1_segment_3 = sysml_value_3;
    *io_1_segment_4 = sysml_value_4;
    *io_1_segment_5 = sysml_value_5;
    *io_1_segment_6 = sysml_value_6;
    *io_1_segment_7 = sysml_value_7;
    *io_1_segment_8 = sysml_value_8;
    *io_1_segment_9 = sysml_value_9;
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
static int32_t hw_value_io_0 = 0;
static bool hw_value_io_1_segment_0 = true;
static bool hw_value_io_1_segment_1 = false;
static bool hw_value_io_1_segment_2 = true;
static bool hw_value_io_1_segment_3 = false;
static bool hw_value_io_1_segment_4 = true;
static bool hw_value_io_1_segment_5 = false;
static bool hw_value_io_1_segment_6 = true;
static bool hw_value_io_1_segment_7 = false;
static bool hw_value_io_1_segment_8 = true;
static bool hw_value_io_1_segment_9 = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, OUTPUT);
  digitalWrite(2, hw_value_io_1_segment_0 ? HIGH : LOW);
  pinMode(3, OUTPUT);
  digitalWrite(3, hw_value_io_1_segment_1 ? HIGH : LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, hw_value_io_1_segment_2 ? HIGH : LOW);
  pinMode(5, OUTPUT);
  digitalWrite(5, hw_value_io_1_segment_3 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  digitalWrite(6, hw_value_io_1_segment_4 ? HIGH : LOW);
  pinMode(7, OUTPUT);
  digitalWrite(7, hw_value_io_1_segment_5 ? HIGH : LOW);
  pinMode(8, OUTPUT);
  digitalWrite(8, hw_value_io_1_segment_6 ? HIGH : LOW);
  pinMode(9, OUTPUT);
  digitalWrite(9, hw_value_io_1_segment_7 ? HIGH : LOW);
  pinMode(10, OUTPUT);
  digitalWrite(10, hw_value_io_1_segment_8 ? HIGH : LOW);
  pinMode(11, OUTPUT);
  digitalWrite(11, hw_value_io_1_segment_9 ? HIGH : LOW);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_0 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, &hw_value_io_1_segment_0, &hw_value_io_1_segment_1, &hw_value_io_1_segment_2, &hw_value_io_1_segment_3, &hw_value_io_1_segment_4, &hw_value_io_1_segment_5, &hw_value_io_1_segment_6, &hw_value_io_1_segment_7, &hw_value_io_1_segment_8, &hw_value_io_1_segment_9);
  digitalWrite(2, hw_value_io_1_segment_0 ? HIGH : LOW);
  digitalWrite(3, hw_value_io_1_segment_1 ? HIGH : LOW);
  digitalWrite(4, hw_value_io_1_segment_2 ? HIGH : LOW);
  digitalWrite(5, hw_value_io_1_segment_3 ? HIGH : LOW);
  digitalWrite(6, hw_value_io_1_segment_4 ? HIGH : LOW);
  digitalWrite(7, hw_value_io_1_segment_5 ? HIGH : LOW);
  digitalWrite(8, hw_value_io_1_segment_6 ? HIGH : LOW);
  digitalWrite(9, hw_value_io_1_segment_7 ? HIGH : LOW);
  digitalWrite(10, hw_value_io_1_segment_8 ? HIGH : LOW);
  digitalWrite(11, hw_value_io_1_segment_9 ? HIGH : LOW);
}

