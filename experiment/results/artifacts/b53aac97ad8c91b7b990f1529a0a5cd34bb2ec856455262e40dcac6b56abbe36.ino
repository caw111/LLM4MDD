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
extern int32_t sysml_state_pollAccumMs;
/* Global Variable define of  */
extern int32_t sysml_state_distancePersistCount;
/* Global Variable define of  */
extern bool sysml_state_distanceDetected;
/* Global Variable define of  */
extern bool sysml_state_lastTemperatureValid;
/* Global Variable define of  */
extern bool sysml_state_lastDistanceValid;
/* Global Variable define of  */
extern bool sysml_state_prevDhtValid;
/* Global Variable define of  */
extern bool sysml_state_prevUltrasonicValid;
/* Global Variable define of  */
extern double sysml_state_lastTempSnapshot;
/* Global Variable define of  */
extern double sysml_state_lastDistSnapshot;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_1, double io_2, bool io_3, double io_4, bool io_5, int32_t io_6, bool* io_0, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, int32_t* serial_0_value_2, bool* serial_1_trigger, bool* serial_1_value_0);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_pollAccumMs = 0;
/* Global Variable define of  */
int32_t sysml_state_distancePersistCount = 0;
/* Global Variable define of  */
bool sysml_state_distanceDetected = false;
/* Global Variable define of  */
bool sysml_state_lastTemperatureValid = false;
/* Global Variable define of  */
bool sysml_state_lastDistanceValid = false;
/* Global Variable define of  */
bool sysml_state_prevDhtValid = false;
/* Global Variable define of  */
bool sysml_state_prevUltrasonicValid = false;
/* Global Variable define of  */
double sysml_state_lastTempSnapshot = 0.0;
/* Global Variable define of  */
double sysml_state_lastDistSnapshot = 0.0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_pollAccumMs = 0;
    sysml_state_distancePersistCount = 0;
    sysml_state_distanceDetected = false;
    sysml_state_lastTemperatureValid = false;
    sysml_state_lastDistanceValid = false;
    sysml_state_prevDhtValid = false;
    sysml_state_prevUltrasonicValid = false;
    sysml_state_lastTempSnapshot = 0.0;
    sysml_state_lastDistSnapshot = 0.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_1, double io_2, bool io_3, double io_4, bool io_5, int32_t io_6, bool* io_0, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, int32_t* serial_0_value_2, bool* serial_1_trigger, bool* serial_1_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    int32_t sysml_conditional_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_conditional_4 = 0;
    bool sysml_lazy_5 = 0;
    double sysml_conditional_6 = 0;
    double sysml_conditional_7 = 0;
    int32_t sysml_conditional_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_conditional_10 = 0;
    sysml_state_pollAccumMs = (sysml_state_pollAccumMs + io_6);
    if (io_3) {
        sysml_state_lastTempSnapshot = io_1;
        sysml_state_lastTemperatureValid = true;
    }
    else {
        sysml_state_lastTemperatureValid = false;
    }
    if (io_5) {
        sysml_state_lastDistSnapshot = io_4;
        sysml_state_lastDistanceValid = true;
    }
    else {
        sysml_state_lastDistanceValid = false;
    }
    if ((sysml_state_pollAccumMs >= 3000)) {
        sysml_state_pollAccumMs = (sysml_state_pollAccumMs - 3000);
        sysml_lazy_0 = sysml_state_lastDistanceValid;
        if (sysml_lazy_0) {
            sysml_lazy_0 = sysml_state_lastDistSnapshot < 20.0;
        }
        if (sysml_lazy_0) {
            sysml_state_distancePersistCount = (sysml_state_distancePersistCount + 1);
        }
        else {
            sysml_state_distancePersistCount = 0;
        }
        sysml_lazy_1 = sysml_state_lastDistanceValid;
        if (sysml_lazy_1) {
            sysml_lazy_1 = sysml_state_lastDistSnapshot < 20.0;
        }
        if (sysml_lazy_1) {
            sysml_conditional_2 = 1;
        }
        else {
            sysml_conditional_2 = 0;
        }
        if ((sysml_state_distancePersistCount + sysml_conditional_2) >= 3) {
            sysml_state_distanceDetected = true;
        }
        else {
            sysml_lazy_3 = sysml_state_lastDistanceValid;
            if (sysml_lazy_3) {
                sysml_lazy_3 = sysml_state_lastDistSnapshot < 20.0;
            }
            if (!sysml_lazy_3) {
                sysml_state_distanceDetected = false;
            }
        }
    }
    sysml_state_prevDhtValid = io_3;
    sysml_state_prevUltrasonicValid = io_5;
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_distanceDetected;
    bool sysml_value_1 = 0;
    if ((sysml_state_pollAccumMs + io_6) >= 3000) {
        sysml_lazy_5 = sysml_state_lastTemperatureValid;
        if (sysml_lazy_5) {
            sysml_lazy_5 = sysml_state_lastDistanceValid;
        }
        sysml_conditional_4 = sysml_lazy_5;
    }
    else {
        sysml_conditional_4 = false;
    }
    sysml_value_1 = sysml_conditional_4;
    double sysml_value_2 = 0;
    if ((sysml_state_pollAccumMs + io_6) >= 3000) {
        sysml_conditional_6 = sysml_state_lastTempSnapshot;
    }
    else {
        sysml_conditional_6 = 0.0;
    }
    sysml_value_2 = sysml_conditional_6;
    double sysml_value_3 = 0;
    if ((sysml_state_pollAccumMs + io_6) >= 3000) {
        sysml_conditional_7 = sysml_state_lastDistSnapshot;
    }
    else {
        sysml_conditional_7 = 0.0;
    }
    sysml_value_3 = sysml_conditional_7;
    int32_t sysml_value_4 = 0;
    if ((sysml_state_pollAccumMs + io_6) >= 3000) {
        sysml_conditional_8 = sysml_state_distancePersistCount;
    }
    else {
        sysml_conditional_8 = 0;
    }
    sysml_value_4 = sysml_conditional_8;
    bool sysml_value_5 = 0;
    sysml_lazy_9 = (sysml_state_pollAccumMs + io_6) >= 3000;
    if (sysml_lazy_9) {
        sysml_lazy_9 = sysml_state_distanceDetected;
    }
    if (sysml_lazy_9) {
        sysml_conditional_10 = true;
    }
    else {
        sysml_conditional_10 = false;
    }
    sysml_value_5 = sysml_conditional_10;
    bool sysml_value_6 = 0;
    sysml_value_6 = sysml_state_distanceDetected;
    *io_0 = sysml_value_0;
    *serial_0_trigger = sysml_value_1;
    *serial_0_value_0 = sysml_value_2;
    *serial_0_value_1 = sysml_value_3;
    *serial_0_value_2 = sysml_value_4;
    *serial_1_trigger = sysml_value_5;
    *serial_1_value_0 = sysml_value_6;
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
static int32_t hw_value_serial_0_value_2 = 0;
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
  if (!hw_ultrasonic_attempted_0 || (uint32_t)(hw_ultrasonic_now_0 - hw_ultrasonic_sampled_0) >= 60UL) {
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
  ProgramPage1(hw_value_io_1, hw_value_io_2, hw_value_io_3, hw_value_io_4, hw_value_io_5, hw_value_io_6, &hw_value_io_0, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0, &hw_value_serial_0_value_1, &hw_value_serial_0_value_2, &hw_value_serial_1_trigger, &hw_value_serial_1_value_0);
  digitalWrite(13, hw_value_io_0 ? HIGH : LOW);
  if (hw_value_serial_0_trigger) { Serial.print("T"); Serial.println(hw_value_serial_0_value_0); }
  if (hw_value_serial_0_trigger) { Serial.print("D"); Serial.println(hw_value_serial_0_value_1); }
  if (hw_value_serial_0_trigger) { Serial.print("K"); Serial.println(hw_value_serial_0_value_2); }
  if (hw_value_serial_1_trigger) { Serial.print("detected"); Serial.println(hw_value_serial_1_value_0); }
}
