#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision4\\cases\\pceval_7_segment_display_serial\\arduino\\sketch\\sketch.ino"
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
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_8, bool io_8_valid, bool* io_0, bool* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7, bool* serial_0_trigger, int32_t* serial_0_value_0);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Init function of CompositeActor named "ProgramPage1" */
#line 65 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision4\\cases\\pceval_7_segment_display_serial\\arduino\\sketch\\sketch.ino"
static bool hw_rx_parse(int32_t* value);
#line 89 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision4\\cases\\pceval_7_segment_display_serial\\arduino\\sketch\\sketch.ino"
static bool hw_rx_finish(int32_t* value);
#line 94 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision4\\cases\\pceval_7_segment_display_serial\\arduino\\sketch\\sketch.ino"
static bool hw_rx_poll(uint32_t hw_now, int32_t* value);
#line 115 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision4\\cases\\pceval_7_segment_display_serial\\arduino\\sketch\\sketch.ino"
void setup();
#line 136 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision4\\cases\\pceval_7_segment_display_serial\\arduino\\sketch\\sketch.ino"
void loop();
#line 25 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision4\\cases\\pceval_7_segment_display_serial\\arduino\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_8, bool io_8_valid, bool* io_0, bool* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7, bool* serial_0_trigger, int32_t* serial_0_value_0) {
    static uint8_t digit=0;
    if(io_8_valid&&io_8>=0&&io_8<=9) digit=io_8;
    const uint8_t patterns[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
    *io_0=(patterns[digit]&1)!=0;
    *io_1=(patterns[digit]&2)!=0;
    *io_2=(patterns[digit]&4)!=0;
    *io_3=(patterns[digit]&8)!=0;
    *io_4=(patterns[digit]&16)!=0;
    *io_5=(patterns[digit]&32)!=0;
    *io_6=(patterns[digit]&64)!=0;
    *io_7=false;*serial_0_trigger=io_8_valid;*serial_0_value_0=digit;
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
static bool hw_value_io_5 = false;
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
  if (parsed < 0L || parsed > 9L) return false;
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
static bool hw_value_io_8_valid = false;
static bool hw_value_serial_0_trigger = false;
static int32_t hw_value_serial_0_value_0 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  digitalWrite(2, hw_value_io_0 ? LOW : HIGH);
  pinMode(3, OUTPUT);
  digitalWrite(3, hw_value_io_1 ? LOW : HIGH);
  pinMode(4, OUTPUT);
  digitalWrite(4, hw_value_io_2 ? LOW : HIGH);
  pinMode(5, OUTPUT);
  digitalWrite(5, hw_value_io_3 ? LOW : HIGH);
  pinMode(6, OUTPUT);
  digitalWrite(6, hw_value_io_4 ? LOW : HIGH);
  pinMode(7, OUTPUT);
  digitalWrite(7, hw_value_io_5 ? LOW : HIGH);
  pinMode(8, OUTPUT);
  digitalWrite(8, hw_value_io_6 ? LOW : HIGH);
  pinMode(9, OUTPUT);
  digitalWrite(9, hw_value_io_7 ? LOW : HIGH);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_8_valid = hw_rx_poll(hw_now, &hw_value_io_8);
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_8, hw_value_io_8_valid, &hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_io_7, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0);
  digitalWrite(2, hw_value_io_0 ? LOW : HIGH);
  digitalWrite(3, hw_value_io_1 ? LOW : HIGH);
  digitalWrite(4, hw_value_io_2 ? LOW : HIGH);
  digitalWrite(5, hw_value_io_3 ? LOW : HIGH);
  digitalWrite(6, hw_value_io_4 ? LOW : HIGH);
  digitalWrite(7, hw_value_io_5 ? LOW : HIGH);
  digitalWrite(8, hw_value_io_6 ? LOW : HIGH);
  digitalWrite(9, hw_value_io_7 ? LOW : HIGH);
  if (hw_value_serial_0_trigger) { Serial.print("serial_display_value"); Serial.println(hw_value_serial_0_value_0); }
}

