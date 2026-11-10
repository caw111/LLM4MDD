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
extern int32_t sysml_state_stageIndex;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool* io_0, bool* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7, bool* io_8, bool* io_9, bool* io_10_segment_0, bool* io_10_segment_1, bool* io_10_segment_2, bool* io_10_segment_3);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_stageIndex = 1;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_stageIndex = 1;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool* io_0, bool* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7, bool* io_8, bool* io_9, bool* io_10_segment_0, bool* io_10_segment_1, bool* io_10_segment_2, bool* io_10_segment_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int32_t sysml_conditional_0 = 0;
    int32_t sysml_conditional_1 = 0;
    int32_t sysml_conditional_2 = 0;
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
    bool sysml_conditional_13 = 0;
    bool sysml_conditional_14 = 0;
    bool sysml_conditional_15 = 0;
    bool sysml_conditional_16 = 0;
    bool sysml_conditional_17 = 0;
    bool sysml_conditional_18 = 0;
    bool sysml_conditional_19 = 0;
    bool sysml_conditional_20 = 0;
    bool sysml_conditional_21 = 0;
    bool sysml_conditional_22 = 0;
    bool sysml_conditional_23 = 0;
    bool sysml_conditional_24 = 0;
    bool sysml_lazy_25 = 0;
    bool sysml_lazy_26 = 0;
    bool sysml_lazy_27 = 0;
    bool sysml_conditional_28 = 0;
    bool sysml_lazy_29 = 0;
    bool sysml_lazy_30 = 0;
    bool sysml_conditional_31 = 0;
    bool sysml_lazy_32 = 0;
    bool sysml_conditional_33 = 0;
    bool sysml_conditional_34 = 0;
    if (sysml_state_stageIndex == 1) {
        sysml_conditional_0 = 2;
    }
    else {
        if (sysml_state_stageIndex == 2) {
            sysml_conditional_1 = 3;
        }
        else {
            if (sysml_state_stageIndex == 3) {
                sysml_conditional_2 = 4;
            }
            else {
                sysml_conditional_2 = 1;
            }
            sysml_conditional_1 = sysml_conditional_2;
        }
        sysml_conditional_0 = sysml_conditional_1;
    }
    sysml_state_stageIndex = sysml_conditional_0;
    bool sysml_value_0 = 0;
    sysml_lazy_3 = sysml_state_stageIndex == 2;
    if (!sysml_lazy_3) {
        sysml_lazy_3 = sysml_state_stageIndex == 4;
    }
    if (sysml_lazy_3) {
        sysml_conditional_4 = true;
    }
    else {
        sysml_conditional_4 = false;
    }
    sysml_value_0 = sysml_conditional_4;
    bool sysml_value_1 = 0;
    if (sysml_state_stageIndex == 1) {
        sysml_conditional_5 = true;
    }
    else {
        if (sysml_state_stageIndex == 2) {
            sysml_conditional_6 = false;
        }
        else {
            if (sysml_state_stageIndex == 3) {
                sysml_conditional_7 = false;
            }
            else {
                sysml_conditional_7 = true;
            }
            sysml_conditional_6 = sysml_conditional_7;
        }
        sysml_conditional_5 = sysml_conditional_6;
    }
    sysml_value_1 = sysml_conditional_5;
    bool sysml_value_2 = 0;
    if (sysml_state_stageIndex == 2) {
        sysml_conditional_8 = true;
    }
    else {
        if (sysml_state_stageIndex == 3) {
            sysml_conditional_9 = false;
        }
        else {
            if (sysml_state_stageIndex == 4) {
                sysml_conditional_10 = true;
            }
            else {
                sysml_conditional_10 = false;
            }
            sysml_conditional_9 = sysml_conditional_10;
        }
        sysml_conditional_8 = sysml_conditional_9;
    }
    sysml_value_2 = sysml_conditional_8;
    bool sysml_value_3 = 0;
    if (sysml_state_stageIndex == 3) {
        sysml_conditional_11 = true;
    }
    else {
        if (sysml_state_stageIndex == 1) {
            sysml_conditional_12 = false;
        }
        else {
            if (sysml_state_stageIndex == 4) {
                sysml_conditional_13 = true;
            }
            else {
                sysml_conditional_13 = false;
            }
            sysml_conditional_12 = sysml_conditional_13;
        }
        sysml_conditional_11 = sysml_conditional_12;
    }
    sysml_value_3 = sysml_conditional_11;
    bool sysml_value_4 = 0;
    if (sysml_state_stageIndex == 3) {
        sysml_conditional_14 = true;
    }
    else {
        if (sysml_state_stageIndex == 2) {
            sysml_conditional_15 = false;
        }
        else {
            if (sysml_state_stageIndex == 4) {
                sysml_conditional_16 = true;
            }
            else {
                sysml_conditional_16 = false;
            }
            sysml_conditional_15 = sysml_conditional_16;
        }
        sysml_conditional_14 = sysml_conditional_15;
    }
    sysml_value_4 = sysml_conditional_14;
    bool sysml_value_5 = 0;
    if (sysml_state_stageIndex == 4) {
        sysml_conditional_17 = true;
    }
    else {
        sysml_conditional_17 = false;
    }
    sysml_value_5 = sysml_conditional_17;
    bool sysml_value_6 = 0;
    if (sysml_state_stageIndex == 2) {
        sysml_conditional_18 = true;
    }
    else {
        if (sysml_state_stageIndex == 4) {
            sysml_conditional_19 = true;
        }
        else {
            sysml_conditional_19 = false;
        }
        sysml_conditional_18 = sysml_conditional_19;
    }
    sysml_value_6 = sysml_conditional_18;
    bool sysml_value_7 = 0;
    if (sysml_state_stageIndex == 4) {
        sysml_conditional_20 = true;
    }
    else {
        sysml_conditional_20 = false;
    }
    sysml_value_7 = sysml_conditional_20;
    bool sysml_value_8 = 0;
    if (sysml_state_stageIndex == 3) {
        sysml_conditional_21 = true;
    }
    else {
        if (sysml_state_stageIndex == 4) {
            sysml_conditional_22 = true;
        }
        else {
            sysml_conditional_22 = false;
        }
        sysml_conditional_21 = sysml_conditional_22;
    }
    sysml_value_8 = sysml_conditional_21;
    bool sysml_value_9 = 0;
    if (sysml_state_stageIndex == 1) {
        sysml_conditional_23 = false;
    }
    else {
        if (sysml_state_stageIndex == 4) {
            sysml_conditional_24 = true;
        }
        else {
            sysml_conditional_24 = false;
        }
        sysml_conditional_23 = sysml_conditional_24;
    }
    sysml_value_9 = sysml_conditional_23;
    bool sysml_value_10 = 0;
    sysml_lazy_25 = sysml_state_stageIndex == 1;
    if (!sysml_lazy_25) {
        sysml_lazy_25 = sysml_state_stageIndex == 2;
    }
    sysml_lazy_26 = sysml_lazy_25;
    if (!sysml_lazy_26) {
        sysml_lazy_26 = sysml_state_stageIndex == 3;
    }
    sysml_lazy_27 = sysml_lazy_26;
    if (!sysml_lazy_27) {
        sysml_lazy_27 = sysml_state_stageIndex == 4;
    }
    if (sysml_lazy_27) {
        sysml_conditional_28 = true;
    }
    else {
        sysml_conditional_28 = false;
    }
    sysml_value_10 = sysml_conditional_28;
    bool sysml_value_11 = 0;
    sysml_lazy_29 = sysml_state_stageIndex == 2;
    if (!sysml_lazy_29) {
        sysml_lazy_29 = sysml_state_stageIndex == 3;
    }
    sysml_lazy_30 = sysml_lazy_29;
    if (!sysml_lazy_30) {
        sysml_lazy_30 = sysml_state_stageIndex == 4;
    }
    if (sysml_lazy_30) {
        sysml_conditional_31 = true;
    }
    else {
        sysml_conditional_31 = false;
    }
    sysml_value_11 = sysml_conditional_31;
    bool sysml_value_12 = 0;
    sysml_lazy_32 = sysml_state_stageIndex == 3;
    if (!sysml_lazy_32) {
        sysml_lazy_32 = sysml_state_stageIndex == 4;
    }
    if (sysml_lazy_32) {
        sysml_conditional_33 = true;
    }
    else {
        sysml_conditional_33 = false;
    }
    sysml_value_12 = sysml_conditional_33;
    bool sysml_value_13 = 0;
    if (sysml_state_stageIndex == 4) {
        sysml_conditional_34 = true;
    }
    else {
        sysml_conditional_34 = false;
    }
    sysml_value_13 = sysml_conditional_34;
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
    *io_10_segment_0 = sysml_value_10;
    *io_10_segment_1 = sysml_value_11;
    *io_10_segment_2 = sysml_value_12;
    *io_10_segment_3 = sysml_value_13;
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
static bool hw_value_io_10_segment_0 = false;
static bool hw_value_io_10_segment_1 = false;
static bool hw_value_io_10_segment_2 = false;
static bool hw_value_io_10_segment_3 = false;

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
  digitalWrite(12, hw_value_io_10_segment_0 ? HIGH : LOW);
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_10_segment_1 ? HIGH : LOW);
  pinMode(44, OUTPUT);
  digitalWrite(44, hw_value_io_10_segment_2 ? HIGH : LOW);
  pinMode(45, OUTPUT);
  digitalWrite(45, hw_value_io_10_segment_3 ? HIGH : LOW);
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
  ProgramPage1(&hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_io_7, &hw_value_io_8, &hw_value_io_9, &hw_value_io_10_segment_0, &hw_value_io_10_segment_1, &hw_value_io_10_segment_2, &hw_value_io_10_segment_3);
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
  digitalWrite(12, hw_value_io_10_segment_0 ? HIGH : LOW);
  digitalWrite(13, hw_value_io_10_segment_1 ? HIGH : LOW);
  digitalWrite(44, hw_value_io_10_segment_2 ? HIGH : LOW);
  digitalWrite(45, hw_value_io_10_segment_3 ? HIGH : LOW);
}
