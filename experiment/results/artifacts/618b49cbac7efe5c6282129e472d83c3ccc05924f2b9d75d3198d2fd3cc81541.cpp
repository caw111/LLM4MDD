#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision10\\cases\\complex_013\\arduino\\sketch\\sketch.ino"
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
extern bool sysml_state_lightOn;
/* Global Variable define of  */
extern int32_t sysml_state_count;
/* Global Variable define of  */
extern bool sysml_state_prevIo0;
/* Global Variable define of  */
extern bool sysml_state_prevIo1;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, int32_t io_5, bool io_5_valid, bool* io_2, int32_t* io_3, int32_t* io_4);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_lightOn = false;
/* Global Variable define of  */
int32_t sysml_state_count = 0;
/* Global Variable define of  */
bool sysml_state_prevIo0 = false;
/* Global Variable define of  */
bool sysml_state_prevIo1 = false;
/* Init function of CompositeActor named "ProgramPage1" */
#line 93 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision10\\cases\\complex_013\\arduino\\sketch\\sketch.ino"
static bool hw_rx_parse(int32_t* value);
#line 108 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision10\\cases\\complex_013\\arduino\\sketch\\sketch.ino"
static bool hw_rx_finish(int32_t* value);
#line 113 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision10\\cases\\complex_013\\arduino\\sketch\\sketch.ino"
static bool hw_rx_poll(uint32_t hw_now, int32_t* value);
#line 132 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision10\\cases\\complex_013\\arduino\\sketch\\sketch.ino"
void setup();
#line 154 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision10\\cases\\complex_013\\arduino\\sketch\\sketch.ino"
void loop();
#line 41 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision10\\cases\\complex_013\\arduino\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_lightOn = false;
    sysml_state_count = 0;
    sysml_state_prevIo0 = false;
    sysml_state_prevIo1 = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, int32_t io_5, bool io_5_valid, bool* io_2, int32_t* io_3, int32_t* io_4) {
    static bool previous[2]={},beeping=false;
    static uint32_t beepStart=0;
    if(io_0&&!previous[0]) {sysml_state_lightOn=true;++sysml_state_count;}
    if(io_1&&!previous[1]) {sysml_state_lightOn=false;++sysml_state_count;}
    if(io_5_valid) {
      if(io_5==6) {sysml_state_lightOn=true;++sysml_state_count;}
      else if(io_5==5) {sysml_state_lightOn=false;++sysml_state_count;}
      else if(io_5==1) {beeping=true;beepStart=millis();++sysml_state_count;}
      else if(io_5==3) sysml_state_count=0;
    }
    if(beeping && (uint32_t)(millis()-beepStart)>=200) beeping=false;
    *io_2=sysml_state_lightOn;*io_3=beeping ? 1000 : 0;*io_4=sysml_state_count;
    previous[0]=io_0;previous[1]=io_1;
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
static int32_t hw_value_io_3 = 0;
static int32_t hw_tone_io_3 = -1;
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
  if (strcmp(begin, "BEEP") == 0) { *value = 1L; return true; }
  if (strcmp(begin, "CLK") == 0) { *value = 2L; return true; }
  if (strcmp(begin, "COUNT_RESET") == 0) { *value = 3L; return true; }
  if (strcmp(begin, "DIO") == 0) { *value = 4L; return true; }
  if (strcmp(begin, "LIGHT_OFF") == 0) { *value = 5L; return true; }
  if (strcmp(begin, "LIGHT_ON") == 0) { *value = 6L; return true; }
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

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_2 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  int32_t hw_tone_command_io_3 = (hw_value_io_3 >= 31 && hw_value_io_3 <= 65535) ? hw_value_io_3 : 0;
  if (hw_tone_command_io_3 != hw_tone_io_3) {
    if (hw_tone_command_io_3 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_3);
    hw_tone_io_3 = hw_tone_command_io_3;
  }
  hw_display_io_4.setBrightness(7, true);
  if (hw_value_io_4 != hw_display_previous_io_4) {
    if (hw_value_io_4 < -999 || hw_value_io_4 > 9999) hw_display_io_4.clear();
    else hw_display_io_4.showNumberDec((int)hw_value_io_4, false, 4, 0);
    hw_display_previous_io_4 = hw_value_io_4;
  }
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 5UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (digitalRead(2) == LOW);
  }
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 5UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = (digitalRead(3) == LOW);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_5_valid = hw_rx_poll(hw_now, &hw_value_io_5);
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_5, hw_value_io_5_valid, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4);
  digitalWrite(13, hw_value_io_2 ? HIGH : LOW);
  int32_t hw_tone_command_io_3 = (hw_value_io_3 >= 31 && hw_value_io_3 <= 65535) ? hw_value_io_3 : 0;
  if (hw_tone_command_io_3 != hw_tone_io_3) {
    if (hw_tone_command_io_3 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_3);
    hw_tone_io_3 = hw_tone_command_io_3;
  }
  if (hw_value_io_4 != hw_display_previous_io_4) {
    if (hw_value_io_4 < -999 || hw_value_io_4 > 9999) hw_display_io_4.clear();
    else hw_display_io_4.showNumberDec((int)hw_value_io_4, false, 4, 0);
    hw_display_previous_io_4 = hw_value_io_4;
  }
}

