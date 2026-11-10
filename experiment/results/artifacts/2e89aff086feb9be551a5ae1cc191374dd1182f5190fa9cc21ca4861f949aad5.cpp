#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_022\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
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
extern int32_t sysml_state_mode;
/* Global Variable define of  */
extern int32_t sysml_state_brightness;
/* Global Variable define of  */
extern int32_t sysml_state_breathSpeed;
/* Global Variable define of  */
extern int32_t sysml_state_accumMs;
/* Global Variable define of  */
extern bool sysml_state_prevModeBtnPressed;
/* Global Variable define of  */
extern bool sysml_state_prevParamBtnPressed;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, int32_t io_5, double* io_2, int32_t* io_3, bool* io_4);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_mode = 0;
/* Global Variable define of  */
int32_t sysml_state_brightness = 255;
/* Global Variable define of  */
int32_t sysml_state_breathSpeed = 2000;
/* Global Variable define of  */
int32_t sysml_state_accumMs = 0;
/* Global Variable define of  */
bool sysml_state_prevModeBtnPressed = false;
/* Global Variable define of  */
bool sysml_state_prevParamBtnPressed = false;
/* Init function of CompositeActor named "ProgramPage1" */
#line 226 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_022\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 242 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_022\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 49 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_022\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_mode = 0;
    sysml_state_brightness = 255;
    sysml_state_breathSpeed = 2000;
    sysml_state_accumMs = 0;
    sysml_state_prevModeBtnPressed = false;
    sysml_state_prevParamBtnPressed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, int32_t io_5, double* io_2, int32_t* io_3, bool* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int32_t sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    int32_t sysml_conditional_3 = 0;
    int32_t sysml_conditional_4 = 0;
    double sysml_conditional_5 = 0;
    double sysml_conditional_6 = 0;
    bool sysml_lazy_7 = 0;
    double sysml_conditional_8 = 0;
    double sysml_conditional_9 = 0;
    double sysml_conditional_10 = 0;
    double sysml_conditional_11 = 0;
    double sysml_arithmetic_12 = 0;
    double sysml_arithmetic_13 = 0;
    double sysml_conditional_14 = 0;
    double sysml_arithmetic_15 = 0;
    double sysml_arithmetic_16 = 0;
    int32_t sysml_conditional_17 = 0;
    int32_t sysml_conditional_18 = 0;
    bool sysml_conditional_19 = 0;
    sysml_state_accumMs = (sysml_state_accumMs + io_5);
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_prevModeBtnPressed;
    }
    if (sysml_lazy_0) {
        if (sysml_state_mode == 2) {
            sysml_conditional_1 = 0;
        }
        else {
            sysml_conditional_1 = sysml_state_mode + 1;
        }
        sysml_state_mode = sysml_conditional_1;
    }
    sysml_lazy_2 = io_1;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_prevParamBtnPressed;
    }
    if (sysml_lazy_2) {
        if ((sysml_state_mode == 1)) {
            if (sysml_state_brightness == 255) {
                sysml_conditional_3 = 16;
            }
            else {
                sysml_conditional_3 = sysml_state_brightness + 16;
            }
            sysml_state_brightness = sysml_conditional_3;
        }
        if ((sysml_state_mode == 2)) {
            if (sysml_state_breathSpeed == 5000) {
                sysml_conditional_4 = 500;
            }
            else {
                sysml_conditional_4 = sysml_state_breathSpeed + 500;
            }
            sysml_state_breathSpeed = sysml_conditional_4;
        }
    }
    sysml_state_prevModeBtnPressed = io_0;
    sysml_state_prevParamBtnPressed = io_1;
    if ((sysml_state_mode != 2)) {
        sysml_state_accumMs = 0;
    }
    double sysml_value_0 = 0;
    if (sysml_state_mode == 0) {
        sysml_conditional_5 = 0.0;
    }
    else {
        if (sysml_state_mode == 1) {
            sysml_lazy_7 = sysml_state_brightness >= 0;
            if (sysml_lazy_7) {
                sysml_lazy_7 = sysml_state_brightness <= 255;
            }
            if (sysml_lazy_7) {
                sysml_conditional_8 = sysml_state_brightness * 1.0;
            }
            else {
                sysml_conditional_8 = 0.0;
            }
            sysml_conditional_6 = sysml_conditional_8;
        }
        else {
            if (sysml_state_breathSpeed <= 0) {
                sysml_conditional_9 = 0.0;
            }
            else {
                if (sysml_state_accumMs < (sysml_state_breathSpeed * 0.5)) {
                    if (sysml_state_breathSpeed == 0) {
                        sysml_conditional_11 = 0.0;
                    }
                    else {
                        sysml_arithmetic_12 = sysml_state_accumMs * 1.0;
                        sysml_arithmetic_13 = (sysml_state_breathSpeed * 1.0) * 0.5;
                        sysml_conditional_11 = ((double)((sysml_state_brightness * 1.0) * sysml_arithmetic_12)) / sysml_arithmetic_13;
                    }
                    sysml_conditional_10 = sysml_conditional_11;
                }
                else {
                    if (sysml_state_breathSpeed == 0) {
                        sysml_conditional_14 = 0.0;
                    }
                    else {
                        sysml_arithmetic_15 = ((sysml_state_breathSpeed * 1.0) * 1.0) - (sysml_state_accumMs * 1.0);
                        sysml_arithmetic_16 = (sysml_state_breathSpeed * 1.0) * 0.5;
                        sysml_conditional_14 = ((double)((sysml_state_brightness * 1.0) * sysml_arithmetic_15)) / sysml_arithmetic_16;
                    }
                    sysml_conditional_10 = sysml_conditional_14;
                }
                sysml_conditional_9 = sysml_conditional_10;
            }
            sysml_conditional_6 = sysml_conditional_9;
        }
        sysml_conditional_5 = sysml_conditional_6;
    }
    sysml_value_0 = sysml_conditional_5;
    int32_t sysml_value_1 = 0;
    if (sysml_state_mode == 0) {
        sysml_conditional_17 = 0;
    }
    else {
        if (sysml_state_mode == 1) {
            sysml_conditional_18 = sysml_state_brightness;
        }
        else {
            sysml_conditional_18 = sysml_state_breathSpeed;
        }
        sysml_conditional_17 = sysml_conditional_18;
    }
    sysml_value_1 = sysml_conditional_17;
    bool sysml_value_2 = 0;
    if (sysml_state_mode == 0) {
        sysml_conditional_19 = false;
    }
    else {
        sysml_conditional_19 = true;
    }
    sysml_value_2 = sysml_conditional_19;
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
static bool hw_value_io_0 = false;
static uint32_t hw_sampled_io_0 = 0;
static bool hw_attempted_io_0 = false;
static bool hw_value_io_1 = false;
static uint32_t hw_sampled_io_1 = 0;
static bool hw_attempted_io_1 = false;
static double hw_value_io_2 = 0;
static int32_t hw_value_io_3 = 0;
TM1637Display hw_display_io_3(4, 5);
static int32_t hw_display_previous_io_3 = INT32_MIN;
static bool hw_value_io_4 = false;
static int32_t hw_value_io_5 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(9, OUTPUT);
  analogWrite(9, (int)constrain((hw_value_io_2 * 1.0 + 0.0), 0.0, 255.0));
  hw_display_io_3.setBrightness(7, true);
  if (hw_value_io_3 != hw_display_previous_io_3) {
    if (hw_value_io_3 < -999 || hw_value_io_3 > 9999) hw_display_io_3.clear();
    else hw_display_io_3.showNumberDec((int)hw_value_io_3, false, 4, 0);
    hw_display_previous_io_3 = hw_value_io_3;
  }
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_4 ? HIGH : LOW);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 10UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (digitalRead(2) == HIGH);
  }
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 10UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = (digitalRead(3) == HIGH);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_5 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_5, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4);
  analogWrite(9, (int)constrain((hw_value_io_2 * 1.0 + 0.0), 0.0, 255.0));
  if (hw_value_io_3 != hw_display_previous_io_3) {
    if (hw_value_io_3 < -999 || hw_value_io_3 > 9999) hw_display_io_3.clear();
    else hw_display_io_3.showNumberDec((int)hw_value_io_3, false, 4, 0);
    hw_display_previous_io_3 = hw_value_io_3;
  }
  digitalWrite(13, hw_value_io_4 ? HIGH : LOW);
}

