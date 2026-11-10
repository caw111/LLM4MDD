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
extern bool sysml_state_startupBlank;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_1, bool io_2, bool* io_0, int32_t* io_3);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_startupBlank = true;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_startupBlank = true;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_1, bool io_2, bool* io_0, int32_t* io_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    int32_t sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    int32_t sysml_conditional_7 = 0;
    int32_t sysml_conditional_8 = 0;
    int32_t sysml_conditional_9 = 0;
    sysml_lazy_0 = sysml_state_startupBlank;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_2;
    }
    if (sysml_lazy_0) {
        sysml_state_startupBlank = false;
    }
    bool sysml_value_0 = 0;
    sysml_lazy_1 = !sysml_state_startupBlank;
    if (sysml_lazy_1) {
        sysml_lazy_1 = io_2;
    }
    sysml_lazy_2 = sysml_lazy_1;
    if (sysml_lazy_2) {
        sysml_lazy_2 = io_1 < 10.0;
    }
    if (sysml_lazy_2) {
        sysml_conditional_3 = true;
    }
    else {
        sysml_conditional_3 = false;
    }
    sysml_value_0 = sysml_conditional_3;
    int32_t sysml_value_1 = 0;
    sysml_lazy_4 = sysml_state_startupBlank;
    if (!sysml_lazy_4) {
        sysml_lazy_4 = !io_2;
    }
    if (sysml_lazy_4) {
        sysml_conditional_5 = 10000;
    }
    else {
        sysml_lazy_6 = io_1 < 2.0;
        if (!sysml_lazy_6) {
            sysml_lazy_6 = io_1 > 400.0;
        }
        if (sysml_lazy_6) {
            sysml_conditional_7 = 10000;
        }
        else {
            if (io_2) {
                if (io_1 < ((double)((int32_t)io_1))) {
                    sysml_conditional_9 = 1;
                }
                else {
                    sysml_conditional_9 = 0;
                }
                sysml_conditional_8 = ((int32_t)io_1) - sysml_conditional_9;
            }
            else {
                sysml_conditional_8 = 10000;
            }
            sysml_conditional_7 = sysml_conditional_8;
        }
        sysml_conditional_5 = sysml_conditional_7;
    }
    sysml_value_1 = sysml_conditional_5;
    *io_0 = sysml_value_0;
    *io_3 = sysml_value_1;
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
static double hw_value_io_1 = 0;
static bool hw_value_io_2 = false;
static int32_t hw_value_io_3 = 10000;
TM1637Display hw_display_io_3(4, 5);
static int32_t hw_display_previous_io_3 = INT32_MIN;
static uint32_t hw_ultrasonic_sampled_0 = 0;
static bool hw_ultrasonic_attempted_0 = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_0 ? HIGH : LOW);
  hw_display_io_3.setBrightness(7, true);
  if (hw_value_io_3 != hw_display_previous_io_3) {
    if (hw_value_io_3 < -999 || hw_value_io_3 > 9999) hw_display_io_3.clear();
    else hw_display_io_3.showNumberDec((int)hw_value_io_3, false, 4, 0);
    hw_display_previous_io_3 = hw_value_io_3;
  }
  pinMode(8, OUTPUT);
  digitalWrite(8, LOW);
  pinMode(9, INPUT);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  uint32_t hw_ultrasonic_now_0 = (uint32_t)millis();
  if (!hw_ultrasonic_attempted_0 || (uint32_t)(hw_ultrasonic_now_0 - hw_ultrasonic_sampled_0) >= 60UL) {
    hw_ultrasonic_attempted_0 = true; hw_ultrasonic_sampled_0 = hw_ultrasonic_now_0;
    digitalWrite(8, LOW); delayMicroseconds(2);
    digitalWrite(8, HIGH); delayMicroseconds(10); digitalWrite(8, LOW);
    unsigned long hw_ultrasonic_pulse_0 = pulseIn(9, HIGH, 25000UL);
    hw_value_io_2 = (hw_ultrasonic_pulse_0 >= 116UL && hw_ultrasonic_pulse_0 <= 23200UL);
    if (hw_value_io_2) hw_value_io_1 = hw_ultrasonic_pulse_0 / 58.0;
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_1, hw_value_io_2, &hw_value_io_0, &hw_value_io_3);
  digitalWrite(13, hw_value_io_0 ? HIGH : LOW);
  if (hw_value_io_3 != hw_display_previous_io_3) {
    if (hw_value_io_3 < -999 || hw_value_io_3 > 9999) hw_display_io_3.clear();
    else hw_display_io_3.showNumberDec((int)hw_value_io_3, false, 4, 0);
    hw_display_previous_io_3 = hw_value_io_3;
  }
}
