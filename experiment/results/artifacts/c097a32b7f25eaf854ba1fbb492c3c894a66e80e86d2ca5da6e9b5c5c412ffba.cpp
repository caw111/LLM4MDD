#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_018\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
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
extern bool sysml_state_prevPressed;
/* Global Variable define of  */
extern bool sysml_state_modeIsSetting;
/* Global Variable define of  */
extern double sysml_state_targetTemp;
/* Global Variable define of  */
extern double sysml_state_tempLastValid;
/* Global Variable define of  */
extern bool sysml_state_alarmActive;
/* Global Variable define of  */
extern int32_t sysml_state_abnormalHighCount;
/* Global Variable define of  */
extern int32_t sysml_state_abnormalLowCount;
/* Global Variable define of  */
extern bool sysml_state_fanOn;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, double io_5, double io_6, bool io_7, bool* io_1, bool* io_2, bool* io_3, int32_t* io_4, int32_t* io_8, bool* serial_0_trigger, bool* serial_0_value_0);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_prevPressed = false;
/* Global Variable define of  */
bool sysml_state_modeIsSetting = false;
/* Global Variable define of  */
double sysml_state_targetTemp = 37.0;
/* Global Variable define of  */
double sysml_state_tempLastValid = 25.0;
/* Global Variable define of  */
bool sysml_state_alarmActive = false;
/* Global Variable define of  */
int32_t sysml_state_abnormalHighCount = 0;
/* Global Variable define of  */
int32_t sysml_state_abnormalLowCount = 0;
/* Global Variable define of  */
bool sysml_state_fanOn = false;
/* Init function of CompositeActor named "ProgramPage1" */
#line 248 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_018\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void setup();
#line 274 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_018\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void loop();
#line 57 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_018\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_prevPressed = false;
    sysml_state_modeIsSetting = false;
    sysml_state_targetTemp = 37.0;
    sysml_state_tempLastValid = 25.0;
    sysml_state_alarmActive = false;
    sysml_state_abnormalHighCount = 0;
    sysml_state_abnormalLowCount = 0;
    sysml_state_fanOn = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, double io_5, double io_6, bool io_7, bool* io_1, bool* io_2, bool* io_3, int32_t* io_4, int32_t* io_8, bool* serial_0_trigger, bool* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_conditional_1 = 0;
    double sysml_conditional_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_conditional_7 = 0;
    bool sysml_conditional_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_lazy_10 = 0;
    int32_t sysml_conditional_11 = 0;
    int32_t sysml_conditional_12 = 0;
    int32_t sysml_conditional_13 = 0;
    int32_t sysml_conditional_14 = 0;
    if (io_7) {
        sysml_state_tempLastValid = io_5;
    }
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_prevPressed;
    }
    if (sysml_lazy_0) {
        sysml_state_modeIsSetting = (!sysml_state_modeIsSetting);
        if (sysml_state_modeIsSetting) {
            sysml_conditional_1 = false;
        }
        else {
            sysml_conditional_1 = true;
        }
        if (sysml_conditional_1) {
            sysml_conditional_2 = 37.0;
        }
        else {
            sysml_conditional_2 = 35.0;
        }
        sysml_state_targetTemp = sysml_conditional_2;
    }
    sysml_state_prevPressed = io_0;
    sysml_lazy_3 = io_7;
    if (sysml_lazy_3) {
        sysml_lazy_3 = sysml_state_tempLastValid >= 38.0;
    }
    if (sysml_lazy_3) {
        sysml_state_abnormalHighCount = (sysml_state_abnormalHighCount + 1);
    }
    else {
        sysml_state_abnormalHighCount = 0;
    }
    sysml_lazy_4 = io_7;
    if (sysml_lazy_4) {
        sysml_lazy_4 = sysml_state_tempLastValid <= 36.0;
    }
    if (sysml_lazy_4) {
        sysml_state_abnormalLowCount = (sysml_state_abnormalLowCount + 1);
    }
    else {
        sysml_state_abnormalLowCount = 0;
    }
    sysml_lazy_5 = sysml_state_abnormalHighCount >= 6000;
    if (!sysml_lazy_5) {
        sysml_lazy_5 = sysml_state_abnormalLowCount >= 6000;
    }
    if (sysml_lazy_5) {
        sysml_state_alarmActive = true;
    }
    else {
        sysml_state_alarmActive = false;
    }
    sysml_lazy_6 = !sysml_state_alarmActive;
    if (sysml_lazy_6) {
        sysml_lazy_6 = io_7;
    }
    if (sysml_lazy_6) {
        if (sysml_state_fanOn) {
            sysml_conditional_7 = false;
        }
        else {
            sysml_conditional_7 = true;
        }
        sysml_state_fanOn = sysml_conditional_7;
    }
    else {
        sysml_state_fanOn = false;
    }
    bool sysml_value_0 = 0;
    if (sysml_state_alarmActive) {
        sysml_conditional_8 = false;
    }
    else {
        sysml_lazy_9 = io_7;
        if (sysml_lazy_9) {
            sysml_lazy_9 = sysml_state_tempLastValid < sysml_state_targetTemp;
        }
        sysml_conditional_8 = sysml_lazy_9;
    }
    sysml_value_0 = sysml_conditional_8;
    bool sysml_value_1 = 0;
    sysml_value_1 = sysml_state_alarmActive;
    bool sysml_value_2 = 0;
    sysml_lazy_10 = sysml_state_fanOn;
    if (sysml_lazy_10) {
        sysml_lazy_10 = !sysml_state_alarmActive;
    }
    sysml_value_2 = sysml_lazy_10;
    int32_t sysml_value_3 = 0;
    if (sysml_state_alarmActive) {
        sysml_conditional_11 = 2000;
    }
    else {
        sysml_conditional_11 = 0;
    }
    sysml_value_3 = sysml_conditional_11;
    int32_t sysml_value_4 = 0;
    if (sysml_state_modeIsSetting) {
        if (sysml_state_targetTemp < ((double)((int32_t)sysml_state_targetTemp))) {
            sysml_conditional_13 = 1;
        }
        else {
            sysml_conditional_13 = 0;
        }
        sysml_conditional_12 = ((int32_t)sysml_state_targetTemp) - sysml_conditional_13;
    }
    else {
        if (sysml_state_tempLastValid < ((double)((int32_t)sysml_state_tempLastValid))) {
            sysml_conditional_14 = 1;
        }
        else {
            sysml_conditional_14 = 0;
        }
        sysml_conditional_12 = ((int32_t)sysml_state_tempLastValid) - sysml_conditional_14;
    }
    sysml_value_4 = sysml_conditional_12;
    bool sysml_value_5 = 0;
    sysml_value_5 = sysml_state_alarmActive;
    bool sysml_value_6 = 0;
    sysml_value_6 = sysml_state_alarmActive;
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
    *io_3 = sysml_value_2;
    *io_4 = sysml_value_3;
    *io_8 = sysml_value_4;
    *serial_0_trigger = sysml_value_5;
    *serial_0_value_0 = sysml_value_6;
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
static bool hw_value_io_3 = false;
static int32_t hw_value_io_4 = 0;
static int32_t hw_tone_io_4 = -1;
static double hw_value_io_5 = 0;
static double hw_value_io_6 = 0;
static bool hw_value_io_7 = false;
static int32_t hw_value_io_8 = 0;
TM1637Display hw_display_io_8(4, 3);
static int32_t hw_display_previous_io_8 = INT32_MIN;
static bool hw_value_serial_0_trigger = false;
static bool hw_value_serial_0_value_0 = false;
#include <DHT.h>
DHT hw_dht_0(7, DHT11);
static bool hw_dht_0_attempted = false;
static uint32_t hw_dht_0_sampled = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(2, INPUT);
  pinMode(10, OUTPUT);
  digitalWrite(10, hw_value_io_1 ? HIGH : LOW);
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_2 ? HIGH : LOW);
  pinMode(5, OUTPUT);
  digitalWrite(5, hw_value_io_3 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  int32_t hw_tone_command_io_4 = (hw_value_io_4 >= 31 && hw_value_io_4 <= 65535) ? hw_value_io_4 : 0;
  if (hw_tone_command_io_4 != hw_tone_io_4) {
    if (hw_tone_command_io_4 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_4);
    hw_tone_io_4 = hw_tone_command_io_4;
  }
  hw_display_io_8.setBrightness(0, true);
  if (hw_value_io_8 != hw_display_previous_io_8) {
    if (hw_value_io_8 < -999 || hw_value_io_8 > 9999) hw_display_io_8.clear();
    else hw_display_io_8.showNumberDec((int)hw_value_io_8, false, 4, 0);
    hw_display_previous_io_8 = hw_value_io_8;
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
    hw_value_io_0 = (digitalRead(2) == HIGH);
  }
  uint32_t hw_dht_0_now = (uint32_t)millis();
  if (!hw_dht_0_attempted || (uint32_t)(hw_dht_0_now - hw_dht_0_sampled) >= 2000UL) {
    hw_dht_0_attempted = true; hw_dht_0_sampled = hw_dht_0_now;
    bool hw_dht_0_valid = hw_dht_0.read();
    double hw_dht_0_io_5 = hw_dht_0_valid ? hw_dht_0.readTemperature() : NAN;
    double hw_dht_0_io_6 = hw_dht_0_valid ? hw_dht_0.readHumidity() : NAN;
    hw_dht_0_valid = hw_dht_0_valid && isfinite(hw_dht_0_io_5) && isfinite(hw_dht_0_io_6);
    if (hw_dht_0_valid) {
      hw_value_io_5 = hw_dht_0_io_5;
      hw_value_io_6 = hw_dht_0_io_6;
    }
    hw_value_io_7 = hw_dht_0_valid;
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_5, hw_value_io_6, hw_value_io_7, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_8, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0);
  digitalWrite(10, hw_value_io_1 ? HIGH : LOW);
  digitalWrite(13, hw_value_io_2 ? HIGH : LOW);
  digitalWrite(5, hw_value_io_3 ? HIGH : LOW);
  int32_t hw_tone_command_io_4 = (hw_value_io_4 >= 31 && hw_value_io_4 <= 65535) ? hw_value_io_4 : 0;
  if (hw_tone_command_io_4 != hw_tone_io_4) {
    if (hw_tone_command_io_4 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_4);
    hw_tone_io_4 = hw_tone_command_io_4;
  }
  if (hw_value_io_8 != hw_display_previous_io_8) {
    if (hw_value_io_8 < -999 || hw_value_io_8 > 9999) hw_display_io_8.clear();
    else hw_display_io_8.showNumberDec((int)hw_value_io_8, false, 4, 0);
    hw_display_previous_io_8 = hw_value_io_8;
  }
  if (hw_value_serial_0_trigger) { Serial.print("incubation_abnormal"); Serial.println(hw_value_serial_0_value_0); }
}

