#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_001\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
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
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double* io_1, bool* io_2, int32_t* io_3);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Init function of CompositeActor named "ProgramPage1" */
#line 107 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_001\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 123 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_001\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 25 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_001\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double* io_1, bool* io_2, int32_t* io_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_value_0 = 0;
    double sysml_conditional_0 = 0;
    double sysml_conditional_1 = 0;
    double sysml_conditional_2 = 0;
    double sysml_conditional_3 = 0;
    double sysml_conditional_4 = 0;
    bool sysml_conditional_5 = 0;
    int32_t sysml_conditional_6 = 0;
    if (io_0 > 800.0) {
        sysml_conditional_0 = 0.0;
    }
    else {
        if (io_0 > 0.0) {
            if (io_0 <= 1023.0) {
                if (io_0 == 0.0) {
                    sysml_conditional_3 = 1023.0;
                }
                else {
                    if ((1023.0 - io_0) == 1023.0) {
                        sysml_conditional_4 = 1023.0;
                    }
                    else {
                        sysml_conditional_4 = 1023.0 - io_0;
                    }
                    sysml_conditional_3 = sysml_conditional_4;
                }
                sysml_conditional_2 = sysml_conditional_3;
            }
            else {
                sysml_conditional_2 = 0.0;
            }
            sysml_conditional_1 = sysml_conditional_2;
        }
        else {
            sysml_conditional_1 = 1023.0;
        }
        sysml_conditional_0 = sysml_conditional_1;
    }
    sysml_value_0 = sysml_conditional_0;
    bool sysml_value_1 = 0;
    if (io_0 > 800.0) {
        sysml_conditional_5 = true;
    }
    else {
        sysml_conditional_5 = false;
    }
    sysml_value_1 = sysml_conditional_5;
    int32_t sysml_value_2 = 0;
    if (io_0 > 800.0) {
        sysml_conditional_6 = 1000;
    }
    else {
        sysml_conditional_6 = 0;
    }
    sysml_value_2 = sysml_conditional_6;
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
    *io_3 = sysml_value_2;
}
// END COMPILED SYSML MODEL

// LLM4MDD target-board: arduino:avr:uno
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
static bool hw_value_io_2 = false;
static int32_t hw_value_io_3 = 0;
static int32_t hw_tone_io_3 = -1;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(A0, INPUT);
  pinMode(9, OUTPUT);
  analogWrite(9, (int)constrain((hw_value_io_1 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_2 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  int32_t hw_tone_command_io_3 = (hw_value_io_3 >= 31 && hw_value_io_3 <= 65535) ? hw_value_io_3 : 0;
  if (hw_tone_command_io_3 != hw_tone_io_3) {
    if (hw_tone_command_io_3 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_3);
    hw_tone_io_3 = hw_tone_command_io_3;
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
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3);
  analogWrite(9, (int)constrain((hw_value_io_1 * 1.0 + 0.0), 0.0, 255.0));
  digitalWrite(13, hw_value_io_2 ? HIGH : LOW);
  int32_t hw_tone_command_io_3 = (hw_value_io_3 >= 31 && hw_value_io_3 <= 65535) ? hw_value_io_3 : 0;
  if (hw_tone_command_io_3 != hw_tone_io_3) {
    if (hw_tone_command_io_3 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_3);
    hw_tone_io_3 = hw_tone_command_io_3;
  }
}

