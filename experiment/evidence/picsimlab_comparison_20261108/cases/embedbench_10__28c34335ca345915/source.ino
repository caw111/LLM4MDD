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
extern bool sysml_state_directionReversed;
/* Global Variable define of  */
extern bool sysml_state_previousPressed;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_1, bool io_2, bool* io_0_segment_0, bool* io_0_segment_1, bool* io_0_segment_2, bool* io_0_segment_3, bool* io_0_segment_4, bool* io_0_segment_5, bool* io_0_segment_6, bool* io_0_segment_7, bool* io_0_segment_8, bool* io_0_segment_9);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_directionReversed = false;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_directionReversed = false;
    sysml_state_previousPressed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_1, bool io_2, bool* io_0_segment_0, bool* io_0_segment_1, bool* io_0_segment_2, bool* io_0_segment_3, bool* io_0_segment_4, bool* io_0_segment_5, bool* io_0_segment_6, bool* io_0_segment_7, bool* io_0_segment_8, bool* io_0_segment_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    int32_t sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    int32_t sysml_conditional_6 = 0;
    bool sysml_lazy_7 = 0;
    int32_t sysml_conditional_8 = 0;
    sysml_lazy_0 = io_2;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        sysml_state_directionReversed = (!sysml_state_directionReversed);
    }
    sysml_state_previousPressed = io_2;
    bool sysml_value_0 = 0;
    if (sysml_state_directionReversed) {
        sysml_lazy_2 = io_1 >= 0.0;
        if (!sysml_lazy_2) {
            sysml_lazy_2 = false;
        }
        sysml_conditional_1 = sysml_lazy_2;
    }
    else {
        if (io_1 < ((double)((int32_t)io_1))) {
            sysml_conditional_3 = 1;
        }
        else {
            sysml_conditional_3 = 0;
        }
        sysml_lazy_4 = (((int32_t)io_1) - sysml_conditional_3) >= 1;
        if (!sysml_lazy_4) {
            sysml_lazy_4 = false;
        }
        sysml_conditional_1 = sysml_lazy_4;
    }
    sysml_value_0 = sysml_conditional_1;
    bool sysml_value_1 = 0;
    sysml_lazy_5 = sysml_state_directionReversed;
    if (sysml_lazy_5) {
        if (io_1 < ((double)((int32_t)io_1))) {
            sysml_conditional_6 = 1;
        }
        else {
            sysml_conditional_6 = 0;
        }
        sysml_lazy_5 = (((int32_t)io_1) - sysml_conditional_6) <= 9;
    }
    sysml_lazy_7 = sysml_lazy_5;
    if (sysml_lazy_7) {
        if (io_1 < ((double)((int32_t)io_1))) {
            sysml_conditional_8 = 1;
        }
        else {
            sysml_conditional_8 = 0;
        }
        sysml_lazy_7 = (((int32_t)io_1) - sysml_conditional_8) >= 1;
    }
    sysml_value_1 = sysml_lazy_7;
    bool sysml_value_2 = 0;
    sysml_value_2 = false;
    bool sysml_value_3 = 0;
    sysml_value_3 = false;
    bool sysml_value_4 = 0;
    sysml_value_4 = false;
    bool sysml_value_5 = 0;
    sysml_value_5 = false;
    bool sysml_value_6 = 0;
    sysml_value_6 = false;
    bool sysml_value_7 = 0;
    sysml_value_7 = false;
    bool sysml_value_8 = 0;
    sysml_value_8 = false;
    bool sysml_value_9 = 0;
    sysml_value_9 = false;
    *io_0_segment_0 = sysml_value_0;
    *io_0_segment_1 = sysml_value_1;
    *io_0_segment_2 = sysml_value_2;
    *io_0_segment_3 = sysml_value_3;
    *io_0_segment_4 = sysml_value_4;
    *io_0_segment_5 = sysml_value_5;
    *io_0_segment_6 = sysml_value_6;
    *io_0_segment_7 = sysml_value_7;
    *io_0_segment_8 = sysml_value_8;
    *io_0_segment_9 = sysml_value_9;
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
static bool hw_value_io_0_segment_0 = false;
static bool hw_value_io_0_segment_1 = false;
static bool hw_value_io_0_segment_2 = false;
static bool hw_value_io_0_segment_3 = false;
static bool hw_value_io_0_segment_4 = false;
static bool hw_value_io_0_segment_5 = false;
static bool hw_value_io_0_segment_6 = false;
static bool hw_value_io_0_segment_7 = false;
static bool hw_value_io_0_segment_8 = false;
static bool hw_value_io_0_segment_9 = false;
static double hw_value_io_1 = 0;
static uint32_t hw_sampled_io_1 = 0;
static bool hw_attempted_io_1 = false;
static bool hw_value_io_2 = false;
static uint32_t hw_sampled_io_2 = 0;
static bool hw_attempted_io_2 = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, OUTPUT);
  digitalWrite(2, hw_value_io_0_segment_0 ? HIGH : LOW);
  pinMode(3, OUTPUT);
  digitalWrite(3, hw_value_io_0_segment_1 ? HIGH : LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, hw_value_io_0_segment_2 ? HIGH : LOW);
  pinMode(5, OUTPUT);
  digitalWrite(5, hw_value_io_0_segment_3 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  digitalWrite(6, hw_value_io_0_segment_4 ? HIGH : LOW);
  pinMode(7, OUTPUT);
  digitalWrite(7, hw_value_io_0_segment_5 ? HIGH : LOW);
  pinMode(8, OUTPUT);
  digitalWrite(8, hw_value_io_0_segment_6 ? HIGH : LOW);
  pinMode(9, OUTPUT);
  digitalWrite(9, hw_value_io_0_segment_7 ? HIGH : LOW);
  pinMode(10, OUTPUT);
  digitalWrite(10, hw_value_io_0_segment_8 ? HIGH : LOW);
  pinMode(11, OUTPUT);
  digitalWrite(11, hw_value_io_0_segment_9 ? HIGH : LOW);
  pinMode(A3, INPUT);
  pinMode(12, INPUT_PULLUP);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 5UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = (analogRead(A3) * 1.0 + 0.0);
  }
  if (!hw_attempted_io_2 || (uint32_t)(hw_now - hw_sampled_io_2) >= 5UL) {
    hw_attempted_io_2 = true; hw_sampled_io_2 = hw_now;
    hw_value_io_2 = (digitalRead(12) == LOW);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_1, hw_value_io_2, &hw_value_io_0_segment_0, &hw_value_io_0_segment_1, &hw_value_io_0_segment_2, &hw_value_io_0_segment_3, &hw_value_io_0_segment_4, &hw_value_io_0_segment_5, &hw_value_io_0_segment_6, &hw_value_io_0_segment_7, &hw_value_io_0_segment_8, &hw_value_io_0_segment_9);
  digitalWrite(2, hw_value_io_0_segment_0 ? HIGH : LOW);
  digitalWrite(3, hw_value_io_0_segment_1 ? HIGH : LOW);
  digitalWrite(4, hw_value_io_0_segment_2 ? HIGH : LOW);
  digitalWrite(5, hw_value_io_0_segment_3 ? HIGH : LOW);
  digitalWrite(6, hw_value_io_0_segment_4 ? HIGH : LOW);
  digitalWrite(7, hw_value_io_0_segment_5 ? HIGH : LOW);
  digitalWrite(8, hw_value_io_0_segment_6 ? HIGH : LOW);
  digitalWrite(9, hw_value_io_0_segment_7 ? HIGH : LOW);
  digitalWrite(10, hw_value_io_0_segment_8 ? HIGH : LOW);
  digitalWrite(11, hw_value_io_0_segment_9 ? HIGH : LOW);
}
