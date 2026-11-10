#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\repaired_workflow_continuation_20261016\\advanced_001\\workflow\\attempt_0\\sketch\\sketch.ino"
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
extern bool sysml_state_modeAuto;
/* Global Variable define of  */
extern bool sysml_state_manualTargetFan;
/* Global Variable define of  */
extern bool sysml_state_prevModeBtn;
/* Global Variable define of  */
extern bool sysml_state_prevTargetBtn;
/* Global Variable define of  */
extern bool sysml_state_manualFanActive;
/* Global Variable define of  */
extern bool sysml_state_manualLightActive;
/* Global Variable define of  */
extern bool sysml_state_manualPumpActive;
/* Global Variable define of  */
extern bool sysml_state_faultAlarmed;
/* Global Variable define of  */
extern bool sysml_state_prevFault;
/* Global Variable define of  */
extern int32_t sysml_state_displayIndex;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double io_2, double io_3, double io_4, double io_9, double io_10, bool io_11, double* io_5, bool* io_6, bool* io_7, int32_t* io_8, int32_t* io_12, bool* serial_0_trigger, bool* serial_0_value_0, double* serial_0_value_1, double* serial_0_value_2);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_modeAuto = true;
/* Global Variable define of  */
bool sysml_state_manualTargetFan = true;
/* Global Variable define of  */
bool sysml_state_prevModeBtn = false;
/* Global Variable define of  */
bool sysml_state_prevTargetBtn = false;
/* Global Variable define of  */
bool sysml_state_manualFanActive = false;
/* Global Variable define of  */
bool sysml_state_manualLightActive = false;
/* Global Variable define of  */
bool sysml_state_manualPumpActive = false;
/* Global Variable define of  */
bool sysml_state_faultAlarmed = false;
/* Global Variable define of  */
bool sysml_state_prevFault = false;
/* Global Variable define of  */
int32_t sysml_state_displayIndex = 0;
/* Init function of CompositeActor named "ProgramPage1" */
#line 312 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\repaired_workflow_continuation_20261016\\advanced_001\\workflow\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 342 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\repaired_workflow_continuation_20261016\\advanced_001\\workflow\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 65 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\repaired_workflow_continuation_20261016\\advanced_001\\workflow\\attempt_0\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_modeAuto = true;
    sysml_state_manualTargetFan = true;
    sysml_state_prevModeBtn = false;
    sysml_state_prevTargetBtn = false;
    sysml_state_manualFanActive = false;
    sysml_state_manualLightActive = false;
    sysml_state_manualPumpActive = false;
    sysml_state_faultAlarmed = false;
    sysml_state_prevFault = false;
    sysml_state_displayIndex = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double io_2, double io_3, double io_4, double io_9, double io_10, bool io_11, double* io_5, bool* io_6, bool* io_7, int32_t* io_8, int32_t* io_12, bool* serial_0_trigger, bool* serial_0_value_0, double* serial_0_value_1, double* serial_0_value_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_conditional_3 = 0;
    bool sysml_conditional_4 = 0;
    double sysml_conditional_5 = 0;
    double sysml_conditional_6 = 0;
    double sysml_conditional_7 = 0;
    bool sysml_conditional_8 = 0;
    bool sysml_conditional_9 = 0;
    bool sysml_conditional_10 = 0;
    bool sysml_conditional_11 = 0;
    bool sysml_conditional_12 = 0;
    bool sysml_conditional_13 = 0;
    int32_t sysml_conditional_14 = 0;
    int32_t sysml_conditional_15 = 0;
    int32_t sysml_conditional_16 = 0;
    int32_t sysml_conditional_17 = 0;
    int32_t sysml_conditional_18 = 0;
    int32_t sysml_conditional_19 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_prevModeBtn;
    }
    if (sysml_lazy_0) {
        if (sysml_state_modeAuto) {
            sysml_conditional_1 = false;
        }
        else {
            sysml_conditional_1 = true;
        }
        sysml_state_modeAuto = sysml_conditional_1;
    }
    sysml_lazy_2 = io_1;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_prevTargetBtn;
    }
    if (sysml_lazy_2) {
        if (sysml_state_manualTargetFan) {
            sysml_conditional_3 = false;
        }
        else {
            sysml_conditional_3 = true;
        }
        sysml_state_manualTargetFan = sysml_conditional_3;
    }
    sysml_state_prevModeBtn = io_0;
    sysml_state_prevTargetBtn = io_1;
    if ((!sysml_state_modeAuto)) {
        sysml_state_manualFanActive = sysml_state_manualTargetFan;
        if (sysml_state_manualTargetFan) {
            sysml_conditional_4 = false;
        }
        else {
            sysml_conditional_4 = true;
        }
        sysml_state_manualLightActive = sysml_conditional_4;
        sysml_state_manualPumpActive = false;
    }
    else {
        sysml_state_manualFanActive = false;
        sysml_state_manualLightActive = false;
        sysml_state_manualPumpActive = false;
    }
    sysml_state_faultAlarmed = (!io_11);
    sysml_state_prevFault = sysml_state_faultAlarmed;
    sysml_state_displayIndex = (sysml_state_displayIndex + 1);
    double sysml_value_0 = 0;
    if (sysml_state_modeAuto) {
        if (io_9 >= 30.0) {
            sysml_conditional_6 = 255.0;
        }
        else {
            sysml_conditional_6 = 0.0;
        }
        sysml_conditional_5 = sysml_conditional_6;
    }
    else {
        if (sysml_state_manualFanActive) {
            sysml_conditional_7 = io_2;
        }
        else {
            sysml_conditional_7 = 0.0;
        }
        sysml_conditional_5 = sysml_conditional_7;
    }
    sysml_value_0 = sysml_conditional_5;
    bool sysml_value_1 = 0;
    if (sysml_state_modeAuto) {
        if (io_3 <= 400.0) {
            sysml_conditional_9 = true;
        }
        else {
            sysml_conditional_9 = false;
        }
        sysml_conditional_8 = sysml_conditional_9;
    }
    else {
        if (sysml_state_manualLightActive) {
            sysml_conditional_10 = io_2 > 0.0;
        }
        else {
            sysml_conditional_10 = false;
        }
        sysml_conditional_8 = sysml_conditional_10;
    }
    sysml_value_1 = sysml_conditional_8;
    bool sysml_value_2 = 0;
    if (sysml_state_modeAuto) {
        if (io_4 <= 400.0) {
            sysml_conditional_12 = true;
        }
        else {
            sysml_conditional_12 = false;
        }
        sysml_conditional_11 = sysml_conditional_12;
    }
    else {
        if (sysml_state_manualPumpActive) {
            sysml_conditional_13 = io_2 > 0.0;
        }
        else {
            sysml_conditional_13 = false;
        }
        sysml_conditional_11 = sysml_conditional_13;
    }
    sysml_value_2 = sysml_conditional_11;
    int32_t sysml_value_3 = 0;
    if (sysml_state_faultAlarmed) {
        sysml_conditional_14 = 2000;
    }
    else {
        sysml_conditional_14 = 0;
    }
    sysml_value_3 = sysml_conditional_14;
    int32_t sysml_value_4 = 0;
    if (sysml_state_displayIndex == 0) {
        if (io_9 < ((double)((int32_t)io_9))) {
            sysml_conditional_16 = 1;
        }
        else {
            sysml_conditional_16 = 0;
        }
        sysml_conditional_15 = ((int32_t)io_9) - sysml_conditional_16;
    }
    else {
        if (sysml_state_displayIndex == 1) {
            if (io_3 < ((double)((int32_t)io_3))) {
                sysml_conditional_18 = 1;
            }
            else {
                sysml_conditional_18 = 0;
            }
            sysml_conditional_17 = ((int32_t)io_3) - sysml_conditional_18;
        }
        else {
            if (io_4 < ((double)((int32_t)io_4))) {
                sysml_conditional_19 = 1;
            }
            else {
                sysml_conditional_19 = 0;
            }
            sysml_conditional_17 = ((int32_t)io_4) - sysml_conditional_19;
        }
        sysml_conditional_15 = sysml_conditional_17;
    }
    sysml_value_4 = sysml_conditional_15;
    bool sysml_value_5 = 0;
    sysml_value_5 = sysml_state_faultAlarmed;
    bool sysml_value_6 = 0;
    sysml_value_6 = sysml_state_faultAlarmed;
    double sysml_value_7 = 0;
    sysml_value_7 = io_3;
    double sysml_value_8 = 0;
    sysml_value_8 = io_4;
    *io_5 = sysml_value_0;
    *io_6 = sysml_value_1;
    *io_7 = sysml_value_2;
    *io_8 = sysml_value_3;
    *io_12 = sysml_value_4;
    *serial_0_trigger = sysml_value_5;
    *serial_0_value_0 = sysml_value_6;
    *serial_0_value_1 = sysml_value_7;
    *serial_0_value_2 = sysml_value_8;
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
static uint32_t hw_sampled_io_1 = 0;
static bool hw_attempted_io_1 = false;
static double hw_value_io_2 = 0;
static uint32_t hw_sampled_io_2 = 0;
static bool hw_attempted_io_2 = false;
static double hw_value_io_3 = 0;
static uint32_t hw_sampled_io_3 = 0;
static bool hw_attempted_io_3 = false;
static double hw_value_io_4 = 0;
static uint32_t hw_sampled_io_4 = 0;
static bool hw_attempted_io_4 = false;
static double hw_value_io_5 = 0;
static bool hw_value_io_6 = false;
static bool hw_value_io_7 = false;
static int32_t hw_value_io_8 = 0;
static int32_t hw_tone_io_8 = -1;
static double hw_value_io_9 = 0;
static double hw_value_io_10 = 0;
static bool hw_value_io_11 = false;
static int32_t hw_value_io_12 = 10000;
TM1637Display hw_display_io_12(4, 8);
static int32_t hw_display_previous_io_12 = INT32_MIN;
static bool hw_value_serial_0_trigger = false;
static bool hw_value_serial_0_value_0 = false;
static double hw_value_serial_0_value_1 = 0;
static double hw_value_serial_0_value_2 = 0;
#include <DHT.h>
DHT hw_dht_0(7, DHT11);
static bool hw_dht_0_attempted = false;
static uint32_t hw_dht_0_sampled = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(A2, INPUT);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(5, OUTPUT);
  analogWrite(5, (int)constrain((hw_value_io_5 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(10, OUTPUT);
  digitalWrite(10, hw_value_io_6 ? HIGH : LOW);
  pinMode(9, OUTPUT);
  digitalWrite(9, hw_value_io_7 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  int32_t hw_tone_command_io_8 = (hw_value_io_8 >= 31 && hw_value_io_8 <= 65535) ? hw_value_io_8 : 0;
  if (hw_tone_command_io_8 != hw_tone_io_8) {
    if (hw_tone_command_io_8 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_8);
    hw_tone_io_8 = hw_tone_command_io_8;
  }
  hw_display_io_12.setBrightness(7, true);
  if (hw_value_io_12 != hw_display_previous_io_12) {
    if (hw_value_io_12 < -999 || hw_value_io_12 > 9999) hw_display_io_12.clear();
    else hw_display_io_12.showNumberDec((int)hw_value_io_12, false, 4, 0);
    hw_display_previous_io_12 = hw_value_io_12;
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
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 10UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = (digitalRead(3) == HIGH);
  }
  if (!hw_attempted_io_2 || (uint32_t)(hw_now - hw_sampled_io_2) >= 10UL) {
    hw_attempted_io_2 = true; hw_sampled_io_2 = hw_now;
    hw_value_io_2 = (analogRead(A2) * 1.0 + 0.0);
  }
  if (!hw_attempted_io_3 || (uint32_t)(hw_now - hw_sampled_io_3) >= 10UL) {
    hw_attempted_io_3 = true; hw_sampled_io_3 = hw_now;
    hw_value_io_3 = (analogRead(A0) * 1.0 + 0.0);
  }
  if (!hw_attempted_io_4 || (uint32_t)(hw_now - hw_sampled_io_4) >= 10UL) {
    hw_attempted_io_4 = true; hw_sampled_io_4 = hw_now;
    hw_value_io_4 = (analogRead(A1) * 1.0 + 0.0);
  }
  uint32_t hw_dht_0_now = (uint32_t)millis();
  if (!hw_dht_0_attempted || (uint32_t)(hw_dht_0_now - hw_dht_0_sampled) >= 2000UL) {
    hw_dht_0_attempted = true; hw_dht_0_sampled = hw_dht_0_now;
    bool hw_dht_0_valid = hw_dht_0.read();
    double hw_dht_0_io_9 = hw_dht_0_valid ? hw_dht_0.readTemperature() : NAN;
    double hw_dht_0_io_10 = hw_dht_0_valid ? hw_dht_0.readHumidity() : NAN;
    hw_dht_0_valid = hw_dht_0_valid && isfinite(hw_dht_0_io_9) && isfinite(hw_dht_0_io_10);
    if (hw_dht_0_valid) {
      hw_value_io_9 = hw_dht_0_io_9;
      hw_value_io_10 = hw_dht_0_io_10;
    }
    hw_value_io_11 = hw_dht_0_valid;
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, hw_value_io_3, hw_value_io_4, hw_value_io_9, hw_value_io_10, hw_value_io_11, &hw_value_io_5, &hw_value_io_6, &hw_value_io_7, &hw_value_io_8, &hw_value_io_12, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0, &hw_value_serial_0_value_1, &hw_value_serial_0_value_2);
  analogWrite(5, (int)constrain((hw_value_io_5 * 1.0 + 0.0), 0.0, 255.0));
  digitalWrite(10, hw_value_io_6 ? HIGH : LOW);
  digitalWrite(9, hw_value_io_7 ? HIGH : LOW);
  int32_t hw_tone_command_io_8 = (hw_value_io_8 >= 31 && hw_value_io_8 <= 65535) ? hw_value_io_8 : 0;
  if (hw_tone_command_io_8 != hw_tone_io_8) {
    if (hw_tone_command_io_8 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_8);
    hw_tone_io_8 = hw_tone_command_io_8;
  }
  if (hw_value_io_12 != hw_display_previous_io_12) {
    if (hw_value_io_12 < -999 || hw_value_io_12 > 9999) hw_display_io_12.clear();
    else hw_display_io_12.showNumberDec((int)hw_value_io_12, false, 4, 0);
    hw_display_previous_io_12 = hw_value_io_12;
  }
  if (hw_value_serial_0_trigger) { Serial.print("dht_valid"); Serial.println(hw_value_serial_0_value_0); }
  if (hw_value_serial_0_trigger) { Serial.print("light_value"); Serial.println(hw_value_serial_0_value_1); }
  if (hw_value_serial_0_trigger) { Serial.print("soil_value"); Serial.println(hw_value_serial_0_value_2); }
}

