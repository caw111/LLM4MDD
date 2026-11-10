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
extern int32_t sysml_state_digit;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, double io_2, int32_t* io_3, bool* io_4_segment_0, bool* io_4_segment_1, bool* io_4_segment_2, bool* io_4_segment_3, bool* io_4_segment_4, bool* io_4_segment_5, bool* io_4_segment_6, bool* io_4_segment_7, bool* io_4_segment_8, bool* io_4_segment_9);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_digit = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_digit = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, double io_2, int32_t* io_3, bool* io_4_segment_0, bool* io_4_segment_1, bool* io_4_segment_2, bool* io_4_segment_3, bool* io_4_segment_4, bool* io_4_segment_5, bool* io_4_segment_6, bool* io_4_segment_7, bool* io_4_segment_8, bool* io_4_segment_9) {
    static uint32_t previous=0;
    static uint8_t digit=0;
    uint32_t now=millis();
    uint32_t period=2000+(uint32_t)((io_1+io_2)*1000.0/1023.0);
    if ((uint32_t)(now-previous)>=period) {previous=now;digit=(digit+1)%10;}
    *io_3=digit;
    *io_4_segment_0=digit>0;
    *io_4_segment_1=digit>1;
    *io_4_segment_2=digit>2;
    *io_4_segment_3=digit>3;
    *io_4_segment_4=digit>4;
    *io_4_segment_5=digit>5;
    *io_4_segment_6=digit>6;
    *io_4_segment_7=digit>7;
    *io_4_segment_8=digit>8;
    *io_4_segment_9=digit>9;

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
static double hw_value_io_1 = 0;
static uint32_t hw_sampled_io_1 = 0;
static bool hw_attempted_io_1 = false;
static double hw_value_io_2 = 0;
static uint32_t hw_sampled_io_2 = 0;
static bool hw_attempted_io_2 = false;
static int32_t hw_value_io_3 = 0;
static void hw_shift595_io_3(int32_t value) {
  static const uint8_t digits[10] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
  bool valid = value >= 0 && value < 10L;
  digitalWrite(A1, LOW);
  {
    uint8_t segments = valid && (true) ? digits[(value / 1L) % 10] : 0;
    uint8_t bits = 0;
    if (segments & 1) bits |= 1;
    if (segments & 2) bits |= 2;
    if (segments & 4) bits |= 4;
    if (segments & 8) bits |= 8;
    if (segments & 16) bits |= 16;
    if (segments & 32) bits |= 32;
    if (segments & 64) bits |= 64;
    bits = (uint8_t)~bits;
    shiftOut(A0, A2, MSBFIRST, bits);
  }
  digitalWrite(A1, HIGH);
}
static int32_t hw_shift595_previous_io_3 = INT32_MIN;
static bool hw_value_io_4_segment_0 = false;
static bool hw_value_io_4_segment_1 = false;
static bool hw_value_io_4_segment_2 = false;
static bool hw_value_io_4_segment_3 = false;
static bool hw_value_io_4_segment_4 = false;
static bool hw_value_io_4_segment_5 = false;
static bool hw_value_io_4_segment_6 = false;
static bool hw_value_io_4_segment_7 = false;
static bool hw_value_io_4_segment_8 = false;
static bool hw_value_io_4_segment_9 = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(A0, OUTPUT); digitalWrite(A0, LOW);
  pinMode(A2, OUTPUT); digitalWrite(A2, LOW);
  pinMode(A1, OUTPUT); digitalWrite(A1, LOW);
  if (hw_value_io_3 != hw_shift595_previous_io_3) {
    hw_shift595_io_3(hw_value_io_3); hw_shift595_previous_io_3 = hw_value_io_3;
  }
  pinMode(2, OUTPUT);
  analogWrite(2, hw_value_io_4_segment_0 ? (int)(hw_value_io_0*255.0/1023.0) : 0);
  pinMode(3, OUTPUT);
  analogWrite(3, hw_value_io_4_segment_1 ? (int)(hw_value_io_0*255.0/1023.0) : 0);
  pinMode(4, OUTPUT);
  analogWrite(4, hw_value_io_4_segment_2 ? (int)(hw_value_io_0*255.0/1023.0) : 0);
  pinMode(5, OUTPUT);
  analogWrite(5, hw_value_io_4_segment_3 ? (int)(hw_value_io_0*255.0/1023.0) : 0);
  pinMode(6, OUTPUT);
  analogWrite(6, hw_value_io_4_segment_4 ? (int)(hw_value_io_0*255.0/1023.0) : 0);
  pinMode(7, OUTPUT);
  analogWrite(7, hw_value_io_4_segment_5 ? (int)(hw_value_io_0*255.0/1023.0) : 0);
  pinMode(8, OUTPUT);
  analogWrite(8, hw_value_io_4_segment_6 ? (int)(hw_value_io_0*255.0/1023.0) : 0);
  pinMode(9, OUTPUT);
  analogWrite(9, hw_value_io_4_segment_7 ? (int)(hw_value_io_0*255.0/1023.0) : 0);
  pinMode(10, OUTPUT);
  analogWrite(10, hw_value_io_4_segment_8 ? (int)(hw_value_io_0*255.0/1023.0) : 0);
  pinMode(11, OUTPUT);
  analogWrite(11, hw_value_io_4_segment_9 ? (int)(hw_value_io_0*255.0/1023.0) : 0);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 10UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (analogRead(A3) * 1.0 + 0.0);
  }
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 10UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = (analogRead(A4) * 1.0 + 0.0);
  }
  if (!hw_attempted_io_2 || (uint32_t)(hw_now - hw_sampled_io_2) >= 10UL) {
    hw_attempted_io_2 = true; hw_sampled_io_2 = hw_now;
    hw_value_io_2 = (analogRead(A5) * 1.0 + 0.0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, &hw_value_io_3, &hw_value_io_4_segment_0, &hw_value_io_4_segment_1, &hw_value_io_4_segment_2, &hw_value_io_4_segment_3, &hw_value_io_4_segment_4, &hw_value_io_4_segment_5, &hw_value_io_4_segment_6, &hw_value_io_4_segment_7, &hw_value_io_4_segment_8, &hw_value_io_4_segment_9);
  if (hw_value_io_3 != hw_shift595_previous_io_3) {
    hw_shift595_io_3(hw_value_io_3); hw_shift595_previous_io_3 = hw_value_io_3;
  }
  analogWrite(2, hw_value_io_4_segment_0 ? (int)(hw_value_io_0*255.0/1023.0) : 0);
  analogWrite(3, hw_value_io_4_segment_1 ? (int)(hw_value_io_0*255.0/1023.0) : 0);
  analogWrite(4, hw_value_io_4_segment_2 ? (int)(hw_value_io_0*255.0/1023.0) : 0);
  analogWrite(5, hw_value_io_4_segment_3 ? (int)(hw_value_io_0*255.0/1023.0) : 0);
  analogWrite(6, hw_value_io_4_segment_4 ? (int)(hw_value_io_0*255.0/1023.0) : 0);
  analogWrite(7, hw_value_io_4_segment_5 ? (int)(hw_value_io_0*255.0/1023.0) : 0);
  analogWrite(8, hw_value_io_4_segment_6 ? (int)(hw_value_io_0*255.0/1023.0) : 0);
  analogWrite(9, hw_value_io_4_segment_7 ? (int)(hw_value_io_0*255.0/1023.0) : 0);
  analogWrite(10, hw_value_io_4_segment_8 ? (int)(hw_value_io_0*255.0/1023.0) : 0);
  analogWrite(11, hw_value_io_4_segment_9 ? (int)(hw_value_io_0*255.0/1023.0) : 0);
}
