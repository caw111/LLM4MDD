#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_027\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
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
extern int32_t sysml_state_pollTimerMs;
/* Global Variable define of  */
extern int32_t sysml_state_distanceBelowCount;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_1, double io_2, bool io_3, double io_4, bool io_5, int32_t io_6, bool* io_0, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, bool* serial_1_trigger, bool* serial_1_value_0);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_pollTimerMs = 0;
/* Global Variable define of  */
int32_t sysml_state_distanceBelowCount = 0;
/* Init function of CompositeActor named "ProgramPage1" */
#line 109 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_027\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 120 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_027\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 33 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_027\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_pollTimerMs = 0;
    sysml_state_distanceBelowCount = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_1, double io_2, bool io_3, double io_4, bool io_5, int32_t io_6, bool* io_0, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, bool* serial_1_trigger, bool* serial_1_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    sysml_state_pollTimerMs = (sysml_state_pollTimerMs + io_6);
    if ((sysml_state_pollTimerMs >= 3000)) {
        sysml_state_pollTimerMs = (sysml_state_pollTimerMs - 3000);
        if (io_5) {
            if ((io_4 < 20.0)) {
                sysml_state_distanceBelowCount = (sysml_state_distanceBelowCount + 1);
            }
            else {
                sysml_state_distanceBelowCount = 0;
            }
        }
        else {
            sysml_state_distanceBelowCount = 0;
        }
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = (sysml_state_distanceBelowCount >= 3);
    bool sysml_value_1 = 0;
    sysml_value_1 = (sysml_state_pollTimerMs >= 3000);
    double sysml_value_2 = 0;
    sysml_value_2 = io_1;
    double sysml_value_3 = 0;
    sysml_value_3 = io_4;
    bool sysml_value_4 = 0;
    sysml_lazy_0 = sysml_state_pollTimerMs >= 3000;
    if (sysml_lazy_0) {
        sysml_lazy_0 = sysml_state_distanceBelowCount >= 3;
    }
    sysml_value_4 = sysml_lazy_0;
    bool sysml_value_5 = 0;
    sysml_value_5 = (sysml_state_distanceBelowCount >= 3);
    *io_0 = sysml_value_0;
    *serial_0_trigger = sysml_value_1;
    *serial_0_value_0 = sysml_value_2;
    *serial_0_value_1 = sysml_value_3;
    *serial_1_trigger = sysml_value_4;
    *serial_1_value_0 = sysml_value_5;
}
// END COMPILED SYSML MODEL

// LLM4MDD target-board: arduino:avr:uno
#include <Arduino.h>
#include <math.h>
#if defined(ARDUINO) && !defined(ARDUINO_AVR_UNO)
#error "Selected Arduino board differs from the interface contract"
#endif
static uint32_t hw_last_step = 0;
static bool hw_started = false;
static bool hw_value_io_0 = false;
static double hw_value_io_1 = 0;
static double hw_value_io_2 = 0;
static bool hw_value_io_3 = false;
static double hw_value_io_4 = 0;
static bool hw_value_io_5 = false;
static int32_t hw_value_io_6 = 0;
static bool hw_value_serial_0_trigger = false;
static double hw_value_serial_0_value_0 = 0;
static double hw_value_serial_0_value_1 = 0;
static bool hw_value_serial_1_trigger = false;
static bool hw_value_serial_1_value_0 = false;
static uint32_t hw_ultrasonic_sampled_0 = 0;
static bool hw_ultrasonic_attempted_0 = false;
#include <DHT.h>
DHT hw_dht_0(7, DHT11);
static bool hw_dht_0_attempted = false;
static uint32_t hw_dht_0_sampled = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_0 ? HIGH : LOW);
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
  if (!hw_ultrasonic_attempted_0 || (uint32_t)(hw_ultrasonic_now_0 - hw_ultrasonic_sampled_0) >= 3000UL) {
    hw_ultrasonic_attempted_0 = true; hw_ultrasonic_sampled_0 = hw_ultrasonic_now_0;
    digitalWrite(8, LOW); delayMicroseconds(2);
    digitalWrite(8, HIGH); delayMicroseconds(10); digitalWrite(8, LOW);
    unsigned long hw_ultrasonic_pulse_0 = pulseIn(9, HIGH, 30000UL);
    hw_value_io_5 = (hw_ultrasonic_pulse_0 >= 116UL && hw_ultrasonic_pulse_0 <= 23200UL);
    if (hw_value_io_5) hw_value_io_4 = hw_ultrasonic_pulse_0 / 58.0;
  }
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
  hw_value_io_6 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_1, hw_value_io_2, hw_value_io_3, hw_value_io_4, hw_value_io_5, hw_value_io_6, &hw_value_io_0, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0, &hw_value_serial_0_value_1, &hw_value_serial_1_trigger, &hw_value_serial_1_value_0);
  digitalWrite(13, hw_value_io_0 ? HIGH : LOW);
  if (hw_value_serial_0_trigger) { Serial.print("Temperature"); Serial.println(hw_value_serial_0_value_0); }
  if (hw_value_serial_0_trigger) { Serial.print("Distance"); Serial.println(hw_value_serial_0_value_1); }
  if (hw_value_serial_1_trigger) { Serial.print("PersistentDetectedGate"); Serial.println(hw_value_serial_1_value_0); }
}

