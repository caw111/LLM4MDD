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
extern int32_t sysml_state_stage;
/* Global Variable define of  */
extern bool sysml_state_inFault;
/* Global Variable define of  */
extern bool sysml_state_prevButton;
/* Global Variable define of  */
extern int32_t sysml_state_silenceRun;
/* Global Variable define of  */
extern bool sysml_state_faultLatched;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_4, double io_5, bool* io_1, int32_t* io_2, int32_t* io_3, bool* serial_0_trigger, int32_t* serial_0_value_0);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_stage = 1;
/* Global Variable define of  */
bool sysml_state_inFault = false;
/* Global Variable define of  */
bool sysml_state_prevButton = false;
/* Global Variable define of  */
int32_t sysml_state_silenceRun = 0;
/* Global Variable define of  */
bool sysml_state_faultLatched = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_stage = 1;
    sysml_state_inFault = false;
    sysml_state_prevButton = false;
    sysml_state_silenceRun = 0;
    sysml_state_faultLatched = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_4, double io_5, bool* io_1, int32_t* io_2, int32_t* io_3, bool* serial_0_trigger, int32_t* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    int32_t sysml_conditional_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    int32_t sysml_conditional_9 = 0;
    int32_t sysml_conditional_10 = 0;
    bool sysml_lazy_11 = 0;
    bool sysml_lazy_12 = 0;
    bool sysml_lazy_13 = 0;
    bool sysml_conditional_14 = 0;
    int32_t sysml_conditional_15 = 0;
    int32_t sysml_arithmetic_16 = 0;
    bool sysml_conditional_17 = 0;
    bool sysml_lazy_18 = 0;
    int32_t sysml_conditional_19 = 0;
    int32_t sysml_conditional_20 = 0;
    sysml_state_prevButton = io_0;
    if (sysml_state_inFault) {
        sysml_state_stage = 1;
        sysml_state_inFault = false;
        sysml_state_faultLatched = false;
        sysml_state_silenceRun = 0;
    }
    sysml_lazy_0 = sysml_state_silenceRun >= 50;
    if (sysml_lazy_0) {
        sysml_lazy_1 = io_5 < 10.0;
        if (!sysml_lazy_1) {
            sysml_lazy_1 = io_5 > 950.0;
        }
        sysml_lazy_0 = sysml_lazy_1;
    }
    if (sysml_lazy_0) {
        sysml_state_faultLatched = true;
        sysml_state_inFault = true;
        sysml_state_stage = 1;
    }
    else {
        sysml_state_faultLatched = false;
    }
    if (!io_4) {
        sysml_conditional_2 = sysml_state_silenceRun + 1;
    }
    else {
        sysml_conditional_2 = 0;
    }
    sysml_state_silenceRun = sysml_conditional_2;
    sysml_lazy_3 = io_5 >= 10.0;
    if (sysml_lazy_3) {
        sysml_lazy_3 = io_5 <= 950.0;
    }
    sysml_lazy_4 = sysml_lazy_3;
    if (sysml_lazy_4) {
        sysml_lazy_4 = sysml_state_silenceRun >= 20;
    }
    if (sysml_lazy_4) {
        sysml_state_stage = 4;
    }
    sysml_lazy_5 = !(sysml_state_stage == 4);
    if (sysml_lazy_5) {
        sysml_lazy_5 = io_5 < 10.0;
    }
    if (sysml_lazy_5) {
        sysml_state_stage = 1;
    }
    sysml_lazy_6 = !(sysml_state_stage == 4);
    if (sysml_lazy_6) {
        sysml_lazy_6 = io_5 >= 10.0;
    }
    sysml_lazy_7 = sysml_lazy_6;
    if (sysml_lazy_7) {
        sysml_lazy_7 = io_5 <= 950.0;
    }
    sysml_lazy_8 = sysml_lazy_7;
    if (sysml_lazy_8) {
        sysml_lazy_8 = !io_4;
    }
    if (sysml_lazy_8) {
        sysml_state_stage = 2;
    }
    else {
        if (!(sysml_state_stage == 4)) {
            if (sysml_state_silenceRun >= 20) {
                sysml_conditional_10 = 3;
            }
            else {
                sysml_conditional_10 = sysml_state_stage;
            }
            sysml_conditional_9 = sysml_conditional_10;
        }
        else {
            sysml_conditional_9 = sysml_state_stage;
        }
        sysml_state_stage = sysml_conditional_9;
    }
    sysml_lazy_11 = sysml_state_stage == 4;
    if (sysml_lazy_11) {
        sysml_lazy_11 = io_0;
    }
    sysml_lazy_12 = sysml_lazy_11;
    if (sysml_lazy_12) {
        sysml_lazy_12 = !sysml_state_prevButton;
    }
    if (sysml_lazy_12) {
        sysml_state_stage = 1;
        sysml_state_faultLatched = false;
        sysml_state_inFault = false;
        sysml_state_silenceRun = 0;
    }
    bool sysml_value_0 = 0;
    sysml_lazy_13 = sysml_state_stage == 4;
    if (sysml_lazy_13) {
        sysml_lazy_13 = !sysml_state_faultLatched;
    }
    if (sysml_lazy_13) {
        if ((((double)sysml_state_silenceRun) / 2) < ((double)((int32_t)(((double)sysml_state_silenceRun) / 2)))) {
            sysml_conditional_15 = 1;
        }
        else {
            sysml_conditional_15 = 0;
        }
        sysml_arithmetic_16 = ((int32_t)(((double)sysml_state_silenceRun) / 2)) - sysml_conditional_15;
        if ((sysml_state_silenceRun - (2 * sysml_arithmetic_16)) == 0) {
            sysml_conditional_17 = true;
        }
        else {
            sysml_conditional_17 = false;
        }
        sysml_conditional_14 = sysml_conditional_17;
    }
    else {
        sysml_conditional_14 = false;
    }
    sysml_value_0 = sysml_conditional_14;
    int32_t sysml_value_1 = 0;
    sysml_lazy_18 = sysml_state_stage == 4;
    if (sysml_lazy_18) {
        sysml_lazy_18 = !sysml_state_faultLatched;
    }
    if (sysml_lazy_18) {
        sysml_conditional_19 = 1000;
    }
    else {
        sysml_conditional_19 = 0;
    }
    sysml_value_1 = sysml_conditional_19;
    int32_t sysml_value_2 = 0;
    sysml_value_2 = sysml_state_stage;
    bool sysml_value_3 = 0;
    sysml_value_3 = sysml_state_inFault;
    int32_t sysml_value_4 = 0;
    if (sysml_state_inFault) {
        sysml_conditional_20 = 1;
    }
    else {
        sysml_conditional_20 = 0;
    }
    sysml_value_4 = sysml_conditional_20;
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
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 5UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (digitalRead(3) == HIGH);
  }
  if (!hw_attempted_io_4 || (uint32_t)(hw_now - hw_sampled_io_4) >= 5UL) {
    hw_attempted_io_4 = true; hw_sampled_io_4 = hw_now;
    hw_value_io_4 = (digitalRead(2) == HIGH);
  }
  if (!hw_attempted_io_5 || (uint32_t)(hw_now - hw_sampled_io_5) >= 5UL) {
    hw_attempted_io_5 = true; hw_sampled_io_5 = hw_now;
    hw_value_io_5 = (analogRead(A0) * 1 + 0);
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
  if (hw_value_serial_0_trigger) { Serial.print("wash fault"); Serial.println(hw_value_serial_0_value_0); }
}
