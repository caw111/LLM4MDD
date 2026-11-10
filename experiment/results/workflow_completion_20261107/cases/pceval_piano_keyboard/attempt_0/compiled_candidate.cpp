#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_completion_20261107\\run7_clarified\\cases\\pceval_piano_keyboard\\attempt_0\\arduino\\sketch\\sketch.ino"
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
void ProgramPage1(bool io_0, bool io_1, bool io_2, bool io_3, int32_t* io_4, int32_t* io_5, int32_t* io_6, int32_t* io_7);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Init function of CompositeActor named "ProgramPage1" */
#line 99 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_completion_20261107\\run7_clarified\\cases\\pceval_piano_keyboard\\attempt_0\\arduino\\sketch\\sketch.ino"
void setup();
#line 111 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_completion_20261107\\run7_clarified\\cases\\pceval_piano_keyboard\\attempt_0\\arduino\\sketch\\sketch.ino"
void loop();
#line 25 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_completion_20261107\\run7_clarified\\cases\\pceval_piano_keyboard\\attempt_0\\arduino\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, bool io_3, int32_t* io_4, int32_t* io_5, int32_t* io_6, int32_t* io_7) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int32_t sysml_value_0 = 0;
    int32_t sysml_conditional_0 = 0;
    int32_t sysml_conditional_1 = 0;
    int32_t sysml_conditional_2 = 0;
    int32_t sysml_conditional_3 = 0;
    if (io_0) {
        sysml_conditional_0 = 262;
    }
    else {
        sysml_conditional_0 = 0;
    }
    sysml_value_0 = sysml_conditional_0;
    int32_t sysml_value_1 = 0;
    if (io_1) {
        sysml_conditional_1 = 294;
    }
    else {
        sysml_conditional_1 = 0;
    }
    sysml_value_1 = sysml_conditional_1;
    int32_t sysml_value_2 = 0;
    if (io_2) {
        sysml_conditional_2 = 330;
    }
    else {
        sysml_conditional_2 = 0;
    }
    sysml_value_2 = sysml_conditional_2;
    int32_t sysml_value_3 = 0;
    if (io_3) {
        sysml_conditional_3 = 349;
    }
    else {
        sysml_conditional_3 = 0;
    }
    sysml_value_3 = sysml_conditional_3;
    *io_4 = sysml_value_0;
    *io_5 = sysml_value_1;
    *io_6 = sysml_value_2;
    *io_7 = sysml_value_3;
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
static uint32_t hw_sampled_io_1 = 0;
static bool hw_attempted_io_1 = false;
static bool hw_value_io_2 = false;
static uint32_t hw_sampled_io_2 = 0;
static bool hw_attempted_io_2 = false;
static bool hw_value_io_3 = false;
static uint32_t hw_sampled_io_3 = 0;
static bool hw_attempted_io_3 = false;
static int32_t hw_value_io_4 = 0;
static int32_t hw_value_io_5 = 0;
static int32_t hw_value_io_6 = 0;
static int32_t hw_value_io_7 = 0;
static int hw_active_tone_pin=-1; static int32_t hw_active_tone_frequency=0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 5UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (digitalRead(2) == HIGH);
  }
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 5UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = (digitalRead(3) == HIGH);
  }
  if (!hw_attempted_io_2 || (uint32_t)(hw_now - hw_sampled_io_2) >= 5UL) {
    hw_attempted_io_2 = true; hw_sampled_io_2 = hw_now;
    hw_value_io_2 = (digitalRead(4) == HIGH);
  }
  if (!hw_attempted_io_3 || (uint32_t)(hw_now - hw_sampled_io_3) >= 5UL) {
    hw_attempted_io_3 = true; hw_sampled_io_3 = hw_now;
    hw_value_io_3 = (digitalRead(5) == HIGH);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_io_7);
  { int selected=-1; int32_t frequency=0;
  if(selected<0 && hw_value_io_4>=31 && hw_value_io_4<=65535) {selected=9;frequency=hw_value_io_4;}
  if(selected<0 && hw_value_io_5>=31 && hw_value_io_5<=65535) {selected=10;frequency=hw_value_io_5;}
  if(selected<0 && hw_value_io_6>=31 && hw_value_io_6<=65535) {selected=11;frequency=hw_value_io_6;}
  if(selected<0 && hw_value_io_7>=31 && hw_value_io_7<=65535) {selected=12;frequency=hw_value_io_7;}
  if(selected!=hw_active_tone_pin || frequency!=hw_active_tone_frequency) {
  if(hw_active_tone_pin>=0) noTone(hw_active_tone_pin);
  if(selected>=0) tone(selected,(unsigned int)frequency);
  hw_active_tone_pin=selected;hw_active_tone_frequency=frequency;
  }
  }
}

