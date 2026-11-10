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
extern bool sysml_state_previousPressed1;
/* Global Variable define of  */
extern bool sysml_state_previousPressed2;
/* Global Variable define of  */
extern bool sysml_state_previousPressed3;
/* Global Variable define of  */
extern bool sysml_state_previousPressed4;
/* Global Variable define of  */
extern bool sysml_state_locked;
/* Global Variable define of  */
extern int32_t sysml_state_winnerId;
/* Global Variable define of  */
extern int32_t sysml_state_resetAccumulatedMs;
/* Global Variable define of  */
extern bool sysml_state_resetting;
/* Global Variable define of  */
extern bool sysml_state_resetRequest;
/* Global Variable define of  */
extern bool sysml_state_serialTriggerNext;
/* Global Variable define of  */
extern int32_t sysml_state_serialTimestampMsNext;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, bool io_3, bool io_4, bool* io_5, int32_t* io_6, int32_t* io_7, bool* serial_0_trigger, int32_t* serial_0_value_0, int32_t* serial_0_value_1);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousPressed1 = false;
/* Global Variable define of  */
bool sysml_state_previousPressed2 = false;
/* Global Variable define of  */
bool sysml_state_previousPressed3 = false;
/* Global Variable define of  */
bool sysml_state_previousPressed4 = false;
/* Global Variable define of  */
bool sysml_state_locked = false;
/* Global Variable define of  */
int32_t sysml_state_winnerId = 0;
/* Global Variable define of  */
int32_t sysml_state_resetAccumulatedMs = 0;
/* Global Variable define of  */
bool sysml_state_resetting = false;
/* Global Variable define of  */
bool sysml_state_resetRequest = false;
/* Global Variable define of  */
bool sysml_state_serialTriggerNext = false;
/* Global Variable define of  */
int32_t sysml_state_serialTimestampMsNext = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousPressed1 = false;
    sysml_state_previousPressed2 = false;
    sysml_state_previousPressed3 = false;
    sysml_state_previousPressed4 = false;
    sysml_state_locked = false;
    sysml_state_winnerId = 0;
    sysml_state_resetAccumulatedMs = 0;
    sysml_state_resetting = false;
    sysml_state_resetRequest = false;
    sysml_state_serialTriggerNext = false;
    sysml_state_serialTimestampMsNext = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, bool io_3, bool io_4, bool* io_5, int32_t* io_6, int32_t* io_7, bool* serial_0_trigger, int32_t* serial_0_value_0, int32_t* serial_0_value_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    if (io_4) {
        sysml_state_resetting = true;
        sysml_state_resetAccumulatedMs = 0;
    }
    if ((!io_4)) {
        sysml_state_resetting = false;
        sysml_state_resetAccumulatedMs = 0;
    }
    sysml_lazy_0 = io_4;
    if (sysml_lazy_0) {
        sysml_lazy_0 = sysml_state_resetAccumulatedMs >= 3000;
    }
    if (sysml_lazy_0) {
        sysml_state_resetRequest = true;
    }
    else {
        sysml_state_resetRequest = false;
    }
    if (sysml_state_resetRequest) {
        sysml_state_locked = false;
        sysml_state_winnerId = 0;
        sysml_state_serialTriggerNext = false;
    }
    if ((!sysml_state_resetRequest)) {
        if ((!sysml_state_locked)) {
            sysml_lazy_1 = io_0;
            if (sysml_lazy_1) {
                sysml_lazy_1 = !sysml_state_previousPressed1;
            }
            if (sysml_lazy_1) {
                sysml_state_locked = true;
                sysml_state_winnerId = 1;
                sysml_state_serialTriggerNext = true;
                sysml_state_serialTimestampMsNext = 0;
            }
            sysml_lazy_2 = io_1;
            if (sysml_lazy_2) {
                sysml_lazy_2 = !sysml_state_previousPressed2;
            }
            if (sysml_lazy_2) {
                sysml_state_locked = true;
                sysml_state_winnerId = 2;
                sysml_state_serialTriggerNext = true;
                sysml_state_serialTimestampMsNext = 0;
            }
            sysml_lazy_3 = io_2;
            if (sysml_lazy_3) {
                sysml_lazy_3 = !sysml_state_previousPressed3;
            }
            if (sysml_lazy_3) {
                sysml_state_locked = true;
                sysml_state_winnerId = 3;
                sysml_state_serialTriggerNext = true;
                sysml_state_serialTimestampMsNext = 0;
            }
            sysml_lazy_4 = io_3;
            if (sysml_lazy_4) {
                sysml_lazy_4 = !sysml_state_previousPressed4;
            }
            if (sysml_lazy_4) {
                sysml_state_locked = true;
                sysml_state_winnerId = 4;
                sysml_state_serialTriggerNext = true;
                sysml_state_serialTimestampMsNext = 0;
            }
        }
        else {
            sysml_state_serialTriggerNext = false;
        }
    }
    sysml_state_serialTriggerNext = false;
    sysml_state_previousPressed1 = io_0;
    sysml_state_previousPressed2 = io_1;
    sysml_state_previousPressed3 = io_2;
    sysml_state_previousPressed4 = io_3;
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_locked;
    int32_t sysml_value_1 = 0;
    sysml_value_1 = 0;
    int32_t sysml_value_2 = 0;
    sysml_value_2 = sysml_state_winnerId;
    bool sysml_value_3 = 0;
    sysml_value_3 = false;
    int32_t sysml_value_4 = 0;
    sysml_value_4 = 0;
    int32_t sysml_value_5 = 0;
    sysml_value_5 = sysml_state_winnerId;
    *io_5 = sysml_value_0;
    *io_6 = sysml_value_1;
    *io_7 = sysml_value_2;
    *serial_0_trigger = sysml_value_3;
    *serial_0_value_0 = sysml_value_4;
    *serial_0_value_1 = sysml_value_5;
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
static bool hw_value_io_3 = false;
static uint32_t hw_sampled_io_3 = 0;
static bool hw_attempted_io_3 = false;
static bool hw_value_io_4 = false;
static uint32_t hw_sampled_io_4 = 0;
static bool hw_attempted_io_4 = false;
static bool hw_value_io_5 = false;
static int32_t hw_value_io_6 = 0;
static int32_t hw_tone_io_6 = -1;
static int32_t hw_value_io_7 = 0;
TM1637Display hw_display_io_7(4, 5);
static int32_t hw_display_previous_io_7 = INT32_MIN;
static bool hw_value_serial_0_trigger = false;
static int32_t hw_value_serial_0_value_0 = 0;
static int32_t hw_value_serial_0_value_1 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_5 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  int32_t hw_tone_command_io_6 = (hw_value_io_6 >= 31 && hw_value_io_6 <= 65535) ? hw_value_io_6 : 0;
  if (hw_tone_command_io_6 != hw_tone_io_6) {
    if (hw_tone_command_io_6 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_6);
    hw_tone_io_6 = hw_tone_command_io_6;
  }
  hw_display_io_7.setBrightness(7, true);
  if (hw_value_io_7 != hw_display_previous_io_7) {
    if (hw_value_io_7 < -999 || hw_value_io_7 > 9999) hw_display_io_7.clear();
    else hw_display_io_7.showNumberDec((int)hw_value_io_7, false, 4, 0);
    hw_display_previous_io_7 = hw_value_io_7;
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
    hw_value_io_2 = (digitalRead(7) == HIGH);
  }
  if (!hw_attempted_io_3 || (uint32_t)(hw_now - hw_sampled_io_3) >= 5UL) {
    hw_attempted_io_3 = true; hw_sampled_io_3 = hw_now;
    hw_value_io_3 = (digitalRead(8) == HIGH);
  }
  if (!hw_attempted_io_4 || (uint32_t)(hw_now - hw_sampled_io_4) >= 5UL) {
    hw_attempted_io_4 = true; hw_sampled_io_4 = hw_now;
    hw_value_io_4 = (digitalRead(9) == HIGH);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, hw_value_io_3, hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_io_7, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0, &hw_value_serial_0_value_1);
  digitalWrite(13, hw_value_io_5 ? HIGH : LOW);
  int32_t hw_tone_command_io_6 = (hw_value_io_6 >= 31 && hw_value_io_6 <= 65535) ? hw_value_io_6 : 0;
  if (hw_tone_command_io_6 != hw_tone_io_6) {
    if (hw_tone_command_io_6 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_6);
    hw_tone_io_6 = hw_tone_command_io_6;
  }
  if (hw_value_io_7 != hw_display_previous_io_7) {
    if (hw_value_io_7 < -999 || hw_value_io_7 > 9999) hw_display_io_7.clear();
    else hw_display_io_7.showNumberDec((int)hw_value_io_7, false, 4, 0);
    hw_display_previous_io_7 = hw_value_io_7;
  }
  if (hw_value_serial_0_trigger) { Serial.print("timestamp_ms"); Serial.println(hw_value_serial_0_value_0); }
  if (hw_value_serial_0_trigger) { Serial.print("winner_id"); Serial.println(hw_value_serial_0_value_1); }
}
