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
extern int32_t sysml_state_mode;
/* Global Variable define of  */
extern int32_t sysml_state_occupancyCount;
/* Global Variable define of  */
extern bool sysml_state_prevParkingValid;
/* Global Variable define of  */
extern bool sysml_state_prevEntranceApproach;
/* Global Variable define of  */
extern bool sysml_state_prevReverseAlarmCond;
/* Global Variable define of  */
extern bool sysml_state_lastAlarmOn;
/* Global Variable define of  */
extern bool sysml_state_prevPressed;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, double io_4, bool io_5, double io_6, bool io_7, bool* io_1, bool* io_2, int32_t* io_3, int32_t* io_8);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_mode = 0;
/* Global Variable define of  */
int32_t sysml_state_occupancyCount = 0;
/* Global Variable define of  */
bool sysml_state_prevParkingValid = false;
/* Global Variable define of  */
bool sysml_state_prevEntranceApproach = false;
/* Global Variable define of  */
bool sysml_state_prevReverseAlarmCond = false;
/* Global Variable define of  */
bool sysml_state_lastAlarmOn = false;
/* Global Variable define of  */
bool sysml_state_prevPressed = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_mode = 0;
    sysml_state_occupancyCount = 0;
    sysml_state_prevParkingValid = false;
    sysml_state_prevEntranceApproach = false;
    sysml_state_prevReverseAlarmCond = false;
    sysml_state_lastAlarmOn = false;
    sysml_state_prevPressed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, double io_4, bool io_5, double io_6, bool io_7, bool* io_1, bool* io_2, int32_t* io_3, int32_t* io_8) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int32_t sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    int32_t sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_lazy_10 = 0;
    int32_t sysml_conditional_11 = 0;
    bool sysml_lazy_12 = 0;
    bool sysml_lazy_13 = 0;
    int32_t sysml_conditional_14 = 0;
    bool sysml_lazy_15 = 0;
    bool sysml_lazy_16 = 0;
    int32_t sysml_conditional_17 = 0;
    bool sysml_lazy_18 = 0;
    int32_t sysml_conditional_19 = 0;
    bool sysml_lazy_20 = 0;
    int32_t sysml_conditional_21 = 0;
    int32_t sysml_conditional_22 = 0;
    int32_t sysml_conditional_23 = 0;
    bool sysml_lazy_24 = 0;
    int32_t sysml_conditional_25 = 0;
    int32_t sysml_conditional_26 = 0;
    int32_t sysml_conditional_27 = 0;
    int32_t sysml_conditional_28 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_prevPressed;
    }
    if (sysml_lazy_0) {
        if (sysml_state_mode == 2) {
            sysml_conditional_1 = 0;
        }
        else {
            sysml_conditional_1 = sysml_state_mode + 1;
        }
        sysml_state_mode = sysml_conditional_1;
    }
    sysml_state_prevPressed = io_0;
    sysml_lazy_2 = io_7;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_prevParkingValid;
    }
    if (sysml_lazy_2) {
        if (sysml_state_occupancyCount < 2147483647) {
            sysml_conditional_3 = sysml_state_occupancyCount + 1;
        }
        else {
            sysml_conditional_3 = 2147483647;
        }
        sysml_state_occupancyCount = sysml_conditional_3;
    }
    sysml_state_prevParkingValid = io_7;
    sysml_lazy_4 = io_5;
    if (sysml_lazy_4) {
        sysml_lazy_4 = !sysml_state_prevEntranceApproach;
    }
    if (sysml_lazy_4) {
        sysml_state_prevEntranceApproach = true;
    }
    else {
        sysml_state_prevEntranceApproach = io_5;
    }
    sysml_lazy_5 = io_7;
    if (sysml_lazy_5) {
        sysml_lazy_5 = io_6 < 5.0;
    }
    sysml_state_prevReverseAlarmCond = sysml_lazy_5;
    sysml_lazy_6 = io_7;
    if (sysml_lazy_6) {
        sysml_lazy_6 = io_6 < 5.0;
    }
    sysml_state_lastAlarmOn = sysml_lazy_6;
    bool sysml_value_0 = 0;
    sysml_lazy_7 = !io_7;
    if (!sysml_lazy_7) {
        sysml_lazy_7 = io_6 >= 5.0;
    }
    sysml_lazy_8 = sysml_lazy_7;
    if (sysml_lazy_8) {
        sysml_lazy_8 = io_5;
    }
    sysml_value_0 = sysml_lazy_8;
    bool sysml_value_1 = 0;
    sysml_lazy_9 = io_7;
    if (sysml_lazy_9) {
        sysml_lazy_9 = io_6 >= 5.0;
    }
    sysml_value_1 = sysml_lazy_9;
    int32_t sysml_value_2 = 0;
    sysml_lazy_10 = io_7;
    if (sysml_lazy_10) {
        sysml_lazy_10 = io_6 < 5.0;
    }
    if (sysml_lazy_10) {
        sysml_conditional_11 = 2000;
    }
    else {
        sysml_lazy_12 = io_7;
        if (sysml_lazy_12) {
            sysml_lazy_12 = io_6 >= 2.0;
        }
        sysml_lazy_13 = sysml_lazy_12;
        if (sysml_lazy_13) {
            sysml_lazy_13 = io_6 < 10.0;
        }
        if (sysml_lazy_13) {
            sysml_conditional_14 = 1200;
        }
        else {
            sysml_lazy_15 = io_7;
            if (sysml_lazy_15) {
                sysml_lazy_15 = io_6 >= 10.0;
            }
            sysml_lazy_16 = sysml_lazy_15;
            if (sysml_lazy_16) {
                sysml_lazy_16 = io_6 < 30.0;
            }
            if (sysml_lazy_16) {
                sysml_conditional_17 = 800;
            }
            else {
                sysml_lazy_18 = io_7;
                if (sysml_lazy_18) {
                    sysml_lazy_18 = io_6 >= 30.0;
                }
                if (sysml_lazy_18) {
                    sysml_conditional_19 = 400;
                }
                else {
                    sysml_conditional_19 = 0;
                }
                sysml_conditional_17 = sysml_conditional_19;
            }
            sysml_conditional_14 = sysml_conditional_17;
        }
        sysml_conditional_11 = sysml_conditional_14;
    }
    sysml_value_2 = sysml_conditional_11;
    int32_t sysml_value_3 = 0;
    sysml_lazy_20 = sysml_state_mode == 0;
    if (sysml_lazy_20) {
        sysml_lazy_20 = io_5;
    }
    if (sysml_lazy_20) {
        if (io_4 >= 0.0) {
            if (io_4 < ((double)((int32_t)io_4))) {
                sysml_conditional_23 = 1;
            }
            else {
                sysml_conditional_23 = 0;
            }
            sysml_conditional_22 = ((int32_t)io_4) - sysml_conditional_23;
        }
        else {
            sysml_conditional_22 = 0;
        }
        sysml_conditional_21 = sysml_conditional_22;
    }
    else {
        sysml_lazy_24 = sysml_state_mode == 1;
        if (sysml_lazy_24) {
            sysml_lazy_24 = io_7;
        }
        if (sysml_lazy_24) {
            if (io_6 >= 0.0) {
                if (io_6 < ((double)((int32_t)io_6))) {
                    sysml_conditional_27 = 1;
                }
                else {
                    sysml_conditional_27 = 0;
                }
                sysml_conditional_26 = ((int32_t)io_6) - sysml_conditional_27;
            }
            else {
                sysml_conditional_26 = 0;
            }
            sysml_conditional_25 = sysml_conditional_26;
        }
        else {
            if (sysml_state_mode == 2) {
                sysml_conditional_28 = sysml_state_occupancyCount;
            }
            else {
                sysml_conditional_28 = 0;
            }
            sysml_conditional_25 = sysml_conditional_28;
        }
        sysml_conditional_21 = sysml_conditional_25;
    }
    sysml_value_3 = sysml_conditional_21;
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
    *io_3 = sysml_value_2;
    *io_8 = sysml_value_3;
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
static bool hw_value_io_2 = false;
static int32_t hw_value_io_3 = 0;
static int32_t hw_tone_io_3 = -1;
static double hw_value_io_4 = 0;
static bool hw_value_io_5 = false;
static double hw_value_io_6 = 0;
static bool hw_value_io_7 = false;
static int32_t hw_value_io_8 = 0;
TM1637Display hw_display_io_8(4, 5);
static int32_t hw_display_previous_io_8 = INT32_MIN;
static uint32_t hw_ultrasonic_sampled_0 = 0;
static bool hw_ultrasonic_attempted_0 = false;
static uint32_t hw_ultrasonic_sampled_1 = 0;
static bool hw_ultrasonic_attempted_1 = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, INPUT);
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_1 ? HIGH : LOW);
  pinMode(10, OUTPUT);
  digitalWrite(10, hw_value_io_2 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  int32_t hw_tone_command_io_3 = (hw_value_io_3 >= 31 && hw_value_io_3 <= 65535) ? hw_value_io_3 : 0;
  if (hw_tone_command_io_3 != hw_tone_io_3) {
    if (hw_tone_command_io_3 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_3);
    hw_tone_io_3 = hw_tone_command_io_3;
  }
  hw_display_io_8.setBrightness(7, true);
  if (hw_value_io_8 != hw_display_previous_io_8) {
    if (hw_value_io_8 < -999 || hw_value_io_8 > 9999) hw_display_io_8.clear();
    else hw_display_io_8.showNumberDec((int)hw_value_io_8, false, 4, 0);
    hw_display_previous_io_8 = hw_value_io_8;
  }
  pinMode(8, OUTPUT);
  digitalWrite(8, LOW);
  pinMode(9, INPUT);
  pinMode(11, OUTPUT);
  digitalWrite(11, LOW);
  pinMode(12, INPUT);
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
    hw_value_io_5 = (hw_ultrasonic_pulse_0 >= 116UL && hw_ultrasonic_pulse_0 <= 23200UL);
    if (hw_value_io_5) hw_value_io_4 = hw_ultrasonic_pulse_0 / 58.0;
  }
  uint32_t hw_ultrasonic_now_1 = (uint32_t)millis();
  if (!hw_ultrasonic_attempted_1 || (uint32_t)(hw_ultrasonic_now_1 - hw_ultrasonic_sampled_1) >= 60UL) {
    hw_ultrasonic_attempted_1 = true; hw_ultrasonic_sampled_1 = hw_ultrasonic_now_1;
    digitalWrite(11, LOW); delayMicroseconds(2);
    digitalWrite(11, HIGH); delayMicroseconds(10); digitalWrite(11, LOW);
    unsigned long hw_ultrasonic_pulse_1 = pulseIn(12, HIGH, 30000UL);
    hw_value_io_7 = (hw_ultrasonic_pulse_1 >= 116UL && hw_ultrasonic_pulse_1 <= 23200UL);
    if (hw_value_io_7) hw_value_io_6 = hw_ultrasonic_pulse_1 / 58.0;
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_4, hw_value_io_5, hw_value_io_6, hw_value_io_7, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_io_8);
  digitalWrite(13, hw_value_io_1 ? HIGH : LOW);
  digitalWrite(10, hw_value_io_2 ? HIGH : LOW);
  int32_t hw_tone_command_io_3 = (hw_value_io_3 >= 31 && hw_value_io_3 <= 65535) ? hw_value_io_3 : 0;
  if (hw_tone_command_io_3 != hw_tone_io_3) {
    if (hw_tone_command_io_3 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_3);
    hw_tone_io_3 = hw_tone_command_io_3;
  }
  if (hw_value_io_8 != hw_display_previous_io_8) {
    if (hw_value_io_8 < -999 || hw_value_io_8 > 9999) hw_display_io_8.clear();
    else hw_display_io_8.showNumberDec((int)hw_value_io_8, false, 4, 0);
    hw_display_previous_io_8 = hw_value_io_8;
  }
}
