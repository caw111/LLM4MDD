#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision23\\cases\\embedbench_50\\arduino\\sketch\\sketch.ino"
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
extern int32_t sysml_state_segIndex;
/* Global Variable define of  */
extern int32_t sysml_state_elapsedMs;
/* Global Variable define of  */
extern bool sysml_state_paused;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool* io_0, bool* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7, bool* io_8, bool* io_9, bool* io_10, bool* io_11, bool* io_12, bool* io_13);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_segIndex = 1;
/* Global Variable define of  */
int32_t sysml_state_elapsedMs = 0;
/* Global Variable define of  */
bool sysml_state_paused = false;
/* Init function of CompositeActor named "ProgramPage1" */
#line 88 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision23\\cases\\embedbench_50\\arduino\\sketch\\sketch.ino"
void setup();
#line 120 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision23\\cases\\embedbench_50\\arduino\\sketch\\sketch.ino"
void loop();
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision23\\cases\\embedbench_50\\arduino\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_segIndex = 1;
    sysml_state_elapsedMs = 0;
    sysml_state_paused = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool* io_0, bool* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7, bool* io_8, bool* io_9, bool* io_10, bool* io_11, bool* io_12, bool* io_13) {
    uint32_t tick=millis()/2000UL;
    int index=tick ? (int)((tick-1)%8) : -1;
    bool first=index>=0 && index<4;
    bool second=index>=4;
    *io_0=index==0;
    *io_1=index==1;
    *io_2=index==2;
    *io_3=index==3;
    *io_4=index==4;
    *io_5=index==5;
    *io_6=index==6;
    *io_7=index==7;
    *io_8=first;
    *io_9=second;
    *io_10=false;
    *io_11=*io_12=second;
    *io_13=first;
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
static bool hw_value_io_1 = false;
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
static bool hw_value_io_12 = false;
static bool hw_value_io_13 = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(8, OUTPUT);
  digitalWrite(8, hw_value_io_0 ? HIGH : LOW);
  pinMode(9, OUTPUT);
  digitalWrite(9, hw_value_io_1 ? HIGH : LOW);
  pinMode(10, OUTPUT);
  digitalWrite(10, hw_value_io_2 ? HIGH : LOW);
  pinMode(11, OUTPUT);
  digitalWrite(11, hw_value_io_3 ? HIGH : LOW);
  pinMode(12, OUTPUT);
  digitalWrite(12, hw_value_io_4 ? HIGH : LOW);
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_5 ? HIGH : LOW);
  pinMode(44, OUTPUT);
  digitalWrite(44, hw_value_io_6 ? HIGH : LOW);
  pinMode(45, OUTPUT);
  digitalWrite(45, hw_value_io_7 ? HIGH : LOW);
  pinMode(2, OUTPUT);
  digitalWrite(2, hw_value_io_8 ? HIGH : LOW);
  pinMode(3, OUTPUT);
  digitalWrite(3, hw_value_io_9 ? HIGH : LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, hw_value_io_10 ? HIGH : LOW);
  pinMode(5, OUTPUT);
  digitalWrite(5, hw_value_io_11 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  digitalWrite(6, hw_value_io_12 ? HIGH : LOW);
  pinMode(7, OUTPUT);
  digitalWrite(7, hw_value_io_13 ? HIGH : LOW);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(&hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_io_7, &hw_value_io_8, &hw_value_io_9, &hw_value_io_10, &hw_value_io_11, &hw_value_io_12, &hw_value_io_13);
  digitalWrite(8, hw_value_io_0 ? HIGH : LOW);
  digitalWrite(9, hw_value_io_1 ? HIGH : LOW);
  digitalWrite(10, hw_value_io_2 ? HIGH : LOW);
  digitalWrite(11, hw_value_io_3 ? HIGH : LOW);
  digitalWrite(12, hw_value_io_4 ? HIGH : LOW);
  digitalWrite(13, hw_value_io_5 ? HIGH : LOW);
  digitalWrite(44, hw_value_io_6 ? HIGH : LOW);
  digitalWrite(45, hw_value_io_7 ? HIGH : LOW);
  digitalWrite(2, hw_value_io_8 ? HIGH : LOW);
  digitalWrite(3, hw_value_io_9 ? HIGH : LOW);
  digitalWrite(4, hw_value_io_10 ? HIGH : LOW);
  digitalWrite(5, hw_value_io_11 ? HIGH : LOW);
  digitalWrite(6, hw_value_io_12 ? HIGH : LOW);
  digitalWrite(7, hw_value_io_13 ? HIGH : LOW);
}

