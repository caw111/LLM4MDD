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
extern bool sysml_state_armed;
/* Global Variable define of  */
extern bool sysml_state_nightAuto;
/* Global Variable define of  */
extern int32_t sysml_state_eventCount;
/* Global Variable define of  */
extern bool sysml_state_previousDoorOpened;
/* Global Variable define of  */
extern bool sysml_state_previousPIR;
/* Global Variable define of  */
extern bool sysml_state_serialCommandActive;
/* Global Variable define of  */
extern int32_t sysml_state_pendingReportCmd;
/* Global Variable define of  */
extern bool sysml_state_doorRising;
/* Global Variable define of  */
extern bool sysml_state_pirRising;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double io_2, int32_t* io_3, bool* io_4, int32_t* io_5, bool* serial_0_trigger, int32_t* serial_0_value_0);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_armed = false;
/* Global Variable define of  */
bool sysml_state_nightAuto = false;
/* Global Variable define of  */
int32_t sysml_state_eventCount = 0;
/* Global Variable define of  */
bool sysml_state_previousDoorOpened = false;
/* Global Variable define of  */
bool sysml_state_previousPIR = false;
/* Global Variable define of  */
bool sysml_state_serialCommandActive = false;
/* Global Variable define of  */
int32_t sysml_state_pendingReportCmd = 0;
/* Global Variable define of  */
bool sysml_state_doorRising = false;
/* Global Variable define of  */
bool sysml_state_pirRising = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_armed = false;
    sysml_state_nightAuto = false;
    sysml_state_eventCount = 0;
    sysml_state_previousDoorOpened = false;
    sysml_state_previousPIR = false;
    sysml_state_serialCommandActive = false;
    sysml_state_pendingReportCmd = 0;
    sysml_state_doorRising = false;
    sysml_state_pirRising = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double io_2, int32_t* io_3, bool* io_4, int32_t* io_5, bool* serial_0_trigger, int32_t* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    int32_t sysml_conditional_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_lazy_10 = 0;
    bool sysml_lazy_11 = 0;
    bool sysml_conditional_12 = 0;
    int32_t sysml_conditional_13 = 0;
    if ((sysml_state_pendingReportCmd == 1)) {
        sysml_state_armed = true;
    }
    if ((sysml_state_pendingReportCmd == 2)) {
        sysml_state_armed = false;
    }
    if ((sysml_state_pendingReportCmd == 3)) {
        sysml_state_armed = false;
        sysml_state_eventCount = 0;
        sysml_state_nightAuto = false;
    }
    if ((sysml_state_pendingReportCmd == 4)) {
        sysml_state_serialCommandActive = true;
    }
    if ((sysml_state_serialCommandActive == true)) {
        sysml_state_serialCommandActive = false;
    }
    sysml_lazy_0 = io_1;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousDoorOpened;
    }
    sysml_state_doorRising = sysml_lazy_0;
    sysml_lazy_1 = io_0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_previousPIR;
    }
    sysml_state_pirRising = sysml_lazy_1;
    sysml_state_nightAuto = (io_2 < 512.0);
    sysml_lazy_2 = sysml_state_armed;
    if (!sysml_lazy_2) {
        sysml_lazy_2 = sysml_state_nightAuto;
    }
    sysml_lazy_3 = sysml_lazy_2;
    if (sysml_lazy_3) {
        sysml_lazy_3 = sysml_state_doorRising;
    }
    sysml_lazy_4 = sysml_lazy_3;
    if (sysml_lazy_4) {
        sysml_lazy_4 = sysml_state_pirRising;
    }
    if (sysml_lazy_4) {
        sysml_state_eventCount = (sysml_state_eventCount + 1);
    }
    sysml_state_previousDoorOpened = io_1;
    sysml_state_previousPIR = io_0;
    int32_t sysml_value_0 = 0;
    sysml_lazy_5 = sysml_state_armed;
    if (!sysml_lazy_5) {
        sysml_lazy_5 = sysml_state_nightAuto;
    }
    sysml_lazy_6 = sysml_lazy_5;
    if (sysml_lazy_6) {
        sysml_lazy_6 = sysml_state_doorRising;
    }
    sysml_lazy_7 = sysml_lazy_6;
    if (sysml_lazy_7) {
        sysml_lazy_7 = sysml_state_pirRising;
    }
    if (sysml_lazy_7) {
        sysml_conditional_8 = 2000;
    }
    else {
        sysml_conditional_8 = 0;
    }
    sysml_value_0 = sysml_conditional_8;
    bool sysml_value_1 = 0;
    sysml_lazy_9 = sysml_state_armed;
    if (!sysml_lazy_9) {
        sysml_lazy_9 = sysml_state_nightAuto;
    }
    sysml_lazy_10 = sysml_lazy_9;
    if (sysml_lazy_10) {
        sysml_lazy_10 = sysml_state_doorRising;
    }
    sysml_lazy_11 = sysml_lazy_10;
    if (sysml_lazy_11) {
        sysml_lazy_11 = sysml_state_pirRising;
    }
    sysml_value_1 = sysml_lazy_11;
    int32_t sysml_value_2 = 0;
    sysml_value_2 = sysml_state_eventCount;
    bool sysml_value_3 = 0;
    if (sysml_state_pendingReportCmd == 4) {
        sysml_conditional_12 = true;
    }
    else {
        sysml_conditional_12 = false;
    }
    sysml_value_3 = sysml_conditional_12;
    int32_t sysml_value_4 = 0;
    if (sysml_state_pendingReportCmd == 4) {
        sysml_conditional_13 = sysml_state_eventCount;
    }
    else {
        sysml_conditional_13 = 0;
    }
    sysml_value_4 = sysml_conditional_13;
    *io_3 = sysml_value_0;
    *io_4 = sysml_value_1;
    *io_5 = sysml_value_2;
    *serial_0_trigger = sysml_value_3;
    *serial_0_value_0 = sysml_value_4;
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
static double hw_value_io_2 = 0;
static uint32_t hw_sampled_io_2 = 0;
static bool hw_attempted_io_2 = false;
static int32_t hw_value_io_3 = 0;
static int32_t hw_tone_io_3 = -1;
static bool hw_value_io_4 = false;
static int32_t hw_value_io_5 = 0;
TM1637Display hw_display_io_5(4, 5);
static int32_t hw_display_previous_io_5 = INT32_MIN;
static bool hw_value_serial_0_trigger = false;
static int32_t hw_value_serial_0_value_0 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(A0, INPUT);
  pinMode(6, OUTPUT);
  int32_t hw_tone_command_io_3 = (hw_value_io_3 >= 31 && hw_value_io_3 <= 65535) ? hw_value_io_3 : 0;
  if (hw_tone_command_io_3 != hw_tone_io_3) {
    if (hw_tone_command_io_3 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_3);
    hw_tone_io_3 = hw_tone_command_io_3;
  }
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_4 ? HIGH : LOW);
  hw_display_io_5.setBrightness(7, true);
  if (hw_value_io_5 != hw_display_previous_io_5) {
    if (hw_value_io_5 < -999 || hw_value_io_5 > 9999) hw_display_io_5.clear();
    else hw_display_io_5.showNumberDec((int)hw_value_io_5, false, 4, 0);
    hw_display_previous_io_5 = hw_value_io_5;
  }
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 10UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (digitalRead(2) == HIGH);
  }
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 10UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = (digitalRead(3) == HIGH);
  }
  if (!hw_attempted_io_2 || (uint32_t)(hw_now - hw_sampled_io_2) >= 10UL) {
    hw_attempted_io_2 = true; hw_sampled_io_2 = hw_now;
    hw_value_io_2 = (analogRead(A0) * 1.0 + 0.0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0);
  int32_t hw_tone_command_io_3 = (hw_value_io_3 >= 31 && hw_value_io_3 <= 65535) ? hw_value_io_3 : 0;
  if (hw_tone_command_io_3 != hw_tone_io_3) {
    if (hw_tone_command_io_3 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_3);
    hw_tone_io_3 = hw_tone_command_io_3;
  }
  digitalWrite(13, hw_value_io_4 ? HIGH : LOW);
  if (hw_value_io_5 != hw_display_previous_io_5) {
    if (hw_value_io_5 < -999 || hw_value_io_5 > 9999) hw_display_io_5.clear();
    else hw_display_io_5.showNumberDec((int)hw_value_io_5, false, 4, 0);
    hw_display_previous_io_5 = hw_value_io_5;
  }
  if (hw_value_serial_0_trigger) { Serial.print("cmd"); Serial.println(hw_value_serial_0_value_0); }
}
