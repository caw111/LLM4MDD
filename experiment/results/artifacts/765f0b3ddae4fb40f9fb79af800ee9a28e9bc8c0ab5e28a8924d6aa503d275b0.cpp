#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_47\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
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
extern double sysml_state_rgb1R;
/* Global Variable define of  */
extern double sysml_state_rgb1G;
/* Global Variable define of  */
extern double sysml_state_rgb1B;
/* Global Variable define of  */
extern double sysml_state_rgb2R;
/* Global Variable define of  */
extern double sysml_state_rgb2G;
/* Global Variable define of  */
extern double sysml_state_rgb2B;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, double* io_2, double* io_3, double* io_4, double* io_5, double* io_6, double* io_7);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_rgb1R = 0.0;
/* Global Variable define of  */
double sysml_state_rgb1G = 0.0;
/* Global Variable define of  */
double sysml_state_rgb1B = 0.0;
/* Global Variable define of  */
double sysml_state_rgb2R = 0.0;
/* Global Variable define of  */
double sysml_state_rgb2G = 0.0;
/* Global Variable define of  */
double sysml_state_rgb2B = 0.0;
/* Init function of CompositeActor named "ProgramPage1" */
#line 109 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_47\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void setup();
#line 127 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_47\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void loop();
#line 49 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_47\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_rgb1R = 0.0;
    sysml_state_rgb1G = 0.0;
    sysml_state_rgb1B = 0.0;
    sysml_state_rgb2R = 0.0;
    sysml_state_rgb2G = 0.0;
    sysml_state_rgb2B = 0.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, double* io_2, double* io_3, double* io_4, double* io_5, double* io_6, double* io_7) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    sysml_state_rgb1R = 0.0;
    sysml_state_rgb1G = 0.0;
    sysml_state_rgb1B = 0.0;
    sysml_state_rgb2R = 0.0;
    sysml_state_rgb2G = 0.0;
    sysml_state_rgb2B = 0.0;
    double sysml_value_0 = 0;
    sysml_value_0 = sysml_state_rgb1R;
    double sysml_value_1 = 0;
    sysml_value_1 = sysml_state_rgb1G;
    double sysml_value_2 = 0;
    sysml_value_2 = sysml_state_rgb1B;
    double sysml_value_3 = 0;
    sysml_value_3 = sysml_state_rgb2R;
    double sysml_value_4 = 0;
    sysml_value_4 = sysml_state_rgb2G;
    double sysml_value_5 = 0;
    sysml_value_5 = sysml_state_rgb2B;
    *io_2 = sysml_value_0;
    *io_3 = sysml_value_1;
    *io_4 = sysml_value_2;
    *io_5 = sysml_value_3;
    *io_6 = sysml_value_4;
    *io_7 = sysml_value_5;
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
static uint32_t hw_sampled_io_1 = 0;
static bool hw_attempted_io_1 = false;
static double hw_value_io_2 = 0;
static double hw_value_io_3 = 0;
static double hw_value_io_4 = 0;
static double hw_value_io_5 = 0;
static double hw_value_io_6 = 0;
static double hw_value_io_7 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(2, OUTPUT);
  analogWrite(2, (int)constrain((hw_value_io_2 * 1 + 0), 0.0, 255.0));
  pinMode(3, OUTPUT);
  analogWrite(3, (int)constrain((hw_value_io_3 * 1 + 0), 0.0, 255.0));
  pinMode(4, OUTPUT);
  analogWrite(4, (int)constrain((hw_value_io_4 * 1 + 0), 0.0, 255.0));
  pinMode(5, OUTPUT);
  analogWrite(5, (int)constrain((hw_value_io_5 * 1 + 0), 0.0, 255.0));
  pinMode(6, OUTPUT);
  analogWrite(6, (int)constrain((hw_value_io_6 * 1 + 0), 0.0, 255.0));
  pinMode(7, OUTPUT);
  analogWrite(7, (int)constrain((hw_value_io_7 * 1 + 0), 0.0, 255.0));
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 5UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (analogRead(A3) * 1 + 0);
  }
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 5UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = (analogRead(A4) * 1 + 0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_io_7);
  analogWrite(2, (int)constrain((hw_value_io_2 * 1 + 0), 0.0, 255.0));
  analogWrite(3, (int)constrain((hw_value_io_3 * 1 + 0), 0.0, 255.0));
  analogWrite(4, (int)constrain((hw_value_io_4 * 1 + 0), 0.0, 255.0));
  analogWrite(5, (int)constrain((hw_value_io_5 * 1 + 0), 0.0, 255.0));
  analogWrite(6, (int)constrain((hw_value_io_6 * 1 + 0), 0.0, 255.0));
  analogWrite(7, (int)constrain((hw_value_io_7 * 1 + 0), 0.0, 255.0));
}

