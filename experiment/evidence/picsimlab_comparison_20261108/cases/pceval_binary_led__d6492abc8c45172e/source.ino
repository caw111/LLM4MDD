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
extern int32_t sysml_state_displayValue;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_5, bool io_5_valid, bool* io_0, bool* io_1, bool* io_2, bool* io_3, bool* io_4, bool* serial_0_trigger, int32_t* serial_0_value_0);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_displayValue = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_displayValue = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_5, bool io_5_valid, bool* io_0, bool* io_1, bool* io_2, bool* io_3, bool* io_4, bool* serial_0_trigger, int32_t* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int32_t sysml_conditional_0 = 0;
    int32_t sysml_arithmetic_1 = 0;
    int32_t sysml_conditional_2 = 0;
    int32_t sysml_conditional_3 = 0;
    int32_t sysml_conditional_4 = 0;
    int32_t sysml_conditional_5 = 0;
    int32_t sysml_conditional_6 = 0;
    int32_t sysml_arithmetic_7 = 0;
    int32_t sysml_conditional_8 = 0;
    int32_t sysml_conditional_9 = 0;
    int32_t sysml_conditional_10 = 0;
    int32_t sysml_conditional_11 = 0;
    int32_t sysml_conditional_12 = 0;
    int32_t sysml_arithmetic_13 = 0;
    int32_t sysml_conditional_14 = 0;
    int32_t sysml_conditional_15 = 0;
    int32_t sysml_conditional_16 = 0;
    int32_t sysml_conditional_17 = 0;
    int32_t sysml_conditional_18 = 0;
    int32_t sysml_arithmetic_19 = 0;
    int32_t sysml_conditional_20 = 0;
    int32_t sysml_conditional_21 = 0;
    int32_t sysml_conditional_22 = 0;
    int32_t sysml_conditional_23 = 0;
    int32_t sysml_conditional_24 = 0;
    int32_t sysml_arithmetic_25 = 0;
    if (io_5_valid) {
        sysml_state_displayValue = io_5;
    }
    bool sysml_value_0 = 0;
    if ((((double)sysml_state_displayValue) / 2) < ((double)((int32_t)(((double)sysml_state_displayValue) / 2)))) {
        sysml_conditional_0 = 1;
    }
    else {
        sysml_conditional_0 = 0;
    }
    sysml_arithmetic_1 = ((int32_t)(((double)sysml_state_displayValue) / 2)) - sysml_conditional_0;
    sysml_value_0 = (sysml_state_displayValue - (1 * sysml_arithmetic_1)) == 1;
    bool sysml_value_1 = 0;
    if ((((double)sysml_state_displayValue) / 2) < ((double)((int32_t)(((double)sysml_state_displayValue) / 2)))) {
        sysml_conditional_2 = 1;
    }
    else {
        sysml_conditional_2 = 0;
    }
    if ((((double)sysml_state_displayValue) / 2) < ((double)((int32_t)(((double)sysml_state_displayValue) / 2)))) {
        sysml_conditional_3 = 1;
    }
    else {
        sysml_conditional_3 = 0;
    }
    if ((((double)sysml_state_displayValue) / 2) < ((double)((int32_t)(((double)sysml_state_displayValue) / 2)))) {
        sysml_conditional_4 = 1;
    }
    else {
        sysml_conditional_4 = 0;
    }
    if ((((double)sysml_state_displayValue) / 2) < ((double)((int32_t)(((double)sysml_state_displayValue) / 2)))) {
        sysml_conditional_5 = 1;
    }
    else {
        sysml_conditional_5 = 0;
    }
    if ((((double)(((int32_t)(((double)sysml_state_displayValue) / 2)) - sysml_conditional_4)) / 2) < ((double)((int32_t)(((double)(((int32_t)(((double)sysml_state_displayValue) / 2)) - sysml_conditional_5)) / 2)))) {
        sysml_conditional_6 = 1;
    }
    else {
        sysml_conditional_6 = 0;
    }
    sysml_arithmetic_7 = ((int32_t)(((double)(((int32_t)(((double)sysml_state_displayValue) / 2)) - sysml_conditional_3)) / 2)) - sysml_conditional_6;
    sysml_value_1 = ((((int32_t)(((double)sysml_state_displayValue) / 2)) - sysml_conditional_2) - (1 * sysml_arithmetic_7)) == 1;
    bool sysml_value_2 = 0;
    if ((((double)sysml_state_displayValue) / 4) < ((double)((int32_t)(((double)sysml_state_displayValue) / 4)))) {
        sysml_conditional_8 = 1;
    }
    else {
        sysml_conditional_8 = 0;
    }
    if ((((double)sysml_state_displayValue) / 4) < ((double)((int32_t)(((double)sysml_state_displayValue) / 4)))) {
        sysml_conditional_9 = 1;
    }
    else {
        sysml_conditional_9 = 0;
    }
    if ((((double)sysml_state_displayValue) / 4) < ((double)((int32_t)(((double)sysml_state_displayValue) / 4)))) {
        sysml_conditional_10 = 1;
    }
    else {
        sysml_conditional_10 = 0;
    }
    if ((((double)sysml_state_displayValue) / 4) < ((double)((int32_t)(((double)sysml_state_displayValue) / 4)))) {
        sysml_conditional_11 = 1;
    }
    else {
        sysml_conditional_11 = 0;
    }
    if ((((double)(((int32_t)(((double)sysml_state_displayValue) / 4)) - sysml_conditional_10)) / 2) < ((double)((int32_t)(((double)(((int32_t)(((double)sysml_state_displayValue) / 4)) - sysml_conditional_11)) / 2)))) {
        sysml_conditional_12 = 1;
    }
    else {
        sysml_conditional_12 = 0;
    }
    sysml_arithmetic_13 = ((int32_t)(((double)(((int32_t)(((double)sysml_state_displayValue) / 4)) - sysml_conditional_9)) / 2)) - sysml_conditional_12;
    sysml_value_2 = ((((int32_t)(((double)sysml_state_displayValue) / 4)) - sysml_conditional_8) - (1 * sysml_arithmetic_13)) == 1;
    bool sysml_value_3 = 0;
    if ((((double)sysml_state_displayValue) / 8) < ((double)((int32_t)(((double)sysml_state_displayValue) / 8)))) {
        sysml_conditional_14 = 1;
    }
    else {
        sysml_conditional_14 = 0;
    }
    if ((((double)sysml_state_displayValue) / 8) < ((double)((int32_t)(((double)sysml_state_displayValue) / 8)))) {
        sysml_conditional_15 = 1;
    }
    else {
        sysml_conditional_15 = 0;
    }
    if ((((double)sysml_state_displayValue) / 8) < ((double)((int32_t)(((double)sysml_state_displayValue) / 8)))) {
        sysml_conditional_16 = 1;
    }
    else {
        sysml_conditional_16 = 0;
    }
    if ((((double)sysml_state_displayValue) / 8) < ((double)((int32_t)(((double)sysml_state_displayValue) / 8)))) {
        sysml_conditional_17 = 1;
    }
    else {
        sysml_conditional_17 = 0;
    }
    if ((((double)(((int32_t)(((double)sysml_state_displayValue) / 8)) - sysml_conditional_16)) / 2) < ((double)((int32_t)(((double)(((int32_t)(((double)sysml_state_displayValue) / 8)) - sysml_conditional_17)) / 2)))) {
        sysml_conditional_18 = 1;
    }
    else {
        sysml_conditional_18 = 0;
    }
    sysml_arithmetic_19 = ((int32_t)(((double)(((int32_t)(((double)sysml_state_displayValue) / 8)) - sysml_conditional_15)) / 2)) - sysml_conditional_18;
    sysml_value_3 = ((((int32_t)(((double)sysml_state_displayValue) / 8)) - sysml_conditional_14) - (1 * sysml_arithmetic_19)) == 1;
    bool sysml_value_4 = 0;
    if ((((double)sysml_state_displayValue) / 16) < ((double)((int32_t)(((double)sysml_state_displayValue) / 16)))) {
        sysml_conditional_20 = 1;
    }
    else {
        sysml_conditional_20 = 0;
    }
    if ((((double)sysml_state_displayValue) / 16) < ((double)((int32_t)(((double)sysml_state_displayValue) / 16)))) {
        sysml_conditional_21 = 1;
    }
    else {
        sysml_conditional_21 = 0;
    }
    if ((((double)sysml_state_displayValue) / 16) < ((double)((int32_t)(((double)sysml_state_displayValue) / 16)))) {
        sysml_conditional_22 = 1;
    }
    else {
        sysml_conditional_22 = 0;
    }
    if ((((double)sysml_state_displayValue) / 16) < ((double)((int32_t)(((double)sysml_state_displayValue) / 16)))) {
        sysml_conditional_23 = 1;
    }
    else {
        sysml_conditional_23 = 0;
    }
    if ((((double)(((int32_t)(((double)sysml_state_displayValue) / 16)) - sysml_conditional_22)) / 2) < ((double)((int32_t)(((double)(((int32_t)(((double)sysml_state_displayValue) / 16)) - sysml_conditional_23)) / 2)))) {
        sysml_conditional_24 = 1;
    }
    else {
        sysml_conditional_24 = 0;
    }
    sysml_arithmetic_25 = ((int32_t)(((double)(((int32_t)(((double)sysml_state_displayValue) / 16)) - sysml_conditional_21)) / 2)) - sysml_conditional_24;
    sysml_value_4 = ((((int32_t)(((double)sysml_state_displayValue) / 16)) - sysml_conditional_20) - (1 * sysml_arithmetic_25)) == 1;
    bool sysml_value_5 = 0;
    sysml_value_5 = io_5_valid;
    int32_t sysml_value_6 = 0;
    sysml_value_6 = sysml_state_displayValue;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
    *io_4 = sysml_value_4;
    *serial_0_trigger = sysml_value_5;
    *serial_0_value_0 = sysml_value_6;
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
static bool hw_value_io_1 = false;
static bool hw_value_io_2 = false;
static bool hw_value_io_3 = false;
static bool hw_value_io_4 = false;
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
  char* cursor = begin;
  bool negative = *cursor == '-';
  if (*cursor == '-' || *cursor == '+') ++cursor;
  if (!*cursor) return false;
  uint32_t magnitude = 0;
  const uint32_t limit = negative ? 2147483648UL : 2147483647UL;
  while (*cursor) {
    if (*cursor < '0' || *cursor > '9') return false;
    uint32_t digit = (uint32_t)(*cursor++ - '0');
    if (magnitude > (limit - digit) / 10UL) return false;
    magnitude = magnitude * 10UL + digit;
  }
  int32_t parsed = negative ? (magnitude == 2147483648UL ? INT32_MIN : -(int32_t)magnitude) : (int32_t)magnitude;
  if (parsed < 0L || parsed > 31L) return false;
  *value = parsed;
  return true;
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
static int32_t hw_value_serial_0_value_0 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  digitalWrite(2, hw_value_io_0 ? HIGH : LOW);
  pinMode(3, OUTPUT);
  digitalWrite(3, hw_value_io_1 ? HIGH : LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, hw_value_io_2 ? HIGH : LOW);
  pinMode(5, OUTPUT);
  digitalWrite(5, hw_value_io_3 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  digitalWrite(6, hw_value_io_4 ? HIGH : LOW);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_5_valid = hw_rx_poll(hw_now, &hw_value_io_5);
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_5, hw_value_io_5_valid, &hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0);
  digitalWrite(2, hw_value_io_0 ? HIGH : LOW);
  digitalWrite(3, hw_value_io_1 ? HIGH : LOW);
  digitalWrite(4, hw_value_io_2 ? HIGH : LOW);
  digitalWrite(5, hw_value_io_3 ? HIGH : LOW);
  digitalWrite(6, hw_value_io_4 ? HIGH : LOW);
  if (hw_value_serial_0_trigger) { Serial.print("binary_input"); Serial.println(hw_value_serial_0_value_0); }
}
