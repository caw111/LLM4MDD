#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_completion_20261107\\run5_remaining\\cases\\advanced_016\\attempt_1\\arduino\\sketch\\sketch.ino"
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
extern double sysml_state_displayCycleMs;
/* Global Variable define of  */
extern bool sysml_state_previousVibration;
/* Global Variable define of  */
extern bool sysml_state_faultEntry;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, bool io_2, int32_t io_7, double* io_3, int32_t* io_4, bool* io_5, int32_t* io_6, bool* serial_0_trigger, int32_t* serial_0_value_0);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_displayCycleMs = 0.0;
/* Global Variable define of  */
bool sysml_state_previousVibration = false;
/* Global Variable define of  */
bool sysml_state_faultEntry = false;
/* Init function of CompositeActor named "ProgramPage1" */
#line 190 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_completion_20261107\\run5_remaining\\cases\\advanced_016\\attempt_1\\arduino\\sketch\\sketch.ino"
void setup();
#line 215 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_completion_20261107\\run5_remaining\\cases\\advanced_016\\attempt_1\\arduino\\sketch\\sketch.ino"
void loop();
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_completion_20261107\\run5_remaining\\cases\\advanced_016\\attempt_1\\arduino\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_displayCycleMs = 0.0;
    sysml_state_previousVibration = false;
    sysml_state_faultEntry = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, bool io_2, int32_t io_7, double* io_3, int32_t* io_4, bool* io_5, int32_t* io_6, bool* serial_0_trigger, int32_t* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int32_t sysml_conditional_1 = 0;
    int32_t sysml_arithmetic_2 = 0;
    int32_t sysml_conditional_3 = 0;
    int32_t sysml_conditional_4 = 0;
    int32_t sysml_conditional_5 = 0;
    int32_t sysml_conditional_6 = 0;
    int32_t sysml_conditional_7 = 0;
    double sysml_conditional_8 = 0;
    int32_t sysml_conditional_9 = 0;
    int32_t sysml_conditional_10 = 0;
    int32_t sysml_conditional_11 = 0;
    int32_t sysml_conditional_12 = 0;
    sysml_lazy_0 = io_2;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousVibration;
    }
    sysml_state_faultEntry = sysml_lazy_0;
    sysml_state_displayCycleMs = (sysml_state_displayCycleMs + io_7);
    if ((((double)sysml_state_displayCycleMs) / 6000.0) < ((double)((int32_t)(((double)sysml_state_displayCycleMs) / 6000.0)))) {
        sysml_conditional_1 = 1;
    }
    else {
        sysml_conditional_1 = 0;
    }
    sysml_arithmetic_2 = ((int32_t)(((double)sysml_state_displayCycleMs) / 6000.0)) - sysml_conditional_1;
    sysml_state_displayCycleMs = sysml_state_displayCycleMs - (6000.0 * sysml_arithmetic_2);
    sysml_state_previousVibration = io_2;
    double sysml_value_0 = 0;
    sysml_value_0 = ((double)((io_0 * 10.0)) / 1023.0);
    double sysml_value_1 = 0;
    sysml_value_1 = ((double)((io_1 * 500.0)) / 1023.0);
    int32_t sysml_value_2 = 0;
    if (io_2) {
        sysml_conditional_3 = 1000;
    }
    else {
        sysml_conditional_3 = 0;
    }
    sysml_value_2 = sysml_conditional_3;
    bool sysml_value_3 = 0;
    sysml_value_3 = io_2;
    bool sysml_value_4 = 0;
    sysml_value_4 = sysml_state_faultEntry;
    int32_t sysml_value_5 = 0;
    if (io_2) {
        sysml_conditional_4 = 3;
    }
    else {
        sysml_conditional_4 = 0;
    }
    sysml_value_5 = sysml_conditional_4;
    int32_t sysml_value_6 = 0;
    if (io_2) {
        sysml_conditional_5 = 3;
    }
    else {
        if (sysml_value_1 > 60.0) {
            sysml_conditional_6 = 2;
        }
        else {
            if (sysml_value_0 > 7.0) {
                sysml_conditional_7 = 1;
            }
            else {
                sysml_conditional_7 = 0;
            }
            sysml_conditional_6 = sysml_conditional_7;
        }
        sysml_conditional_5 = sysml_conditional_6;
    }
    sysml_value_6 = sysml_conditional_5;
    double sysml_value_7 = 0;
    if (sysml_value_1 > 60.0) {
        sysml_conditional_8 = 255.0;
    }
    else {
        sysml_conditional_8 = 0.0;
    }
    sysml_value_7 = sysml_conditional_8;
    int32_t sysml_value_8 = 0;
    if (sysml_state_displayCycleMs < 2000.0) {
        if (sysml_value_0 < ((double)((int32_t)sysml_value_0))) {
            sysml_conditional_10 = 1;
        }
        else {
            sysml_conditional_10 = 0;
        }
        sysml_conditional_9 = ((int32_t)sysml_value_0) - sysml_conditional_10;
    }
    else {
        if (sysml_state_displayCycleMs < 4000.0) {
            if (sysml_value_1 < ((double)((int32_t)sysml_value_1))) {
                sysml_conditional_12 = 1;
            }
            else {
                sysml_conditional_12 = 0;
            }
            sysml_conditional_11 = ((int32_t)sysml_value_1) - sysml_conditional_12;
        }
        else {
            sysml_conditional_11 = sysml_value_6;
        }
        sysml_conditional_9 = sysml_conditional_11;
    }
    sysml_value_8 = sysml_conditional_9;
    *io_3 = sysml_value_7;
    *io_4 = sysml_value_2;
    *io_5 = sysml_value_3;
    *io_6 = sysml_value_8;
    *serial_0_trigger = sysml_value_4;
    *serial_0_value_0 = sysml_value_5;
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
static bool hw_value_io_2 = false;
static uint32_t hw_sampled_io_2 = 0;
static bool hw_attempted_io_2 = false;
static double hw_value_io_3 = 0;
static int32_t hw_value_io_4 = 0;
static int32_t hw_tone_io_4 = -1;
static bool hw_value_io_5 = false;
static int32_t hw_value_io_6 = 0;
TM1637Display hw_display_io_6(4, 3);
static int32_t hw_display_previous_io_6 = INT32_MIN;
static int32_t hw_value_io_7 = 0;
static bool hw_value_serial_0_trigger = false;
static int32_t hw_value_serial_0_value_0 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(2, INPUT_PULLUP);
  pinMode(5, OUTPUT);
  analogWrite(5, (int)constrain((hw_value_io_3 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(6, OUTPUT);
  int32_t hw_tone_command_io_4 = (hw_value_io_4 >= 31 && hw_value_io_4 <= 65535) ? hw_value_io_4 : 0;
  if (hw_tone_command_io_4 != hw_tone_io_4) {
    if (hw_tone_command_io_4 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_4);
    hw_tone_io_4 = hw_tone_command_io_4;
  }
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_5 ? HIGH : LOW);
  hw_display_io_6.setBrightness(7, true);
  if (hw_value_io_6 != hw_display_previous_io_6) {
    if (hw_value_io_6 < -999 || hw_value_io_6 > 9999) hw_display_io_6.clear();
    else hw_display_io_6.showNumberDec((int)hw_value_io_6, false, 4, 0);
    hw_display_previous_io_6 = hw_value_io_6;
  }
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 5UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (analogRead(A0) * 1.0 + 0.0);
  }
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 5UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = (analogRead(A1) * 1.0 + 0.0);
  }
  if (!hw_attempted_io_2 || (uint32_t)(hw_now - hw_sampled_io_2) >= 5UL) {
    hw_attempted_io_2 = true; hw_sampled_io_2 = hw_now;
    hw_value_io_2 = (digitalRead(2) == LOW);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_7 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, hw_value_io_7, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0);
  analogWrite(5, (int)constrain((hw_value_io_3 * 1.0 + 0.0), 0.0, 255.0));
  int32_t hw_tone_command_io_4 = (hw_value_io_4 >= 31 && hw_value_io_4 <= 65535) ? hw_value_io_4 : 0;
  if (hw_tone_command_io_4 != hw_tone_io_4) {
    if (hw_tone_command_io_4 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_4);
    hw_tone_io_4 = hw_tone_command_io_4;
  }
  digitalWrite(13, hw_value_io_5 ? HIGH : LOW);
  if (hw_value_io_6 != hw_display_previous_io_6) {
    if (hw_value_io_6 < -999 || hw_value_io_6 > 9999) hw_display_io_6.clear();
    else hw_display_io_6.showNumberDec((int)hw_value_io_6, false, 4, 0);
    hw_display_previous_io_6 = hw_value_io_6;
  }
  if (hw_value_serial_0_trigger) { Serial.print(""); Serial.println(hw_value_serial_0_value_0); }
}

