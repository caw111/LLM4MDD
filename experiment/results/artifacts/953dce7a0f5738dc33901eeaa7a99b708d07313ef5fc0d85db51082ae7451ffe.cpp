#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_008\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
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
extern double sysml_state_lastValidDistance;
/* Global Variable define of  */
extern bool sysml_state_hasEverValidDistance;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_2, bool io_3, bool* io_0, int32_t* io_1, int32_t* io_4);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_lastValidDistance = 0.0;
/* Global Variable define of  */
bool sysml_state_hasEverValidDistance = false;
/* Init function of CompositeActor named "ProgramPage1" */
#line 117 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_008\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void setup();
#line 139 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_008\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void loop();
#line 33 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_008\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_lastValidDistance = 0.0;
    sysml_state_hasEverValidDistance = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_2, bool io_3, bool* io_0, int32_t* io_1, int32_t* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int32_t sysml_conditional_1 = 0;
    int32_t sysml_conditional_2 = 0;
    int32_t sysml_conditional_3 = 0;
    int32_t sysml_conditional_4 = 0;
    int32_t sysml_conditional_5 = 0;
    if (io_3) {
        sysml_state_lastValidDistance = io_2;
        sysml_state_hasEverValidDistance = true;
    }
    bool sysml_value_0 = 0;
    sysml_lazy_0 = sysml_state_hasEverValidDistance;
    if (sysml_lazy_0) {
        sysml_lazy_0 = sysml_state_lastValidDistance < 30.0;
    }
    sysml_value_0 = sysml_lazy_0;
    int32_t sysml_value_1 = 0;
    if (!sysml_state_hasEverValidDistance) {
        sysml_conditional_1 = 0;
    }
    else {
        if (sysml_state_lastValidDistance < 8.0) {
            sysml_conditional_2 = 1000;
        }
        else {
            if (sysml_state_lastValidDistance < 15.0) {
                sysml_conditional_3 = 600;
            }
            else {
                sysml_conditional_3 = 0;
            }
            sysml_conditional_2 = sysml_conditional_3;
        }
        sysml_conditional_1 = sysml_conditional_2;
    }
    sysml_value_1 = sysml_conditional_1;
    int32_t sysml_value_2 = 0;
    if (sysml_state_hasEverValidDistance) {
        if (sysml_state_lastValidDistance < ((double)((int32_t)sysml_state_lastValidDistance))) {
            sysml_conditional_5 = 1;
        }
        else {
            sysml_conditional_5 = 0;
        }
        sysml_conditional_4 = ((int32_t)sysml_state_lastValidDistance) - sysml_conditional_5;
    }
    else {
        sysml_conditional_4 = 0;
    }
    sysml_value_2 = sysml_conditional_4;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_4 = sysml_value_2;
}
// END COMPILED SYSML MODEL

// LLM4MDD target-board: arduino:avr:uno
#include <TM1637Display.h>
#include <Arduino.h>
#include <math.h>
#if defined(ARDUINO) && !defined(ARDUINO_AVR_UNO)
#error "Selected Arduino board differs from the interface contract"
#endif
static uint32_t hw_last_step = 0;
static bool hw_started = false;
static bool hw_value_io_0 = false;
static int32_t hw_value_io_1 = 0;
static int32_t hw_tone_io_1 = -1;
static double hw_value_io_2 = 0;
static bool hw_value_io_3 = false;
static int32_t hw_value_io_4 = 0;
TM1637Display hw_display_io_4(4, 5);
static int32_t hw_display_previous_io_4 = INT32_MIN;
static uint32_t hw_ultrasonic_sampled_0 = 0;
static bool hw_ultrasonic_attempted_0 = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_0 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  int32_t hw_tone_command_io_1 = (hw_value_io_1 >= 31 && hw_value_io_1 <= 65535) ? hw_value_io_1 : 0;
  if (hw_tone_command_io_1 != hw_tone_io_1) {
    if (hw_tone_command_io_1 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_1);
    hw_tone_io_1 = hw_tone_command_io_1;
  }
  hw_display_io_4.setBrightness(7, true);
  if (hw_value_io_4 != hw_display_previous_io_4) {
    if (hw_value_io_4 < -999 || hw_value_io_4 > 9999) hw_display_io_4.clear();
    else hw_display_io_4.showNumberDec((int)hw_value_io_4, false, 4, 0);
    hw_display_previous_io_4 = hw_value_io_4;
  }
  pinMode(8, OUTPUT);
  digitalWrite(8, LOW);
  pinMode(9, INPUT);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  uint32_t hw_ultrasonic_now_0 = (uint32_t)millis();
  if (!hw_ultrasonic_attempted_0 || (uint32_t)(hw_ultrasonic_now_0 - hw_ultrasonic_sampled_0) >= 60UL) {
    hw_ultrasonic_attempted_0 = true; hw_ultrasonic_sampled_0 = hw_ultrasonic_now_0;
    digitalWrite(8, LOW); delayMicroseconds(2);
    digitalWrite(8, HIGH); delayMicroseconds(10); digitalWrite(8, LOW);
    unsigned long hw_ultrasonic_pulse_0 = pulseIn(9, HIGH, 30000UL);
    hw_value_io_3 = (hw_ultrasonic_pulse_0 >= 116UL && hw_ultrasonic_pulse_0 <= 23200UL);
    if (hw_value_io_3) hw_value_io_2 = hw_ultrasonic_pulse_0 / 58.0;
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_2, hw_value_io_3, &hw_value_io_0, &hw_value_io_1, &hw_value_io_4);
  digitalWrite(13, hw_value_io_0 ? HIGH : LOW);
  int32_t hw_tone_command_io_1 = (hw_value_io_1 >= 31 && hw_value_io_1 <= 65535) ? hw_value_io_1 : 0;
  if (hw_tone_command_io_1 != hw_tone_io_1) {
    if (hw_tone_command_io_1 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_1);
    hw_tone_io_1 = hw_tone_command_io_1;
  }
  if (hw_value_io_4 != hw_display_previous_io_4) {
    if (hw_value_io_4 < -999 || hw_value_io_4 > 9999) hw_display_io_4.clear();
    else hw_display_io_4.showNumberDec((int)hw_value_io_4, false, 4, 0);
    hw_display_previous_io_4 = hw_value_io_4;
  }
}

