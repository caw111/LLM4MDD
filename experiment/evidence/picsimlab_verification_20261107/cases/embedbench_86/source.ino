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
extern int32_t sysml_state_mode;
/* Global Variable define of  */
extern int32_t sysml_state_pressHoldMs;
/* Global Variable define of  */
extern bool sysml_state_previousPressed;
/* Global Variable define of  */
extern bool sysml_state_debouncedPressed;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, double* io_1, double* io_2, double* io_3, double* io_4, double* io_5, double* io_6, bool* io_7_segment_0, bool* io_7_segment_1, bool* io_7_segment_2, bool* io_7_segment_3, bool* io_7_segment_4, bool* io_7_segment_5, bool* io_7_segment_6);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_mode = 0;
/* Global Variable define of  */
int32_t sysml_state_pressHoldMs = 0;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
bool sysml_state_debouncedPressed = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_mode = 0;
    sysml_state_pressHoldMs = 0;
    sysml_state_previousPressed = false;
    sysml_state_debouncedPressed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, double* io_1, double* io_2, double* io_3, double* io_4, double* io_5, double* io_6, bool* io_7_segment_0, bool* io_7_segment_1, bool* io_7_segment_2, bool* io_7_segment_3, bool* io_7_segment_4, bool* io_7_segment_5, bool* io_7_segment_6) {
    static bool previous=false;
    static uint8_t mode=0;
    if (io_0 && !previous) mode=(mode+1)%8;
    previous=io_0;
    const uint8_t colors[][2]={{0,0},{1,4},{2,6},{3,5},{6,3},{5,6},{7,3},{0,7}};
    uint8_t a=colors[mode][0],b=colors[mode][1];
    *io_1=a&1 ? 255 : 0;
    *io_2=a&2 ? 255 : 0;
    *io_3=a&4 ? 255 : 0;
    *io_4=b&1 ? 255 : 0;
    *io_5=b&2 ? 255 : 0;
    *io_6=b&4 ? 255 : 0;
    *io_7_segment_0=mode>0;
    *io_7_segment_1=mode>1;
    *io_7_segment_2=mode>2;
    *io_7_segment_3=mode>3;
    *io_7_segment_4=mode>4;
    *io_7_segment_5=mode>5;
    *io_7_segment_6=mode>6;

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
static double hw_value_io_2 = 0;
static double hw_value_io_3 = 0;
static double hw_value_io_4 = 0;
static double hw_value_io_5 = 0;
static double hw_value_io_6 = 0;
static bool hw_value_io_7_segment_0 = false;
static bool hw_value_io_7_segment_1 = false;
static bool hw_value_io_7_segment_2 = false;
static bool hw_value_io_7_segment_3 = false;
static bool hw_value_io_7_segment_4 = false;
static bool hw_value_io_7_segment_5 = false;
static bool hw_value_io_7_segment_6 = false;

static bool manual_debounce_input(bool raw, uint8_t index) {
  static bool previous[8]={}, stable[8]={};
  static uint32_t changed[8]={};
  uint32_t now=millis();
  if(raw!=previous[index]) {previous[index]=raw;changed[index]=now;}
  if((uint32_t)(now-changed[index])>=150UL) stable[index]=raw;
  if(!raw) stable[index]=false;
  return stable[index];
}

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, OUTPUT);
  analogWrite(3, (int)constrain((hw_value_io_1 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(4, OUTPUT);
  analogWrite(4, (int)constrain((hw_value_io_2 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(5, OUTPUT);
  analogWrite(5, (int)constrain((hw_value_io_3 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(6, OUTPUT);
  analogWrite(6, (int)constrain((hw_value_io_4 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(7, OUTPUT);
  analogWrite(7, (int)constrain((hw_value_io_5 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(8, OUTPUT);
  analogWrite(8, (int)constrain((hw_value_io_6 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(9, OUTPUT);
  digitalWrite(9, hw_value_io_7_segment_0 ? HIGH : LOW);
  pinMode(10, OUTPUT);
  digitalWrite(10, hw_value_io_7_segment_1 ? HIGH : LOW);
  pinMode(11, OUTPUT);
  digitalWrite(11, hw_value_io_7_segment_2 ? HIGH : LOW);
  pinMode(12, OUTPUT);
  digitalWrite(12, hw_value_io_7_segment_3 ? HIGH : LOW);
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_7_segment_4 ? HIGH : LOW);
  pinMode(44, OUTPUT);
  digitalWrite(44, hw_value_io_7_segment_5 ? HIGH : LOW);
  pinMode(45, OUTPUT);
  digitalWrite(45, hw_value_io_7_segment_6 ? HIGH : LOW);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 10UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = manual_debounce_input((digitalRead(2) == LOW), 0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_io_7_segment_0, &hw_value_io_7_segment_1, &hw_value_io_7_segment_2, &hw_value_io_7_segment_3, &hw_value_io_7_segment_4, &hw_value_io_7_segment_5, &hw_value_io_7_segment_6);
  analogWrite(3, (int)constrain((hw_value_io_1 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(4, (int)constrain((hw_value_io_2 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(5, (int)constrain((hw_value_io_3 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(6, (int)constrain((hw_value_io_4 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(7, (int)constrain((hw_value_io_5 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(8, (int)constrain((hw_value_io_6 * 1.0 + 0.0), 0.0, 255.0));
  digitalWrite(9, hw_value_io_7_segment_0 ? HIGH : LOW);
  digitalWrite(10, hw_value_io_7_segment_1 ? HIGH : LOW);
  digitalWrite(11, hw_value_io_7_segment_2 ? HIGH : LOW);
  digitalWrite(12, hw_value_io_7_segment_3 ? HIGH : LOW);
  digitalWrite(13, hw_value_io_7_segment_4 ? HIGH : LOW);
  digitalWrite(44, hw_value_io_7_segment_5 ? HIGH : LOW);
  digitalWrite(45, hw_value_io_7_segment_6 ? HIGH : LOW);
}
