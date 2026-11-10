#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision22\\cases\\embedbench_85\\arduino\\sketch\\sketch.ino"
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
extern int32_t sysml_state_mode1Index;
/* Global Variable define of  */
extern int32_t sysml_state_mode2Index;
/* Global Variable define of  */
extern bool sysml_state_rgbOff;
/* Global Variable define of  */
extern bool sysml_state_prevIo0;
/* Global Variable define of  */
extern bool sysml_state_prevIo1;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double io_2, bool* io_3, bool* io_4, bool* io_5);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_mode1Index = 0;
/* Global Variable define of  */
int32_t sysml_state_mode2Index = 0;
/* Global Variable define of  */
bool sysml_state_rgbOff = false;
/* Global Variable define of  */
bool sysml_state_prevIo0 = false;
/* Global Variable define of  */
bool sysml_state_prevIo1 = false;
/* Init function of CompositeActor named "ProgramPage1" */
#line 96 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision22\\cases\\embedbench_85\\arduino\\sketch\\sketch.ino"
static bool manual_debounce_input(bool raw, uint8_t index);
#line 105 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision22\\cases\\embedbench_85\\arduino\\sketch\\sketch.ino"
void setup();
#line 118 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision22\\cases\\embedbench_85\\arduino\\sketch\\sketch.ino"
void loop();
#line 45 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision22\\cases\\embedbench_85\\arduino\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_mode1Index = 0;
    sysml_state_mode2Index = 0;
    sysml_state_rgbOff = false;
    sysml_state_prevIo0 = false;
    sysml_state_prevIo1 = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double io_2, bool* io_3, bool* io_4, bool* io_5) {
    static int group=1,index=0;
    static bool previousPrimary=false,previousSecondary=false;
    bool primary=io_0 && !previousPrimary,secondary=io_1 && !previousSecondary;
    int selection=primary ? 1 : (secondary ? 2 : 0);
    previousPrimary=io_0;
    previousSecondary=io_1;
    if (primary && secondary) group=0;
    else if (selection) {
        if (selection==group) index=(index+1)%3;
        else if (group==0) { group=selection; index=0; }
        else group=0;
    }
    const uint8_t colors[][3]={{0,0,0},{1,2,4},{3,6,5}};
    uint8_t color=colors[group][index];
    *io_3=color&1;
    *io_4=color&2;
    *io_5=color&4;
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
static bool hw_value_io_1 = false;
static uint32_t hw_sampled_io_1 = 0;
static bool hw_attempted_io_1 = false;
static double hw_value_io_2 = 0;
static uint32_t hw_sampled_io_2 = 0;
static bool hw_attempted_io_2 = false;
static bool hw_value_io_3 = true;
static bool hw_value_io_4 = false;
static bool hw_value_io_5 = false;

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
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(A3, INPUT);
  pinMode(4, OUTPUT);
  analogWrite(4, hw_value_io_3 ? (int)(hw_value_io_2*255.0/1023.0) : 0);
  pinMode(5, OUTPUT);
  analogWrite(5, hw_value_io_4 ? (int)(hw_value_io_2*255.0/1023.0) : 0);
  pinMode(6, OUTPUT);
  analogWrite(6, hw_value_io_5 ? (int)(hw_value_io_2*255.0/1023.0) : 0);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 5UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = manual_debounce_input((digitalRead(2) == LOW), 0);
  }
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 5UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = manual_debounce_input((digitalRead(3) == LOW), 1);
  }
  if (!hw_attempted_io_2 || (uint32_t)(hw_now - hw_sampled_io_2) >= 5UL) {
    hw_attempted_io_2 = true; hw_sampled_io_2 = hw_now;
    hw_value_io_2 = (analogRead(A3) * 1.0 + 0.0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5);
  analogWrite(4, hw_value_io_3 ? (int)(hw_value_io_2*255.0/1023.0) : 0);
  analogWrite(5, hw_value_io_4 ? (int)(hw_value_io_2*255.0/1023.0) : 0);
  analogWrite(6, hw_value_io_5 ? (int)(hw_value_io_2*255.0/1023.0) : 0);
}

