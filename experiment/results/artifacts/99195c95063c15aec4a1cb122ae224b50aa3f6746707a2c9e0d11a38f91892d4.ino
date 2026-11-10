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
extern bool sysml_state_armed;
/* Global Variable define of  */
extern bool sysml_state_nightMode;
/* Global Variable define of  */
extern int32_t sysml_state_eventCount;
/* Global Variable define of  */
extern bool sysml_state_previousPressed;
/* Global Variable define of  */
extern int32_t sysml_state_alarmRemainingMs;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double io_2, int32_t io_6, bool io_6_valid, int32_t* io_3, bool* io_4, int32_t* io_5);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_armed = false;
/* Global Variable define of  */
bool sysml_state_nightMode = false;
/* Global Variable define of  */
int32_t sysml_state_eventCount = 0;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
int32_t sysml_state_alarmRemainingMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_armed = false;
    sysml_state_nightMode = false;
    sysml_state_eventCount = 0;
    sysml_state_previousPressed = false;
    sysml_state_alarmRemainingMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double io_2, int32_t io_6, bool io_6_valid, int32_t* io_3, bool* io_4, int32_t* io_5) {
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
    bool sysml_lazy_10 = 0;
    bool sysml_lazy_11 = 0;
    bool sysml_lazy_12 = 0;
    bool sysml_lazy_13 = 0;
    bool sysml_lazy_14 = 0;
    bool sysml_lazy_15 = 0;
    int32_t sysml_conditional_16 = 0;
    bool sysml_lazy_17 = 0;
    bool sysml_conditional_18 = 0;
    sysml_lazy_0 = io_2 < 500.0;
    if (!sysml_lazy_0) {
        sysml_lazy_1 = io_2 >= 500.0;
        if (sysml_lazy_1) {
            sysml_lazy_1 = false;
        }
        sysml_lazy_0 = sysml_lazy_1;
    }
    if (sysml_lazy_0) {
        sysml_state_nightMode = true;
    }
    sysml_lazy_2 = io_2 < 500.0;
    if (!sysml_lazy_2) {
        sysml_lazy_3 = io_2 >= 500.0;
        if (sysml_lazy_3) {
            sysml_lazy_3 = false;
        }
        sysml_lazy_2 = sysml_lazy_3;
    }
    if (!sysml_lazy_2) {
        sysml_state_nightMode = false;
    }
    sysml_lazy_4 = io_6_valid;
    if (sysml_lazy_4) {
        sysml_lazy_4 = io_6 == 1;
    }
    if (sysml_lazy_4) {
        sysml_state_armed = true;
    }
    sysml_lazy_5 = io_6_valid;
    if (sysml_lazy_5) {
        sysml_lazy_5 = io_6 == 2;
    }
    if (sysml_lazy_5) {
        sysml_state_armed = false;
    }
    sysml_lazy_6 = io_6_valid;
    if (sysml_lazy_6) {
        sysml_lazy_6 = io_6 == 3;
    }
    if (sysml_lazy_6) {
        sysml_state_armed = false;
        sysml_state_eventCount = 0;
        sysml_state_alarmRemainingMs = 0;
    }
    sysml_lazy_7 = io_6_valid;
    if (sysml_lazy_7) {
        sysml_lazy_7 = io_6 == 4;
    }
    if (sysml_lazy_7) {
    }
    sysml_lazy_8 = sysml_state_armed;
    if (sysml_lazy_8) {
        sysml_lazy_8 = sysml_state_nightMode;
    }
    sysml_lazy_9 = sysml_lazy_8;
    if (sysml_lazy_9) {
        sysml_lazy_9 = io_1;
    }
    if (sysml_lazy_9) {
    }
    sysml_lazy_10 = sysml_state_armed;
    if (sysml_lazy_10) {
        sysml_lazy_10 = !sysml_state_nightMode;
    }
    sysml_lazy_11 = sysml_lazy_10;
    if (sysml_lazy_11) {
        sysml_lazy_11 = io_1;
    }
    if (sysml_lazy_11) {
    }
    sysml_lazy_12 = sysml_state_armed;
    if (sysml_lazy_12) {
        sysml_lazy_13 = io_1;
        if (sysml_lazy_13) {
            sysml_lazy_14 = io_0;
            if (sysml_lazy_14) {
                sysml_lazy_14 = !sysml_state_previousPressed;
            }
            sysml_lazy_13 = sysml_lazy_14;
        }
        sysml_lazy_12 = sysml_lazy_13;
    }
    if (sysml_lazy_12) {
        sysml_state_eventCount = (sysml_state_eventCount + 1);
        sysml_state_alarmRemainingMs = 5000;
    }
    if ((sysml_state_alarmRemainingMs > 0)) {
        sysml_state_alarmRemainingMs = (sysml_state_alarmRemainingMs - 5);
    }
    sysml_state_previousPressed = io_0;
    int32_t sysml_value_0 = 0;
    sysml_lazy_15 = sysml_state_armed;
    if (sysml_lazy_15) {
        sysml_lazy_15 = sysml_state_alarmRemainingMs > 0;
    }
    if (sysml_lazy_15) {
        sysml_conditional_16 = 2000;
    }
    else {
        sysml_conditional_16 = 0;
    }
    sysml_value_0 = sysml_conditional_16;
    bool sysml_value_1 = 0;
    sysml_lazy_17 = sysml_state_armed;
    if (sysml_lazy_17) {
        sysml_lazy_17 = sysml_state_alarmRemainingMs > 0;
    }
    if (sysml_lazy_17) {
        sysml_conditional_18 = true;
    }
    else {
        sysml_conditional_18 = false;
    }
    sysml_value_1 = sysml_conditional_18;
    int32_t sysml_value_2 = 0;
    sysml_value_2 = sysml_state_eventCount;
    *io_3 = sysml_value_0;
    *io_4 = sysml_value_1;
    *io_5 = sysml_value_2;
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
static uint32_t hw_sampled_io_1 = 0;
static bool hw_attempted_io_1 = false;
static double hw_value_io_2 = 0;
static uint32_t hw_sampled_io_2 = 0;
static bool hw_attempted_io_2 = false;
static int32_t hw_value_io_3 = 0;
static int32_t hw_tone_io_3 = -1;
static bool hw_value_io_4 = false;
static int32_t hw_value_io_5 = 0;
TM1637Display hw_display_io_5(4, 5);
static int32_t hw_display_previous_io_5 = INT32_MIN;
static int32_t hw_value_io_6 = 0;
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
  if (strcmp(begin, "ARM") == 0) { *value = 1L; return true; }
  if (strcmp(begin, "DISARM") == 0) { *value = 2L; return true; }
  if (strcmp(begin, "RESET") == 0) { *value = 3L; return true; }
  if (strcmp(begin, "STATUS") == 0) { *value = 4L; return true; }
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
static bool hw_value_io_6_valid = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(A0, INPUT);
  pinMode(6, OUTPUT);
  int32_t hw_tone_command_io_3 = (hw_value_io_3 >= 31 && hw_value_io_3 <= 65535) ? hw_value_io_3 : 0;
  if (hw_tone_command_io_3 != hw_tone_io_3) {
    if (hw_tone_command_io_3 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_3);
    hw_tone_io_3 = hw_tone_command_io_3;
  }
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_4 ? HIGH : LOW);
  hw_display_io_5.setBrightness(7, true);
  if (hw_value_io_5 != hw_display_previous_io_5) {
    if (hw_value_io_5 < -999 || hw_value_io_5 > 9999) hw_display_io_5.clear();
    else hw_display_io_5.showNumberDec((int)hw_value_io_5, false, 4, 0);
    hw_display_previous_io_5 = hw_value_io_5;
  }
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 5UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (digitalRead(2) == HIGH);
  }
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 5UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = (digitalRead(3) == HIGH);
  }
  if (!hw_attempted_io_2 || (uint32_t)(hw_now - hw_sampled_io_2) >= 5UL) {
    hw_attempted_io_2 = true; hw_sampled_io_2 = hw_now;
    hw_value_io_2 = (analogRead(A0) * 1.0 + 0.0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_6_valid = hw_rx_poll(hw_now, &hw_value_io_6);
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, hw_value_io_6, hw_value_io_6_valid, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5);
  int32_t hw_tone_command_io_3 = (hw_value_io_3 >= 31 && hw_value_io_3 <= 65535) ? hw_value_io_3 : 0;
  if (hw_tone_command_io_3 != hw_tone_io_3) {
    if (hw_tone_command_io_3 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_3);
    hw_tone_io_3 = hw_tone_command_io_3;
  }
  digitalWrite(13, hw_value_io_4 ? HIGH : LOW);
  if (hw_value_io_5 != hw_display_previous_io_5) {
    if (hw_value_io_5 < -999 || hw_value_io_5 > 9999) hw_display_io_5.clear();
    else hw_display_io_5.showNumberDec((int)hw_value_io_5, false, 4, 0);
    hw_display_previous_io_5 = hw_value_io_5;
  }
}
