#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_006\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
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
extern bool sysml_state_prevButton;
/* Global Variable define of  */
extern int32_t sysml_state_manualForcedUntil;
/* Global Variable define of  */
extern int32_t sysml_state_manualRemainingMs;
/* Global Variable define of  */
extern int32_t sysml_state_displayPhase;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, double io_2, bool io_7, bool* io_3, bool* io_4, int32_t* io_5, int32_t* io_6);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_modeAuto = true;
/* Global Variable define of  */
bool sysml_state_prevButton = false;
/* Global Variable define of  */
int32_t sysml_state_manualForcedUntil = 0;
/* Global Variable define of  */
int32_t sysml_state_manualRemainingMs = 0;
/* Global Variable define of  */
int32_t sysml_state_displayPhase = 0;
/* Init function of CompositeActor named "ProgramPage1" */
#line 217 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_006\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 242 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_006\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 45 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_006\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_modeAuto = true;
    sysml_state_prevButton = false;
    sysml_state_manualForcedUntil = 0;
    sysml_state_manualRemainingMs = 0;
    sysml_state_displayPhase = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, double io_2, bool io_7, bool* io_3, bool* io_4, int32_t* io_5, int32_t* io_6) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_conditional_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_conditional_7 = 0;
    bool sysml_lazy_8 = 0;
    int32_t sysml_conditional_9 = 0;
    int32_t sysml_conditional_10 = 0;
    int32_t sysml_conditional_11 = 0;
    int32_t sysml_conditional_12 = 0;
    int32_t sysml_conditional_13 = 0;
    int32_t sysml_conditional_14 = 0;
    sysml_lazy_0 = io_7;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_prevButton;
    }
    if (sysml_lazy_0) {
        if (sysml_state_modeAuto) {
            sysml_state_modeAuto = false;
        }
        else {
            sysml_state_modeAuto = true;
        }
        if ((!sysml_state_modeAuto)) {
            sysml_state_manualRemainingMs = 5000;
            sysml_state_manualForcedUntil = 1;
        }
    }
    sysml_state_prevButton = io_7;
    sysml_lazy_1 = sysml_state_manualForcedUntil != 0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = sysml_state_manualRemainingMs > 0;
    }
    if (sysml_lazy_1) {
        sysml_state_manualRemainingMs = sysml_state_manualRemainingMs;
        sysml_state_manualForcedUntil = sysml_state_manualForcedUntil;
    }
    else {
        sysml_state_manualForcedUntil = 0;
    }
    if (sysml_state_modeAuto) {
        sysml_state_displayPhase = (sysml_state_displayPhase + 1);
    }
    else {
        sysml_state_displayPhase = (sysml_state_displayPhase + 1);
    }
    bool sysml_value_0 = 0;
    if (sysml_state_modeAuto) {
        sysml_lazy_3 = io_0 < 512.0;
        if (sysml_lazy_3) {
            sysml_lazy_3 = io_1 >= 512.0;
        }
        sysml_lazy_4 = sysml_lazy_3;
        if (sysml_lazy_4) {
            sysml_lazy_4 = io_2 >= 512.0;
        }
        sysml_conditional_2 = sysml_lazy_4;
    }
    else {
        sysml_lazy_5 = sysml_state_manualForcedUntil != 0;
        if (sysml_lazy_5) {
            sysml_lazy_5 = sysml_state_manualRemainingMs > 0;
        }
        sysml_conditional_2 = sysml_lazy_5;
    }
    sysml_value_0 = sysml_conditional_2;
    bool sysml_value_1 = 0;
    sysml_lazy_6 = sysml_state_modeAuto;
    if (sysml_lazy_6) {
        sysml_lazy_6 = io_2 < 512.0;
    }
    if (sysml_lazy_6) {
        sysml_conditional_7 = true;
    }
    else {
        sysml_conditional_7 = false;
    }
    sysml_value_1 = sysml_conditional_7;
    int32_t sysml_value_2 = 0;
    sysml_lazy_8 = sysml_state_modeAuto;
    if (sysml_lazy_8) {
        sysml_lazy_8 = io_2 < 512.0;
    }
    if (sysml_lazy_8) {
        sysml_conditional_9 = 1000;
    }
    else {
        sysml_conditional_9 = 0;
    }
    sysml_value_2 = sysml_conditional_9;
    int32_t sysml_value_3 = 0;
    if (sysml_state_displayPhase == 0) {
        if (io_0 < ((double)((int32_t)io_0))) {
            sysml_conditional_11 = 1;
        }
        else {
            sysml_conditional_11 = 0;
        }
        sysml_conditional_10 = ((int32_t)io_0) - sysml_conditional_11;
    }
    else {
        if (sysml_state_displayPhase == 1) {
            if (io_2 < ((double)((int32_t)io_2))) {
                sysml_conditional_13 = 1;
            }
            else {
                sysml_conditional_13 = 0;
            }
            sysml_conditional_12 = ((int32_t)io_2) - sysml_conditional_13;
        }
        else {
            if (sysml_state_modeAuto) {
                sysml_conditional_14 = 0;
            }
            else {
                sysml_conditional_14 = 1;
            }
            sysml_conditional_12 = sysml_conditional_14;
        }
        sysml_conditional_10 = sysml_conditional_12;
    }
    sysml_value_3 = sysml_conditional_10;
    *io_3 = sysml_value_0;
    *io_4 = sysml_value_1;
    *io_5 = sysml_value_2;
    *io_6 = sysml_value_3;
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
static bool hw_value_io_3 = false;
static bool hw_value_io_4 = false;
static int32_t hw_value_io_5 = 0;
static int32_t hw_tone_io_5 = -1;
static int32_t hw_value_io_6 = 0;
TM1637Display hw_display_io_6(4, 5);
static int32_t hw_display_previous_io_6 = INT32_MIN;
static bool hw_value_io_7 = false;
static uint32_t hw_sampled_io_7 = 0;
static bool hw_attempted_io_7 = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(10, OUTPUT);
  digitalWrite(10, hw_value_io_3 ? HIGH : LOW);
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_4 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  int32_t hw_tone_command_io_5 = (hw_value_io_5 >= 31 && hw_value_io_5 <= 65535) ? hw_value_io_5 : 0;
  if (hw_tone_command_io_5 != hw_tone_io_5) {
    if (hw_tone_command_io_5 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_5);
    hw_tone_io_5 = hw_tone_command_io_5;
  }
  hw_display_io_6.setBrightness(7, true);
  if (hw_value_io_6 != hw_display_previous_io_6) {
    if (hw_value_io_6 < -999 || hw_value_io_6 > 9999) hw_display_io_6.clear();
    else hw_display_io_6.showNumberDec((int)hw_value_io_6, false, 4, 0);
    hw_display_previous_io_6 = hw_value_io_6;
  }
  pinMode(2, INPUT);
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
    hw_value_io_2 = (analogRead(A2) * 1.0 + 0.0);
  }
  if (!hw_attempted_io_7 || (uint32_t)(hw_now - hw_sampled_io_7) >= 5UL) {
    hw_attempted_io_7 = true; hw_sampled_io_7 = hw_now;
    hw_value_io_7 = (digitalRead(2) == HIGH);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, hw_value_io_7, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6);
  digitalWrite(10, hw_value_io_3 ? HIGH : LOW);
  digitalWrite(13, hw_value_io_4 ? HIGH : LOW);
  int32_t hw_tone_command_io_5 = (hw_value_io_5 >= 31 && hw_value_io_5 <= 65535) ? hw_value_io_5 : 0;
  if (hw_tone_command_io_5 != hw_tone_io_5) {
    if (hw_tone_command_io_5 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_5);
    hw_tone_io_5 = hw_tone_command_io_5;
  }
  if (hw_value_io_6 != hw_display_previous_io_6) {
    if (hw_value_io_6 < -999 || hw_value_io_6 > 9999) hw_display_io_6.clear();
    else hw_display_io_6.showNumberDec((int)hw_value_io_6, false, 4, 0);
    hw_display_previous_io_6 = hw_value_io_6;
  }
}

