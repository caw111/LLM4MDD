#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_023\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
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
extern double sysml_state_distance_cm_last_valid;
/* Global Variable define of  */
extern double sysml_state_distance_cm_effective;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, int32_t* io_2, bool* io_3, double* io_4);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_distance_cm_last_valid = 0.0;
/* Global Variable define of  */
double sysml_state_distance_cm_effective = 0.0;
/* Init function of CompositeActor named "ProgramPage1" */
#line 145 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_023\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void setup();
#line 162 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_023\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void loop();
#line 33 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_023\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_distance_cm_last_valid = 0.0;
    sysml_state_distance_cm_effective = 0.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, int32_t* io_2, bool* io_3, double* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    int32_t sysml_conditional_2 = 0;
    int32_t sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    double sysml_conditional_5 = 0;
    double sysml_conditional_6 = 0;
    double sysml_conditional_7 = 0;
    double sysml_conditional_8 = 0;
    int32_t sysml_conditional_9 = 0;
    if (io_1) {
        sysml_state_distance_cm_last_valid = io_0;
    }
    if (io_1) {
        sysml_state_distance_cm_effective = io_0;
    }
    else {
        sysml_state_distance_cm_effective = sysml_state_distance_cm_last_valid;
    }
    int32_t sysml_value_0 = 0;
    sysml_lazy_0 = io_1;
    if (sysml_lazy_0) {
        sysml_lazy_0 = sysml_state_distance_cm_effective >= 2.0;
    }
    sysml_lazy_1 = sysml_lazy_0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = sysml_state_distance_cm_effective <= 400.0;
    }
    if (sysml_lazy_1) {
        if (sysml_state_distance_cm_effective < ((double)((int32_t)sysml_state_distance_cm_effective))) {
            sysml_conditional_3 = 1;
        }
        else {
            sysml_conditional_3 = 0;
        }
        sysml_conditional_2 = ((int32_t)sysml_state_distance_cm_effective) - sysml_conditional_3;
    }
    else {
        sysml_conditional_2 = 0;
    }
    sysml_value_0 = sysml_conditional_2;
    bool sysml_value_1 = 0;
    sysml_lazy_4 = io_1;
    if (sysml_lazy_4) {
        sysml_lazy_4 = sysml_state_distance_cm_effective < 20.0;
    }
    sysml_value_1 = sysml_lazy_4;
    double sysml_value_2 = 0;
    if (io_1) {
        if (sysml_state_distance_cm_effective <= 10.0) {
            sysml_conditional_6 = 1023.0;
        }
        else {
            if (sysml_state_distance_cm_effective >= 100.0) {
                sysml_conditional_7 = 0.0;
            }
            else {
                if (sysml_state_distance_cm_effective > 10.0) {
                    if ((((double)((100.0 - sysml_state_distance_cm_effective) * 1023.0)) / 90.0) < ((double)((int32_t)(((double)((100.0 - sysml_state_distance_cm_effective) * 1023.0)) / 90.0)))) {
                        sysml_conditional_9 = 1;
                    }
                    else {
                        sysml_conditional_9 = 0;
                    }
                    sysml_conditional_8 = ((int32_t)(((double)((100.0 - sysml_state_distance_cm_effective) * 1023.0)) / 90.0)) - sysml_conditional_9;
                }
                else {
                    sysml_conditional_8 = 1023.0;
                }
                sysml_conditional_7 = sysml_conditional_8;
            }
            sysml_conditional_6 = sysml_conditional_7;
        }
        sysml_conditional_5 = sysml_conditional_6;
    }
    else {
        sysml_conditional_5 = 0.0;
    }
    sysml_value_2 = sysml_conditional_5;
    *io_2 = sysml_value_0;
    *io_3 = sysml_value_1;
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
static double hw_value_io_0 = 0;
static bool hw_value_io_1 = false;
static int32_t hw_value_io_2 = 0;
TM1637Display hw_display_io_2(4, 5);
static int32_t hw_display_previous_io_2 = INT32_MIN;
static bool hw_value_io_3 = false;
static double hw_value_io_4 = 0;
static uint32_t hw_ultrasonic_sampled_0 = 0;
static bool hw_ultrasonic_attempted_0 = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  hw_display_io_2.setBrightness(7, true);
  if (hw_value_io_2 != hw_display_previous_io_2) {
    if (hw_value_io_2 < -999 || hw_value_io_2 > 9999) hw_display_io_2.clear();
    else hw_display_io_2.showNumberDec((int)hw_value_io_2, false, 4, 0);
    hw_display_previous_io_2 = hw_value_io_2;
  }
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_3 ? HIGH : LOW);
  pinMode(9, OUTPUT);
  analogWrite(9, (int)constrain((hw_value_io_4 * 1 + 0), 0.0, 255.0));
  pinMode(8, OUTPUT);
  digitalWrite(8, LOW);
  pinMode(2, INPUT);
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
    unsigned long hw_ultrasonic_pulse_0 = pulseIn(2, HIGH, 30000UL);
    hw_value_io_1 = (hw_ultrasonic_pulse_0 >= 116UL && hw_ultrasonic_pulse_0 <= 23200UL);
    if (hw_value_io_1) hw_value_io_0 = hw_ultrasonic_pulse_0 / 58.0;
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4);
  if (hw_value_io_2 != hw_display_previous_io_2) {
    if (hw_value_io_2 < -999 || hw_value_io_2 > 9999) hw_display_io_2.clear();
    else hw_display_io_2.showNumberDec((int)hw_value_io_2, false, 4, 0);
    hw_display_previous_io_2 = hw_value_io_2;
  }
  digitalWrite(13, hw_value_io_3 ? HIGH : LOW);
  analogWrite(9, (int)constrain((hw_value_io_4 * 1 + 0), 0.0, 255.0));
}

