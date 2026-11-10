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
extern bool sysml_state_approachActive;
/* Global Variable define of  */
extern int32_t sysml_state_inactivityMs;
/* Global Variable define of  */
extern int32_t sysml_state_flashPhaseMs;
/* Global Variable define of  */
extern int32_t sysml_state_beepSequenceMs;
/* Global Variable define of  */
extern bool sysml_state_welcomePulse;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, int32_t io_5, bool* io_2, int32_t* io_3, bool* io_4);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_approachActive = false;
/* Global Variable define of  */
int32_t sysml_state_inactivityMs = 0;
/* Global Variable define of  */
int32_t sysml_state_flashPhaseMs = 0;
/* Global Variable define of  */
int32_t sysml_state_beepSequenceMs = 0;
/* Global Variable define of  */
bool sysml_state_welcomePulse = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_approachActive = false;
    sysml_state_inactivityMs = 0;
    sysml_state_flashPhaseMs = 0;
    sysml_state_beepSequenceMs = 0;
    sysml_state_welcomePulse = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, int32_t io_5, bool* io_2, int32_t* io_3, bool* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int32_t sysml_conditional_0 = 0;
    int32_t sysml_conditional_1 = 0;
    int32_t sysml_arithmetic_2 = 0;
    int32_t sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_lazy_10 = 0;
    bool sysml_lazy_11 = 0;
    bool sysml_lazy_12 = 0;
    bool sysml_lazy_13 = 0;
    bool sysml_lazy_14 = 0;
    int32_t sysml_conditional_15 = 0;
    sysml_state_welcomePulse = false;
    if (sysml_state_approachActive) {
        if (io_5 >= (5000 - sysml_state_inactivityMs)) {
            sysml_conditional_0 = 5000;
        }
        else {
            sysml_conditional_0 = sysml_state_inactivityMs + io_5;
        }
        sysml_state_inactivityMs = sysml_conditional_0;
        if ((((double)io_5) / 200) < ((double)((int32_t)(((double)io_5) / 200)))) {
            sysml_conditional_1 = 1;
        }
        else {
            sysml_conditional_1 = 0;
        }
        sysml_arithmetic_2 = ((int32_t)(((double)io_5) / 200)) - sysml_conditional_1;
        sysml_state_flashPhaseMs = sysml_state_flashPhaseMs + (io_5 - (200 * sysml_arithmetic_2));
        if ((sysml_state_flashPhaseMs >= 200)) {
            sysml_state_flashPhaseMs = (sysml_state_flashPhaseMs - 200);
        }
        if (io_5 >= (500 - sysml_state_beepSequenceMs)) {
            sysml_conditional_3 = 500;
        }
        else {
            sysml_conditional_3 = sysml_state_beepSequenceMs + io_5;
        }
        sysml_state_beepSequenceMs = sysml_conditional_3;
    }
    sysml_lazy_4 = io_1;
    if (sysml_lazy_4) {
        sysml_lazy_4 = io_0 >= 2.0;
    }
    sysml_lazy_5 = sysml_lazy_4;
    if (sysml_lazy_5) {
        sysml_lazy_5 = io_0 <= 10.0;
    }
    if (sysml_lazy_5) {
        sysml_state_inactivityMs = 0;
        if ((!sysml_state_approachActive)) {
            sysml_state_approachActive = true;
            sysml_state_flashPhaseMs = 0;
            sysml_state_beepSequenceMs = 0;
            sysml_state_welcomePulse = true;
        }
    }
    else {
        sysml_lazy_6 = sysml_state_approachActive;
        if (sysml_lazy_6) {
            sysml_lazy_6 = sysml_state_inactivityMs >= 5000;
        }
        if (sysml_lazy_6) {
            sysml_state_approachActive = false;
            sysml_state_flashPhaseMs = 0;
            sysml_state_beepSequenceMs = 0;
        }
    }
    bool sysml_value_0 = 0;
    sysml_lazy_7 = io_1;
    if (sysml_lazy_7) {
        sysml_lazy_7 = io_0 >= 2.0;
    }
    sysml_lazy_8 = sysml_lazy_7;
    if (sysml_lazy_8) {
        sysml_lazy_8 = io_0 <= 10.0;
    }
    sysml_value_0 = sysml_lazy_8;
    bool sysml_value_1 = 0;
    sysml_lazy_9 = sysml_state_approachActive;
    if (sysml_lazy_9) {
        sysml_lazy_9 = sysml_state_flashPhaseMs < 100;
    }
    sysml_value_1 = sysml_lazy_9;
    int32_t sysml_value_2 = 0;
    sysml_lazy_10 = sysml_state_approachActive;
    if (sysml_lazy_10) {
        sysml_lazy_11 = sysml_state_beepSequenceMs < 100;
        if (!sysml_lazy_11) {
            sysml_lazy_12 = sysml_state_beepSequenceMs >= 200;
            if (sysml_lazy_12) {
                sysml_lazy_12 = sysml_state_beepSequenceMs < 300;
            }
            sysml_lazy_11 = sysml_lazy_12;
        }
        sysml_lazy_13 = sysml_lazy_11;
        if (!sysml_lazy_13) {
            sysml_lazy_14 = sysml_state_beepSequenceMs >= 400;
            if (sysml_lazy_14) {
                sysml_lazy_14 = sysml_state_beepSequenceMs < 500;
            }
            sysml_lazy_13 = sysml_lazy_14;
        }
        sysml_lazy_10 = sysml_lazy_13;
    }
    if (sysml_lazy_10) {
        sysml_conditional_15 = 2000;
    }
    else {
        sysml_conditional_15 = 0;
    }
    sysml_value_2 = sysml_conditional_15;
    bool sysml_value_3 = 0;
    sysml_value_3 = sysml_state_welcomePulse;
    *io_2 = sysml_value_1;
    *io_3 = sysml_value_2;
    *io_4 = sysml_value_3;
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
static bool hw_value_io_1 = false;
static bool hw_value_io_2 = false;
static int32_t hw_value_io_3 = 0;
static int32_t hw_tone_io_3 = -1;
static bool hw_value_io_4 = false;
static int32_t hw_value_io_5 = 0;
static uint32_t hw_ultrasonic_sampled_0 = 0;
static bool hw_ultrasonic_attempted_0 = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_2 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  int32_t hw_tone_command_io_3 = (hw_value_io_3 >= 31 && hw_value_io_3 <= 65535) ? hw_value_io_3 : 0;
  if (hw_tone_command_io_3 != hw_tone_io_3) {
    if (hw_tone_command_io_3 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_3);
    hw_tone_io_3 = hw_tone_command_io_3;
  }
  pinMode(8, OUTPUT);
  digitalWrite(8, LOW);
  pinMode(9, INPUT);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  uint32_t hw_ultrasonic_now_0 = (uint32_t)millis();
  if (!hw_ultrasonic_attempted_0 || (uint32_t)(hw_ultrasonic_now_0 - hw_ultrasonic_sampled_0) >= 60UL) {
    hw_ultrasonic_attempted_0 = true; hw_ultrasonic_sampled_0 = hw_ultrasonic_now_0;
    digitalWrite(8, LOW); delayMicroseconds(2);
    digitalWrite(8, HIGH); delayMicroseconds(10); digitalWrite(8, LOW);
    unsigned long hw_ultrasonic_pulse_0 = pulseIn(9, HIGH, 25000UL);
    hw_value_io_1 = (hw_ultrasonic_pulse_0 >= 116UL && hw_ultrasonic_pulse_0 <= 23200UL);
    if (hw_value_io_1) hw_value_io_0 = hw_ultrasonic_pulse_0 / 58.0;
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_5 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_5, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4);
  digitalWrite(13, hw_value_io_2 ? HIGH : LOW);
  int32_t hw_tone_command_io_3 = (hw_value_io_3 >= 31 && hw_value_io_3 <= 65535) ? hw_value_io_3 : 0;
  if (hw_tone_command_io_3 != hw_tone_io_3) {
    if (hw_tone_command_io_3 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_3);
    hw_tone_io_3 = hw_tone_command_io_3;
  }
  if (hw_value_io_4) Serial.println("welcome");
}
