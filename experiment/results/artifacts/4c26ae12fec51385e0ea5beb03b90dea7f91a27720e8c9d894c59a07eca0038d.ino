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
extern int32_t sysml_state_potI32;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool* io_1, bool* io_2, bool* io_3);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_potI32 = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_potI32 = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool* io_1, bool* io_2, bool* io_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int32_t sysml_conditional_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_conditional_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_lazy_10 = 0;
    bool sysml_lazy_11 = 0;
    bool sysml_lazy_12 = 0;
    bool sysml_lazy_13 = 0;
    bool sysml_lazy_14 = 0;
    bool sysml_lazy_15 = 0;
    bool sysml_conditional_16 = 0;
    bool sysml_lazy_17 = 0;
    bool sysml_lazy_18 = 0;
    bool sysml_lazy_19 = 0;
    bool sysml_lazy_20 = 0;
    bool sysml_lazy_21 = 0;
    bool sysml_lazy_22 = 0;
    bool sysml_lazy_23 = 0;
    bool sysml_conditional_24 = 0;
    if (io_0 < ((double)((int32_t)io_0))) {
        sysml_conditional_0 = 1;
    }
    else {
        sysml_conditional_0 = 0;
    }
    sysml_state_potI32 = ((int32_t)io_0) - sysml_conditional_0;
    bool sysml_value_0 = 0;
    sysml_lazy_1 = sysml_state_potI32 >= 128;
    if (sysml_lazy_1) {
        sysml_lazy_1 = sysml_state_potI32 <= 255;
    }
    sysml_lazy_2 = sysml_lazy_1;
    if (!sysml_lazy_2) {
        sysml_lazy_3 = sysml_state_potI32 >= 512;
        if (sysml_lazy_3) {
            sysml_lazy_3 = sysml_state_potI32 <= 639;
        }
        sysml_lazy_2 = sysml_lazy_3;
    }
    sysml_lazy_4 = sysml_lazy_2;
    if (!sysml_lazy_4) {
        sysml_lazy_5 = sysml_state_potI32 >= 640;
        if (sysml_lazy_5) {
            sysml_lazy_5 = sysml_state_potI32 <= 767;
        }
        sysml_lazy_4 = sysml_lazy_5;
    }
    sysml_lazy_6 = sysml_lazy_4;
    if (!sysml_lazy_6) {
        sysml_lazy_7 = sysml_state_potI32 >= 896;
        if (sysml_lazy_7) {
            sysml_lazy_7 = sysml_state_potI32 <= 1023;
        }
        sysml_lazy_6 = sysml_lazy_7;
    }
    if (sysml_lazy_6) {
        sysml_conditional_8 = true;
    }
    else {
        sysml_conditional_8 = false;
    }
    sysml_value_0 = sysml_conditional_8;
    bool sysml_value_1 = 0;
    sysml_lazy_9 = sysml_state_potI32 >= 256;
    if (sysml_lazy_9) {
        sysml_lazy_9 = sysml_state_potI32 <= 383;
    }
    sysml_lazy_10 = sysml_lazy_9;
    if (!sysml_lazy_10) {
        sysml_lazy_11 = sysml_state_potI32 >= 512;
        if (sysml_lazy_11) {
            sysml_lazy_11 = sysml_state_potI32 <= 639;
        }
        sysml_lazy_10 = sysml_lazy_11;
    }
    sysml_lazy_12 = sysml_lazy_10;
    if (!sysml_lazy_12) {
        sysml_lazy_13 = sysml_state_potI32 >= 768;
        if (sysml_lazy_13) {
            sysml_lazy_13 = sysml_state_potI32 <= 895;
        }
        sysml_lazy_12 = sysml_lazy_13;
    }
    sysml_lazy_14 = sysml_lazy_12;
    if (!sysml_lazy_14) {
        sysml_lazy_15 = sysml_state_potI32 >= 896;
        if (sysml_lazy_15) {
            sysml_lazy_15 = sysml_state_potI32 <= 1023;
        }
        sysml_lazy_14 = sysml_lazy_15;
    }
    if (sysml_lazy_14) {
        sysml_conditional_16 = true;
    }
    else {
        sysml_conditional_16 = false;
    }
    sysml_value_1 = sysml_conditional_16;
    bool sysml_value_2 = 0;
    sysml_lazy_17 = sysml_state_potI32 >= 384;
    if (sysml_lazy_17) {
        sysml_lazy_17 = sysml_state_potI32 <= 511;
    }
    sysml_lazy_18 = sysml_lazy_17;
    if (!sysml_lazy_18) {
        sysml_lazy_19 = sysml_state_potI32 >= 640;
        if (sysml_lazy_19) {
            sysml_lazy_19 = sysml_state_potI32 <= 767;
        }
        sysml_lazy_18 = sysml_lazy_19;
    }
    sysml_lazy_20 = sysml_lazy_18;
    if (!sysml_lazy_20) {
        sysml_lazy_21 = sysml_state_potI32 >= 768;
        if (sysml_lazy_21) {
            sysml_lazy_21 = sysml_state_potI32 <= 895;
        }
        sysml_lazy_20 = sysml_lazy_21;
    }
    sysml_lazy_22 = sysml_lazy_20;
    if (!sysml_lazy_22) {
        sysml_lazy_23 = sysml_state_potI32 >= 896;
        if (sysml_lazy_23) {
            sysml_lazy_23 = sysml_state_potI32 <= 1023;
        }
        sysml_lazy_22 = sysml_lazy_23;
    }
    if (sysml_lazy_22) {
        sysml_conditional_24 = true;
    }
    else {
        sysml_conditional_24 = false;
    }
    sysml_value_2 = sysml_conditional_24;
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
    *io_3 = sysml_value_2;
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
static double hw_value_io_0 = 0;
static uint32_t hw_sampled_io_0 = 0;
static bool hw_attempted_io_0 = false;
static bool hw_value_io_1 = false;
static bool hw_value_io_2 = false;
static bool hw_value_io_3 = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(A3, INPUT);
  pinMode(2, OUTPUT);
  digitalWrite(2, hw_value_io_1 ? HIGH : LOW);
  pinMode(3, OUTPUT);
  digitalWrite(3, hw_value_io_2 ? HIGH : LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, hw_value_io_3 ? HIGH : LOW);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 5UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (analogRead(A3) * 1.0 + 0.0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3);
  digitalWrite(2, hw_value_io_1 ? HIGH : LOW);
  digitalWrite(3, hw_value_io_2 ? HIGH : LOW);
  digitalWrite(4, hw_value_io_3 ? HIGH : LOW);
}
