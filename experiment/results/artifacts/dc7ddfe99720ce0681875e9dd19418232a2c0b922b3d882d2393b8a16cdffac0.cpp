#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_011\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
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
extern int32_t sysml_state_alertLevel;
/* Global Variable define of  */
extern double sysml_state_keyValueTemp;
/* Global Variable define of  */
extern double sysml_state_keyValueSmoke;
/* Global Variable define of  */
extern bool sysml_state_flameDetectedLatched;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, bool io_2, int32_t* io_3, bool* io_4, bool* io_5, int32_t* io_6, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, bool* serial_0_value_2, int32_t* serial_0_value_3);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_alertLevel = 0;
/* Global Variable define of  */
double sysml_state_keyValueTemp = 0.0;
/* Global Variable define of  */
double sysml_state_keyValueSmoke = 0.0;
/* Global Variable define of  */
bool sysml_state_flameDetectedLatched = false;
/* Init function of CompositeActor named "ProgramPage1" */
#line 167 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_011\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 192 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_011\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 41 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_011\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_alertLevel = 0;
    sysml_state_keyValueTemp = 0.0;
    sysml_state_keyValueSmoke = 0.0;
    sysml_state_flameDetectedLatched = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, bool io_2, int32_t* io_3, bool* io_4, bool* io_5, int32_t* io_6, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, bool* serial_0_value_2, int32_t* serial_0_value_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    int32_t sysml_conditional_6 = 0;
    sysml_state_flameDetectedLatched = io_2;
    sysml_lazy_0 = sysml_state_flameDetectedLatched;
    if (!sysml_lazy_0) {
        sysml_lazy_1 = io_0 >= 600.0;
        if (sysml_lazy_1) {
            sysml_lazy_1 = io_1 >= 600.0;
        }
        sysml_lazy_2 = sysml_lazy_1;
        if (!sysml_lazy_2) {
            sysml_lazy_3 = io_0 >= 700.0;
            if (sysml_lazy_3) {
                sysml_lazy_3 = !(io_0 >= 900.0);
            }
            sysml_lazy_2 = sysml_lazy_3;
        }
        sysml_lazy_4 = sysml_lazy_2;
        if (!sysml_lazy_4) {
            sysml_lazy_4 = io_1 >= 900.0;
        }
        sysml_lazy_0 = sysml_lazy_4;
    }
    if (sysml_lazy_0) {
        sysml_state_alertLevel = 2;
        sysml_state_keyValueTemp = io_0;
        sysml_state_keyValueSmoke = io_1;
    }
    else {
        sysml_lazy_5 = io_0 >= 650.0;
        if (!sysml_lazy_5) {
            sysml_lazy_5 = io_1 >= 650.0;
        }
        if (sysml_lazy_5) {
            sysml_state_alertLevel = 1;
            sysml_state_keyValueTemp = io_0;
            sysml_state_keyValueSmoke = io_1;
        }
        else {
            sysml_state_alertLevel = 0;
            sysml_state_keyValueTemp = 0.0;
            sysml_state_keyValueSmoke = 0.0;
        }
    }
    int32_t sysml_value_0 = 0;
    if (sysml_state_alertLevel == 2) {
        sysml_conditional_6 = 2000;
    }
    else {
        sysml_conditional_6 = 0;
    }
    sysml_value_0 = sysml_conditional_6;
    bool sysml_value_1 = 0;
    sysml_value_1 = (sysml_state_alertLevel == 2);
    bool sysml_value_2 = 0;
    sysml_value_2 = (sysml_state_alertLevel == 1);
    int32_t sysml_value_3 = 0;
    sysml_value_3 = sysml_state_alertLevel;
    bool sysml_value_4 = 0;
    sysml_value_4 = (sysml_state_alertLevel == 2);
    double sysml_value_5 = 0;
    sysml_value_5 = sysml_state_keyValueTemp;
    double sysml_value_6 = 0;
    sysml_value_6 = sysml_state_keyValueSmoke;
    bool sysml_value_7 = 0;
    sysml_value_7 = sysml_state_flameDetectedLatched;
    int32_t sysml_value_8 = 0;
    sysml_value_8 = sysml_state_alertLevel;
    *io_3 = sysml_value_0;
    *io_4 = sysml_value_1;
    *io_5 = sysml_value_2;
    *io_6 = sysml_value_3;
    *serial_0_trigger = sysml_value_4;
    *serial_0_value_0 = sysml_value_5;
    *serial_0_value_1 = sysml_value_6;
    *serial_0_value_2 = sysml_value_7;
    *serial_0_value_3 = sysml_value_8;
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
static int32_t hw_value_io_3 = 0;
static int32_t hw_tone_io_3 = -1;
static bool hw_value_io_4 = false;
static bool hw_value_io_5 = false;
static int32_t hw_value_io_6 = 0;
TM1637Display hw_display_io_6(4, 5);
static int32_t hw_display_previous_io_6 = INT32_MIN;
static bool hw_value_serial_0_trigger = false;
static double hw_value_serial_0_value_0 = 0;
static double hw_value_serial_0_value_1 = 0;
static bool hw_value_serial_0_value_2 = false;
static int32_t hw_value_serial_0_value_3 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(A1, INPUT);
  pinMode(A0, INPUT);
  pinMode(2, INPUT);
  pinMode(6, OUTPUT);
  int32_t hw_tone_command_io_3 = (hw_value_io_3 >= 31 && hw_value_io_3 <= 65535) ? hw_value_io_3 : 0;
  if (hw_tone_command_io_3 != hw_tone_io_3) {
    if (hw_tone_command_io_3 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_3);
    hw_tone_io_3 = hw_tone_command_io_3;
  }
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_4 ? HIGH : LOW);
  pinMode(12, OUTPUT);
  digitalWrite(12, hw_value_io_5 ? HIGH : LOW);
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
    hw_value_io_2 = (digitalRead(2) == HIGH);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0, &hw_value_serial_0_value_1, &hw_value_serial_0_value_2, &hw_value_serial_0_value_3);
  int32_t hw_tone_command_io_3 = (hw_value_io_3 >= 31 && hw_value_io_3 <= 65535) ? hw_value_io_3 : 0;
  if (hw_tone_command_io_3 != hw_tone_io_3) {
    if (hw_tone_command_io_3 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_3);
    hw_tone_io_3 = hw_tone_command_io_3;
  }
  digitalWrite(13, hw_value_io_4 ? HIGH : LOW);
  digitalWrite(12, hw_value_io_5 ? HIGH : LOW);
  if (hw_value_io_6 != hw_display_previous_io_6) {
    if (hw_value_io_6 < -999 || hw_value_io_6 > 9999) hw_display_io_6.clear();
    else hw_display_io_6.showNumberDec((int)hw_value_io_6, false, 4, 0);
    hw_display_previous_io_6 = hw_value_io_6;
  }
  if (hw_value_serial_0_trigger) { Serial.print("temp"); Serial.println(hw_value_serial_0_value_0); }
  if (hw_value_serial_0_trigger) { Serial.print("smoke"); Serial.println(hw_value_serial_0_value_1); }
  if (hw_value_serial_0_trigger) { Serial.print("flame"); Serial.println(hw_value_serial_0_value_2); }
  if (hw_value_serial_0_trigger) { Serial.print("level"); Serial.println(hw_value_serial_0_value_3); }
}

