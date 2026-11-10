#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_001\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
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
extern int32_t sysml_state_timeSinceTempSample;
/* Global Variable define of  */
extern int32_t sysml_state_timeSinceDistSample;
/* Global Variable define of  */
extern int32_t sysml_state_displayPhaseMs;
/* Global Variable define of  */
extern bool sysml_state_displayPageIsTemp;
/* Global Variable define of  */
extern int32_t sysml_state_buzzerPhaseMs;
/* Global Variable define of  */
extern bool sysml_state_buzzerWasArmed;
/* Global Variable define of  */
extern bool sysml_state_buzzerPrevAlarm;
/* Global Variable define of  */
extern bool sysml_state_buzzerHasTempAlarm;
/* Global Variable define of  */
extern bool sysml_state_buzzerHasDistAlarm;
/* Global Variable define of  */
extern bool sysml_state_lastReportedTempAlarm;
/* Global Variable define of  */
extern bool sysml_state_lastReportedDistAlarm;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, bool io_2, double io_3, bool io_4, int32_t io_7, int32_t* io_5, int32_t* io_6, bool* serial_0_trigger, bool* serial_0_value_0, bool* serial_0_value_1);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_timeSinceTempSample = 0;
/* Global Variable define of  */
int32_t sysml_state_timeSinceDistSample = 0;
/* Global Variable define of  */
int32_t sysml_state_displayPhaseMs = 0;
/* Global Variable define of  */
bool sysml_state_displayPageIsTemp = true;
/* Global Variable define of  */
int32_t sysml_state_buzzerPhaseMs = 0;
/* Global Variable define of  */
bool sysml_state_buzzerWasArmed = false;
/* Global Variable define of  */
bool sysml_state_buzzerPrevAlarm = false;
/* Global Variable define of  */
bool sysml_state_buzzerHasTempAlarm = false;
/* Global Variable define of  */
bool sysml_state_buzzerHasDistAlarm = false;
/* Global Variable define of  */
bool sysml_state_lastReportedTempAlarm = false;
/* Global Variable define of  */
bool sysml_state_lastReportedDistAlarm = false;
/* Init function of CompositeActor named "ProgramPage1" */
#line 304 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_001\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void setup();
#line 326 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_001\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void loop();
#line 69 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_001\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_timeSinceTempSample = 0;
    sysml_state_timeSinceDistSample = 0;
    sysml_state_displayPhaseMs = 0;
    sysml_state_displayPageIsTemp = true;
    sysml_state_buzzerPhaseMs = 0;
    sysml_state_buzzerWasArmed = false;
    sysml_state_buzzerPrevAlarm = false;
    sysml_state_buzzerHasTempAlarm = false;
    sysml_state_buzzerHasDistAlarm = false;
    sysml_state_lastReportedTempAlarm = false;
    sysml_state_lastReportedDistAlarm = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, bool io_2, double io_3, bool io_4, int32_t io_7, int32_t* io_5, int32_t* io_6, bool* serial_0_trigger, bool* serial_0_value_0, bool* serial_0_value_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_conditional_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    int32_t sysml_conditional_7 = 0;
    bool sysml_lazy_8 = 0;
    int32_t sysml_conditional_9 = 0;
    int32_t sysml_conditional_10 = 0;
    int32_t sysml_conditional_11 = 0;
    int32_t sysml_conditional_12 = 0;
    int32_t sysml_conditional_13 = 0;
    int32_t sysml_conditional_14 = 0;
    bool sysml_lazy_15 = 0;
    bool sysml_lazy_16 = 0;
    bool sysml_lazy_17 = 0;
    bool sysml_lazy_18 = 0;
    bool sysml_lazy_19 = 0;
    bool sysml_lazy_20 = 0;
    bool sysml_conditional_21 = 0;
    sysml_state_timeSinceTempSample = (sysml_state_timeSinceTempSample + io_7);
    sysml_state_timeSinceDistSample = (sysml_state_timeSinceDistSample + io_7);
    if ((sysml_state_timeSinceTempSample >= 2000)) {
        sysml_state_timeSinceTempSample = (sysml_state_timeSinceTempSample - 2000);
    }
    if ((sysml_state_timeSinceDistSample >= 60)) {
        sysml_state_timeSinceDistSample = (sysml_state_timeSinceDistSample - 60);
    }
    sysml_state_displayPhaseMs = (sysml_state_displayPhaseMs + io_7);
    if ((sysml_state_displayPhaseMs >= 2000)) {
        sysml_state_displayPhaseMs = (sysml_state_displayPhaseMs - 2000);
        sysml_state_displayPageIsTemp = (!sysml_state_displayPageIsTemp);
    }
    sysml_lazy_0 = io_2;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_0 > 32.0;
    }
    if (sysml_lazy_0) {
        sysml_state_buzzerHasTempAlarm = true;
    }
    else {
        sysml_state_buzzerHasTempAlarm = false;
    }
    sysml_lazy_1 = io_4;
    if (sysml_lazy_1) {
        sysml_lazy_1 = io_3 < 15.0;
    }
    if (sysml_lazy_1) {
        sysml_state_buzzerHasDistAlarm = true;
    }
    else {
        sysml_state_buzzerHasDistAlarm = false;
    }
    if (sysml_state_buzzerPrevAlarm) {
        sysml_conditional_2 = sysml_state_buzzerPrevAlarm;
    }
    else {
        sysml_conditional_2 = sysml_state_buzzerPrevAlarm;
    }
    sysml_state_buzzerPrevAlarm = sysml_conditional_2;
    sysml_lazy_3 = sysml_state_buzzerHasTempAlarm;
    if (!sysml_lazy_3) {
        sysml_lazy_3 = sysml_state_buzzerHasDistAlarm;
    }
    sysml_lazy_4 = sysml_lazy_3;
    if (sysml_lazy_4) {
        sysml_lazy_4 = !sysml_state_buzzerWasArmed;
    }
    if (sysml_lazy_4) {
        sysml_state_buzzerWasArmed = true;
        sysml_state_buzzerPhaseMs = 0;
    }
    sysml_lazy_5 = sysml_state_buzzerHasTempAlarm;
    if (!sysml_lazy_5) {
        sysml_lazy_5 = sysml_state_buzzerHasDistAlarm;
    }
    sysml_lazy_6 = !sysml_lazy_5;
    if (sysml_lazy_6) {
        sysml_lazy_6 = sysml_state_buzzerWasArmed;
    }
    if (sysml_lazy_6) {
        sysml_state_buzzerWasArmed = false;
    }
    if (sysml_state_buzzerWasArmed) {
        sysml_state_buzzerPhaseMs = (sysml_state_buzzerPhaseMs + io_7);
        if ((sysml_state_buzzerPhaseMs >= 1000)) {
            sysml_state_buzzerPhaseMs = (sysml_state_buzzerPhaseMs - 1000);
        }
    }
    else {
        sysml_state_buzzerPhaseMs = 0;
    }
    int32_t sysml_value_0 = 0;
    if (sysml_state_displayPageIsTemp) {
        sysml_lazy_8 = io_2;
        if (sysml_lazy_8) {
            sysml_lazy_8 = io_0 > 0.0;
        }
        if (sysml_lazy_8) {
            if (io_0 < ((double)((int32_t)io_0))) {
                sysml_conditional_10 = 1;
            }
            else {
                sysml_conditional_10 = 0;
            }
            sysml_conditional_9 = ((int32_t)io_0) - sysml_conditional_10;
        }
        else {
            sysml_conditional_9 = 0;
        }
        sysml_conditional_7 = sysml_conditional_9;
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
            sysml_conditional_11 = 0;
        }
        sysml_conditional_7 = sysml_conditional_11;
    }
    sysml_value_0 = sysml_conditional_7;
    int32_t sysml_value_1 = 0;
    if (sysml_state_buzzerWasArmed) {
        if (sysml_state_buzzerPhaseMs < 500) {
            sysml_conditional_14 = 1000;
        }
        else {
            sysml_conditional_14 = 0;
        }
        sysml_conditional_13 = sysml_conditional_14;
    }
    else {
        sysml_conditional_13 = 0;
    }
    sysml_value_1 = sysml_conditional_13;
    bool sysml_value_2 = 0;
    sysml_lazy_15 = sysml_state_buzzerWasArmed;
    if (sysml_lazy_15) {
        sysml_lazy_15 = !sysml_state_buzzerPrevAlarm;
    }
    sysml_lazy_16 = sysml_lazy_15;
    if (!sysml_lazy_16) {
        sysml_lazy_17 = sysml_state_buzzerWasArmed;
        if (sysml_lazy_17) {
            sysml_lazy_18 = io_7 == io_7;
            if (sysml_lazy_18) {
                sysml_lazy_18 = true;
            }
            sysml_lazy_17 = sysml_lazy_18 == true;
        }
        sysml_lazy_16 = sysml_lazy_17;
    }
    sysml_lazy_19 = sysml_lazy_16;
    if (!sysml_lazy_19) {
        sysml_lazy_20 = sysml_state_buzzerWasArmed;
        if (sysml_lazy_20) {
            sysml_lazy_20 = sysml_state_buzzerPhaseMs == 0;
        }
        sysml_lazy_19 = sysml_lazy_20;
    }
    if (sysml_lazy_19) {
        sysml_conditional_21 = true;
    }
    else {
        sysml_conditional_21 = false;
    }
    sysml_value_2 = sysml_conditional_21;
    bool sysml_value_3 = 0;
    sysml_value_3 = sysml_state_buzzerHasTempAlarm;
    bool sysml_value_4 = 0;
    sysml_value_4 = sysml_state_buzzerHasDistAlarm;
    *io_5 = sysml_value_0;
    *io_6 = sysml_value_1;
    *serial_0_trigger = sysml_value_2;
    *serial_0_value_0 = sysml_value_3;
    *serial_0_value_1 = sysml_value_4;
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
static bool hw_value_io_4 = false;
static int32_t hw_value_io_5 = 0;
TM1637Display hw_display_io_5(4, 5);
static int32_t hw_display_previous_io_5 = INT32_MIN;
static int32_t hw_value_io_6 = 0;
static int32_t hw_tone_io_6 = -1;
static int32_t hw_value_io_7 = 0;
static bool hw_value_serial_0_trigger = false;
static bool hw_value_serial_0_value_0 = false;
static bool hw_value_serial_0_value_1 = false;
static uint32_t hw_ultrasonic_sampled_0 = 0;
static bool hw_ultrasonic_attempted_0 = false;
#include <DHT.h>
DHT hw_dht_0(7, DHT11);
static bool hw_dht_0_attempted = false;
static uint32_t hw_dht_0_sampled = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(115200);
  hw_display_io_5.setBrightness(7, true);
  if (hw_value_io_5 != hw_display_previous_io_5) {
    if (hw_value_io_5 < -999 || hw_value_io_5 > 9999) hw_display_io_5.clear();
    else hw_display_io_5.showNumberDec((int)hw_value_io_5, false, 4, 0);
    hw_display_previous_io_5 = hw_value_io_5;
  }
  pinMode(6, OUTPUT);
  int32_t hw_tone_command_io_6 = (hw_value_io_6 >= 31 && hw_value_io_6 <= 65535) ? hw_value_io_6 : 0;
  if (hw_tone_command_io_6 != hw_tone_io_6) {
    if (hw_tone_command_io_6 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_6);
    hw_tone_io_6 = hw_tone_command_io_6;
  }
  pinMode(8, OUTPUT);
  digitalWrite(8, LOW);
  pinMode(9, INPUT);
  hw_dht_0.begin();
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  uint32_t hw_ultrasonic_now_0 = (uint32_t)millis();
  if (!hw_ultrasonic_attempted_0 || (uint32_t)(hw_ultrasonic_now_0 - hw_ultrasonic_sampled_0) >= 60UL) {
    hw_ultrasonic_attempted_0 = true; hw_ultrasonic_sampled_0 = hw_ultrasonic_now_0;
    digitalWrite(8, LOW); delayMicroseconds(2);
    digitalWrite(8, HIGH); delayMicroseconds(10); digitalWrite(8, LOW);
    unsigned long hw_ultrasonic_pulse_0 = pulseIn(9, HIGH, 25000UL);
    hw_value_io_4 = (hw_ultrasonic_pulse_0 >= 116UL && hw_ultrasonic_pulse_0 <= 23200UL);
    if (hw_value_io_4) hw_value_io_3 = hw_ultrasonic_pulse_0 / 58.0;
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
  hw_value_io_7 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, hw_value_io_3, hw_value_io_4, hw_value_io_7, &hw_value_io_5, &hw_value_io_6, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0, &hw_value_serial_0_value_1);
  if (hw_value_io_5 != hw_display_previous_io_5) {
    if (hw_value_io_5 < -999 || hw_value_io_5 > 9999) hw_display_io_5.clear();
    else hw_display_io_5.showNumberDec((int)hw_value_io_5, false, 4, 0);
    hw_display_previous_io_5 = hw_value_io_5;
  }
  int32_t hw_tone_command_io_6 = (hw_value_io_6 >= 31 && hw_value_io_6 <= 65535) ? hw_value_io_6 : 0;
  if (hw_tone_command_io_6 != hw_tone_io_6) {
    if (hw_tone_command_io_6 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_6);
    hw_tone_io_6 = hw_tone_command_io_6;
  }
  if (hw_value_serial_0_trigger) { Serial.print("ALARM_TEMP"); Serial.println(hw_value_serial_0_value_0); }
  if (hw_value_serial_0_trigger) { Serial.print("ALARM_DISTANCE"); Serial.println(hw_value_serial_0_value_1); }
}

