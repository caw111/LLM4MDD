#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_010\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
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
extern bool sysml_state_showHumidity;
/* Global Variable define of  */
extern bool sysml_state_prevPressed;
/* Global Variable define of  */
extern int32_t sysml_state_pwmCountFan;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, bool io_2, bool io_5, double* io_3, int32_t* io_4);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_showHumidity = false;
/* Global Variable define of  */
bool sysml_state_prevPressed = false;
/* Global Variable define of  */
int32_t sysml_state_pwmCountFan = 0;
/* Init function of CompositeActor named "ProgramPage1" */
#line 207 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_010\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void setup();
#line 221 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_010\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void loop();
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_010\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_showHumidity = false;
    sysml_state_prevPressed = false;
    sysml_state_pwmCountFan = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, bool io_2, bool io_5, double* io_3, int32_t* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    int32_t sysml_conditional_2 = 0;
    int32_t sysml_conditional_3 = 0;
    int32_t sysml_conditional_4 = 0;
    int32_t sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    int32_t sysml_conditional_7 = 0;
    int32_t sysml_conditional_8 = 0;
    int32_t sysml_conditional_9 = 0;
    int32_t sysml_conditional_10 = 0;
    bool sysml_lazy_11 = 0;
    double sysml_conditional_12 = 0;
    int32_t sysml_conditional_13 = 0;
    int32_t sysml_conditional_14 = 0;
    int32_t sysml_conditional_15 = 0;
    int32_t sysml_conditional_16 = 0;
    int32_t sysml_conditional_17 = 0;
    sysml_lazy_0 = io_5;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_prevPressed;
    }
    if (sysml_lazy_0) {
        sysml_state_showHumidity = (!sysml_state_showHumidity);
    }
    sysml_lazy_1 = !sysml_state_showHumidity;
    if (sysml_lazy_1) {
        sysml_lazy_1 = io_2;
    }
    if (sysml_lazy_1) {
        if (io_0 > 30.0) {
            if (io_0 >= 40.0) {
                sysml_conditional_3 = 255;
            }
            else {
                if (io_0 <= 30.0) {
                    sysml_conditional_4 = 0;
                }
                else {
                    if (((io_0 - 30.0) * 25.5) < ((double)((int32_t)((io_0 - 30.0) * 25.5)))) {
                        sysml_conditional_5 = 1;
                    }
                    else {
                        sysml_conditional_5 = 0;
                    }
                    sysml_conditional_4 = ((int32_t)((io_0 - 30.0) * 25.5)) - sysml_conditional_5;
                }
                sysml_conditional_3 = sysml_conditional_4;
            }
            sysml_conditional_2 = sysml_conditional_3;
        }
        else {
            sysml_conditional_2 = 0;
        }
        sysml_state_pwmCountFan = sysml_conditional_2;
    }
    else {
        sysml_lazy_6 = io_2;
        if (sysml_lazy_6) {
            sysml_lazy_6 = io_0 > 30.0;
        }
        if (sysml_lazy_6) {
            if (io_0 >= 40.0) {
                sysml_conditional_8 = 255;
            }
            else {
                if (io_0 <= 30.0) {
                    sysml_conditional_9 = 0;
                }
                else {
                    if (((io_0 - 30.0) * 25.5) < ((double)((int32_t)((io_0 - 30.0) * 25.5)))) {
                        sysml_conditional_10 = 1;
                    }
                    else {
                        sysml_conditional_10 = 0;
                    }
                    sysml_conditional_9 = ((int32_t)((io_0 - 30.0) * 25.5)) - sysml_conditional_10;
                }
                sysml_conditional_8 = sysml_conditional_9;
            }
            sysml_conditional_7 = sysml_conditional_8;
        }
        else {
            sysml_conditional_7 = 0;
        }
        sysml_state_pwmCountFan = sysml_conditional_7;
    }
    sysml_state_prevPressed = io_5;
    double sysml_value_0 = 0;
    sysml_lazy_11 = io_2;
    if (sysml_lazy_11) {
        sysml_lazy_11 = io_0 > 30.0;
    }
    if (sysml_lazy_11) {
        sysml_conditional_12 = sysml_state_pwmCountFan;
    }
    else {
        sysml_conditional_12 = 0.0;
    }
    sysml_value_0 = sysml_conditional_12;
    int32_t sysml_value_1 = 0;
    if (sysml_state_showHumidity) {
        if (io_2) {
            if (io_1 < ((double)((int32_t)io_1))) {
                sysml_conditional_15 = 1;
            }
            else {
                sysml_conditional_15 = 0;
            }
            sysml_conditional_14 = ((int32_t)io_1) - sysml_conditional_15;
        }
        else {
            sysml_conditional_14 = 0;
        }
        sysml_conditional_13 = sysml_conditional_14;
    }
    else {
        if (io_2) {
            if (io_0 < ((double)((int32_t)io_0))) {
                sysml_conditional_17 = 1;
            }
            else {
                sysml_conditional_17 = 0;
            }
            sysml_conditional_16 = ((int32_t)io_0) - sysml_conditional_17;
        }
        else {
            sysml_conditional_16 = 0;
        }
        sysml_conditional_13 = sysml_conditional_16;
    }
    sysml_value_1 = sysml_conditional_13;
    *io_3 = sysml_value_0;
    *io_4 = sysml_value_1;
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
static double hw_value_io_1 = 0;
static bool hw_value_io_2 = false;
static double hw_value_io_3 = 0;
static int32_t hw_value_io_4 = 0;
TM1637Display hw_display_io_4(4, 3);
static int32_t hw_display_previous_io_4 = INT32_MIN;
static bool hw_value_io_5 = false;
static uint32_t hw_sampled_io_5 = 0;
static bool hw_attempted_io_5 = false;
#include <DHT.h>
DHT hw_dht_0(7, DHT11);
static bool hw_dht_0_attempted = false;
static uint32_t hw_dht_0_sampled = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(5, OUTPUT);
  analogWrite(5, (int)constrain((hw_value_io_3 * 1 + 0), 0.0, 255.0));
  hw_display_io_4.setBrightness(7, true);
  if (hw_value_io_4 != hw_display_previous_io_4) {
    if (hw_value_io_4 < -999 || hw_value_io_4 > 9999) hw_display_io_4.clear();
    else hw_display_io_4.showNumberDec((int)hw_value_io_4, false, 4, 0);
    hw_display_previous_io_4 = hw_value_io_4;
  }
  pinMode(2, INPUT_PULLUP);
  hw_dht_0.begin();
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_5 || (uint32_t)(hw_now - hw_sampled_io_5) >= 10UL) {
    hw_attempted_io_5 = true; hw_sampled_io_5 = hw_now;
    hw_value_io_5 = (digitalRead(2) == LOW);
  }
  uint32_t hw_dht_0_now = (uint32_t)millis();
  if (!hw_dht_0_attempted || (uint32_t)(hw_dht_0_now - hw_dht_0_sampled) >= 2000UL) {
    hw_dht_0_attempted = true; hw_dht_0_sampled = hw_dht_0_now;
    bool hw_dht_0_valid = hw_dht_0.read();
    double hw_dht_0_io_0 = hw_dht_0_valid ? hw_dht_0.readTemperature() : NAN;
    double hw_dht_0_io_1 = hw_dht_0_valid ? hw_dht_0.readHumidity() : NAN;
    hw_dht_0_valid = hw_dht_0_valid && isfinite(hw_dht_0_io_0) && isfinite(hw_dht_0_io_1);
    if (hw_dht_0_valid) {
      hw_value_io_0 = hw_dht_0_io_0;
      hw_value_io_1 = hw_dht_0_io_1;
    }
    hw_value_io_2 = hw_dht_0_valid;
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, hw_value_io_5, &hw_value_io_3, &hw_value_io_4);
  analogWrite(5, (int)constrain((hw_value_io_3 * 1 + 0), 0.0, 255.0));
  if (hw_value_io_4 != hw_display_previous_io_4) {
    if (hw_value_io_4 < -999 || hw_value_io_4 > 9999) hw_display_io_4.clear();
    else hw_display_io_4.showNumberDec((int)hw_value_io_4, false, 4, 0);
    hw_display_previous_io_4 = hw_value_io_4;
  }
}

