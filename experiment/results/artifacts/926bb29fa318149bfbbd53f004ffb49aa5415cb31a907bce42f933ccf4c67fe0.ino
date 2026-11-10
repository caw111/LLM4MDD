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
extern bool sysml_state_running;
/* Global Variable define of  */
extern int32_t sysml_state_remainingSeconds;
/* Global Variable define of  */
extern bool sysml_state_beeping;
/* Global Variable define of  */
extern bool sysml_state_prevAddPressed;
/* Global Variable define of  */
extern bool sysml_state_prevStartPressed;
/* Global Variable define of  */
extern int32_t sysml_state_beepRemainingSteps;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, int32_t* io_2, int32_t* io_3);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_running = false;
/* Global Variable define of  */
int32_t sysml_state_remainingSeconds = 0;
/* Global Variable define of  */
bool sysml_state_beeping = false;
/* Global Variable define of  */
bool sysml_state_prevAddPressed = false;
/* Global Variable define of  */
bool sysml_state_prevStartPressed = false;
/* Global Variable define of  */
int32_t sysml_state_beepRemainingSteps = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_running = false;
    sysml_state_remainingSeconds = 0;
    sysml_state_beeping = false;
    sysml_state_prevAddPressed = false;
    sysml_state_prevStartPressed = false;
    sysml_state_beepRemainingSteps = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, int32_t* io_2, int32_t* io_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    int32_t sysml_conditional_4 = 0;
    int32_t sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    int32_t sysml_conditional_7 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_prevAddPressed;
    }
    if (sysml_lazy_0) {
        sysml_state_remainingSeconds = (sysml_state_remainingSeconds + 10);
    }
    sysml_lazy_1 = io_1;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_prevStartPressed;
    }
    if (sysml_lazy_1) {
        sysml_state_running = true;
    }
    sysml_lazy_2 = sysml_state_running;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_beeping;
    }
    if (sysml_lazy_2) {
        sysml_lazy_3 = sysml_state_remainingSeconds == 0;
        if (sysml_lazy_3) {
            sysml_lazy_3 = true;
        }
        if (sysml_lazy_3) {
            sysml_conditional_4 = 0;
        }
        else {
            if (sysml_state_remainingSeconds > 0) {
                sysml_conditional_5 = sysml_state_remainingSeconds - 1;
            }
            else {
                sysml_conditional_5 = sysml_state_remainingSeconds;
            }
            sysml_conditional_4 = sysml_conditional_5;
        }
        sysml_state_remainingSeconds = sysml_conditional_4;
        if ((sysml_state_remainingSeconds <= 0)) {
            sysml_state_beeping = true;
            sysml_state_beepRemainingSteps = 0;
            sysml_state_running = false;
        }
    }
    sysml_lazy_6 = sysml_state_beeping;
    if (sysml_lazy_6) {
        sysml_lazy_6 = sysml_state_beepRemainingSteps > 0;
    }
    if (sysml_lazy_6) {
        sysml_state_beepRemainingSteps = (sysml_state_beepRemainingSteps - 1);
        if ((sysml_state_beepRemainingSteps <= 1)) {
            sysml_state_beeping = false;
        }
    }
    sysml_state_prevAddPressed = io_0;
    sysml_state_prevStartPressed = io_1;
    int32_t sysml_value_0 = 0;
    if (sysml_state_beeping) {
        sysml_conditional_7 = 31;
    }
    else {
        sysml_conditional_7 = 0;
    }
    sysml_value_0 = sysml_conditional_7;
    int32_t sysml_value_1 = 0;
    sysml_value_1 = sysml_state_remainingSeconds;
    *io_2 = sysml_value_0;
    *io_3 = sysml_value_1;
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
static bool hw_value_io_0 = true;
static uint32_t hw_sampled_io_0 = 0;
static bool hw_attempted_io_0 = false;
static bool hw_value_io_1 = true;
static uint32_t hw_sampled_io_1 = 0;
static bool hw_attempted_io_1 = false;
static int32_t hw_value_io_2 = 31;
static int32_t hw_tone_io_2 = -1;
static int32_t hw_value_io_3 = 0;
TM1637Display hw_display_io_3(4, 5);
static int32_t hw_display_previous_io_3 = INT32_MIN;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
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
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, &hw_value_io_2, &hw_value_io_3);
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
}
