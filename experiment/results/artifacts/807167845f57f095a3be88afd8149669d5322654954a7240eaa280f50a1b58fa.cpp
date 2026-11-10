#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\repaired_workflow_continuation_20261016\\advanced_020\\workflow\\attempt_0\\sketch\\sketch.ino"
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
extern bool sysml_state_pendingMove;
/* Global Variable define of  */
extern int32_t sysml_state_moveRemainingMs;
/* Global Variable define of  */
extern bool sysml_state_previousMoveRequest;
/* Global Variable define of  */
extern bool sysml_state_movementCompletePulse;
/* Global Variable define of  */
extern bool sysml_state_previousPressedAny;
/* Global Variable define of  */
extern int32_t sysml_state_loadPercent;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, double io_3, int32_t* io_4, bool* io_5, int32_t* io_6, bool* serial_0_trigger, int32_t* serial_0_value_0);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_currentFloor = 1;
/* Global Variable define of  */
int32_t sysml_state_targetFloor = 1;
/* Global Variable define of  */
bool sysml_state_pendingMove = false;
/* Global Variable define of  */
int32_t sysml_state_moveRemainingMs = 0;
/* Global Variable define of  */
bool sysml_state_previousMoveRequest = false;
/* Global Variable define of  */
bool sysml_state_movementCompletePulse = false;
/* Global Variable define of  */
bool sysml_state_previousPressedAny = false;
/* Global Variable define of  */
int32_t sysml_state_loadPercent = 0;
/* Init function of CompositeActor named "ProgramPage1" */
#line 224 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\repaired_workflow_continuation_20261016\\advanced_020\\workflow\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 248 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\repaired_workflow_continuation_20261016\\advanced_020\\workflow\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 57 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\repaired_workflow_continuation_20261016\\advanced_020\\workflow\\attempt_0\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_currentFloor = 1;
    sysml_state_targetFloor = 1;
    sysml_state_pendingMove = false;
    sysml_state_moveRemainingMs = 0;
    sysml_state_previousMoveRequest = false;
    sysml_state_movementCompletePulse = false;
    sysml_state_previousPressedAny = false;
    sysml_state_loadPercent = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, double io_3, int32_t* io_4, bool* io_5, int32_t* io_6, bool* serial_0_trigger, int32_t* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    int32_t sysml_conditional_3 = 0;
    int32_t sysml_conditional_4 = 0;
    bool sysml_conditional_5 = 0;
    int32_t sysml_conditional_6 = 0;
    bool sysml_conditional_7 = 0;
    bool sysml_lazy_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_lazy_10 = 0;
    bool sysml_lazy_11 = 0;
    int32_t sysml_conditional_12 = 0;
    bool sysml_conditional_13 = 0;
    sysml_lazy_0 = io_0;
    if (!sysml_lazy_0) {
        sysml_lazy_0 = io_1;
    }
    sysml_lazy_1 = sysml_lazy_0;
    if (!sysml_lazy_1) {
        sysml_lazy_1 = io_2;
    }
    sysml_lazy_2 = sysml_lazy_1;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_previousPressedAny;
    }
    if (sysml_lazy_2) {
        sysml_state_movementCompletePulse = false;
        sysml_state_pendingMove = true;
        sysml_state_previousMoveRequest = true;
        if (io_0) {
            sysml_conditional_3 = 1;
        }
        else {
            if (io_1) {
                sysml_conditional_4 = 2;
            }
            else {
                sysml_conditional_4 = 3;
            }
            sysml_conditional_3 = sysml_conditional_4;
        }
        sysml_state_targetFloor = sysml_conditional_3;
        sysml_state_moveRemainingMs = 1000;
    }
    else {
        if (sysml_state_pendingMove) {
            sysml_state_movementCompletePulse = false;
        }
    }
    if (sysml_state_pendingMove) {
        if (sysml_state_moveRemainingMs <= 0) {
            sysml_conditional_5 = false;
        }
        else {
            sysml_conditional_5 = true;
        }
        sysml_state_pendingMove = sysml_conditional_5;
        if (sysml_state_moveRemainingMs <= 0) {
            sysml_conditional_6 = sysml_state_targetFloor;
        }
        else {
            sysml_conditional_6 = sysml_state_currentFloor;
        }
        sysml_state_currentFloor = sysml_conditional_6;
        if (sysml_state_moveRemainingMs <= 0) {
            sysml_conditional_7 = true;
        }
        else {
            sysml_conditional_7 = false;
        }
        sysml_state_movementCompletePulse = sysml_conditional_7;
    }
    sysml_lazy_8 = io_0;
    if (!sysml_lazy_8) {
        sysml_lazy_8 = io_1;
    }
    sysml_lazy_9 = sysml_lazy_8;
    if (!sysml_lazy_9) {
        sysml_lazy_9 = io_2;
    }
    if (!sysml_lazy_9) {
        sysml_state_previousMoveRequest = false;
    }
    sysml_lazy_10 = io_0;
    if (!sysml_lazy_10) {
        sysml_lazy_10 = io_1;
    }
    sysml_lazy_11 = sysml_lazy_10;
    if (!sysml_lazy_11) {
        sysml_lazy_11 = io_2;
    }
    sysml_state_previousPressedAny = sysml_lazy_11;
    int32_t sysml_value_0 = 0;
    if (sysml_state_loadPercent > 80) {
        sysml_conditional_12 = 1000;
    }
    else {
        sysml_conditional_12 = 0;
    }
    sysml_value_0 = sysml_conditional_12;
    bool sysml_value_1 = 0;
    if (sysml_state_loadPercent > 80) {
        sysml_conditional_13 = true;
    }
    else {
        sysml_conditional_13 = false;
    }
    sysml_value_1 = sysml_conditional_13;
    int32_t sysml_value_2 = 0;
    sysml_value_2 = sysml_state_currentFloor;
    bool sysml_value_3 = 0;
    sysml_value_3 = sysml_state_movementCompletePulse;
    int32_t sysml_value_4 = 0;
    sysml_value_4 = sysml_state_targetFloor;
    *io_4 = sysml_value_0;
    *io_5 = sysml_value_1;
    *io_6 = sysml_value_2;
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
  if (!hw_attempted_io_2 || (uint32_t)(hw_now - hw_sampled_io_2) >= 10UL) {
    hw_attempted_io_2 = true; hw_sampled_io_2 = hw_now;
    hw_value_io_2 = (digitalRead(4) == HIGH);
  }
  if (!hw_attempted_io_3 || (uint32_t)(hw_now - hw_sampled_io_3) >= 10UL) {
    hw_attempted_io_3 = true; hw_sampled_io_3 = hw_now;
    hw_value_io_3 = (analogRead(A0) * 1 + 0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0);
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
  if (hw_value_serial_0_trigger) { Serial.print("floor"); Serial.println(hw_value_serial_0_value_0); }
}

