#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_bar_led_basic\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
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
void ProgramPage1(bool* io_0_segment_0, bool* io_0_segment_1, bool* io_0_segment_2, bool* io_0_segment_3, bool* io_0_segment_4, bool* io_0_segment_5, bool* io_0_segment_6, bool* io_0_segment_7, bool* io_0_segment_8, bool* io_0_segment_9);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Init function of CompositeActor named "ProgramPage1" */
#line 82 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_bar_led_basic\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 106 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_bar_led_basic\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 25 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_bar_led_basic\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool* io_0_segment_0, bool* io_0_segment_1, bool* io_0_segment_2, bool* io_0_segment_3, bool* io_0_segment_4, bool* io_0_segment_5, bool* io_0_segment_6, bool* io_0_segment_7, bool* io_0_segment_8, bool* io_0_segment_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_value_0 = 0;
    sysml_value_0 = true;
    bool sysml_value_1 = 0;
    sysml_value_1 = true;
    bool sysml_value_2 = 0;
    sysml_value_2 = true;
    bool sysml_value_3 = 0;
    sysml_value_3 = true;
    bool sysml_value_4 = 0;
    sysml_value_4 = true;
    bool sysml_value_5 = 0;
    sysml_value_5 = true;
    bool sysml_value_6 = 0;
    sysml_value_6 = false;
    bool sysml_value_7 = 0;
    sysml_value_7 = false;
    bool sysml_value_8 = 0;
    sysml_value_8 = false;
    bool sysml_value_9 = 0;
    sysml_value_9 = false;
    *io_0_segment_0 = sysml_value_0;
    *io_0_segment_1 = sysml_value_1;
    *io_0_segment_2 = sysml_value_2;
    *io_0_segment_3 = sysml_value_3;
    *io_0_segment_4 = sysml_value_4;
    *io_0_segment_5 = sysml_value_5;
    *io_0_segment_6 = sysml_value_6;
    *io_0_segment_7 = sysml_value_7;
    *io_0_segment_8 = sysml_value_8;
    *io_0_segment_9 = sysml_value_9;
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
static bool hw_value_io_0_segment_0 = true;
static bool hw_value_io_0_segment_1 = true;
static bool hw_value_io_0_segment_2 = true;
static bool hw_value_io_0_segment_3 = true;
static bool hw_value_io_0_segment_4 = true;
static bool hw_value_io_0_segment_5 = true;
static bool hw_value_io_0_segment_6 = true;
static bool hw_value_io_0_segment_7 = true;
static bool hw_value_io_0_segment_8 = true;
static bool hw_value_io_0_segment_9 = true;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, OUTPUT);
  digitalWrite(2, hw_value_io_0_segment_0 ? HIGH : LOW);
  pinMode(3, OUTPUT);
  digitalWrite(3, hw_value_io_0_segment_1 ? HIGH : LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, hw_value_io_0_segment_2 ? HIGH : LOW);
  pinMode(5, OUTPUT);
  digitalWrite(5, hw_value_io_0_segment_3 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  digitalWrite(6, hw_value_io_0_segment_4 ? HIGH : LOW);
  pinMode(7, OUTPUT);
  digitalWrite(7, hw_value_io_0_segment_5 ? HIGH : LOW);
  pinMode(8, OUTPUT);
  digitalWrite(8, hw_value_io_0_segment_6 ? HIGH : LOW);
  pinMode(9, OUTPUT);
  digitalWrite(9, hw_value_io_0_segment_7 ? HIGH : LOW);
  pinMode(10, OUTPUT);
  digitalWrite(10, hw_value_io_0_segment_8 ? HIGH : LOW);
  pinMode(11, OUTPUT);
  digitalWrite(11, hw_value_io_0_segment_9 ? HIGH : LOW);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(&hw_value_io_0_segment_0, &hw_value_io_0_segment_1, &hw_value_io_0_segment_2, &hw_value_io_0_segment_3, &hw_value_io_0_segment_4, &hw_value_io_0_segment_5, &hw_value_io_0_segment_6, &hw_value_io_0_segment_7, &hw_value_io_0_segment_8, &hw_value_io_0_segment_9);
  digitalWrite(2, hw_value_io_0_segment_0 ? HIGH : LOW);
  digitalWrite(3, hw_value_io_0_segment_1 ? HIGH : LOW);
  digitalWrite(4, hw_value_io_0_segment_2 ? HIGH : LOW);
  digitalWrite(5, hw_value_io_0_segment_3 ? HIGH : LOW);
  digitalWrite(6, hw_value_io_0_segment_4 ? HIGH : LOW);
  digitalWrite(7, hw_value_io_0_segment_5 ? HIGH : LOW);
  digitalWrite(8, hw_value_io_0_segment_6 ? HIGH : LOW);
  digitalWrite(9, hw_value_io_0_segment_7 ? HIGH : LOW);
  digitalWrite(10, hw_value_io_0_segment_8 ? HIGH : LOW);
  digitalWrite(11, hw_value_io_0_segment_9 ? HIGH : LOW);
}

