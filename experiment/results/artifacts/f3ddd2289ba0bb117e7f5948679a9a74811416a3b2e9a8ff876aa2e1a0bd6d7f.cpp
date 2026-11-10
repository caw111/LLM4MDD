#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision10\\cases\\embedbench_69\\arduino\\sketch\\sketch.ino"
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
/* Global Variable define of  */
extern int32_t sysml_state_pwmBrightness;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, double* io_2, double* io_3, double* io_4);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_selectedChannel = 0;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
int32_t sysml_state_pwmBrightness = 0;
/* Init function of CompositeActor named "ProgramPage1" */
#line 72 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision10\\cases\\embedbench_69\\arduino\\sketch\\sketch.ino"
static bool manual_debounce_input(bool raw, uint8_t index);
#line 81 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision10\\cases\\embedbench_69\\arduino\\sketch\\sketch.ino"
void setup();
#line 93 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision10\\cases\\embedbench_69\\arduino\\sketch\\sketch.ino"
void loop();
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision10\\cases\\embedbench_69\\arduino\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_selectedChannel = 0;
    sysml_state_previousPressed = false;
    sysml_state_pwmBrightness = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, double* io_2, double* io_3, double* io_4) {
    if(io_1&&!sysml_state_previousPressed) sysml_state_selectedChannel=sysml_state_selectedChannel%3+1;
    sysml_state_previousPressed=io_1;
    double level=io_0*255.0/1023.0;
    *io_2=sysml_state_selectedChannel==1 ? level : 0;
    *io_3=sysml_state_selectedChannel==2 ? level : 0;
    *io_4=sysml_state_selectedChannel==3 ? level : 0;
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
static bool hw_value_io_1 = false;
static uint32_t hw_sampled_io_1 = 0;
static bool hw_attempted_io_1 = false;
static double hw_value_io_2 = 0;
static double hw_value_io_3 = 0;
static double hw_value_io_4 = 0;

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
  pinMode(A3, INPUT);
  pinMode(2, INPUT_PULLUP);
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
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 5UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (analogRead(A3) * 1.0 + 0.0);
  }
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 5UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = manual_debounce_input((digitalRead(2) == LOW), 0);
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

