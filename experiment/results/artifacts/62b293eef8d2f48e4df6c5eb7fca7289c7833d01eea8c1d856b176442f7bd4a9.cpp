#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision15\\cases\\embedbench_56\\arduino\\sketch\\sketch.ino"
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
void ProgramPage1(bool* io_0, bool* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, int32_t* io_6);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Init function of CompositeActor named "ProgramPage1" */
#line 62 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision15\\cases\\embedbench_56\\arduino\\sketch\\sketch.ino"
static void hw_shift595_io_6(int32_t value);
#line 96 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision15\\cases\\embedbench_56\\arduino\\sketch\\sketch.ino"
void setup();
#line 118 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision15\\cases\\embedbench_56\\arduino\\sketch\\sketch.ino"
void loop();
#line 25 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision15\\cases\\embedbench_56\\arduino\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool* io_0, bool* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, int32_t* io_6) {
    const uint8_t colors[]={1,2,4,3,6,5,7};
    static uint32_t tick=0;
    static uint8_t colorIndex=0, number=0;
    uint32_t elapsed=(uint32_t)(millis()-tick);
    if (elapsed>=2000) {
        uint32_t steps=elapsed/2000;
        tick+=steps*2000;
        colorIndex=(colorIndex+steps)%7;
        number=(number+steps)%100;
    }
    uint8_t color=colors[colorIndex];
    *io_0=*io_3=color&1;
    *io_1=*io_4=color&2;
    *io_2=*io_5=color&4;
    *io_6=number;
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
static bool hw_value_io_0 = true;
static bool hw_value_io_1 = false;
static bool hw_value_io_2 = false;
static bool hw_value_io_3 = true;
static bool hw_value_io_4 = false;
static bool hw_value_io_5 = false;
static int32_t hw_value_io_6 = 0;
static void hw_shift595_io_6(int32_t value) {
  static const uint8_t digits[10] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
  bool valid = value >= 0 && value < 100L;
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
  {
    uint8_t segments = valid ? digits[(value / 10L) % 10] : 0;
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
static int32_t hw_shift595_previous_io_6 = INT32_MIN;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, OUTPUT);
  digitalWrite(2, hw_value_io_0 ? HIGH : LOW);
  pinMode(3, OUTPUT);
  digitalWrite(3, hw_value_io_1 ? HIGH : LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, hw_value_io_2 ? HIGH : LOW);
  pinMode(5, OUTPUT);
  digitalWrite(5, hw_value_io_3 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  digitalWrite(6, hw_value_io_4 ? HIGH : LOW);
  pinMode(7, OUTPUT);
  digitalWrite(7, hw_value_io_5 ? HIGH : LOW);
  pinMode(A0, OUTPUT); digitalWrite(A0, LOW);
  pinMode(A2, OUTPUT); digitalWrite(A2, LOW);
  pinMode(A1, OUTPUT); digitalWrite(A1, LOW);
  if (hw_value_io_6 != hw_shift595_previous_io_6) {
    hw_shift595_io_6(hw_value_io_6); hw_shift595_previous_io_6 = hw_value_io_6;
  }
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
  ProgramPage1(&hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6);
  digitalWrite(2, hw_value_io_0 ? HIGH : LOW);
  digitalWrite(3, hw_value_io_1 ? HIGH : LOW);
  digitalWrite(4, hw_value_io_2 ? HIGH : LOW);
  digitalWrite(5, hw_value_io_3 ? HIGH : LOW);
  digitalWrite(6, hw_value_io_4 ? HIGH : LOW);
  digitalWrite(7, hw_value_io_5 ? HIGH : LOW);
  if (hw_value_io_6 != hw_shift595_previous_io_6) {
    hw_shift595_io_6(hw_value_io_6); hw_shift595_previous_io_6 = hw_value_io_6;
  }
}

