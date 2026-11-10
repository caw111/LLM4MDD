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
extern bool sysml_state_previousNear;
/* Global Variable define of  */
extern int32_t sysml_state_todayUsageCount;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, double io_2, bool io_3, bool* io_1, int32_t* io_4, int32_t* io_5, bool* io_6, bool* serial_0_trigger, int32_t* serial_0_value_0, double* serial_0_value_1, bool* serial_0_value_2);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousNear = false;
/* Global Variable define of  */
int32_t sysml_state_todayUsageCount = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousNear = false;
    sysml_state_todayUsageCount = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, double io_2, bool io_3, bool* io_1, int32_t* io_4, int32_t* io_5, bool* io_6, bool* serial_0_trigger, int32_t* serial_0_value_0, double* serial_0_value_1, bool* serial_0_value_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_conditional_2 = 0;
    bool sysml_lazy_3 = 0;
    int32_t sysml_conditional_4 = 0;
    int32_t sysml_conditional_5 = 0;
    int32_t sysml_conditional_6 = 0;
    int32_t sysml_conditional_7 = 0;
    bool sysml_lazy_8 = 0;
    bool sysml_lazy_9 = 0;
    int32_t sysml_conditional_10 = 0;
    bool sysml_lazy_11 = 0;
    bool sysml_lazy_12 = 0;
    bool sysml_conditional_13 = 0;
    bool sysml_conditional_14 = 0;
    double sysml_conditional_15 = 0;
    double sysml_conditional_16 = 0;
    double sysml_conditional_17 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousNear;
    }
    if (sysml_lazy_0) {
        sysml_state_todayUsageCount = (sysml_state_todayUsageCount + 1);
    }
    sysml_state_previousNear = io_0;
    bool sysml_value_0 = 0;
    sysml_lazy_1 = io_0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = io_3;
    }
    if (sysml_lazy_1) {
        sysml_lazy_3 = io_2 >= 10.0;
        if (sysml_lazy_3) {
            sysml_lazy_3 = io_3;
        }
        sysml_conditional_2 = sysml_lazy_3;
    }
    else {
        sysml_conditional_2 = false;
    }
    sysml_value_0 = sysml_conditional_2;
    int32_t sysml_value_1 = 0;
    if (io_3) {
        if (io_2 < 2.0) {
            sysml_conditional_5 = 0;
        }
        else {
            if (io_2 >= 400.0) {
                sysml_conditional_6 = 100;
            }
            else {
                if ((100 - (((double)(400.0 - io_2)) / 3.98)) < ((double)((int32_t)(100 - (((double)(400.0 - io_2)) / 3.98))))) {
                    sysml_conditional_7 = 1;
                }
                else {
                    sysml_conditional_7 = 0;
                }
                sysml_conditional_6 = ((int32_t)(100 - (((double)(400.0 - io_2)) / 3.98))) - sysml_conditional_7;
            }
            sysml_conditional_5 = sysml_conditional_6;
        }
        sysml_conditional_4 = sysml_conditional_5;
    }
    else {
        sysml_conditional_4 = 0;
    }
    sysml_value_1 = sysml_conditional_4;
    int32_t sysml_value_2 = 0;
    sysml_lazy_8 = io_0;
    if (sysml_lazy_8) {
        sysml_lazy_8 = io_3;
    }
    sysml_lazy_9 = sysml_lazy_8;
    if (sysml_lazy_9) {
        sysml_lazy_9 = io_2 < 10.0;
    }
    if (sysml_lazy_9) {
        sysml_conditional_10 = 2000;
    }
    else {
        sysml_conditional_10 = 0;
    }
    sysml_value_2 = sysml_conditional_10;
    bool sysml_value_3 = 0;
    sysml_lazy_11 = io_0;
    if (sysml_lazy_11) {
        sysml_lazy_11 = io_3;
    }
    sysml_lazy_12 = sysml_lazy_11;
    if (sysml_lazy_12) {
        sysml_lazy_12 = io_2 >= 10.0;
    }
    if (sysml_lazy_12) {
        sysml_conditional_13 = true;
    }
    else {
        sysml_conditional_13 = false;
    }
    sysml_value_3 = sysml_conditional_13;
    bool sysml_value_4 = 0;
    if (false) {
        sysml_conditional_14 = true;
    }
    else {
        sysml_conditional_14 = false;
    }
    sysml_value_4 = sysml_conditional_14;
    int32_t sysml_value_5 = 0;
    sysml_value_5 = sysml_state_todayUsageCount;
    double sysml_value_6 = 0;
    if (io_3) {
        if (io_2 < 2.0) {
            sysml_conditional_16 = 0.0;
        }
        else {
            if (io_2 >= 400.0) {
                sysml_conditional_17 = 100.0;
            }
            else {
                sysml_conditional_17 = 100.0 - (((double)(400.0 - io_2)) / 3.98);
            }
            sysml_conditional_16 = sysml_conditional_17;
        }
        sysml_conditional_15 = sysml_conditional_16;
    }
    else {
        sysml_conditional_15 = 0.0;
    }
    sysml_value_6 = sysml_conditional_15;
    bool sysml_value_7 = 0;
    sysml_value_7 = io_3;
    *io_1 = sysml_value_0;
    *io_4 = sysml_value_1;
    *io_5 = sysml_value_2;
    *io_6 = sysml_value_3;
    *serial_0_trigger = sysml_value_4;
    *serial_0_value_0 = sysml_value_5;
    *serial_0_value_1 = sysml_value_6;
    *serial_0_value_2 = sysml_value_7;
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
static double hw_value_io_2 = 0;
static bool hw_value_io_3 = false;
static int32_t hw_value_io_4 = 0;
TM1637Display hw_display_io_4(4, 5);
static int32_t hw_display_previous_io_4 = INT32_MIN;
static int32_t hw_value_io_5 = 0;
static int32_t hw_tone_io_5 = -1;
static bool hw_value_io_6 = false;
static bool hw_value_serial_0_trigger = false;
static int32_t hw_value_serial_0_value_0 = 0;
static double hw_value_serial_0_value_1 = 0;
static bool hw_value_serial_0_value_2 = false;
static uint32_t hw_ultrasonic_sampled_0 = 0;
static bool hw_ultrasonic_attempted_0 = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(2, INPUT);
  pinMode(10, OUTPUT);
  digitalWrite(10, hw_value_io_1 ? HIGH : LOW);
  hw_display_io_4.setBrightness(7, true);
  if (hw_value_io_4 != hw_display_previous_io_4) {
    if (hw_value_io_4 < -999 || hw_value_io_4 > 9999) hw_display_io_4.clear();
    else hw_display_io_4.showNumberDec((int)hw_value_io_4, false, 4, 0);
    hw_display_previous_io_4 = hw_value_io_4;
  }
  pinMode(6, OUTPUT);
  int32_t hw_tone_command_io_5 = (hw_value_io_5 >= 31 && hw_value_io_5 <= 65535) ? hw_value_io_5 : 0;
  if (hw_tone_command_io_5 != hw_tone_io_5) {
    if (hw_tone_command_io_5 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_5);
    hw_tone_io_5 = hw_tone_command_io_5;
  }
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_6 ? HIGH : LOW);
  pinMode(8, OUTPUT);
  digitalWrite(8, LOW);
  pinMode(9, INPUT);
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
  uint32_t hw_ultrasonic_now_0 = (uint32_t)millis();
  if (!hw_ultrasonic_attempted_0 || (uint32_t)(hw_ultrasonic_now_0 - hw_ultrasonic_sampled_0) >= 60UL) {
    hw_ultrasonic_attempted_0 = true; hw_ultrasonic_sampled_0 = hw_ultrasonic_now_0;
    digitalWrite(8, LOW); delayMicroseconds(2);
    digitalWrite(8, HIGH); delayMicroseconds(10); digitalWrite(8, LOW);
    unsigned long hw_ultrasonic_pulse_0 = pulseIn(9, HIGH, 30000UL);
    hw_value_io_3 = (hw_ultrasonic_pulse_0 >= 116UL && hw_ultrasonic_pulse_0 <= 23200UL);
    if (hw_value_io_3) hw_value_io_2 = hw_ultrasonic_pulse_0 / 58.0;
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_2, hw_value_io_3, &hw_value_io_1, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0, &hw_value_serial_0_value_1, &hw_value_serial_0_value_2);
  digitalWrite(10, hw_value_io_1 ? HIGH : LOW);
  if (hw_value_io_4 != hw_display_previous_io_4) {
    if (hw_value_io_4 < -999 || hw_value_io_4 > 9999) hw_display_io_4.clear();
    else hw_display_io_4.showNumberDec((int)hw_value_io_4, false, 4, 0);
    hw_display_previous_io_4 = hw_value_io_4;
  }
  int32_t hw_tone_command_io_5 = (hw_value_io_5 >= 31 && hw_value_io_5 <= 65535) ? hw_value_io_5 : 0;
  if (hw_tone_command_io_5 != hw_tone_io_5) {
    if (hw_tone_command_io_5 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_5);
    hw_tone_io_5 = hw_tone_command_io_5;
  }
  digitalWrite(13, hw_value_io_6 ? HIGH : LOW);
  if (hw_value_serial_0_trigger) { Serial.print("today_usage_count"); Serial.println(hw_value_serial_0_value_0); }
  if (hw_value_serial_0_trigger) { Serial.print("remaining_water_percent"); Serial.println(hw_value_serial_0_value_1); }
  if (hw_value_serial_0_trigger) { Serial.print("water_level_valid"); Serial.println(hw_value_serial_0_value_2); }
}
