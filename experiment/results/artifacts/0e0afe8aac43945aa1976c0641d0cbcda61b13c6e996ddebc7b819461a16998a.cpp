#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_gpt61_failures_20261020\\pceval_serial_bar_led\\workflow\\attempt_0\\sketch\\sketch.ino"
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
extern int32_t sysml_state_savedThreshold;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_0, bool io_0_valid, bool* io_1_segment_0, bool* io_1_segment_1, bool* io_1_segment_2, bool* io_1_segment_3, bool* io_1_segment_4, bool* io_1_segment_5, bool* io_1_segment_6, bool* io_1_segment_7, bool* io_1_segment_8, bool* io_1_segment_9);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_savedThreshold = 0;
/* Init function of CompositeActor named "ProgramPage1" */
#line 86 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_gpt61_failures_20261020\\pceval_serial_bar_led\\workflow\\attempt_0\\sketch\\sketch.ino"
static bool hw_rx_parse(int32_t* value);
#line 110 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_gpt61_failures_20261020\\pceval_serial_bar_led\\workflow\\attempt_0\\sketch\\sketch.ino"
static bool hw_rx_finish(int32_t* value);
#line 115 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_gpt61_failures_20261020\\pceval_serial_bar_led\\workflow\\attempt_0\\sketch\\sketch.ino"
static bool hw_rx_poll(uint32_t hw_now, int32_t* value);
#line 144 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_gpt61_failures_20261020\\pceval_serial_bar_led\\workflow\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 169 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_gpt61_failures_20261020\\pceval_serial_bar_led\\workflow\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 29 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_gpt61_failures_20261020\\pceval_serial_bar_led\\workflow\\attempt_0\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_savedThreshold = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_0, bool io_0_valid, bool* io_1_segment_0, bool* io_1_segment_1, bool* io_1_segment_2, bool* io_1_segment_3, bool* io_1_segment_4, bool* io_1_segment_5, bool* io_1_segment_6, bool* io_1_segment_7, bool* io_1_segment_8, bool* io_1_segment_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    if (io_0_valid) {
        sysml_state_savedThreshold = io_0;
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = (1 < sysml_state_savedThreshold);
    bool sysml_value_1 = 0;
    sysml_value_1 = (2 < sysml_state_savedThreshold);
    bool sysml_value_2 = 0;
    sysml_value_2 = (3 < sysml_state_savedThreshold);
    bool sysml_value_3 = 0;
    sysml_value_3 = (4 < sysml_state_savedThreshold);
    bool sysml_value_4 = 0;
    sysml_value_4 = (5 < sysml_state_savedThreshold);
    bool sysml_value_5 = 0;
    sysml_value_5 = (6 < sysml_state_savedThreshold);
    bool sysml_value_6 = 0;
    sysml_value_6 = (7 < sysml_state_savedThreshold);
    bool sysml_value_7 = 0;
    sysml_value_7 = (8 < sysml_state_savedThreshold);
    bool sysml_value_8 = 0;
    sysml_value_8 = (9 < sysml_state_savedThreshold);
    bool sysml_value_9 = 0;
    sysml_value_9 = (10 < sysml_state_savedThreshold);
    *io_1_segment_0 = sysml_value_0;
    *io_1_segment_1 = sysml_value_1;
    *io_1_segment_2 = sysml_value_2;
    *io_1_segment_3 = sysml_value_3;
    *io_1_segment_4 = sysml_value_4;
    *io_1_segment_5 = sysml_value_5;
    *io_1_segment_6 = sysml_value_6;
    *io_1_segment_7 = sysml_value_7;
    *io_1_segment_8 = sysml_value_8;
    *io_1_segment_9 = sysml_value_9;
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
static int32_t hw_value_io_0 = 0;
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
  if (parsed < INT32_MIN || parsed > 2147483647L) return false;
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
static bool hw_value_io_0_valid = false;
static bool hw_value_io_1_segment_0 = false;
static bool hw_value_io_1_segment_1 = false;
static bool hw_value_io_1_segment_2 = false;
static bool hw_value_io_1_segment_3 = false;
static bool hw_value_io_1_segment_4 = false;
static bool hw_value_io_1_segment_5 = false;
static bool hw_value_io_1_segment_6 = false;
static bool hw_value_io_1_segment_7 = false;
static bool hw_value_io_1_segment_8 = false;
static bool hw_value_io_1_segment_9 = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  digitalWrite(2, hw_value_io_1_segment_0 ? HIGH : LOW);
  pinMode(3, OUTPUT);
  digitalWrite(3, hw_value_io_1_segment_1 ? HIGH : LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, hw_value_io_1_segment_2 ? HIGH : LOW);
  pinMode(5, OUTPUT);
  digitalWrite(5, hw_value_io_1_segment_3 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  digitalWrite(6, hw_value_io_1_segment_4 ? HIGH : LOW);
  pinMode(7, OUTPUT);
  digitalWrite(7, hw_value_io_1_segment_5 ? HIGH : LOW);
  pinMode(8, OUTPUT);
  digitalWrite(8, hw_value_io_1_segment_6 ? HIGH : LOW);
  pinMode(9, OUTPUT);
  digitalWrite(9, hw_value_io_1_segment_7 ? HIGH : LOW);
  pinMode(10, OUTPUT);
  digitalWrite(10, hw_value_io_1_segment_8 ? HIGH : LOW);
  pinMode(11, OUTPUT);
  digitalWrite(11, hw_value_io_1_segment_9 ? HIGH : LOW);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_0_valid = hw_rx_poll(hw_now, &hw_value_io_0);
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_0_valid, &hw_value_io_1_segment_0, &hw_value_io_1_segment_1, &hw_value_io_1_segment_2, &hw_value_io_1_segment_3, &hw_value_io_1_segment_4, &hw_value_io_1_segment_5, &hw_value_io_1_segment_6, &hw_value_io_1_segment_7, &hw_value_io_1_segment_8, &hw_value_io_1_segment_9);
  digitalWrite(2, hw_value_io_1_segment_0 ? HIGH : LOW);
  digitalWrite(3, hw_value_io_1_segment_1 ? HIGH : LOW);
  digitalWrite(4, hw_value_io_1_segment_2 ? HIGH : LOW);
  digitalWrite(5, hw_value_io_1_segment_3 ? HIGH : LOW);
  digitalWrite(6, hw_value_io_1_segment_4 ? HIGH : LOW);
  digitalWrite(7, hw_value_io_1_segment_5 ? HIGH : LOW);
  digitalWrite(8, hw_value_io_1_segment_6 ? HIGH : LOW);
  digitalWrite(9, hw_value_io_1_segment_7 ? HIGH : LOW);
  digitalWrite(10, hw_value_io_1_segment_8 ? HIGH : LOW);
  digitalWrite(11, hw_value_io_1_segment_9 ? HIGH : LOW);
}

