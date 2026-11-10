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
extern int32_t sysml_state_elapsedSeconds_i32;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_0, bool* io_1_segment_0, bool* io_1_segment_1, bool* io_1_segment_2, bool* io_1_segment_3, bool* io_1_segment_4, bool* io_1_segment_5, bool* io_1_segment_6, bool* io_1_segment_7);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_elapsedSeconds_i32 = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_elapsedSeconds_i32 = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_0, bool* io_1_segment_0, bool* io_1_segment_1, bool* io_1_segment_2, bool* io_1_segment_3, bool* io_1_segment_4, bool* io_1_segment_5, bool* io_1_segment_6, bool* io_1_segment_7) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int32_t sysml_conditional_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_conditional_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_conditional_10 = 0;
    bool sysml_lazy_11 = 0;
    bool sysml_lazy_12 = 0;
    bool sysml_lazy_13 = 0;
    bool sysml_conditional_14 = 0;
    bool sysml_lazy_15 = 0;
    bool sysml_lazy_16 = 0;
    bool sysml_lazy_17 = 0;
    bool sysml_conditional_18 = 0;
    bool sysml_lazy_19 = 0;
    bool sysml_lazy_20 = 0;
    bool sysml_lazy_21 = 0;
    bool sysml_lazy_22 = 0;
    bool sysml_lazy_23 = 0;
    bool sysml_conditional_24 = 0;
    bool sysml_lazy_25 = 0;
    bool sysml_lazy_26 = 0;
    bool sysml_lazy_27 = 0;
    bool sysml_lazy_28 = 0;
    bool sysml_lazy_29 = 0;
    bool sysml_conditional_30 = 0;
    bool sysml_lazy_31 = 0;
    bool sysml_lazy_32 = 0;
    bool sysml_lazy_33 = 0;
    bool sysml_conditional_34 = 0;
    bool sysml_lazy_35 = 0;
    bool sysml_conditional_36 = 0;
    if ((((double)io_0) / 1000.0) < ((double)((int32_t)(((double)io_0) / 1000.0)))) {
        sysml_conditional_0 = 1;
    }
    else {
        sysml_conditional_0 = 0;
    }
    sysml_state_elapsedSeconds_i32 = sysml_state_elapsedSeconds_i32 + (((int32_t)(((double)io_0) / 1000.0)) - sysml_conditional_0);
    if ((sysml_state_elapsedSeconds_i32 < 0)) {
        sysml_state_elapsedSeconds_i32 = 0;
    }
    bool sysml_value_0 = 0;
    sysml_lazy_1 = sysml_state_elapsedSeconds_i32 >= 1;
    if (sysml_lazy_1) {
        sysml_lazy_1 = sysml_state_elapsedSeconds_i32 < 2;
    }
    sysml_lazy_2 = sysml_lazy_1;
    if (!sysml_lazy_2) {
        sysml_lazy_3 = sysml_state_elapsedSeconds_i32 >= 3;
        if (sysml_lazy_3) {
            sysml_lazy_3 = sysml_state_elapsedSeconds_i32 < 4;
        }
        sysml_lazy_2 = sysml_lazy_3;
    }
    if (sysml_lazy_2) {
        sysml_conditional_4 = false;
    }
    else {
        sysml_conditional_4 = true;
    }
    sysml_value_0 = sysml_conditional_4;
    bool sysml_value_1 = 0;
    sysml_lazy_5 = sysml_state_elapsedSeconds_i32 >= 1;
    if (sysml_lazy_5) {
        sysml_lazy_5 = sysml_state_elapsedSeconds_i32 < 2;
    }
    sysml_lazy_6 = sysml_lazy_5;
    if (!sysml_lazy_6) {
        sysml_lazy_7 = sysml_state_elapsedSeconds_i32 >= 2;
        if (sysml_lazy_7) {
            sysml_lazy_7 = sysml_state_elapsedSeconds_i32 < 3;
        }
        sysml_lazy_6 = sysml_lazy_7;
    }
    sysml_lazy_8 = sysml_lazy_6;
    if (!sysml_lazy_8) {
        sysml_lazy_9 = sysml_state_elapsedSeconds_i32 >= 3;
        if (sysml_lazy_9) {
            sysml_lazy_9 = sysml_state_elapsedSeconds_i32 < 4;
        }
        sysml_lazy_8 = sysml_lazy_9;
    }
    if (sysml_lazy_8) {
        sysml_conditional_10 = true;
    }
    else {
        sysml_conditional_10 = false;
    }
    sysml_value_1 = sysml_conditional_10;
    bool sysml_value_2 = 0;
    sysml_lazy_11 = sysml_state_elapsedSeconds_i32 >= 1;
    if (sysml_lazy_11) {
        sysml_lazy_11 = sysml_state_elapsedSeconds_i32 < 2;
    }
    sysml_lazy_12 = sysml_lazy_11;
    if (!sysml_lazy_12) {
        sysml_lazy_13 = sysml_state_elapsedSeconds_i32 >= 2;
        if (sysml_lazy_13) {
            sysml_lazy_13 = sysml_state_elapsedSeconds_i32 < 3;
        }
        sysml_lazy_12 = sysml_lazy_13;
    }
    if (sysml_lazy_12) {
        sysml_conditional_14 = true;
    }
    else {
        sysml_conditional_14 = false;
    }
    sysml_value_2 = sysml_conditional_14;
    bool sysml_value_3 = 0;
    sysml_lazy_15 = sysml_state_elapsedSeconds_i32 >= 2;
    if (sysml_lazy_15) {
        sysml_lazy_15 = sysml_state_elapsedSeconds_i32 < 3;
    }
    sysml_lazy_16 = sysml_lazy_15;
    if (!sysml_lazy_16) {
        sysml_lazy_17 = sysml_state_elapsedSeconds_i32 >= 3;
        if (sysml_lazy_17) {
            sysml_lazy_17 = sysml_state_elapsedSeconds_i32 < 4;
        }
        sysml_lazy_16 = sysml_lazy_17;
    }
    if (sysml_lazy_16) {
        sysml_conditional_18 = true;
    }
    else {
        sysml_conditional_18 = false;
    }
    sysml_value_3 = sysml_conditional_18;
    bool sysml_value_4 = 0;
    sysml_lazy_19 = sysml_state_elapsedSeconds_i32 >= 1;
    if (sysml_lazy_19) {
        sysml_lazy_19 = sysml_state_elapsedSeconds_i32 < 2;
    }
    sysml_lazy_20 = sysml_lazy_19;
    if (!sysml_lazy_20) {
        sysml_lazy_21 = sysml_state_elapsedSeconds_i32 >= 2;
        if (sysml_lazy_21) {
            sysml_lazy_21 = sysml_state_elapsedSeconds_i32 < 3;
        }
        sysml_lazy_20 = sysml_lazy_21;
    }
    sysml_lazy_22 = sysml_lazy_20;
    if (!sysml_lazy_22) {
        sysml_lazy_23 = sysml_state_elapsedSeconds_i32 >= 3;
        if (sysml_lazy_23) {
            sysml_lazy_23 = sysml_state_elapsedSeconds_i32 < 4;
        }
        sysml_lazy_22 = sysml_lazy_23;
    }
    if (sysml_lazy_22) {
        sysml_conditional_24 = true;
    }
    else {
        sysml_conditional_24 = false;
    }
    sysml_value_4 = sysml_conditional_24;
    bool sysml_value_5 = 0;
    sysml_lazy_25 = sysml_state_elapsedSeconds_i32 >= 1;
    if (sysml_lazy_25) {
        sysml_lazy_25 = sysml_state_elapsedSeconds_i32 < 2;
    }
    sysml_lazy_26 = sysml_lazy_25;
    if (!sysml_lazy_26) {
        sysml_lazy_27 = sysml_state_elapsedSeconds_i32 >= 2;
        if (sysml_lazy_27) {
            sysml_lazy_27 = sysml_state_elapsedSeconds_i32 < 3;
        }
        sysml_lazy_26 = sysml_lazy_27;
    }
    sysml_lazy_28 = sysml_lazy_26;
    if (!sysml_lazy_28) {
        sysml_lazy_29 = sysml_state_elapsedSeconds_i32 >= 3;
        if (sysml_lazy_29) {
            sysml_lazy_29 = sysml_state_elapsedSeconds_i32 < 4;
        }
        sysml_lazy_28 = sysml_lazy_29;
    }
    if (sysml_lazy_28) {
        sysml_conditional_30 = true;
    }
    else {
        sysml_conditional_30 = false;
    }
    sysml_value_5 = sysml_conditional_30;
    bool sysml_value_6 = 0;
    sysml_lazy_31 = sysml_state_elapsedSeconds_i32 >= 2;
    if (sysml_lazy_31) {
        sysml_lazy_31 = sysml_state_elapsedSeconds_i32 < 3;
    }
    sysml_lazy_32 = sysml_lazy_31;
    if (!sysml_lazy_32) {
        sysml_lazy_33 = sysml_state_elapsedSeconds_i32 >= 3;
        if (sysml_lazy_33) {
            sysml_lazy_33 = sysml_state_elapsedSeconds_i32 < 4;
        }
        sysml_lazy_32 = sysml_lazy_33;
    }
    if (sysml_lazy_32) {
        sysml_conditional_34 = false;
    }
    else {
        sysml_conditional_34 = true;
    }
    sysml_value_6 = sysml_conditional_34;
    bool sysml_value_7 = 0;
    sysml_lazy_35 = sysml_state_elapsedSeconds_i32 >= 3;
    if (sysml_lazy_35) {
        sysml_lazy_35 = sysml_state_elapsedSeconds_i32 < 4;
    }
    if (sysml_lazy_35) {
        sysml_conditional_36 = false;
    }
    else {
        sysml_conditional_36 = true;
    }
    sysml_value_7 = sysml_conditional_36;
    *io_1_segment_0 = sysml_value_0;
    *io_1_segment_1 = sysml_value_1;
    *io_1_segment_2 = sysml_value_2;
    *io_1_segment_3 = sysml_value_3;
    *io_1_segment_4 = sysml_value_4;
    *io_1_segment_5 = sysml_value_5;
    *io_1_segment_6 = sysml_value_6;
    *io_1_segment_7 = sysml_value_7;
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
static int32_t hw_value_io_0 = 0;
static bool hw_value_io_1_segment_0 = false;
static bool hw_value_io_1_segment_1 = false;
static bool hw_value_io_1_segment_2 = false;
static bool hw_value_io_1_segment_3 = false;
static bool hw_value_io_1_segment_4 = false;
static bool hw_value_io_1_segment_5 = false;
static bool hw_value_io_1_segment_6 = false;
static bool hw_value_io_1_segment_7 = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(7, OUTPUT);
  digitalWrite(7, hw_value_io_1_segment_0 ? HIGH : LOW);
  pinMode(9, OUTPUT);
  digitalWrite(9, hw_value_io_1_segment_1 ? HIGH : LOW);
  pinMode(5, OUTPUT);
  digitalWrite(5, hw_value_io_1_segment_2 ? HIGH : LOW);
  pinMode(8, OUTPUT);
  digitalWrite(8, hw_value_io_1_segment_3 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  digitalWrite(6, hw_value_io_1_segment_4 ? HIGH : LOW);
  pinMode(3, OUTPUT);
  digitalWrite(3, hw_value_io_1_segment_5 ? HIGH : LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, hw_value_io_1_segment_6 ? HIGH : LOW);
  pinMode(2, OUTPUT);
  digitalWrite(2, hw_value_io_1_segment_7 ? HIGH : LOW);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_0 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, &hw_value_io_1_segment_0, &hw_value_io_1_segment_1, &hw_value_io_1_segment_2, &hw_value_io_1_segment_3, &hw_value_io_1_segment_4, &hw_value_io_1_segment_5, &hw_value_io_1_segment_6, &hw_value_io_1_segment_7);
  digitalWrite(7, hw_value_io_1_segment_0 ? HIGH : LOW);
  digitalWrite(9, hw_value_io_1_segment_1 ? HIGH : LOW);
  digitalWrite(5, hw_value_io_1_segment_2 ? HIGH : LOW);
  digitalWrite(8, hw_value_io_1_segment_3 ? HIGH : LOW);
  digitalWrite(6, hw_value_io_1_segment_4 ? HIGH : LOW);
  digitalWrite(3, hw_value_io_1_segment_5 ? HIGH : LOW);
  digitalWrite(4, hw_value_io_1_segment_6 ? HIGH : LOW);
  digitalWrite(2, hw_value_io_1_segment_7 ? HIGH : LOW);
}
