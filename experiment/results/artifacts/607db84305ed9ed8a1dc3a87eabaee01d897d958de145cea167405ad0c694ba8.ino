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
extern int32_t sysml_state_led1AccumulatorMs;
/* Global Variable define of  */
extern bool sysml_state_led1On;
/* Global Variable define of  */
extern int32_t sysml_state_rgbAccumulatorMs;
/* Global Variable define of  */
extern int32_t sysml_state_rgbIndex;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_10, bool* io_0, bool* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7, bool* io_8, bool* io_9);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_led1AccumulatorMs = 0;
/* Global Variable define of  */
bool sysml_state_led1On = false;
/* Global Variable define of  */
int32_t sysml_state_rgbAccumulatorMs = 0;
/* Global Variable define of  */
int32_t sysml_state_rgbIndex = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_led1AccumulatorMs = 0;
    sysml_state_led1On = false;
    sysml_state_rgbAccumulatorMs = 0;
    sysml_state_rgbIndex = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_10, bool* io_0, bool* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7, bool* io_8, bool* io_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int32_t sysml_conditional_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_conditional_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_conditional_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_lazy_10 = 0;
    bool sysml_lazy_11 = 0;
    bool sysml_conditional_12 = 0;
    bool sysml_lazy_13 = 0;
    bool sysml_lazy_14 = 0;
    bool sysml_lazy_15 = 0;
    bool sysml_conditional_16 = 0;
    bool sysml_lazy_17 = 0;
    bool sysml_lazy_18 = 0;
    bool sysml_lazy_19 = 0;
    bool sysml_conditional_20 = 0;
    bool sysml_lazy_21 = 0;
    bool sysml_lazy_22 = 0;
    bool sysml_lazy_23 = 0;
    bool sysml_conditional_24 = 0;
    bool sysml_lazy_25 = 0;
    bool sysml_lazy_26 = 0;
    bool sysml_lazy_27 = 0;
    bool sysml_conditional_28 = 0;
    bool sysml_lazy_29 = 0;
    bool sysml_lazy_30 = 0;
    bool sysml_lazy_31 = 0;
    bool sysml_conditional_32 = 0;
    bool sysml_lazy_33 = 0;
    bool sysml_lazy_34 = 0;
    bool sysml_lazy_35 = 0;
    bool sysml_conditional_36 = 0;
    sysml_state_led1AccumulatorMs = (sysml_state_led1AccumulatorMs + io_10);
    if ((sysml_state_led1AccumulatorMs >= 2000)) {
        sysml_state_led1AccumulatorMs = (sysml_state_led1AccumulatorMs - 2000);
        sysml_state_led1On = (!sysml_state_led1On);
    }
    sysml_state_rgbAccumulatorMs = (sysml_state_rgbAccumulatorMs + io_10);
    if ((sysml_state_rgbAccumulatorMs >= 2000)) {
        sysml_state_rgbAccumulatorMs = (sysml_state_rgbAccumulatorMs - 2000);
        if (sysml_state_rgbIndex >= 6) {
            sysml_conditional_0 = 0;
        }
        else {
            sysml_conditional_0 = sysml_state_rgbIndex + 1;
        }
        sysml_state_rgbIndex = sysml_conditional_0;
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_led1On;
    bool sysml_value_1 = 0;
    sysml_lazy_1 = sysml_state_rgbIndex == 0;
    if (!sysml_lazy_1) {
        sysml_lazy_1 = sysml_state_rgbIndex == 3;
    }
    sysml_lazy_2 = sysml_lazy_1;
    if (!sysml_lazy_2) {
        sysml_lazy_2 = sysml_state_rgbIndex == 5;
    }
    sysml_lazy_3 = sysml_lazy_2;
    if (!sysml_lazy_3) {
        sysml_lazy_3 = sysml_state_rgbIndex == 6;
    }
    if (sysml_lazy_3) {
        sysml_conditional_4 = true;
    }
    else {
        sysml_conditional_4 = false;
    }
    sysml_value_1 = sysml_conditional_4;
    bool sysml_value_2 = 0;
    sysml_lazy_5 = sysml_state_rgbIndex == 1;
    if (!sysml_lazy_5) {
        sysml_lazy_5 = sysml_state_rgbIndex == 3;
    }
    sysml_lazy_6 = sysml_lazy_5;
    if (!sysml_lazy_6) {
        sysml_lazy_6 = sysml_state_rgbIndex == 4;
    }
    sysml_lazy_7 = sysml_lazy_6;
    if (!sysml_lazy_7) {
        sysml_lazy_7 = sysml_state_rgbIndex == 6;
    }
    if (sysml_lazy_7) {
        sysml_conditional_8 = true;
    }
    else {
        sysml_conditional_8 = false;
    }
    sysml_value_2 = sysml_conditional_8;
    bool sysml_value_3 = 0;
    sysml_lazy_9 = sysml_state_rgbIndex == 2;
    if (!sysml_lazy_9) {
        sysml_lazy_9 = sysml_state_rgbIndex == 3;
    }
    sysml_lazy_10 = sysml_lazy_9;
    if (!sysml_lazy_10) {
        sysml_lazy_10 = sysml_state_rgbIndex == 5;
    }
    sysml_lazy_11 = sysml_lazy_10;
    if (!sysml_lazy_11) {
        sysml_lazy_11 = sysml_state_rgbIndex == 6;
    }
    if (sysml_lazy_11) {
        sysml_conditional_12 = true;
    }
    else {
        sysml_conditional_12 = false;
    }
    sysml_value_3 = sysml_conditional_12;
    bool sysml_value_4 = 0;
    sysml_lazy_13 = sysml_state_rgbIndex == 0;
    if (!sysml_lazy_13) {
        sysml_lazy_13 = sysml_state_rgbIndex == 3;
    }
    sysml_lazy_14 = sysml_lazy_13;
    if (!sysml_lazy_14) {
        sysml_lazy_14 = sysml_state_rgbIndex == 5;
    }
    sysml_lazy_15 = sysml_lazy_14;
    if (!sysml_lazy_15) {
        sysml_lazy_15 = sysml_state_rgbIndex == 6;
    }
    if (sysml_lazy_15) {
        sysml_conditional_16 = true;
    }
    else {
        sysml_conditional_16 = false;
    }
    sysml_value_4 = sysml_conditional_16;
    bool sysml_value_5 = 0;
    sysml_lazy_17 = sysml_state_rgbIndex == 1;
    if (!sysml_lazy_17) {
        sysml_lazy_17 = sysml_state_rgbIndex == 3;
    }
    sysml_lazy_18 = sysml_lazy_17;
    if (!sysml_lazy_18) {
        sysml_lazy_18 = sysml_state_rgbIndex == 4;
    }
    sysml_lazy_19 = sysml_lazy_18;
    if (!sysml_lazy_19) {
        sysml_lazy_19 = sysml_state_rgbIndex == 6;
    }
    if (sysml_lazy_19) {
        sysml_conditional_20 = true;
    }
    else {
        sysml_conditional_20 = false;
    }
    sysml_value_5 = sysml_conditional_20;
    bool sysml_value_6 = 0;
    sysml_lazy_21 = sysml_state_rgbIndex == 2;
    if (!sysml_lazy_21) {
        sysml_lazy_21 = sysml_state_rgbIndex == 3;
    }
    sysml_lazy_22 = sysml_lazy_21;
    if (!sysml_lazy_22) {
        sysml_lazy_22 = sysml_state_rgbIndex == 5;
    }
    sysml_lazy_23 = sysml_lazy_22;
    if (!sysml_lazy_23) {
        sysml_lazy_23 = sysml_state_rgbIndex == 6;
    }
    if (sysml_lazy_23) {
        sysml_conditional_24 = true;
    }
    else {
        sysml_conditional_24 = false;
    }
    sysml_value_6 = sysml_conditional_24;
    bool sysml_value_7 = 0;
    sysml_lazy_25 = sysml_state_rgbIndex == 0;
    if (!sysml_lazy_25) {
        sysml_lazy_25 = sysml_state_rgbIndex == 3;
    }
    sysml_lazy_26 = sysml_lazy_25;
    if (!sysml_lazy_26) {
        sysml_lazy_26 = sysml_state_rgbIndex == 5;
    }
    sysml_lazy_27 = sysml_lazy_26;
    if (!sysml_lazy_27) {
        sysml_lazy_27 = sysml_state_rgbIndex == 6;
    }
    if (sysml_lazy_27) {
        sysml_conditional_28 = true;
    }
    else {
        sysml_conditional_28 = false;
    }
    sysml_value_7 = sysml_conditional_28;
    bool sysml_value_8 = 0;
    sysml_lazy_29 = sysml_state_rgbIndex == 1;
    if (!sysml_lazy_29) {
        sysml_lazy_29 = sysml_state_rgbIndex == 3;
    }
    sysml_lazy_30 = sysml_lazy_29;
    if (!sysml_lazy_30) {
        sysml_lazy_30 = sysml_state_rgbIndex == 4;
    }
    sysml_lazy_31 = sysml_lazy_30;
    if (!sysml_lazy_31) {
        sysml_lazy_31 = sysml_state_rgbIndex == 6;
    }
    if (sysml_lazy_31) {
        sysml_conditional_32 = true;
    }
    else {
        sysml_conditional_32 = false;
    }
    sysml_value_8 = sysml_conditional_32;
    bool sysml_value_9 = 0;
    sysml_lazy_33 = sysml_state_rgbIndex == 2;
    if (!sysml_lazy_33) {
        sysml_lazy_33 = sysml_state_rgbIndex == 3;
    }
    sysml_lazy_34 = sysml_lazy_33;
    if (!sysml_lazy_34) {
        sysml_lazy_34 = sysml_state_rgbIndex == 5;
    }
    sysml_lazy_35 = sysml_lazy_34;
    if (!sysml_lazy_35) {
        sysml_lazy_35 = sysml_state_rgbIndex == 6;
    }
    if (sysml_lazy_35) {
        sysml_conditional_36 = true;
    }
    else {
        sysml_conditional_36 = false;
    }
    sysml_value_9 = sysml_conditional_36;
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
static bool hw_value_io_1 = true;
static bool hw_value_io_2 = false;
static bool hw_value_io_3 = false;
static bool hw_value_io_4 = true;
static bool hw_value_io_5 = false;
static bool hw_value_io_6 = false;
static bool hw_value_io_7 = true;
static bool hw_value_io_8 = false;
static bool hw_value_io_9 = false;
static int32_t hw_value_io_10 = 0;

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
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_10 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_10, &hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_io_7, &hw_value_io_8, &hw_value_io_9);
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
}
