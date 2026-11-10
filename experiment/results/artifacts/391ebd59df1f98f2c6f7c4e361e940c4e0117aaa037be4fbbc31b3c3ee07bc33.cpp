#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision10\\cases\\pceval_distance_sensor\\arduino\\sketch\\sketch.ino"
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
extern int32_t sysml_state_savedTarget;
/* Global Variable define of  */
extern bool sysml_state_targetPending;
/* Global Variable define of  */
extern bool sysml_state_promptDue;
/* Global Variable define of  */
extern bool sysml_state_promptGate;
/* Global Variable define of  */
extern bool sysml_state_greaterGate;
/* Global Variable define of  */
extern bool sysml_state_equalGate;
/* Global Variable define of  */
extern bool sysml_state_lessGate;
/* Global Variable define of  */
extern int32_t sysml_state_measuredInteger;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, int32_t io_2, bool io_2_valid, bool* io_3, bool* io_4, bool* io_5, bool* io_6);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_savedTarget = 0;
/* Global Variable define of  */
bool sysml_state_targetPending = false;
/* Global Variable define of  */
bool sysml_state_promptDue = true;
/* Global Variable define of  */
bool sysml_state_promptGate = false;
/* Global Variable define of  */
bool sysml_state_greaterGate = false;
/* Global Variable define of  */
bool sysml_state_equalGate = false;
/* Global Variable define of  */
bool sysml_state_lessGate = false;
/* Global Variable define of  */
int32_t sysml_state_measuredInteger = 0;
/* Init function of CompositeActor named "ProgramPage1" */
#line 131 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision10\\cases\\pceval_distance_sensor\\arduino\\sketch\\sketch.ino"
static bool hw_rx_parse(int32_t* value);
#line 155 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision10\\cases\\pceval_distance_sensor\\arduino\\sketch\\sketch.ino"
static bool hw_rx_finish(int32_t* value);
#line 160 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision10\\cases\\pceval_distance_sensor\\arduino\\sketch\\sketch.ino"
static bool hw_rx_poll(uint32_t hw_now, int32_t* value);
#line 185 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision10\\cases\\pceval_distance_sensor\\arduino\\sketch\\sketch.ino"
void setup();
#line 193 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision10\\cases\\pceval_distance_sensor\\arduino\\sketch\\sketch.ino"
void loop();
#line 57 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision10\\cases\\pceval_distance_sensor\\arduino\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_savedTarget = 0;
    sysml_state_targetPending = false;
    sysml_state_promptDue = true;
    sysml_state_promptGate = false;
    sysml_state_greaterGate = false;
    sysml_state_equalGate = false;
    sysml_state_lessGate = false;
    sysml_state_measuredInteger = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, int32_t io_2, bool io_2_valid, bool* io_3, bool* io_4, bool* io_5, bool* io_6) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int32_t sysml_conditional_1 = 0;
    sysml_state_promptGate = sysml_state_promptDue;
    sysml_state_promptDue = false;
    sysml_state_greaterGate = false;
    sysml_state_equalGate = false;
    sysml_state_lessGate = false;
    if (io_2_valid) {
        sysml_state_savedTarget = io_2;
        sysml_state_targetPending = true;
    }
    sysml_lazy_0 = sysml_state_targetPending;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_1;
    }
    if (sysml_lazy_0) {
        if (io_0 < ((double)((int32_t)io_0))) {
            sysml_conditional_1 = 1;
        }
        else {
            sysml_conditional_1 = 0;
        }
        sysml_state_measuredInteger = ((int32_t)io_0) - sysml_conditional_1;
        sysml_state_greaterGate = (sysml_state_measuredInteger > sysml_state_savedTarget);
        sysml_state_equalGate = (sysml_state_measuredInteger == sysml_state_savedTarget);
        sysml_state_lessGate = (sysml_state_measuredInteger < sysml_state_savedTarget);
        sysml_state_targetPending = false;
        sysml_state_promptDue = true;
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_promptGate;
    bool sysml_value_1 = 0;
    sysml_value_1 = sysml_state_greaterGate;
    bool sysml_value_2 = 0;
    sysml_value_2 = sysml_state_equalGate;
    bool sysml_value_3 = 0;
    sysml_value_3 = sysml_state_lessGate;
    *io_3 = sysml_value_0;
    *io_4 = sysml_value_1;
    *io_5 = sysml_value_2;
    *io_6 = sysml_value_3;
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
static double hw_value_io_0 = 0;
static bool hw_value_io_1 = false;
static int32_t hw_value_io_2 = 0;
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
static bool hw_value_io_2_valid = false;
static bool hw_value_io_3 = true;
static bool hw_value_io_4 = false;
static bool hw_value_io_5 = false;
static bool hw_value_io_6 = false;
static uint32_t hw_ultrasonic_sampled_0 = 0;
static bool hw_ultrasonic_attempted_0 = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(3, OUTPUT);
  digitalWrite(3, LOW);
  pinMode(2, INPUT);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  uint32_t hw_ultrasonic_now_0 = (uint32_t)millis();
  if (!hw_ultrasonic_attempted_0 || (uint32_t)(hw_ultrasonic_now_0 - hw_ultrasonic_sampled_0) >= 60UL) {
    hw_ultrasonic_attempted_0 = true; hw_ultrasonic_sampled_0 = hw_ultrasonic_now_0;
    digitalWrite(3, LOW); delayMicroseconds(2);
    digitalWrite(3, HIGH); delayMicroseconds(10); digitalWrite(3, LOW);
    unsigned long hw_ultrasonic_pulse_0 = pulseIn(2, HIGH, 25000UL);
    hw_value_io_1 = (hw_ultrasonic_pulse_0 >= 116UL && hw_ultrasonic_pulse_0 < 25000UL);
    if (hw_value_io_1) hw_value_io_0 = hw_ultrasonic_pulse_0 / 58.0;
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_2_valid = hw_rx_poll(hw_now, &hw_value_io_2);
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, hw_value_io_2_valid, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6);
  if (hw_value_io_3) Serial.println("Enter a distance in cm to compare: ");
  if (hw_value_io_4) Serial.println("Measured distance is greater");
  if (hw_value_io_5) Serial.println("Measured distance is equal to the target");
  if (hw_value_io_6) Serial.println("Measured distance is less");
}

