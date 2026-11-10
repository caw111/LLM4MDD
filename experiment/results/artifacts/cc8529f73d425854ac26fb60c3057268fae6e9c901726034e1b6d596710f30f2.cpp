#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_traffic_light\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
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
extern bool sysml_state_previousPressed;
/* Global Variable define of  */
extern int32_t sysml_state_mode;
/* Global Variable define of  */
extern int32_t sysml_state_yellowRemainingMs;
/* Global Variable define of  */
extern int32_t sysml_state_redRemainingMs;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool* io_1, bool* io_2, bool* io_3);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
int32_t sysml_state_mode = 0;
/* Global Variable define of  */
int32_t sysml_state_yellowRemainingMs = 0;
/* Global Variable define of  */
int32_t sysml_state_redRemainingMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
#line 101 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_traffic_light\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void setup();
#line 112 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_traffic_light\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void loop();
#line 41 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_traffic_light\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousPressed = false;
    sysml_state_mode = 0;
    sysml_state_yellowRemainingMs = 0;
    sysml_state_redRemainingMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool* io_1, bool* io_2, bool* io_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        sysml_state_mode = 1;
        sysml_state_yellowRemainingMs = 1000;
        sysml_state_redRemainingMs = 2000;
    }
    if ((sysml_state_mode == 1)) {
        sysml_state_yellowRemainingMs = (sysml_state_yellowRemainingMs - 10);
        if (((sysml_state_yellowRemainingMs - 10) <= 0)) {
            sysml_state_mode = 2;
        }
    }
    if ((sysml_state_mode == 2)) {
        sysml_state_redRemainingMs = (sysml_state_redRemainingMs - 10);
        if (((sysml_state_redRemainingMs - 10) <= 0)) {
            sysml_state_mode = 0;
        }
    }
    sysml_state_previousPressed = io_0;
    bool sysml_value_0 = 0;
    sysml_value_0 = (sysml_state_mode == 2);
    bool sysml_value_1 = 0;
    sysml_value_1 = (sysml_state_mode == 1);
    bool sysml_value_2 = 0;
    sysml_value_2 = (sysml_state_mode == 0);
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
    *io_3 = sysml_value_2;
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
static bool hw_value_io_0 = false;
static uint32_t hw_sampled_io_0 = 0;
static bool hw_attempted_io_0 = false;
static bool hw_value_io_1 = false;
static bool hw_value_io_2 = false;
static bool hw_value_io_3 = true;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, INPUT);
  pinMode(8, OUTPUT);
  digitalWrite(8, hw_value_io_1 ? HIGH : LOW);
  pinMode(9, OUTPUT);
  digitalWrite(9, hw_value_io_2 ? HIGH : LOW);
  pinMode(10, OUTPUT);
  digitalWrite(10, hw_value_io_3 ? HIGH : LOW);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 10UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (digitalRead(2) == HIGH);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3);
  digitalWrite(8, hw_value_io_1 ? HIGH : LOW);
  digitalWrite(9, hw_value_io_2 ? HIGH : LOW);
  digitalWrite(10, hw_value_io_3 ? HIGH : LOW);
}

