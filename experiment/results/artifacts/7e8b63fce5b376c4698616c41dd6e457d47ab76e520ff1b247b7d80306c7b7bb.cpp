#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision10\\cases\\embedbench_99\\arduino\\sketch\\sketch.ino"
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
extern int32_t sysml_state_blinkPhaseMs;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_1, int32_t io_3, double* io_0, bool* io_2_segment_0, bool* io_2_segment_1, bool* io_2_segment_2, bool* io_2_segment_3, bool* io_2_segment_4, bool* io_2_segment_5, bool* io_2_segment_6, bool* io_2_segment_7, bool* io_2_segment_8, bool* io_2_segment_9);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_blinkPhaseMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
#line 80 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision10\\cases\\embedbench_99\\arduino\\sketch\\sketch.ino"
void setup();
#line 107 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision10\\cases\\embedbench_99\\arduino\\sketch\\sketch.ino"
void loop();
#line 29 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision10\\cases\\embedbench_99\\arduino\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_blinkPhaseMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_1, int32_t io_3, double* io_0, bool* io_2_segment_0, bool* io_2_segment_1, bool* io_2_segment_2, bool* io_2_segment_3, bool* io_2_segment_4, bool* io_2_segment_5, bool* io_2_segment_6, bool* io_2_segment_7, bool* io_2_segment_8, bool* io_2_segment_9) {
    static bool previousMax=false;
    static uint32_t began=0;
    bool maximum=io_1>=1023;
    if(maximum&&!previousMax) began=millis();
    *io_0=maximum ? (((millis()-began)/2000UL%2UL==0) ? 255 : 0) : io_1*255.0/1023.0;
    previousMax=maximum;
    int count=(int)(io_1*10.0/1023.0);
    *io_2_segment_0=count>0;
    *io_2_segment_1=count>1;
    *io_2_segment_2=count>2;
    *io_2_segment_3=count>3;
    *io_2_segment_4=count>4;
    *io_2_segment_5=count>5;
    *io_2_segment_6=count>6;
    *io_2_segment_7=count>7;
    *io_2_segment_8=count>8;
    *io_2_segment_9=count>9;

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
static double hw_value_io_1 = 0;
static uint32_t hw_sampled_io_1 = 0;
static bool hw_attempted_io_1 = false;
static bool hw_value_io_2_segment_0 = false;
static bool hw_value_io_2_segment_1 = false;
static bool hw_value_io_2_segment_2 = false;
static bool hw_value_io_2_segment_3 = false;
static bool hw_value_io_2_segment_4 = false;
static bool hw_value_io_2_segment_5 = false;
static bool hw_value_io_2_segment_6 = false;
static bool hw_value_io_2_segment_7 = false;
static bool hw_value_io_2_segment_8 = false;
static bool hw_value_io_2_segment_9 = false;
static int32_t hw_value_io_3 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, OUTPUT);
  analogWrite(2, (int)constrain((hw_value_io_0 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(A3, INPUT);
  pinMode(3, OUTPUT);
  digitalWrite(3, hw_value_io_2_segment_0 ? HIGH : LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, hw_value_io_2_segment_1 ? HIGH : LOW);
  pinMode(5, OUTPUT);
  digitalWrite(5, hw_value_io_2_segment_2 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  digitalWrite(6, hw_value_io_2_segment_3 ? HIGH : LOW);
  pinMode(7, OUTPUT);
  digitalWrite(7, hw_value_io_2_segment_4 ? HIGH : LOW);
  pinMode(8, OUTPUT);
  digitalWrite(8, hw_value_io_2_segment_5 ? HIGH : LOW);
  pinMode(9, OUTPUT);
  digitalWrite(9, hw_value_io_2_segment_6 ? HIGH : LOW);
  pinMode(10, OUTPUT);
  digitalWrite(10, hw_value_io_2_segment_7 ? HIGH : LOW);
  pinMode(11, OUTPUT);
  digitalWrite(11, hw_value_io_2_segment_8 ? HIGH : LOW);
  pinMode(12, OUTPUT);
  digitalWrite(12, hw_value_io_2_segment_9 ? HIGH : LOW);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 5UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = (analogRead(A3) * 1.0 + 0.0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_3 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_1, hw_value_io_3, &hw_value_io_0, &hw_value_io_2_segment_0, &hw_value_io_2_segment_1, &hw_value_io_2_segment_2, &hw_value_io_2_segment_3, &hw_value_io_2_segment_4, &hw_value_io_2_segment_5, &hw_value_io_2_segment_6, &hw_value_io_2_segment_7, &hw_value_io_2_segment_8, &hw_value_io_2_segment_9);
  analogWrite(2, (int)constrain((hw_value_io_0 * 1.0 + 0.0), 0.0, 255.0));
  digitalWrite(3, hw_value_io_2_segment_0 ? HIGH : LOW);
  digitalWrite(4, hw_value_io_2_segment_1 ? HIGH : LOW);
  digitalWrite(5, hw_value_io_2_segment_2 ? HIGH : LOW);
  digitalWrite(6, hw_value_io_2_segment_3 ? HIGH : LOW);
  digitalWrite(7, hw_value_io_2_segment_4 ? HIGH : LOW);
  digitalWrite(8, hw_value_io_2_segment_5 ? HIGH : LOW);
  digitalWrite(9, hw_value_io_2_segment_6 ? HIGH : LOW);
  digitalWrite(10, hw_value_io_2_segment_7 ? HIGH : LOW);
  digitalWrite(11, hw_value_io_2_segment_8 ? HIGH : LOW);
  digitalWrite(12, hw_value_io_2_segment_9 ? HIGH : LOW);
}

