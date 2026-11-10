#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_015\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
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
extern int32_t sysml_state_todayUsageCount;
/* Global Variable define of  */
extern bool sysml_state_previousNear;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, double io_2, bool io_3, bool* io_1, int32_t* io_4, int32_t* io_5, bool* io_6, bool* serial_0_trigger, int32_t* serial_0_value_0, double* serial_0_value_1, bool* serial_0_value_2);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_todayUsageCount = 0;
/* Global Variable define of  */
bool sysml_state_previousNear = false;
/* Init function of CompositeActor named "ProgramPage1" */
#line 273 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_015\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void setup();
#line 299 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_015\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void loop();
#line 33 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_015\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_todayUsageCount = 0;
    sysml_state_previousNear = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, double io_2, bool io_3, bool* io_1, int32_t* io_4, int32_t* io_5, bool* io_6, bool* serial_0_trigger, int32_t* serial_0_value_0, double* serial_0_value_1, bool* serial_0_value_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    int32_t sysml_conditional_7 = 0;
    int32_t sysml_conditional_8 = 0;
    int32_t sysml_conditional_9 = 0;
    int32_t sysml_conditional_10 = 0;
    int32_t sysml_conditional_11 = 0;
    int32_t sysml_conditional_12 = 0;
    bool sysml_lazy_13 = 0;
    int32_t sysml_conditional_14 = 0;
    bool sysml_lazy_15 = 0;
    bool sysml_lazy_16 = 0;
    int32_t sysml_conditional_17 = 0;
    bool sysml_lazy_18 = 0;
    bool sysml_lazy_19 = 0;
    bool sysml_conditional_20 = 0;
    bool sysml_lazy_21 = 0;
    bool sysml_lazy_22 = 0;
    bool sysml_conditional_23 = 0;
    int32_t sysml_conditional_24 = 0;
    bool sysml_lazy_25 = 0;
    double sysml_conditional_26 = 0;
    double sysml_conditional_27 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousNear;
    }
    if (sysml_lazy_0) {
        sysml_state_previousNear = io_0;
    }
    sysml_lazy_1 = io_0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_previousNear;
    }
    if (sysml_lazy_1) {
        sysml_lazy_2 = io_3;
        if (sysml_lazy_2) {
            sysml_lazy_2 = io_2 >= 0.0;
        }
        if (sysml_lazy_2) {
            sysml_state_todayUsageCount = (sysml_state_todayUsageCount + 1);
        }
    }
    sysml_state_previousNear = io_0;
    bool sysml_value_0 = 0;
    sysml_lazy_3 = io_0;
    if (sysml_lazy_3) {
        sysml_lazy_3 = io_3;
    }
    sysml_lazy_4 = sysml_lazy_3;
    if (sysml_lazy_4) {
        sysml_lazy_4 = io_2 >= 20.0;
    }
    if (sysml_lazy_4) {
        sysml_conditional_5 = true;
    }
    else {
        sysml_conditional_5 = false;
    }
    sysml_value_0 = sysml_conditional_5;
    int32_t sysml_value_1 = 0;
    sysml_lazy_6 = io_3;
    if (sysml_lazy_6) {
        sysml_lazy_6 = io_2 >= 2.0;
    }
    if (sysml_lazy_6) {
        if (io_2 <= 400.0) {
            if ((((double)(400.0 - io_2)) / 3.98) < ((double)((int32_t)(((double)(400.0 - io_2)) / 3.98)))) {
                sysml_conditional_9 = 1;
            }
            else {
                sysml_conditional_9 = 0;
            }
            if ((((double)(400.0 - io_2)) / 3.98) < ((double)((int32_t)(((double)(400.0 - io_2)) / 3.98)))) {
                sysml_conditional_10 = 1;
            }
            else {
                sysml_conditional_10 = 0;
            }
            if ((((double)(400.0 - io_2)) / 3.98) < ((double)((int32_t)(((double)(400.0 - io_2)) / 3.98)))) {
                sysml_conditional_11 = 1;
            }
            else {
                sysml_conditional_11 = 0;
            }
            if ((100 - (((int32_t)(((double)(400.0 - io_2)) / 3.98)) - sysml_conditional_10)) < ((double)((int32_t)(100 - (((int32_t)(((double)(400.0 - io_2)) / 3.98)) - sysml_conditional_11))))) {
                sysml_conditional_12 = 1;
            }
            else {
                sysml_conditional_12 = 0;
            }
            sysml_conditional_8 = ((int32_t)(100 - (((int32_t)(((double)(400.0 - io_2)) / 3.98)) - sysml_conditional_9))) - sysml_conditional_12;
        }
        else {
            sysml_conditional_8 = 0;
        }
        sysml_conditional_7 = sysml_conditional_8;
    }
    else {
        sysml_conditional_7 = 0;
    }
    sysml_value_1 = sysml_conditional_7;
    int32_t sysml_value_2 = 0;
    sysml_lazy_13 = io_0;
    if (sysml_lazy_13) {
        sysml_lazy_13 = !io_3;
    }
    if (sysml_lazy_13) {
        sysml_conditional_14 = 0;
    }
    else {
        sysml_lazy_15 = io_0;
        if (sysml_lazy_15) {
            sysml_lazy_15 = io_3;
        }
        sysml_lazy_16 = sysml_lazy_15;
        if (sysml_lazy_16) {
            sysml_lazy_16 = io_2 < 20.0;
        }
        if (sysml_lazy_16) {
            sysml_conditional_17 = 2000;
        }
        else {
            sysml_conditional_17 = 0;
        }
        sysml_conditional_14 = sysml_conditional_17;
    }
    sysml_value_2 = sysml_conditional_14;
    bool sysml_value_3 = 0;
    sysml_lazy_18 = io_0;
    if (sysml_lazy_18) {
        sysml_lazy_18 = io_3;
    }
    sysml_lazy_19 = sysml_lazy_18;
    if (sysml_lazy_19) {
        sysml_lazy_19 = io_2 >= 20.0;
    }
    if (sysml_lazy_19) {
        sysml_conditional_20 = true;
    }
    else {
        sysml_lazy_21 = io_0;
        if (sysml_lazy_21) {
            sysml_lazy_21 = io_3;
        }
        sysml_lazy_22 = sysml_lazy_21;
        if (sysml_lazy_22) {
            sysml_lazy_22 = io_2 < 20.0;
        }
        if (sysml_lazy_22) {
            sysml_conditional_23 = true;
        }
        else {
            sysml_conditional_23 = false;
        }
        sysml_conditional_20 = sysml_conditional_23;
    }
    sysml_value_3 = sysml_conditional_20;
    bool sysml_value_4 = 0;
    sysml_value_4 = io_0;
    int32_t sysml_value_5 = 0;
    if (io_0) {
        sysml_conditional_24 = sysml_state_todayUsageCount;
    }
    else {
        sysml_conditional_24 = sysml_state_todayUsageCount;
    }
    sysml_value_5 = sysml_conditional_24;
    double sysml_value_6 = 0;
    sysml_lazy_25 = io_3;
    if (sysml_lazy_25) {
        sysml_lazy_25 = io_2 >= 2.0;
    }
    if (sysml_lazy_25) {
        if (io_2 <= 400.0) {
            sysml_conditional_27 = ((double)(400.0 - io_2)) / 3.98;
        }
        else {
            sysml_conditional_27 = 0.0;
        }
        sysml_conditional_26 = sysml_conditional_27;
    }
    else {
        sysml_conditional_26 = 0.0;
    }
    sysml_value_6 = sysml_conditional_26;
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

