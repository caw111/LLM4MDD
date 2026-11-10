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
extern int32_t sysml_state_doorOpenMs;
/* Global Variable define of  */
extern bool sysml_state_prevDoorOpen;
/* Global Variable define of  */
extern int32_t sysml_state_doorOpenSeconds;
/* Global Variable define of  */
extern bool sysml_state_fanCoolRelayOn;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, bool* io_2, int32_t* io_3, int32_t* io_4, bool* io_5, bool* serial_0_trigger, int32_t* serial_0_value_0, bool* serial_1_trigger, double* serial_1_value_0, int32_t* serial_1_value_1);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_doorOpenMs = 0;
/* Global Variable define of  */
bool sysml_state_prevDoorOpen = false;
/* Global Variable define of  */
int32_t sysml_state_doorOpenSeconds = 0;
/* Global Variable define of  */
bool sysml_state_fanCoolRelayOn = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_doorOpenMs = 0;
    sysml_state_prevDoorOpen = false;
    sysml_state_doorOpenSeconds = 0;
    sysml_state_fanCoolRelayOn = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, bool* io_2, int32_t* io_3, int32_t* io_4, bool* io_5, bool* serial_0_trigger, int32_t* serial_0_value_0, bool* serial_1_trigger, double* serial_1_value_0, int32_t* serial_1_value_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int32_t sysml_conditional_0 = 0;
    int32_t sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    int32_t sysml_conditional_5 = 0;
    int32_t sysml_conditional_6 = 0;
    int32_t sysml_conditional_7 = 0;
    int32_t sysml_conditional_8 = 0;
    bool sysml_conditional_9 = 0;
    bool sysml_lazy_10 = 0;
    bool sysml_lazy_11 = 0;
    if (io_1) {
        sysml_state_doorOpenMs = (sysml_state_doorOpenMs + 10);
    }
    else {
        sysml_state_doorOpenMs = 0;
    }
    if (sysml_state_doorOpenMs >= 0) {
        if ((((double)sysml_state_doorOpenMs) / 1000.0) < ((double)((int32_t)(((double)sysml_state_doorOpenMs) / 1000.0)))) {
            sysml_conditional_1 = 1;
        }
        else {
            sysml_conditional_1 = 0;
        }
        sysml_conditional_0 = ((int32_t)(((double)sysml_state_doorOpenMs) / 1000.0)) - sysml_conditional_1;
    }
    else {
        sysml_conditional_0 = 0;
    }
    sysml_state_doorOpenSeconds = sysml_conditional_0;
    sysml_state_fanCoolRelayOn = sysml_state_fanCoolRelayOn;
    sysml_lazy_2 = io_0 >= 300.0;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !io_1;
    }
    if (sysml_lazy_2) {
        sysml_state_fanCoolRelayOn = true;
    }
    sysml_lazy_3 = io_0 <= 280.0;
    if (sysml_lazy_3) {
        sysml_lazy_3 = !io_1;
    }
    if (sysml_lazy_3) {
        sysml_state_fanCoolRelayOn = false;
    }
    sysml_lazy_4 = io_1;
    if (sysml_lazy_4) {
        sysml_lazy_4 = sysml_state_fanCoolRelayOn;
    }
    if (sysml_lazy_4) {
        sysml_state_fanCoolRelayOn = false;
    }
    sysml_state_prevDoorOpen = io_1;
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_fanCoolRelayOn;
    int32_t sysml_value_1 = 0;
    if (sysml_state_doorOpenMs > 10000) {
        sysml_conditional_5 = 2500;
    }
    else {
        sysml_conditional_5 = 0;
    }
    sysml_value_1 = sysml_conditional_5;
    int32_t sysml_value_2 = 0;
    if (io_1) {
        if (io_0 < ((double)((int32_t)io_0))) {
            sysml_conditional_7 = 1;
        }
        else {
            sysml_conditional_7 = 0;
        }
        sysml_conditional_6 = ((int32_t)io_0) - sysml_conditional_7;
    }
    else {
        if (io_0 < ((double)((int32_t)io_0))) {
            sysml_conditional_8 = 1;
        }
        else {
            sysml_conditional_8 = 0;
        }
        sysml_conditional_6 = ((int32_t)io_0) - sysml_conditional_8;
    }
    sysml_value_2 = sysml_conditional_6;
    bool sysml_value_3 = 0;
    if (sysml_state_doorOpenMs > 10000) {
        sysml_conditional_9 = true;
    }
    else {
        sysml_conditional_9 = false;
    }
    sysml_value_3 = sysml_conditional_9;
    bool sysml_value_4 = 0;
    sysml_lazy_10 = io_0 > 30.0;
    if (sysml_lazy_10) {
        sysml_lazy_10 = !io_1;
    }
    sysml_value_4 = sysml_lazy_10;
    int32_t sysml_value_5 = 0;
    sysml_value_5 = 1;
    bool sysml_value_6 = 0;
    sysml_lazy_11 = io_0 > 30.0;
    if (sysml_lazy_11) {
        sysml_lazy_11 = !io_1;
    }
    sysml_value_6 = !sysml_lazy_11;
    double sysml_value_7 = 0;
    sysml_value_7 = io_0;
    int32_t sysml_value_8 = 0;
    sysml_value_8 = sysml_state_doorOpenSeconds;
    *io_2 = sysml_value_0;
    *io_3 = sysml_value_1;
    *io_4 = sysml_value_2;
    *io_5 = sysml_value_3;
    *serial_0_trigger = sysml_value_4;
    *serial_0_value_0 = sysml_value_5;
    *serial_1_trigger = sysml_value_6;
    *serial_1_value_0 = sysml_value_7;
    *serial_1_value_1 = sysml_value_8;
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
static bool hw_value_io_2 = false;
static int32_t hw_value_io_3 = 0;
static int32_t hw_tone_io_3 = -1;
static int32_t hw_value_io_4 = 0;
TM1637Display hw_display_io_4(4, 5);
static int32_t hw_display_previous_io_4 = INT32_MIN;
static bool hw_value_io_5 = false;
static bool hw_value_serial_0_trigger = false;
static int32_t hw_value_serial_0_value_0 = 0;
static bool hw_value_serial_1_trigger = false;
static double hw_value_serial_1_value_0 = 0;
static int32_t hw_value_serial_1_value_1 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(A1, INPUT);
  pinMode(2, INPUT);
  pinMode(10, OUTPUT);
  digitalWrite(10, hw_value_io_2 ? HIGH : LOW);
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
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_5 ? HIGH : LOW);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 10UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (analogRead(A1) * 1.0 + 0.0);
  }
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 10UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = (digitalRead(2) == HIGH);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0, &hw_value_serial_1_trigger, &hw_value_serial_1_value_0, &hw_value_serial_1_value_1);
  digitalWrite(10, hw_value_io_2 ? HIGH : LOW);
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
  digitalWrite(13, hw_value_io_5 ? HIGH : LOW);
  if (hw_value_serial_0_trigger) { Serial.print("critical_cooling_loss"); Serial.println(hw_value_serial_0_value_0); }
  if (hw_value_serial_1_trigger) { Serial.print("temperature_degC"); Serial.println(hw_value_serial_1_value_0); }
  if (hw_value_serial_1_trigger) { Serial.print("door_open_seconds"); Serial.println(hw_value_serial_1_value_1); }
}
