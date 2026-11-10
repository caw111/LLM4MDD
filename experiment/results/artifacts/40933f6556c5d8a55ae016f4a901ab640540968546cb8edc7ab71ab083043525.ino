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
extern bool sysml_state_nextLedPhase;
/* Global Variable define of  */
extern int32_t sysml_state_segmentIndex;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_14, bool* io_0, bool* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7, bool* io_8, bool* io_9, bool* io_10, bool* io_11, bool* io_12, bool* io_13);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_accumulatedMs = 0;
/* Global Variable define of  */
bool sysml_state_nextLedPhase = false;
/* Global Variable define of  */
int32_t sysml_state_segmentIndex = 1;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_accumulatedMs = 0;
    sysml_state_nextLedPhase = false;
    sysml_state_segmentIndex = 1;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_14, bool* io_0, bool* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7, bool* io_8, bool* io_9, bool* io_10, bool* io_11, bool* io_12, bool* io_13) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int32_t sysml_conditional_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    sysml_state_accumulatedMs = (sysml_state_accumulatedMs + io_14);
    if ((sysml_state_accumulatedMs >= 2000)) {
        sysml_state_accumulatedMs = (sysml_state_accumulatedMs - 2000);
        if (sysml_state_segmentIndex >= 10) {
            sysml_conditional_0 = 1;
        }
        else {
            sysml_conditional_0 = sysml_state_segmentIndex + 1;
        }
        sysml_state_segmentIndex = sysml_conditional_0;
        sysml_state_nextLedPhase = (!sysml_state_nextLedPhase);
    }
    bool sysml_value_0 = 0;
    sysml_lazy_1 = sysml_state_segmentIndex >= 1;
    if (sysml_lazy_1) {
        sysml_lazy_1 = sysml_state_segmentIndex <= 10;
    }
    sysml_lazy_2 = sysml_lazy_1;
    if (sysml_lazy_2) {
        sysml_lazy_2 = sysml_state_nextLedPhase;
    }
    sysml_value_0 = sysml_lazy_2;
    bool sysml_value_1 = 0;
    sysml_lazy_3 = sysml_state_segmentIndex >= 1;
    if (sysml_lazy_3) {
        sysml_lazy_3 = sysml_state_segmentIndex <= 10;
    }
    sysml_lazy_4 = sysml_lazy_3;
    if (sysml_lazy_4) {
        sysml_lazy_4 = !sysml_state_nextLedPhase;
    }
    sysml_value_1 = sysml_lazy_4;
    bool sysml_value_2 = 0;
    sysml_value_2 = (sysml_state_segmentIndex == 1);
    bool sysml_value_3 = 0;
    sysml_value_3 = (sysml_state_segmentIndex == 10);
    bool sysml_value_4 = 0;
    sysml_value_4 = (sysml_state_segmentIndex == 2);
    bool sysml_value_5 = 0;
    sysml_value_5 = (sysml_state_segmentIndex == 3);
    bool sysml_value_6 = 0;
    sysml_value_6 = (sysml_state_segmentIndex == 4);
    bool sysml_value_7 = 0;
    sysml_value_7 = (sysml_state_segmentIndex == 5);
    bool sysml_value_8 = 0;
    sysml_value_8 = (sysml_state_segmentIndex == 6);
    bool sysml_value_9 = 0;
    sysml_value_9 = (sysml_state_segmentIndex == 7);
    bool sysml_value_10 = 0;
    sysml_value_10 = (sysml_state_segmentIndex == 8);
    bool sysml_value_11 = 0;
    sysml_value_11 = (sysml_state_segmentIndex == 9);
    bool sysml_value_12 = 0;
    sysml_value_12 = false;
    bool sysml_value_13 = 0;
    sysml_value_13 = false;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
    *io_4 = sysml_value_4;
    *io_5 = sysml_value_5;
    *io_6 = sysml_value_6;
    *io_7 = sysml_value_7;
    *io_8 = sysml_value_8;
    *io_9 = sysml_value_9;
    *io_10 = sysml_value_10;
    *io_11 = sysml_value_11;
    *io_12 = sysml_value_12;
    *io_13 = sysml_value_13;
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
static bool hw_value_io_3 = false;
static bool hw_value_io_4 = false;
static bool hw_value_io_5 = false;
static bool hw_value_io_6 = false;
static bool hw_value_io_7 = false;
static bool hw_value_io_8 = false;
static bool hw_value_io_9 = false;
static bool hw_value_io_10 = false;
static bool hw_value_io_11 = false;
static bool hw_value_io_12 = false;
static bool hw_value_io_13 = false;
static int32_t hw_value_io_14 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(10, OUTPUT);
  digitalWrite(10, hw_value_io_0 ? HIGH : LOW);
  pinMode(11, OUTPUT);
  digitalWrite(11, hw_value_io_1 ? HIGH : LOW);
  pinMode(A1, OUTPUT);
  digitalWrite(A1, hw_value_io_2 ? HIGH : LOW);
  pinMode(A10, OUTPUT);
  digitalWrite(A10, hw_value_io_3 ? HIGH : LOW);
  pinMode(2, OUTPUT);
  digitalWrite(2, hw_value_io_4 ? HIGH : LOW);
  pinMode(3, OUTPUT);
  digitalWrite(3, hw_value_io_5 ? HIGH : LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, hw_value_io_6 ? HIGH : LOW);
  pinMode(5, OUTPUT);
  digitalWrite(5, hw_value_io_7 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  digitalWrite(6, hw_value_io_8 ? HIGH : LOW);
  pinMode(7, OUTPUT);
  digitalWrite(7, hw_value_io_9 ? HIGH : LOW);
  pinMode(8, OUTPUT);
  digitalWrite(8, hw_value_io_10 ? HIGH : LOW);
  pinMode(9, OUTPUT);
  digitalWrite(9, hw_value_io_11 ? HIGH : LOW);
  pinMode(12, OUTPUT);
  digitalWrite(12, hw_value_io_12 ? HIGH : LOW);
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_13 ? HIGH : LOW);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_14 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_14, &hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_io_7, &hw_value_io_8, &hw_value_io_9, &hw_value_io_10, &hw_value_io_11, &hw_value_io_12, &hw_value_io_13);
  digitalWrite(10, hw_value_io_0 ? HIGH : LOW);
  digitalWrite(11, hw_value_io_1 ? HIGH : LOW);
  digitalWrite(A1, hw_value_io_2 ? HIGH : LOW);
  digitalWrite(A10, hw_value_io_3 ? HIGH : LOW);
  digitalWrite(2, hw_value_io_4 ? HIGH : LOW);
  digitalWrite(3, hw_value_io_5 ? HIGH : LOW);
  digitalWrite(4, hw_value_io_6 ? HIGH : LOW);
  digitalWrite(5, hw_value_io_7 ? HIGH : LOW);
  digitalWrite(6, hw_value_io_8 ? HIGH : LOW);
  digitalWrite(7, hw_value_io_9 ? HIGH : LOW);
  digitalWrite(8, hw_value_io_10 ? HIGH : LOW);
  digitalWrite(9, hw_value_io_11 ? HIGH : LOW);
  digitalWrite(12, hw_value_io_12 ? HIGH : LOW);
  digitalWrite(13, hw_value_io_13 ? HIGH : LOW);
}
