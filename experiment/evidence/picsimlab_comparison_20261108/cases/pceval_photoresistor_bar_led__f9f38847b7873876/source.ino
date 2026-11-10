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
extern int32_t sysml_state_mappedValue;
/* Global Variable define of  */
extern bool sysml_state_prevAbove;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7, bool* io_8, bool* io_9, bool* io_10);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_mappedValue = 0;
/* Global Variable define of  */
bool sysml_state_prevAbove = true;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_mappedValue = 0;
    sysml_state_prevAbove = true;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7, bool* io_8, bool* io_9, bool* io_10) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int32_t sysml_conditional_0 = 0;
    int32_t sysml_conditional_1 = 0;
    int32_t sysml_conditional_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    if (io_0 <= 0.0) {
        sysml_conditional_0 = 10;
    }
    else {
        if (io_0 >= 1023.0) {
            sysml_conditional_1 = 0;
        }
        else {
            if ((((double)((1023.0 - io_0) * 10.0)) / 1023.0) < ((double)((int32_t)(((double)((1023.0 - io_0) * 10.0)) / 1023.0)))) {
                sysml_conditional_2 = 1;
            }
            else {
                sysml_conditional_2 = 0;
            }
            sysml_conditional_1 = ((int32_t)(((double)((1023.0 - io_0) * 10.0)) / 1023.0)) - sysml_conditional_2;
        }
        sysml_conditional_0 = sysml_conditional_1;
    }
    sysml_state_mappedValue = sysml_conditional_0;
    if ((!sysml_state_prevAbove)) {
        sysml_lazy_3 = sysml_state_prevAbove;
        if (sysml_lazy_3) {
            sysml_lazy_4 = true;
            if (sysml_lazy_4) {
                sysml_lazy_4 = true;
            }
            sysml_lazy_3 = sysml_lazy_4;
        }
        sysml_state_prevAbove = sysml_lazy_3;
    }
    sysml_state_prevAbove = (sysml_state_mappedValue >= 0);
    bool sysml_value_0 = 0;
    sysml_value_0 = (sysml_state_mappedValue > 0);
    bool sysml_value_1 = 0;
    sysml_value_1 = (sysml_state_mappedValue > 1);
    bool sysml_value_2 = 0;
    sysml_value_2 = (sysml_state_mappedValue > 2);
    bool sysml_value_3 = 0;
    sysml_value_3 = (sysml_state_mappedValue > 3);
    bool sysml_value_4 = 0;
    sysml_value_4 = (sysml_state_mappedValue > 4);
    bool sysml_value_5 = 0;
    sysml_value_5 = (sysml_state_mappedValue > 5);
    bool sysml_value_6 = 0;
    sysml_value_6 = (sysml_state_mappedValue > 6);
    bool sysml_value_7 = 0;
    sysml_value_7 = (sysml_state_mappedValue > 7);
    bool sysml_value_8 = 0;
    sysml_value_8 = (sysml_state_mappedValue > 8);
    bool sysml_value_9 = 0;
    sysml_value_9 = (sysml_state_mappedValue > 9);
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
    *io_3 = sysml_value_2;
    *io_4 = sysml_value_3;
    *io_5 = sysml_value_4;
    *io_6 = sysml_value_5;
    *io_7 = sysml_value_6;
    *io_8 = sysml_value_7;
    *io_9 = sysml_value_8;
    *io_10 = sysml_value_9;
}
// END COMPILED SYSML MODEL

// LLM4MDD target-board: arduino:avr:uno
#include <Arduino.h>
#include <math.h>
#if defined(ARDUINO) && !defined(ARDUINO_AVR_UNO)
#error "Selected Arduino board differs from the interface contract"
#endif
static uint32_t hw_last_step = 0;
static bool hw_started = false;
static double hw_value_io_0 = 0;
static uint32_t hw_sampled_io_0 = 0;
static bool hw_attempted_io_0 = false;
static bool hw_value_io_1 = true;
static bool hw_value_io_2 = true;
static bool hw_value_io_3 = true;
static bool hw_value_io_4 = true;
static bool hw_value_io_5 = true;
static bool hw_value_io_6 = true;
static bool hw_value_io_7 = true;
static bool hw_value_io_8 = true;
static bool hw_value_io_9 = true;
static bool hw_value_io_10 = true;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(A0, INPUT);
  pinMode(2, OUTPUT);
  digitalWrite(2, hw_value_io_1 ? HIGH : LOW);
  pinMode(3, OUTPUT);
  digitalWrite(3, hw_value_io_2 ? HIGH : LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, hw_value_io_3 ? HIGH : LOW);
  pinMode(5, OUTPUT);
  digitalWrite(5, hw_value_io_4 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  digitalWrite(6, hw_value_io_5 ? HIGH : LOW);
  pinMode(7, OUTPUT);
  digitalWrite(7, hw_value_io_6 ? HIGH : LOW);
  pinMode(8, OUTPUT);
  digitalWrite(8, hw_value_io_7 ? HIGH : LOW);
  pinMode(9, OUTPUT);
  digitalWrite(9, hw_value_io_8 ? HIGH : LOW);
  pinMode(10, OUTPUT);
  digitalWrite(10, hw_value_io_9 ? HIGH : LOW);
  pinMode(11, OUTPUT);
  digitalWrite(11, hw_value_io_10 ? HIGH : LOW);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 10UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (analogRead(A0) * 1.0 + 0.0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_io_7, &hw_value_io_8, &hw_value_io_9, &hw_value_io_10);
  digitalWrite(2, hw_value_io_1 ? HIGH : LOW);
  digitalWrite(3, hw_value_io_2 ? HIGH : LOW);
  digitalWrite(4, hw_value_io_3 ? HIGH : LOW);
  digitalWrite(5, hw_value_io_4 ? HIGH : LOW);
  digitalWrite(6, hw_value_io_5 ? HIGH : LOW);
  digitalWrite(7, hw_value_io_6 ? HIGH : LOW);
  digitalWrite(8, hw_value_io_7 ? HIGH : LOW);
  digitalWrite(9, hw_value_io_8 ? HIGH : LOW);
  digitalWrite(10, hw_value_io_9 ? HIGH : LOW);
  digitalWrite(11, hw_value_io_10 ? HIGH : LOW);
}
