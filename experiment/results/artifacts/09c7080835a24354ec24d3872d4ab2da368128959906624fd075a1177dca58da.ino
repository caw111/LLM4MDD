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
extern int32_t sysml_state_redIntensity;
/* Global Variable define of  */
extern int32_t sysml_state_greenIntensity;
/* Global Variable define of  */
extern int32_t sysml_state_blueIntensity;
/* Global Variable define of  */
extern int32_t sysml_state_runMs;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double* io_1, double* io_2, double* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7, bool* io_8, bool* io_9, bool* io_10, bool* io_11, bool* io_12, bool* io_13);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_redIntensity = 0;
/* Global Variable define of  */
int32_t sysml_state_greenIntensity = 0;
/* Global Variable define of  */
int32_t sysml_state_blueIntensity = 0;
/* Global Variable define of  */
int32_t sysml_state_runMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_redIntensity = 0;
    sysml_state_greenIntensity = 0;
    sysml_state_blueIntensity = 0;
    sysml_state_runMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double* io_1, double* io_2, double* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7, bool* io_8, bool* io_9, bool* io_10, bool* io_11, bool* io_12, bool* io_13) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int32_t sysml_conditional_0 = 0;
    int32_t sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    int32_t sysml_conditional_3 = 0;
    int32_t sysml_conditional_4 = 0;
    bool sysml_lazy_5 = 0;
    int32_t sysml_conditional_6 = 0;
    int32_t sysml_conditional_7 = 0;
    double sysml_conditional_8 = 0;
    double sysml_conditional_9 = 0;
    double sysml_conditional_10 = 0;
    double sysml_conditional_11 = 0;
    double sysml_conditional_12 = 0;
    double sysml_conditional_13 = 0;
    int32_t sysml_conditional_14 = 0;
    bool sysml_conditional_15 = 0;
    int32_t sysml_conditional_16 = 0;
    bool sysml_conditional_17 = 0;
    int32_t sysml_conditional_18 = 0;
    bool sysml_conditional_19 = 0;
    int32_t sysml_conditional_20 = 0;
    bool sysml_conditional_21 = 0;
    int32_t sysml_conditional_22 = 0;
    bool sysml_conditional_23 = 0;
    int32_t sysml_conditional_24 = 0;
    bool sysml_conditional_25 = 0;
    int32_t sysml_conditional_26 = 0;
    bool sysml_conditional_27 = 0;
    int32_t sysml_conditional_28 = 0;
    bool sysml_conditional_29 = 0;
    int32_t sysml_conditional_30 = 0;
    bool sysml_conditional_31 = 0;
    int32_t sysml_conditional_32 = 0;
    bool sysml_conditional_33 = 0;
    if (true) {
        sysml_state_runMs = (sysml_state_runMs + 5);
        if (((sysml_state_runMs - 0) - 150000) < 5000) {
            if (io_0 < ((double)((int32_t)io_0))) {
                sysml_conditional_1 = 1;
            }
            else {
                sysml_conditional_1 = 0;
            }
            sysml_conditional_0 = ((int32_t)io_0) - sysml_conditional_1;
        }
        else {
            sysml_conditional_0 = sysml_state_redIntensity;
        }
        sysml_state_redIntensity = sysml_conditional_0;
        sysml_lazy_2 = ((sysml_state_runMs - 0) - 150000) >= 5000;
        if (sysml_lazy_2) {
            sysml_lazy_2 = ((sysml_state_runMs - 0) - 150000) < 10000;
        }
        if (sysml_lazy_2) {
            if (io_0 < ((double)((int32_t)io_0))) {
                sysml_conditional_4 = 1;
            }
            else {
                sysml_conditional_4 = 0;
            }
            sysml_conditional_3 = ((int32_t)io_0) - sysml_conditional_4;
        }
        else {
            sysml_conditional_3 = sysml_state_greenIntensity;
        }
        sysml_state_greenIntensity = sysml_conditional_3;
        sysml_lazy_5 = ((sysml_state_runMs - 0) - 150000) >= 10000;
        if (sysml_lazy_5) {
            sysml_lazy_5 = ((sysml_state_runMs - 0) - 150000) < 15000;
        }
        if (sysml_lazy_5) {
            if (io_0 < ((double)((int32_t)io_0))) {
                sysml_conditional_7 = 1;
            }
            else {
                sysml_conditional_7 = 0;
            }
            sysml_conditional_6 = ((int32_t)io_0) - sysml_conditional_7;
        }
        else {
            sysml_conditional_6 = sysml_state_blueIntensity;
        }
        sysml_state_blueIntensity = sysml_conditional_6;
    }
    double sysml_value_0 = 0;
    if (sysml_state_redIntensity >= 0) {
        if (sysml_state_redIntensity <= 1023) {
            sysml_conditional_9 = ((double)(sysml_state_redIntensity * 255.0)) / 1023.0;
        }
        else {
            sysml_conditional_9 = 255.0;
        }
        sysml_conditional_8 = sysml_conditional_9;
    }
    else {
        sysml_conditional_8 = 0.0;
    }
    sysml_value_0 = sysml_conditional_8;
    double sysml_value_1 = 0;
    if (sysml_state_greenIntensity >= 0) {
        if (sysml_state_greenIntensity <= 1023) {
            sysml_conditional_11 = ((double)(sysml_state_greenIntensity * 255.0)) / 1023.0;
        }
        else {
            sysml_conditional_11 = 255.0;
        }
        sysml_conditional_10 = sysml_conditional_11;
    }
    else {
        sysml_conditional_10 = 0.0;
    }
    sysml_value_1 = sysml_conditional_10;
    double sysml_value_2 = 0;
    if (sysml_state_blueIntensity >= 0) {
        if (sysml_state_blueIntensity <= 1023) {
            sysml_conditional_13 = ((double)(sysml_state_blueIntensity * 255.0)) / 1023.0;
        }
        else {
            sysml_conditional_13 = 255.0;
        }
        sysml_conditional_12 = sysml_conditional_13;
    }
    else {
        sysml_conditional_12 = 0.0;
    }
    sysml_value_2 = sysml_conditional_12;
    bool sysml_value_3 = 0;
    if ((((double)(io_0 * 10.0)) / 1023.0) < ((double)((int32_t)(((double)(io_0 * 10.0)) / 1023.0)))) {
        sysml_conditional_14 = 1;
    }
    else {
        sysml_conditional_14 = 0;
    }
    if ((((int32_t)(((double)(io_0 * 10.0)) / 1023.0)) - sysml_conditional_14) >= 1) {
        sysml_conditional_15 = true;
    }
    else {
        sysml_conditional_15 = false;
    }
    sysml_value_3 = sysml_conditional_15;
    bool sysml_value_4 = 0;
    if ((((double)(io_0 * 10.0)) / 1023.0) < ((double)((int32_t)(((double)(io_0 * 10.0)) / 1023.0)))) {
        sysml_conditional_16 = 1;
    }
    else {
        sysml_conditional_16 = 0;
    }
    if ((((int32_t)(((double)(io_0 * 10.0)) / 1023.0)) - sysml_conditional_16) >= 2) {
        sysml_conditional_17 = true;
    }
    else {
        sysml_conditional_17 = false;
    }
    sysml_value_4 = sysml_conditional_17;
    bool sysml_value_5 = 0;
    if ((((double)(io_0 * 10.0)) / 1023.0) < ((double)((int32_t)(((double)(io_0 * 10.0)) / 1023.0)))) {
        sysml_conditional_18 = 1;
    }
    else {
        sysml_conditional_18 = 0;
    }
    if ((((int32_t)(((double)(io_0 * 10.0)) / 1023.0)) - sysml_conditional_18) >= 3) {
        sysml_conditional_19 = true;
    }
    else {
        sysml_conditional_19 = false;
    }
    sysml_value_5 = sysml_conditional_19;
    bool sysml_value_6 = 0;
    if ((((double)(io_0 * 10.0)) / 1023.0) < ((double)((int32_t)(((double)(io_0 * 10.0)) / 1023.0)))) {
        sysml_conditional_20 = 1;
    }
    else {
        sysml_conditional_20 = 0;
    }
    if ((((int32_t)(((double)(io_0 * 10.0)) / 1023.0)) - sysml_conditional_20) >= 4) {
        sysml_conditional_21 = true;
    }
    else {
        sysml_conditional_21 = false;
    }
    sysml_value_6 = sysml_conditional_21;
    bool sysml_value_7 = 0;
    if ((((double)(io_0 * 10.0)) / 1023.0) < ((double)((int32_t)(((double)(io_0 * 10.0)) / 1023.0)))) {
        sysml_conditional_22 = 1;
    }
    else {
        sysml_conditional_22 = 0;
    }
    if ((((int32_t)(((double)(io_0 * 10.0)) / 1023.0)) - sysml_conditional_22) >= 5) {
        sysml_conditional_23 = true;
    }
    else {
        sysml_conditional_23 = false;
    }
    sysml_value_7 = sysml_conditional_23;
    bool sysml_value_8 = 0;
    if ((((double)(io_0 * 10.0)) / 1023.0) < ((double)((int32_t)(((double)(io_0 * 10.0)) / 1023.0)))) {
        sysml_conditional_24 = 1;
    }
    else {
        sysml_conditional_24 = 0;
    }
    if ((((int32_t)(((double)(io_0 * 10.0)) / 1023.0)) - sysml_conditional_24) >= 6) {
        sysml_conditional_25 = true;
    }
    else {
        sysml_conditional_25 = false;
    }
    sysml_value_8 = sysml_conditional_25;
    bool sysml_value_9 = 0;
    if ((((double)(io_0 * 10.0)) / 1023.0) < ((double)((int32_t)(((double)(io_0 * 10.0)) / 1023.0)))) {
        sysml_conditional_26 = 1;
    }
    else {
        sysml_conditional_26 = 0;
    }
    if ((((int32_t)(((double)(io_0 * 10.0)) / 1023.0)) - sysml_conditional_26) >= 7) {
        sysml_conditional_27 = true;
    }
    else {
        sysml_conditional_27 = false;
    }
    sysml_value_9 = sysml_conditional_27;
    bool sysml_value_10 = 0;
    if ((((double)(io_0 * 10.0)) / 1023.0) < ((double)((int32_t)(((double)(io_0 * 10.0)) / 1023.0)))) {
        sysml_conditional_28 = 1;
    }
    else {
        sysml_conditional_28 = 0;
    }
    if ((((int32_t)(((double)(io_0 * 10.0)) / 1023.0)) - sysml_conditional_28) >= 8) {
        sysml_conditional_29 = true;
    }
    else {
        sysml_conditional_29 = false;
    }
    sysml_value_10 = sysml_conditional_29;
    bool sysml_value_11 = 0;
    if ((((double)(io_0 * 10.0)) / 1023.0) < ((double)((int32_t)(((double)(io_0 * 10.0)) / 1023.0)))) {
        sysml_conditional_30 = 1;
    }
    else {
        sysml_conditional_30 = 0;
    }
    if ((((int32_t)(((double)(io_0 * 10.0)) / 1023.0)) - sysml_conditional_30) >= 9) {
        sysml_conditional_31 = true;
    }
    else {
        sysml_conditional_31 = false;
    }
    sysml_value_11 = sysml_conditional_31;
    bool sysml_value_12 = 0;
    if ((((double)(io_0 * 10.0)) / 1023.0) < ((double)((int32_t)(((double)(io_0 * 10.0)) / 1023.0)))) {
        sysml_conditional_32 = 1;
    }
    else {
        sysml_conditional_32 = 0;
    }
    if ((((int32_t)(((double)(io_0 * 10.0)) / 1023.0)) - sysml_conditional_32) >= 10) {
        sysml_conditional_33 = true;
    }
    else {
        sysml_conditional_33 = false;
    }
    sysml_value_12 = sysml_conditional_33;
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
    *io_11 = sysml_value_10;
    *io_12 = sysml_value_11;
    *io_13 = sysml_value_12;
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
static double hw_value_io_1 = 0;
static double hw_value_io_2 = 0;
static double hw_value_io_3 = 0;
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
  pinMode(A3, INPUT);
  pinMode(2, OUTPUT);
  analogWrite(2, (int)constrain((hw_value_io_1 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(3, OUTPUT);
  analogWrite(3, (int)constrain((hw_value_io_2 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(4, OUTPUT);
  analogWrite(4, (int)constrain((hw_value_io_3 * 1.0 + 0.0), 0.0, 255.0));
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
  pinMode(12, OUTPUT);
  digitalWrite(12, hw_value_io_11 ? HIGH : LOW);
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_12 ? HIGH : LOW);
  pinMode(44, OUTPUT);
  digitalWrite(44, hw_value_io_13 ? HIGH : LOW);
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
  ProgramPage1(hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_io_7, &hw_value_io_8, &hw_value_io_9, &hw_value_io_10, &hw_value_io_11, &hw_value_io_12, &hw_value_io_13);
  analogWrite(2, (int)constrain((hw_value_io_1 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(3, (int)constrain((hw_value_io_2 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(4, (int)constrain((hw_value_io_3 * 1.0 + 0.0), 0.0, 255.0));
  digitalWrite(5, hw_value_io_4 ? HIGH : LOW);
  digitalWrite(6, hw_value_io_5 ? HIGH : LOW);
  digitalWrite(7, hw_value_io_6 ? HIGH : LOW);
  digitalWrite(8, hw_value_io_7 ? HIGH : LOW);
  digitalWrite(9, hw_value_io_8 ? HIGH : LOW);
  digitalWrite(10, hw_value_io_9 ? HIGH : LOW);
  digitalWrite(11, hw_value_io_10 ? HIGH : LOW);
  digitalWrite(12, hw_value_io_11 ? HIGH : LOW);
  digitalWrite(13, hw_value_io_12 ? HIGH : LOW);
  digitalWrite(44, hw_value_io_13 ? HIGH : LOW);
}
