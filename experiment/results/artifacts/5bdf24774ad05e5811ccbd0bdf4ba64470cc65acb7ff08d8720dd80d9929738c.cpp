#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\repaired_workflow_continuation_20261016\\embedbench_50\\workflow\\attempt_0\\sketch\\sketch.ino"
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
extern int32_t sysml_state_segIndex;
/* Global Variable define of  */
extern int32_t sysml_state_elapsedMs;
/* Global Variable define of  */
extern bool sysml_state_paused;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool* io_0, bool* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7, bool* io_8, bool* io_9, bool* io_10, bool* io_11, bool* io_12, bool* io_13);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_segIndex = 1;
/* Global Variable define of  */
int32_t sysml_state_elapsedMs = 0;
/* Global Variable define of  */
bool sysml_state_paused = false;
/* Init function of CompositeActor named "ProgramPage1" */
#line 386 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\repaired_workflow_continuation_20261016\\embedbench_50\\workflow\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 418 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\repaired_workflow_continuation_20261016\\embedbench_50\\workflow\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\repaired_workflow_continuation_20261016\\embedbench_50\\workflow\\attempt_0\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_segIndex = 1;
    sysml_state_elapsedMs = 0;
    sysml_state_paused = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool* io_0, bool* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7, bool* io_8, bool* io_9, bool* io_10, bool* io_11, bool* io_12, bool* io_13) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_conditional_0 = 0;
    bool sysml_conditional_1 = 0;
    bool sysml_conditional_2 = 0;
    bool sysml_conditional_3 = 0;
    bool sysml_conditional_4 = 0;
    bool sysml_conditional_5 = 0;
    bool sysml_conditional_6 = 0;
    bool sysml_conditional_7 = 0;
    bool sysml_lazy_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_lazy_10 = 0;
    bool sysml_lazy_11 = 0;
    bool sysml_lazy_12 = 0;
    bool sysml_lazy_13 = 0;
    bool sysml_lazy_14 = 0;
    bool sysml_conditional_15 = 0;
    bool sysml_lazy_16 = 0;
    bool sysml_lazy_17 = 0;
    bool sysml_lazy_18 = 0;
    bool sysml_lazy_19 = 0;
    bool sysml_lazy_20 = 0;
    bool sysml_lazy_21 = 0;
    bool sysml_lazy_22 = 0;
    bool sysml_conditional_23 = 0;
    bool sysml_lazy_24 = 0;
    bool sysml_lazy_25 = 0;
    bool sysml_lazy_26 = 0;
    bool sysml_lazy_27 = 0;
    bool sysml_lazy_28 = 0;
    bool sysml_lazy_29 = 0;
    bool sysml_lazy_30 = 0;
    bool sysml_conditional_31 = 0;
    bool sysml_lazy_32 = 0;
    bool sysml_lazy_33 = 0;
    bool sysml_lazy_34 = 0;
    bool sysml_lazy_35 = 0;
    bool sysml_lazy_36 = 0;
    bool sysml_lazy_37 = 0;
    bool sysml_lazy_38 = 0;
    bool sysml_conditional_39 = 0;
    bool sysml_lazy_40 = 0;
    bool sysml_lazy_41 = 0;
    bool sysml_lazy_42 = 0;
    bool sysml_lazy_43 = 0;
    bool sysml_lazy_44 = 0;
    bool sysml_lazy_45 = 0;
    bool sysml_lazy_46 = 0;
    bool sysml_conditional_47 = 0;
    if (sysml_state_paused) {
    }
    else {
        sysml_state_elapsedMs = sysml_state_elapsedMs;
        sysml_state_segIndex = sysml_state_segIndex;
    }
    bool sysml_value_0 = 0;
    if (sysml_state_segIndex == 1) {
        sysml_conditional_0 = true;
    }
    else {
        sysml_conditional_0 = false;
    }
    sysml_value_0 = sysml_conditional_0;
    bool sysml_value_1 = 0;
    if (sysml_state_segIndex == 2) {
        sysml_conditional_1 = true;
    }
    else {
        sysml_conditional_1 = false;
    }
    sysml_value_1 = sysml_conditional_1;
    bool sysml_value_2 = 0;
    if (sysml_state_segIndex == 3) {
        sysml_conditional_2 = true;
    }
    else {
        sysml_conditional_2 = false;
    }
    sysml_value_2 = sysml_conditional_2;
    bool sysml_value_3 = 0;
    if (sysml_state_segIndex == 4) {
        sysml_conditional_3 = true;
    }
    else {
        sysml_conditional_3 = false;
    }
    sysml_value_3 = sysml_conditional_3;
    bool sysml_value_4 = 0;
    if (sysml_state_segIndex == 5) {
        sysml_conditional_4 = true;
    }
    else {
        sysml_conditional_4 = false;
    }
    sysml_value_4 = sysml_conditional_4;
    bool sysml_value_5 = 0;
    if (sysml_state_segIndex == 6) {
        sysml_conditional_5 = true;
    }
    else {
        sysml_conditional_5 = false;
    }
    sysml_value_5 = sysml_conditional_5;
    bool sysml_value_6 = 0;
    if (sysml_state_segIndex == 7) {
        sysml_conditional_6 = true;
    }
    else {
        sysml_conditional_6 = false;
    }
    sysml_value_6 = sysml_conditional_6;
    bool sysml_value_7 = 0;
    if (sysml_state_segIndex == 8) {
        sysml_conditional_7 = true;
    }
    else {
        sysml_conditional_7 = false;
    }
    sysml_value_7 = sysml_conditional_7;
    bool sysml_value_8 = 0;
    sysml_lazy_8 = sysml_state_segIndex == 1;
    if (sysml_lazy_8) {
        sysml_lazy_8 = true;
    }
    sysml_lazy_9 = sysml_lazy_8;
    if (!sysml_lazy_9) {
        sysml_lazy_10 = sysml_state_segIndex == 2;
        if (sysml_lazy_10) {
            sysml_lazy_10 = true;
        }
        sysml_lazy_9 = sysml_lazy_10;
    }
    sysml_lazy_11 = sysml_lazy_9;
    if (!sysml_lazy_11) {
        sysml_lazy_12 = sysml_state_segIndex == 3;
        if (sysml_lazy_12) {
            sysml_lazy_12 = true;
        }
        sysml_lazy_11 = sysml_lazy_12;
    }
    sysml_lazy_13 = sysml_lazy_11;
    if (!sysml_lazy_13) {
        sysml_lazy_14 = sysml_state_segIndex == 4;
        if (sysml_lazy_14) {
            sysml_lazy_14 = true;
        }
        sysml_lazy_13 = sysml_lazy_14;
    }
    if (sysml_lazy_13) {
        sysml_conditional_15 = true;
    }
    else {
        sysml_conditional_15 = false;
    }
    sysml_value_8 = sysml_conditional_15;
    bool sysml_value_9 = 0;
    sysml_lazy_16 = sysml_state_segIndex == 5;
    if (sysml_lazy_16) {
        sysml_lazy_16 = true;
    }
    sysml_lazy_17 = sysml_lazy_16;
    if (!sysml_lazy_17) {
        sysml_lazy_18 = sysml_state_segIndex == 6;
        if (sysml_lazy_18) {
            sysml_lazy_18 = true;
        }
        sysml_lazy_17 = sysml_lazy_18;
    }
    sysml_lazy_19 = sysml_lazy_17;
    if (!sysml_lazy_19) {
        sysml_lazy_20 = sysml_state_segIndex == 7;
        if (sysml_lazy_20) {
            sysml_lazy_20 = true;
        }
        sysml_lazy_19 = sysml_lazy_20;
    }
    sysml_lazy_21 = sysml_lazy_19;
    if (!sysml_lazy_21) {
        sysml_lazy_22 = sysml_state_segIndex == 8;
        if (sysml_lazy_22) {
            sysml_lazy_22 = true;
        }
        sysml_lazy_21 = sysml_lazy_22;
    }
    if (sysml_lazy_21) {
        sysml_conditional_23 = true;
    }
    else {
        sysml_conditional_23 = false;
    }
    sysml_value_9 = sysml_conditional_23;
    bool sysml_value_10 = 0;
    sysml_value_10 = false;
    bool sysml_value_11 = 0;
    sysml_lazy_24 = sysml_state_segIndex == 5;
    if (sysml_lazy_24) {
        sysml_lazy_24 = true;
    }
    sysml_lazy_25 = sysml_lazy_24;
    if (!sysml_lazy_25) {
        sysml_lazy_26 = sysml_state_segIndex == 6;
        if (sysml_lazy_26) {
            sysml_lazy_26 = true;
        }
        sysml_lazy_25 = sysml_lazy_26;
    }
    sysml_lazy_27 = sysml_lazy_25;
    if (!sysml_lazy_27) {
        sysml_lazy_28 = sysml_state_segIndex == 7;
        if (sysml_lazy_28) {
            sysml_lazy_28 = true;
        }
        sysml_lazy_27 = sysml_lazy_28;
    }
    sysml_lazy_29 = sysml_lazy_27;
    if (!sysml_lazy_29) {
        sysml_lazy_30 = sysml_state_segIndex == 8;
        if (sysml_lazy_30) {
            sysml_lazy_30 = true;
        }
        sysml_lazy_29 = sysml_lazy_30;
    }
    if (sysml_lazy_29) {
        sysml_conditional_31 = true;
    }
    else {
        sysml_conditional_31 = false;
    }
    sysml_value_11 = sysml_conditional_31;
    bool sysml_value_12 = 0;
    sysml_lazy_32 = sysml_state_segIndex == 5;
    if (sysml_lazy_32) {
        sysml_lazy_32 = true;
    }
    sysml_lazy_33 = sysml_lazy_32;
    if (!sysml_lazy_33) {
        sysml_lazy_34 = sysml_state_segIndex == 6;
        if (sysml_lazy_34) {
            sysml_lazy_34 = true;
        }
        sysml_lazy_33 = sysml_lazy_34;
    }
    sysml_lazy_35 = sysml_lazy_33;
    if (!sysml_lazy_35) {
        sysml_lazy_36 = sysml_state_segIndex == 7;
        if (sysml_lazy_36) {
            sysml_lazy_36 = true;
        }
        sysml_lazy_35 = sysml_lazy_36;
    }
    sysml_lazy_37 = sysml_lazy_35;
    if (!sysml_lazy_37) {
        sysml_lazy_38 = sysml_state_segIndex == 8;
        if (sysml_lazy_38) {
            sysml_lazy_38 = true;
        }
        sysml_lazy_37 = sysml_lazy_38;
    }
    if (sysml_lazy_37) {
        sysml_conditional_39 = true;
    }
    else {
        sysml_conditional_39 = false;
    }
    sysml_value_12 = sysml_conditional_39;
    bool sysml_value_13 = 0;
    sysml_lazy_40 = sysml_state_segIndex == 1;
    if (sysml_lazy_40) {
        sysml_lazy_40 = true;
    }
    sysml_lazy_41 = sysml_lazy_40;
    if (!sysml_lazy_41) {
        sysml_lazy_42 = sysml_state_segIndex == 2;
        if (sysml_lazy_42) {
            sysml_lazy_42 = true;
        }
        sysml_lazy_41 = sysml_lazy_42;
    }
    sysml_lazy_43 = sysml_lazy_41;
    if (!sysml_lazy_43) {
        sysml_lazy_44 = sysml_state_segIndex == 3;
        if (sysml_lazy_44) {
            sysml_lazy_44 = true;
        }
        sysml_lazy_43 = sysml_lazy_44;
    }
    sysml_lazy_45 = sysml_lazy_43;
    if (!sysml_lazy_45) {
        sysml_lazy_46 = sysml_state_segIndex == 4;
        if (sysml_lazy_46) {
            sysml_lazy_46 = true;
        }
        sysml_lazy_45 = sysml_lazy_46;
    }
    if (sysml_lazy_45) {
        sysml_conditional_47 = true;
    }
    else {
        sysml_conditional_47 = false;
    }
    sysml_value_13 = sysml_conditional_47;
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

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(8, OUTPUT);
  digitalWrite(8, hw_value_io_0 ? HIGH : LOW);
  pinMode(9, OUTPUT);
  digitalWrite(9, hw_value_io_1 ? HIGH : LOW);
  pinMode(10, OUTPUT);
  digitalWrite(10, hw_value_io_2 ? HIGH : LOW);
  pinMode(11, OUTPUT);
  digitalWrite(11, hw_value_io_3 ? HIGH : LOW);
  pinMode(12, OUTPUT);
  digitalWrite(12, hw_value_io_4 ? HIGH : LOW);
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_5 ? HIGH : LOW);
  pinMode(44, OUTPUT);
  digitalWrite(44, hw_value_io_6 ? HIGH : LOW);
  pinMode(45, OUTPUT);
  digitalWrite(45, hw_value_io_7 ? HIGH : LOW);
  pinMode(2, OUTPUT);
  digitalWrite(2, hw_value_io_8 ? HIGH : LOW);
  pinMode(3, OUTPUT);
  digitalWrite(3, hw_value_io_9 ? HIGH : LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, hw_value_io_10 ? HIGH : LOW);
  pinMode(5, OUTPUT);
  digitalWrite(5, hw_value_io_11 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  digitalWrite(6, hw_value_io_12 ? HIGH : LOW);
  pinMode(7, OUTPUT);
  digitalWrite(7, hw_value_io_13 ? HIGH : LOW);
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
  ProgramPage1(&hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_io_7, &hw_value_io_8, &hw_value_io_9, &hw_value_io_10, &hw_value_io_11, &hw_value_io_12, &hw_value_io_13);
  digitalWrite(8, hw_value_io_0 ? HIGH : LOW);
  digitalWrite(9, hw_value_io_1 ? HIGH : LOW);
  digitalWrite(10, hw_value_io_2 ? HIGH : LOW);
  digitalWrite(11, hw_value_io_3 ? HIGH : LOW);
  digitalWrite(12, hw_value_io_4 ? HIGH : LOW);
  digitalWrite(13, hw_value_io_5 ? HIGH : LOW);
  digitalWrite(44, hw_value_io_6 ? HIGH : LOW);
  digitalWrite(45, hw_value_io_7 ? HIGH : LOW);
  digitalWrite(2, hw_value_io_8 ? HIGH : LOW);
  digitalWrite(3, hw_value_io_9 ? HIGH : LOW);
  digitalWrite(4, hw_value_io_10 ? HIGH : LOW);
  digitalWrite(5, hw_value_io_11 ? HIGH : LOW);
  digitalWrite(6, hw_value_io_12 ? HIGH : LOW);
  digitalWrite(7, hw_value_io_13 ? HIGH : LOW);
}

