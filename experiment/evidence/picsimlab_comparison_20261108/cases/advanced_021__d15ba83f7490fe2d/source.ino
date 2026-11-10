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
extern bool sysml_state_previousPressed;
/* Global Variable define of  */
extern int32_t sysml_state_displayMode;
/* Global Variable define of  */
extern bool sysml_state_heaterEnabled;
/* Global Variable define of  */
extern bool sysml_state_aeratorEnabled;
/* Global Variable define of  */
extern bool sysml_state_alarmEnabled;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, double io_2, bool io_6, double* io_3, double* io_4, double* io_5, int32_t* io_7, int32_t* io_8);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
int32_t sysml_state_displayMode = 0;
/* Global Variable define of  */
bool sysml_state_heaterEnabled = true;
/* Global Variable define of  */
bool sysml_state_aeratorEnabled = true;
/* Global Variable define of  */
bool sysml_state_alarmEnabled = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousPressed = false;
    sysml_state_displayMode = 0;
    sysml_state_heaterEnabled = true;
    sysml_state_aeratorEnabled = true;
    sysml_state_alarmEnabled = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, double io_2, bool io_6, double* io_3, double* io_4, double* io_5, int32_t* io_7, int32_t* io_8) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int32_t sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_conditional_5 = 0;
    bool sysml_conditional_6 = 0;
    double sysml_conditional_7 = 0;
    double sysml_conditional_8 = 0;
    bool sysml_lazy_9 = 0;
    double sysml_conditional_10 = 0;
    double sysml_conditional_11 = 0;
    int32_t sysml_conditional_12 = 0;
    int32_t sysml_conditional_13 = 0;
    int32_t sysml_conditional_14 = 0;
    int32_t sysml_conditional_15 = 0;
    int32_t sysml_conditional_16 = 0;
    int32_t sysml_conditional_17 = 0;
    int32_t sysml_conditional_18 = 0;
    sysml_lazy_0 = io_6;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        if (sysml_state_displayMode == 2) {
            sysml_conditional_1 = 0;
        }
        else {
            sysml_conditional_1 = sysml_state_displayMode + 1;
        }
        sysml_state_displayMode = sysml_conditional_1;
    }
    sysml_state_previousPressed = io_6;
    sysml_lazy_2 = sysml_state_alarmEnabled;
    if (sysml_lazy_2) {
        sysml_lazy_3 = io_2 <= 0.0;
        if (!sysml_lazy_3) {
            sysml_lazy_3 = io_2 < 10.0;
        }
        sysml_lazy_2 = sysml_lazy_3;
    }
    sysml_state_heaterEnabled = !sysml_lazy_2;
    sysml_lazy_4 = io_2 > 0.0;
    if (sysml_lazy_4) {
        sysml_lazy_4 = io_2 >= 10.0;
    }
    if (sysml_lazy_4) {
        sysml_conditional_5 = true;
    }
    else {
        sysml_conditional_5 = false;
    }
    sysml_state_aeratorEnabled = sysml_conditional_5;
    if (io_2 < 10.0) {
        sysml_conditional_6 = true;
    }
    else {
        sysml_conditional_6 = false;
    }
    sysml_state_alarmEnabled = sysml_conditional_6;
    double sysml_value_0 = 0;
    if (sysml_state_alarmEnabled) {
        sysml_conditional_7 = 0.0;
    }
    else {
        if (io_0 < 30.0) {
            sysml_conditional_8 = 255.0;
        }
        else {
            sysml_conditional_8 = 0.0;
        }
        sysml_conditional_7 = sysml_conditional_8;
    }
    sysml_value_0 = sysml_conditional_7;
    double sysml_value_1 = 0;
    sysml_lazy_9 = !sysml_state_alarmEnabled;
    if (sysml_lazy_9) {
        sysml_lazy_9 = io_1 < 500.0;
    }
    if (sysml_lazy_9) {
        sysml_conditional_10 = 255.0;
    }
    else {
        sysml_conditional_10 = 0.0;
    }
    sysml_value_1 = sysml_conditional_10;
    double sysml_value_2 = 0;
    if (sysml_state_aeratorEnabled) {
        sysml_conditional_11 = 255.0;
    }
    else {
        sysml_conditional_11 = 0.0;
    }
    sysml_value_2 = sysml_conditional_11;
    int32_t sysml_value_3 = 0;
    if (sysml_state_alarmEnabled) {
        sysml_conditional_12 = 2000;
    }
    else {
        sysml_conditional_12 = 0;
    }
    sysml_value_3 = sysml_conditional_12;
    int32_t sysml_value_4 = 0;
    if (sysml_state_displayMode == 0) {
        if (io_0 <= 0.0) {
            sysml_conditional_14 = 0;
        }
        else {
            if (io_0 < ((double)((int32_t)io_0))) {
                sysml_conditional_15 = 1;
            }
            else {
                sysml_conditional_15 = 0;
            }
            sysml_conditional_14 = ((int32_t)io_0) - sysml_conditional_15;
        }
        sysml_conditional_13 = sysml_conditional_14;
    }
    else {
        if (sysml_state_displayMode == 1) {
            if (io_1 < ((double)((int32_t)io_1))) {
                sysml_conditional_17 = 1;
            }
            else {
                sysml_conditional_17 = 0;
            }
            sysml_conditional_16 = ((int32_t)io_1) - sysml_conditional_17;
        }
        else {
            if (io_2 < ((double)((int32_t)io_2))) {
                sysml_conditional_18 = 1;
            }
            else {
                sysml_conditional_18 = 0;
            }
            sysml_conditional_16 = ((int32_t)io_2) - sysml_conditional_18;
        }
        sysml_conditional_13 = sysml_conditional_16;
    }
    sysml_value_4 = sysml_conditional_13;
    *io_3 = sysml_value_0;
    *io_4 = sysml_value_1;
    *io_5 = sysml_value_2;
    *io_7 = sysml_value_3;
    *io_8 = sysml_value_4;
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
static double hw_value_io_1 = 0;
static uint32_t hw_sampled_io_1 = 0;
static bool hw_attempted_io_1 = false;
static double hw_value_io_2 = 0;
static uint32_t hw_sampled_io_2 = 0;
static bool hw_attempted_io_2 = false;
static double hw_value_io_3 = 0;
static double hw_value_io_4 = 0;
static double hw_value_io_5 = 0;
static bool hw_value_io_6 = false;
static uint32_t hw_sampled_io_6 = 0;
static bool hw_attempted_io_6 = false;
static int32_t hw_value_io_7 = 0;
static int32_t hw_tone_io_7 = -1;
static int32_t hw_value_io_8 = 0;
TM1637Display hw_display_io_8(4, 3);
static int32_t hw_display_previous_io_8 = INT32_MIN;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(A1, INPUT);
  pinMode(A0, INPUT);
  pinMode(A2, INPUT);
  pinMode(10, OUTPUT);
  analogWrite(10, (int)constrain((hw_value_io_3 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(9, OUTPUT);
  analogWrite(9, (int)constrain((hw_value_io_4 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(5, OUTPUT);
  analogWrite(5, (int)constrain((hw_value_io_5 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(2, INPUT);
  pinMode(6, OUTPUT);
  int32_t hw_tone_command_io_7 = (hw_value_io_7 >= 31 && hw_value_io_7 <= 65535) ? hw_value_io_7 : 0;
  if (hw_tone_command_io_7 != hw_tone_io_7) {
    if (hw_tone_command_io_7 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_7);
    hw_tone_io_7 = hw_tone_command_io_7;
  }
  hw_display_io_8.setBrightness(7, true);
  if (hw_value_io_8 != hw_display_previous_io_8) {
    if (hw_value_io_8 < -999 || hw_value_io_8 > 9999) hw_display_io_8.clear();
    else hw_display_io_8.showNumberDec((int)hw_value_io_8, false, 4, 0);
    hw_display_previous_io_8 = hw_value_io_8;
  }
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 10UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (analogRead(A1) * 1.0 + 0.0);
  }
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 10UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = (analogRead(A0) * 1.0 + 0.0);
  }
  if (!hw_attempted_io_2 || (uint32_t)(hw_now - hw_sampled_io_2) >= 10UL) {
    hw_attempted_io_2 = true; hw_sampled_io_2 = hw_now;
    hw_value_io_2 = (analogRead(A2) * 1.0 + 0.0);
  }
  if (!hw_attempted_io_6 || (uint32_t)(hw_now - hw_sampled_io_6) >= 10UL) {
    hw_attempted_io_6 = true; hw_sampled_io_6 = hw_now;
    hw_value_io_6 = (digitalRead(2) == HIGH);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, hw_value_io_6, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_7, &hw_value_io_8);
  analogWrite(10, (int)constrain((hw_value_io_3 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(9, (int)constrain((hw_value_io_4 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(5, (int)constrain((hw_value_io_5 * 1.0 + 0.0), 0.0, 255.0));
  int32_t hw_tone_command_io_7 = (hw_value_io_7 >= 31 && hw_value_io_7 <= 65535) ? hw_value_io_7 : 0;
  if (hw_tone_command_io_7 != hw_tone_io_7) {
    if (hw_tone_command_io_7 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_7);
    hw_tone_io_7 = hw_tone_command_io_7;
  }
  if (hw_value_io_8 != hw_display_previous_io_8) {
    if (hw_value_io_8 < -999 || hw_value_io_8 > 9999) hw_display_io_8.clear();
    else hw_display_io_8.showNumberDec((int)hw_value_io_8, false, 4, 0);
    hw_display_previous_io_8 = hw_value_io_8;
  }
}
