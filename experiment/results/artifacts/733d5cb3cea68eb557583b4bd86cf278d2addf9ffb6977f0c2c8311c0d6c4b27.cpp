#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\repaired_workflow_continuation_20261016\\medium_023\\workflow\\attempt_0\\sketch\\sketch.ino"
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
extern bool sysml_state_fanOn;
/* Global Variable define of  */
extern int32_t sysml_state_lastDisplayedC;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_1, double io_2, bool io_3, double* io_0, int32_t* io_4);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_fanOn = false;
/* Global Variable define of  */
int32_t sysml_state_lastDisplayedC = 0;
/* Init function of CompositeActor named "ProgramPage1" */
#line 103 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\repaired_workflow_continuation_20261016\\medium_023\\workflow\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 116 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\repaired_workflow_continuation_20261016\\medium_023\\workflow\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 33 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\repaired_workflow_continuation_20261016\\medium_023\\workflow\\attempt_0\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_fanOn = false;
    sysml_state_lastDisplayedC = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_1, double io_2, bool io_3, double* io_0, int32_t* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int32_t sysml_conditional_0 = 0;
    int32_t sysml_conditional_1 = 0;
    int32_t sysml_conditional_2 = 0;
    double sysml_conditional_3 = 0;
    if (io_3) {
        sysml_state_fanOn = (io_1 > 30.0);
        if (io_1 >= 0.0) {
            if (io_1 < ((double)((int32_t)io_1))) {
                sysml_conditional_1 = 1;
            }
            else {
                sysml_conditional_1 = 0;
            }
            sysml_conditional_0 = ((int32_t)io_1) - sysml_conditional_1;
        }
        else {
            if ((0.0 - io_1) < ((double)((int32_t)(0.0 - io_1)))) {
                sysml_conditional_2 = 1;
            }
            else {
                sysml_conditional_2 = 0;
            }
            sysml_conditional_0 = -(((int32_t)(0.0 - io_1)) - sysml_conditional_2);
        }
        sysml_state_lastDisplayedC = sysml_conditional_0;
    }
    double sysml_value_0 = 0;
    if (sysml_state_fanOn) {
        sysml_conditional_3 = 255.0;
    }
    else {
        sysml_conditional_3 = 0.0;
    }
    sysml_value_0 = sysml_conditional_3;
    int32_t sysml_value_1 = 0;
    sysml_value_1 = sysml_state_lastDisplayedC;
    *io_0 = sysml_value_0;
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
static double hw_value_io_2 = 0;
static bool hw_value_io_3 = false;
static int32_t hw_value_io_4 = 0;
TM1637Display hw_display_io_4(4, 2);
static int32_t hw_display_previous_io_4 = INT32_MIN;
#include <DHT.h>
DHT hw_dht_0(7, DHT11);
static bool hw_dht_0_attempted = false;
static uint32_t hw_dht_0_sampled = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(5, OUTPUT);
  analogWrite(5, (int)constrain((hw_value_io_0 * 1.0 + 0.0), 0.0, 255.0));
  hw_display_io_4.setBrightness(7, true);
  if (hw_value_io_4 != hw_display_previous_io_4) {
    if (hw_value_io_4 < -999 || hw_value_io_4 > 9999) hw_display_io_4.clear();
    else hw_display_io_4.showNumberDec((int)hw_value_io_4, false, 4, 0);
    hw_display_previous_io_4 = hw_value_io_4;
  }
  hw_dht_0.begin();
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  uint32_t hw_dht_0_now = (uint32_t)millis();
  if (!hw_dht_0_attempted || (uint32_t)(hw_dht_0_now - hw_dht_0_sampled) >= 2000UL) {
    hw_dht_0_attempted = true; hw_dht_0_sampled = hw_dht_0_now;
    bool hw_dht_0_valid = hw_dht_0.read();
    double hw_dht_0_io_1 = hw_dht_0_valid ? hw_dht_0.readTemperature() : NAN;
    double hw_dht_0_io_2 = hw_dht_0_valid ? hw_dht_0.readHumidity() : NAN;
    hw_dht_0_valid = hw_dht_0_valid && isfinite(hw_dht_0_io_1) && isfinite(hw_dht_0_io_2);
    if (hw_dht_0_valid) {
      hw_value_io_1 = hw_dht_0_io_1;
      hw_value_io_2 = hw_dht_0_io_2;
    }
    hw_value_io_3 = hw_dht_0_valid;
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_1, hw_value_io_2, hw_value_io_3, &hw_value_io_0, &hw_value_io_4);
  analogWrite(5, (int)constrain((hw_value_io_0 * 1.0 + 0.0), 0.0, 255.0));
  if (hw_value_io_4 != hw_display_previous_io_4) {
    if (hw_value_io_4 < -999 || hw_value_io_4 > 9999) hw_display_io_4.clear();
    else hw_display_io_4.showNumberDec((int)hw_value_io_4, false, 4, 0);
    hw_display_previous_io_4 = hw_value_io_4;
  }
}

