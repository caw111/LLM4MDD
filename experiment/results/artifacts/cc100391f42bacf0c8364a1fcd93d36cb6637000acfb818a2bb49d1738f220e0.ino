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
extern int32_t sysml_state_inWashStage;
/* Global Variable define of  */
extern bool sysml_state_prevPressed;
/* Global Variable define of  */
extern bool sysml_state_faultActive;
/* Global Variable define of  */
extern bool sysml_state_endConfirmed;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_4, double io_5, bool* io_1, int32_t* io_2, int32_t* io_3, bool* serial_0_trigger, int32_t* serial_0_value_0);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_inWashStage = 0;
/* Global Variable define of  */
bool sysml_state_prevPressed = false;
/* Global Variable define of  */
bool sysml_state_faultActive = false;
/* Global Variable define of  */
bool sysml_state_endConfirmed = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_inWashStage = 0;
    sysml_state_prevPressed = false;
    sysml_state_faultActive = false;
    sysml_state_endConfirmed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_4, double io_5, bool* io_1, int32_t* io_2, int32_t* io_3, bool* serial_0_trigger, int32_t* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_conditional_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_conditional_4 = 0;
    int32_t sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    int32_t sysml_conditional_7 = 0;
    int32_t sysml_conditional_8 = 0;
    if ((!io_4)) {
        sysml_state_faultActive = false;
        sysml_state_endConfirmed = false;
        sysml_state_inWashStage = 0;
        sysml_state_prevPressed = false;
    }
    else {
        if (sysml_state_faultActive) {
            sysml_state_inWashStage = 0;
            sysml_state_endConfirmed = sysml_state_endConfirmed;
        }
        sysml_lazy_0 = io_0;
        if (sysml_lazy_0) {
            sysml_lazy_0 = !sysml_state_prevPressed;
        }
        if (sysml_lazy_0) {
            sysml_state_endConfirmed = true;
        }
        sysml_lazy_1 = !sysml_state_faultActive;
        if (sysml_lazy_1) {
            sysml_lazy_1 = !sysml_state_endConfirmed;
        }
        if (sysml_lazy_1) {
            sysml_state_inWashStage = 3;
        }
        else {
            sysml_state_inWashStage = sysml_state_inWashStage;
        }
        sysml_state_prevPressed = io_4;
    }
    bool sysml_value_0 = 0;
    if (sysml_state_faultActive) {
        sysml_conditional_2 = false;
    }
    else {
        sysml_lazy_3 = sysml_state_inWashStage == 3;
        if (sysml_lazy_3) {
            sysml_lazy_3 = !sysml_state_endConfirmed;
        }
        if (sysml_lazy_3) {
            sysml_conditional_4 = true;
        }
        else {
            sysml_conditional_4 = false;
        }
        sysml_conditional_2 = sysml_conditional_4;
    }
    sysml_value_0 = sysml_conditional_2;
    int32_t sysml_value_1 = 0;
    if (sysml_state_faultActive) {
        sysml_conditional_5 = 0;
    }
    else {
        sysml_lazy_6 = sysml_state_inWashStage == 3;
        if (sysml_lazy_6) {
            sysml_lazy_6 = !sysml_state_endConfirmed;
        }
        if (sysml_lazy_6) {
            sysml_conditional_7 = 1000;
        }
        else {
            sysml_conditional_7 = 0;
        }
        sysml_conditional_5 = sysml_conditional_7;
    }
    sysml_value_1 = sysml_conditional_5;
    int32_t sysml_value_2 = 0;
    sysml_value_2 = sysml_state_inWashStage;
    bool sysml_value_3 = 0;
    sysml_value_3 = sysml_state_faultActive;
    int32_t sysml_value_4 = 0;
    if (sysml_state_faultActive) {
        sysml_conditional_8 = 1;
    }
    else {
        sysml_conditional_8 = 0;
    }
    sysml_value_4 = sysml_conditional_8;
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
    *io_3 = sysml_value_2;
    *serial_0_trigger = sysml_value_3;
    *serial_0_value_0 = sysml_value_4;
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
static int32_t hw_value_io_2 = 0;
static int32_t hw_tone_io_2 = -1;
static int32_t hw_value_io_3 = 0;
TM1637Display hw_display_io_3(4, 5);
static int32_t hw_display_previous_io_3 = INT32_MIN;
static bool hw_value_io_4 = false;
static uint32_t hw_sampled_io_4 = 0;
static bool hw_attempted_io_4 = false;
static double hw_value_io_5 = 0;
static uint32_t hw_sampled_io_5 = 0;
static bool hw_attempted_io_5 = false;
static bool hw_value_serial_0_trigger = false;
static int32_t hw_value_serial_0_value_0 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(3, INPUT);
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_1 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  int32_t hw_tone_command_io_2 = (hw_value_io_2 >= 31 && hw_value_io_2 <= 65535) ? hw_value_io_2 : 0;
  if (hw_tone_command_io_2 != hw_tone_io_2) {
    if (hw_tone_command_io_2 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_2);
    hw_tone_io_2 = hw_tone_command_io_2;
  }
  hw_display_io_3.setBrightness(7, true);
  if (hw_value_io_3 != hw_display_previous_io_3) {
    if (hw_value_io_3 < -999 || hw_value_io_3 > 9999) hw_display_io_3.clear();
    else hw_display_io_3.showNumberDec((int)hw_value_io_3, false, 4, 0);
    hw_display_previous_io_3 = hw_value_io_3;
  }
  pinMode(2, INPUT);
  pinMode(A0, INPUT);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 10UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (digitalRead(3) == HIGH);
  }
  if (!hw_attempted_io_4 || (uint32_t)(hw_now - hw_sampled_io_4) >= 10UL) {
    hw_attempted_io_4 = true; hw_sampled_io_4 = hw_now;
    hw_value_io_4 = (digitalRead(2) == HIGH);
  }
  if (!hw_attempted_io_5 || (uint32_t)(hw_now - hw_sampled_io_5) >= 10UL) {
    hw_attempted_io_5 = true; hw_sampled_io_5 = hw_now;
    hw_value_io_5 = (analogRead(A0) * 1.0 + 0.0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_4, hw_value_io_5, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0);
  digitalWrite(13, hw_value_io_1 ? HIGH : LOW);
  int32_t hw_tone_command_io_2 = (hw_value_io_2 >= 31 && hw_value_io_2 <= 65535) ? hw_value_io_2 : 0;
  if (hw_tone_command_io_2 != hw_tone_io_2) {
    if (hw_tone_command_io_2 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_2);
    hw_tone_io_2 = hw_tone_command_io_2;
  }
  if (hw_value_io_3 != hw_display_previous_io_3) {
    if (hw_value_io_3 < -999 || hw_value_io_3 > 9999) hw_display_io_3.clear();
    else hw_display_io_3.showNumberDec((int)hw_value_io_3, false, 4, 0);
    hw_display_previous_io_3 = hw_value_io_3;
  }
  if (hw_value_serial_0_trigger) { Serial.print("wash_fault_state"); Serial.println(hw_value_serial_0_value_0); }
}
