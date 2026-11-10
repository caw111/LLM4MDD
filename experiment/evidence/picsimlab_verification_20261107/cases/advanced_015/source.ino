#ifdef ARDUINO
#include <Arduino.h>
#include <string.h>
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
extern bool sysml_state_previousProximity;
/* Global Variable define of  */
extern bool sysml_state_valveOn;
/* Global Variable define of  */
extern int32_t sysml_state_valveRemainingMs;
/* Global Variable define of  */
extern int32_t sysml_state_todayUses;
/* Global Variable define of  */
extern bool sysml_state_alarmActive;
/* Global Variable define of  */
extern int32_t sysml_state_alarmRemainingMs;
/* Global Variable define of  */
extern bool sysml_state_lastDistanceValid;
/* Global Variable define of  */
extern int32_t sysml_state_lastDisplayedPercent;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, double io_2, bool io_3, bool* io_1, int32_t* io_4, int32_t* io_5, bool* io_6, bool* serial_0_trigger, int32_t* serial_0_value_0, double* serial_0_value_1);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousProximity = false;
/* Global Variable define of  */
bool sysml_state_valveOn = false;
/* Global Variable define of  */
int32_t sysml_state_valveRemainingMs = 0;
/* Global Variable define of  */
int32_t sysml_state_todayUses = 0;
/* Global Variable define of  */
bool sysml_state_alarmActive = false;
/* Global Variable define of  */
int32_t sysml_state_alarmRemainingMs = 0;
/* Global Variable define of  */
bool sysml_state_lastDistanceValid = false;
/* Global Variable define of  */
int32_t sysml_state_lastDisplayedPercent = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousProximity = false;
    sysml_state_valveOn = false;
    sysml_state_valveRemainingMs = 0;
    sysml_state_todayUses = 0;
    sysml_state_alarmActive = false;
    sysml_state_alarmRemainingMs = 0;
    sysml_state_lastDistanceValid = false;
    sysml_state_lastDisplayedPercent = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, double io_2, bool io_3, bool* io_1, int32_t* io_4, int32_t* io_5, bool* io_6, bool* serial_0_trigger, int32_t* serial_0_value_0, double* serial_0_value_1) {
    static bool previous=false,open=false,overflow=false;
    static uint32_t opened=0;
    static uint8_t used=0;
    static char command[7];
    static int uses=0;
    uint32_t now=millis();
    double percent=io_3 ? constrain((45-io_2)*2.5,0.0,100.0) : 0;
    bool low=!io_3 || percent<20;
    if (low || (uint32_t)(now-opened)>=3000) open=false;
    if (io_0 && !previous && !low && !open) {
        open=true;
        opened=now;
        if (uses<9999) ++uses;
    }
    previous=io_0;
    *serial_0_trigger=false;
    for (uint8_t budget=0;budget<32 && Serial.available();++budget) {
        char c=(char)Serial.read();
        if (c=='\r' || c=='\n') {
            command[used]='\0';
            if (!overflow && strcmp(command,"STATUS")==0) *serial_0_trigger=true;
            used=0;
            overflow=false;
        } else if (used<sizeof(command)-1) command[used++]=c;
        else overflow=true;
    }
    *io_1=open;
    *io_4=now/2000%2 ? uses : (int32_t)percent;
    *io_5=low && now%1000<400 ? 1000 : 0;
    *io_6=low;
    *serial_0_value_0=uses;
    *serial_0_value_1=percent;
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
static double hw_value_io_2 = 0;
static bool hw_value_io_3 = false;
static int32_t hw_value_io_4 = 0;
TM1637Display hw_display_io_4(4, 5);
static int32_t hw_display_previous_io_4 = INT32_MIN;
static int32_t hw_value_io_5 = 0;
static int32_t hw_tone_io_5 = -1;
static bool hw_value_io_6 = false;
static bool hw_value_serial_0_trigger = false;
static int32_t hw_value_serial_0_value_0 = 0;
static double hw_value_serial_0_value_1 = 0;
static uint32_t hw_ultrasonic_sampled_0 = 0;
static bool hw_ultrasonic_attempted_0 = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(2, INPUT);
  pinMode(10, OUTPUT);
  digitalWrite(10, hw_value_io_1 ? HIGH : LOW);
  hw_display_io_4.setBrightness(7, true);
  if (hw_value_io_4 != hw_display_previous_io_4) {
    if (hw_value_io_4 < -999 || hw_value_io_4 > 9999) hw_display_io_4.clear();
    else hw_display_io_4.showNumberDec((int)hw_value_io_4, false, 4, 0);
    hw_display_previous_io_4 = hw_value_io_4;
  }
  pinMode(6, OUTPUT);
  int32_t hw_tone_command_io_5 = (hw_value_io_5 >= 31 && hw_value_io_5 <= 65535) ? hw_value_io_5 : 0;
  if (hw_tone_command_io_5 != hw_tone_io_5) {
    if (hw_tone_command_io_5 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_5);
    hw_tone_io_5 = hw_tone_command_io_5;
  }
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_6 ? HIGH : LOW);
  pinMode(8, OUTPUT);
  digitalWrite(8, LOW);
  pinMode(9, INPUT);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 5UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (digitalRead(2) == HIGH);
  }
  uint32_t hw_ultrasonic_now_0 = (uint32_t)millis();
  if (!hw_ultrasonic_attempted_0 || (uint32_t)(hw_ultrasonic_now_0 - hw_ultrasonic_sampled_0) >= 60UL) {
    hw_ultrasonic_attempted_0 = true; hw_ultrasonic_sampled_0 = hw_ultrasonic_now_0;
    digitalWrite(8, LOW); delayMicroseconds(2);
    digitalWrite(8, HIGH); delayMicroseconds(10); digitalWrite(8, LOW);
    unsigned long hw_ultrasonic_pulse_0 = pulseIn(9, HIGH, 25000UL);
    hw_value_io_3 = (hw_ultrasonic_pulse_0 >= 116UL && hw_ultrasonic_pulse_0 <= 23200UL);
    if (hw_value_io_3) hw_value_io_2 = hw_ultrasonic_pulse_0 / 58.0;
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_2, hw_value_io_3, &hw_value_io_1, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0, &hw_value_serial_0_value_1);
  digitalWrite(10, hw_value_io_1 ? HIGH : LOW);
  if (hw_value_io_4 != hw_display_previous_io_4) {
    if (hw_value_io_4 < -999 || hw_value_io_4 > 9999) hw_display_io_4.clear();
    else hw_display_io_4.showNumberDec((int)hw_value_io_4, false, 4, 0);
    hw_display_previous_io_4 = hw_value_io_4;
  }
  int32_t hw_tone_command_io_5 = (hw_value_io_5 >= 31 && hw_value_io_5 <= 65535) ? hw_value_io_5 : 0;
  if (hw_tone_command_io_5 != hw_tone_io_5) {
    if (hw_tone_command_io_5 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_5);
    hw_tone_io_5 = hw_tone_command_io_5;
  }
  digitalWrite(13, hw_value_io_6 ? HIGH : LOW);
  if (hw_value_serial_0_trigger) { Serial.print("today_uses"); Serial.println(hw_value_serial_0_value_0); }
  if (hw_value_serial_0_trigger) { Serial.print("remaining_percent"); Serial.println(hw_value_serial_0_value_1); }
}
