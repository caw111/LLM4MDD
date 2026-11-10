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
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool* io_1_segment_0, bool* io_1_segment_1, bool* io_1_segment_2, bool* io_1_segment_3, bool* io_1_segment_4, bool* io_1_segment_5, bool* io_1_segment_6, bool* io_1_segment_7, bool* io_1_segment_8, bool* io_1_segment_9);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool* io_1_segment_0, bool* io_1_segment_1, bool* io_1_segment_2, bool* io_1_segment_3, bool* io_1_segment_4, bool* io_1_segment_5, bool* io_1_segment_6, bool* io_1_segment_7, bool* io_1_segment_8, bool* io_1_segment_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int32_t sysml_value_0 = 0;
    int32_t sysml_conditional_0 = 0;
    if ((((double)((1023.0 - io_0) * 10.0)) / 1023.0) < ((double)((int32_t)(((double)((1023.0 - io_0) * 10.0)) / 1023.0)))) {
        sysml_conditional_0 = 1;
    }
    else {
        sysml_conditional_0 = 0;
    }
    sysml_value_0 = ((int32_t)(((double)((1023.0 - io_0) * 10.0)) / 1023.0)) - sysml_conditional_0;
    bool sysml_value_1 = 0;
    sysml_value_1 = (sysml_value_0 > 0);
    bool sysml_value_2 = 0;
    sysml_value_2 = (sysml_value_0 > 1);
    bool sysml_value_3 = 0;
    sysml_value_3 = (sysml_value_0 > 2);
    bool sysml_value_4 = 0;
    sysml_value_4 = (sysml_value_0 > 3);
    bool sysml_value_5 = 0;
    sysml_value_5 = (sysml_value_0 > 4);
    bool sysml_value_6 = 0;
    sysml_value_6 = (sysml_value_0 > 5);
    bool sysml_value_7 = 0;
    sysml_value_7 = (sysml_value_0 > 6);
    bool sysml_value_8 = 0;
    sysml_value_8 = (sysml_value_0 > 7);
    bool sysml_value_9 = 0;
    sysml_value_9 = (sysml_value_0 > 8);
    bool sysml_value_10 = 0;
    sysml_value_10 = (sysml_value_0 > 9);
    *io_1_segment_0 = sysml_value_1;
    *io_1_segment_1 = sysml_value_2;
    *io_1_segment_2 = sysml_value_3;
    *io_1_segment_3 = sysml_value_4;
    *io_1_segment_4 = sysml_value_5;
    *io_1_segment_5 = sysml_value_6;
    *io_1_segment_6 = sysml_value_7;
    *io_1_segment_7 = sysml_value_8;
    *io_1_segment_8 = sysml_value_9;
    *io_1_segment_9 = sysml_value_10;
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
static bool hw_value_io_1_segment_0 = false;
static bool hw_value_io_1_segment_1 = false;
static bool hw_value_io_1_segment_2 = false;
static bool hw_value_io_1_segment_3 = false;
static bool hw_value_io_1_segment_4 = false;
static bool hw_value_io_1_segment_5 = false;
static bool hw_value_io_1_segment_6 = false;
static bool hw_value_io_1_segment_7 = false;
static bool hw_value_io_1_segment_8 = false;
static bool hw_value_io_1_segment_9 = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(A0, INPUT);
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
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 10UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (analogRead(A0) * 1.0 + 0.0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
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
