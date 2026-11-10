#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\repaired_workflow_continuation_20261016\\complex_026\\workflow\\attempt_0\\sketch\\sketch.ino"
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
extern bool sysml_state_previousPressed;
/* Global Variable define of  */
extern bool sysml_state_serialEnabled;
/* Global Variable define of  */
extern int32_t sysml_state_mode;
/* Global Variable define of  */
extern double sysml_state_dispTemp;
/* Global Variable define of  */
extern double sysml_state_dispHumi;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, double io_1, double io_2, bool io_3, int32_t io_5, bool io_5_valid, int32_t* io_4, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, bool* serial_0_value_2);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
bool sysml_state_serialEnabled = false;
/* Global Variable define of  */
int32_t sysml_state_mode = 1;
/* Global Variable define of  */
double sysml_state_dispTemp = 0.0;
/* Global Variable define of  */
double sysml_state_dispHumi = 0.0;
/* Init function of CompositeActor named "ProgramPage1" */
#line 221 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\repaired_workflow_continuation_20261016\\complex_026\\workflow\\attempt_0\\sketch\\sketch.ino"
static bool hw_rx_parse(int32_t* value);
#line 233 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\repaired_workflow_continuation_20261016\\complex_026\\workflow\\attempt_0\\sketch\\sketch.ino"
static bool hw_rx_finish(int32_t* value);
#line 238 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\repaired_workflow_continuation_20261016\\complex_026\\workflow\\attempt_0\\sketch\\sketch.ino"
static bool hw_rx_poll(uint32_t hw_now, int32_t* value);
#line 265 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\repaired_workflow_continuation_20261016\\complex_026\\workflow\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 278 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\repaired_workflow_continuation_20261016\\complex_026\\workflow\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 45 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\repaired_workflow_continuation_20261016\\complex_026\\workflow\\attempt_0\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousPressed = false;
    sysml_state_serialEnabled = false;
    sysml_state_mode = 1;
    sysml_state_dispTemp = 0.0;
    sysml_state_dispHumi = 0.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, double io_1, double io_2, bool io_3, int32_t io_5, bool io_5_valid, int32_t* io_4, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, bool* serial_0_value_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int32_t sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    int32_t sysml_conditional_3 = 0;
    int32_t sysml_conditional_4 = 0;
    int32_t sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    int32_t sysml_conditional_7 = 0;
    int32_t sysml_conditional_8 = 0;
    bool sysml_lazy_9 = 0;
    int32_t sysml_conditional_10 = 0;
    int32_t sysml_conditional_11 = 0;
    bool sysml_lazy_12 = 0;
    double sysml_conditional_13 = 0;
    bool sysml_lazy_14 = 0;
    double sysml_conditional_15 = 0;
    bool sysml_lazy_16 = 0;
    bool sysml_conditional_17 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        sysml_state_serialEnabled = (!sysml_state_serialEnabled);
    }
    sysml_state_previousPressed = io_0;
    if (io_5_valid) {
        sysml_state_mode = io_5;
    }
    if (io_3) {
        sysml_state_dispTemp = io_1;
        sysml_state_dispHumi = io_2;
    }
    int32_t sysml_value_0 = 0;
    if (sysml_state_mode == 2) {
        sysml_lazy_2 = sysml_state_dispHumi >= 0.0;
        if (sysml_lazy_2) {
            sysml_lazy_2 = sysml_state_dispHumi <= 2147483647.0;
        }
        if (sysml_lazy_2) {
            if (sysml_state_dispHumi < ((double)((int32_t)sysml_state_dispHumi))) {
                sysml_conditional_4 = 1;
            }
            else {
                sysml_conditional_4 = 0;
            }
            sysml_conditional_3 = ((int32_t)sysml_state_dispHumi) - sysml_conditional_4;
        }
        else {
            sysml_conditional_3 = 0;
        }
        sysml_conditional_1 = sysml_conditional_3;
    }
    else {
        if (sysml_state_mode == 3) {
            sysml_lazy_6 = sysml_state_dispTemp >= 0.0;
            if (sysml_lazy_6) {
                sysml_lazy_6 = sysml_state_dispTemp <= 2147483647.0;
            }
            if (sysml_lazy_6) {
                if (sysml_state_dispTemp < ((double)((int32_t)sysml_state_dispTemp))) {
                    sysml_conditional_8 = 1;
                }
                else {
                    sysml_conditional_8 = 0;
                }
                sysml_conditional_7 = ((int32_t)sysml_state_dispTemp) - sysml_conditional_8;
            }
            else {
                sysml_conditional_7 = 0;
            }
            sysml_conditional_5 = sysml_conditional_7;
        }
        else {
            sysml_lazy_9 = sysml_state_dispTemp >= 0.0;
            if (sysml_lazy_9) {
                sysml_lazy_9 = sysml_state_dispTemp <= 2147483647.0;
            }
            if (sysml_lazy_9) {
                if (sysml_state_dispTemp < ((double)((int32_t)sysml_state_dispTemp))) {
                    sysml_conditional_11 = 1;
                }
                else {
                    sysml_conditional_11 = 0;
                }
                sysml_conditional_10 = ((int32_t)sysml_state_dispTemp) - sysml_conditional_11;
            }
            else {
                sysml_conditional_10 = 0;
            }
            sysml_conditional_5 = sysml_conditional_10;
        }
        sysml_conditional_1 = sysml_conditional_5;
    }
    sysml_value_0 = sysml_conditional_1;
    bool sysml_value_1 = 0;
    sysml_value_1 = sysml_state_serialEnabled;
    double sysml_value_2 = 0;
    sysml_lazy_12 = sysml_state_serialEnabled;
    if (sysml_lazy_12) {
        sysml_lazy_12 = sysml_state_mode == 3;
    }
    if (sysml_lazy_12) {
        sysml_conditional_13 = sysml_state_dispTemp;
    }
    else {
        sysml_conditional_13 = 0.0;
    }
    sysml_value_2 = sysml_conditional_13;
    double sysml_value_3 = 0;
    sysml_lazy_14 = sysml_state_serialEnabled;
    if (sysml_lazy_14) {
        sysml_lazy_14 = sysml_state_mode == 2;
    }
    if (sysml_lazy_14) {
        sysml_conditional_15 = sysml_state_dispHumi;
    }
    else {
        sysml_conditional_15 = 0.0;
    }
    sysml_value_3 = sysml_conditional_15;
    bool sysml_value_4 = 0;
    sysml_lazy_16 = sysml_state_serialEnabled;
    if (sysml_lazy_16) {
        sysml_lazy_16 = sysml_state_mode == 1;
    }
    if (sysml_lazy_16) {
        sysml_conditional_17 = true;
    }
    else {
        sysml_conditional_17 = false;
    }
    sysml_value_4 = sysml_conditional_17;
    *io_4 = sysml_value_0;
    *serial_0_trigger = sysml_value_1;
    *serial_0_value_0 = sysml_value_2;
    *serial_0_value_1 = sysml_value_3;
    *serial_0_value_2 = sysml_value_4;
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
static double hw_value_io_1 = 0;
static double hw_value_io_2 = 0;
static bool hw_value_io_3 = false;
static int32_t hw_value_io_4 = 0;
TM1637Display hw_display_io_4(4, 5);
static int32_t hw_display_previous_io_4 = INT32_MIN;
static int32_t hw_value_io_5 = 0;
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
  if (strcmp(begin, "BOTH") == 0) { *value = 1L; return true; }
  if (strcmp(begin, "HUMI") == 0) { *value = 2L; return true; }
  if (strcmp(begin, "TEMP") == 0) { *value = 3L; return true; }
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
static bool hw_value_io_5_valid = false;
static bool hw_value_serial_0_trigger = false;
static double hw_value_serial_0_value_0 = 0;
static double hw_value_serial_0_value_1 = 0;
static bool hw_value_serial_0_value_2 = false;
#include <DHT.h>
DHT hw_dht_0(7, DHT11);
static bool hw_dht_0_attempted = false;
static uint32_t hw_dht_0_sampled = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
  hw_display_io_4.setBrightness(7, true);
  if (hw_value_io_4 != hw_display_previous_io_4) {
    if (hw_value_io_4 < -999 || hw_value_io_4 > 9999) hw_display_io_4.clear();
    else hw_display_io_4.showNumberDec((int)hw_value_io_4, false, 4, 0);
    hw_display_previous_io_4 = hw_value_io_4;
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
    hw_value_io_0 = (digitalRead(2) == LOW);
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
  hw_value_io_5_valid = hw_rx_poll(hw_now, &hw_value_io_5);
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, hw_value_io_3, hw_value_io_5, hw_value_io_5_valid, &hw_value_io_4, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0, &hw_value_serial_0_value_1, &hw_value_serial_0_value_2);
  if (hw_value_io_4 != hw_display_previous_io_4) {
    if (hw_value_io_4 < -999 || hw_value_io_4 > 9999) hw_display_io_4.clear();
    else hw_display_io_4.showNumberDec((int)hw_value_io_4, false, 4, 0);
    hw_display_previous_io_4 = hw_value_io_4;
  }
  if (hw_value_serial_0_trigger) { Serial.print("TEMP"); Serial.println(hw_value_serial_0_value_0); }
  if (hw_value_serial_0_trigger) { Serial.print("HUMI"); Serial.println(hw_value_serial_0_value_1); }
  if (hw_value_serial_0_trigger) { Serial.print("BOTH"); Serial.println(hw_value_serial_0_value_2); }
}

