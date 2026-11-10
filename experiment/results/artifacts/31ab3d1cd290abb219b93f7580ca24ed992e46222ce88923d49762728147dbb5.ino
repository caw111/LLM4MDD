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
void ProgramPage1(double io_3, double io_4, double io_5, double* io_0, double* io_1, bool* io_2_segment_0, bool* io_2_segment_1, bool* io_2_segment_2, bool* io_2_segment_3, bool* io_2_segment_4, bool* io_2_segment_5, bool* io_2_segment_6, bool* io_2_segment_7, bool* io_2_segment_8, bool* io_2_segment_9);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_3, double io_4, double io_5, double* io_0, double* io_1, bool* io_2_segment_0, bool* io_2_segment_1, bool* io_2_segment_2, bool* io_2_segment_3, bool* io_2_segment_4, bool* io_2_segment_5, bool* io_2_segment_6, bool* io_2_segment_7, bool* io_2_segment_8, bool* io_2_segment_9) {
    *io_0=io_3*255.0/1023.0;*io_1=io_4*255.0/1023.0;
    int count=(int)(io_5*10.0/1023.0);
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
static double hw_value_io_3 = 0;
static uint32_t hw_sampled_io_3 = 0;
static bool hw_attempted_io_3 = false;
static double hw_value_io_4 = 0;
static uint32_t hw_sampled_io_4 = 0;
static bool hw_attempted_io_4 = false;
static double hw_value_io_5 = 0;
static uint32_t hw_sampled_io_5 = 0;
static bool hw_attempted_io_5 = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, OUTPUT);
  analogWrite(2, (int)constrain((hw_value_io_0 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(3, OUTPUT);
  analogWrite(3, (int)constrain((hw_value_io_1 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(4, OUTPUT);
  digitalWrite(4, hw_value_io_2_segment_0 ? HIGH : LOW);
  pinMode(5, OUTPUT);
  digitalWrite(5, hw_value_io_2_segment_1 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  digitalWrite(6, hw_value_io_2_segment_2 ? HIGH : LOW);
  pinMode(7, OUTPUT);
  digitalWrite(7, hw_value_io_2_segment_3 ? HIGH : LOW);
  pinMode(8, OUTPUT);
  digitalWrite(8, hw_value_io_2_segment_4 ? HIGH : LOW);
  pinMode(9, OUTPUT);
  digitalWrite(9, hw_value_io_2_segment_5 ? HIGH : LOW);
  pinMode(10, OUTPUT);
  digitalWrite(10, hw_value_io_2_segment_6 ? HIGH : LOW);
  pinMode(11, OUTPUT);
  digitalWrite(11, hw_value_io_2_segment_7 ? HIGH : LOW);
  pinMode(12, OUTPUT);
  digitalWrite(12, hw_value_io_2_segment_8 ? HIGH : LOW);
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_2_segment_9 ? HIGH : LOW);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_3 || (uint32_t)(hw_now - hw_sampled_io_3) >= 5UL) {
    hw_attempted_io_3 = true; hw_sampled_io_3 = hw_now;
    hw_value_io_3 = (analogRead(A3) * 1 + 0.0);
  }
  if (!hw_attempted_io_4 || (uint32_t)(hw_now - hw_sampled_io_4) >= 5UL) {
    hw_attempted_io_4 = true; hw_sampled_io_4 = hw_now;
    hw_value_io_4 = (analogRead(A4) * 1 + 0.0);
  }
  if (!hw_attempted_io_5 || (uint32_t)(hw_now - hw_sampled_io_5) >= 5UL) {
    hw_attempted_io_5 = true; hw_sampled_io_5 = hw_now;
    hw_value_io_5 = (analogRead(A5) * 1 + 0.0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_3, hw_value_io_4, hw_value_io_5, &hw_value_io_0, &hw_value_io_1, &hw_value_io_2_segment_0, &hw_value_io_2_segment_1, &hw_value_io_2_segment_2, &hw_value_io_2_segment_3, &hw_value_io_2_segment_4, &hw_value_io_2_segment_5, &hw_value_io_2_segment_6, &hw_value_io_2_segment_7, &hw_value_io_2_segment_8, &hw_value_io_2_segment_9);
  analogWrite(2, (int)constrain((hw_value_io_0 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(3, (int)constrain((hw_value_io_1 * 1.0 + 0.0), 0.0, 255.0));
  digitalWrite(4, hw_value_io_2_segment_0 ? HIGH : LOW);
  digitalWrite(5, hw_value_io_2_segment_1 ? HIGH : LOW);
  digitalWrite(6, hw_value_io_2_segment_2 ? HIGH : LOW);
  digitalWrite(7, hw_value_io_2_segment_3 ? HIGH : LOW);
  digitalWrite(8, hw_value_io_2_segment_4 ? HIGH : LOW);
  digitalWrite(9, hw_value_io_2_segment_5 ? HIGH : LOW);
  digitalWrite(10, hw_value_io_2_segment_6 ? HIGH : LOW);
  digitalWrite(11, hw_value_io_2_segment_7 ? HIGH : LOW);
  digitalWrite(12, hw_value_io_2_segment_8 ? HIGH : LOW);
  digitalWrite(13, hw_value_io_2_segment_9 ? HIGH : LOW);
}
