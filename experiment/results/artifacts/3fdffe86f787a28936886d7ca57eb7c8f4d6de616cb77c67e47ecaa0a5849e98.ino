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
extern bool sysml_state_dhtValidLatched;
/* Global Variable define of  */
extern double sysml_state_tempLatched;
/* Global Variable define of  */
extern double sysml_state_distLatched;
/* Global Variable define of  */
extern bool sysml_state_alarmOn;
/* Global Variable define of  */
extern int32_t sysml_state_alarmPhaseMs;
/* Global Variable define of  */
extern bool sysml_state_prevAlarmOn;
/* Global Variable define of  */
extern bool sysml_state_lastReasonAny;
/* Global Variable define of  */
extern int32_t sysml_state_displaySwitchAccumMs;
/* Global Variable define of  */
extern bool sysml_state_displayIsTemp;
/* Global Variable define of  */
extern int32_t sysml_state_reportGateHoldMs;
/* Global Variable define of  */
extern int32_t sysml_state_reportEveryAccumMs;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, bool io_2, double io_3, bool io_4, int32_t io_7, int32_t* io_5, int32_t* io_6, bool* serial_0_trigger, bool* serial_0_value_0, bool* serial_0_value_1);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_dhtValidLatched = false;
/* Global Variable define of  */
double sysml_state_tempLatched = 0.0;
/* Global Variable define of  */
double sysml_state_distLatched = 0.0;
/* Global Variable define of  */
bool sysml_state_alarmOn = false;
/* Global Variable define of  */
int32_t sysml_state_alarmPhaseMs = 0;
/* Global Variable define of  */
bool sysml_state_prevAlarmOn = false;
/* Global Variable define of  */
bool sysml_state_lastReasonAny = false;
/* Global Variable define of  */
int32_t sysml_state_displaySwitchAccumMs = 0;
/* Global Variable define of  */
bool sysml_state_displayIsTemp = true;
/* Global Variable define of  */
int32_t sysml_state_reportGateHoldMs = 0;
/* Global Variable define of  */
int32_t sysml_state_reportEveryAccumMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_dhtValidLatched = false;
    sysml_state_tempLatched = 0.0;
    sysml_state_distLatched = 0.0;
    sysml_state_alarmOn = false;
    sysml_state_alarmPhaseMs = 0;
    sysml_state_prevAlarmOn = false;
    sysml_state_lastReasonAny = false;
    sysml_state_displaySwitchAccumMs = 0;
    sysml_state_displayIsTemp = true;
    sysml_state_reportGateHoldMs = 0;
    sysml_state_reportEveryAccumMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, bool io_2, double io_3, bool io_4, int32_t io_7, int32_t* io_5, int32_t* io_6, bool* serial_0_trigger, bool* serial_0_value_0, bool* serial_0_value_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    int32_t sysml_conditional_3 = 0;
    int32_t sysml_conditional_4 = 0;
    int32_t sysml_conditional_5 = 0;
    int32_t sysml_conditional_6 = 0;
    int32_t sysml_conditional_7 = 0;
    int32_t sysml_conditional_8 = 0;
    int32_t sysml_conditional_9 = 0;
    bool sysml_conditional_10 = 0;
    bool sysml_conditional_11 = 0;
    bool sysml_conditional_12 = 0;
    bool sysml_conditional_13 = 0;
    bool sysml_conditional_14 = 0;
    bool sysml_conditional_15 = 0;
    if (io_2) {
        sysml_state_dhtValidLatched = true;
        sysml_state_tempLatched = io_0;
    }
    if ((!io_2)) {
        sysml_state_dhtValidLatched = false;
    }
    if (io_4) {
        sysml_state_distLatched = io_3;
    }
    if ((!io_4)) {
        sysml_state_distLatched = sysml_state_distLatched;
    }
    sysml_state_displaySwitchAccumMs = (sysml_state_displaySwitchAccumMs + io_7);
    if ((sysml_state_displaySwitchAccumMs >= 2000)) {
        sysml_state_displaySwitchAccumMs = (sysml_state_displaySwitchAccumMs - 2000);
        sysml_state_displayIsTemp = (!sysml_state_displayIsTemp);
    }
    sysml_lazy_0 = sysml_state_dhtValidLatched;
    if (sysml_lazy_0) {
        sysml_lazy_0 = sysml_state_tempLatched > 32.0;
    }
    sysml_lazy_1 = sysml_lazy_0;
    if (!sysml_lazy_1) {
        sysml_lazy_2 = io_4;
        if (sysml_lazy_2) {
            sysml_lazy_2 = sysml_state_distLatched < 15.0;
        }
        sysml_lazy_1 = sysml_lazy_2;
    }
    sysml_state_alarmOn = sysml_lazy_1;
    if (sysml_state_alarmOn) {
        if (sysml_state_prevAlarmOn) {
            sysml_state_alarmPhaseMs = (sysml_state_alarmPhaseMs + io_7);
        }
        else {
            sysml_state_alarmPhaseMs = 0;
        }
    }
    else {
        sysml_state_alarmPhaseMs = sysml_state_alarmPhaseMs;
    }
    sysml_state_prevAlarmOn = sysml_state_alarmOn;
    sysml_state_lastReasonAny = sysml_state_alarmOn;
    if (sysml_state_alarmOn) {
        sysml_state_reportEveryAccumMs = (sysml_state_reportEveryAccumMs + io_7);
        if ((sysml_state_reportEveryAccumMs >= 1000)) {
            sysml_state_reportEveryAccumMs = (sysml_state_reportEveryAccumMs - 1000);
            sysml_state_reportGateHoldMs = 200;
        }
        else {
            sysml_state_reportGateHoldMs = sysml_state_reportGateHoldMs;
        }
    }
    else {
        sysml_state_reportEveryAccumMs = 0;
        sysml_state_reportGateHoldMs = 0;
    }
    if ((sysml_state_reportGateHoldMs > 0)) {
        sysml_state_reportGateHoldMs = (sysml_state_reportGateHoldMs - io_7);
    }
    else {
        sysml_state_reportGateHoldMs = sysml_state_reportGateHoldMs;
    }
    int32_t sysml_value_0 = 0;
    if (sysml_state_displayIsTemp) {
        if (sysml_state_dhtValidLatched) {
            if (sysml_state_tempLatched < ((double)((int32_t)sysml_state_tempLatched))) {
                sysml_conditional_5 = 1;
            }
            else {
                sysml_conditional_5 = 0;
            }
            sysml_conditional_4 = ((int32_t)sysml_state_tempLatched) - sysml_conditional_5;
        }
        else {
            sysml_conditional_4 = 0;
        }
        sysml_conditional_3 = sysml_conditional_4;
    }
    else {
        if (io_4) {
            if (sysml_state_distLatched < ((double)((int32_t)sysml_state_distLatched))) {
                sysml_conditional_7 = 1;
            }
            else {
                sysml_conditional_7 = 0;
            }
            sysml_conditional_6 = ((int32_t)sysml_state_distLatched) - sysml_conditional_7;
        }
        else {
            sysml_conditional_6 = 0;
        }
        sysml_conditional_3 = sysml_conditional_6;
    }
    sysml_value_0 = sysml_conditional_3;
    int32_t sysml_value_1 = 0;
    if (sysml_state_alarmOn) {
        if (sysml_state_alarmPhaseMs < 500) {
            sysml_conditional_9 = 1000;
        }
        else {
            sysml_conditional_9 = 0;
        }
        sysml_conditional_8 = sysml_conditional_9;
    }
    else {
        sysml_conditional_8 = 0;
    }
    sysml_value_1 = sysml_conditional_8;
    bool sysml_value_2 = 0;
    if (sysml_state_alarmOn) {
        if (sysml_state_reportGateHoldMs > 0) {
            sysml_conditional_11 = true;
        }
        else {
            sysml_conditional_11 = false;
        }
        sysml_conditional_10 = sysml_conditional_11;
    }
    else {
        sysml_conditional_10 = false;
    }
    sysml_value_2 = sysml_conditional_10;
    bool sysml_value_3 = 0;
    if (sysml_state_dhtValidLatched) {
        if (sysml_state_tempLatched > 32.0) {
            sysml_conditional_13 = true;
        }
        else {
            sysml_conditional_13 = false;
        }
        sysml_conditional_12 = sysml_conditional_13;
    }
    else {
        sysml_conditional_12 = false;
    }
    sysml_value_3 = sysml_conditional_12;
    bool sysml_value_4 = 0;
    if (io_4) {
        if (sysml_state_distLatched < 15.0) {
            sysml_conditional_15 = true;
        }
        else {
            sysml_conditional_15 = false;
        }
        sysml_conditional_14 = sysml_conditional_15;
    }
    else {
        sysml_conditional_14 = false;
    }
    sysml_value_4 = sysml_conditional_14;
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
    if (hw_value_io_5 < 0 || hw_value_io_5 > 9999) hw_display_io_5.clear();
    else hw_display_io_5.showNumberDec((int)hw_value_io_5, true, 4, 0);
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
    if (hw_value_io_5 < 0 || hw_value_io_5 > 9999) hw_display_io_5.clear();
    else hw_display_io_5.showNumberDec((int)hw_value_io_5, true, 4, 0);
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
