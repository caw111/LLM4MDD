#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\repaired_workflow_continuation_20261016\\advanced_022\\workflow\\attempt_0\\sketch\\sketch.ino"
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
extern bool sysml_state_occupied;
/* Global Variable define of  */
extern bool sysml_state_overtimeActive;
/* Global Variable define of  */
extern int32_t sysml_state_startMin;
/* Global Variable define of  */
extern int32_t sysml_state_endMin;
/* Global Variable define of  */
extern bool sysml_state_prevPressed;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_4, bool io_5, int32_t io_8, bool io_8_valid, bool* io_1, int32_t* io_2, int32_t* io_3, bool* io_6, bool* io_7, bool* serial_0_trigger, int32_t* serial_0_value_0, int32_t* serial_0_value_1);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_occupied = false;
/* Global Variable define of  */
bool sysml_state_overtimeActive = false;
/* Global Variable define of  */
int32_t sysml_state_startMin = 0;
/* Global Variable define of  */
int32_t sysml_state_endMin = 0;
/* Global Variable define of  */
bool sysml_state_prevPressed = false;
/* Init function of CompositeActor named "ProgramPage1" */
#line 250 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\repaired_workflow_continuation_20261016\\advanced_022\\workflow\\attempt_0\\sketch\\sketch.ino"
static bool hw_rx_parse(int32_t* value);
#line 291 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\repaired_workflow_continuation_20261016\\advanced_022\\workflow\\attempt_0\\sketch\\sketch.ino"
static bool hw_rx_finish(int32_t* value);
#line 296 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\repaired_workflow_continuation_20261016\\advanced_022\\workflow\\attempt_0\\sketch\\sketch.ino"
static bool hw_rx_poll(uint32_t hw_now, int32_t* value);
#line 318 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\repaired_workflow_continuation_20261016\\advanced_022\\workflow\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 345 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\repaired_workflow_continuation_20261016\\advanced_022\\workflow\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 45 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\repaired_workflow_continuation_20261016\\advanced_022\\workflow\\attempt_0\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_occupied = false;
    sysml_state_overtimeActive = false;
    sysml_state_startMin = 0;
    sysml_state_endMin = 0;
    sysml_state_prevPressed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_4, bool io_5, int32_t io_8, bool io_8_valid, bool* io_1, int32_t* io_2, int32_t* io_3, bool* io_6, bool* io_7, bool* serial_0_trigger, int32_t* serial_0_value_0, int32_t* serial_0_value_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_conditional_10 = 0;
    int32_t sysml_conditional_11 = 0;
    int32_t sysml_conditional_12 = 0;
    bool sysml_lazy_13 = 0;
    bool sysml_conditional_14 = 0;
    bool sysml_conditional_15 = 0;
    bool sysml_lazy_16 = 0;
    bool sysml_conditional_17 = 0;
    sysml_lazy_0 = io_8_valid;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_8 == 0;
    }
    if (sysml_lazy_0) {
        sysml_state_startMin = 0;
        sysml_state_endMin = 0;
        sysml_state_occupied = false;
        sysml_state_overtimeActive = false;
    }
    sysml_lazy_1 = io_8_valid;
    if (sysml_lazy_1) {
        sysml_lazy_1 = io_8 == 1;
    }
    if (sysml_lazy_1) {
        sysml_state_startMin = 10;
        sysml_state_endMin = 20;
    }
    sysml_lazy_2 = io_8_valid;
    if (sysml_lazy_2) {
        sysml_lazy_2 = io_8 == 2;
    }
    if (sysml_lazy_2) {
        sysml_state_startMin = 30;
        sysml_state_endMin = 60;
    }
    sysml_lazy_3 = sysml_state_startMin == 0;
    if (sysml_lazy_3) {
        sysml_lazy_3 = sysml_state_endMin == 0;
    }
    sysml_lazy_4 = sysml_lazy_3;
    if (sysml_lazy_4) {
        sysml_lazy_4 = false;
    }
    if (sysml_lazy_4) {
    }
    if (io_4) {
        sysml_state_occupied = true;
    }
    sysml_lazy_5 = !io_5;
    if (sysml_lazy_5) {
        sysml_lazy_5 = sysml_state_occupied;
    }
    if (sysml_lazy_5) {
        sysml_state_occupied = false;
    }
    sysml_lazy_6 = io_0;
    if (sysml_lazy_6) {
        sysml_lazy_6 = !sysml_state_prevPressed;
    }
    if (sysml_lazy_6) {
        sysml_state_occupied = false;
        sysml_state_overtimeActive = false;
    }
    sysml_lazy_7 = sysml_state_occupied;
    if (sysml_lazy_7) {
        sysml_lazy_7 = sysml_state_startMin > 0;
    }
    sysml_lazy_8 = sysml_lazy_7;
    if (sysml_lazy_8) {
        sysml_lazy_8 = sysml_state_endMin > sysml_state_startMin;
    }
    sysml_lazy_9 = sysml_lazy_8;
    if (sysml_lazy_9) {
        sysml_lazy_9 = sysml_state_startMin >= sysml_state_endMin;
    }
    if (sysml_lazy_9) {
        sysml_state_overtimeActive = true;
    }
    else {
        sysml_state_overtimeActive = false;
    }
    sysml_state_prevPressed = io_0;
    bool sysml_value_0 = 0;
    if (sysml_state_occupied) {
        sysml_conditional_10 = true;
    }
    else {
        sysml_conditional_10 = false;
    }
    sysml_value_0 = sysml_conditional_10;
    int32_t sysml_value_1 = 0;
    if (sysml_state_overtimeActive) {
        sysml_conditional_11 = 1000;
    }
    else {
        sysml_conditional_11 = 0;
    }
    sysml_value_1 = sysml_conditional_11;
    int32_t sysml_value_2 = 0;
    if (sysml_state_occupied) {
        sysml_conditional_12 = 1;
    }
    else {
        sysml_conditional_12 = 0;
    }
    sysml_value_2 = sysml_conditional_12;
    bool sysml_value_3 = 0;
    sysml_lazy_13 = !sysml_state_occupied;
    if (sysml_lazy_13) {
        sysml_lazy_13 = !sysml_state_overtimeActive;
    }
    if (sysml_lazy_13) {
        sysml_conditional_14 = true;
    }
    else {
        sysml_conditional_14 = false;
    }
    sysml_value_3 = sysml_conditional_14;
    bool sysml_value_4 = 0;
    if (sysml_state_occupied) {
        sysml_conditional_15 = true;
    }
    else {
        sysml_conditional_15 = false;
    }
    sysml_value_4 = sysml_conditional_15;
    bool sysml_value_5 = 0;
    sysml_lazy_16 = io_8_valid;
    if (sysml_lazy_16) {
        sysml_lazy_16 = sysml_state_endMin > 0;
    }
    if (sysml_lazy_16) {
        sysml_conditional_17 = true;
    }
    else {
        sysml_conditional_17 = false;
    }
    sysml_value_5 = sysml_conditional_17;
    int32_t sysml_value_6 = 0;
    sysml_value_6 = sysml_state_startMin;
    int32_t sysml_value_7 = 0;
    sysml_value_7 = sysml_state_endMin;
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
    *io_3 = sysml_value_2;
    *io_6 = sysml_value_3;
    *io_7 = sysml_value_4;
    *serial_0_trigger = sysml_value_5;
    *serial_0_value_0 = sysml_value_6;
    *serial_0_value_1 = sysml_value_7;
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
static bool hw_value_io_1 = false;
static int32_t hw_value_io_2 = 0;
static int32_t hw_tone_io_2 = -1;
static int32_t hw_value_io_3 = 0;
TM1637Display hw_display_io_3(4, 5);
static int32_t hw_display_previous_io_3 = INT32_MIN;
static bool hw_value_io_4 = false;
static uint32_t hw_sampled_io_4 = 0;
static bool hw_attempted_io_4 = false;
static bool hw_value_io_5 = false;
static uint32_t hw_sampled_io_5 = 0;
static bool hw_attempted_io_5 = false;
static bool hw_value_io_6 = false;
static bool hw_value_io_7 = false;
static int32_t hw_value_io_8 = 0;
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
  if (strcmp(begin, "T1") == 0) { *value = 1L; return true; }
  if (strcmp(begin, "T2") == 0) { *value = 2L; return true; }
  if (strcmp(begin, "T3") == 0) { *value = 3L; return true; }
  if (strcmp(begin, "T4") == 0) { *value = 4L; return true; }
  if (strcmp(begin, "T5") == 0) { *value = 5L; return true; }
  if (strcmp(begin, "T6") == 0) { *value = 6L; return true; }
  if (strcmp(begin, "T7") == 0) { *value = 7L; return true; }
  if (strcmp(begin, "T8") == 0) { *value = 8L; return true; }
  if (strcmp(begin, "T9") == 0) { *value = 9L; return true; }
  if (strcmp(begin, "T10") == 0) { *value = 10L; return true; }
  if (strcmp(begin, "T11") == 0) { *value = 11L; return true; }
  if (strcmp(begin, "T12") == 0) { *value = 12L; return true; }
  if (strcmp(begin, "T13") == 0) { *value = 13L; return true; }
  if (strcmp(begin, "T14") == 0) { *value = 14L; return true; }
  if (strcmp(begin, "T15") == 0) { *value = 15L; return true; }
  if (strcmp(begin, "T16") == 0) { *value = 16L; return true; }
  if (strcmp(begin, "T17") == 0) { *value = 17L; return true; }
  if (strcmp(begin, "T18") == 0) { *value = 18L; return true; }
  if (strcmp(begin, "T19") == 0) { *value = 19L; return true; }
  if (strcmp(begin, "T20") == 0) { *value = 20L; return true; }
  if (strcmp(begin, "T21") == 0) { *value = 21L; return true; }
  if (strcmp(begin, "T22") == 0) { *value = 22L; return true; }
  if (strcmp(begin, "T23") == 0) { *value = 23L; return true; }
  if (strcmp(begin, "T24") == 0) { *value = 24L; return true; }
  if (strcmp(begin, "T25") == 0) { *value = 25L; return true; }
  if (strcmp(begin, "T26") == 0) { *value = 26L; return true; }
  if (strcmp(begin, "T27") == 0) { *value = 27L; return true; }
  if (strcmp(begin, "T28") == 0) { *value = 28L; return true; }
  if (strcmp(begin, "T29") == 0) { *value = 29L; return true; }
  if (strcmp(begin, "T30") == 0) { *value = 30L; return true; }
  if (strcmp(begin, "T31") == 0) { *value = 31L; return true; }
  if (strcmp(begin, "T32") == 0) { *value = 32L; return true; }
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
static bool hw_value_io_8_valid = false;
static bool hw_value_serial_0_trigger = false;
static int32_t hw_value_serial_0_value_0 = 0;
static int32_t hw_value_serial_0_value_1 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, OUTPUT);
  digitalWrite(8, hw_value_io_1 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  int32_t hw_tone_command_io_2 = (hw_value_io_2 >= 31 && hw_value_io_2 <= 65535) ? hw_value_io_2 : 0;
  if (hw_tone_command_io_2 != hw_tone_io_2) {
    if (hw_tone_command_io_2 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_2);
    hw_tone_io_2 = hw_tone_command_io_2;
  }
  hw_display_io_3.setBrightness(3, true);
  if (hw_value_io_3 != hw_display_previous_io_3) {
    if (hw_value_io_3 < -999 || hw_value_io_3 > 9999) hw_display_io_3.clear();
    else hw_display_io_3.showNumberDec((int)hw_value_io_3, false, 4, 0);
    hw_display_previous_io_3 = hw_value_io_3;
  }
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(12, OUTPUT);
  digitalWrite(12, hw_value_io_6 ? HIGH : LOW);
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_7 ? HIGH : LOW);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 10UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (digitalRead(7) == HIGH);
  }
  if (!hw_attempted_io_4 || (uint32_t)(hw_now - hw_sampled_io_4) >= 10UL) {
    hw_attempted_io_4 = true; hw_sampled_io_4 = hw_now;
    hw_value_io_4 = (digitalRead(2) == HIGH);
  }
  if (!hw_attempted_io_5 || (uint32_t)(hw_now - hw_sampled_io_5) >= 10UL) {
    hw_attempted_io_5 = true; hw_sampled_io_5 = hw_now;
    hw_value_io_5 = (digitalRead(3) == HIGH);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_8_valid = hw_rx_poll(hw_now, &hw_value_io_8);
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_4, hw_value_io_5, hw_value_io_8, hw_value_io_8_valid, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_io_6, &hw_value_io_7, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0, &hw_value_serial_0_value_1);
  digitalWrite(8, hw_value_io_1 ? HIGH : LOW);
  int32_t hw_tone_command_io_2 = (hw_value_io_2 >= 31 && hw_value_io_2 <= 65535) ? hw_value_io_2 : 0;
  if (hw_tone_command_io_2 != hw_tone_io_2) {
    if (hw_tone_command_io_2 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_2);
    hw_tone_io_2 = hw_tone_command_io_2;
  }
  if (hw_value_io_3 != hw_display_previous_io_3) {
    if (hw_value_io_3 < -999 || hw_value_io_3 > 9999) hw_display_io_3.clear();
    else hw_display_io_3.showNumberDec((int)hw_value_io_3, false, 4, 0);
    hw_display_previous_io_3 = hw_value_io_3;
  }
  digitalWrite(12, hw_value_io_6 ? HIGH : LOW);
  digitalWrite(13, hw_value_io_7 ? HIGH : LOW);
  if (hw_value_serial_0_trigger) { Serial.print("start_min"); Serial.println(hw_value_serial_0_value_0); }
  if (hw_value_serial_0_trigger) { Serial.print("end_min"); Serial.println(hw_value_serial_0_value_1); }
}

