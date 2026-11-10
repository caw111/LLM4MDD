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
extern int32_t sysml_state_phaseIndex;
/* Global Variable define of  */
extern bool sysml_state_direction;
/* Global Variable define of  */
extern bool sysml_state_justAdvanced;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool* io_0, bool* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7, bool* io_8, bool* io_9, bool* io_10);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_phaseIndex = 0;
/* Global Variable define of  */
bool sysml_state_direction = true;
/* Global Variable define of  */
bool sysml_state_justAdvanced = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_phaseIndex = 0;
    sysml_state_direction = true;
    sysml_state_justAdvanced = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool* io_0, bool* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7, bool* io_8, bool* io_9, bool* io_10) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int32_t sysml_conditional_0 = 0;
    int32_t sysml_conditional_1 = 0;
    int32_t sysml_conditional_2 = 0;
    bool sysml_conditional_3 = 0;
    bool sysml_conditional_4 = 0;
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
    bool sysml_conditional_15 = 0;
    if ((!sysml_state_justAdvanced)) {
        sysml_state_justAdvanced = true;
    }
    else {
        if (sysml_state_direction) {
            if (sysml_state_phaseIndex == 10) {
                sysml_conditional_1 = 9;
            }
            else {
                sysml_conditional_1 = sysml_state_phaseIndex + 1;
            }
            sysml_conditional_0 = sysml_conditional_1;
        }
        else {
            if (sysml_state_phaseIndex == 1) {
                sysml_conditional_2 = 2;
            }
            else {
                sysml_conditional_2 = sysml_state_phaseIndex - 1;
            }
            sysml_conditional_0 = sysml_conditional_2;
        }
        sysml_state_phaseIndex = sysml_conditional_0;
        if (sysml_state_direction) {
            if (sysml_state_phaseIndex == 10) {
                sysml_conditional_4 = false;
            }
            else {
                sysml_conditional_4 = true;
            }
            sysml_conditional_3 = sysml_conditional_4;
        }
        else {
            if (sysml_state_phaseIndex == 1) {
                sysml_conditional_5 = true;
            }
            else {
                sysml_conditional_5 = false;
            }
            sysml_conditional_3 = sysml_conditional_5;
        }
        sysml_state_direction = sysml_conditional_3;
        sysml_state_justAdvanced = false;
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = false;
    bool sysml_value_1 = 0;
    if (sysml_state_direction) {
        sysml_conditional_6 = sysml_state_phaseIndex == 1;
    }
    else {
        sysml_conditional_6 = sysml_state_phaseIndex == 9;
    }
    sysml_value_1 = sysml_conditional_6;
    bool sysml_value_2 = 0;
    if (sysml_state_direction) {
        sysml_conditional_7 = sysml_state_phaseIndex == 2;
    }
    else {
        sysml_conditional_7 = sysml_state_phaseIndex == 8;
    }
    sysml_value_2 = sysml_conditional_7;
    bool sysml_value_3 = 0;
    if (sysml_state_direction) {
        sysml_conditional_8 = sysml_state_phaseIndex == 3;
    }
    else {
        sysml_conditional_8 = sysml_state_phaseIndex == 7;
    }
    sysml_value_3 = sysml_conditional_8;
    bool sysml_value_4 = 0;
    if (sysml_state_direction) {
        sysml_conditional_9 = sysml_state_phaseIndex == 4;
    }
    else {
        sysml_conditional_9 = sysml_state_phaseIndex == 6;
    }
    sysml_value_4 = sysml_conditional_9;
    bool sysml_value_5 = 0;
    if (sysml_state_direction) {
        sysml_conditional_10 = sysml_state_phaseIndex == 5;
    }
    else {
        sysml_conditional_10 = sysml_state_phaseIndex == 5;
    }
    sysml_value_5 = sysml_conditional_10;
    bool sysml_value_6 = 0;
    if (sysml_state_direction) {
        sysml_conditional_11 = sysml_state_phaseIndex == 6;
    }
    else {
        sysml_conditional_11 = sysml_state_phaseIndex == 4;
    }
    sysml_value_6 = sysml_conditional_11;
    bool sysml_value_7 = 0;
    if (sysml_state_direction) {
        sysml_conditional_12 = sysml_state_phaseIndex == 7;
    }
    else {
        sysml_conditional_12 = sysml_state_phaseIndex == 3;
    }
    sysml_value_7 = sysml_conditional_12;
    bool sysml_value_8 = 0;
    if (sysml_state_direction) {
        sysml_conditional_13 = sysml_state_phaseIndex == 8;
    }
    else {
        sysml_conditional_13 = sysml_state_phaseIndex == 2;
    }
    sysml_value_8 = sysml_conditional_13;
    bool sysml_value_9 = 0;
    if (sysml_state_direction) {
        sysml_conditional_14 = sysml_state_phaseIndex == 9;
    }
    else {
        sysml_conditional_14 = sysml_state_phaseIndex == 1;
    }
    sysml_value_9 = sysml_conditional_14;
    bool sysml_value_10 = 0;
    if (sysml_state_direction) {
        sysml_conditional_15 = sysml_state_phaseIndex == 10;
    }
    else {
        sysml_conditional_15 = false;
    }
    sysml_value_10 = sysml_conditional_15;
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

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, OUTPUT);
  digitalWrite(2, hw_value_io_0 ? HIGH : LOW);
  pinMode(3, OUTPUT);
  digitalWrite(3, hw_value_io_1 ? HIGH : LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, hw_value_io_2 ? HIGH : LOW);
  pinMode(5, OUTPUT);
  digitalWrite(5, hw_value_io_3 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  digitalWrite(6, hw_value_io_4 ? HIGH : LOW);
  pinMode(7, OUTPUT);
  digitalWrite(7, hw_value_io_5 ? HIGH : LOW);
  pinMode(8, OUTPUT);
  digitalWrite(8, hw_value_io_6 ? HIGH : LOW);
  pinMode(9, OUTPUT);
  digitalWrite(9, hw_value_io_7 ? HIGH : LOW);
  pinMode(10, OUTPUT);
  digitalWrite(10, hw_value_io_8 ? HIGH : LOW);
  pinMode(11, OUTPUT);
  digitalWrite(11, hw_value_io_9 ? HIGH : LOW);
  pinMode(12, OUTPUT);
  digitalWrite(12, hw_value_io_10 ? HIGH : LOW);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(&hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_io_7, &hw_value_io_8, &hw_value_io_9, &hw_value_io_10);
  digitalWrite(2, hw_value_io_0 ? HIGH : LOW);
  digitalWrite(3, hw_value_io_1 ? HIGH : LOW);
  digitalWrite(4, hw_value_io_2 ? HIGH : LOW);
  digitalWrite(5, hw_value_io_3 ? HIGH : LOW);
  digitalWrite(6, hw_value_io_4 ? HIGH : LOW);
  digitalWrite(7, hw_value_io_5 ? HIGH : LOW);
  digitalWrite(8, hw_value_io_6 ? HIGH : LOW);
  digitalWrite(9, hw_value_io_7 ? HIGH : LOW);
  digitalWrite(10, hw_value_io_8 ? HIGH : LOW);
  digitalWrite(11, hw_value_io_9 ? HIGH : LOW);
  digitalWrite(12, hw_value_io_10 ? HIGH : LOW);
}
