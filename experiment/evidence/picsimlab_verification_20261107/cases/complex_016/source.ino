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
extern int32_t sysml_state_menuIndex;
/* Global Variable define of  */
extern int32_t sysml_state_confirmedIndex;
/* Global Variable define of  */
extern bool sysml_state_previousMenuPressed;
/* Global Variable define of  */
extern bool sysml_state_previousConfirmPressed;
/* Global Variable define of  */
extern bool sysml_state_tempValid;
/* Global Variable define of  */
extern double sysml_state_lastTemp;
/* Global Variable define of  */
extern double sysml_state_lastHumidity;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, bool io_2, double io_3, double io_4, bool io_5, int32_t* io_6);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_menuIndex = 0;
/* Global Variable define of  */
int32_t sysml_state_confirmedIndex = 0;
/* Global Variable define of  */
bool sysml_state_previousMenuPressed = false;
/* Global Variable define of  */
bool sysml_state_previousConfirmPressed = false;
/* Global Variable define of  */
bool sysml_state_tempValid = false;
/* Global Variable define of  */
double sysml_state_lastTemp = 0.0;
/* Global Variable define of  */
double sysml_state_lastHumidity = 0.0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_menuIndex = 0;
    sysml_state_confirmedIndex = 0;
    sysml_state_previousMenuPressed = false;
    sysml_state_previousConfirmPressed = false;
    sysml_state_tempValid = false;
    sysml_state_lastTemp = 0.0;
    sysml_state_lastHumidity = 0.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, bool io_2, double io_3, double io_4, bool io_5, int32_t* io_6) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int32_t sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    int32_t sysml_conditional_3 = 0;
    int32_t sysml_conditional_4 = 0;
    int32_t sysml_conditional_5 = 0;
    int32_t sysml_conditional_6 = 0;
    int32_t sysml_conditional_7 = 0;
    int32_t sysml_conditional_8 = 0;
    int32_t sysml_conditional_9 = 0;
    int32_t sysml_conditional_10 = 0;
    int32_t sysml_conditional_11 = 0;
    int32_t sysml_conditional_12 = 0;
    int32_t sysml_conditional_13 = 0;
    int32_t sysml_conditional_14 = 0;
    int32_t sysml_conditional_15 = 0;
    if (io_5) {
        sysml_state_tempValid = true;
        sysml_state_lastTemp = io_3;
        sysml_state_lastHumidity = io_4;
    }
    else {
        sysml_state_tempValid = false;
    }
    sysml_lazy_0 = io_1;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousMenuPressed;
    }
    if (sysml_lazy_0) {
        if (sysml_state_menuIndex == 2) {
            sysml_conditional_1 = 0;
        }
        else {
            sysml_conditional_1 = sysml_state_menuIndex + 1;
        }
        sysml_state_menuIndex = sysml_conditional_1;
    }
    sysml_lazy_2 = io_2;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_previousConfirmPressed;
    }
    if (sysml_lazy_2) {
        sysml_state_confirmedIndex = sysml_state_menuIndex;
    }
    sysml_state_previousMenuPressed = io_1;
    sysml_state_previousConfirmPressed = io_2;
    int32_t sysml_value_0 = 0;
    if (sysml_state_confirmedIndex == 0) {
        if (sysml_state_tempValid) {
            if (sysml_state_lastTemp >= 0.0) {
                if (((sysml_state_lastTemp * 10.0) + 0.5) < ((double)((int32_t)((sysml_state_lastTemp * 10.0) + 0.5)))) {
                    sysml_conditional_6 = 1;
                }
                else {
                    sysml_conditional_6 = 0;
                }
                sysml_conditional_5 = ((int32_t)((sysml_state_lastTemp * 10.0) + 0.5)) - sysml_conditional_6;
            }
            else {
                if (((sysml_state_lastTemp * 10.0) - 0.5) < ((double)((int32_t)((sysml_state_lastTemp * 10.0) - 0.5)))) {
                    sysml_conditional_7 = 1;
                }
                else {
                    sysml_conditional_7 = 0;
                }
                sysml_conditional_5 = ((int32_t)((sysml_state_lastTemp * 10.0) - 0.5)) - sysml_conditional_7;
            }
            sysml_conditional_4 = sysml_conditional_5;
        }
        else {
            sysml_conditional_4 = 0;
        }
        sysml_conditional_3 = sysml_conditional_4;
    }
    else {
        if (sysml_state_confirmedIndex == 1) {
            if (sysml_state_tempValid) {
                if (sysml_state_lastHumidity >= 0.0) {
                    if (((sysml_state_lastHumidity * 10.0) + 0.5) < ((double)((int32_t)((sysml_state_lastHumidity * 10.0) + 0.5)))) {
                        sysml_conditional_11 = 1;
                    }
                    else {
                        sysml_conditional_11 = 0;
                    }
                    sysml_conditional_10 = ((int32_t)((sysml_state_lastHumidity * 10.0) + 0.5)) - sysml_conditional_11;
                }
                else {
                    if (((sysml_state_lastHumidity * 10.0) - 0.5) < ((double)((int32_t)((sysml_state_lastHumidity * 10.0) - 0.5)))) {
                        sysml_conditional_12 = 1;
                    }
                    else {
                        sysml_conditional_12 = 0;
                    }
                    sysml_conditional_10 = ((int32_t)((sysml_state_lastHumidity * 10.0) - 0.5)) - sysml_conditional_12;
                }
                sysml_conditional_9 = sysml_conditional_10;
            }
            else {
                sysml_conditional_9 = 0;
            }
            sysml_conditional_8 = sysml_conditional_9;
        }
        else {
            if (io_0 <= 1023.0) {
                if (io_0 < ((double)((int32_t)io_0))) {
                    sysml_conditional_14 = 1;
                }
                else {
                    sysml_conditional_14 = 0;
                }
                sysml_conditional_13 = ((int32_t)io_0) - sysml_conditional_14;
            }
            else {
                if (io_0 < ((double)((int32_t)io_0))) {
                    sysml_conditional_15 = 1;
                }
                else {
                    sysml_conditional_15 = 0;
                }
                sysml_conditional_13 = ((int32_t)io_0) - sysml_conditional_15;
            }
            sysml_conditional_8 = sysml_conditional_13;
        }
        sysml_conditional_3 = sysml_conditional_8;
    }
    sysml_value_0 = sysml_conditional_3;
    *io_6 = sysml_value_0;
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
static double hw_value_io_3 = 0;
static double hw_value_io_4 = 0;
static bool hw_value_io_5 = false;
static int32_t hw_value_io_6 = 0;
TM1637Display hw_display_io_6(4, 5);
static int32_t hw_display_previous_io_6 = INT32_MIN;
#include <DHT.h>
DHT hw_dht_0(7, DHT11);
static bool hw_dht_0_attempted = false;
static uint32_t hw_dht_0_sampled = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(A0, INPUT);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  hw_display_io_6.setBrightness(7, true);
  if (hw_value_io_6 != hw_display_previous_io_6) {
    if (hw_value_io_6 < -999 || hw_value_io_6 > 9999) hw_display_io_6.clear();
    else hw_display_io_6.showNumberDec((int)hw_value_io_6, false, 4, 0);
    hw_display_previous_io_6 = hw_value_io_6;
  }
  hw_dht_0.begin();
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 10UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (analogRead(A0) * 1.0 + 0.0);
  }
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 10UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = (digitalRead(2) == LOW);
  }
  if (!hw_attempted_io_2 || (uint32_t)(hw_now - hw_sampled_io_2) >= 10UL) {
    hw_attempted_io_2 = true; hw_sampled_io_2 = hw_now;
    hw_value_io_2 = (digitalRead(3) == LOW);
  }
  uint32_t hw_dht_0_now = (uint32_t)millis();
  if (!hw_dht_0_attempted || (uint32_t)(hw_dht_0_now - hw_dht_0_sampled) >= 2000UL) {
    hw_dht_0_attempted = true; hw_dht_0_sampled = hw_dht_0_now;
    bool hw_dht_0_valid = hw_dht_0.read();
    double hw_dht_0_io_3 = hw_dht_0_valid ? hw_dht_0.readTemperature() : NAN;
    double hw_dht_0_io_4 = hw_dht_0_valid ? hw_dht_0.readHumidity() : NAN;
    hw_dht_0_valid = hw_dht_0_valid && isfinite(hw_dht_0_io_3) && isfinite(hw_dht_0_io_4);
    if (hw_dht_0_valid) {
      hw_value_io_3 = hw_dht_0_io_3;
      hw_value_io_4 = hw_dht_0_io_4;
    }
    hw_value_io_5 = hw_dht_0_valid;
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, hw_value_io_3, hw_value_io_4, hw_value_io_5, &hw_value_io_6);
  if (hw_value_io_6 != hw_display_previous_io_6) {
    if (hw_value_io_6 < -999 || hw_value_io_6 > 9999) hw_display_io_6.clear();
    else hw_display_io_6.showNumberDec((int)hw_value_io_6, false, 4, 0);
    hw_display_previous_io_6 = hw_value_io_6;
  }
}
