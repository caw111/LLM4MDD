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
extern bool sysml_state_previousCounterInput;
/* Global Variable define of  */
extern int32_t sysml_state_accumulatedNoProductMs;
/* Global Variable define of  */
extern int32_t sysml_state_cumulativeCount;
/* Global Variable define of  */
extern int32_t sysml_state_stopCount;
/* Global Variable define of  */
extern bool sysml_state_alarmActive;
/* Global Variable define of  */
extern bool sysml_state_previousStopButton;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, int32_t io_6, bool* io_3, int32_t* io_4, int32_t* io_5, bool* serial_0_trigger, int32_t* serial_0_value_0, double* serial_0_value_1, int32_t* serial_0_value_2, bool* serial_0_value_3);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousCounterInput = false;
/* Global Variable define of  */
int32_t sysml_state_accumulatedNoProductMs = 0;
/* Global Variable define of  */
int32_t sysml_state_cumulativeCount = 0;
/* Global Variable define of  */
int32_t sysml_state_stopCount = 0;
/* Global Variable define of  */
bool sysml_state_alarmActive = false;
/* Global Variable define of  */
bool sysml_state_previousStopButton = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousCounterInput = false;
    sysml_state_accumulatedNoProductMs = 0;
    sysml_state_cumulativeCount = 0;
    sysml_state_stopCount = 0;
    sysml_state_alarmActive = false;
    sysml_state_previousStopButton = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, int32_t io_6, bool* io_3, int32_t* io_4, int32_t* io_5, bool* serial_0_trigger, int32_t* serial_0_value_0, double* serial_0_value_1, int32_t* serial_0_value_2, bool* serial_0_value_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    int32_t sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    int32_t sysml_conditional_6 = 0;
    int32_t sysml_conditional_7 = 0;
    bool sysml_lazy_8 = 0;
    double sysml_conditional_9 = 0;
    sysml_state_previousCounterInput = io_0;
    sysml_lazy_0 = io_1;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousStopButton;
    }
    if (sysml_lazy_0) {
        sysml_state_stopCount = (sysml_state_stopCount + 1);
    }
    sysml_state_previousStopButton = io_1;
    sysml_lazy_1 = sysml_state_alarmActive;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !io_1;
    }
    if (sysml_lazy_1) {
        sysml_state_alarmActive = false;
    }
    sysml_lazy_2 = io_1;
    if (!sysml_lazy_2) {
        sysml_lazy_2 = sysml_state_previousCounterInput;
    }
    if (sysml_lazy_2) {
        sysml_conditional_3 = 0;
    }
    else {
        sysml_conditional_3 = sysml_state_accumulatedNoProductMs + io_6;
    }
    sysml_state_accumulatedNoProductMs = sysml_conditional_3;
    sysml_lazy_4 = io_1;
    if (!sysml_lazy_4) {
        sysml_lazy_4 = sysml_state_accumulatedNoProductMs >= 30000;
    }
    if (sysml_lazy_4) {
        sysml_state_alarmActive = true;
    }
    sysml_lazy_5 = io_0;
    if (sysml_lazy_5) {
        sysml_lazy_5 = !sysml_state_previousCounterInput;
    }
    if (sysml_lazy_5) {
        sysml_state_cumulativeCount = (sysml_state_cumulativeCount + 1);
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_alarmActive;
    int32_t sysml_value_1 = 0;
    if (sysml_state_alarmActive) {
        sysml_conditional_6 = 2000;
    }
    else {
        sysml_conditional_6 = 0;
    }
    sysml_value_1 = sysml_conditional_6;
    int32_t sysml_value_2 = 0;
    if (sysml_state_alarmActive) {
        sysml_conditional_7 = 0;
    }
    else {
        sysml_conditional_7 = 0;
    }
    sysml_value_2 = sysml_conditional_7;
    bool sysml_value_3 = 0;
    sysml_value_3 = sysml_state_alarmActive;
    int32_t sysml_value_4 = 0;
    sysml_value_4 = sysml_state_cumulativeCount;
    double sysml_value_5 = 0;
    sysml_lazy_8 = 0 != 0;
    if (sysml_lazy_8) {
        sysml_lazy_8 = true;
    }
    if (sysml_lazy_8) {
        sysml_conditional_9 = 0.0;
    }
    else {
        sysml_conditional_9 = 0.0;
    }
    sysml_value_5 = sysml_conditional_9;
    int32_t sysml_value_6 = 0;
    sysml_value_6 = sysml_state_stopCount;
    bool sysml_value_7 = 0;
    sysml_value_7 = sysml_state_alarmActive;
    *io_3 = sysml_value_0;
    *io_4 = sysml_value_1;
    *io_5 = sysml_value_2;
    *serial_0_trigger = sysml_value_3;
    *serial_0_value_0 = sysml_value_4;
    *serial_0_value_1 = sysml_value_5;
    *serial_0_value_2 = sysml_value_6;
    *serial_0_value_3 = sysml_value_7;
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
static uint32_t hw_sampled_io_2 = 0;
static bool hw_attempted_io_2 = false;
static bool hw_value_io_3 = false;
static int32_t hw_value_io_4 = 0;
static int32_t hw_tone_io_4 = -1;
static int32_t hw_value_io_5 = 0;
TM1637Display hw_display_io_5(4, 5);
static int32_t hw_display_previous_io_5 = INT32_MIN;
static int32_t hw_value_io_6 = 0;
static bool hw_value_serial_0_trigger = false;
static int32_t hw_value_serial_0_value_0 = 0;
static double hw_value_serial_0_value_1 = 0;
static int32_t hw_value_serial_0_value_2 = 0;
static bool hw_value_serial_0_value_3 = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(7, INPUT);
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_3 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  int32_t hw_tone_command_io_4 = (hw_value_io_4 >= 31 && hw_value_io_4 <= 65535) ? hw_value_io_4 : 0;
  if (hw_tone_command_io_4 != hw_tone_io_4) {
    if (hw_tone_command_io_4 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_4);
    hw_tone_io_4 = hw_tone_command_io_4;
  }
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
    hw_value_io_2 = (digitalRead(7) == HIGH);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_6 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, hw_value_io_6, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0, &hw_value_serial_0_value_1, &hw_value_serial_0_value_2, &hw_value_serial_0_value_3);
  digitalWrite(13, hw_value_io_3 ? HIGH : LOW);
  int32_t hw_tone_command_io_4 = (hw_value_io_4 >= 31 && hw_value_io_4 <= 65535) ? hw_value_io_4 : 0;
  if (hw_tone_command_io_4 != hw_tone_io_4) {
    if (hw_tone_command_io_4 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_4);
    hw_tone_io_4 = hw_tone_command_io_4;
  }
  if (hw_value_io_5 != hw_display_previous_io_5) {
    if (hw_value_io_5 < -999 || hw_value_io_5 > 9999) hw_display_io_5.clear();
    else hw_display_io_5.showNumberDec((int)hw_value_io_5, false, 4, 0);
    hw_display_previous_io_5 = hw_value_io_5;
  }
  if (hw_value_serial_0_trigger) { Serial.print("cumulative_count"); Serial.println(hw_value_serial_0_value_0); }
  if (hw_value_serial_0_trigger) { Serial.print("achievement_rate_percent"); Serial.println(hw_value_serial_0_value_1); }
  if (hw_value_serial_0_trigger) { Serial.print("stop_count"); Serial.println(hw_value_serial_0_value_2); }
  if (hw_value_serial_0_trigger) { Serial.print("alarm_active"); Serial.println(hw_value_serial_0_value_3); }
}
