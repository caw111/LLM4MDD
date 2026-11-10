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
extern bool sysml_state_modeAuto;
/* Global Variable define of  */
extern bool sysml_state_previousPressed;
/* Global Variable define of  */
extern double sysml_state_manualThresholdCm;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, double io_3, bool io_4, int32_t* io_2, int32_t* io_5);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_modeAuto = true;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
double sysml_state_manualThresholdCm = 15.0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_modeAuto = true;
    sysml_state_previousPressed = false;
    sysml_state_manualThresholdCm = 15.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, double io_3, bool io_4, int32_t* io_2, int32_t* io_5) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    double sysml_conditional_1 = 0;
    int32_t sysml_conditional_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    int32_t sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    int32_t sysml_conditional_8 = 0;
    int32_t sysml_conditional_9 = 0;
    int32_t sysml_conditional_10 = 0;
    int32_t sysml_conditional_11 = 0;
    sysml_lazy_0 = io_1;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        sysml_state_modeAuto = (!sysml_state_modeAuto);
        if (io_0 <= 15.0) {
            sysml_conditional_1 = 15.0;
        }
        else {
            if ((((double)(io_0 * 400.0)) / 1023.0) < ((double)((int32_t)(((double)(io_0 * 400.0)) / 1023.0)))) {
                sysml_conditional_2 = 1;
            }
            else {
                sysml_conditional_2 = 0;
            }
            sysml_conditional_1 = ((int32_t)(((double)(io_0 * 400.0)) / 1023.0)) - sysml_conditional_2;
        }
        sysml_state_manualThresholdCm = sysml_conditional_1;
    }
    sysml_state_previousPressed = io_1;
    int32_t sysml_value_0 = 0;
    sysml_lazy_3 = sysml_state_modeAuto;
    if (sysml_lazy_3) {
        sysml_lazy_3 = io_4;
    }
    sysml_lazy_4 = sysml_lazy_3;
    if (sysml_lazy_4) {
        sysml_lazy_4 = io_3 < 15.0;
    }
    if (sysml_lazy_4) {
        sysml_conditional_5 = 1000;
    }
    else {
        sysml_lazy_6 = !sysml_state_modeAuto;
        if (sysml_lazy_6) {
            sysml_lazy_6 = io_4;
        }
        sysml_lazy_7 = sysml_lazy_6;
        if (sysml_lazy_7) {
            sysml_lazy_7 = io_3 < sysml_state_manualThresholdCm;
        }
        if (sysml_lazy_7) {
            sysml_conditional_8 = 1000;
        }
        else {
            sysml_conditional_8 = 0;
        }
        sysml_conditional_5 = sysml_conditional_8;
    }
    sysml_value_0 = sysml_conditional_5;
    int32_t sysml_value_1 = 0;
    if (sysml_state_modeAuto) {
        sysml_conditional_9 = 15;
    }
    else {
        if (sysml_state_manualThresholdCm <= 0.0) {
            sysml_conditional_10 = 0;
        }
        else {
            if (sysml_state_manualThresholdCm < ((double)((int32_t)sysml_state_manualThresholdCm))) {
                sysml_conditional_11 = 1;
            }
            else {
                sysml_conditional_11 = 0;
            }
            sysml_conditional_10 = ((int32_t)sysml_state_manualThresholdCm) - sysml_conditional_11;
        }
        sysml_conditional_9 = sysml_conditional_10;
    }
    sysml_value_1 = sysml_conditional_9;
    *io_2 = sysml_value_0;
    *io_5 = sysml_value_1;
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
static uint32_t hw_sampled_io_0 = 0;
static bool hw_attempted_io_0 = false;
static bool hw_value_io_1 = false;
static uint32_t hw_sampled_io_1 = 0;
static bool hw_attempted_io_1 = false;
static int32_t hw_value_io_2 = 0;
static int32_t hw_tone_io_2 = -1;
static double hw_value_io_3 = 0;
static bool hw_value_io_4 = false;
static int32_t hw_value_io_5 = 0;
TM1637Display hw_display_io_5(4, 5);
static int32_t hw_display_previous_io_5 = INT32_MIN;
static uint32_t hw_ultrasonic_sampled_0 = 0;
static bool hw_ultrasonic_attempted_0 = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(A0, INPUT);
  pinMode(2, INPUT_PULLUP);
  pinMode(6, OUTPUT);
  int32_t hw_tone_command_io_2 = (hw_value_io_2 >= 31 && hw_value_io_2 <= 65535) ? hw_value_io_2 : 0;
  if (hw_tone_command_io_2 != hw_tone_io_2) {
    if (hw_tone_command_io_2 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_2);
    hw_tone_io_2 = hw_tone_command_io_2;
  }
  hw_display_io_5.setBrightness(7, true);
  if (hw_value_io_5 != hw_display_previous_io_5) {
    if (hw_value_io_5 < -999 || hw_value_io_5 > 9999) hw_display_io_5.clear();
    else hw_display_io_5.showNumberDec((int)hw_value_io_5, false, 4, 0);
    hw_display_previous_io_5 = hw_value_io_5;
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
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 10UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (analogRead(A0) * 1.0 + 0.0);
  }
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 10UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = (digitalRead(2) == LOW);
  }
  uint32_t hw_ultrasonic_now_0 = (uint32_t)millis();
  if (!hw_ultrasonic_attempted_0 || (uint32_t)(hw_ultrasonic_now_0 - hw_ultrasonic_sampled_0) >= 60UL) {
    hw_ultrasonic_attempted_0 = true; hw_ultrasonic_sampled_0 = hw_ultrasonic_now_0;
    digitalWrite(8, LOW); delayMicroseconds(2);
    digitalWrite(8, HIGH); delayMicroseconds(10); digitalWrite(8, LOW);
    unsigned long hw_ultrasonic_pulse_0 = pulseIn(9, HIGH, 30000UL);
    hw_value_io_4 = (hw_ultrasonic_pulse_0 >= 116UL && hw_ultrasonic_pulse_0 <= 23200UL);
    if (hw_value_io_4) hw_value_io_3 = hw_ultrasonic_pulse_0 / 58.0;
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_3, hw_value_io_4, &hw_value_io_2, &hw_value_io_5);
  int32_t hw_tone_command_io_2 = (hw_value_io_2 >= 31 && hw_value_io_2 <= 65535) ? hw_value_io_2 : 0;
  if (hw_tone_command_io_2 != hw_tone_io_2) {
    if (hw_tone_command_io_2 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_2);
    hw_tone_io_2 = hw_tone_command_io_2;
  }
  if (hw_value_io_5 != hw_display_previous_io_5) {
    if (hw_value_io_5 < -999 || hw_value_io_5 > 9999) hw_display_io_5.clear();
    else hw_display_io_5.showNumberDec((int)hw_value_io_5, false, 4, 0);
    hw_display_previous_io_5 = hw_value_io_5;
  }
}
