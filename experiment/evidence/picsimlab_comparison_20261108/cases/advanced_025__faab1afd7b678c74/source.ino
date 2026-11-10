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
extern int32_t sysml_state_stageId;
/* Global Variable define of  */
extern int32_t sysml_state_remainingSeconds;
/* Global Variable define of  */
extern double sysml_state_targetDegC;
/* Global Variable define of  */
extern bool sysml_state_prevButtonD2;
/* Global Variable define of  */
extern bool sysml_state_prevButtonD3;
/* Global Variable define of  */
extern bool sysml_state_prevButtonD4;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, bool io_2, bool io_3, bool* io_4, int32_t* io_5, int32_t* io_6, bool* serial_0_trigger, int32_t* serial_0_value_0, int32_t* serial_0_value_1, double* serial_0_value_2, int32_t* serial_0_value_3);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_stageId = 1;
/* Global Variable define of  */
int32_t sysml_state_remainingSeconds = 0;
/* Global Variable define of  */
double sysml_state_targetDegC = 0.0;
/* Global Variable define of  */
bool sysml_state_prevButtonD2 = false;
/* Global Variable define of  */
bool sysml_state_prevButtonD3 = false;
/* Global Variable define of  */
bool sysml_state_prevButtonD4 = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_stageId = 1;
    sysml_state_remainingSeconds = 0;
    sysml_state_targetDegC = 0.0;
    sysml_state_prevButtonD2 = false;
    sysml_state_prevButtonD3 = false;
    sysml_state_prevButtonD4 = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, bool io_2, bool io_3, bool* io_4, int32_t* io_5, int32_t* io_6, bool* serial_0_trigger, int32_t* serial_0_value_0, int32_t* serial_0_value_1, double* serial_0_value_2, int32_t* serial_0_value_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    int32_t sysml_conditional_2 = 0;
    int32_t sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    int32_t sysml_conditional_5 = 0;
    int32_t sysml_conditional_6 = 0;
    int32_t sysml_conditional_7 = 0;
    int32_t sysml_conditional_8 = 0;
    bool sysml_conditional_9 = 0;
    int32_t sysml_conditional_10 = 0;
    int32_t sysml_conditional_11 = 0;
    bool sysml_conditional_12 = 0;
    sysml_lazy_0 = io_1;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_prevButtonD2;
    }
    if (sysml_lazy_0) {
        sysml_state_stageId = 1;
        sysml_state_remainingSeconds = 0;
        sysml_state_targetDegC = 0.0;
    }
    sysml_lazy_1 = io_2;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_prevButtonD3;
    }
    if (sysml_lazy_1) {
        if (sysml_state_stageId == 1) {
            sysml_conditional_2 = 2;
        }
        else {
            if (sysml_state_stageId == 2) {
                sysml_conditional_3 = 3;
            }
            else {
                sysml_conditional_3 = 1;
            }
            sysml_conditional_2 = sysml_conditional_3;
        }
        sysml_state_stageId = sysml_conditional_2;
        sysml_state_remainingSeconds = 0;
        sysml_state_targetDegC = 0.0;
    }
    sysml_lazy_4 = io_3;
    if (sysml_lazy_4) {
        sysml_lazy_4 = !sysml_state_prevButtonD4;
    }
    if (sysml_lazy_4) {
        if (sysml_state_stageId == 1) {
            sysml_conditional_5 = 3;
        }
        else {
            if (sysml_state_stageId == 3) {
                sysml_conditional_6 = 2;
            }
            else {
                sysml_conditional_6 = 1;
            }
            sysml_conditional_5 = sysml_conditional_6;
        }
        sysml_state_stageId = sysml_conditional_5;
        sysml_state_remainingSeconds = 0;
        sysml_state_targetDegC = 0.0;
    }
    if ((io_0 >= sysml_state_targetDegC)) {
        if (sysml_state_stageId == 1) {
            sysml_conditional_7 = 2;
        }
        else {
            if (sysml_state_stageId == 2) {
                sysml_conditional_8 = 3;
            }
            else {
                sysml_conditional_8 = 1;
            }
            sysml_conditional_7 = sysml_conditional_8;
        }
        sysml_state_stageId = sysml_conditional_7;
        sysml_state_remainingSeconds = 0;
    }
    sysml_state_prevButtonD2 = io_1;
    sysml_state_prevButtonD3 = io_2;
    sysml_state_prevButtonD4 = io_3;
    bool sysml_value_0 = 0;
    if (sysml_state_stageId != 0) {
        sysml_conditional_9 = true;
    }
    else {
        sysml_conditional_9 = false;
    }
    sysml_value_0 = sysml_conditional_9;
    int32_t sysml_value_1 = 0;
    if (io_0 >= sysml_state_targetDegC) {
        sysml_conditional_10 = 440;
    }
    else {
        sysml_conditional_10 = 0;
    }
    sysml_value_1 = sysml_conditional_10;
    int32_t sysml_value_2 = 0;
    if (sysml_state_stageId >= 1) {
        sysml_conditional_11 = sysml_state_stageId;
    }
    else {
        sysml_conditional_11 = 0;
    }
    sysml_value_2 = sysml_conditional_11;
    bool sysml_value_3 = 0;
    if (io_0 >= sysml_state_targetDegC) {
        sysml_conditional_12 = true;
    }
    else {
        sysml_conditional_12 = false;
    }
    sysml_value_3 = sysml_conditional_12;
    int32_t sysml_value_4 = 0;
    sysml_value_4 = 0;
    int32_t sysml_value_5 = 0;
    sysml_value_5 = sysml_state_stageId;
    double sysml_value_6 = 0;
    sysml_value_6 = io_0;
    int32_t sysml_value_7 = 0;
    sysml_value_7 = sysml_state_remainingSeconds;
    *io_4 = sysml_value_0;
    *io_5 = sysml_value_1;
    *io_6 = sysml_value_2;
    *serial_0_trigger = sysml_value_3;
    *serial_0_value_0 = sysml_value_4;
    *serial_0_value_1 = sysml_value_5;
    *serial_0_value_2 = sysml_value_6;
    *serial_0_value_3 = sysml_value_7;
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
static int32_t hw_value_io_5 = 0;
static int32_t hw_tone_io_5 = -1;
static int32_t hw_value_io_6 = 0;
TM1637Display hw_display_io_6(7, 5);
static int32_t hw_display_previous_io_6 = INT32_MIN;
static bool hw_value_serial_0_trigger = false;
static int32_t hw_value_serial_0_value_0 = 0;
static int32_t hw_value_serial_0_value_1 = 0;
static double hw_value_serial_0_value_2 = 0;
static int32_t hw_value_serial_0_value_3 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(A1, INPUT);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_4 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  int32_t hw_tone_command_io_5 = (hw_value_io_5 >= 31 && hw_value_io_5 <= 65535) ? hw_value_io_5 : 0;
  if (hw_tone_command_io_5 != hw_tone_io_5) {
    if (hw_tone_command_io_5 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_5);
    hw_tone_io_5 = hw_tone_command_io_5;
  }
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
    hw_value_io_1 = (digitalRead(2) == HIGH);
  }
  if (!hw_attempted_io_2 || (uint32_t)(hw_now - hw_sampled_io_2) >= 10UL) {
    hw_attempted_io_2 = true; hw_sampled_io_2 = hw_now;
    hw_value_io_2 = (digitalRead(3) == HIGH);
  }
  if (!hw_attempted_io_3 || (uint32_t)(hw_now - hw_sampled_io_3) >= 10UL) {
    hw_attempted_io_3 = true; hw_sampled_io_3 = hw_now;
    hw_value_io_3 = (digitalRead(4) == HIGH);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0, &hw_value_serial_0_value_1, &hw_value_serial_0_value_2, &hw_value_serial_0_value_3);
  digitalWrite(13, hw_value_io_4 ? HIGH : LOW);
  int32_t hw_tone_command_io_5 = (hw_value_io_5 >= 31 && hw_value_io_5 <= 65535) ? hw_value_io_5 : 0;
  if (hw_tone_command_io_5 != hw_tone_io_5) {
    if (hw_tone_command_io_5 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_5);
    hw_tone_io_5 = hw_tone_command_io_5;
  }
  if (hw_value_io_6 != hw_display_previous_io_6) {
    if (hw_value_io_6 < -999 || hw_value_io_6 > 9999) hw_display_io_6.clear();
    else hw_display_io_6.showNumberDec((int)hw_value_io_6, false, 4, 0);
    hw_display_previous_io_6 = hw_value_io_6;
  }
  if (hw_value_serial_0_trigger) { Serial.print("elapsed_ms_delta"); Serial.println(hw_value_serial_0_value_0); }
  if (hw_value_serial_0_trigger) { Serial.print("stage_id"); Serial.println(hw_value_serial_0_value_1); }
  if (hw_value_serial_0_trigger) { Serial.print("temperature_degC"); Serial.println(hw_value_serial_0_value_2); }
  if (hw_value_serial_0_trigger) { Serial.print("remaining_seconds"); Serial.println(hw_value_serial_0_value_3); }
}
