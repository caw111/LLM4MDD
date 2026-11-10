#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision10\\cases\\embedbench_89\\arduino\\sketch\\sketch.ino"
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
extern int32_t sysml_state_count;
/* Global Variable define of  */
extern bool sysml_state_prevBtn0;
/* Global Variable define of  */
extern bool sysml_state_prevBtn1;
/* Global Variable define of  */
extern bool sysml_state_prevBtn2;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, double* io_3, double* io_4, double* io_5, double* io_6, double* io_7, double* io_8, int32_t* io_9);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_count = 0;
/* Global Variable define of  */
bool sysml_state_prevBtn0 = false;
/* Global Variable define of  */
bool sysml_state_prevBtn1 = false;
/* Global Variable define of  */
bool sysml_state_prevBtn2 = false;
/* Init function of CompositeActor named "ProgramPage1" */
#line 273 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision10\\cases\\embedbench_89\\arduino\\sketch\\sketch.ino"
static void hw_shift595_io_9(int32_t value);
#line 294 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision10\\cases\\embedbench_89\\arduino\\sketch\\sketch.ino"
static bool manual_debounce_input(bool raw, uint8_t index);
#line 303 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision10\\cases\\embedbench_89\\arduino\\sketch\\sketch.ino"
void setup();
#line 328 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision10\\cases\\embedbench_89\\arduino\\sketch\\sketch.ino"
void loop();
#line 41 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision10\\cases\\embedbench_89\\arduino\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_count = 0;
    sysml_state_prevBtn0 = false;
    sysml_state_prevBtn1 = false;
    sysml_state_prevBtn2 = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, double* io_3, double* io_4, double* io_5, double* io_6, double* io_7, double* io_8, int32_t* io_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int32_t sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    int32_t sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    int32_t sysml_conditional_5 = 0;
    double sysml_conditional_6 = 0;
    double sysml_conditional_7 = 0;
    double sysml_conditional_8 = 0;
    bool sysml_lazy_9 = 0;
    double sysml_conditional_10 = 0;
    bool sysml_lazy_11 = 0;
    double sysml_conditional_12 = 0;
    double sysml_conditional_13 = 0;
    double sysml_conditional_14 = 0;
    double sysml_conditional_15 = 0;
    bool sysml_lazy_16 = 0;
    double sysml_conditional_17 = 0;
    double sysml_conditional_18 = 0;
    double sysml_conditional_19 = 0;
    double sysml_conditional_20 = 0;
    bool sysml_lazy_21 = 0;
    double sysml_conditional_22 = 0;
    double sysml_conditional_23 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_prevBtn0;
    }
    if (sysml_lazy_0) {
        if (sysml_state_count >= 15) {
            sysml_conditional_1 = 0;
        }
        else {
            sysml_conditional_1 = sysml_state_count + 1;
        }
        sysml_state_count = sysml_conditional_1;
    }
    sysml_lazy_2 = io_1;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_prevBtn1;
    }
    if (sysml_lazy_2) {
        if (sysml_state_count >= 15) {
            sysml_conditional_3 = 0;
        }
        else {
            sysml_conditional_3 = sysml_state_count + 1;
        }
        sysml_state_count = sysml_conditional_3;
    }
    sysml_lazy_4 = io_2;
    if (sysml_lazy_4) {
        sysml_lazy_4 = !sysml_state_prevBtn2;
    }
    if (sysml_lazy_4) {
        if (sysml_state_count >= 15) {
            sysml_conditional_5 = 0;
        }
        else {
            sysml_conditional_5 = sysml_state_count + 1;
        }
        sysml_state_count = sysml_conditional_5;
    }
    sysml_state_prevBtn0 = io_0;
    sysml_state_prevBtn1 = io_1;
    sysml_state_prevBtn2 = io_2;
    double sysml_value_0 = 0;
    if (sysml_state_count >= 10) {
        sysml_conditional_6 = 255.0;
    }
    else {
        if (sysml_state_count >= 5) {
            sysml_conditional_7 = 0.0;
        }
        else {
            if (sysml_state_count < 5) {
                sysml_conditional_8 = 255.0;
            }
            else {
                sysml_conditional_8 = 0.0;
            }
            sysml_conditional_7 = sysml_conditional_8;
        }
        sysml_conditional_6 = sysml_conditional_7;
    }
    sysml_value_0 = sysml_conditional_6;
    double sysml_value_1 = 0;
    sysml_lazy_9 = sysml_state_count >= 5;
    if (sysml_lazy_9) {
        sysml_lazy_9 = sysml_state_count <= 9;
    }
    if (sysml_lazy_9) {
        sysml_conditional_10 = 255.0;
    }
    else {
        sysml_lazy_11 = sysml_state_count >= 10;
        if (sysml_lazy_11) {
            sysml_lazy_11 = sysml_state_count <= 15;
        }
        if (sysml_lazy_11) {
            sysml_conditional_12 = 0.0;
        }
        else {
            sysml_conditional_12 = 0.0;
        }
        sysml_conditional_10 = sysml_conditional_12;
    }
    sysml_value_1 = sysml_conditional_10;
    double sysml_value_2 = 0;
    if (sysml_state_count < 5) {
        sysml_conditional_13 = 255.0;
    }
    else {
        if (sysml_state_count >= 10) {
            sysml_conditional_14 = 255.0;
        }
        else {
            sysml_conditional_14 = 0.0;
        }
        sysml_conditional_13 = sysml_conditional_14;
    }
    sysml_value_2 = sysml_conditional_13;
    double sysml_value_3 = 0;
    if (sysml_state_count < 5) {
        sysml_conditional_15 = 0.0;
    }
    else {
        sysml_lazy_16 = sysml_state_count >= 5;
        if (sysml_lazy_16) {
            sysml_lazy_16 = sysml_state_count <= 9;
        }
        if (sysml_lazy_16) {
            sysml_conditional_17 = 255.0;
        }
        else {
            if (sysml_state_count >= 10) {
                sysml_conditional_18 = 255.0;
            }
            else {
                sysml_conditional_18 = 0.0;
            }
            sysml_conditional_17 = sysml_conditional_18;
        }
        sysml_conditional_15 = sysml_conditional_17;
    }
    sysml_value_3 = sysml_conditional_15;
    double sysml_value_4 = 0;
    if (sysml_state_count < 5) {
        sysml_conditional_19 = 0.0;
    }
    else {
        if (sysml_state_count <= 9) {
            sysml_conditional_20 = 255.0;
        }
        else {
            sysml_conditional_20 = 0.0;
        }
        sysml_conditional_19 = sysml_conditional_20;
    }
    sysml_value_4 = sysml_conditional_19;
    double sysml_value_5 = 0;
    sysml_lazy_21 = sysml_state_count >= 5;
    if (sysml_lazy_21) {
        sysml_lazy_21 = sysml_state_count <= 9;
    }
    if (sysml_lazy_21) {
        sysml_conditional_22 = 0.0;
    }
    else {
        if (sysml_state_count >= 10) {
            sysml_conditional_23 = 255.0;
        }
        else {
            sysml_conditional_23 = 255.0;
        }
        sysml_conditional_22 = sysml_conditional_23;
    }
    sysml_value_5 = sysml_conditional_22;
    int32_t sysml_value_6 = 0;
    sysml_value_6 = sysml_state_count;
    *io_3 = sysml_value_0;
    *io_4 = sysml_value_1;
    *io_5 = sysml_value_2;
    *io_6 = sysml_value_3;
    *io_7 = sysml_value_4;
    *io_8 = sysml_value_5;
    *io_9 = sysml_value_6;
    int n=sysml_state_count;
    *io_9=n%10;
    *io_3=n>0&&(n<5||n>=10) ? 255 : 0;
    *io_4=n>=5&&n<10 ? 255 : 0;
    *io_5=n>=10 ? 255 : 0;
    *io_6=n>=5&&n<10 ? 255 : 0;
    *io_7=n>=5 ? 255 : 0;
    *io_8=n>0&&(n<5||n>=10) ? 255 : 0;
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
static bool hw_value_io_2 = false;
static uint32_t hw_sampled_io_2 = 0;
static bool hw_attempted_io_2 = false;
static double hw_value_io_3 = 0;
static double hw_value_io_4 = 0;
static double hw_value_io_5 = 0;
static double hw_value_io_6 = 0;
static double hw_value_io_7 = 0;
static double hw_value_io_8 = 0;
static int32_t hw_value_io_9 = 0;
static void hw_shift595_io_9(int32_t value) {
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
static int32_t hw_shift595_previous_io_9 = INT32_MIN;

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
  pinMode(4, INPUT_PULLUP);
  pinMode(5, OUTPUT);
  analogWrite(5, (int)constrain((hw_value_io_3 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(6, OUTPUT);
  analogWrite(6, (int)constrain((hw_value_io_4 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(7, OUTPUT);
  analogWrite(7, (int)constrain((hw_value_io_5 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(8, OUTPUT);
  analogWrite(8, (int)constrain((hw_value_io_6 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(9, OUTPUT);
  analogWrite(9, (int)constrain((hw_value_io_7 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(10, OUTPUT);
  analogWrite(10, (int)constrain((hw_value_io_8 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(A0, OUTPUT); digitalWrite(A0, LOW);
  pinMode(A2, OUTPUT); digitalWrite(A2, LOW);
  pinMode(A1, OUTPUT); digitalWrite(A1, LOW);
  if (hw_value_io_9 != hw_shift595_previous_io_9) {
    hw_shift595_io_9(hw_value_io_9); hw_shift595_previous_io_9 = hw_value_io_9;
  }
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
    hw_value_io_2 = manual_debounce_input((digitalRead(4) == LOW), 2);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_io_7, &hw_value_io_8, &hw_value_io_9);
  analogWrite(5, (int)constrain((hw_value_io_3 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(6, (int)constrain((hw_value_io_4 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(7, (int)constrain((hw_value_io_5 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(8, (int)constrain((hw_value_io_6 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(9, (int)constrain((hw_value_io_7 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(10, (int)constrain((hw_value_io_8 * 1.0 + 0.0), 0.0, 255.0));
  if (hw_value_io_9 != hw_shift595_previous_io_9) {
    hw_shift595_io_9(hw_value_io_9); hw_shift595_previous_io_9 = hw_value_io_9;
  }
}

