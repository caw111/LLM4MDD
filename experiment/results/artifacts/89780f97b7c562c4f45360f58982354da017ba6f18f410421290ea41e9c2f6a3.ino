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
extern double sysml_state_servoAngleDeg;
/* Global Variable define of  */
extern bool sysml_state_directionUp;
/* Global Variable define of  */
extern bool sysml_state_started;
/* Global Variable define of  */
extern bool sysml_state_reportPending;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double* io_0, bool* serial_0_trigger, int32_t* serial_0_value_0);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_servoAngleDeg = 0.0;
/* Global Variable define of  */
bool sysml_state_directionUp = true;
/* Global Variable define of  */
bool sysml_state_started = false;
/* Global Variable define of  */
bool sysml_state_reportPending = true;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_servoAngleDeg = 0.0;
    sysml_state_directionUp = true;
    sysml_state_started = false;
    sysml_state_reportPending = true;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double* io_0, bool* serial_0_trigger, int32_t* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_conditional_0 = 0;
    double sysml_conditional_1 = 0;
    int32_t sysml_conditional_2 = 0;
    if ((!sysml_state_started)) {
        sysml_state_started = true;
        sysml_state_directionUp = true;
        sysml_state_servoAngleDeg = 0.0;
        sysml_state_reportPending = true;
    }
    if (sysml_state_directionUp) {
        if (sysml_state_servoAngleDeg >= 180.0) {
            sysml_conditional_0 = 180.0;
        }
        else {
            sysml_conditional_0 = sysml_state_servoAngleDeg + 1.0;
        }
        sysml_state_servoAngleDeg = sysml_conditional_0;
        if ((sysml_state_servoAngleDeg >= 179.0)) {
            sysml_state_directionUp = false;
            sysml_state_reportPending = true;
        }
        else {
            sysml_state_reportPending = false;
        }
    }
    else {
        if (sysml_state_servoAngleDeg <= 0.0) {
            sysml_conditional_1 = 0.0;
        }
        else {
            sysml_conditional_1 = sysml_state_servoAngleDeg - 1.0;
        }
        sysml_state_servoAngleDeg = sysml_conditional_1;
        if ((sysml_state_servoAngleDeg <= 1.0)) {
            sysml_state_directionUp = true;
            sysml_state_reportPending = true;
        }
        else {
            sysml_state_reportPending = false;
        }
    }
    double sysml_value_0 = 0;
    sysml_value_0 = sysml_state_servoAngleDeg;
    bool sysml_value_1 = 0;
    sysml_value_1 = sysml_state_reportPending;
    int32_t sysml_value_2 = 0;
    if (sysml_state_servoAngleDeg < ((double)((int32_t)sysml_state_servoAngleDeg))) {
        sysml_conditional_2 = 1;
    }
    else {
        sysml_conditional_2 = 0;
    }
    sysml_value_2 = ((int32_t)sysml_state_servoAngleDeg) - sysml_conditional_2;
    *io_0 = sysml_value_0;
    *serial_0_trigger = sysml_value_1;
    *serial_0_value_0 = sysml_value_2;
}
// END COMPILED SYSML MODEL

// LLM4MDD target-board: arduino:avr:uno
#include <Servo.h>
#include <Arduino.h>
#include <math.h>
#if defined(ARDUINO) && !defined(ARDUINO_AVR_UNO)
#error "Selected Arduino board differs from the interface contract"
#endif
static uint32_t hw_last_step = 0;
static bool hw_started = false;
static double hw_value_io_0 = 0;
Servo hw_servo_io_0;
static int hw_servo_previous_io_0 = -1;
static bool hw_value_serial_0_trigger = false;
static int32_t hw_value_serial_0_value_0 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  hw_servo_io_0.attach(9);
  if (isfinite((double)hw_value_io_0) && hw_value_io_0 >= 0 && hw_value_io_0 <= 180) {
    int command = (int)hw_value_io_0;
    if (command != hw_servo_previous_io_0) {
      hw_servo_io_0.write(command); hw_servo_previous_io_0 = command;
    }
  }
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(&hw_value_io_0, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0);
  if (isfinite((double)hw_value_io_0) && hw_value_io_0 >= 0 && hw_value_io_0 <= 180) {
    int command = (int)hw_value_io_0;
    if (command != hw_servo_previous_io_0) {
      hw_servo_io_0.write(command); hw_servo_previous_io_0 = command;
    }
  }
  if (hw_value_serial_0_trigger) { Serial.print("angle"); Serial.println(hw_value_serial_0_value_0); }
}
