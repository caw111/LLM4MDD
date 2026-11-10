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
extern double sysml_state_lastValidDistanceCm;
/* Global Variable define of  */
extern bool sysml_state_distanceValidHeld;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, int32_t io_2, bool* serial_0_trigger, double* serial_0_value_0);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_lastValidDistanceCm = 400.0;
/* Global Variable define of  */
bool sysml_state_distanceValidHeld = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_lastValidDistanceCm = 400.0;
    sysml_state_distanceValidHeld = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, int32_t io_2, bool* serial_0_trigger, double* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_conditional_0 = 0;
    if (io_1) {
        sysml_state_lastValidDistanceCm = io_0;
        sysml_state_distanceValidHeld = true;
    }
    else {
        sysml_state_distanceValidHeld = false;
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = true;
    double sysml_value_1 = 0;
    if (sysml_state_distanceValidHeld) {
        sysml_conditional_0 = sysml_state_lastValidDistanceCm;
    }
    else {
        sysml_conditional_0 = sysml_state_lastValidDistanceCm;
    }
    sysml_value_1 = sysml_conditional_0;
    *serial_0_trigger = sysml_value_0;
    *serial_0_value_0 = sysml_value_1;
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
static bool hw_value_serial_0_trigger = true;
static double hw_value_serial_0_value_0 = 0;
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
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  uint32_t hw_ultrasonic_now_0 = (uint32_t)millis();
  if (!hw_ultrasonic_attempted_0 || (uint32_t)(hw_ultrasonic_now_0 - hw_ultrasonic_sampled_0) >= 60UL) {
    hw_ultrasonic_attempted_0 = true; hw_ultrasonic_sampled_0 = hw_ultrasonic_now_0;
    digitalWrite(3, LOW); delayMicroseconds(2);
    digitalWrite(3, HIGH); delayMicroseconds(10); digitalWrite(3, LOW);
    unsigned long hw_ultrasonic_pulse_0 = pulseIn(2, HIGH, 30000UL);
    hw_value_io_1 = (hw_ultrasonic_pulse_0 >= 116UL && hw_ultrasonic_pulse_0 <= 23200UL);
    if (hw_value_io_1) hw_value_io_0 = hw_ultrasonic_pulse_0 / 58.0;
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_2 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0);
  if (hw_value_serial_0_trigger) { Serial.print("Distance in CM"); Serial.println(hw_value_serial_0_value_0); }
}
