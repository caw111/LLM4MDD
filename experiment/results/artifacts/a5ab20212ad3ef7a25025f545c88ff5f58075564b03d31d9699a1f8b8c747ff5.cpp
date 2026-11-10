#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\cases\\pceval_7_segment_display_counter\\arduino\\sketch\\sketch.ino"
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
extern int32_t sysml_state_cycleMilliseconds;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_8, bool* io_0, bool* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_cycleMilliseconds = 0;
/* Init function of CompositeActor named "ProgramPage1" */
#line 180 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\cases\\pceval_7_segment_display_counter\\arduino\\sketch\\sketch.ino"
void setup();
#line 200 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\cases\\pceval_7_segment_display_counter\\arduino\\sketch\\sketch.ino"
void loop();
#line 29 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\cases\\pceval_7_segment_display_counter\\arduino\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_cycleMilliseconds = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_8, bool* io_0, bool* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int32_t sysml_conditional_0 = 0;
    int32_t sysml_arithmetic_1 = 0;
    int32_t sysml_conditional_2 = 0;
    int32_t sysml_arithmetic_3 = 0;
    int32_t sysml_conditional_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_lazy_10 = 0;
    bool sysml_lazy_11 = 0;
    bool sysml_lazy_12 = 0;
    bool sysml_lazy_13 = 0;
    bool sysml_lazy_14 = 0;
    bool sysml_lazy_15 = 0;
    bool sysml_lazy_16 = 0;
    bool sysml_lazy_17 = 0;
    bool sysml_lazy_18 = 0;
    if ((((double)io_8) / 10000) < ((double)((int32_t)(((double)io_8) / 10000)))) {
        sysml_conditional_0 = 1;
    }
    else {
        sysml_conditional_0 = 0;
    }
    sysml_arithmetic_1 = ((int32_t)(((double)io_8) / 10000)) - sysml_conditional_0;
    sysml_state_cycleMilliseconds = sysml_state_cycleMilliseconds + (io_8 - (10000 * sysml_arithmetic_1));
    if ((((double)sysml_state_cycleMilliseconds) / 10000) < ((double)((int32_t)(((double)sysml_state_cycleMilliseconds) / 10000)))) {
        sysml_conditional_2 = 1;
    }
    else {
        sysml_conditional_2 = 0;
    }
    sysml_arithmetic_3 = ((int32_t)(((double)sysml_state_cycleMilliseconds) / 10000)) - sysml_conditional_2;
    sysml_state_cycleMilliseconds = sysml_state_cycleMilliseconds - (10000 * sysml_arithmetic_3);
    int32_t sysml_value_0 = 0;
    if ((((double)sysml_state_cycleMilliseconds) / 1000) < ((double)((int32_t)(((double)sysml_state_cycleMilliseconds) / 1000)))) {
        sysml_conditional_4 = 1;
    }
    else {
        sysml_conditional_4 = 0;
    }
    sysml_value_0 = ((int32_t)(((double)sysml_state_cycleMilliseconds) / 1000)) - sysml_conditional_4;
    bool sysml_value_1 = 0;
    sysml_value_1 = false;
    bool sysml_value_2 = 0;
    sysml_lazy_5 = sysml_value_0 != 1;
    if (sysml_lazy_5) {
        sysml_lazy_5 = sysml_value_0 != 4;
    }
    sysml_value_2 = sysml_lazy_5;
    bool sysml_value_3 = 0;
    sysml_lazy_6 = sysml_value_0 != 5;
    if (sysml_lazy_6) {
        sysml_lazy_6 = sysml_value_0 != 6;
    }
    sysml_value_3 = sysml_lazy_6;
    bool sysml_value_4 = 0;
    sysml_value_4 = (sysml_value_0 != 2);
    bool sysml_value_5 = 0;
    sysml_lazy_7 = sysml_value_0 != 1;
    if (sysml_lazy_7) {
        sysml_lazy_7 = sysml_value_0 != 4;
    }
    sysml_lazy_8 = sysml_lazy_7;
    if (sysml_lazy_8) {
        sysml_lazy_8 = sysml_value_0 != 7;
    }
    sysml_value_5 = sysml_lazy_8;
    bool sysml_value_6 = 0;
    sysml_lazy_9 = sysml_value_0 == 0;
    if (!sysml_lazy_9) {
        sysml_lazy_9 = sysml_value_0 == 2;
    }
    sysml_lazy_10 = sysml_lazy_9;
    if (!sysml_lazy_10) {
        sysml_lazy_10 = sysml_value_0 == 6;
    }
    sysml_lazy_11 = sysml_lazy_10;
    if (!sysml_lazy_11) {
        sysml_lazy_11 = sysml_value_0 == 8;
    }
    sysml_value_6 = sysml_lazy_11;
    bool sysml_value_7 = 0;
    sysml_lazy_12 = sysml_value_0 == 0;
    if (!sysml_lazy_12) {
        sysml_lazy_12 = sysml_value_0 == 4;
    }
    sysml_lazy_13 = sysml_lazy_12;
    if (!sysml_lazy_13) {
        sysml_lazy_13 = sysml_value_0 == 5;
    }
    sysml_lazy_14 = sysml_lazy_13;
    if (!sysml_lazy_14) {
        sysml_lazy_14 = sysml_value_0 == 6;
    }
    sysml_lazy_15 = sysml_lazy_14;
    if (!sysml_lazy_15) {
        sysml_lazy_15 = sysml_value_0 == 8;
    }
    sysml_lazy_16 = sysml_lazy_15;
    if (!sysml_lazy_16) {
        sysml_lazy_16 = sysml_value_0 == 9;
    }
    sysml_value_7 = sysml_lazy_16;
    bool sysml_value_8 = 0;
    sysml_lazy_17 = sysml_value_0 != 0;
    if (sysml_lazy_17) {
        sysml_lazy_17 = sysml_value_0 != 1;
    }
    sysml_lazy_18 = sysml_lazy_17;
    if (sysml_lazy_18) {
        sysml_lazy_18 = sysml_value_0 != 7;
    }
    sysml_value_8 = sysml_lazy_18;
    *io_0 = sysml_value_2;
    *io_1 = sysml_value_3;
    *io_2 = sysml_value_4;
    *io_3 = sysml_value_5;
    *io_4 = sysml_value_6;
    *io_5 = sysml_value_7;
    *io_6 = sysml_value_8;
    *io_7 = sysml_value_1;
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
static bool hw_value_io_0 = true;
static bool hw_value_io_1 = true;
static bool hw_value_io_2 = true;
static bool hw_value_io_3 = true;
static bool hw_value_io_4 = true;
static bool hw_value_io_5 = true;
static bool hw_value_io_6 = false;
static bool hw_value_io_7 = false;
static int32_t hw_value_io_8 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, OUTPUT);
  digitalWrite(2, hw_value_io_0 ? LOW : HIGH);
  pinMode(3, OUTPUT);
  digitalWrite(3, hw_value_io_1 ? LOW : HIGH);
  pinMode(4, OUTPUT);
  digitalWrite(4, hw_value_io_2 ? LOW : HIGH);
  pinMode(5, OUTPUT);
  digitalWrite(5, hw_value_io_3 ? LOW : HIGH);
  pinMode(6, OUTPUT);
  digitalWrite(6, hw_value_io_4 ? LOW : HIGH);
  pinMode(7, OUTPUT);
  digitalWrite(7, hw_value_io_5 ? LOW : HIGH);
  pinMode(8, OUTPUT);
  digitalWrite(8, hw_value_io_6 ? LOW : HIGH);
  pinMode(9, OUTPUT);
  digitalWrite(9, hw_value_io_7 ? LOW : HIGH);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : hw_now;
  if (hw_started && hw_elapsed < 1UL) return;
  if (hw_elapsed > 2147483647UL) return;
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : hw_now;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_8 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_8, &hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_io_7);
  digitalWrite(2, hw_value_io_0 ? LOW : HIGH);
  digitalWrite(3, hw_value_io_1 ? LOW : HIGH);
  digitalWrite(4, hw_value_io_2 ? LOW : HIGH);
  digitalWrite(5, hw_value_io_3 ? LOW : HIGH);
  digitalWrite(6, hw_value_io_4 ? LOW : HIGH);
  digitalWrite(7, hw_value_io_5 ? LOW : HIGH);
  digitalWrite(8, hw_value_io_6 ? LOW : HIGH);
  digitalWrite(9, hw_value_io_7 ? LOW : HIGH);
}

