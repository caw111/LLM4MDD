#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_100\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
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
void ProgramPage1(double io_12, double io_13, double io_14, double* io_0, double* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7, bool* io_8, bool* io_9, bool* io_10, bool* io_11);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Init function of CompositeActor named "ProgramPage1" */
#line 99 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_100\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void setup();
#line 130 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_100\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void loop();
#line 25 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_100\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_12, double io_13, double io_14, double* io_0, double* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7, bool* io_8, bool* io_9, bool* io_10, bool* io_11) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_value_0 = 0;
    sysml_value_0 = io_12;
    double sysml_value_1 = 0;
    sysml_value_1 = io_13;
    bool sysml_value_2 = 0;
    sysml_value_2 = (io_14 >= 1);
    bool sysml_value_3 = 0;
    sysml_value_3 = (io_14 >= 2);
    bool sysml_value_4 = 0;
    sysml_value_4 = (io_14 >= 3);
    bool sysml_value_5 = 0;
    sysml_value_5 = (io_14 >= 4);
    bool sysml_value_6 = 0;
    sysml_value_6 = (io_14 >= 5);
    bool sysml_value_7 = 0;
    sysml_value_7 = (io_14 >= 6);
    bool sysml_value_8 = 0;
    sysml_value_8 = (io_14 >= 7);
    bool sysml_value_9 = 0;
    sysml_value_9 = (io_14 >= 8);
    bool sysml_value_10 = 0;
    sysml_value_10 = (io_14 >= 9);
    bool sysml_value_11 = 0;
    sysml_value_11 = (io_14 >= 10);
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
static double hw_value_io_12 = 0;
static uint32_t hw_sampled_io_12 = 0;
static bool hw_attempted_io_12 = false;
static double hw_value_io_13 = 0;
static uint32_t hw_sampled_io_13 = 0;
static bool hw_attempted_io_13 = false;
static double hw_value_io_14 = 0;
static uint32_t hw_sampled_io_14 = 0;
static bool hw_attempted_io_14 = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, OUTPUT);
  analogWrite(2, (int)constrain((hw_value_io_0 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(3, OUTPUT);
  analogWrite(3, (int)constrain((hw_value_io_1 * 1.0 + 0.0), 0.0, 255.0));
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
  digitalWrite(12, hw_value_io_10 ? HIGH : LOW);
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_11 ? HIGH : LOW);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_12 || (uint32_t)(hw_now - hw_sampled_io_12) >= 10UL) {
    hw_attempted_io_12 = true; hw_sampled_io_12 = hw_now;
    hw_value_io_12 = (analogRead(A3) * 1.0 + 0.0);
  }
  if (!hw_attempted_io_13 || (uint32_t)(hw_now - hw_sampled_io_13) >= 10UL) {
    hw_attempted_io_13 = true; hw_sampled_io_13 = hw_now;
    hw_value_io_13 = (analogRead(A4) * 1.0 + 0.0);
  }
  if (!hw_attempted_io_14 || (uint32_t)(hw_now - hw_sampled_io_14) >= 10UL) {
    hw_attempted_io_14 = true; hw_sampled_io_14 = hw_now;
    hw_value_io_14 = (analogRead(A5) * 1.0 + 0.0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_12, hw_value_io_13, hw_value_io_14, &hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_io_7, &hw_value_io_8, &hw_value_io_9, &hw_value_io_10, &hw_value_io_11);
  analogWrite(2, (int)constrain((hw_value_io_0 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(3, (int)constrain((hw_value_io_1 * 1.0 + 0.0), 0.0, 255.0));
  digitalWrite(4, hw_value_io_2 ? HIGH : LOW);
  digitalWrite(5, hw_value_io_3 ? HIGH : LOW);
  digitalWrite(6, hw_value_io_4 ? HIGH : LOW);
  digitalWrite(7, hw_value_io_5 ? HIGH : LOW);
  digitalWrite(8, hw_value_io_6 ? HIGH : LOW);
  digitalWrite(9, hw_value_io_7 ? HIGH : LOW);
  digitalWrite(10, hw_value_io_8 ? HIGH : LOW);
  digitalWrite(11, hw_value_io_9 ? HIGH : LOW);
  digitalWrite(12, hw_value_io_10 ? HIGH : LOW);
  digitalWrite(13, hw_value_io_11 ? HIGH : LOW);
}

