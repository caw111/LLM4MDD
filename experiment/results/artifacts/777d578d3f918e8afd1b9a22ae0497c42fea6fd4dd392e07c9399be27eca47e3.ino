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
extern int32_t sysml_state_blinkPhaseMs;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_1, int32_t io_3, double* io_0, bool* io_2_segment_0, bool* io_2_segment_1, bool* io_2_segment_2, bool* io_2_segment_3, bool* io_2_segment_4, bool* io_2_segment_5, bool* io_2_segment_6, bool* io_2_segment_7, bool* io_2_segment_8, bool* io_2_segment_9);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_blinkPhaseMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_blinkPhaseMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_1, int32_t io_3, double* io_0, bool* io_2_segment_0, bool* io_2_segment_1, bool* io_2_segment_2, bool* io_2_segment_3, bool* io_2_segment_4, bool* io_2_segment_5, bool* io_2_segment_6, bool* io_2_segment_7, bool* io_2_segment_8, bool* io_2_segment_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int32_t sysml_conditional_0 = 0;
    double sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    double sysml_conditional_3 = 0;
    double sysml_conditional_4 = 0;
    bool sysml_conditional_5 = 0;
    bool sysml_conditional_6 = 0;
    bool sysml_conditional_7 = 0;
    bool sysml_conditional_8 = 0;
    bool sysml_conditional_9 = 0;
    bool sysml_conditional_10 = 0;
    bool sysml_conditional_11 = 0;
    bool sysml_conditional_12 = 0;
    bool sysml_conditional_13 = 0;
    bool sysml_conditional_14 = 0;
    if (io_1 == 1023.0) {
        sysml_conditional_0 = (sysml_state_blinkPhaseMs + io_3) - 0;
    }
    else {
        sysml_conditional_0 = 0;
    }
    sysml_state_blinkPhaseMs = sysml_conditional_0;
    if ((sysml_state_blinkPhaseMs >= 4000)) {
        sysml_state_blinkPhaseMs = (sysml_state_blinkPhaseMs - 4000);
    }
    double sysml_value_0 = 0;
    if (io_1 == 1023.0) {
        sysml_lazy_2 = sysml_state_blinkPhaseMs < 2000;
        if (sysml_lazy_2) {
            sysml_lazy_2 = sysml_state_blinkPhaseMs >= 0;
        }
        if (sysml_lazy_2) {
            sysml_conditional_3 = 1023.0;
        }
        else {
            sysml_conditional_3 = 0.0;
        }
        sysml_conditional_1 = sysml_conditional_3;
    }
    else {
        if (io_1 == 1023.0) {
            sysml_conditional_4 = 1023.0;
        }
        else {
            sysml_conditional_4 = io_1;
        }
        sysml_conditional_1 = sysml_conditional_4;
    }
    sysml_value_0 = sysml_conditional_1;
    bool sysml_value_1 = 0;
    if (io_1 >= 103.0) {
        sysml_conditional_5 = true;
    }
    else {
        sysml_conditional_5 = false;
    }
    sysml_value_1 = sysml_conditional_5;
    bool sysml_value_2 = 0;
    if (io_1 >= 204.0) {
        sysml_conditional_6 = true;
    }
    else {
        sysml_conditional_6 = false;
    }
    sysml_value_2 = sysml_conditional_6;
    bool sysml_value_3 = 0;
    if (io_1 >= 307.0) {
        sysml_conditional_7 = true;
    }
    else {
        sysml_conditional_7 = false;
    }
    sysml_value_3 = sysml_conditional_7;
    bool sysml_value_4 = 0;
    if (io_1 >= 410.0) {
        sysml_conditional_8 = true;
    }
    else {
        sysml_conditional_8 = false;
    }
    sysml_value_4 = sysml_conditional_8;
    bool sysml_value_5 = 0;
    if (io_1 >= 513.0) {
        sysml_conditional_9 = true;
    }
    else {
        sysml_conditional_9 = false;
    }
    sysml_value_5 = sysml_conditional_9;
    bool sysml_value_6 = 0;
    if (io_1 >= 616.0) {
        sysml_conditional_10 = true;
    }
    else {
        sysml_conditional_10 = false;
    }
    sysml_value_6 = sysml_conditional_10;
    bool sysml_value_7 = 0;
    if (io_1 >= 719.0) {
        sysml_conditional_11 = true;
    }
    else {
        sysml_conditional_11 = false;
    }
    sysml_value_7 = sysml_conditional_11;
    bool sysml_value_8 = 0;
    if (io_1 >= 822.0) {
        sysml_conditional_12 = true;
    }
    else {
        sysml_conditional_12 = false;
    }
    sysml_value_8 = sysml_conditional_12;
    bool sysml_value_9 = 0;
    if (io_1 >= 925.0) {
        sysml_conditional_13 = true;
    }
    else {
        sysml_conditional_13 = false;
    }
    sysml_value_9 = sysml_conditional_13;
    bool sysml_value_10 = 0;
    if (io_1 >= 1028.0) {
        sysml_conditional_14 = true;
    }
    else {
        sysml_conditional_14 = false;
    }
    sysml_value_10 = sysml_conditional_14;
    *io_0 = sysml_value_0;
    *io_2_segment_0 = sysml_value_1;
    *io_2_segment_1 = sysml_value_2;
    *io_2_segment_2 = sysml_value_3;
    *io_2_segment_3 = sysml_value_4;
    *io_2_segment_4 = sysml_value_5;
    *io_2_segment_5 = sysml_value_6;
    *io_2_segment_6 = sysml_value_7;
    *io_2_segment_7 = sysml_value_8;
    *io_2_segment_8 = sysml_value_9;
    *io_2_segment_9 = sysml_value_10;
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
static double hw_value_io_0 = 0;
static double hw_value_io_1 = 0;
static uint32_t hw_sampled_io_1 = 0;
static bool hw_attempted_io_1 = false;
static bool hw_value_io_2_segment_0 = false;
static bool hw_value_io_2_segment_1 = false;
static bool hw_value_io_2_segment_2 = false;
static bool hw_value_io_2_segment_3 = false;
static bool hw_value_io_2_segment_4 = false;
static bool hw_value_io_2_segment_5 = false;
static bool hw_value_io_2_segment_6 = false;
static bool hw_value_io_2_segment_7 = false;
static bool hw_value_io_2_segment_8 = false;
static bool hw_value_io_2_segment_9 = false;
static int32_t hw_value_io_3 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, OUTPUT);
  analogWrite(2, (int)constrain((hw_value_io_0 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(A3, INPUT);
  pinMode(3, OUTPUT);
  digitalWrite(3, hw_value_io_2_segment_0 ? HIGH : LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, hw_value_io_2_segment_1 ? HIGH : LOW);
  pinMode(5, OUTPUT);
  digitalWrite(5, hw_value_io_2_segment_2 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  digitalWrite(6, hw_value_io_2_segment_3 ? HIGH : LOW);
  pinMode(7, OUTPUT);
  digitalWrite(7, hw_value_io_2_segment_4 ? HIGH : LOW);
  pinMode(8, OUTPUT);
  digitalWrite(8, hw_value_io_2_segment_5 ? HIGH : LOW);
  pinMode(9, OUTPUT);
  digitalWrite(9, hw_value_io_2_segment_6 ? HIGH : LOW);
  pinMode(10, OUTPUT);
  digitalWrite(10, hw_value_io_2_segment_7 ? HIGH : LOW);
  pinMode(11, OUTPUT);
  digitalWrite(11, hw_value_io_2_segment_8 ? HIGH : LOW);
  pinMode(12, OUTPUT);
  digitalWrite(12, hw_value_io_2_segment_9 ? HIGH : LOW);
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
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_3 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_1, hw_value_io_3, &hw_value_io_0, &hw_value_io_2_segment_0, &hw_value_io_2_segment_1, &hw_value_io_2_segment_2, &hw_value_io_2_segment_3, &hw_value_io_2_segment_4, &hw_value_io_2_segment_5, &hw_value_io_2_segment_6, &hw_value_io_2_segment_7, &hw_value_io_2_segment_8, &hw_value_io_2_segment_9);
  analogWrite(2, (int)constrain((hw_value_io_0 * 1.0 + 0.0), 0.0, 255.0));
  digitalWrite(3, hw_value_io_2_segment_0 ? HIGH : LOW);
  digitalWrite(4, hw_value_io_2_segment_1 ? HIGH : LOW);
  digitalWrite(5, hw_value_io_2_segment_2 ? HIGH : LOW);
  digitalWrite(6, hw_value_io_2_segment_3 ? HIGH : LOW);
  digitalWrite(7, hw_value_io_2_segment_4 ? HIGH : LOW);
  digitalWrite(8, hw_value_io_2_segment_5 ? HIGH : LOW);
  digitalWrite(9, hw_value_io_2_segment_6 ? HIGH : LOW);
  digitalWrite(10, hw_value_io_2_segment_7 ? HIGH : LOW);
  digitalWrite(11, hw_value_io_2_segment_8 ? HIGH : LOW);
  digitalWrite(12, hw_value_io_2_segment_9 ? HIGH : LOW);
}
