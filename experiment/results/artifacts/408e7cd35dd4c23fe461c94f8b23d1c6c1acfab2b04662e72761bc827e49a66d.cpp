#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision24\\cases\\advanced_013\\arduino\\sketch\\sketch.ino"
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
#line 144 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision24\\cases\\advanced_013\\arduino\\sketch\\sketch.ino"
static bool hw_rx_parse(int32_t* value);
#line 155 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision24\\cases\\advanced_013\\arduino\\sketch\\sketch.ino"
static bool hw_rx_finish(int32_t* value);
#line 160 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision24\\cases\\advanced_013\\arduino\\sketch\\sketch.ino"
static bool hw_rx_poll(uint32_t hw_now, int32_t* value);
#line 184 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision24\\cases\\advanced_013\\arduino\\sketch\\sketch.ino"
void setup();
#line 207 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision24\\cases\\advanced_013\\arduino\\sketch\\sketch.ino"
void loop();
#line 69 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision24\\cases\\advanced_013\\arduino\\sketch\\sketch.ino"
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
    static uint32_t vacantSince[3]={},filledAt=0;
    static bool reported[3]={},wasFull=false,beeping=false;
    bool occupied[]={io_0,io_1,io_2};
    uint32_t now=millis();
    int count=(int)io_0+(int)io_1+(int)io_2;
    bool full=count==3;
    bool reset=io_7_valid && io_7==2;
    *io_8=false;
    for (uint8_t i=0; i<3; ++i) {
        if (occupied[i] || reset) {vacantSince[i]=now;reported[i]=false;}
        else if (!reported[i] && (uint32_t)(now-vacantSince[i])>60000) {
            *io_8=true;
            reported[i]=true;
        }
    }
    if (full && !wasFull) {filledAt=now;beeping=true;}
    if (!full || (uint32_t)(now-filledAt)>=250) beeping=false;
    wasFull=full;
    *io_3=count;
    *io_4=beeping ? 1000 : 0;
    *io_5=full;
    *serial_0_trigger=io_7_valid && io_7==1;
    *serial_0_value_0=io_0;
    *serial_0_value_1=io_1;
    *serial_0_value_2=io_2;
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

