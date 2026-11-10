#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_030\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
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
extern int32_t sysml_state_pageIndex;
/* Global Variable define of  */
extern bool sysml_state_previousPressed;
/* Global Variable define of  */
extern double sysml_state_dhtTempLastValid;
/* Global Variable define of  */
extern double sysml_state_dhtHumLastValid;
/* Global Variable define of  */
extern int32_t sysml_state_reportAccumMs;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, double io_2, double io_3, bool io_4, int32_t io_6, int32_t* io_5, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, double* serial_0_value_2);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_pageIndex = 0;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
double sysml_state_dhtTempLastValid = 0.0;
/* Global Variable define of  */
double sysml_state_dhtHumLastValid = 0.0;
/* Global Variable define of  */
int32_t sysml_state_reportAccumMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
#line 227 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_030\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 241 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_030\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 45 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_030\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_pageIndex = 0;
    sysml_state_previousPressed = false;
    sysml_state_dhtTempLastValid = 0.0;
    sysml_state_dhtHumLastValid = 0.0;
    sysml_state_reportAccumMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, double io_2, double io_3, bool io_4, int32_t io_6, int32_t* io_5, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, double* serial_0_value_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int32_t sysml_conditional_1 = 0;
    int32_t sysml_conditional_2 = 0;
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
    double sysml_conditional_14 = 0;
    double sysml_conditional_15 = 0;
    if (io_4) {
        sysml_state_dhtTempLastValid = io_2;
        sysml_state_dhtHumLastValid = io_3;
    }
    sysml_lazy_0 = io_1;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        if (sysml_state_pageIndex == 0) {
            sysml_conditional_1 = 1;
        }
        else {
            if (sysml_state_pageIndex == 1) {
                sysml_conditional_2 = 2;
            }
            else {
                sysml_conditional_2 = 0;
            }
            sysml_conditional_1 = sysml_conditional_2;
        }
        sysml_state_pageIndex = sysml_conditional_1;
    }
    sysml_state_previousPressed = io_1;
    sysml_state_reportAccumMs = (sysml_state_reportAccumMs + io_6);
    if ((sysml_state_reportAccumMs >= 2000)) {
        sysml_state_reportAccumMs = (sysml_state_reportAccumMs - 2000);
    }
    int32_t sysml_value_0 = 0;
    if (sysml_state_pageIndex == 0) {
        if (io_0 >= 1000.0) {
            sysml_conditional_4 = 9999;
        }
        else {
            if (io_0 <= 0.0) {
                sysml_conditional_5 = 0;
            }
            else {
                if (io_0 < ((double)((int32_t)io_0))) {
                    sysml_conditional_6 = 1;
                }
                else {
                    sysml_conditional_6 = 0;
                }
                sysml_conditional_5 = ((int32_t)io_0) - sysml_conditional_6;
            }
            sysml_conditional_4 = sysml_conditional_5;
        }
        sysml_conditional_3 = sysml_conditional_4;
    }
    else {
        if (sysml_state_pageIndex == 1) {
            if (io_4) {
                if (io_2 < ((double)((int32_t)io_2))) {
                    sysml_conditional_9 = 1;
                }
                else {
                    sysml_conditional_9 = 0;
                }
                sysml_conditional_8 = ((int32_t)io_2) - sysml_conditional_9;
            }
            else {
                if (sysml_state_dhtTempLastValid < ((double)((int32_t)sysml_state_dhtTempLastValid))) {
                    sysml_conditional_10 = 1;
                }
                else {
                    sysml_conditional_10 = 0;
                }
                sysml_conditional_8 = ((int32_t)sysml_state_dhtTempLastValid) - sysml_conditional_10;
            }
            sysml_conditional_7 = sysml_conditional_8;
        }
        else {
            if (io_4) {
                if (io_3 < ((double)((int32_t)io_3))) {
                    sysml_conditional_12 = 1;
                }
                else {
                    sysml_conditional_12 = 0;
                }
                sysml_conditional_11 = ((int32_t)io_3) - sysml_conditional_12;
            }
            else {
                if (sysml_state_dhtHumLastValid < ((double)((int32_t)sysml_state_dhtHumLastValid))) {
                    sysml_conditional_13 = 1;
                }
                else {
                    sysml_conditional_13 = 0;
                }
                sysml_conditional_11 = ((int32_t)sysml_state_dhtHumLastValid) - sysml_conditional_13;
            }
            sysml_conditional_7 = sysml_conditional_11;
        }
        sysml_conditional_3 = sysml_conditional_7;
    }
    sysml_value_0 = sysml_conditional_3;
    bool sysml_value_1 = 0;
    sysml_value_1 = (sysml_state_reportAccumMs >= 2000);
    double sysml_value_2 = 0;
    sysml_value_2 = io_0;
    double sysml_value_3 = 0;
    if (io_4) {
        sysml_conditional_14 = io_2;
    }
    else {
        sysml_conditional_14 = sysml_state_dhtTempLastValid;
    }
    sysml_value_3 = sysml_conditional_14;
    double sysml_value_4 = 0;
    if (io_4) {
        sysml_conditional_15 = io_3;
    }
    else {
        sysml_conditional_15 = sysml_state_dhtHumLastValid;
    }
    sysml_value_4 = sysml_conditional_15;
    *io_5 = sysml_value_0;
    *serial_0_trigger = sysml_value_1;
    *serial_0_value_0 = sysml_value_2;
    *serial_0_value_1 = sysml_value_3;
    *serial_0_value_2 = sysml_value_4;
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
static double hw_value_io_2 = 0;
static double hw_value_io_3 = 0;
static bool hw_value_io_4 = false;
static int32_t hw_value_io_5 = 0;
TM1637Display hw_display_io_5(4, 5);
static int32_t hw_display_previous_io_5 = INT32_MIN;
static int32_t hw_value_io_6 = 0;
static bool hw_value_serial_0_trigger = false;
static double hw_value_serial_0_value_0 = 0;
static double hw_value_serial_0_value_1 = 0;
static double hw_value_serial_0_value_2 = 0;
#include <DHT.h>
DHT hw_dht_0(7, DHT11);
static bool hw_dht_0_attempted = false;
static uint32_t hw_dht_0_sampled = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(2, INPUT);
  hw_display_io_5.setBrightness(7, true);
  if (hw_value_io_5 != hw_display_previous_io_5) {
    if (hw_value_io_5 < -999 || hw_value_io_5 > 9999) hw_display_io_5.clear();
    else hw_display_io_5.showNumberDec((int)hw_value_io_5, false, 4, 0);
    hw_display_previous_io_5 = hw_value_io_5;
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
    hw_value_io_1 = (digitalRead(2) == HIGH);
  }
  uint32_t hw_dht_0_now = (uint32_t)millis();
  if (!hw_dht_0_attempted || (uint32_t)(hw_dht_0_now - hw_dht_0_sampled) >= 2000UL) {
    hw_dht_0_attempted = true; hw_dht_0_sampled = hw_dht_0_now;
    bool hw_dht_0_valid = hw_dht_0.read();
    double hw_dht_0_io_2 = hw_dht_0_valid ? hw_dht_0.readTemperature() : NAN;
    double hw_dht_0_io_3 = hw_dht_0_valid ? hw_dht_0.readHumidity() : NAN;
    hw_dht_0_valid = hw_dht_0_valid && isfinite(hw_dht_0_io_2) && isfinite(hw_dht_0_io_3);
    if (hw_dht_0_valid) {
      hw_value_io_2 = hw_dht_0_io_2;
      hw_value_io_3 = hw_dht_0_io_3;
    }
    hw_value_io_4 = hw_dht_0_valid;
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_6 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, hw_value_io_3, hw_value_io_4, hw_value_io_6, &hw_value_io_5, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0, &hw_value_serial_0_value_1, &hw_value_serial_0_value_2);
  if (hw_value_io_5 != hw_display_previous_io_5) {
    if (hw_value_io_5 < -999 || hw_value_io_5 > 9999) hw_display_io_5.clear();
    else hw_display_io_5.showNumberDec((int)hw_value_io_5, false, 4, 0);
    hw_display_previous_io_5 = hw_value_io_5;
  }
  if (hw_value_serial_0_trigger) { Serial.print("illumination"); Serial.println(hw_value_serial_0_value_0); }
  if (hw_value_serial_0_trigger) { Serial.print("temperature"); Serial.println(hw_value_serial_0_value_1); }
  if (hw_value_serial_0_trigger) { Serial.print("humidity"); Serial.println(hw_value_serial_0_value_2); }
}

