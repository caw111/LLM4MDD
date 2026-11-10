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
extern bool sysml_state_previousPressed;
/* Global Variable define of  */
extern int32_t sysml_state_sampleCount;
/* Global Variable define of  */
extern bool sysml_state_buzzerOn;
/* Global Variable define of  */
extern bool sysml_state_beepPending;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, double io_1, double io_2, bool* io_3, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, int32_t* serial_0_value_2);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
int32_t sysml_state_sampleCount = 0;
/* Global Variable define of  */
bool sysml_state_buzzerOn = false;
/* Global Variable define of  */
bool sysml_state_beepPending = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousPressed = false;
    sysml_state_sampleCount = 0;
    sysml_state_buzzerOn = false;
    sysml_state_beepPending = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, double io_1, double io_2, bool* io_3, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, int32_t* serial_0_value_2) {
    static bool previous=false,beeping=false;
    static uint32_t beepStart=0;
    bool pressed=io_0 && !previous;
    if (pressed) {
        ++sysml_state_sampleCount;
        if (sysml_state_sampleCount%5==0) {beepStart=millis();beeping=true;}
    }
    if (beeping && (uint32_t)(millis()-beepStart)>=200) beeping=false;
    *io_3=beeping;
    *serial_0_trigger=pressed;
    *serial_0_value_0=io_1;
    *serial_0_value_1=io_2*500.0/1023.0;
    *serial_0_value_2=sysml_state_sampleCount;
    previous=io_0;
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
static uint32_t hw_sampled_io_0 = 0;
static bool hw_attempted_io_0 = false;
static double hw_value_io_1 = 0;
static uint32_t hw_sampled_io_1 = 0;
static bool hw_attempted_io_1 = false;
static double hw_value_io_2 = 0;
static uint32_t hw_sampled_io_2 = 0;
static bool hw_attempted_io_2 = false;
static bool hw_value_io_3 = false;
static bool hw_value_serial_0_trigger = false;
static double hw_value_serial_0_value_0 = 0;
static double hw_value_serial_0_value_1 = 0;
static int32_t hw_value_serial_0_value_2 = 0;

static bool manual_debounce_input(bool raw, uint8_t index) {
  static bool previous[8]={}, stable[8]={};
  static uint32_t changed[8]={};
  uint32_t now=millis();
  if(raw!=previous[index]) {previous[index]=raw;changed[index]=now;}
  if((uint32_t)(now-changed[index])>=25UL) stable[index]=raw;
  return stable[index];
}

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(6, OUTPUT);
  digitalWrite(6, hw_value_io_3 ? HIGH : LOW);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 5UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = manual_debounce_input((digitalRead(2) == LOW), 0);
  }
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 5UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = (analogRead(A0) * 1.0 + 0.0);
  }
  if (!hw_attempted_io_2 || (uint32_t)(hw_now - hw_sampled_io_2) >= 5UL) {
    hw_attempted_io_2 = true; hw_sampled_io_2 = hw_now;
    hw_value_io_2 = (analogRead(A1) * 1.0 + 0.0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, &hw_value_io_3, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0, &hw_value_serial_0_value_1, &hw_value_serial_0_value_2);
  digitalWrite(6, hw_value_io_3 ? HIGH : LOW);
  if (hw_value_serial_0_trigger) { Serial.print("light"); Serial.println(hw_value_serial_0_value_0); }
  if (hw_value_serial_0_trigger) { Serial.print("temp"); Serial.println(hw_value_serial_0_value_1); }
  if (hw_value_serial_0_trigger) { Serial.print("count"); Serial.println(hw_value_serial_0_value_2); }
}
