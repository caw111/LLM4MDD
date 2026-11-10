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
extern int32_t sysml_state_blinkAccumulatorMs;
/* Global Variable define of  */
extern bool sysml_state_blinkOn;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_0, double io_2, double* io_1, bool* io_3_segment_0, bool* io_3_segment_1, bool* io_3_segment_2, bool* io_3_segment_3, bool* io_3_segment_4, bool* io_3_segment_5, bool* io_3_segment_6, bool* io_3_segment_7, bool* io_3_segment_8, bool* io_3_segment_9);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_blinkAccumulatorMs = 0;
/* Global Variable define of  */
bool sysml_state_blinkOn = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_blinkAccumulatorMs = 0;
    sysml_state_blinkOn = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_0, double io_2, double* io_1, bool* io_3_segment_0, bool* io_3_segment_1, bool* io_3_segment_2, bool* io_3_segment_3, bool* io_3_segment_4, bool* io_3_segment_5, bool* io_3_segment_6, bool* io_3_segment_7, bool* io_3_segment_8, bool* io_3_segment_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_conditional_0 = 0;
    double sysml_conditional_1 = 0;
    bool sysml_conditional_2 = 0;
    bool sysml_conditional_3 = 0;
    bool sysml_conditional_4 = 0;
    bool sysml_conditional_5 = 0;
    bool sysml_conditional_6 = 0;
    bool sysml_conditional_7 = 0;
    bool sysml_conditional_8 = 0;
    bool sysml_conditional_9 = 0;
    bool sysml_conditional_10 = 0;
    bool sysml_conditional_11 = 0;
    if ((io_2 == 1023.0)) {
        sysml_state_blinkAccumulatorMs = (sysml_state_blinkAccumulatorMs + io_0);
        if ((sysml_state_blinkAccumulatorMs >= 2000)) {
            sysml_state_blinkAccumulatorMs = (sysml_state_blinkAccumulatorMs - 2000);
            sysml_state_blinkOn = (!sysml_state_blinkOn);
        }
        if ((sysml_state_blinkAccumulatorMs >= 2000)) {
            sysml_state_blinkAccumulatorMs = (sysml_state_blinkAccumulatorMs - 2000);
            sysml_state_blinkOn = (!sysml_state_blinkOn);
        }
    }
    else {
        sysml_state_blinkAccumulatorMs = 0;
        sysml_state_blinkOn = false;
    }
    double sysml_value_0 = 0;
    if (io_2 == 1023.0) {
        if (sysml_state_blinkOn) {
            sysml_conditional_1 = 1023.0;
        }
        else {
            sysml_conditional_1 = 0.0;
        }
        sysml_conditional_0 = sysml_conditional_1;
    }
    else {
        sysml_conditional_0 = io_2;
    }
    sysml_value_0 = sysml_conditional_0;
    bool sysml_value_1 = 0;
    if (io_2 <= 102.0) {
        sysml_conditional_2 = false;
    }
    else {
        sysml_conditional_2 = false;
    }
    sysml_value_1 = sysml_conditional_2;
    bool sysml_value_2 = 0;
    if (io_2 >= 103.0) {
        sysml_conditional_3 = io_2 <= 204.0;
    }
    else {
        sysml_conditional_3 = false;
    }
    sysml_value_2 = sysml_conditional_3;
    bool sysml_value_3 = 0;
    if (io_2 >= 205.0) {
        sysml_conditional_4 = io_2 <= 306.0;
    }
    else {
        sysml_conditional_4 = false;
    }
    sysml_value_3 = sysml_conditional_4;
    bool sysml_value_4 = 0;
    if (io_2 >= 307.0) {
        sysml_conditional_5 = io_2 <= 408.0;
    }
    else {
        sysml_conditional_5 = false;
    }
    sysml_value_4 = sysml_conditional_5;
    bool sysml_value_5 = 0;
    if (io_2 >= 409.0) {
        sysml_conditional_6 = io_2 <= 510.0;
    }
    else {
        sysml_conditional_6 = false;
    }
    sysml_value_5 = sysml_conditional_6;
    bool sysml_value_6 = 0;
    if (io_2 >= 511.0) {
        sysml_conditional_7 = io_2 <= 612.0;
    }
    else {
        sysml_conditional_7 = false;
    }
    sysml_value_6 = sysml_conditional_7;
    bool sysml_value_7 = 0;
    if (io_2 >= 613.0) {
        sysml_conditional_8 = io_2 <= 714.0;
    }
    else {
        sysml_conditional_8 = false;
    }
    sysml_value_7 = sysml_conditional_8;
    bool sysml_value_8 = 0;
    if (io_2 >= 715.0) {
        sysml_conditional_9 = io_2 <= 816.0;
    }
    else {
        sysml_conditional_9 = false;
    }
    sysml_value_8 = sysml_conditional_9;
    bool sysml_value_9 = 0;
    if (io_2 >= 817.0) {
        sysml_conditional_10 = io_2 <= 918.0;
    }
    else {
        sysml_conditional_10 = false;
    }
    sysml_value_9 = sysml_conditional_10;
    bool sysml_value_10 = 0;
    if (io_2 >= 919.0) {
        sysml_conditional_11 = io_2 <= 1023.0;
    }
    else {
        sysml_conditional_11 = false;
    }
    sysml_value_10 = sysml_conditional_11;
    *io_1 = sysml_value_0;
    *io_3_segment_0 = sysml_value_1;
    *io_3_segment_1 = sysml_value_2;
    *io_3_segment_2 = sysml_value_3;
    *io_3_segment_3 = sysml_value_4;
    *io_3_segment_4 = sysml_value_5;
    *io_3_segment_5 = sysml_value_6;
    *io_3_segment_6 = sysml_value_7;
    *io_3_segment_7 = sysml_value_8;
    *io_3_segment_8 = sysml_value_9;
    *io_3_segment_9 = sysml_value_10;
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
static double hw_value_io_1 = 0;
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

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, OUTPUT);
  analogWrite(2, (int)constrain((hw_value_io_1 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(A3, INPUT);
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
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_2 || (uint32_t)(hw_now - hw_sampled_io_2) >= 10UL) {
    hw_attempted_io_2 = true; hw_sampled_io_2 = hw_now;
    hw_value_io_2 = (analogRead(A3) * 1.0 + 0.0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_0 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_2, &hw_value_io_1, &hw_value_io_3_segment_0, &hw_value_io_3_segment_1, &hw_value_io_3_segment_2, &hw_value_io_3_segment_3, &hw_value_io_3_segment_4, &hw_value_io_3_segment_5, &hw_value_io_3_segment_6, &hw_value_io_3_segment_7, &hw_value_io_3_segment_8, &hw_value_io_3_segment_9);
  analogWrite(2, (int)constrain((hw_value_io_1 * 1.0 + 0.0), 0.0, 255.0));
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
