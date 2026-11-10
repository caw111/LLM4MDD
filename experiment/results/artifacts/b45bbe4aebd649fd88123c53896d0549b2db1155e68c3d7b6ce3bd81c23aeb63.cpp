#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision10\\cases\\embedbench_87\\arduino\\sketch\\sketch.ino"
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
extern int32_t sysml_state_redIntensity;
/* Global Variable define of  */
extern int32_t sysml_state_greenIntensity;
/* Global Variable define of  */
extern int32_t sysml_state_blueIntensity;
/* Global Variable define of  */
extern int32_t sysml_state_selectedColor;
/* Global Variable define of  */
extern bool sysml_state_previousBtn1Pressed;
/* Global Variable define of  */
extern bool sysml_state_previousBtn2Pressed;
/* Global Variable define of  */
extern bool sysml_state_previousBtn3Pressed;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, double* io_3, double* io_4, double* io_5, bool* io_6_segment_0, bool* io_6_segment_1, bool* io_6_segment_2, bool* io_6_segment_3, bool* io_6_segment_4, bool* io_6_segment_5, bool* io_6_segment_6, bool* io_6_segment_7);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_redIntensity = 0;
/* Global Variable define of  */
int32_t sysml_state_greenIntensity = 0;
/* Global Variable define of  */
int32_t sysml_state_blueIntensity = 0;
/* Global Variable define of  */
int32_t sysml_state_selectedColor = 0;
/* Global Variable define of  */
bool sysml_state_previousBtn1Pressed = false;
/* Global Variable define of  */
bool sysml_state_previousBtn2Pressed = false;
/* Global Variable define of  */
bool sysml_state_previousBtn3Pressed = false;
/* Init function of CompositeActor named "ProgramPage1" */
#line 368 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision10\\cases\\embedbench_87\\arduino\\sketch\\sketch.ino"
static bool manual_debounce_input(bool raw, uint8_t index);
#line 377 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision10\\cases\\embedbench_87\\arduino\\sketch\\sketch.ino"
void setup();
#line 406 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision10\\cases\\embedbench_87\\arduino\\sketch\\sketch.ino"
void loop();
#line 53 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision10\\cases\\embedbench_87\\arduino\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_redIntensity = 0;
    sysml_state_greenIntensity = 0;
    sysml_state_blueIntensity = 0;
    sysml_state_selectedColor = 0;
    sysml_state_previousBtn1Pressed = false;
    sysml_state_previousBtn2Pressed = false;
    sysml_state_previousBtn3Pressed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, double* io_3, double* io_4, double* io_5, bool* io_6_segment_0, bool* io_6_segment_1, bool* io_6_segment_2, bool* io_6_segment_3, bool* io_6_segment_4, bool* io_6_segment_5, bool* io_6_segment_6, bool* io_6_segment_7) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int32_t sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    int32_t sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    int32_t sysml_conditional_5 = 0;
    int32_t sysml_conditional_6 = 0;
    int32_t sysml_conditional_7 = 0;
    int32_t sysml_conditional_8 = 0;
    int32_t sysml_conditional_9 = 0;
    int32_t sysml_conditional_10 = 0;
    int32_t sysml_conditional_11 = 0;
    int32_t sysml_conditional_12 = 0;
    int32_t sysml_conditional_13 = 0;
    int32_t sysml_conditional_14 = 0;
    bool sysml_conditional_15 = 0;
    bool sysml_conditional_16 = 0;
    bool sysml_conditional_17 = 0;
    bool sysml_conditional_18 = 0;
    bool sysml_conditional_19 = 0;
    bool sysml_conditional_20 = 0;
    bool sysml_conditional_21 = 0;
    bool sysml_conditional_22 = 0;
    bool sysml_conditional_23 = 0;
    bool sysml_conditional_24 = 0;
    bool sysml_conditional_25 = 0;
    bool sysml_conditional_26 = 0;
    bool sysml_conditional_27 = 0;
    bool sysml_conditional_28 = 0;
    bool sysml_conditional_29 = 0;
    bool sysml_conditional_30 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousBtn1Pressed;
    }
    if (sysml_lazy_0) {
        sysml_state_selectedColor = 0;
        if (sysml_state_redIntensity >= 255) {
            sysml_conditional_1 = 0;
        }
        else {
            sysml_conditional_1 = (sysml_state_redIntensity <= 223 ? sysml_state_redIntensity + 32 : 255);
        }
        sysml_state_redIntensity = sysml_conditional_1;
    }
    sysml_lazy_2 = io_1;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_previousBtn2Pressed;
    }
    if (sysml_lazy_2) {
        sysml_state_selectedColor = 1;
        if (sysml_state_greenIntensity >= 255) {
            sysml_conditional_3 = 0;
        }
        else {
            sysml_conditional_3 = (sysml_state_greenIntensity <= 223 ? sysml_state_greenIntensity + 32 : 255);
        }
        sysml_state_greenIntensity = sysml_conditional_3;
    }
    sysml_lazy_4 = io_2;
    if (sysml_lazy_4) {
        sysml_lazy_4 = !sysml_state_previousBtn3Pressed;
    }
    if (sysml_lazy_4) {
        sysml_state_selectedColor = 2;
        if (sysml_state_blueIntensity >= 255) {
            sysml_conditional_5 = 0;
        }
        else {
            sysml_conditional_5 = (sysml_state_blueIntensity <= 223 ? sysml_state_blueIntensity + 32 : 255);
        }
        sysml_state_blueIntensity = sysml_conditional_5;
    }
    sysml_state_previousBtn1Pressed = io_0;
    sysml_state_previousBtn2Pressed = io_1;
    sysml_state_previousBtn3Pressed = io_2;
    double sysml_value_0 = 0;
    if (sysml_state_selectedColor == 0) {
        if (sysml_state_redIntensity < ((double)((int32_t)sysml_state_redIntensity))) {
            sysml_conditional_7 = 1;
        }
        else {
            sysml_conditional_7 = 0;
        }
        sysml_conditional_6 = ((int32_t)sysml_state_redIntensity) - sysml_conditional_7;
    }
    else {
        if (sysml_state_redIntensity < ((double)((int32_t)sysml_state_redIntensity))) {
            sysml_conditional_8 = 1;
        }
        else {
            sysml_conditional_8 = 0;
        }
        sysml_conditional_6 = ((int32_t)sysml_state_redIntensity) - sysml_conditional_8;
    }
    sysml_value_0 = sysml_conditional_6;
    double sysml_value_1 = 0;
    if (sysml_state_selectedColor == 1) {
        if (sysml_state_greenIntensity < ((double)((int32_t)sysml_state_greenIntensity))) {
            sysml_conditional_10 = 1;
        }
        else {
            sysml_conditional_10 = 0;
        }
        sysml_conditional_9 = ((int32_t)sysml_state_greenIntensity) - sysml_conditional_10;
    }
    else {
        if (sysml_state_greenIntensity < ((double)((int32_t)sysml_state_greenIntensity))) {
            sysml_conditional_11 = 1;
        }
        else {
            sysml_conditional_11 = 0;
        }
        sysml_conditional_9 = ((int32_t)sysml_state_greenIntensity) - sysml_conditional_11;
    }
    sysml_value_1 = sysml_conditional_9;
    double sysml_value_2 = 0;
    if (sysml_state_selectedColor == 2) {
        if (sysml_state_blueIntensity < ((double)((int32_t)sysml_state_blueIntensity))) {
            sysml_conditional_13 = 1;
        }
        else {
            sysml_conditional_13 = 0;
        }
        sysml_conditional_12 = ((int32_t)sysml_state_blueIntensity) - sysml_conditional_13;
    }
    else {
        if (sysml_state_blueIntensity < ((double)((int32_t)sysml_state_blueIntensity))) {
            sysml_conditional_14 = 1;
        }
        else {
            sysml_conditional_14 = 0;
        }
        sysml_conditional_12 = ((int32_t)sysml_state_blueIntensity) - sysml_conditional_14;
    }
    sysml_value_2 = sysml_conditional_12;
    bool sysml_value_3 = 0;
    if (sysml_state_selectedColor == 0) {
        sysml_conditional_15 = sysml_state_redIntensity >= 32;
    }
    else {
        if (sysml_state_selectedColor == 1) {
            sysml_conditional_16 = sysml_state_greenIntensity >= 32;
        }
        else {
            sysml_conditional_16 = sysml_state_blueIntensity >= 32;
        }
        sysml_conditional_15 = sysml_conditional_16;
    }
    sysml_value_3 = sysml_conditional_15;
    bool sysml_value_4 = 0;
    if (sysml_state_selectedColor == 0) {
        sysml_conditional_17 = sysml_state_redIntensity >= 64;
    }
    else {
        if (sysml_state_selectedColor == 1) {
            sysml_conditional_18 = sysml_state_greenIntensity >= 64;
        }
        else {
            sysml_conditional_18 = sysml_state_blueIntensity >= 64;
        }
        sysml_conditional_17 = sysml_conditional_18;
    }
    sysml_value_4 = sysml_conditional_17;
    bool sysml_value_5 = 0;
    if (sysml_state_selectedColor == 0) {
        sysml_conditional_19 = sysml_state_redIntensity >= 96;
    }
    else {
        if (sysml_state_selectedColor == 1) {
            sysml_conditional_20 = sysml_state_greenIntensity >= 96;
        }
        else {
            sysml_conditional_20 = sysml_state_blueIntensity >= 96;
        }
        sysml_conditional_19 = sysml_conditional_20;
    }
    sysml_value_5 = sysml_conditional_19;
    bool sysml_value_6 = 0;
    if (sysml_state_selectedColor == 0) {
        sysml_conditional_21 = sysml_state_redIntensity >= 128;
    }
    else {
        if (sysml_state_selectedColor == 1) {
            sysml_conditional_22 = sysml_state_greenIntensity >= 128;
        }
        else {
            sysml_conditional_22 = sysml_state_blueIntensity >= 128;
        }
        sysml_conditional_21 = sysml_conditional_22;
    }
    sysml_value_6 = sysml_conditional_21;
    bool sysml_value_7 = 0;
    if (sysml_state_selectedColor == 0) {
        sysml_conditional_23 = sysml_state_redIntensity >= 160;
    }
    else {
        if (sysml_state_selectedColor == 1) {
            sysml_conditional_24 = sysml_state_greenIntensity >= 160;
        }
        else {
            sysml_conditional_24 = sysml_state_blueIntensity >= 160;
        }
        sysml_conditional_23 = sysml_conditional_24;
    }
    sysml_value_7 = sysml_conditional_23;
    bool sysml_value_8 = 0;
    if (sysml_state_selectedColor == 0) {
        sysml_conditional_25 = sysml_state_redIntensity >= 192;
    }
    else {
        if (sysml_state_selectedColor == 1) {
            sysml_conditional_26 = sysml_state_greenIntensity >= 192;
        }
        else {
            sysml_conditional_26 = sysml_state_blueIntensity >= 192;
        }
        sysml_conditional_25 = sysml_conditional_26;
    }
    sysml_value_8 = sysml_conditional_25;
    bool sysml_value_9 = 0;
    if (sysml_state_selectedColor == 0) {
        sysml_conditional_27 = sysml_state_redIntensity >= 224;
    }
    else {
        if (sysml_state_selectedColor == 1) {
            sysml_conditional_28 = sysml_state_greenIntensity >= 224;
        }
        else {
            sysml_conditional_28 = sysml_state_blueIntensity >= 224;
        }
        sysml_conditional_27 = sysml_conditional_28;
    }
    sysml_value_9 = sysml_conditional_27;
    bool sysml_value_10 = 0;
    if (sysml_state_selectedColor == 0) {
        sysml_conditional_29 = sysml_state_redIntensity >= 255;
    }
    else {
        if (sysml_state_selectedColor == 1) {
            sysml_conditional_30 = sysml_state_greenIntensity >= 255;
        }
        else {
            sysml_conditional_30 = sysml_state_blueIntensity >= 255;
        }
        sysml_conditional_29 = sysml_conditional_30;
    }
    sysml_value_10 = sysml_conditional_29;
    *io_3 = sysml_value_0;
    *io_4 = sysml_value_1;
    *io_5 = sysml_value_2;
    *io_6_segment_0 = sysml_value_3;
    *io_6_segment_1 = sysml_value_4;
    *io_6_segment_2 = sysml_value_5;
    *io_6_segment_3 = sysml_value_6;
    *io_6_segment_4 = sysml_value_7;
    *io_6_segment_5 = sysml_value_8;
    *io_6_segment_6 = sysml_value_9;
    *io_6_segment_7 = sysml_value_10;
    int latest=sysml_state_selectedColor==0 ? sysml_state_redIntensity : (sysml_state_selectedColor==1 ? sysml_state_greenIntensity : sysml_state_blueIntensity);
    int segments=latest==255 ? 8 : latest/32;
    *io_6_segment_0=segments>0;
    *io_6_segment_1=segments>1;
    *io_6_segment_2=segments>2;
    *io_6_segment_3=segments>3;
    *io_6_segment_4=segments>4;
    *io_6_segment_5=segments>5;
    *io_6_segment_6=segments>6;
    *io_6_segment_7=segments>7;

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
static bool hw_value_io_6_segment_0 = false;
static bool hw_value_io_6_segment_1 = false;
static bool hw_value_io_6_segment_2 = false;
static bool hw_value_io_6_segment_3 = false;
static bool hw_value_io_6_segment_4 = false;
static bool hw_value_io_6_segment_5 = false;
static bool hw_value_io_6_segment_6 = false;
static bool hw_value_io_6_segment_7 = false;

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
  analogWrite(5, (int)constrain((hw_value_io_3 * 1 + 0.0), 0.0, 255.0));
  pinMode(6, OUTPUT);
  analogWrite(6, (int)constrain((hw_value_io_4 * 1 + 0.0), 0.0, 255.0));
  pinMode(7, OUTPUT);
  analogWrite(7, (int)constrain((hw_value_io_5 * 1 + 0.0), 0.0, 255.0));
  pinMode(8, OUTPUT);
  digitalWrite(8, hw_value_io_6_segment_0 ? HIGH : LOW);
  pinMode(9, OUTPUT);
  digitalWrite(9, hw_value_io_6_segment_1 ? HIGH : LOW);
  pinMode(10, OUTPUT);
  digitalWrite(10, hw_value_io_6_segment_2 ? HIGH : LOW);
  pinMode(11, OUTPUT);
  digitalWrite(11, hw_value_io_6_segment_3 ? HIGH : LOW);
  pinMode(12, OUTPUT);
  digitalWrite(12, hw_value_io_6_segment_4 ? HIGH : LOW);
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_6_segment_5 ? HIGH : LOW);
  pinMode(44, OUTPUT);
  digitalWrite(44, hw_value_io_6_segment_6 ? HIGH : LOW);
  pinMode(45, OUTPUT);
  digitalWrite(45, hw_value_io_6_segment_7 ? HIGH : LOW);
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
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6_segment_0, &hw_value_io_6_segment_1, &hw_value_io_6_segment_2, &hw_value_io_6_segment_3, &hw_value_io_6_segment_4, &hw_value_io_6_segment_5, &hw_value_io_6_segment_6, &hw_value_io_6_segment_7);
  analogWrite(5, (int)constrain((hw_value_io_3 * 1 + 0.0), 0.0, 255.0));
  analogWrite(6, (int)constrain((hw_value_io_4 * 1 + 0.0), 0.0, 255.0));
  analogWrite(7, (int)constrain((hw_value_io_5 * 1 + 0.0), 0.0, 255.0));
  digitalWrite(8, hw_value_io_6_segment_0 ? HIGH : LOW);
  digitalWrite(9, hw_value_io_6_segment_1 ? HIGH : LOW);
  digitalWrite(10, hw_value_io_6_segment_2 ? HIGH : LOW);
  digitalWrite(11, hw_value_io_6_segment_3 ? HIGH : LOW);
  digitalWrite(12, hw_value_io_6_segment_4 ? HIGH : LOW);
  digitalWrite(13, hw_value_io_6_segment_5 ? HIGH : LOW);
  digitalWrite(44, hw_value_io_6_segment_6 ? HIGH : LOW);
  digitalWrite(45, hw_value_io_6_segment_7 ? HIGH : LOW);
}

