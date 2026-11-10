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
extern int32_t sysml_state_currentFloor;
/* Global Variable define of  */
extern int32_t sysml_state_targetFloor;
/* Global Variable define of  */
extern int32_t sysml_state_movementRemainingMs;
/* Global Variable define of  */
extern bool sysml_state_previousPressed0;
/* Global Variable define of  */
extern bool sysml_state_previousPressed1;
/* Global Variable define of  */
extern bool sysml_state_previousPressed2;
/* Global Variable define of  */
extern bool sysml_state_overload;
/* Global Variable define of  */
extern bool sysml_state_movementCompletePending;
/* Global Variable define of  */
extern int32_t sysml_state_movementCompleteLastDurationMs;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, double io_3, int32_t* io_4, bool* io_5, int32_t* io_6, bool* serial_0_trigger, int32_t* serial_0_value_0, bool* serial_1_trigger, double* serial_1_value_0, int32_t* serial_1_value_1, bool* serial_1_value_2);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_currentFloor = 1;
/* Global Variable define of  */
int32_t sysml_state_targetFloor = 1;
/* Global Variable define of  */
int32_t sysml_state_movementRemainingMs = 0;
/* Global Variable define of  */
bool sysml_state_previousPressed0 = false;
/* Global Variable define of  */
bool sysml_state_previousPressed1 = false;
/* Global Variable define of  */
bool sysml_state_previousPressed2 = false;
/* Global Variable define of  */
bool sysml_state_overload = false;
/* Global Variable define of  */
bool sysml_state_movementCompletePending = false;
/* Global Variable define of  */
int32_t sysml_state_movementCompleteLastDurationMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_currentFloor = 1;
    sysml_state_targetFloor = 1;
    sysml_state_movementRemainingMs = 0;
    sysml_state_previousPressed0 = false;
    sysml_state_previousPressed1 = false;
    sysml_state_previousPressed2 = false;
    sysml_state_overload = false;
    sysml_state_movementCompletePending = false;
    sysml_state_movementCompleteLastDurationMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, double io_3, int32_t* io_4, bool* io_5, int32_t* io_6, bool* serial_0_trigger, int32_t* serial_0_value_0, bool* serial_1_trigger, double* serial_1_value_0, int32_t* serial_1_value_1, bool* serial_1_value_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    int32_t sysml_conditional_9 = 0;
    bool sysml_conditional_10 = 0;
    bool sysml_lazy_11 = 0;
    int32_t sysml_conditional_12 = 0;
    int32_t sysml_conditional_13 = 0;
    int32_t sysml_conditional_14 = 0;
    double sysml_conditional_15 = 0;
    sysml_lazy_0 = io_0;
    if (!sysml_lazy_0) {
        sysml_lazy_0 = io_1;
    }
    sysml_lazy_1 = sysml_lazy_0;
    if (!sysml_lazy_1) {
        sysml_lazy_1 = io_2;
    }
    if (sysml_lazy_1) {
    }
    if ((io_3 >= 900.0)) {
        sysml_state_overload = true;
    }
    else {
        sysml_state_overload = false;
    }
    sysml_lazy_2 = io_0;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_previousPressed0;
    }
    if (sysml_lazy_2) {
        sysml_state_targetFloor = 1;
    }
    sysml_lazy_3 = io_1;
    if (sysml_lazy_3) {
        sysml_lazy_3 = !sysml_state_previousPressed1;
    }
    if (sysml_lazy_3) {
        sysml_state_targetFloor = 2;
    }
    sysml_lazy_4 = io_2;
    if (sysml_lazy_4) {
        sysml_lazy_4 = !sysml_state_previousPressed2;
    }
    if (sysml_lazy_4) {
        sysml_state_targetFloor = 3;
    }
    sysml_lazy_5 = !sysml_state_overload;
    if (sysml_lazy_5) {
        sysml_lazy_5 = sysml_state_targetFloor != sysml_state_currentFloor;
    }
    if (sysml_lazy_5) {
        sysml_state_movementRemainingMs = 1000;
        sysml_state_movementCompletePending = false;
    }
    sysml_lazy_6 = sysml_state_movementRemainingMs == 0;
    if (sysml_lazy_6) {
        sysml_lazy_6 = sysml_state_currentFloor == sysml_state_targetFloor;
    }
    if (sysml_lazy_6) {
        sysml_state_movementCompletePending = false;
    }
    sysml_lazy_7 = sysml_state_movementRemainingMs > 0;
    if (sysml_lazy_7) {
        sysml_lazy_7 = !sysml_state_overload;
    }
    if (sysml_lazy_7) {
        sysml_state_movementRemainingMs = 0;
        sysml_state_currentFloor = sysml_state_targetFloor;
        sysml_state_movementCompletePending = true;
        sysml_state_movementCompleteLastDurationMs = 1000;
    }
    sysml_lazy_8 = sysml_state_overload;
    if (sysml_lazy_8) {
        sysml_lazy_8 = sysml_state_currentFloor != sysml_state_targetFloor;
    }
    if (sysml_lazy_8) {
        sysml_state_movementRemainingMs = 0;
        sysml_state_movementCompletePending = false;
    }
    sysml_state_previousPressed0 = io_0;
    sysml_state_previousPressed1 = io_1;
    sysml_state_previousPressed2 = io_2;
    int32_t sysml_value_0 = 0;
    if (sysml_state_overload) {
        sysml_conditional_9 = 1000;
    }
    else {
        sysml_conditional_9 = 0;
    }
    sysml_value_0 = sysml_conditional_9;
    bool sysml_value_1 = 0;
    if (sysml_state_overload) {
        sysml_conditional_10 = true;
    }
    else {
        sysml_conditional_10 = false;
    }
    sysml_value_1 = sysml_conditional_10;
    int32_t sysml_value_2 = 0;
    sysml_lazy_11 = sysml_state_overload;
    if (sysml_lazy_11) {
        sysml_lazy_11 = sysml_state_currentFloor == 1;
    }
    if (sysml_lazy_11) {
        sysml_conditional_12 = 10;
    }
    else {
        if (sysml_state_currentFloor == 1) {
            sysml_conditional_13 = 1;
        }
        else {
            if (sysml_state_currentFloor == 2) {
                sysml_conditional_14 = 2;
            }
            else {
                sysml_conditional_14 = 3;
            }
            sysml_conditional_13 = sysml_conditional_14;
        }
        sysml_conditional_12 = sysml_conditional_13;
    }
    sysml_value_2 = sysml_conditional_12;
    bool sysml_value_3 = 0;
    sysml_value_3 = sysml_state_movementCompletePending;
    int32_t sysml_value_4 = 0;
    sysml_value_4 = sysml_state_movementCompleteLastDurationMs;
    bool sysml_value_5 = 0;
    sysml_value_5 = sysml_state_overload;
    double sysml_value_6 = 0;
    if (io_3 >= 900.0) {
        sysml_conditional_15 = 100.0;
    }
    else {
        sysml_conditional_15 = 0.0;
    }
    sysml_value_6 = sysml_conditional_15;
    int32_t sysml_value_7 = 0;
    sysml_value_7 = sysml_state_currentFloor;
    bool sysml_value_8 = 0;
    sysml_value_8 = sysml_state_overload;
    *io_4 = sysml_value_0;
    *io_5 = sysml_value_1;
    *io_6 = sysml_value_2;
    *serial_0_trigger = sysml_value_3;
    *serial_0_value_0 = sysml_value_4;
    *serial_1_trigger = sysml_value_5;
    *serial_1_value_0 = sysml_value_6;
    *serial_1_value_1 = sysml_value_7;
    *serial_1_value_2 = sysml_value_8;
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
static bool hw_value_io_2 = false;
static uint32_t hw_sampled_io_2 = 0;
static bool hw_attempted_io_2 = false;
static double hw_value_io_3 = 0;
static uint32_t hw_sampled_io_3 = 0;
static bool hw_attempted_io_3 = false;
static int32_t hw_value_io_4 = 0;
static int32_t hw_tone_io_4 = -1;
static bool hw_value_io_5 = false;
static int32_t hw_value_io_6 = 0;
TM1637Display hw_display_io_6(7, 5);
static int32_t hw_display_previous_io_6 = INT32_MIN;
static bool hw_value_serial_0_trigger = false;
static int32_t hw_value_serial_0_value_0 = 0;
static bool hw_value_serial_1_trigger = false;
static double hw_value_serial_1_value_0 = 0;
static int32_t hw_value_serial_1_value_1 = 1;
static bool hw_value_serial_1_value_2 = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(A0, INPUT);
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
    hw_value_io_0 = (digitalRead(2) == HIGH);
  }
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 5UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = (digitalRead(3) == HIGH);
  }
  if (!hw_attempted_io_2 || (uint32_t)(hw_now - hw_sampled_io_2) >= 5UL) {
    hw_attempted_io_2 = true; hw_sampled_io_2 = hw_now;
    hw_value_io_2 = (digitalRead(4) == HIGH);
  }
  if (!hw_attempted_io_3 || (uint32_t)(hw_now - hw_sampled_io_3) >= 5UL) {
    hw_attempted_io_3 = true; hw_sampled_io_3 = hw_now;
    hw_value_io_3 = (analogRead(A0) * 1.0 + 0.0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0, &hw_value_serial_1_trigger, &hw_value_serial_1_value_0, &hw_value_serial_1_value_1, &hw_value_serial_1_value_2);
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
  if (hw_value_serial_0_trigger) { Serial.print("movement_complete_ms"); Serial.println(hw_value_serial_0_value_0); }
  if (hw_value_serial_1_trigger) { Serial.print("load_percent"); Serial.println(hw_value_serial_1_value_0); }
  if (hw_value_serial_1_trigger) { Serial.print("floor"); Serial.println(hw_value_serial_1_value_1); }
  if (hw_value_serial_1_trigger) { Serial.print("overload"); Serial.println(hw_value_serial_1_value_2); }
}
