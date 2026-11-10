#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_gpt61_failures_20261020\\advanced_018\\workflow\\attempt_0\\sketch\\sketch.ino"
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
extern bool sysml_state_component_showSetting_value;
/* Global Variable define of  */
extern bool sysml_state_component_showSetting_previous;
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
extern bool sysml_state_heaterOn;
/* Global Variable define of  */
extern int32_t sysml_state_previousSide;
/* Global Variable define of  */
extern int32_t sysml_state_outsideDuration;
/* Global Variable define of  */
extern bool sysml_state_previousAlarm;
/* Global Variable define of  */
extern bool sysml_state_reportPulse;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, bool io_7, int32_t io_8, bool* io_2, bool* io_3, int32_t* io_4, int32_t* io_5, bool* io_6, bool* io_9);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_component_showSetting_value = false;
/* Global Variable define of  */
bool sysml_state_component_showSetting_previous = false;
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
bool sysml_state_heaterOn = false;
/* Global Variable define of  */
int32_t sysml_state_previousSide = 0;
/* Global Variable define of  */
int32_t sysml_state_outsideDuration = 0;
/* Global Variable define of  */
bool sysml_state_previousAlarm = false;
/* Global Variable define of  */
bool sysml_state_reportPulse = false;
/* Init function of CompositeActor named "ProgramPage1" */
#line 468 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_gpt61_failures_20261020\\advanced_018\\workflow\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 494 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_gpt61_failures_20261020\\advanced_018\\workflow\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 73 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_gpt61_failures_20261020\\advanced_018\\workflow\\attempt_0\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_component_showSetting_value = false;
    sysml_state_component_showSetting_previous = false;
    sysml_state_component_ventilation_phase = 0;
    sysml_state_component_ventilation_started = false;
    sysml_state_component_ventilation_enabled = false;
    sysml_state_component_ventilation_remainder = 0;
    sysml_state_component_ventilation_value = false;
    sysml_state_heaterOn = false;
    sysml_state_previousSide = 0;
    sysml_state_outsideDuration = 0;
    sysml_state_previousAlarm = false;
    sysml_state_reportPulse = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, bool io_7, int32_t io_8, bool* io_2, bool* io_3, int32_t* io_4, int32_t* io_5, bool* io_6, bool* io_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    int32_t sysml_conditional_4 = 0;
    int32_t sysml_conditional_5 = 0;
    int32_t sysml_conditional_6 = 0;
    bool sysml_lazy_7 = 0;
    int32_t sysml_conditional_8 = 0;
    int32_t sysml_conditional_9 = 0;
    int32_t sysml_conditional_10 = 0;
    int32_t sysml_conditional_11 = 0;
    int32_t sysml_conditional_12 = 0;
    int32_t sysml_conditional_13 = 0;
    bool sysml_lazy_14 = 0;
    int32_t sysml_conditional_15 = 0;
    int32_t sysml_conditional_16 = 0;
    int32_t sysml_conditional_17 = 0;
    bool sysml_lazy_18 = 0;
    bool sysml_lazy_19 = 0;
    bool sysml_lazy_20 = 0;
    int32_t sysml_conditional_21 = 0;
    int32_t sysml_conditional_22 = 0;
    int32_t sysml_conditional_23 = 0;
    bool sysml_lazy_24 = 0;
    int32_t sysml_conditional_25 = 0;
    int32_t sysml_conditional_26 = 0;
    int32_t sysml_conditional_27 = 0;
    int32_t sysml_conditional_28 = 0;
    int32_t sysml_conditional_29 = 0;
    bool sysml_lazy_30 = 0;
    bool sysml_lazy_31 = 0;
    int32_t sysml_conditional_32 = 0;
    int32_t sysml_conditional_33 = 0;
    bool sysml_lazy_34 = 0;
    bool sysml_lazy_35 = 0;
    int32_t sysml_conditional_36 = 0;
    sysml_lazy_0 = io_7;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_component_showSetting_previous;
    }
    if (sysml_lazy_0) {
        sysml_state_component_showSetting_value = (!sysml_state_component_showSetting_value);
    }
    sysml_state_component_showSetting_previous = io_7;
    sysml_lazy_1 = sysml_state_component_ventilation_enabled;
    if (sysml_lazy_1) {
        sysml_lazy_1 = io_8 > 0;
    }
    if (sysml_lazy_1) {
        if ((io_8 >= (300000 - sysml_state_component_ventilation_phase))) {
            sysml_state_component_ventilation_started = true;
        }
        sysml_state_component_ventilation_remainder = io_8;
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
        sysml_lazy_3 = sysml_state_component_ventilation_phase < 30000;
    }
    sysml_state_component_ventilation_value = sysml_lazy_3;
    sysml_state_component_ventilation_enabled = true;
    if ((!io_1)) {
        sysml_state_heaterOn = false;
    }
    else {
        if ((io_0 < (37.0))) {
            sysml_state_heaterOn = true;
        }
        else {
            if ((io_0 >= (39.0))) {
                sysml_state_heaterOn = false;
            }
        }
    }
    if (io_1) {
        if (io_0 < 37.0) {
            sysml_conditional_5 = -1;
        }
        else {
            if (io_0 > 39.0) {
                sysml_conditional_6 = 1;
            }
            else {
                sysml_conditional_6 = 0;
            }
            sysml_conditional_5 = sysml_conditional_6;
        }
        sysml_conditional_4 = sysml_conditional_5;
    }
    else {
        sysml_conditional_4 = 0;
    }
    sysml_lazy_7 = sysml_conditional_4 != 0;
    if (sysml_lazy_7) {
        if (io_1) {
            if (io_0 < 37.0) {
                sysml_conditional_9 = -1;
            }
            else {
                if (io_0 > 39.0) {
                    sysml_conditional_10 = 1;
                }
                else {
                    sysml_conditional_10 = 0;
                }
                sysml_conditional_9 = sysml_conditional_10;
            }
            sysml_conditional_8 = sysml_conditional_9;
        }
        else {
            sysml_conditional_8 = 0;
        }
        sysml_lazy_7 = sysml_conditional_8 == sysml_state_previousSide;
    }
    if (sysml_lazy_7) {
        if ((io_8 > 0)) {
            if ((io_8 >= (60000 - sysml_state_outsideDuration))) {
                sysml_state_outsideDuration = 60000;
            }
            else {
                sysml_state_outsideDuration = (sysml_state_outsideDuration + io_8);
            }
        }
    }
    else {
        sysml_state_outsideDuration = 0;
    }
    if (io_1) {
        if (io_0 < 37.0) {
            sysml_conditional_12 = -1;
        }
        else {
            if (io_0 > 39.0) {
                sysml_conditional_13 = 1;
            }
            else {
                sysml_conditional_13 = 0;
            }
            sysml_conditional_12 = sysml_conditional_13;
        }
        sysml_conditional_11 = sysml_conditional_12;
    }
    else {
        sysml_conditional_11 = 0;
    }
    sysml_state_previousSide = sysml_conditional_11;
    sysml_lazy_14 = io_1;
    if (sysml_lazy_14) {
        if (io_1) {
            if (io_0 < 37.0) {
                sysml_conditional_16 = -1;
            }
            else {
                if (io_0 > 39.0) {
                    sysml_conditional_17 = 1;
                }
                else {
                    sysml_conditional_17 = 0;
                }
                sysml_conditional_16 = sysml_conditional_17;
            }
            sysml_conditional_15 = sysml_conditional_16;
        }
        else {
            sysml_conditional_15 = 0;
        }
        sysml_lazy_14 = sysml_conditional_15 != 0;
    }
    sysml_lazy_18 = sysml_lazy_14;
    if (sysml_lazy_18) {
        sysml_lazy_18 = sysml_state_outsideDuration >= 60000;
    }
    sysml_lazy_19 = sysml_lazy_18;
    if (sysml_lazy_19) {
        sysml_lazy_19 = !sysml_state_previousAlarm;
    }
    sysml_state_reportPulse = sysml_lazy_19;
    sysml_lazy_20 = io_1;
    if (sysml_lazy_20) {
        if (io_1) {
            if (io_0 < 37.0) {
                sysml_conditional_22 = -1;
            }
            else {
                if (io_0 > 39.0) {
                    sysml_conditional_23 = 1;
                }
                else {
                    sysml_conditional_23 = 0;
                }
                sysml_conditional_22 = sysml_conditional_23;
            }
            sysml_conditional_21 = sysml_conditional_22;
        }
        else {
            sysml_conditional_21 = 0;
        }
        sysml_lazy_20 = sysml_conditional_21 != 0;
    }
    sysml_lazy_24 = sysml_lazy_20;
    if (sysml_lazy_24) {
        sysml_lazy_24 = sysml_state_outsideDuration >= 60000;
    }
    sysml_state_previousAlarm = sysml_lazy_24;
    double sysml_value_0 = 0;
    sysml_value_0 = 37.0;
    double sysml_value_1 = 0;
    sysml_value_1 = 39.0;
    double sysml_value_2 = 0;
    sysml_value_2 = 38.0;
    bool sysml_value_3 = 0;
    sysml_value_3 = sysml_state_heaterOn;
    bool sysml_value_4 = 0;
    sysml_value_4 = sysml_state_component_ventilation_value;
    bool sysml_value_5 = 0;
    sysml_value_5 = sysml_state_reportPulse;
    int32_t sysml_value_6 = 0;
    if (io_1) {
        if (io_0 < sysml_value_0) {
            sysml_conditional_26 = -1;
        }
        else {
            if (io_0 > sysml_value_1) {
                sysml_conditional_27 = 1;
            }
            else {
                sysml_conditional_27 = 0;
            }
            sysml_conditional_26 = sysml_conditional_27;
        }
        sysml_conditional_25 = sysml_conditional_26;
    }
    else {
        sysml_conditional_25 = 0;
    }
    sysml_value_6 = sysml_conditional_25;
    int32_t sysml_value_7 = 0;
    if (sysml_state_component_showSetting_value) {
        if ((sysml_value_2 + 0.5) < ((double)((int32_t)(sysml_value_2 + 0.5)))) {
            sysml_conditional_29 = 1;
        }
        else {
            sysml_conditional_29 = 0;
        }
        sysml_conditional_28 = ((int32_t)(sysml_value_2 + 0.5)) - sysml_conditional_29;
    }
    else {
        sysml_lazy_30 = io_1;
        if (sysml_lazy_30) {
            sysml_lazy_30 = io_0 >= (-999.0);
        }
        sysml_lazy_31 = sysml_lazy_30;
        if (sysml_lazy_31) {
            sysml_lazy_31 = io_0 < 9999.5;
        }
        if (sysml_lazy_31) {
            if ((io_0 + 0.5) < ((double)((int32_t)(io_0 + 0.5)))) {
                sysml_conditional_33 = 1;
            }
            else {
                sysml_conditional_33 = 0;
            }
            sysml_conditional_32 = ((int32_t)(io_0 + 0.5)) - sysml_conditional_33;
        }
        else {
            sysml_conditional_32 = 10000;
        }
        sysml_conditional_28 = sysml_conditional_32;
    }
    sysml_value_7 = sysml_conditional_28;
    bool sysml_value_8 = 0;
    sysml_lazy_34 = io_1;
    if (sysml_lazy_34) {
        sysml_lazy_34 = sysml_value_6 != 0;
    }
    sysml_lazy_35 = sysml_lazy_34;
    if (sysml_lazy_35) {
        sysml_lazy_35 = sysml_state_outsideDuration >= 60000;
    }
    sysml_value_8 = sysml_lazy_35;
    int32_t sysml_value_9 = 0;
    if (sysml_value_8) {
        sysml_conditional_36 = 2000;
    }
    else {
        sysml_conditional_36 = 0;
    }
    sysml_value_9 = sysml_conditional_36;
    bool sysml_value_10 = 0;
    sysml_value_10 = sysml_value_8;
    *io_2 = sysml_value_3;
    *io_3 = sysml_value_4;
    *io_4 = sysml_value_9;
    *io_5 = sysml_value_7;
    *io_6 = sysml_value_10;
    *io_9 = sysml_value_5;
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
static int32_t hw_value_io_8 = 0;
static bool hw_value_io_9 = false;
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
  pinMode(2, INPUT);
  hw_dht_0.begin();
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_7 || (uint32_t)(hw_now - hw_sampled_io_7) >= 10UL) {
    hw_attempted_io_7 = true; hw_sampled_io_7 = hw_now;
    hw_value_io_7 = (digitalRead(2) == HIGH);
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
  hw_value_io_8 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_7, hw_value_io_8, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_io_9);
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
  if (hw_value_io_9) Serial.println("incubation abnormal");
}

