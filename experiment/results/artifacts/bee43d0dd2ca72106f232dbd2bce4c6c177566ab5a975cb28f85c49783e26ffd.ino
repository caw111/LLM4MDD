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
/* Global Variable define of  */
extern bool sysml_state_previousPressed;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, int32_t* io_1, bool* io_2_segment_0, bool* io_2_segment_1, bool* io_2_segment_2, bool* io_2_segment_3, bool* io_2_segment_4, bool* io_2_segment_5, bool* io_2_segment_6, bool* io_2_segment_7, bool* io_2_segment_8, bool* io_2_segment_9);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_digit = 0;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_digit = 0;
    sysml_state_previousPressed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, int32_t* io_1, bool* io_2_segment_0, bool* io_2_segment_1, bool* io_2_segment_2, bool* io_2_segment_3, bool* io_2_segment_4, bool* io_2_segment_5, bool* io_2_segment_6, bool* io_2_segment_7, bool* io_2_segment_8, bool* io_2_segment_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_conditional_1 = 0;
    bool sysml_conditional_2 = 0;
    bool sysml_conditional_3 = 0;
    bool sysml_conditional_4 = 0;
    bool sysml_conditional_5 = 0;
    bool sysml_conditional_6 = 0;
    bool sysml_conditional_7 = 0;
    bool sysml_conditional_8 = 0;
    bool sysml_conditional_9 = 0;
    bool sysml_conditional_10 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        if ((sysml_state_digit >= 9)) {
            sysml_state_digit = 0;
        }
        else {
            sysml_state_digit = (sysml_state_digit + 1);
        }
    }
    sysml_state_previousPressed = io_0;
    int32_t sysml_value_0 = 0;
    sysml_value_0 = sysml_state_digit;
    bool sysml_value_1 = 0;
    if (sysml_state_digit == 0) {
        sysml_conditional_1 = true;
    }
    else {
        sysml_conditional_1 = false;
    }
    sysml_value_1 = sysml_conditional_1;
    bool sysml_value_2 = 0;
    if (sysml_state_digit == 1) {
        sysml_conditional_2 = true;
    }
    else {
        sysml_conditional_2 = false;
    }
    sysml_value_2 = sysml_conditional_2;
    bool sysml_value_3 = 0;
    if (sysml_state_digit == 2) {
        sysml_conditional_3 = true;
    }
    else {
        sysml_conditional_3 = false;
    }
    sysml_value_3 = sysml_conditional_3;
    bool sysml_value_4 = 0;
    if (sysml_state_digit == 3) {
        sysml_conditional_4 = true;
    }
    else {
        sysml_conditional_4 = false;
    }
    sysml_value_4 = sysml_conditional_4;
    bool sysml_value_5 = 0;
    if (sysml_state_digit == 4) {
        sysml_conditional_5 = true;
    }
    else {
        sysml_conditional_5 = false;
    }
    sysml_value_5 = sysml_conditional_5;
    bool sysml_value_6 = 0;
    if (sysml_state_digit == 5) {
        sysml_conditional_6 = true;
    }
    else {
        sysml_conditional_6 = false;
    }
    sysml_value_6 = sysml_conditional_6;
    bool sysml_value_7 = 0;
    if (sysml_state_digit == 6) {
        sysml_conditional_7 = true;
    }
    else {
        sysml_conditional_7 = false;
    }
    sysml_value_7 = sysml_conditional_7;
    bool sysml_value_8 = 0;
    if (sysml_state_digit == 7) {
        sysml_conditional_8 = true;
    }
    else {
        sysml_conditional_8 = false;
    }
    sysml_value_8 = sysml_conditional_8;
    bool sysml_value_9 = 0;
    if (sysml_state_digit == 8) {
        sysml_conditional_9 = true;
    }
    else {
        sysml_conditional_9 = false;
    }
    sysml_value_9 = sysml_conditional_9;
    bool sysml_value_10 = 0;
    if (sysml_state_digit == 9) {
        sysml_conditional_10 = true;
    }
    else {
        sysml_conditional_10 = false;
    }
    sysml_value_10 = sysml_conditional_10;
    *io_1 = sysml_value_0;
    *io_2_segment_0 = sysml_value_1;
    *io_2_segment_1 = sysml_value_2;
    *io_2_segment_2 = sysml_value_3;
    *io_2_segment_3 = sysml_value_4;
    *io_2_segment_4 = sysml_value_5;
    *io_2_segment_5 = sysml_value_6;
    *io_2_segment_6 = sysml_value_7;
    *io_2_segment_7 = sysml_value_8;
    *io_2_segment_8 = sysml_value_9;
    *io_2_segment_9 = sysml_value_10;
    *io_2_segment_0=sysml_state_digit>0;
    *io_2_segment_1=sysml_state_digit>1;
    *io_2_segment_2=sysml_state_digit>2;
    *io_2_segment_3=sysml_state_digit>3;
    *io_2_segment_4=sysml_state_digit>4;
    *io_2_segment_5=sysml_state_digit>5;
    *io_2_segment_6=sysml_state_digit>6;
    *io_2_segment_7=sysml_state_digit>7;
    *io_2_segment_8=sysml_state_digit>8;
    *io_2_segment_9=sysml_state_digit>9;

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
static int32_t hw_value_io_1 = 0;
static void hw_shift595_io_1(int32_t value) {
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
static int32_t hw_shift595_previous_io_1 = INT32_MIN;
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
  pinMode(A0, OUTPUT); digitalWrite(A0, LOW);
  pinMode(A2, OUTPUT); digitalWrite(A2, LOW);
  pinMode(A1, OUTPUT); digitalWrite(A1, LOW);
  if (hw_value_io_1 != hw_shift595_previous_io_1) {
    hw_shift595_io_1(hw_value_io_1); hw_shift595_previous_io_1 = hw_value_io_1;
  }
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
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 5UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = manual_debounce_input((digitalRead(2) == LOW), 0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, &hw_value_io_1, &hw_value_io_2_segment_0, &hw_value_io_2_segment_1, &hw_value_io_2_segment_2, &hw_value_io_2_segment_3, &hw_value_io_2_segment_4, &hw_value_io_2_segment_5, &hw_value_io_2_segment_6, &hw_value_io_2_segment_7, &hw_value_io_2_segment_8, &hw_value_io_2_segment_9);
  if (hw_value_io_1 != hw_shift595_previous_io_1) {
    hw_shift595_io_1(hw_value_io_1); hw_shift595_previous_io_1 = hw_value_io_1;
  }
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
