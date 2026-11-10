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
extern bool sysml_state_prevVehApproach;
/* Global Variable define of  */
extern bool sysml_state_prevPedApproach;
/* Global Variable define of  */
extern bool sysml_state_inDanger;
/* Global Variable define of  */
extern int32_t sysml_state_dangerStartMs;
/* Global Variable define of  */
extern int32_t sysml_state_lastDangerDurationMs;
/* Global Variable define of  */
extern bool sysml_state_dangerEventReported;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, double io_2, bool io_3, bool* io_4, bool* io_5, int32_t* io_6, int32_t* io_7, bool* io_8, bool* serial_0_trigger, int32_t* serial_0_value_0);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_prevVehApproach = false;
/* Global Variable define of  */
bool sysml_state_prevPedApproach = false;
/* Global Variable define of  */
bool sysml_state_inDanger = false;
/* Global Variable define of  */
int32_t sysml_state_dangerStartMs = 0;
/* Global Variable define of  */
int32_t sysml_state_lastDangerDurationMs = 0;
/* Global Variable define of  */
bool sysml_state_dangerEventReported = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_prevVehApproach = false;
    sysml_state_prevPedApproach = false;
    sysml_state_inDanger = false;
    sysml_state_dangerStartMs = 0;
    sysml_state_lastDangerDurationMs = 0;
    sysml_state_dangerEventReported = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, double io_2, bool io_3, bool* io_4, bool* io_5, int32_t* io_6, int32_t* io_7, bool* io_8, bool* serial_0_trigger, int32_t* serial_0_value_0) {
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
    bool sysml_lazy_9 = 0;
    bool sysml_lazy_10 = 0;
    bool sysml_lazy_11 = 0;
    int32_t sysml_conditional_12 = 0;
    bool sysml_lazy_13 = 0;
    bool sysml_lazy_14 = 0;
    bool sysml_lazy_15 = 0;
    int32_t sysml_conditional_16 = 0;
    bool sysml_lazy_17 = 0;
    bool sysml_lazy_18 = 0;
    bool sysml_lazy_19 = 0;
    int32_t sysml_conditional_20 = 0;
    int32_t sysml_conditional_21 = 0;
    sysml_lazy_0 = io_1;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_0 >= 5.0;
    }
    sysml_lazy_1 = sysml_lazy_0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = io_3;
    }
    sysml_lazy_2 = sysml_lazy_1;
    if (sysml_lazy_2) {
        sysml_lazy_2 = io_2 >= 5.0;
    }
    if (sysml_lazy_2) {
        sysml_lazy_3 = !sysml_state_inDanger;
        if (sysml_lazy_3) {
            sysml_lazy_4 = io_1;
            if (sysml_lazy_4) {
                sysml_lazy_4 = io_0 >= 5.0;
            }
            sysml_lazy_5 = sysml_lazy_4;
            if (sysml_lazy_5) {
                sysml_lazy_6 = io_3;
                if (sysml_lazy_6) {
                    sysml_lazy_6 = io_2 >= 5.0;
                }
                sysml_lazy_5 = sysml_lazy_6;
            }
            sysml_lazy_3 = sysml_lazy_5;
        }
        if (sysml_lazy_3) {
            sysml_state_inDanger = true;
            sysml_state_dangerStartMs = 0;
            sysml_state_dangerEventReported = false;
        }
    }
    sysml_lazy_7 = io_1;
    if (sysml_lazy_7) {
        sysml_lazy_7 = io_0 >= 5.0;
    }
    sysml_lazy_8 = sysml_lazy_7;
    if (sysml_lazy_8) {
        sysml_lazy_8 = io_3;
    }
    sysml_lazy_9 = sysml_lazy_8;
    if (sysml_lazy_9) {
        sysml_lazy_9 = io_2 >= 5.0;
    }
    if (!sysml_lazy_9) {
        if (sysml_state_inDanger) {
            sysml_state_inDanger = false;
            sysml_state_lastDangerDurationMs = sysml_state_dangerStartMs;
            sysml_state_dangerEventReported = true;
        }
    }
    sysml_lazy_10 = io_1;
    if (sysml_lazy_10) {
        sysml_lazy_10 = io_0 >= 5.0;
    }
    sysml_state_prevVehApproach = sysml_lazy_10;
    sysml_lazy_11 = io_3;
    if (sysml_lazy_11) {
        sysml_lazy_11 = io_2 >= 5.0;
    }
    sysml_state_prevPedApproach = sysml_lazy_11;
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_inDanger;
    bool sysml_value_1 = 0;
    sysml_value_1 = (!sysml_state_inDanger);
    int32_t sysml_value_2 = 0;
    if (sysml_state_inDanger) {
        sysml_conditional_12 = 2000;
    }
    else {
        sysml_conditional_12 = 0;
    }
    sysml_value_2 = sysml_conditional_12;
    int32_t sysml_value_3 = 0;
    sysml_lazy_13 = io_1;
    if (sysml_lazy_13) {
        sysml_lazy_13 = io_0 < 20.0;
    }
    sysml_lazy_14 = sysml_lazy_13;
    if (sysml_lazy_14) {
        sysml_lazy_14 = io_3;
    }
    sysml_lazy_15 = sysml_lazy_14;
    if (sysml_lazy_15) {
        sysml_lazy_15 = io_2 < 20.0;
    }
    if (sysml_lazy_15) {
        sysml_conditional_16 = 3;
    }
    else {
        sysml_lazy_17 = io_1;
        if (sysml_lazy_17) {
            sysml_lazy_17 = io_0 < 50.0;
        }
        sysml_lazy_18 = sysml_lazy_17;
        if (sysml_lazy_18) {
            sysml_lazy_18 = io_3;
        }
        sysml_lazy_19 = sysml_lazy_18;
        if (sysml_lazy_19) {
            sysml_lazy_19 = io_2 < 50.0;
        }
        if (sysml_lazy_19) {
            sysml_conditional_20 = 2;
        }
        else {
            if (sysml_state_inDanger) {
                sysml_conditional_21 = 1;
            }
            else {
                sysml_conditional_21 = 0;
            }
            sysml_conditional_20 = sysml_conditional_21;
        }
        sysml_conditional_16 = sysml_conditional_20;
    }
    sysml_value_3 = sysml_conditional_16;
    bool sysml_value_4 = 0;
    sysml_value_4 = false;
    bool sysml_value_5 = 0;
    sysml_value_5 = sysml_state_dangerEventReported;
    int32_t sysml_value_6 = 0;
    sysml_value_6 = sysml_state_lastDangerDurationMs;
    *io_4 = sysml_value_0;
    *io_5 = sysml_value_1;
    *io_6 = sysml_value_2;
    *io_7 = sysml_value_3;
    *io_8 = sysml_value_4;
    *serial_0_trigger = sysml_value_5;
    *serial_0_value_0 = sysml_value_6;
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
static bool hw_value_io_1 = false;
static double hw_value_io_2 = 0;
static bool hw_value_io_3 = false;
static bool hw_value_io_4 = false;
static bool hw_value_io_5 = true;
static int32_t hw_value_io_6 = 0;
static int32_t hw_tone_io_6 = -1;
static int32_t hw_value_io_7 = 0;
TM1637Display hw_display_io_7(4, 5);
static int32_t hw_display_previous_io_7 = INT32_MIN;
static bool hw_value_io_8 = false;
static bool hw_value_serial_0_trigger = false;
static int32_t hw_value_serial_0_value_0 = 0;
static uint32_t hw_ultrasonic_sampled_0 = 0;
static bool hw_ultrasonic_attempted_0 = false;
static uint32_t hw_ultrasonic_sampled_1 = 0;
static bool hw_ultrasonic_attempted_1 = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(12, OUTPUT);
  digitalWrite(12, hw_value_io_4 ? HIGH : LOW);
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
  pinMode(2, OUTPUT);
  digitalWrite(2, hw_value_io_8 ? HIGH : LOW);
  pinMode(8, OUTPUT);
  digitalWrite(8, LOW);
  pinMode(9, INPUT);
  pinMode(10, OUTPUT);
  digitalWrite(10, LOW);
  pinMode(11, INPUT);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
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
  uint32_t hw_ultrasonic_now_1 = (uint32_t)millis();
  if (!hw_ultrasonic_attempted_1 || (uint32_t)(hw_ultrasonic_now_1 - hw_ultrasonic_sampled_1) >= 60UL) {
    hw_ultrasonic_attempted_1 = true; hw_ultrasonic_sampled_1 = hw_ultrasonic_now_1;
    digitalWrite(10, LOW); delayMicroseconds(2);
    digitalWrite(10, HIGH); delayMicroseconds(10); digitalWrite(10, LOW);
    unsigned long hw_ultrasonic_pulse_1 = pulseIn(11, HIGH, 25000UL);
    hw_value_io_3 = (hw_ultrasonic_pulse_1 >= 116UL && hw_ultrasonic_pulse_1 <= 23200UL);
    if (hw_value_io_3) hw_value_io_2 = hw_ultrasonic_pulse_1 / 58.0;
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_io_7, &hw_value_io_8, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0);
  digitalWrite(12, hw_value_io_4 ? HIGH : LOW);
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
  digitalWrite(2, hw_value_io_8 ? HIGH : LOW);
  if (hw_value_serial_0_trigger) { Serial.print("last_danger_duration_ms"); Serial.println(hw_value_serial_0_value_0); }
}
