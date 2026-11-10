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
void ProgramPage1(double io_0, int32_t* io_1);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, int32_t* io_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int32_t sysml_value_0 = 0;
    int32_t sysml_conditional_0 = 0;
    int32_t sysml_conditional_1 = 0;
    int32_t sysml_conditional_2 = 0;
    if (io_0 >= 1023.0) {
        sysml_conditional_0 = 2000;
    }
    else {
        if (io_0 <= 0.0) {
            sysml_conditional_1 = 0;
        }
        else {
            if (((((double)io_0) / 1023.0) * 2000.0) < ((double)((int32_t)((((double)io_0) / 1023.0) * 2000.0)))) {
                sysml_conditional_2 = 1;
            }
            else {
                sysml_conditional_2 = 0;
            }
            sysml_conditional_1 = ((int32_t)((((double)io_0) / 1023.0) * 2000.0)) - sysml_conditional_2;
        }
        sysml_conditional_0 = sysml_conditional_1;
    }
    sysml_value_0 = sysml_conditional_0;
    *io_1 = sysml_value_0;
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
static double hw_value_io_0 = 0;
static uint32_t hw_sampled_io_0 = 0;
static bool hw_attempted_io_0 = false;
static int32_t hw_value_io_1 = 0;
static int32_t hw_tone_io_1 = -1;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(A0, INPUT);
  pinMode(6, OUTPUT);
  int32_t hw_tone_command_io_1 = (hw_value_io_1 >= 31 && hw_value_io_1 <= 65535) ? hw_value_io_1 : 0;
  if (hw_tone_command_io_1 != hw_tone_io_1) {
    if (hw_tone_command_io_1 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_1);
    hw_tone_io_1 = hw_tone_command_io_1;
  }
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 5UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (analogRead(A0) * 1.0 + 0.0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, &hw_value_io_1);
  int32_t hw_tone_command_io_1 = (hw_value_io_1 >= 31 && hw_value_io_1 <= 65535) ? hw_value_io_1 : 0;
  if (hw_tone_command_io_1 != hw_tone_io_1) {
    if (hw_tone_command_io_1 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_1);
    hw_tone_io_1 = hw_tone_command_io_1;
  }
}
