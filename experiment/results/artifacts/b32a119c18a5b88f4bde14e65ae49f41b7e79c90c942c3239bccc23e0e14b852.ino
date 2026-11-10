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
extern double sysml_state_maxAdc;
/* Global Variable define of  */
extern bool sysml_state_previousPressed;
/* Global Variable define of  */
extern bool sysml_state_showMax;
/* Global Variable define of  */
extern int32_t sysml_state_displayMaxRemaining;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, int32_t* io_2, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_maxAdc = 0.0;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
bool sysml_state_showMax = false;
/* Global Variable define of  */
int32_t sysml_state_displayMaxRemaining = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_maxAdc = 0.0;
    sysml_state_previousPressed = false;
    sysml_state_showMax = false;
    sysml_state_displayMaxRemaining = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, int32_t* io_2, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1) {
    static unsigned long deadline=0;
    if (io_0>sysml_state_maxAdc) sysml_state_maxAdc=io_0;
    if (io_1 && !sysml_state_previousPressed) {deadline=millis()+3000UL;sysml_state_showMax=true;}
    if (sysml_state_showMax && (int32_t)(millis()-deadline)>=0) sysml_state_showMax=false;
    sysml_state_previousPressed=io_1;
    *io_2=(int32_t)(sysml_state_showMax ? sysml_state_maxAdc : io_0);
    *serial_0_trigger=true;
    *serial_0_value_0=io_0; *serial_0_value_1=sysml_state_maxAdc;
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
static int32_t hw_value_io_2 = 0;
TM1637Display hw_display_io_2(4, 5);
static int32_t hw_display_previous_io_2 = INT32_MIN;
static bool hw_value_serial_0_trigger = false;
static double hw_value_serial_0_value_0 = 0;
static double hw_value_serial_0_value_1 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(A1, INPUT);
  pinMode(2, INPUT_PULLUP);
  hw_display_io_2.setBrightness(7, true);
  if (hw_value_io_2 != hw_display_previous_io_2) {
    if (hw_value_io_2 < -999 || hw_value_io_2 > 9999) hw_display_io_2.clear();
    else hw_display_io_2.showNumberDec((int)hw_value_io_2, false, 4, 0);
    hw_display_previous_io_2 = hw_value_io_2;
  }
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 10UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = analogRead(A1)*500.0/1023.0;
  }
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 10UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = (digitalRead(2) == LOW);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, &hw_value_io_2, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0, &hw_value_serial_0_value_1);
  if (hw_value_io_2 != hw_display_previous_io_2) {
    if (hw_value_io_2 < -999 || hw_value_io_2 > 9999) hw_display_io_2.clear();
    else hw_display_io_2.showNumberDec((int)hw_value_io_2, false, 4, 0);
    hw_display_previous_io_2 = hw_value_io_2;
  }
  if (hw_value_serial_0_trigger) { Serial.print("current_temperature_degC"); Serial.println(hw_value_serial_0_value_0); }
  if (hw_value_serial_0_trigger) { Serial.print("historical_max_temperature_degC"); Serial.println(hw_value_serial_0_value_1); }
}
