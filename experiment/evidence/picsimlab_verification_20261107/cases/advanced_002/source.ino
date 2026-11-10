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
extern bool sysml_state_previousPressed;
/* Global Variable define of  */
extern int32_t sysml_state_displayMode;
/* Global Variable define of  */
extern int32_t sysml_state_occupiedCount;
/* Global Variable define of  */
extern bool sysml_state_prevInPositionOccupied;
/* Global Variable define of  */
extern bool sysml_state_prevOccupied;
/* Global Variable define of  */
extern double sysml_state_entryDistanceCm;
/* Global Variable define of  */
extern double sysml_state_parkingDistanceCm;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, double io_2, bool io_3, bool io_8, int32_t io_9, int32_t* io_4, bool* io_5, bool* io_6, int32_t* io_7);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
int32_t sysml_state_displayMode = 0;
/* Global Variable define of  */
int32_t sysml_state_occupiedCount = 0;
/* Global Variable define of  */
bool sysml_state_prevInPositionOccupied = false;
/* Global Variable define of  */
bool sysml_state_prevOccupied = false;
/* Global Variable define of  */
double sysml_state_entryDistanceCm = 0.0;
/* Global Variable define of  */
double sysml_state_parkingDistanceCm = 0.0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousPressed = false;
    sysml_state_displayMode = 0;
    sysml_state_occupiedCount = 0;
    sysml_state_prevInPositionOccupied = false;
    sysml_state_prevOccupied = false;
    sysml_state_entryDistanceCm = 0.0;
    sysml_state_parkingDistanceCm = 0.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, double io_2, bool io_3, bool io_8, int32_t io_9, int32_t* io_4, bool* io_5, bool* io_6, int32_t* io_7) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_conditional_0 = 0;
    double sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    int32_t sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    int32_t sysml_conditional_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    int32_t sysml_conditional_9 = 0;
    int32_t sysml_conditional_10 = 0;
    int32_t sysml_conditional_11 = 0;
    int32_t sysml_conditional_12 = 0;
    int32_t sysml_conditional_13 = 0;
    int32_t sysml_conditional_14 = 0;
    int32_t sysml_conditional_15 = 0;
    bool sysml_lazy_16 = 0;
    bool sysml_conditional_17 = 0;
    bool sysml_lazy_18 = 0;
    bool sysml_conditional_19 = 0;
    bool sysml_lazy_20 = 0;
    bool sysml_lazy_21 = 0;
    bool sysml_lazy_22 = 0;
    bool sysml_lazy_23 = 0;
    bool sysml_conditional_24 = 0;
    bool sysml_lazy_25 = 0;
    int32_t sysml_conditional_26 = 0;
    bool sysml_lazy_27 = 0;
    int32_t sysml_conditional_28 = 0;
    int32_t sysml_conditional_29 = 0;
    int32_t sysml_conditional_30 = 0;
    if (io_1) {
        sysml_conditional_0 = io_0;
    }
    else {
        sysml_conditional_0 = sysml_state_entryDistanceCm;
    }
    sysml_state_entryDistanceCm = sysml_conditional_0;
    if (io_3) {
        sysml_conditional_1 = io_2;
    }
    else {
        sysml_conditional_1 = sysml_state_parkingDistanceCm;
    }
    sysml_state_parkingDistanceCm = sysml_conditional_1;
    sysml_lazy_2 = io_8;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_2) {
        sysml_conditional_3 = (sysml_state_displayMode + 1) + 0;
    }
    else {
        sysml_conditional_3 = sysml_state_displayMode;
    }
    sysml_state_displayMode = sysml_conditional_3;
    if ((sysml_state_displayMode >= 3)) {
        sysml_state_displayMode = 0;
    }
    sysml_lazy_4 = !sysml_state_prevInPositionOccupied;
    if (sysml_lazy_4) {
        sysml_lazy_5 = sysml_state_parkingDistanceCm <= 20.0;
        if (sysml_lazy_5) {
            sysml_lazy_5 = io_3;
        }
        sysml_lazy_4 = sysml_lazy_5;
    }
    if (sysml_lazy_4) {
        sysml_conditional_6 = sysml_state_occupiedCount + 1;
    }
    else {
        sysml_conditional_6 = sysml_state_occupiedCount;
    }
    sysml_state_occupiedCount = sysml_conditional_6;
    sysml_lazy_7 = sysml_state_parkingDistanceCm <= 20.0;
    if (sysml_lazy_7) {
        sysml_lazy_7 = io_3;
    }
    sysml_state_prevInPositionOccupied = sysml_lazy_7;
    sysml_lazy_8 = sysml_state_parkingDistanceCm <= 20.0;
    if (sysml_lazy_8) {
        sysml_lazy_8 = io_3;
    }
    sysml_state_prevOccupied = sysml_lazy_8;
    sysml_state_previousPressed = io_8;
    int32_t sysml_value_0 = 0;
    if (sysml_state_displayMode == 0) {
        if (io_1) {
            if (sysml_state_entryDistanceCm < ((double)((int32_t)sysml_state_entryDistanceCm))) {
                sysml_conditional_11 = 1;
            }
            else {
                sysml_conditional_11 = 0;
            }
            sysml_conditional_10 = ((int32_t)sysml_state_entryDistanceCm) - sysml_conditional_11;
        }
        else {
            sysml_conditional_10 = 0;
        }
        sysml_conditional_9 = sysml_conditional_10;
    }
    else {
        if (sysml_state_displayMode == 1) {
            if (io_3) {
                if (sysml_state_parkingDistanceCm < ((double)((int32_t)sysml_state_parkingDistanceCm))) {
                    sysml_conditional_14 = 1;
                }
                else {
                    sysml_conditional_14 = 0;
                }
                sysml_conditional_13 = ((int32_t)sysml_state_parkingDistanceCm) - sysml_conditional_14;
            }
            else {
                sysml_conditional_13 = 0;
            }
            sysml_conditional_12 = sysml_conditional_13;
        }
        else {
            if (sysml_state_displayMode == 2) {
                sysml_conditional_15 = sysml_state_occupiedCount;
            }
            else {
                sysml_conditional_15 = sysml_state_occupiedCount;
            }
            sysml_conditional_12 = sysml_conditional_15;
        }
        sysml_conditional_9 = sysml_conditional_12;
    }
    sysml_value_0 = sysml_conditional_9;
    bool sysml_value_1 = 0;
    sysml_lazy_16 = io_3;
    if (sysml_lazy_16) {
        sysml_lazy_16 = sysml_state_parkingDistanceCm < 5.0;
    }
    if (sysml_lazy_16) {
        sysml_conditional_17 = true;
    }
    else {
        sysml_lazy_18 = io_3;
        if (sysml_lazy_18) {
            sysml_lazy_18 = sysml_state_parkingDistanceCm <= 20.0;
        }
        if (sysml_lazy_18) {
            sysml_lazy_20 = sysml_state_parkingDistanceCm >= 5.0;
            if (!sysml_lazy_20) {
                sysml_lazy_20 = true;
            }
            sysml_conditional_19 = sysml_lazy_20;
        }
        else {
            sysml_conditional_19 = false;
        }
        sysml_conditional_17 = sysml_conditional_19;
    }
    sysml_value_1 = sysml_conditional_17;
    bool sysml_value_2 = 0;
    sysml_lazy_21 = io_1;
    if (sysml_lazy_21) {
        sysml_lazy_21 = sysml_state_entryDistanceCm < 30.0;
    }
    sysml_lazy_22 = sysml_lazy_21;
    if (sysml_lazy_22) {
        sysml_lazy_23 = io_3;
        if (sysml_lazy_23) {
            sysml_lazy_23 = sysml_state_parkingDistanceCm <= 20.0;
        }
        sysml_lazy_22 = !sysml_lazy_23;
    }
    if (sysml_lazy_22) {
        sysml_conditional_24 = true;
    }
    else {
        sysml_conditional_24 = false;
    }
    sysml_value_2 = sysml_conditional_24;
    int32_t sysml_value_3 = 0;
    sysml_lazy_25 = io_3;
    if (sysml_lazy_25) {
        sysml_lazy_25 = sysml_state_parkingDistanceCm < 5.0;
    }
    if (sysml_lazy_25) {
        sysml_conditional_26 = 2000;
    }
    else {
        sysml_lazy_27 = io_3;
        if (sysml_lazy_27) {
            sysml_lazy_27 = sysml_state_parkingDistanceCm <= 20.0;
        }
        if (sysml_lazy_27) {
            if (sysml_state_parkingDistanceCm < 10.0) {
                sysml_conditional_29 = 1800;
            }
            else {
                if (sysml_state_parkingDistanceCm < 15.0) {
                    sysml_conditional_30 = 1200;
                }
                else {
                    sysml_conditional_30 = 700;
                }
                sysml_conditional_29 = sysml_conditional_30;
            }
            sysml_conditional_28 = sysml_conditional_29;
        }
        else {
            sysml_conditional_28 = 0;
        }
        sysml_conditional_26 = sysml_conditional_28;
    }
    sysml_value_3 = sysml_conditional_26;
    *io_4 = sysml_value_0;
    *io_5 = sysml_value_1;
    *io_6 = sysml_value_2;
    *io_7 = sysml_value_3;
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
static int32_t hw_value_io_4 = 0;
TM1637Display hw_display_io_4(4, 5);
static int32_t hw_display_previous_io_4 = INT32_MIN;
static bool hw_value_io_5 = false;
static bool hw_value_io_6 = false;
static int32_t hw_value_io_7 = 0;
static int32_t hw_tone_io_7 = -1;
static bool hw_value_io_8 = false;
static uint32_t hw_sampled_io_8 = 0;
static bool hw_attempted_io_8 = false;
static int32_t hw_value_io_9 = 0;
static uint32_t hw_ultrasonic_sampled_0 = 0;
static bool hw_ultrasonic_attempted_0 = false;
static uint32_t hw_ultrasonic_sampled_1 = 0;
static bool hw_ultrasonic_attempted_1 = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  hw_display_io_4.setBrightness(7, true);
  if (hw_value_io_4 != hw_display_previous_io_4) {
    if (hw_value_io_4 < -999 || hw_value_io_4 > 9999) hw_display_io_4.clear();
    else hw_display_io_4.showNumberDec((int)hw_value_io_4, false, 4, 0);
    hw_display_previous_io_4 = hw_value_io_4;
  }
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_5 ? HIGH : LOW);
  pinMode(10, OUTPUT);
  digitalWrite(10, hw_value_io_6 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  int32_t hw_tone_command_io_7 = (hw_value_io_7 >= 31 && hw_value_io_7 <= 65535) ? hw_value_io_7 : 0;
  if (hw_tone_command_io_7 != hw_tone_io_7) {
    if (hw_tone_command_io_7 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_7);
    hw_tone_io_7 = hw_tone_command_io_7;
  }
  pinMode(2, INPUT_PULLUP);
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
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_8 || (uint32_t)(hw_now - hw_sampled_io_8) >= 5UL) {
    hw_attempted_io_8 = true; hw_sampled_io_8 = hw_now;
    hw_value_io_8 = (digitalRead(2) == HIGH);
  }
  uint32_t hw_ultrasonic_now_0 = (uint32_t)millis();
  if (!hw_ultrasonic_attempted_0 || (uint32_t)(hw_ultrasonic_now_0 - hw_ultrasonic_sampled_0) >= 60UL) {
    hw_ultrasonic_attempted_0 = true; hw_ultrasonic_sampled_0 = hw_ultrasonic_now_0;
    digitalWrite(8, LOW); delayMicroseconds(2);
    digitalWrite(8, HIGH); delayMicroseconds(10); digitalWrite(8, LOW);
    unsigned long hw_ultrasonic_pulse_0 = pulseIn(9, HIGH, 30000UL);
    hw_value_io_1 = (hw_ultrasonic_pulse_0 >= 116UL && hw_ultrasonic_pulse_0 <= 23200UL);
    if (hw_value_io_1) hw_value_io_0 = hw_ultrasonic_pulse_0 / 58.0;
  }
  uint32_t hw_ultrasonic_now_1 = (uint32_t)millis();
  if (!hw_ultrasonic_attempted_1 || (uint32_t)(hw_ultrasonic_now_1 - hw_ultrasonic_sampled_1) >= 60UL) {
    hw_ultrasonic_attempted_1 = true; hw_ultrasonic_sampled_1 = hw_ultrasonic_now_1;
    digitalWrite(11, LOW); delayMicroseconds(2);
    digitalWrite(11, HIGH); delayMicroseconds(10); digitalWrite(11, LOW);
    unsigned long hw_ultrasonic_pulse_1 = pulseIn(12, HIGH, 30000UL);
    hw_value_io_3 = (hw_ultrasonic_pulse_1 >= 116UL && hw_ultrasonic_pulse_1 <= 23200UL);
    if (hw_value_io_3) hw_value_io_2 = hw_ultrasonic_pulse_1 / 58.0;
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_9 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, hw_value_io_3, hw_value_io_8, hw_value_io_9, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_io_7);
  if (hw_value_io_4 != hw_display_previous_io_4) {
    if (hw_value_io_4 < -999 || hw_value_io_4 > 9999) hw_display_io_4.clear();
    else hw_display_io_4.showNumberDec((int)hw_value_io_4, false, 4, 0);
    hw_display_previous_io_4 = hw_value_io_4;
  }
  digitalWrite(13, hw_value_io_5 ? HIGH : LOW);
  digitalWrite(10, hw_value_io_6 ? HIGH : LOW);
  int32_t hw_tone_command_io_7 = (hw_value_io_7 >= 31 && hw_value_io_7 <= 65535) ? hw_value_io_7 : 0;
  if (hw_tone_command_io_7 != hw_tone_io_7) {
    if (hw_tone_command_io_7 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_7);
    hw_tone_io_7 = hw_tone_command_io_7;
  }
}
