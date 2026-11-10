#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\repaired_workflow_continuation_20261016\\embedbench_75\\workflow\\attempt_0\\sketch\\sketch.ino"
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
extern bool sysml_state_activeIsPot1;
/* Global Variable define of  */
extern int32_t sysml_state_debounceTimerMs;
/* Global Variable define of  */
extern bool sysml_state_previousPressed;
/* Global Variable define of  */
extern int32_t sysml_state_ledsLitCount;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, double io_1, double io_2, int32_t io_4, bool* io_3_segment_0, bool* io_3_segment_1, bool* io_3_segment_2, bool* io_3_segment_3, bool* io_3_segment_4, bool* io_3_segment_5, bool* io_3_segment_6, bool* io_3_segment_7, bool* io_3_segment_8, bool* io_3_segment_9);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_activeIsPot1 = true;
/* Global Variable define of  */
int32_t sysml_state_debounceTimerMs = 0;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
int32_t sysml_state_ledsLitCount = 0;
/* Init function of CompositeActor named "ProgramPage1" */
#line 262 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\repaired_workflow_continuation_20261016\\embedbench_75\\workflow\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 289 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\repaired_workflow_continuation_20261016\\embedbench_75\\workflow\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 41 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\repaired_workflow_continuation_20261016\\embedbench_75\\workflow\\attempt_0\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_activeIsPot1 = true;
    sysml_state_debounceTimerMs = 0;
    sysml_state_previousPressed = false;
    sysml_state_ledsLitCount = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, double io_1, double io_2, int32_t io_4, bool* io_3_segment_0, bool* io_3_segment_1, bool* io_3_segment_2, bool* io_3_segment_3, bool* io_3_segment_4, bool* io_3_segment_5, bool* io_3_segment_6, bool* io_3_segment_7, bool* io_3_segment_8, bool* io_3_segment_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    int32_t sysml_conditional_3 = 0;
    int32_t sysml_conditional_4 = 0;
    int32_t sysml_conditional_5 = 0;
    int32_t sysml_conditional_6 = 0;
    int32_t sysml_conditional_7 = 0;
    int32_t sysml_conditional_8 = 0;
    int32_t sysml_conditional_9 = 0;
    bool sysml_conditional_10 = 0;
    bool sysml_conditional_11 = 0;
    bool sysml_conditional_12 = 0;
    bool sysml_conditional_13 = 0;
    bool sysml_conditional_14 = 0;
    bool sysml_conditional_15 = 0;
    bool sysml_conditional_16 = 0;
    bool sysml_conditional_17 = 0;
    bool sysml_conditional_18 = 0;
    bool sysml_conditional_19 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        sysml_state_debounceTimerMs = 0;
    }
    if (io_0) {
        sysml_state_debounceTimerMs = (sysml_state_debounceTimerMs + io_4);
    }
    else {
        sysml_state_debounceTimerMs = 0;
    }
    sysml_lazy_1 = io_0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = sysml_state_debounceTimerMs >= 150;
    }
    sysml_lazy_2 = sysml_lazy_1;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_2) {
        sysml_state_activeIsPot1 = (!sysml_state_activeIsPot1);
        sysml_state_debounceTimerMs = 0;
    }
    sysml_state_previousPressed = io_0;
    if (sysml_state_activeIsPot1) {
        if (io_1 >= 1023.0) {
            sysml_conditional_4 = 10;
        }
        else {
            if (io_1 <= 0.0) {
                sysml_conditional_5 = 0;
            }
            else {
                if ((((double)(io_1 * 10.0)) / 1023.0) < ((double)((int32_t)(((double)(io_1 * 10.0)) / 1023.0)))) {
                    sysml_conditional_6 = 1;
                }
                else {
                    sysml_conditional_6 = 0;
                }
                sysml_conditional_5 = ((int32_t)(((double)(io_1 * 10.0)) / 1023.0)) - sysml_conditional_6;
            }
            sysml_conditional_4 = sysml_conditional_5;
        }
        sysml_conditional_3 = sysml_conditional_4;
    }
    else {
        if (io_2 >= 1023.0) {
            sysml_conditional_7 = 10;
        }
        else {
            if (io_2 <= 0.0) {
                sysml_conditional_8 = 0;
            }
            else {
                if ((((double)(io_2 * 10.0)) / 1023.0) < ((double)((int32_t)(((double)(io_2 * 10.0)) / 1023.0)))) {
                    sysml_conditional_9 = 1;
                }
                else {
                    sysml_conditional_9 = 0;
                }
                sysml_conditional_8 = ((int32_t)(((double)(io_2 * 10.0)) / 1023.0)) - sysml_conditional_9;
            }
            sysml_conditional_7 = sysml_conditional_8;
        }
        sysml_conditional_3 = sysml_conditional_7;
    }
    sysml_state_ledsLitCount = sysml_conditional_3;
    bool sysml_value_0 = 0;
    if (sysml_state_ledsLitCount >= 1) {
        sysml_conditional_10 = true;
    }
    else {
        sysml_conditional_10 = false;
    }
    sysml_value_0 = sysml_conditional_10;
    bool sysml_value_1 = 0;
    if (sysml_state_ledsLitCount >= 2) {
        sysml_conditional_11 = true;
    }
    else {
        sysml_conditional_11 = false;
    }
    sysml_value_1 = sysml_conditional_11;
    bool sysml_value_2 = 0;
    if (sysml_state_ledsLitCount >= 3) {
        sysml_conditional_12 = true;
    }
    else {
        sysml_conditional_12 = false;
    }
    sysml_value_2 = sysml_conditional_12;
    bool sysml_value_3 = 0;
    if (sysml_state_ledsLitCount >= 4) {
        sysml_conditional_13 = true;
    }
    else {
        sysml_conditional_13 = false;
    }
    sysml_value_3 = sysml_conditional_13;
    bool sysml_value_4 = 0;
    if (sysml_state_ledsLitCount >= 5) {
        sysml_conditional_14 = true;
    }
    else {
        sysml_conditional_14 = false;
    }
    sysml_value_4 = sysml_conditional_14;
    bool sysml_value_5 = 0;
    if (sysml_state_ledsLitCount >= 6) {
        sysml_conditional_15 = true;
    }
    else {
        sysml_conditional_15 = false;
    }
    sysml_value_5 = sysml_conditional_15;
    bool sysml_value_6 = 0;
    if (sysml_state_ledsLitCount >= 7) {
        sysml_conditional_16 = true;
    }
    else {
        sysml_conditional_16 = false;
    }
    sysml_value_6 = sysml_conditional_16;
    bool sysml_value_7 = 0;
    if (sysml_state_ledsLitCount >= 8) {
        sysml_conditional_17 = true;
    }
    else {
        sysml_conditional_17 = false;
    }
    sysml_value_7 = sysml_conditional_17;
    bool sysml_value_8 = 0;
    if (sysml_state_ledsLitCount >= 9) {
        sysml_conditional_18 = true;
    }
    else {
        sysml_conditional_18 = false;
    }
    sysml_value_8 = sysml_conditional_18;
    bool sysml_value_9 = 0;
    if (sysml_state_ledsLitCount >= 10) {
        sysml_conditional_19 = true;
    }
    else {
        sysml_conditional_19 = false;
    }
    sysml_value_9 = sysml_conditional_19;
    *io_3_segment_0 = sysml_value_0;
    *io_3_segment_1 = sysml_value_1;
    *io_3_segment_2 = sysml_value_2;
    *io_3_segment_3 = sysml_value_3;
    *io_3_segment_4 = sysml_value_4;
    *io_3_segment_5 = sysml_value_5;
    *io_3_segment_6 = sysml_value_6;
    *io_3_segment_7 = sysml_value_7;
    *io_3_segment_8 = sysml_value_8;
    *io_3_segment_9 = sysml_value_9;
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
  pinMode(2, INPUT_PULLUP);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(3, OUTPUT);
  digitalWrite(3, hw_value_io_3_segment_0 ? HIGH : LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, hw_value_io_3_segment_1 ? HIGH : LOW);
  pinMode(5, OUTPUT);
  digitalWrite(5, hw_value_io_3_segment_2 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  digitalWrite(6, hw_value_io_3_segment_3 ? HIGH : LOW);
  pinMode(7, OUTPUT);
  digitalWrite(7, hw_value_io_3_segment_4 ? HIGH : LOW);
  pinMode(8, OUTPUT);
  digitalWrite(8, hw_value_io_3_segment_5 ? HIGH : LOW);
  pinMode(9, OUTPUT);
  digitalWrite(9, hw_value_io_3_segment_6 ? HIGH : LOW);
  pinMode(10, OUTPUT);
  digitalWrite(10, hw_value_io_3_segment_7 ? HIGH : LOW);
  pinMode(11, OUTPUT);
  digitalWrite(11, hw_value_io_3_segment_8 ? HIGH : LOW);
  pinMode(12, OUTPUT);
  digitalWrite(12, hw_value_io_3_segment_9 ? HIGH : LOW);
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
  hw_value_io_4 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, hw_value_io_4, &hw_value_io_3_segment_0, &hw_value_io_3_segment_1, &hw_value_io_3_segment_2, &hw_value_io_3_segment_3, &hw_value_io_3_segment_4, &hw_value_io_3_segment_5, &hw_value_io_3_segment_6, &hw_value_io_3_segment_7, &hw_value_io_3_segment_8, &hw_value_io_3_segment_9);
  digitalWrite(3, hw_value_io_3_segment_0 ? HIGH : LOW);
  digitalWrite(4, hw_value_io_3_segment_1 ? HIGH : LOW);
  digitalWrite(5, hw_value_io_3_segment_2 ? HIGH : LOW);
  digitalWrite(6, hw_value_io_3_segment_3 ? HIGH : LOW);
  digitalWrite(7, hw_value_io_3_segment_4 ? HIGH : LOW);
  digitalWrite(8, hw_value_io_3_segment_5 ? HIGH : LOW);
  digitalWrite(9, hw_value_io_3_segment_6 ? HIGH : LOW);
  digitalWrite(10, hw_value_io_3_segment_7 ? HIGH : LOW);
  digitalWrite(11, hw_value_io_3_segment_8 ? HIGH : LOW);
  digitalWrite(12, hw_value_io_3_segment_9 ? HIGH : LOW);
}

