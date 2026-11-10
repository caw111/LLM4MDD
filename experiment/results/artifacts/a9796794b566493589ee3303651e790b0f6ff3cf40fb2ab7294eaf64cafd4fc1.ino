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
extern int32_t sysml_state_vacantTimeOne;
/* Global Variable define of  */
extern int32_t sysml_state_vacantTimeTwo;
/* Global Variable define of  */
extern int32_t sysml_state_vacantTimeThree;
/* Global Variable define of  */
extern bool sysml_state_previousOccupiedOne;
/* Global Variable define of  */
extern bool sysml_state_previousOccupiedTwo;
/* Global Variable define of  */
extern bool sysml_state_previousOccupiedThree;
/* Global Variable define of  */
extern bool sysml_state_reportedOne;
/* Global Variable define of  */
extern bool sysml_state_reportedTwo;
/* Global Variable define of  */
extern bool sysml_state_reportedThree;
/* Global Variable define of  */
extern bool sysml_state_restockReport;
/* Global Variable define of  */
extern bool sysml_state_recordsLocked;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, int32_t io_6, int32_t io_7, bool io_7_valid, int32_t* io_3, int32_t* io_4, bool* io_5, bool* io_8, bool* serial_0_trigger, bool* serial_0_value_0, bool* serial_0_value_1, bool* serial_0_value_2);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_vacantTimeOne = 0;
/* Global Variable define of  */
int32_t sysml_state_vacantTimeTwo = 0;
/* Global Variable define of  */
int32_t sysml_state_vacantTimeThree = 0;
/* Global Variable define of  */
bool sysml_state_previousOccupiedOne = false;
/* Global Variable define of  */
bool sysml_state_previousOccupiedTwo = false;
/* Global Variable define of  */
bool sysml_state_previousOccupiedThree = false;
/* Global Variable define of  */
bool sysml_state_reportedOne = false;
/* Global Variable define of  */
bool sysml_state_reportedTwo = false;
/* Global Variable define of  */
bool sysml_state_reportedThree = false;
/* Global Variable define of  */
bool sysml_state_restockReport = false;
/* Global Variable define of  */
bool sysml_state_recordsLocked = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_vacantTimeOne = 0;
    sysml_state_vacantTimeTwo = 0;
    sysml_state_vacantTimeThree = 0;
    sysml_state_previousOccupiedOne = false;
    sysml_state_previousOccupiedTwo = false;
    sysml_state_previousOccupiedThree = false;
    sysml_state_reportedOne = false;
    sysml_state_reportedTwo = false;
    sysml_state_reportedThree = false;
    sysml_state_restockReport = false;
    sysml_state_recordsLocked = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, int32_t io_6, int32_t io_7, bool io_7_valid, int32_t* io_3, int32_t* io_4, bool* io_5, bool* io_8, bool* serial_0_trigger, bool* serial_0_value_0, bool* serial_0_value_1, bool* serial_0_value_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    int32_t sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    int32_t sysml_conditional_9 = 0;
    bool sysml_lazy_10 = 0;
    bool sysml_lazy_11 = 0;
    bool sysml_lazy_12 = 0;
    int32_t sysml_conditional_13 = 0;
    bool sysml_lazy_14 = 0;
    bool sysml_lazy_15 = 0;
    bool sysml_lazy_16 = 0;
    bool sysml_lazy_17 = 0;
    bool sysml_lazy_18 = 0;
    bool sysml_lazy_19 = 0;
    int32_t sysml_conditional_20 = 0;
    int32_t sysml_conditional_21 = 0;
    int32_t sysml_conditional_22 = 0;
    bool sysml_lazy_23 = 0;
    int32_t sysml_conditional_24 = 0;
    bool sysml_lazy_25 = 0;
    bool sysml_lazy_26 = 0;
    bool sysml_lazy_27 = 0;
    sysml_state_restockReport = false;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_1;
    }
    sysml_lazy_1 = sysml_lazy_0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = io_2;
    }
    sysml_state_recordsLocked = sysml_lazy_1;
    sysml_lazy_2 = io_7_valid;
    if (sysml_lazy_2) {
        sysml_lazy_2 = io_7 == 2;
    }
    sysml_lazy_3 = sysml_lazy_2;
    if (!sysml_lazy_3) {
        sysml_lazy_3 = io_0;
    }
    sysml_lazy_4 = sysml_lazy_3;
    if (!sysml_lazy_4) {
        sysml_lazy_4 = sysml_state_previousOccupiedOne;
    }
    if (sysml_lazy_4) {
        sysml_state_vacantTimeOne = 0;
        sysml_state_reportedOne = false;
    }
    else {
        if (io_6 >= ((60000 + 1) - sysml_state_vacantTimeOne)) {
            sysml_conditional_5 = 60000 + 1;
        }
        else {
            sysml_conditional_5 = sysml_state_vacantTimeOne + io_6;
        }
        sysml_state_vacantTimeOne = sysml_conditional_5;
    }
    sysml_lazy_6 = io_7_valid;
    if (sysml_lazy_6) {
        sysml_lazy_6 = io_7 == 2;
    }
    sysml_lazy_7 = sysml_lazy_6;
    if (!sysml_lazy_7) {
        sysml_lazy_7 = io_1;
    }
    sysml_lazy_8 = sysml_lazy_7;
    if (!sysml_lazy_8) {
        sysml_lazy_8 = sysml_state_previousOccupiedTwo;
    }
    if (sysml_lazy_8) {
        sysml_state_vacantTimeTwo = 0;
        sysml_state_reportedTwo = false;
    }
    else {
        if (io_6 >= ((60000 + 1) - sysml_state_vacantTimeTwo)) {
            sysml_conditional_9 = 60000 + 1;
        }
        else {
            sysml_conditional_9 = sysml_state_vacantTimeTwo + io_6;
        }
        sysml_state_vacantTimeTwo = sysml_conditional_9;
    }
    sysml_lazy_10 = io_7_valid;
    if (sysml_lazy_10) {
        sysml_lazy_10 = io_7 == 2;
    }
    sysml_lazy_11 = sysml_lazy_10;
    if (!sysml_lazy_11) {
        sysml_lazy_11 = io_2;
    }
    sysml_lazy_12 = sysml_lazy_11;
    if (!sysml_lazy_12) {
        sysml_lazy_12 = sysml_state_previousOccupiedThree;
    }
    if (sysml_lazy_12) {
        sysml_state_vacantTimeThree = 0;
        sysml_state_reportedThree = false;
    }
    else {
        if (io_6 >= ((60000 + 1) - sysml_state_vacantTimeThree)) {
            sysml_conditional_13 = 60000 + 1;
        }
        else {
            sysml_conditional_13 = sysml_state_vacantTimeThree + io_6;
        }
        sysml_state_vacantTimeThree = sysml_conditional_13;
    }
    sysml_lazy_14 = sysml_state_vacantTimeOne > 60000;
    if (sysml_lazy_14) {
        sysml_lazy_14 = !sysml_state_reportedOne;
    }
    if (sysml_lazy_14) {
        sysml_state_restockReport = true;
        sysml_state_reportedOne = true;
    }
    sysml_lazy_15 = sysml_state_vacantTimeTwo > 60000;
    if (sysml_lazy_15) {
        sysml_lazy_15 = !sysml_state_reportedTwo;
    }
    if (sysml_lazy_15) {
        sysml_state_restockReport = true;
        sysml_state_reportedTwo = true;
    }
    sysml_lazy_16 = sysml_state_vacantTimeThree > 60000;
    if (sysml_lazy_16) {
        sysml_lazy_16 = !sysml_state_reportedThree;
    }
    if (sysml_lazy_16) {
        sysml_state_restockReport = true;
        sysml_state_reportedThree = true;
    }
    sysml_state_previousOccupiedOne = io_0;
    sysml_state_previousOccupiedTwo = io_1;
    sysml_state_previousOccupiedThree = io_2;
    bool sysml_value_0 = 0;
    sysml_lazy_17 = io_0;
    if (sysml_lazy_17) {
        sysml_lazy_17 = io_1;
    }
    sysml_lazy_18 = sysml_lazy_17;
    if (sysml_lazy_18) {
        sysml_lazy_18 = io_2;
    }
    sysml_value_0 = sysml_lazy_18;
    bool sysml_value_1 = 0;
    sysml_lazy_19 = io_7_valid;
    if (sysml_lazy_19) {
        sysml_lazy_19 = io_7 == 2;
    }
    sysml_value_1 = sysml_lazy_19;
    int32_t sysml_value_2 = 0;
    sysml_value_2 = 60000;
    int32_t sysml_value_3 = 0;
    if (io_0) {
        sysml_conditional_20 = 1;
    }
    else {
        sysml_conditional_20 = 0;
    }
    sysml_value_3 = sysml_conditional_20;
    int32_t sysml_value_4 = 0;
    if (io_1) {
        sysml_conditional_21 = 1;
    }
    else {
        sysml_conditional_21 = 0;
    }
    sysml_value_4 = sysml_conditional_21;
    int32_t sysml_value_5 = 0;
    if (io_2) {
        sysml_conditional_22 = 1;
    }
    else {
        sysml_conditional_22 = 0;
    }
    sysml_value_5 = sysml_conditional_22;
    bool sysml_value_6 = 0;
    sysml_value_6 = sysml_state_restockReport;
    bool sysml_value_7 = 0;
    sysml_lazy_23 = io_7_valid;
    if (sysml_lazy_23) {
        sysml_lazy_23 = io_7 == 1;
    }
    sysml_value_7 = sysml_lazy_23;
    bool sysml_value_8 = 0;
    sysml_value_8 = io_0;
    bool sysml_value_9 = 0;
    sysml_value_9 = io_1;
    bool sysml_value_10 = 0;
    sysml_value_10 = io_2;
    int32_t sysml_value_11 = 0;
    if (sysml_value_0) {
        sysml_conditional_24 = 2000;
    }
    else {
        sysml_conditional_24 = 0;
    }
    sysml_value_11 = sysml_conditional_24;
    bool sysml_value_12 = 0;
    sysml_lazy_25 = sysml_value_0;
    if (!sysml_lazy_25) {
        sysml_lazy_25 = sysml_state_vacantTimeOne > sysml_value_2;
    }
    sysml_lazy_26 = sysml_lazy_25;
    if (!sysml_lazy_26) {
        sysml_lazy_26 = sysml_state_vacantTimeTwo > sysml_value_2;
    }
    sysml_lazy_27 = sysml_lazy_26;
    if (!sysml_lazy_27) {
        sysml_lazy_27 = sysml_state_vacantTimeThree > sysml_value_2;
    }
    sysml_value_12 = sysml_lazy_27;
    int32_t sysml_value_13 = 0;
    sysml_value_13 = ((sysml_value_3 + sysml_value_4) + sysml_value_5);
    *io_3 = sysml_value_13;
    *io_4 = sysml_value_11;
    *io_5 = sysml_value_12;
    *io_8 = sysml_value_6;
    *serial_0_trigger = sysml_value_7;
    *serial_0_value_0 = sysml_value_8;
    *serial_0_value_1 = sysml_value_9;
    *serial_0_value_2 = sysml_value_10;
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
static bool hw_value_io_2 = false;
static uint32_t hw_sampled_io_2 = 0;
static bool hw_attempted_io_2 = false;
static int32_t hw_value_io_3 = 0;
TM1637Display hw_display_io_3(4, 5);
static int32_t hw_display_previous_io_3 = INT32_MIN;
static int32_t hw_value_io_4 = 0;
static int32_t hw_tone_io_4 = -1;
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
  if (strcmp(begin, "QUERY") == 0) { *value = 1L; return true; }
  if (strcmp(begin, "RESET") == 0) { *value = 2L; return true; }
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
static bool hw_value_serial_0_value_0 = false;
static bool hw_value_serial_0_value_1 = false;
static bool hw_value_serial_0_value_2 = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(7, INPUT);
  hw_display_io_3.setBrightness(7, true);
  if (hw_value_io_3 != hw_display_previous_io_3) {
    if (hw_value_io_3 < -999 || hw_value_io_3 > 9999) hw_display_io_3.clear();
    else hw_display_io_3.showNumberDec((int)hw_value_io_3, false, 4, 0);
    hw_display_previous_io_3 = hw_value_io_3;
  }
  pinMode(6, OUTPUT);
  int32_t hw_tone_command_io_4 = (hw_value_io_4 >= 31 && hw_value_io_4 <= 65535) ? hw_value_io_4 : 0;
  if (hw_tone_command_io_4 != hw_tone_io_4) {
    if (hw_tone_command_io_4 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_4);
    hw_tone_io_4 = hw_tone_command_io_4;
  }
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_5 ? HIGH : LOW);
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
    hw_value_io_2 = (digitalRead(7) == HIGH);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_6 = (int32_t)hw_elapsed;
  hw_value_io_7_valid = hw_rx_poll(hw_now, &hw_value_io_7);
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, hw_value_io_6, hw_value_io_7, hw_value_io_7_valid, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_8, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0, &hw_value_serial_0_value_1, &hw_value_serial_0_value_2);
  if (hw_value_io_3 != hw_display_previous_io_3) {
    if (hw_value_io_3 < -999 || hw_value_io_3 > 9999) hw_display_io_3.clear();
    else hw_display_io_3.showNumberDec((int)hw_value_io_3, false, 4, 0);
    hw_display_previous_io_3 = hw_value_io_3;
  }
  int32_t hw_tone_command_io_4 = (hw_value_io_4 >= 31 && hw_value_io_4 <= 65535) ? hw_value_io_4 : 0;
  if (hw_tone_command_io_4 != hw_tone_io_4) {
    if (hw_tone_command_io_4 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_4);
    hw_tone_io_4 = hw_tone_command_io_4;
  }
  digitalWrite(13, hw_value_io_5 ? HIGH : LOW);
  if (hw_value_io_8) Serial.println("restock needed");
  if (hw_value_serial_0_trigger) { Serial.print("level1_occupied"); Serial.println(hw_value_serial_0_value_0); }
  if (hw_value_serial_0_trigger) { Serial.print("level2_occupied"); Serial.println(hw_value_serial_0_value_1); }
  if (hw_value_serial_0_trigger) { Serial.print("level3_occupied"); Serial.println(hw_value_serial_0_value_2); }
}
