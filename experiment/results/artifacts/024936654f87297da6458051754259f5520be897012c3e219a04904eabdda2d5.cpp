#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_69\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
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
extern int32_t sysml_state_selectedChannel;
/* Global Variable define of  */
extern bool sysml_state_previousPressed;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, double io_1, double* io_2, double* io_3, double* io_4);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_selectedChannel = 0;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Init function of CompositeActor named "ProgramPage1" */
#line 156 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_69\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void setup();
#line 168 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_69\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void loop();
#line 33 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_69\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_selectedChannel = 0;
    sysml_state_previousPressed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, double io_1, double* io_2, double* io_3, double* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    double sysml_conditional_1 = 0;
    double sysml_conditional_2 = 0;
    double sysml_conditional_3 = 0;
    double sysml_conditional_4 = 0;
    double sysml_conditional_5 = 0;
    double sysml_conditional_6 = 0;
    double sysml_conditional_7 = 0;
    double sysml_conditional_8 = 0;
    double sysml_conditional_9 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        if ((sysml_state_selectedChannel == 0)) {
            sysml_state_selectedChannel = 1;
        }
        else {
            if ((sysml_state_selectedChannel == 1)) {
                sysml_state_selectedChannel = 2;
            }
            else {
                sysml_state_selectedChannel = 0;
            }
        }
        sysml_state_previousPressed = io_0;
    }
    else {
        sysml_state_previousPressed = io_0;
    }
    double sysml_value_0 = 0;
    if (sysml_state_selectedChannel == 0) {
        if (io_1 >= 255.0) {
            sysml_conditional_2 = 255.0;
        }
        else {
            if (io_1 <= 0.0) {
                sysml_conditional_3 = 0.0;
            }
            else {
                sysml_conditional_3 = io_1;
            }
            sysml_conditional_2 = sysml_conditional_3;
        }
        sysml_conditional_1 = sysml_conditional_2;
    }
    else {
        sysml_conditional_1 = 0.0;
    }
    sysml_value_0 = sysml_conditional_1;
    double sysml_value_1 = 0;
    if (sysml_state_selectedChannel == 1) {
        if (io_1 >= 255.0) {
            sysml_conditional_5 = 255.0;
        }
        else {
            if (io_1 <= 0.0) {
                sysml_conditional_6 = 0.0;
            }
            else {
                sysml_conditional_6 = io_1;
            }
            sysml_conditional_5 = sysml_conditional_6;
        }
        sysml_conditional_4 = sysml_conditional_5;
    }
    else {
        sysml_conditional_4 = 0.0;
    }
    sysml_value_1 = sysml_conditional_4;
    double sysml_value_2 = 0;
    if (sysml_state_selectedChannel == 2) {
        if (io_1 >= 255.0) {
            sysml_conditional_8 = 255.0;
        }
        else {
            if (io_1 <= 0.0) {
                sysml_conditional_9 = 0.0;
            }
            else {
                sysml_conditional_9 = io_1;
            }
            sysml_conditional_8 = sysml_conditional_9;
        }
        sysml_conditional_7 = sysml_conditional_8;
    }
    else {
        sysml_conditional_7 = 0.0;
    }
    sysml_value_2 = sysml_conditional_7;
    *io_2 = sysml_value_0;
    *io_3 = sysml_value_1;
    *io_4 = sysml_value_2;
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
static uint32_t hw_sampled_io_0 = 0;
static bool hw_attempted_io_0 = false;
static double hw_value_io_1 = 0;
static uint32_t hw_sampled_io_1 = 0;
static bool hw_attempted_io_1 = false;
static double hw_value_io_2 = 0;
static double hw_value_io_3 = 0;
static double hw_value_io_4 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, INPUT_PULLUP);
  pinMode(A3, INPUT);
  pinMode(3, OUTPUT);
  analogWrite(3, (int)constrain((hw_value_io_2 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(4, OUTPUT);
  analogWrite(4, (int)constrain((hw_value_io_3 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(5, OUTPUT);
  analogWrite(5, (int)constrain((hw_value_io_4 * 1.0 + 0.0), 0.0, 255.0));
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 10UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (digitalRead(2) == LOW);
  }
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 10UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = (analogRead(A3) * 1.0 + 0.0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4);
  analogWrite(3, (int)constrain((hw_value_io_2 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(4, (int)constrain((hw_value_io_3 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(5, (int)constrain((hw_value_io_4 * 1.0 + 0.0), 0.0, 255.0));
}

