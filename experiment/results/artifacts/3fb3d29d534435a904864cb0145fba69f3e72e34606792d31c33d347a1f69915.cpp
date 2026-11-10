#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_96\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
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
extern int32_t sysml_state_phaseAccumulatedMs;
/* Global Variable define of  */
extern int32_t sysml_state_phaseIndex;
/* Global Variable define of  */
extern bool sysml_state_led1State;
/* Global Variable define of  */
extern bool sysml_state_led2State;
/* Global Variable define of  */
extern bool sysml_state_led3State;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_4, bool* io_0, bool* io_1, bool* io_2, bool* io_3_segment_0, bool* io_3_segment_1, bool* io_3_segment_2, bool* io_3_segment_3, bool* io_3_segment_4, bool* io_3_segment_5, bool* io_3_segment_6, bool* io_3_segment_7, bool* io_3_segment_8, bool* io_3_segment_9);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_phaseAccumulatedMs = 0;
/* Global Variable define of  */
int32_t sysml_state_phaseIndex = 0;
/* Global Variable define of  */
bool sysml_state_led1State = false;
/* Global Variable define of  */
bool sysml_state_led2State = false;
/* Global Variable define of  */
bool sysml_state_led3State = false;
/* Init function of CompositeActor named "ProgramPage1" */
#line 242 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_96\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void setup();
#line 272 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_96\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void loop();
#line 45 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_96\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_phaseAccumulatedMs = 0;
    sysml_state_phaseIndex = 0;
    sysml_state_led1State = false;
    sysml_state_led2State = false;
    sysml_state_led3State = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_4, bool* io_0, bool* io_1, bool* io_2, bool* io_3_segment_0, bool* io_3_segment_1, bool* io_3_segment_2, bool* io_3_segment_3, bool* io_3_segment_4, bool* io_3_segment_5, bool* io_3_segment_6, bool* io_3_segment_7, bool* io_3_segment_8, bool* io_3_segment_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int32_t sysml_conditional_0 = 0;
    bool sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_conditional_4 = 0;
    bool sysml_conditional_5 = 0;
    bool sysml_conditional_6 = 0;
    bool sysml_conditional_7 = 0;
    bool sysml_conditional_8 = 0;
    bool sysml_conditional_9 = 0;
    bool sysml_conditional_10 = 0;
    bool sysml_conditional_11 = 0;
    bool sysml_conditional_12 = 0;
    sysml_state_phaseAccumulatedMs = (sysml_state_phaseAccumulatedMs + io_4);
    if ((sysml_state_phaseAccumulatedMs >= 2000)) {
        sysml_state_phaseAccumulatedMs = (sysml_state_phaseAccumulatedMs - 2000);
        if (sysml_state_phaseIndex == 3) {
            sysml_conditional_0 = 0;
        }
        else {
            sysml_conditional_0 = sysml_state_phaseIndex + 1;
        }
        sysml_state_phaseIndex = sysml_conditional_0;
    }
    if ((sysml_state_phaseIndex == 0)) {
        sysml_state_led1State = true;
        sysml_state_led2State = false;
        sysml_state_led3State = false;
    }
    if ((sysml_state_phaseIndex == 1)) {
        sysml_state_led1State = false;
        sysml_state_led2State = true;
        sysml_state_led3State = false;
    }
    else {
        if ((sysml_state_phaseIndex == 2)) {
            sysml_state_led1State = false;
            sysml_state_led2State = false;
            sysml_state_led3State = true;
        }
        else {
            if ((sysml_state_phaseIndex == 3)) {
                sysml_state_led1State = false;
                sysml_state_led2State = false;
                sysml_state_led3State = false;
            }
            else {
                sysml_state_led1State = false;
                sysml_state_led2State = false;
                sysml_state_led3State = false;
            }
        }
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_led1State;
    bool sysml_value_1 = 0;
    sysml_value_1 = sysml_state_led2State;
    bool sysml_value_2 = 0;
    sysml_value_2 = sysml_state_led3State;
    bool sysml_value_3 = 0;
    if (sysml_state_phaseIndex == 3) {
        sysml_conditional_1 = true;
    }
    else {
        sysml_lazy_2 = sysml_state_phaseIndex == 0;
        if (!sysml_lazy_2) {
            sysml_lazy_2 = sysml_state_phaseIndex == 1;
        }
        sysml_lazy_3 = sysml_lazy_2;
        if (!sysml_lazy_3) {
            sysml_lazy_3 = sysml_state_phaseIndex == 2;
        }
        sysml_conditional_1 = sysml_lazy_3;
    }
    sysml_value_3 = sysml_conditional_1;
    bool sysml_value_4 = 0;
    if (sysml_state_phaseIndex == 3) {
        sysml_conditional_4 = true;
    }
    else {
        sysml_conditional_4 = sysml_state_phaseIndex == 1;
    }
    sysml_value_4 = sysml_conditional_4;
    bool sysml_value_5 = 0;
    if (sysml_state_phaseIndex == 3) {
        sysml_conditional_5 = true;
    }
    else {
        sysml_conditional_5 = false;
    }
    sysml_value_5 = sysml_conditional_5;
    bool sysml_value_6 = 0;
    if (sysml_state_phaseIndex == 3) {
        sysml_conditional_6 = true;
    }
    else {
        sysml_conditional_6 = false;
    }
    sysml_value_6 = sysml_conditional_6;
    bool sysml_value_7 = 0;
    if (sysml_state_phaseIndex == 3) {
        sysml_conditional_7 = true;
    }
    else {
        sysml_conditional_7 = false;
    }
    sysml_value_7 = sysml_conditional_7;
    bool sysml_value_8 = 0;
    if (sysml_state_phaseIndex == 3) {
        sysml_conditional_8 = true;
    }
    else {
        sysml_conditional_8 = false;
    }
    sysml_value_8 = sysml_conditional_8;
    bool sysml_value_9 = 0;
    if (sysml_state_phaseIndex == 3) {
        sysml_conditional_9 = true;
    }
    else {
        sysml_conditional_9 = false;
    }
    sysml_value_9 = sysml_conditional_9;
    bool sysml_value_10 = 0;
    if (sysml_state_phaseIndex == 3) {
        sysml_conditional_10 = true;
    }
    else {
        sysml_conditional_10 = false;
    }
    sysml_value_10 = sysml_conditional_10;
    bool sysml_value_11 = 0;
    if (sysml_state_phaseIndex == 3) {
        sysml_conditional_11 = true;
    }
    else {
        sysml_conditional_11 = false;
    }
    sysml_value_11 = sysml_conditional_11;
    bool sysml_value_12 = 0;
    if (sysml_state_phaseIndex == 3) {
        sysml_conditional_12 = true;
    }
    else {
        sysml_conditional_12 = false;
    }
    sysml_value_12 = sysml_conditional_12;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3_segment_0 = sysml_value_3;
    *io_3_segment_1 = sysml_value_4;
    *io_3_segment_2 = sysml_value_5;
    *io_3_segment_3 = sysml_value_6;
    *io_3_segment_4 = sysml_value_7;
    *io_3_segment_5 = sysml_value_8;
    *io_3_segment_6 = sysml_value_9;
    *io_3_segment_7 = sysml_value_10;
    *io_3_segment_8 = sysml_value_11;
    *io_3_segment_9 = sysml_value_12;
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
static int32_t hw_value_io_4 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, OUTPUT);
  digitalWrite(2, hw_value_io_0 ? HIGH : LOW);
  pinMode(3, OUTPUT);
  digitalWrite(3, hw_value_io_1 ? HIGH : LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, hw_value_io_2 ? HIGH : LOW);
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
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_4 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_4, &hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3_segment_0, &hw_value_io_3_segment_1, &hw_value_io_3_segment_2, &hw_value_io_3_segment_3, &hw_value_io_3_segment_4, &hw_value_io_3_segment_5, &hw_value_io_3_segment_6, &hw_value_io_3_segment_7, &hw_value_io_3_segment_8, &hw_value_io_3_segment_9);
  digitalWrite(2, hw_value_io_0 ? HIGH : LOW);
  digitalWrite(3, hw_value_io_1 ? HIGH : LOW);
  digitalWrite(4, hw_value_io_2 ? HIGH : LOW);
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

