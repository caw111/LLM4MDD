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
extern int32_t sysml_state_accum3sMs;
/* Global Variable define of  */
extern int32_t sysml_state_consecCloseCount;
/* Global Variable define of  */
extern double sysml_state_prevDhtTemp;
/* Global Variable define of  */
extern double sysml_state_prevDhtHum;
/* Global Variable define of  */
extern double sysml_state_prevDistCm;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_1, double io_2, bool io_3, double io_4, bool io_5, int32_t io_6, int32_t io_7, bool io_7_valid, bool* io_0, bool* io_8, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, double* serial_0_value_2, bool* serial_0_value_3, bool* serial_0_value_4);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_accum3sMs = 0;
/* Global Variable define of  */
int32_t sysml_state_consecCloseCount = 0;
/* Global Variable define of  */
double sysml_state_prevDhtTemp = 0.0;
/* Global Variable define of  */
double sysml_state_prevDhtHum = 0.0;
/* Global Variable define of  */
double sysml_state_prevDistCm = 0.0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_accum3sMs = 0;
    sysml_state_consecCloseCount = 0;
    sysml_state_prevDhtTemp = 0.0;
    sysml_state_prevDhtHum = 0.0;
    sysml_state_prevDistCm = 0.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_1, double io_2, bool io_3, double io_4, bool io_5, int32_t io_6, int32_t io_7, bool io_7_valid, bool* io_0, bool* io_8, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, double* serial_0_value_2, bool* serial_0_value_3, bool* serial_0_value_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    double sysml_conditional_5 = 0;
    double sysml_conditional_6 = 0;
    double sysml_conditional_7 = 0;
    sysml_state_accum3sMs = (sysml_state_accum3sMs + io_6);
    if ((sysml_state_accum3sMs >= 3000)) {
        sysml_state_accum3sMs = (sysml_state_accum3sMs - 3000);
    }
    if (io_5) {
        if ((io_4 < 20.0)) {
            sysml_state_consecCloseCount = (sysml_state_consecCloseCount + 1);
        }
        else {
            sysml_state_consecCloseCount = 0;
        }
    }
    else {
        sysml_state_consecCloseCount = 0;
    }
    if (io_3) {
        sysml_state_prevDhtTemp = io_1;
        sysml_state_prevDhtHum = io_2;
    }
    if (io_5) {
        sysml_state_prevDistCm = io_4;
    }
    if ((sysml_state_consecCloseCount > 3)) {
        sysml_state_consecCloseCount = 3;
    }
    bool sysml_value_0 = 0;
    sysml_lazy_0 = sysml_state_consecCloseCount >= 3;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_5;
    }
    sysml_value_0 = sysml_lazy_0;
    bool sysml_value_1 = 0;
    sysml_lazy_1 = sysml_state_consecCloseCount >= 3;
    if (sysml_lazy_1) {
        sysml_lazy_1 = io_5;
    }
    sysml_lazy_2 = sysml_lazy_1;
    if (sysml_lazy_2) {
        sysml_lazy_2 = sysml_state_accum3sMs >= 0;
    }
    sysml_value_1 = sysml_lazy_2;
    bool sysml_value_2 = 0;
    sysml_lazy_3 = sysml_state_consecCloseCount >= 0;
    if (sysml_lazy_3) {
        sysml_lazy_4 = sysml_state_accum3sMs >= 3000;
        if (!sysml_lazy_4) {
            sysml_lazy_4 = (sysml_state_accum3sMs + io_6) >= 3000;
        }
        sysml_lazy_3 = sysml_lazy_4;
    }
    sysml_value_2 = sysml_lazy_3;
    double sysml_value_3 = 0;
    if (io_3) {
        sysml_conditional_5 = io_1;
    }
    else {
        sysml_conditional_5 = sysml_state_prevDhtTemp;
    }
    sysml_value_3 = sysml_conditional_5;
    double sysml_value_4 = 0;
    if (io_3) {
        sysml_conditional_6 = io_2;
    }
    else {
        sysml_conditional_6 = sysml_state_prevDhtHum;
    }
    sysml_value_4 = sysml_conditional_6;
    double sysml_value_5 = 0;
    if (io_5) {
        sysml_conditional_7 = io_4;
    }
    else {
        sysml_conditional_7 = sysml_state_prevDistCm;
    }
    sysml_value_5 = sysml_conditional_7;
    bool sysml_value_6 = 0;
    sysml_value_6 = io_5;
    bool sysml_value_7 = 0;
    sysml_value_7 = io_3;
    *io_0 = sysml_value_0;
    *io_8 = sysml_value_1;
    *serial_0_trigger = sysml_value_2;
    *serial_0_value_0 = sysml_value_3;
    *serial_0_value_1 = sysml_value_4;
    *serial_0_value_2 = sysml_value_5;
    *serial_0_value_3 = sysml_value_6;
    *serial_0_value_4 = sysml_value_7;
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
static int32_t hw_value_io_7 = 0;
#include <string.h>
static char hw_rx_buffer[64];
static uint8_t hw_rx_size = 0;
static bool hw_rx_overflow = false;
static uint32_t hw_rx_last_byte = 0;
static bool hw_rx_parse(int32_t* value) {
  hw_rx_buffer[hw_rx_size] = '\0';
  char* begin = hw_rx_buffer;
  while (*begin == ' ' || *begin == '\t') ++begin;
  char* end = hw_rx_buffer + hw_rx_size;
  while (end > begin && (end[-1] == ' ' || end[-1] == '\t')) *--end = '\0';
  if (!*begin) return false;
  if (strcmp(begin, "ON") == 0) { *value = 1L; return true; }
  if (strcmp(begin, "OFF") == 0) { *value = 2L; return true; }
  return false;
}
static bool hw_rx_finish(int32_t* value) {
  bool accepted = !hw_rx_overflow && hw_rx_parse(value);
  hw_rx_size = 0; hw_rx_overflow = false;
  return accepted;
}
static bool hw_rx_poll(uint32_t hw_now, int32_t* value) {
  for (uint8_t budget = 0; budget < 32 && Serial.available() > 0; ++budget) {
    int byte = Serial.read();
    if (byte < 0) break;
    hw_rx_last_byte = hw_now;
    if (byte == '\r' || byte == '\n') {
      if (!hw_rx_size && !hw_rx_overflow) continue;
      return hw_rx_finish(value);
    }
    if (byte == 0 || byte > 127) hw_rx_overflow = true;
    if (hw_rx_size < 63) hw_rx_buffer[hw_rx_size++] = (char)byte;
    else hw_rx_overflow = true;
  }
  if (Serial.available() > 0) return false;
  if ((hw_rx_size || hw_rx_overflow) && (uint32_t)(hw_now - hw_rx_last_byte) >= 20UL) return hw_rx_finish(value);
  return false;
}
static bool hw_value_io_7_valid = false;
static bool hw_value_io_8 = false;
static bool hw_value_serial_0_trigger = false;
static double hw_value_serial_0_value_0 = 0;
static double hw_value_serial_0_value_1 = 0;
static double hw_value_serial_0_value_2 = 0;
static bool hw_value_serial_0_value_3 = false;
static bool hw_value_serial_0_value_4 = false;
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
  hw_value_io_7_valid = hw_rx_poll(hw_now, &hw_value_io_7);
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_1, hw_value_io_2, hw_value_io_3, hw_value_io_4, hw_value_io_5, hw_value_io_6, hw_value_io_7, hw_value_io_7_valid, &hw_value_io_0, &hw_value_io_8, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0, &hw_value_serial_0_value_1, &hw_value_serial_0_value_2, &hw_value_serial_0_value_3, &hw_value_serial_0_value_4);
  digitalWrite(13, hw_value_io_0 ? HIGH : LOW);
  if (hw_value_io_8) Serial.println("persistent object detected");
  if (hw_value_serial_0_trigger) { Serial.print("temperature"); Serial.println(hw_value_serial_0_value_0); }
  if (hw_value_serial_0_trigger) { Serial.print("humidity"); Serial.println(hw_value_serial_0_value_1); }
  if (hw_value_serial_0_trigger) { Serial.print("distance"); Serial.println(hw_value_serial_0_value_2); }
  if (hw_value_serial_0_trigger) { Serial.print("distance_valid"); Serial.println(hw_value_serial_0_value_3); }
  if (hw_value_serial_0_trigger) { Serial.print("dht_valid"); Serial.println(hw_value_serial_0_value_4); }
}
