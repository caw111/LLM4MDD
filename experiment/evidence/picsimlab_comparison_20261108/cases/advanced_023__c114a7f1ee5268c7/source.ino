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
extern bool sysml_state_previousDoor;
/* Global Variable define of  */
extern bool sysml_state_previousVibration;
/* Global Variable define of  */
extern int32_t sysml_state_abnormalCount;
/* Global Variable define of  */
extern int32_t sysml_state_doorOpenCount;
/* Global Variable define of  */
extern int32_t sysml_state_vibrationEventCount;
/* Global Variable define of  */
extern bool sysml_state_isAlarmLatched;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, bool io_2, int32_t io_6, bool io_6_valid, bool* io_3, int32_t* io_4, int32_t* io_5, bool* serial_0_trigger, double* serial_0_value_0, int32_t* serial_0_value_1, int32_t* serial_0_value_2, int32_t* serial_0_value_3, bool* serial_1_trigger, double* serial_1_value_0, int32_t* serial_1_value_1);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousDoor = false;
/* Global Variable define of  */
bool sysml_state_previousVibration = false;
/* Global Variable define of  */
int32_t sysml_state_abnormalCount = 0;
/* Global Variable define of  */
int32_t sysml_state_doorOpenCount = 0;
/* Global Variable define of  */
int32_t sysml_state_vibrationEventCount = 0;
/* Global Variable define of  */
bool sysml_state_isAlarmLatched = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousDoor = false;
    sysml_state_previousVibration = false;
    sysml_state_abnormalCount = 0;
    sysml_state_doorOpenCount = 0;
    sysml_state_vibrationEventCount = 0;
    sysml_state_isAlarmLatched = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, bool io_2, int32_t io_6, bool io_6_valid, bool* io_3, int32_t* io_4, int32_t* io_5, bool* serial_0_trigger, double* serial_0_value_0, int32_t* serial_0_value_1, int32_t* serial_0_value_2, int32_t* serial_0_value_3, bool* serial_1_trigger, double* serial_1_value_0, int32_t* serial_1_value_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    int32_t sysml_conditional_2 = 0;
    int32_t sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    double sysml_conditional_9 = 0;
    int32_t sysml_conditional_10 = 0;
    int32_t sysml_conditional_11 = 0;
    bool sysml_lazy_12 = 0;
    if (io_6_valid) {
        if ((io_6 == 1)) {
            sysml_state_isAlarmLatched = true;
        }
        if ((io_6 == 0)) {
            sysml_state_isAlarmLatched = false;
        }
    }
    if ((io_0 >= 30.0)) {
        sysml_state_abnormalCount = (sysml_state_abnormalCount + 1);
        sysml_state_isAlarmLatched = true;
    }
    sysml_lazy_0 = io_2;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousDoor;
    }
    if (sysml_lazy_0) {
        sysml_state_doorOpenCount = (sysml_state_doorOpenCount + 1);
        sysml_state_abnormalCount = (sysml_state_abnormalCount + 1);
        sysml_state_isAlarmLatched = true;
    }
    sysml_lazy_1 = io_1;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_previousVibration;
    }
    if (sysml_lazy_1) {
        sysml_state_vibrationEventCount = (sysml_state_vibrationEventCount + 1);
    }
    sysml_state_previousDoor = io_2;
    sysml_state_previousVibration = io_1;
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_isAlarmLatched;
    int32_t sysml_value_1 = 0;
    if (sysml_state_isAlarmLatched) {
        sysml_conditional_2 = 2000;
    }
    else {
        sysml_conditional_2 = 0;
    }
    sysml_value_1 = sysml_conditional_2;
    int32_t sysml_value_2 = 0;
    if (io_0 >= 30.0) {
        sysml_conditional_3 = sysml_state_abnormalCount;
    }
    else {
        sysml_conditional_3 = 0;
    }
    sysml_value_2 = sysml_conditional_3;
    bool sysml_value_3 = 0;
    sysml_lazy_4 = io_0 >= 30.0;
    if (!sysml_lazy_4) {
        sysml_lazy_5 = io_2;
        if (sysml_lazy_5) {
            sysml_lazy_5 = !sysml_state_previousDoor;
        }
        sysml_lazy_4 = sysml_lazy_5;
    }
    sysml_lazy_6 = sysml_lazy_4;
    if (!sysml_lazy_6) {
        sysml_lazy_7 = io_1;
        if (sysml_lazy_7) {
            sysml_lazy_7 = !sysml_state_previousVibration;
        }
        sysml_lazy_6 = sysml_lazy_7;
    }
    sysml_lazy_8 = sysml_lazy_6;
    if (!sysml_lazy_8) {
        sysml_lazy_8 = sysml_state_isAlarmLatched;
    }
    sysml_value_3 = sysml_lazy_8;
    double sysml_value_4 = 0;
    if (io_6_valid) {
        sysml_conditional_9 = io_0;
    }
    else {
        sysml_conditional_9 = io_0;
    }
    sysml_value_4 = sysml_conditional_9;
    int32_t sysml_value_5 = 0;
    sysml_value_5 = sysml_state_abnormalCount;
    int32_t sysml_value_6 = 0;
    if (io_6_valid) {
        sysml_conditional_10 = 0 + 0;
    }
    else {
        sysml_conditional_10 = 0;
    }
    sysml_value_6 = sysml_conditional_10;
    int32_t sysml_value_7 = 0;
    if (io_6_valid) {
        sysml_conditional_11 = 0 + 0;
    }
    else {
        sysml_conditional_11 = 0;
    }
    sysml_value_7 = sysml_conditional_11;
    bool sysml_value_8 = 0;
    sysml_lazy_12 = !sysml_state_isAlarmLatched;
    if (sysml_lazy_12) {
        sysml_lazy_12 = io_6_valid;
    }
    sysml_value_8 = sysml_lazy_12;
    double sysml_value_9 = 0;
    sysml_value_9 = io_0;
    int32_t sysml_value_10 = 0;
    sysml_value_10 = sysml_state_abnormalCount;
    *io_3 = sysml_value_0;
    *io_4 = sysml_value_1;
    *io_5 = sysml_value_2;
    *serial_0_trigger = sysml_value_3;
    *serial_0_value_0 = sysml_value_4;
    *serial_0_value_1 = sysml_value_5;
    *serial_0_value_2 = sysml_value_6;
    *serial_0_value_3 = sysml_value_7;
    *serial_1_trigger = sysml_value_8;
    *serial_1_value_0 = sysml_value_9;
    *serial_1_value_1 = sysml_value_10;
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
static double hw_value_io_0 = 0;
static uint32_t hw_sampled_io_0 = 0;
static bool hw_attempted_io_0 = false;
static bool hw_value_io_1 = false;
static uint32_t hw_sampled_io_1 = 0;
static bool hw_attempted_io_1 = false;
static bool hw_value_io_2 = false;
static uint32_t hw_sampled_io_2 = 0;
static bool hw_attempted_io_2 = false;
static bool hw_value_io_3 = false;
static int32_t hw_value_io_4 = 0;
static int32_t hw_tone_io_4 = -1;
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
static bool hw_value_io_6_valid = false;
static bool hw_value_serial_0_trigger = false;
static double hw_value_serial_0_value_0 = 0;
static int32_t hw_value_serial_0_value_1 = 0;
static int32_t hw_value_serial_0_value_2 = 0;
static int32_t hw_value_serial_0_value_3 = 0;
static bool hw_value_serial_1_trigger = false;
static double hw_value_serial_1_value_0 = 0;
static int32_t hw_value_serial_1_value_1 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(A1, INPUT);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_3 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  int32_t hw_tone_command_io_4 = (hw_value_io_4 >= 31 && hw_value_io_4 <= 65535) ? hw_value_io_4 : 0;
  if (hw_tone_command_io_4 != hw_tone_io_4) {
    if (hw_tone_command_io_4 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_4);
    hw_tone_io_4 = hw_tone_command_io_4;
  }
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
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 10UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (analogRead(A1) * 1.0 + 0.0);
  }
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 10UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = (digitalRead(2) == HIGH);
  }
  if (!hw_attempted_io_2 || (uint32_t)(hw_now - hw_sampled_io_2) >= 10UL) {
    hw_attempted_io_2 = true; hw_sampled_io_2 = hw_now;
    hw_value_io_2 = (digitalRead(3) == HIGH);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_6_valid = hw_rx_poll(hw_now, &hw_value_io_6);
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, hw_value_io_6, hw_value_io_6_valid, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0, &hw_value_serial_0_value_1, &hw_value_serial_0_value_2, &hw_value_serial_0_value_3, &hw_value_serial_1_trigger, &hw_value_serial_1_value_0, &hw_value_serial_1_value_1);
  digitalWrite(13, hw_value_io_3 ? HIGH : LOW);
  int32_t hw_tone_command_io_4 = (hw_value_io_4 >= 31 && hw_value_io_4 <= 65535) ? hw_value_io_4 : 0;
  if (hw_tone_command_io_4 != hw_tone_io_4) {
    if (hw_tone_command_io_4 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_4);
    hw_tone_io_4 = hw_tone_command_io_4;
  }
  if (hw_value_io_5 != hw_display_previous_io_5) {
    if (hw_value_io_5 < -999 || hw_value_io_5 > 9999) hw_display_io_5.clear();
    else hw_display_io_5.showNumberDec((int)hw_value_io_5, false, 4, 0);
    hw_display_previous_io_5 = hw_value_io_5;
  }
  if (hw_value_serial_0_trigger) { Serial.print("temperature_c"); Serial.println(hw_value_serial_0_value_0); }
  if (hw_value_serial_0_trigger) { Serial.print("abnormal_count"); Serial.println(hw_value_serial_0_value_1); }
  if (hw_value_serial_0_trigger) { Serial.print("gps_lat"); Serial.println(hw_value_serial_0_value_2); }
  if (hw_value_serial_0_trigger) { Serial.print("gps_lon"); Serial.println(hw_value_serial_0_value_3); }
  if (hw_value_serial_1_trigger) { Serial.print("temperature_c"); Serial.println(hw_value_serial_1_value_0); }
  if (hw_value_serial_1_trigger) { Serial.print("abnormal_count"); Serial.println(hw_value_serial_1_value_1); }
}
