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
extern int32_t sysml_state_buzzRemainingMs;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_1, bool io_1_valid, int32_t* io_0, bool* serial_0_trigger, int32_t* serial_0_value_0);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_buzzRemainingMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_buzzRemainingMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_1, bool io_1_valid, int32_t* io_0, bool* serial_0_trigger, int32_t* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    int32_t sysml_conditional_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_conditional_4 = 0;
    int32_t sysml_conditional_5 = 0;
    sysml_lazy_0 = sysml_state_buzzRemainingMs > 0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_1_valid;
    }
    if (sysml_lazy_0) {
        sysml_state_buzzRemainingMs = 0;
    }
    else {
        if ((sysml_state_buzzRemainingMs > 0)) {
            sysml_state_buzzRemainingMs = 0;
        }
        sysml_lazy_1 = io_1_valid;
        if (sysml_lazy_1) {
            sysml_lazy_1 = io_1 == 1;
        }
        if (sysml_lazy_1) {
            sysml_state_buzzRemainingMs = 500;
        }
    }
    int32_t sysml_value_0 = 0;
    if (sysml_state_buzzRemainingMs > 0) {
        sysml_conditional_2 = 1000;
    }
    else {
        sysml_conditional_2 = 0;
    }
    sysml_value_0 = sysml_conditional_2;
    bool sysml_value_1 = 0;
    sysml_lazy_3 = io_1_valid;
    if (sysml_lazy_3) {
        sysml_lazy_3 = io_1 == 1;
    }
    if (sysml_lazy_3) {
        sysml_conditional_4 = true;
    }
    else {
        sysml_conditional_4 = false;
    }
    sysml_value_1 = sysml_conditional_4;
    int32_t sysml_value_2 = 0;
    if (sysml_state_buzzRemainingMs > 0) {
        sysml_conditional_5 = 1000;
    }
    else {
        sysml_conditional_5 = 0;
    }
    sysml_value_2 = sysml_conditional_5;
    *io_0 = sysml_value_0;
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
static int32_t hw_value_io_0 = 0;
static int32_t hw_tone_io_0 = -1;
static int32_t hw_value_io_1 = 0;
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
  if (strcmp(begin, "BUZZ") == 0) { *value = 1L; return true; }
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
static bool hw_value_io_1_valid = false;
static bool hw_value_serial_0_trigger = false;
static int32_t hw_value_serial_0_value_0 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(6, OUTPUT);
  int32_t hw_tone_command_io_0 = (hw_value_io_0 >= 31 && hw_value_io_0 <= 65535) ? hw_value_io_0 : 0;
  if (hw_tone_command_io_0 != hw_tone_io_0) {
    if (hw_tone_command_io_0 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_0);
    hw_tone_io_0 = hw_tone_command_io_0;
  }
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_1_valid = hw_rx_poll(hw_now, &hw_value_io_1);
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_1, hw_value_io_1_valid, &hw_value_io_0, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0);
  int32_t hw_tone_command_io_0 = (hw_value_io_0 >= 31 && hw_value_io_0 <= 65535) ? hw_value_io_0 : 0;
  if (hw_tone_command_io_0 != hw_tone_io_0) {
    if (hw_tone_command_io_0 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_0);
    hw_tone_io_0 = hw_tone_command_io_0;
  }
  if (hw_value_serial_0_trigger) { Serial.print("BUZZ_FREQUENCY_HZ"); Serial.println(hw_value_serial_0_value_0); }
}
