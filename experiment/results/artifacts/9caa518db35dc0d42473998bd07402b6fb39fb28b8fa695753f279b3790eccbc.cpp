#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_003\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
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
extern bool sysml_state_alarmActive;
/* Global Variable define of  */
extern int32_t sysml_state_beepPhase;
/* Global Variable define of  */
extern bool sysml_state_lastValidWithin10;
/* Global Variable define of  */
extern bool sysml_state_standbyAfterGuard;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_2, bool io_3, bool* io_0, int32_t* io_1, bool* io_4, bool* serial_0_trigger, double* serial_0_value_0);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_alarmActive = false;
/* Global Variable define of  */
int32_t sysml_state_beepPhase = 0;
/* Global Variable define of  */
bool sysml_state_lastValidWithin10 = false;
/* Global Variable define of  */
bool sysml_state_standbyAfterGuard = false;
/* Init function of CompositeActor named "ProgramPage1" */
#line 118 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_003\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 135 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_003\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 41 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_003\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_alarmActive = false;
    sysml_state_beepPhase = 0;
    sysml_state_lastValidWithin10 = false;
    sysml_state_standbyAfterGuard = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_2, bool io_3, bool* io_0, int32_t* io_1, bool* io_4, bool* serial_0_trigger, double* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    int32_t sysml_conditional_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    if (io_3) {
        sysml_state_lastValidWithin10 = true;
    }
    bool sysml_value_0 = 0;
    sysml_lazy_0 = io_2 >= 10.0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_3;
    }
    sysml_value_0 = sysml_lazy_0;
    int32_t sysml_value_1 = 0;
    sysml_lazy_1 = io_2 >= 10.0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = io_3;
    }
    if (sysml_lazy_1) {
        sysml_conditional_2 = 1000;
    }
    else {
        sysml_conditional_2 = 0;
    }
    sysml_value_1 = sysml_conditional_2;
    bool sysml_value_2 = 0;
    sysml_lazy_3 = io_2 >= 10.0;
    if (sysml_lazy_3) {
        sysml_lazy_3 = io_3;
    }
    sysml_value_2 = sysml_lazy_3;
    bool sysml_value_3 = 0;
    sysml_lazy_4 = io_2 >= 10.0;
    if (sysml_lazy_4) {
        sysml_lazy_4 = io_3;
    }
    sysml_value_3 = sysml_lazy_4;
    double sysml_value_4 = 0;
    sysml_value_4 = io_2;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_4 = sysml_value_2;
    *serial_0_trigger = sysml_value_3;
    *serial_0_value_0 = sysml_value_4;
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
static int32_t hw_value_io_1 = 0;
static int32_t hw_tone_io_1 = -1;
static double hw_value_io_2 = 0;
static bool hw_value_io_3 = false;
static bool hw_value_io_4 = false;
static bool hw_value_serial_0_trigger = false;
static double hw_value_serial_0_value_0 = 0;
static uint32_t hw_ultrasonic_sampled_0 = 0;
static bool hw_ultrasonic_attempted_0 = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_0 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  int32_t hw_tone_command_io_1 = (hw_value_io_1 >= 31 && hw_value_io_1 <= 65535) ? hw_value_io_1 : 0;
  if (hw_tone_command_io_1 != hw_tone_io_1) {
    if (hw_tone_command_io_1 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_1);
    hw_tone_io_1 = hw_tone_command_io_1;
  }
  pinMode(8, OUTPUT);
  digitalWrite(8, LOW);
  pinMode(9, INPUT);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  uint32_t hw_ultrasonic_now_0 = (uint32_t)millis();
  if (!hw_ultrasonic_attempted_0 || (uint32_t)(hw_ultrasonic_now_0 - hw_ultrasonic_sampled_0) >= 70UL) {
    hw_ultrasonic_attempted_0 = true; hw_ultrasonic_sampled_0 = hw_ultrasonic_now_0;
    digitalWrite(8, LOW); delayMicroseconds(2);
    digitalWrite(8, HIGH); delayMicroseconds(10); digitalWrite(8, LOW);
    unsigned long hw_ultrasonic_pulse_0 = pulseIn(9, HIGH, 30000UL);
    hw_value_io_3 = (hw_ultrasonic_pulse_0 >= 116UL && hw_ultrasonic_pulse_0 <= 23200UL);
    if (hw_value_io_3) hw_value_io_2 = hw_ultrasonic_pulse_0 / 58.0;
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_2, hw_value_io_3, &hw_value_io_0, &hw_value_io_1, &hw_value_io_4, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0);
  digitalWrite(13, hw_value_io_0 ? HIGH : LOW);
  int32_t hw_tone_command_io_1 = (hw_value_io_1 >= 31 && hw_value_io_1 <= 65535) ? hw_value_io_1 : 0;
  if (hw_tone_command_io_1 != hw_tone_io_1) {
    if (hw_tone_command_io_1 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_1);
    hw_tone_io_1 = hw_tone_command_io_1;
  }
  if (hw_value_io_4) Serial.println("welcome");
  if (hw_value_serial_0_trigger) { Serial.print("proximity_distance_cm"); Serial.println(hw_value_serial_0_value_0); }
}

