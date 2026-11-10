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
extern int32_t sysml_state_phaseIndex;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_1, bool* io_0_segment_0, bool* io_0_segment_1, bool* io_0_segment_2, bool* io_0_segment_3, bool* io_0_segment_4, bool* io_0_segment_5, bool* io_0_segment_6, bool* io_0_segment_7, bool* io_0_segment_8, bool* io_0_segment_9);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
int32_t sysml_state_phaseIndex = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousPressed = false;
    sysml_state_phaseIndex = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_1, bool* io_0_segment_0, bool* io_0_segment_1, bool* io_0_segment_2, bool* io_0_segment_3, bool* io_0_segment_4, bool* io_0_segment_5, bool* io_0_segment_6, bool* io_0_segment_7, bool* io_0_segment_8, bool* io_0_segment_9) {
    if(io_1&&!sysml_state_previousPressed) sysml_state_phaseIndex=(sysml_state_phaseIndex+1)%6;
    sysml_state_previousPressed=io_1;
    int count=sysml_state_phaseIndex*2;
    *io_0_segment_0=count>9;
    *io_0_segment_1=count>8;
    *io_0_segment_2=count>7;
    *io_0_segment_3=count>6;
    *io_0_segment_4=count>5;
    *io_0_segment_5=count>4;
    *io_0_segment_6=count>3;
    *io_0_segment_7=count>2;
    *io_0_segment_8=count>1;
    *io_0_segment_9=count>0;

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
static bool hw_value_io_0_segment_0 = false;
static bool hw_value_io_0_segment_1 = false;
static bool hw_value_io_0_segment_2 = false;
static bool hw_value_io_0_segment_3 = false;
static bool hw_value_io_0_segment_4 = false;
static bool hw_value_io_0_segment_5 = false;
static bool hw_value_io_0_segment_6 = false;
static bool hw_value_io_0_segment_7 = false;
static bool hw_value_io_0_segment_8 = false;
static bool hw_value_io_0_segment_9 = false;
static bool hw_value_io_1 = false;
static uint32_t hw_sampled_io_1 = 0;
static bool hw_attempted_io_1 = false;

static bool manual_debounce_input(bool raw, uint8_t index) {
  static bool previous[8]={}, stable[8]={};
  static uint32_t changed[8]={};
  uint32_t now=millis();
  if(raw!=previous[index]) {previous[index]=raw;changed[index]=now;}
  if((uint32_t)(now-changed[index])>=25UL) stable[index]=raw;
  return stable[index];
}

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
  pinMode(12, INPUT_PULLUP);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 5UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = manual_debounce_input((digitalRead(12) == LOW), 0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_1, &hw_value_io_0_segment_0, &hw_value_io_0_segment_1, &hw_value_io_0_segment_2, &hw_value_io_0_segment_3, &hw_value_io_0_segment_4, &hw_value_io_0_segment_5, &hw_value_io_0_segment_6, &hw_value_io_0_segment_7, &hw_value_io_0_segment_8, &hw_value_io_0_segment_9);
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
