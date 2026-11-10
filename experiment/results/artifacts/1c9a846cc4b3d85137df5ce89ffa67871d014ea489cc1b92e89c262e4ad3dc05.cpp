#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_humidity_sensor_basic\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
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
extern double sysml_state_lastHumidity;
/* Global Variable define of  */
extern bool sysml_state_initialized;
/* Global Variable define of  */
extern int32_t sysml_state_nextReportDue;
/* Global Variable define of  */
extern bool sysml_state_serialGate;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, int32_t io_2, bool* io_1, bool* serial_0_trigger, double* serial_0_value_0);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_lastHumidity = 0.0;
/* Global Variable define of  */
bool sysml_state_initialized = false;
/* Global Variable define of  */
int32_t sysml_state_nextReportDue = 0;
/* Global Variable define of  */
bool sysml_state_serialGate = false;
/* Init function of CompositeActor named "ProgramPage1" */
#line 99 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_humidity_sensor_basic\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 105 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_humidity_sensor_basic\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 41 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_humidity_sensor_basic\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_lastHumidity = 0.0;
    sysml_state_initialized = false;
    sysml_state_nextReportDue = 0;
    sysml_state_serialGate = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, int32_t io_2, bool* io_1, bool* serial_0_trigger, double* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    if ((!sysml_state_initialized)) {
        sysml_state_initialized = true;
        sysml_state_nextReportDue = 0;
        sysml_state_serialGate = false;
    }
    else {
        sysml_state_nextReportDue = (sysml_state_nextReportDue + io_2);
        if ((sysml_state_nextReportDue >= 2000)) {
            sysml_state_nextReportDue = (sysml_state_nextReportDue - 2000);
            sysml_state_serialGate = true;
        }
        else {
            sysml_state_serialGate = false;
        }
        if ((!(!sysml_state_initialized))) {
        }
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_serialGate;
    bool sysml_value_1 = 0;
    sysml_value_1 = sysml_state_serialGate;
    double sysml_value_2 = 0;
    sysml_value_2 = sysml_state_lastHumidity;
    *io_1 = sysml_value_0;
    *serial_0_trigger = sysml_value_1;
    *serial_0_value_0 = sysml_value_2;
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
static double hw_value_io_0 = 0;
static bool hw_value_io_1 = false;
static int32_t hw_value_io_2 = 0;
static bool hw_value_serial_0_trigger = false;
static double hw_value_serial_0_value_0 = 0;
#include <DHT.h>
DHT hw_dht_0(2, DHT22);
static bool hw_dht_0_attempted = false;
static uint32_t hw_dht_0_sampled = 0;
static bool hw_dht_0_ready = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  hw_dht_0.begin();
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  uint32_t hw_dht_0_now = (uint32_t)millis();
  if (!hw_dht_0_attempted || (uint32_t)(hw_dht_0_now - hw_dht_0_sampled) >= 2000UL) {
    hw_dht_0_attempted = true; hw_dht_0_sampled = hw_dht_0_now;
    bool hw_dht_0_valid = hw_dht_0.read();
    double hw_dht_0_io_0 = hw_dht_0_valid ? hw_dht_0.readHumidity() : NAN;
    hw_dht_0_valid = hw_dht_0_valid && isfinite(hw_dht_0_io_0);
    if (hw_dht_0_valid) {
      hw_value_io_0 = hw_dht_0_io_0;
      hw_dht_0_ready = true;
    }
  }
  if (!hw_dht_0_ready) return;
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_2 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_2, &hw_value_io_1, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0);
  if (hw_value_io_1) Serial.println("Humidity: ");
  if (hw_value_serial_0_trigger) { Serial.print("humidity"); Serial.println(hw_value_serial_0_value_0); }
}

