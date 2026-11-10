#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_020\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
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
extern int32_t sysml_state_requestedFloor;
/* Global Variable define of  */
extern bool sysml_state_movementActive;
/* Global Variable define of  */
extern int32_t sysml_state_movementRemainingSteps;
/* Global Variable define of  */
extern bool sysml_state_previousIo0;
/* Global Variable define of  */
extern bool sysml_state_previousIo1;
/* Global Variable define of  */
extern bool sysml_state_previousIo2;
/* Global Variable define of  */
extern bool sysml_state_previousOverload;
/* Global Variable define of  */
extern bool sysml_state_overload;
/* Global Variable define of  */
extern int32_t sysml_state_loadPercent;
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
int32_t sysml_state_requestedFloor = 1;
/* Global Variable define of  */
bool sysml_state_movementActive = false;
/* Global Variable define of  */
int32_t sysml_state_movementRemainingSteps = 0;
/* Global Variable define of  */
bool sysml_state_previousIo0 = false;
/* Global Variable define of  */
bool sysml_state_previousIo1 = false;
/* Global Variable define of  */
bool sysml_state_previousIo2 = false;
/* Global Variable define of  */
bool sysml_state_previousOverload = false;
/* Global Variable define of  */
bool sysml_state_overload = false;
/* Global Variable define of  */
int32_t sysml_state_loadPercent = 0;
/* Init function of CompositeActor named "ProgramPage1" */
#line 273 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_020\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void setup();
#line 297 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_020\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void loop();
#line 65 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_020\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_currentFloor = 1;
    sysml_state_requestedFloor = 1;
    sysml_state_movementActive = false;
    sysml_state_movementRemainingSteps = 0;
    sysml_state_previousIo0 = false;
    sysml_state_previousIo1 = false;
    sysml_state_previousIo2 = false;
    sysml_state_previousOverload = false;
    sysml_state_overload = false;
    sysml_state_loadPercent = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, double io_3, int32_t* io_4, bool* io_5, int32_t* io_6, bool* serial_0_trigger, int32_t* serial_0_value_0, bool* serial_1_trigger, double* serial_1_value_0, int32_t* serial_1_value_1, bool* serial_1_value_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int32_t sysml_conditional_0 = 0;
    int32_t sysml_conditional_1 = 0;
    int32_t sysml_conditional_2 = 0;
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
    bool sysml_lazy_13 = 0;
    bool sysml_conditional_14 = 0;
    int32_t sysml_conditional_15 = 0;
    if (io_3 >= 0.0) {
        if (io_3 <= 1023.0) {
            if ((((double)(io_3 * 100.0)) / 1023.0) < ((double)((int32_t)(((double)(io_3 * 100.0)) / 1023.0)))) {
                sysml_conditional_2 = 1;
            }
            else {
                sysml_conditional_2 = 0;
            }
            sysml_conditional_1 = ((int32_t)(((double)(io_3 * 100.0)) / 1023.0)) - sysml_conditional_2;
        }
        else {
            sysml_conditional_1 = 100;
        }
        sysml_conditional_0 = sysml_conditional_1;
    }
    else {
        sysml_conditional_0 = 0;
    }
    sysml_state_loadPercent = sysml_conditional_0;
    sysml_state_overload = (sysml_state_loadPercent >= 80);
    sysml_lazy_3 = io_0;
    if (sysml_lazy_3) {
        sysml_lazy_3 = !sysml_state_previousIo0;
    }
    if (sysml_lazy_3) {
        sysml_state_requestedFloor = 1;
    }
    sysml_lazy_4 = io_1;
    if (sysml_lazy_4) {
        sysml_lazy_4 = !sysml_state_previousIo1;
    }
    if (sysml_lazy_4) {
        sysml_state_requestedFloor = 2;
    }
    sysml_lazy_5 = io_2;
    if (sysml_lazy_5) {
        sysml_lazy_5 = !sysml_state_previousIo2;
    }
    if (sysml_lazy_5) {
        sysml_state_requestedFloor = 3;
    }
    if (sysml_state_movementActive) {
        sysml_state_movementRemainingSteps = (sysml_state_movementRemainingSteps - 1);
        if ((sysml_state_movementRemainingSteps <= 0)) {
            sysml_state_movementActive = false;
            sysml_state_currentFloor = sysml_state_requestedFloor;
        }
    }
    sysml_lazy_6 = io_0;
    if (!sysml_lazy_6) {
        sysml_lazy_6 = io_1;
    }
    sysml_lazy_7 = sysml_lazy_6;
    if (!sysml_lazy_7) {
        sysml_lazy_7 = io_2;
    }
    sysml_lazy_8 = sysml_lazy_7;
    if (sysml_lazy_8) {
        sysml_lazy_8 = !sysml_state_movementActive;
    }
    if (sysml_lazy_8) {
        if ((!sysml_state_overload)) {
            sysml_state_movementActive = true;
            sysml_state_movementRemainingSteps = 20;
        }
    }
    if (sysml_state_overload) {
        sysml_state_movementActive = false;
        sysml_state_movementRemainingSteps = 0;
    }
    sysml_state_previousIo0 = io_0;
    sysml_state_previousIo1 = io_1;
    sysml_state_previousIo2 = io_2;
    sysml_state_previousOverload = sysml_state_overload;
    int32_t sysml_value_0 = 0;
    if (sysml_state_overload) {
        sysml_conditional_9 = 1000;
    }
    else {
        sysml_conditional_9 = 0;
    }
    sysml_value_0 = sysml_conditional_9;
    bool sysml_value_1 = 0;
    sysml_value_1 = sysml_state_overload;
    int32_t sysml_value_2 = 0;
    sysml_value_2 = sysml_state_currentFloor;
    bool sysml_value_3 = 0;
    if (sysml_state_movementActive == false) {
        sysml_lazy_11 = sysml_state_movementRemainingSteps == 0;
        if (sysml_lazy_11) {
            sysml_lazy_11 = !sysml_state_overload;
        }
        sysml_conditional_10 = sysml_lazy_11;
    }
    else {
        sysml_conditional_10 = false;
    }
    sysml_value_3 = sysml_conditional_10;
    int32_t sysml_value_4 = 0;
    if (sysml_state_movementActive == false) {
        sysml_conditional_12 = 0;
    }
    else {
        sysml_conditional_12 = 0;
    }
    sysml_value_4 = sysml_conditional_12;
    bool sysml_value_5 = 0;
    sysml_lazy_13 = sysml_state_overload;
    if (sysml_lazy_13) {
        sysml_lazy_13 = !sysml_state_previousOverload;
    }
    if (sysml_lazy_13) {
        sysml_conditional_14 = true;
    }
    else {
        sysml_conditional_14 = false;
    }
    sysml_value_5 = sysml_conditional_14;
    double sysml_value_6 = 0;
    sysml_value_6 = (sysml_state_loadPercent * 1.0);
    int32_t sysml_value_7 = 0;
    if (sysml_state_overload) {
        sysml_conditional_15 = sysml_state_requestedFloor;
    }
    else {
        sysml_conditional_15 = 1;
    }
    sysml_value_7 = sysml_conditional_15;
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

