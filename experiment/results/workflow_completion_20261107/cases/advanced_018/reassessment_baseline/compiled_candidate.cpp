#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_completion_20261107\\run13_offline_recovery\\cases\\advanced_018\\attempt_0\\arduino\\sketch\\sketch.ino"
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
extern bool sysml_state_component_setpointMode_value;
/* Global Variable define of  */
extern bool sysml_state_component_setpointMode_previous;
/* Global Variable define of  */
extern int32_t sysml_state_component_ventilation_phase;
/* Global Variable define of  */
extern bool sysml_state_component_ventilation_started;
/* Global Variable define of  */
extern bool sysml_state_component_ventilation_enabled;
/* Global Variable define of  */
extern int32_t sysml_state_component_ventilation_remainder;
/* Global Variable define of  */
extern bool sysml_state_component_ventilation_value;
/* Global Variable define of  */
extern bool sysml_state_heaterCommand;
/* Global Variable define of  */
extern double sysml_state_cachedTemperature;
/* Global Variable define of  */
extern bool sysml_state_hasReading;
/* Global Variable define of  */
extern bool sysml_state_previousOutside;
/* Global Variable define of  */
extern int32_t sysml_state_outsideDuration;
/* Global Variable define of  */
extern bool sysml_state_alarmActive;
/* Global Variable define of  */
extern bool sysml_state_reportRaised;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, bool io_7, int32_t io_9, bool* io_2, bool* io_3, int32_t* io_4, int32_t* io_5, bool* io_6, bool* io_8);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_component_setpointMode_value = false;
/* Global Variable define of  */
bool sysml_state_component_setpointMode_previous = false;
/* Global Variable define of  */
int32_t sysml_state_component_ventilation_phase = 0;
/* Global Variable define of  */
bool sysml_state_component_ventilation_started = false;
/* Global Variable define of  */
bool sysml_state_component_ventilation_enabled = false;
/* Global Variable define of  */
int32_t sysml_state_component_ventilation_remainder = 0;
/* Global Variable define of  */
bool sysml_state_component_ventilation_value = false;
/* Global Variable define of  */
bool sysml_state_heaterCommand = false;
/* Global Variable define of  */
double sysml_state_cachedTemperature = 0.0;
/* Global Variable define of  */
bool sysml_state_hasReading = false;
/* Global Variable define of  */
bool sysml_state_previousOutside = false;
/* Global Variable define of  */
int32_t sysml_state_outsideDuration = 0;
/* Global Variable define of  */
bool sysml_state_alarmActive = false;
/* Global Variable define of  */
bool sysml_state_reportRaised = false;
/* Init function of CompositeActor named "ProgramPage1" */
#line 327 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_completion_20261107\\run13_offline_recovery\\cases\\advanced_018\\attempt_0\\arduino\\sketch\\sketch.ino"
void setup();
#line 353 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_completion_20261107\\run13_offline_recovery\\cases\\advanced_018\\attempt_0\\arduino\\sketch\\sketch.ino"
void loop();
#line 81 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_completion_20261107\\run13_offline_recovery\\cases\\advanced_018\\attempt_0\\arduino\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_component_setpointMode_value = false;
    sysml_state_component_setpointMode_previous = false;
    sysml_state_component_ventilation_phase = 0;
    sysml_state_component_ventilation_started = false;
    sysml_state_component_ventilation_enabled = false;
    sysml_state_component_ventilation_remainder = 0;
    sysml_state_component_ventilation_value = false;
    sysml_state_heaterCommand = false;
    sysml_state_cachedTemperature = 0.0;
    sysml_state_hasReading = false;
    sysml_state_previousOutside = false;
    sysml_state_outsideDuration = 0;
    sysml_state_alarmActive = false;
    sysml_state_reportRaised = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, bool io_7, int32_t io_9, bool* io_2, bool* io_3, int32_t* io_4, int32_t* io_5, bool* io_6, bool* io_8) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    int32_t sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_lazy_10 = 0;
    int32_t sysml_conditional_11 = 0;
    int32_t sysml_conditional_12 = 0;
    int32_t sysml_conditional_13 = 0;
    int32_t sysml_conditional_14 = 0;
    sysml_lazy_0 = io_7;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_component_setpointMode_previous;
    }
    if (sysml_lazy_0) {
        sysml_state_component_setpointMode_value = (!sysml_state_component_setpointMode_value);
    }
    sysml_state_component_setpointMode_previous = io_7;
    sysml_lazy_1 = sysml_state_component_ventilation_enabled;
    if (sysml_lazy_1) {
        sysml_lazy_1 = io_9 > 0;
    }
    if (sysml_lazy_1) {
        if ((io_9 >= (300000 - sysml_state_component_ventilation_phase))) {
            sysml_state_component_ventilation_started = true;
        }
        sysml_state_component_ventilation_remainder = io_9;
        if ((sysml_state_component_ventilation_remainder >= 1228800000)) {
            sysml_state_component_ventilation_remainder = (sysml_state_component_ventilation_remainder - 1228800000);
        }
        if ((sysml_state_component_ventilation_remainder >= 614400000)) {
            sysml_state_component_ventilation_remainder = (sysml_state_component_ventilation_remainder - 614400000);
        }
        if ((sysml_state_component_ventilation_remainder >= 307200000)) {
            sysml_state_component_ventilation_remainder = (sysml_state_component_ventilation_remainder - 307200000);
        }
        if ((sysml_state_component_ventilation_remainder >= 153600000)) {
            sysml_state_component_ventilation_remainder = (sysml_state_component_ventilation_remainder - 153600000);
        }
        if ((sysml_state_component_ventilation_remainder >= 76800000)) {
            sysml_state_component_ventilation_remainder = (sysml_state_component_ventilation_remainder - 76800000);
        }
        if ((sysml_state_component_ventilation_remainder >= 38400000)) {
            sysml_state_component_ventilation_remainder = (sysml_state_component_ventilation_remainder - 38400000);
        }
        if ((sysml_state_component_ventilation_remainder >= 19200000)) {
            sysml_state_component_ventilation_remainder = (sysml_state_component_ventilation_remainder - 19200000);
        }
        if ((sysml_state_component_ventilation_remainder >= 9600000)) {
            sysml_state_component_ventilation_remainder = (sysml_state_component_ventilation_remainder - 9600000);
        }
        if ((sysml_state_component_ventilation_remainder >= 4800000)) {
            sysml_state_component_ventilation_remainder = (sysml_state_component_ventilation_remainder - 4800000);
        }
        if ((sysml_state_component_ventilation_remainder >= 2400000)) {
            sysml_state_component_ventilation_remainder = (sysml_state_component_ventilation_remainder - 2400000);
        }
        if ((sysml_state_component_ventilation_remainder >= 1200000)) {
            sysml_state_component_ventilation_remainder = (sysml_state_component_ventilation_remainder - 1200000);
        }
        if ((sysml_state_component_ventilation_remainder >= 600000)) {
            sysml_state_component_ventilation_remainder = (sysml_state_component_ventilation_remainder - 600000);
        }
        if ((sysml_state_component_ventilation_remainder >= 300000)) {
            sysml_state_component_ventilation_remainder = (sysml_state_component_ventilation_remainder - 300000);
        }
        if ((sysml_state_component_ventilation_remainder >= (300000 - sysml_state_component_ventilation_phase))) {
            sysml_state_component_ventilation_phase = (sysml_state_component_ventilation_remainder - (300000 - sysml_state_component_ventilation_phase));
        }
        else {
            sysml_state_component_ventilation_phase = (sysml_state_component_ventilation_phase + sysml_state_component_ventilation_remainder);
        }
    }
    sysml_lazy_2 = true;
    if (sysml_lazy_2) {
        sysml_lazy_2 = sysml_state_component_ventilation_started;
    }
    sysml_lazy_3 = sysml_lazy_2;
    if (sysml_lazy_3) {
        sysml_lazy_3 = sysml_state_component_ventilation_phase < 10000;
    }
    sysml_state_component_ventilation_value = sysml_lazy_3;
    sysml_state_component_ventilation_enabled = true;
    sysml_state_reportRaised = false;
    if (io_1) {
        sysml_state_cachedTemperature = io_0;
        sysml_state_hasReading = true;
        if ((io_0 < 37.0)) {
            sysml_state_heaterCommand = true;
        }
        else {
            if ((io_0 > 38.0)) {
                sysml_state_heaterCommand = false;
            }
        }
        sysml_lazy_4 = io_0 < 37.0;
        if (!sysml_lazy_4) {
            sysml_lazy_4 = io_0 > 38.0;
        }
        if (sysml_lazy_4) {
            if (sysml_state_previousOutside) {
                if (io_9 >= (60000 - sysml_state_outsideDuration)) {
                    sysml_conditional_5 = 60000;
                }
                else {
                    sysml_conditional_5 = sysml_state_outsideDuration + io_9;
                }
                sysml_state_outsideDuration = sysml_conditional_5;
            }
            else {
                sysml_state_outsideDuration = 0;
            }
            sysml_lazy_6 = sysml_state_outsideDuration >= 60000;
            if (sysml_lazy_6) {
                sysml_lazy_6 = !sysml_state_alarmActive;
            }
            if (sysml_lazy_6) {
                sysml_state_reportRaised = true;
                sysml_state_alarmActive = true;
            }
        }
        else {
            sysml_state_outsideDuration = 0;
            sysml_state_alarmActive = false;
        }
    }
    else {
        sysml_state_heaterCommand = false;
        sysml_state_outsideDuration = 0;
    }
    sysml_lazy_7 = io_1;
    if (sysml_lazy_7) {
        sysml_lazy_8 = io_0 < 37.0;
        if (!sysml_lazy_8) {
            sysml_lazy_8 = io_0 > 38.0;
        }
        sysml_lazy_7 = sysml_lazy_8;
    }
    sysml_state_previousOutside = sysml_lazy_7;
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_heaterCommand;
    bool sysml_value_1 = 0;
    sysml_lazy_9 = sysml_state_component_ventilation_value;
    if (!sysml_lazy_9) {
        sysml_lazy_10 = io_1;
        if (sysml_lazy_10) {
            sysml_lazy_10 = io_0 > 38.0;
        }
        sysml_lazy_9 = sysml_lazy_10;
    }
    sysml_value_1 = sysml_lazy_9;
    int32_t sysml_value_2 = 0;
    if (sysml_state_alarmActive) {
        sysml_conditional_11 = 1000;
    }
    else {
        sysml_conditional_11 = 0;
    }
    sysml_value_2 = sysml_conditional_11;
    int32_t sysml_value_3 = 0;
    if (sysml_state_hasReading) {
        if (sysml_state_component_setpointMode_value) {
            sysml_conditional_13 = 38;
        }
        else {
            if ((sysml_state_cachedTemperature + 0.5) < ((double)((int32_t)(sysml_state_cachedTemperature + 0.5)))) {
                sysml_conditional_14 = 1;
            }
            else {
                sysml_conditional_14 = 0;
            }
            sysml_conditional_13 = ((int32_t)(sysml_state_cachedTemperature + 0.5)) - sysml_conditional_14;
        }
        sysml_conditional_12 = sysml_conditional_13;
    }
    else {
        sysml_conditional_12 = 10000;
    }
    sysml_value_3 = sysml_conditional_12;
    bool sysml_value_4 = 0;
    sysml_value_4 = sysml_state_alarmActive;
    bool sysml_value_5 = 0;
    sysml_value_5 = sysml_state_reportRaised;
    *io_2 = sysml_value_0;
    *io_3 = sysml_value_1;
    *io_4 = sysml_value_2;
    *io_5 = sysml_value_3;
    *io_6 = sysml_value_4;
    *io_8 = sysml_value_5;
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
static bool hw_value_io_2 = false;
static bool hw_value_io_3 = false;
static int32_t hw_value_io_4 = 0;
static int32_t hw_tone_io_4 = -1;
static int32_t hw_value_io_5 = 10000;
TM1637Display hw_display_io_5(4, 3);
static int32_t hw_display_previous_io_5 = INT32_MIN;
static bool hw_value_io_6 = false;
static bool hw_value_io_7 = false;
static uint32_t hw_sampled_io_7 = 0;
static bool hw_attempted_io_7 = false;
static bool hw_value_io_8 = false;
static int32_t hw_value_io_9 = 0;
#include <DHT.h>
DHT hw_dht_0(7, DHT11);
static bool hw_dht_0_attempted = false;
static uint32_t hw_dht_0_sampled = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(10, OUTPUT);
  digitalWrite(10, hw_value_io_2 ? HIGH : LOW);
  pinMode(5, OUTPUT);
  digitalWrite(5, hw_value_io_3 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  int32_t hw_tone_command_io_4 = (hw_value_io_4 >= 31 && hw_value_io_4 <= 65535) ? hw_value_io_4 : 0;
  if (hw_tone_command_io_4 != hw_tone_io_4) {
    if (hw_tone_command_io_4 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_4);
    hw_tone_io_4 = hw_tone_command_io_4;
  }
  hw_display_io_5.setBrightness(7, true);
  if (hw_value_io_5 != hw_display_previous_io_5) {
    if (hw_value_io_5 < -999 || hw_value_io_5 > 9999) hw_display_io_5.clear();
    else hw_display_io_5.showNumberDec((int)hw_value_io_5, false, 4, 0);
    hw_display_previous_io_5 = hw_value_io_5;
  }
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_6 ? HIGH : LOW);
  pinMode(2, INPUT_PULLUP);
  hw_dht_0.begin();
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_7 || (uint32_t)(hw_now - hw_sampled_io_7) >= 10UL) {
    hw_attempted_io_7 = true; hw_sampled_io_7 = hw_now;
    hw_value_io_7 = (digitalRead(2) == LOW);
  }
  uint32_t hw_dht_0_now = (uint32_t)millis();
  if (!hw_dht_0_attempted || (uint32_t)(hw_dht_0_now - hw_dht_0_sampled) >= 2000UL) {
    hw_dht_0_attempted = true; hw_dht_0_sampled = hw_dht_0_now;
    bool hw_dht_0_valid = hw_dht_0.read();
    double hw_dht_0_io_0 = hw_dht_0_valid ? hw_dht_0.readTemperature() : NAN;
    hw_dht_0_valid = hw_dht_0_valid && isfinite(hw_dht_0_io_0);
    if (hw_dht_0_valid) {
      hw_value_io_0 = hw_dht_0_io_0;
    }
    hw_value_io_1 = hw_dht_0_valid;
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_9 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_7, hw_value_io_9, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_io_8);
  digitalWrite(10, hw_value_io_2 ? HIGH : LOW);
  digitalWrite(5, hw_value_io_3 ? HIGH : LOW);
  int32_t hw_tone_command_io_4 = (hw_value_io_4 >= 31 && hw_value_io_4 <= 65535) ? hw_value_io_4 : 0;
  if (hw_tone_command_io_4 != hw_tone_io_4) {
    if (hw_tone_command_io_4 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_4);
    hw_tone_io_4 = hw_tone_command_io_4;
  }
  if (hw_value_io_5 != hw_display_previous_io_5) {
    if (hw_value_io_5 < -999 || hw_value_io_5 > 9999) hw_display_io_5.clear();
    else hw_display_io_5.showNumberDec((int)hw_value_io_5, false, 4, 0);
    hw_display_previous_io_5 = hw_value_io_5;
  }
  digitalWrite(13, hw_value_io_6 ? HIGH : LOW);
  if (hw_value_io_8) Serial.println("incubation abnormal");
}

