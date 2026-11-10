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
extern bool sysml_state_initialized;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_6, double io_7, double* io_0, double* io_1, double* io_2, double* io_3, double* io_4, double* io_5, bool* io_8_segment_0, bool* io_8_segment_1, bool* io_8_segment_2, bool* io_8_segment_3, bool* io_8_segment_4, bool* io_8_segment_5, bool* io_8_segment_6, bool* io_8_segment_7);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_initialized = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_initialized = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_6, double io_7, double* io_0, double* io_1, double* io_2, double* io_3, double* io_4, double* io_5, bool* io_8_segment_0, bool* io_8_segment_1, bool* io_8_segment_2, bool* io_8_segment_3, bool* io_8_segment_4, bool* io_8_segment_5, bool* io_8_segment_6, bool* io_8_segment_7) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    double sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    double sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    double sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    double sysml_conditional_7 = 0;
    bool sysml_lazy_8 = 0;
    double sysml_conditional_9 = 0;
    bool sysml_lazy_10 = 0;
    double sysml_conditional_11 = 0;
    bool sysml_conditional_12 = 0;
    bool sysml_conditional_13 = 0;
    bool sysml_conditional_14 = 0;
    bool sysml_conditional_15 = 0;
    bool sysml_conditional_16 = 0;
    bool sysml_conditional_17 = 0;
    bool sysml_conditional_18 = 0;
    bool sysml_conditional_19 = 0;
    if ((!sysml_state_initialized)) {
        sysml_state_initialized = true;
    }
    double sysml_value_0 = 0;
    sysml_lazy_0 = io_6 >= 0.0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_6 <= 340.0;
    }
    if (sysml_lazy_0) {
        sysml_conditional_1 = 255.0;
    }
    else {
        sysml_conditional_1 = 0.0;
    }
    sysml_value_0 = sysml_conditional_1;
    double sysml_value_1 = 0;
    sysml_lazy_2 = io_6 >= 341.0;
    if (sysml_lazy_2) {
        sysml_lazy_2 = io_6 <= 681.0;
    }
    if (sysml_lazy_2) {
        sysml_conditional_3 = 255.0;
    }
    else {
        sysml_conditional_3 = 0.0;
    }
    sysml_value_1 = sysml_conditional_3;
    double sysml_value_2 = 0;
    sysml_lazy_4 = io_6 >= 682.0;
    if (sysml_lazy_4) {
        sysml_lazy_4 = io_6 <= 1023.0;
    }
    if (sysml_lazy_4) {
        sysml_conditional_5 = 255.0;
    }
    else {
        sysml_conditional_5 = 0.0;
    }
    sysml_value_2 = sysml_conditional_5;
    double sysml_value_3 = 0;
    sysml_lazy_6 = io_7 >= 0.0;
    if (sysml_lazy_6) {
        sysml_lazy_6 = io_7 <= 340.0;
    }
    if (sysml_lazy_6) {
        sysml_conditional_7 = 255.0;
    }
    else {
        sysml_conditional_7 = 0.0;
    }
    sysml_value_3 = sysml_conditional_7;
    double sysml_value_4 = 0;
    sysml_lazy_8 = io_7 >= 341.0;
    if (sysml_lazy_8) {
        sysml_lazy_8 = io_7 <= 681.0;
    }
    if (sysml_lazy_8) {
        sysml_conditional_9 = 255.0;
    }
    else {
        sysml_conditional_9 = 0.0;
    }
    sysml_value_4 = sysml_conditional_9;
    double sysml_value_5 = 0;
    sysml_lazy_10 = io_7 >= 682.0;
    if (sysml_lazy_10) {
        sysml_lazy_10 = io_7 <= 1023.0;
    }
    if (sysml_lazy_10) {
        sysml_conditional_11 = 255.0;
    }
    else {
        sysml_conditional_11 = 0.0;
    }
    sysml_value_5 = sysml_conditional_11;
    bool sysml_value_6 = 0;
    if ((((double)(io_6 + io_7)) / 2.0) >= (((double)1023.0) / 8.0)) {
        sysml_conditional_12 = true;
    }
    else {
        sysml_conditional_12 = false;
    }
    sysml_value_6 = sysml_conditional_12;
    bool sysml_value_7 = 0;
    if ((((double)(io_6 + io_7)) / 2.0) >= (((double)(2.0 * 1023.0)) / 8.0)) {
        sysml_conditional_13 = true;
    }
    else {
        sysml_conditional_13 = false;
    }
    sysml_value_7 = sysml_conditional_13;
    bool sysml_value_8 = 0;
    if ((((double)(io_6 + io_7)) / 2.0) >= (((double)(3.0 * 1023.0)) / 8.0)) {
        sysml_conditional_14 = true;
    }
    else {
        sysml_conditional_14 = false;
    }
    sysml_value_8 = sysml_conditional_14;
    bool sysml_value_9 = 0;
    if ((((double)(io_6 + io_7)) / 2.0) >= (((double)(4.0 * 1023.0)) / 8.0)) {
        sysml_conditional_15 = true;
    }
    else {
        sysml_conditional_15 = false;
    }
    sysml_value_9 = sysml_conditional_15;
    bool sysml_value_10 = 0;
    if ((((double)(io_6 + io_7)) / 2.0) >= (((double)(5.0 * 1023.0)) / 8.0)) {
        sysml_conditional_16 = true;
    }
    else {
        sysml_conditional_16 = false;
    }
    sysml_value_10 = sysml_conditional_16;
    bool sysml_value_11 = 0;
    if ((((double)(io_6 + io_7)) / 2.0) >= (((double)(6.0 * 1023.0)) / 8.0)) {
        sysml_conditional_17 = true;
    }
    else {
        sysml_conditional_17 = false;
    }
    sysml_value_11 = sysml_conditional_17;
    bool sysml_value_12 = 0;
    if ((((double)(io_6 + io_7)) / 2.0) >= (((double)(7.0 * 1023.0)) / 8.0)) {
        sysml_conditional_18 = true;
    }
    else {
        sysml_conditional_18 = false;
    }
    sysml_value_12 = sysml_conditional_18;
    bool sysml_value_13 = 0;
    if ((((double)(io_6 + io_7)) / 2.0) >= (((double)(8.0 * 1023.0)) / 8.0)) {
        sysml_conditional_19 = true;
    }
    else {
        sysml_conditional_19 = false;
    }
    sysml_value_13 = sysml_conditional_19;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
    *io_4 = sysml_value_4;
    *io_5 = sysml_value_5;
    *io_8_segment_0 = sysml_value_6;
    *io_8_segment_1 = sysml_value_7;
    *io_8_segment_2 = sysml_value_8;
    *io_8_segment_3 = sysml_value_9;
    *io_8_segment_4 = sysml_value_10;
    *io_8_segment_5 = sysml_value_11;
    *io_8_segment_6 = sysml_value_12;
    *io_8_segment_7 = sysml_value_13;
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
static double hw_value_io_1 = 0;
static double hw_value_io_2 = 0;
static double hw_value_io_3 = 0;
static double hw_value_io_4 = 0;
static double hw_value_io_5 = 0;
static double hw_value_io_6 = 0;
static uint32_t hw_sampled_io_6 = 0;
static bool hw_attempted_io_6 = false;
static double hw_value_io_7 = 0;
static uint32_t hw_sampled_io_7 = 0;
static bool hw_attempted_io_7 = false;
static bool hw_value_io_8_segment_0 = false;
static bool hw_value_io_8_segment_1 = false;
static bool hw_value_io_8_segment_2 = false;
static bool hw_value_io_8_segment_3 = false;
static bool hw_value_io_8_segment_4 = false;
static bool hw_value_io_8_segment_5 = false;
static bool hw_value_io_8_segment_6 = false;
static bool hw_value_io_8_segment_7 = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, OUTPUT);
  analogWrite(2, (int)constrain((hw_value_io_0 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(3, OUTPUT);
  analogWrite(3, (int)constrain((hw_value_io_1 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(4, OUTPUT);
  analogWrite(4, (int)constrain((hw_value_io_2 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(5, OUTPUT);
  analogWrite(5, (int)constrain((hw_value_io_3 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(6, OUTPUT);
  analogWrite(6, (int)constrain((hw_value_io_4 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(7, OUTPUT);
  analogWrite(7, (int)constrain((hw_value_io_5 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(8, OUTPUT);
  digitalWrite(8, hw_value_io_8_segment_0 ? HIGH : LOW);
  pinMode(9, OUTPUT);
  digitalWrite(9, hw_value_io_8_segment_1 ? HIGH : LOW);
  pinMode(10, OUTPUT);
  digitalWrite(10, hw_value_io_8_segment_2 ? HIGH : LOW);
  pinMode(11, OUTPUT);
  digitalWrite(11, hw_value_io_8_segment_3 ? HIGH : LOW);
  pinMode(12, OUTPUT);
  digitalWrite(12, hw_value_io_8_segment_4 ? HIGH : LOW);
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_8_segment_5 ? HIGH : LOW);
  pinMode(44, OUTPUT);
  digitalWrite(44, hw_value_io_8_segment_6 ? HIGH : LOW);
  pinMode(45, OUTPUT);
  digitalWrite(45, hw_value_io_8_segment_7 ? HIGH : LOW);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_6 || (uint32_t)(hw_now - hw_sampled_io_6) >= 10UL) {
    hw_attempted_io_6 = true; hw_sampled_io_6 = hw_now;
    hw_value_io_6 = (analogRead(A3) * 1.0 + 0.0);
  }
  if (!hw_attempted_io_7 || (uint32_t)(hw_now - hw_sampled_io_7) >= 10UL) {
    hw_attempted_io_7 = true; hw_sampled_io_7 = hw_now;
    hw_value_io_7 = (analogRead(A4) * 1.0 + 0.0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_6, hw_value_io_7, &hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_8_segment_0, &hw_value_io_8_segment_1, &hw_value_io_8_segment_2, &hw_value_io_8_segment_3, &hw_value_io_8_segment_4, &hw_value_io_8_segment_5, &hw_value_io_8_segment_6, &hw_value_io_8_segment_7);
  analogWrite(2, (int)constrain((hw_value_io_0 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(3, (int)constrain((hw_value_io_1 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(4, (int)constrain((hw_value_io_2 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(5, (int)constrain((hw_value_io_3 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(6, (int)constrain((hw_value_io_4 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(7, (int)constrain((hw_value_io_5 * 1.0 + 0.0), 0.0, 255.0));
  digitalWrite(8, hw_value_io_8_segment_0 ? HIGH : LOW);
  digitalWrite(9, hw_value_io_8_segment_1 ? HIGH : LOW);
  digitalWrite(10, hw_value_io_8_segment_2 ? HIGH : LOW);
  digitalWrite(11, hw_value_io_8_segment_3 ? HIGH : LOW);
  digitalWrite(12, hw_value_io_8_segment_4 ? HIGH : LOW);
  digitalWrite(13, hw_value_io_8_segment_5 ? HIGH : LOW);
  digitalWrite(44, hw_value_io_8_segment_6 ? HIGH : LOW);
  digitalWrite(45, hw_value_io_8_segment_7 ? HIGH : LOW);
}
