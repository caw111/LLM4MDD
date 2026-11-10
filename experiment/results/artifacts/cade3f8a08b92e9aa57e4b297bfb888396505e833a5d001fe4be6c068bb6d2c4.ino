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
extern double sysml_state_thresholdC;
/* Global Variable define of  */
extern double sysml_state_fanMinPwm;
/* Global Variable define of  */
extern bool sysml_state_autoMode;
/* Global Variable define of  */
extern bool sysml_state_prevOverheatCond;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, int32_t io_3, bool io_3_valid, double* io_1, bool* io_2, bool* serial_0_trigger, double* serial_0_value_0);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_thresholdC = 30.0;
/* Global Variable define of  */
double sysml_state_fanMinPwm = 0.0;
/* Global Variable define of  */
bool sysml_state_autoMode = false;
/* Global Variable define of  */
bool sysml_state_prevOverheatCond = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_thresholdC = 30.0;
    sysml_state_fanMinPwm = 0.0;
    sysml_state_autoMode = false;
    sysml_state_prevOverheatCond = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, int32_t io_3, bool io_3_valid, double* io_1, bool* io_2, bool* serial_0_trigger, double* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_conditional_5 = 0;
    double sysml_conditional_6 = 0;
    double sysml_conditional_7 = 0;
    double sysml_conditional_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_conditional_10 = 0;
    bool sysml_lazy_11 = 0;
    bool sysml_lazy_12 = 0;
    bool sysml_conditional_13 = 0;
    sysml_lazy_0 = io_3_valid;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_3 == 10;
    }
    if (sysml_lazy_0) {
        sysml_state_thresholdC = 30.0;
        sysml_state_fanMinPwm = 0.0;
        sysml_state_autoMode = false;
        sysml_state_prevOverheatCond = false;
    }
    sysml_lazy_1 = io_3_valid;
    if (sysml_lazy_1) {
        sysml_lazy_1 = io_3 == 7;
    }
    if (sysml_lazy_1) {
        sysml_state_thresholdC = 0.0;
    }
    sysml_lazy_2 = io_3_valid;
    if (sysml_lazy_2) {
        sysml_lazy_2 = io_3 == 8;
    }
    if (sysml_lazy_2) {
        sysml_state_fanMinPwm = 0.0;
    }
    sysml_lazy_3 = io_3_valid;
    if (sysml_lazy_3) {
        sysml_lazy_3 = io_3 == 11;
    }
    if (sysml_lazy_3) {
        sysml_state_autoMode = true;
    }
    sysml_lazy_4 = io_3_valid;
    if (sysml_lazy_4) {
        sysml_lazy_4 = io_3 == 12;
    }
    if (sysml_lazy_4) {
        sysml_state_autoMode = false;
    }
    if (io_0 >= (sysml_state_thresholdC + 5.0)) {
        sysml_conditional_5 = true;
    }
    else {
        sysml_conditional_5 = false;
    }
    sysml_state_prevOverheatCond = sysml_conditional_5;
    double sysml_value_0 = 0;
    if (sysml_state_autoMode) {
        if (io_0 >= sysml_state_thresholdC) {
            if (io_0 >= (sysml_state_thresholdC + 5.0)) {
                sysml_conditional_8 = sysml_state_fanMinPwm;
            }
            else {
                sysml_conditional_8 = sysml_state_fanMinPwm;
            }
            sysml_conditional_7 = sysml_conditional_8;
        }
        else {
            sysml_conditional_7 = 0.0;
        }
        sysml_conditional_6 = sysml_conditional_7;
    }
    else {
        sysml_conditional_6 = 0.0;
    }
    sysml_value_0 = sysml_conditional_6;
    bool sysml_value_1 = 0;
    sysml_lazy_9 = sysml_state_autoMode;
    if (sysml_lazy_9) {
        sysml_lazy_9 = io_0 >= (sysml_state_thresholdC + 5.0);
    }
    if (sysml_lazy_9) {
        sysml_conditional_10 = true;
    }
    else {
        sysml_conditional_10 = false;
    }
    sysml_value_1 = sysml_conditional_10;
    bool sysml_value_2 = 0;
    sysml_lazy_11 = sysml_state_autoMode;
    if (sysml_lazy_11) {
        sysml_lazy_11 = io_0 >= (sysml_state_thresholdC + 5.0);
    }
    sysml_lazy_12 = sysml_lazy_11;
    if (sysml_lazy_12) {
        sysml_lazy_12 = !sysml_state_prevOverheatCond;
    }
    if (sysml_lazy_12) {
        sysml_conditional_13 = true;
    }
    else {
        sysml_conditional_13 = false;
    }
    sysml_value_2 = sysml_conditional_13;
    double sysml_value_3 = 0;
    sysml_value_3 = io_0;
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
    *serial_0_trigger = sysml_value_2;
    *serial_0_value_0 = sysml_value_3;
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
static uint32_t hw_sampled_io_0 = 0;
static bool hw_attempted_io_0 = false;
static double hw_value_io_1 = 0;
static bool hw_value_io_2 = false;
static int32_t hw_value_io_3 = 0;
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
  if (strcmp(begin, "TEMP_UP") == 0) { *value = 1L; return true; }
  if (strcmp(begin, "TEMP_DOWN") == 0) { *value = 2L; return true; }
  if (strcmp(begin, "THRESH_UP") == 0) { *value = 3L; return true; }
  if (strcmp(begin, "THRESH_DOWN") == 0) { *value = 4L; return true; }
  if (strcmp(begin, "FAN_MIN_UP") == 0) { *value = 5L; return true; }
  if (strcmp(begin, "FAN_MIN_DOWN") == 0) { *value = 6L; return true; }
  if (strcmp(begin, "SET_THRESHOLD") == 0) { *value = 7L; return true; }
  if (strcmp(begin, "SET_FAN_MIN") == 0) { *value = 8L; return true; }
  if (strcmp(begin, "GET_STATUS") == 0) { *value = 9L; return true; }
  if (strcmp(begin, "RESET") == 0) { *value = 10L; return true; }
  if (strcmp(begin, "AUTO_ON") == 0) { *value = 11L; return true; }
  if (strcmp(begin, "AUTO_OFF") == 0) { *value = 12L; return true; }
  if (strcmp(begin, "INC_THRESHOLD") == 0) { *value = 13L; return true; }
  if (strcmp(begin, "DEC_THRESHOLD") == 0) { *value = 14L; return true; }
  if (strcmp(begin, "INC_FAN_MIN") == 0) { *value = 15L; return true; }
  if (strcmp(begin, "DEC_FAN_MIN") == 0) { *value = 16L; return true; }
  if (strcmp(begin, "THRESH_Q1") == 0) { *value = 17L; return true; }
  if (strcmp(begin, "THRESH_Q2") == 0) { *value = 18L; return true; }
  if (strcmp(begin, "THRESH_Q3") == 0) { *value = 19L; return true; }
  if (strcmp(begin, "THRESH_Q4") == 0) { *value = 20L; return true; }
  if (strcmp(begin, "FAN_MIN_Q1") == 0) { *value = 21L; return true; }
  if (strcmp(begin, "FAN_MIN_Q2") == 0) { *value = 22L; return true; }
  if (strcmp(begin, "FAN_MIN_Q3") == 0) { *value = 23L; return true; }
  if (strcmp(begin, "FAN_MIN_Q4") == 0) { *value = 24L; return true; }
  if (strcmp(begin, "OVERHEAT_ACK") == 0) { *value = 25L; return true; }
  if (strcmp(begin, "MODE_0") == 0) { *value = 26L; return true; }
  if (strcmp(begin, "MODE_1") == 0) { *value = 27L; return true; }
  if (strcmp(begin, "MODE_2") == 0) { *value = 28L; return true; }
  if (strcmp(begin, "MODE_3") == 0) { *value = 29L; return true; }
  if (strcmp(begin, "MODE_4") == 0) { *value = 30L; return true; }
  if (strcmp(begin, "MODE_5") == 0) { *value = 31L; return true; }
  if (strcmp(begin, "MODE_6") == 0) { *value = 32L; return true; }
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
static bool hw_value_io_3_valid = false;
static bool hw_value_serial_0_trigger = false;
static double hw_value_serial_0_value_0 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(A1, INPUT);
  pinMode(5, OUTPUT);
  analogWrite(5, (int)constrain((hw_value_io_1 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_2 ? HIGH : LOW);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 40UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (analogRead(A1) * 1.0 + 0.0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_3_valid = hw_rx_poll(hw_now, &hw_value_io_3);
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_3, hw_value_io_3_valid, &hw_value_io_1, &hw_value_io_2, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0);
  analogWrite(5, (int)constrain((hw_value_io_1 * 1.0 + 0.0), 0.0, 255.0));
  digitalWrite(13, hw_value_io_2 ? HIGH : LOW);
  if (hw_value_serial_0_trigger) { Serial.print("overheat_temperature"); Serial.println(hw_value_serial_0_value_0); }
}
