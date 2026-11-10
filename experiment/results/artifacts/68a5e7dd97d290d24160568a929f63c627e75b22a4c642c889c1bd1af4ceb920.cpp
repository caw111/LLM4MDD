#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_54\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
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
extern int32_t sysml_state_elapsedModeMs;
/* Global Variable define of  */
extern int32_t sysml_state_modeIndex;
/* Global Variable define of  */
extern int32_t sysml_state_currentRed;
/* Global Variable define of  */
extern int32_t sysml_state_currentGreen;
/* Global Variable define of  */
extern int32_t sysml_state_currentBlue;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_3, double* io_0, double* io_1, double* io_2, bool* io_4_segment_0, bool* io_4_segment_1, bool* io_4_segment_2, bool* io_4_segment_3, bool* io_4_segment_4, bool* io_4_segment_5, bool* io_4_segment_6, bool* io_4_segment_7, bool* io_4_segment_8, bool* io_4_segment_9);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_elapsedModeMs = 0;
/* Global Variable define of  */
int32_t sysml_state_modeIndex = 0;
/* Global Variable define of  */
int32_t sysml_state_currentRed = 0;
/* Global Variable define of  */
int32_t sysml_state_currentGreen = 0;
/* Global Variable define of  */
int32_t sysml_state_currentBlue = 0;
/* Init function of CompositeActor named "ProgramPage1" */
#line 240 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_54\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void setup();
#line 271 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_54\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void loop();
#line 45 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_54\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_elapsedModeMs = 0;
    sysml_state_modeIndex = 0;
    sysml_state_currentRed = 0;
    sysml_state_currentGreen = 0;
    sysml_state_currentBlue = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_3, double* io_0, double* io_1, double* io_2, bool* io_4_segment_0, bool* io_4_segment_1, bool* io_4_segment_2, bool* io_4_segment_3, bool* io_4_segment_4, bool* io_4_segment_5, bool* io_4_segment_6, bool* io_4_segment_7, bool* io_4_segment_8, bool* io_4_segment_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int32_t sysml_conditional_0 = 0;
    int32_t sysml_conditional_1 = 0;
    int32_t sysml_conditional_2 = 0;
    int32_t sysml_conditional_3 = 0;
    int32_t sysml_conditional_4 = 0;
    int32_t sysml_conditional_5 = 0;
    int32_t sysml_conditional_6 = 0;
    int32_t sysml_conditional_7 = 0;
    int32_t sysml_conditional_8 = 0;
    int32_t sysml_conditional_9 = 0;
    int32_t sysml_conditional_10 = 0;
    int32_t sysml_conditional_11 = 0;
    int32_t sysml_conditional_12 = 0;
    int32_t sysml_conditional_13 = 0;
    int32_t sysml_conditional_14 = 0;
    int32_t sysml_conditional_15 = 0;
    sysml_state_elapsedModeMs = 0;
    sysml_state_modeIndex = 0;
    if (sysml_state_modeIndex == 0) {
        if (io_3 < ((double)((int32_t)io_3))) {
            sysml_conditional_1 = 1;
        }
        else {
            sysml_conditional_1 = 0;
        }
        sysml_conditional_0 = ((int32_t)io_3) - sysml_conditional_1;
    }
    else {
        sysml_conditional_0 = sysml_state_currentRed;
    }
    sysml_state_currentRed = sysml_conditional_0;
    if (sysml_state_modeIndex == 1) {
        if (io_3 < ((double)((int32_t)io_3))) {
            sysml_conditional_3 = 1;
        }
        else {
            sysml_conditional_3 = 0;
        }
        sysml_conditional_2 = ((int32_t)io_3) - sysml_conditional_3;
    }
    else {
        sysml_conditional_2 = sysml_state_currentGreen;
    }
    sysml_state_currentGreen = sysml_conditional_2;
    if (sysml_state_modeIndex == 2) {
        if (io_3 < ((double)((int32_t)io_3))) {
            sysml_conditional_5 = 1;
        }
        else {
            sysml_conditional_5 = 0;
        }
        sysml_conditional_4 = ((int32_t)io_3) - sysml_conditional_5;
    }
    else {
        sysml_conditional_4 = sysml_state_currentBlue;
    }
    sysml_state_currentBlue = sysml_conditional_4;
    double sysml_value_0 = 0;
    sysml_value_0 = (sysml_state_currentRed * 1.0);
    double sysml_value_1 = 0;
    sysml_value_1 = (sysml_state_currentGreen * 1.0);
    double sysml_value_2 = 0;
    sysml_value_2 = (sysml_state_currentBlue * 1.0);
    bool sysml_value_3 = 0;
    if ((((double)(io_3 * 10.0)) / 1023.0) < ((double)((int32_t)(((double)(io_3 * 10.0)) / 1023.0)))) {
        sysml_conditional_6 = 1;
    }
    else {
        sysml_conditional_6 = 0;
    }
    sysml_value_3 = (((int32_t)(((double)(io_3 * 10.0)) / 1023.0)) - sysml_conditional_6) >= 1;
    bool sysml_value_4 = 0;
    if ((((double)(io_3 * 10.0)) / 1023.0) < ((double)((int32_t)(((double)(io_3 * 10.0)) / 1023.0)))) {
        sysml_conditional_7 = 1;
    }
    else {
        sysml_conditional_7 = 0;
    }
    sysml_value_4 = (((int32_t)(((double)(io_3 * 10.0)) / 1023.0)) - sysml_conditional_7) >= 2;
    bool sysml_value_5 = 0;
    if ((((double)(io_3 * 10.0)) / 1023.0) < ((double)((int32_t)(((double)(io_3 * 10.0)) / 1023.0)))) {
        sysml_conditional_8 = 1;
    }
    else {
        sysml_conditional_8 = 0;
    }
    sysml_value_5 = (((int32_t)(((double)(io_3 * 10.0)) / 1023.0)) - sysml_conditional_8) >= 3;
    bool sysml_value_6 = 0;
    if ((((double)(io_3 * 10.0)) / 1023.0) < ((double)((int32_t)(((double)(io_3 * 10.0)) / 1023.0)))) {
        sysml_conditional_9 = 1;
    }
    else {
        sysml_conditional_9 = 0;
    }
    sysml_value_6 = (((int32_t)(((double)(io_3 * 10.0)) / 1023.0)) - sysml_conditional_9) >= 4;
    bool sysml_value_7 = 0;
    if ((((double)(io_3 * 10.0)) / 1023.0) < ((double)((int32_t)(((double)(io_3 * 10.0)) / 1023.0)))) {
        sysml_conditional_10 = 1;
    }
    else {
        sysml_conditional_10 = 0;
    }
    sysml_value_7 = (((int32_t)(((double)(io_3 * 10.0)) / 1023.0)) - sysml_conditional_10) >= 5;
    bool sysml_value_8 = 0;
    if ((((double)(io_3 * 10.0)) / 1023.0) < ((double)((int32_t)(((double)(io_3 * 10.0)) / 1023.0)))) {
        sysml_conditional_11 = 1;
    }
    else {
        sysml_conditional_11 = 0;
    }
    sysml_value_8 = (((int32_t)(((double)(io_3 * 10.0)) / 1023.0)) - sysml_conditional_11) >= 6;
    bool sysml_value_9 = 0;
    if ((((double)(io_3 * 10.0)) / 1023.0) < ((double)((int32_t)(((double)(io_3 * 10.0)) / 1023.0)))) {
        sysml_conditional_12 = 1;
    }
    else {
        sysml_conditional_12 = 0;
    }
    sysml_value_9 = (((int32_t)(((double)(io_3 * 10.0)) / 1023.0)) - sysml_conditional_12) >= 7;
    bool sysml_value_10 = 0;
    if ((((double)(io_3 * 10.0)) / 1023.0) < ((double)((int32_t)(((double)(io_3 * 10.0)) / 1023.0)))) {
        sysml_conditional_13 = 1;
    }
    else {
        sysml_conditional_13 = 0;
    }
    sysml_value_10 = (((int32_t)(((double)(io_3 * 10.0)) / 1023.0)) - sysml_conditional_13) >= 8;
    bool sysml_value_11 = 0;
    if ((((double)(io_3 * 10.0)) / 1023.0) < ((double)((int32_t)(((double)(io_3 * 10.0)) / 1023.0)))) {
        sysml_conditional_14 = 1;
    }
    else {
        sysml_conditional_14 = 0;
    }
    sysml_value_11 = (((int32_t)(((double)(io_3 * 10.0)) / 1023.0)) - sysml_conditional_14) >= 9;
    bool sysml_value_12 = 0;
    if ((((double)(io_3 * 10.0)) / 1023.0) < ((double)((int32_t)(((double)(io_3 * 10.0)) / 1023.0)))) {
        sysml_conditional_15 = 1;
    }
    else {
        sysml_conditional_15 = 0;
    }
    sysml_value_12 = (((int32_t)(((double)(io_3 * 10.0)) / 1023.0)) - sysml_conditional_15) >= 10;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_4_segment_0 = sysml_value_3;
    *io_4_segment_1 = sysml_value_4;
    *io_4_segment_2 = sysml_value_5;
    *io_4_segment_3 = sysml_value_6;
    *io_4_segment_4 = sysml_value_7;
    *io_4_segment_5 = sysml_value_8;
    *io_4_segment_6 = sysml_value_9;
    *io_4_segment_7 = sysml_value_10;
    *io_4_segment_8 = sysml_value_11;
    *io_4_segment_9 = sysml_value_12;
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
static uint32_t hw_sampled_io_3 = 0;
static bool hw_attempted_io_3 = false;
static bool hw_value_io_4_segment_0 = false;
static bool hw_value_io_4_segment_1 = false;
static bool hw_value_io_4_segment_2 = false;
static bool hw_value_io_4_segment_3 = false;
static bool hw_value_io_4_segment_4 = false;
static bool hw_value_io_4_segment_5 = false;
static bool hw_value_io_4_segment_6 = false;
static bool hw_value_io_4_segment_7 = false;
static bool hw_value_io_4_segment_8 = false;
static bool hw_value_io_4_segment_9 = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, OUTPUT);
  analogWrite(2, (int)constrain((hw_value_io_0 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(3, OUTPUT);
  analogWrite(3, (int)constrain((hw_value_io_1 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(4, OUTPUT);
  analogWrite(4, (int)constrain((hw_value_io_2 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(A3, INPUT);
  pinMode(5, OUTPUT);
  digitalWrite(5, hw_value_io_4_segment_0 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  digitalWrite(6, hw_value_io_4_segment_1 ? HIGH : LOW);
  pinMode(7, OUTPUT);
  digitalWrite(7, hw_value_io_4_segment_2 ? HIGH : LOW);
  pinMode(8, OUTPUT);
  digitalWrite(8, hw_value_io_4_segment_3 ? HIGH : LOW);
  pinMode(9, OUTPUT);
  digitalWrite(9, hw_value_io_4_segment_4 ? HIGH : LOW);
  pinMode(10, OUTPUT);
  digitalWrite(10, hw_value_io_4_segment_5 ? HIGH : LOW);
  pinMode(11, OUTPUT);
  digitalWrite(11, hw_value_io_4_segment_6 ? HIGH : LOW);
  pinMode(12, OUTPUT);
  digitalWrite(12, hw_value_io_4_segment_7 ? HIGH : LOW);
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_4_segment_8 ? HIGH : LOW);
  pinMode(44, OUTPUT);
  digitalWrite(44, hw_value_io_4_segment_9 ? HIGH : LOW);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_3 || (uint32_t)(hw_now - hw_sampled_io_3) >= 10UL) {
    hw_attempted_io_3 = true; hw_sampled_io_3 = hw_now;
    hw_value_io_3 = (analogRead(A3) * 1.0 + 0.0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_3, &hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_4_segment_0, &hw_value_io_4_segment_1, &hw_value_io_4_segment_2, &hw_value_io_4_segment_3, &hw_value_io_4_segment_4, &hw_value_io_4_segment_5, &hw_value_io_4_segment_6, &hw_value_io_4_segment_7, &hw_value_io_4_segment_8, &hw_value_io_4_segment_9);
  analogWrite(2, (int)constrain((hw_value_io_0 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(3, (int)constrain((hw_value_io_1 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(4, (int)constrain((hw_value_io_2 * 1.0 + 0.0), 0.0, 255.0));
  digitalWrite(5, hw_value_io_4_segment_0 ? HIGH : LOW);
  digitalWrite(6, hw_value_io_4_segment_1 ? HIGH : LOW);
  digitalWrite(7, hw_value_io_4_segment_2 ? HIGH : LOW);
  digitalWrite(8, hw_value_io_4_segment_3 ? HIGH : LOW);
  digitalWrite(9, hw_value_io_4_segment_4 ? HIGH : LOW);
  digitalWrite(10, hw_value_io_4_segment_5 ? HIGH : LOW);
  digitalWrite(11, hw_value_io_4_segment_6 ? HIGH : LOW);
  digitalWrite(12, hw_value_io_4_segment_7 ? HIGH : LOW);
  digitalWrite(13, hw_value_io_4_segment_8 ? HIGH : LOW);
  digitalWrite(44, hw_value_io_4_segment_9 ? HIGH : LOW);
}

