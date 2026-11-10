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
extern bool sysml_state_prevVehicleApproach;
/* Global Variable define of  */
extern bool sysml_state_prevPedApproach;
/* Global Variable define of  */
extern bool sysml_state_prevDanger;
/* Global Variable define of  */
extern bool sysml_state_dangerActive;
/* Global Variable define of  */
extern int32_t sysml_state_dangerElapsedMs;
/* Global Variable define of  */
extern int32_t sysml_state_dangerMaxElapsedMs;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, double io_2, bool io_3, bool* io_4, bool* io_5, int32_t* io_6, int32_t* io_7, bool* io_8, bool* serial_0_trigger, int32_t* serial_0_value_0);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_prevVehicleApproach = false;
/* Global Variable define of  */
bool sysml_state_prevPedApproach = false;
/* Global Variable define of  */
bool sysml_state_prevDanger = false;
/* Global Variable define of  */
bool sysml_state_dangerActive = false;
/* Global Variable define of  */
int32_t sysml_state_dangerElapsedMs = 0;
/* Global Variable define of  */
int32_t sysml_state_dangerMaxElapsedMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_prevVehicleApproach = false;
    sysml_state_prevPedApproach = false;
    sysml_state_prevDanger = false;
    sysml_state_dangerActive = false;
    sysml_state_dangerElapsedMs = 0;
    sysml_state_dangerMaxElapsedMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, double io_2, bool io_3, bool* io_4, bool* io_5, int32_t* io_6, int32_t* io_7, bool* io_8, bool* serial_0_trigger, int32_t* serial_0_value_0) {
    static bool previous=false,candidate=false;
    static uint32_t entered=0,firstOverlap=0;
    uint32_t now=millis();
    bool overlap=io_1 && io_3 && io_0<50 && io_2<30;
    if (overlap && !candidate) firstOverlap=now;
    candidate=overlap;
    bool risk=overlap && (uint32_t)(now-firstOverlap)>=120;
    int level=risk ? (io_0<25 && io_2<15 ? 2 : 1) : 0;
    if (risk && !previous) entered=firstOverlap;
    *serial_0_trigger=!risk && previous;
    if (*serial_0_trigger) *serial_0_value_0=(int32_t)(now-entered);
    previous=risk;
    *io_4=risk && now%1000<500;
    *io_5=!risk;
    *io_6=risk ? 1000 : 0;
    *io_7=level;
    *io_8=false;
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
static bool hw_value_io_1 = false;
static double hw_value_io_2 = 0;
static bool hw_value_io_3 = false;
static bool hw_value_io_4 = false;
static bool hw_value_io_5 = false;
static int32_t hw_value_io_6 = 0;
static int32_t hw_tone_io_6 = -1;
static int32_t hw_value_io_7 = 0;
TM1637Display hw_display_io_7(4, 5);
static int32_t hw_display_previous_io_7 = INT32_MIN;
static bool hw_value_io_8 = false;
static bool hw_value_serial_0_trigger = false;
static int32_t hw_value_serial_0_value_0 = 0;
static uint32_t hw_ultrasonic_sampled_0 = 0;
static bool hw_ultrasonic_attempted_0 = false;
static uint32_t hw_ultrasonic_sampled_1 = 0;
static bool hw_ultrasonic_attempted_1 = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(12, OUTPUT);
  digitalWrite(12, hw_value_io_4 ? HIGH : LOW);
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_5 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  int32_t hw_tone_command_io_6 = (hw_value_io_6 >= 31 && hw_value_io_6 <= 65535) ? hw_value_io_6 : 0;
  if (hw_tone_command_io_6 != hw_tone_io_6) {
    if (hw_tone_command_io_6 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_6);
    hw_tone_io_6 = hw_tone_command_io_6;
  }
  hw_display_io_7.setBrightness(7, true);
  if (hw_value_io_7 != hw_display_previous_io_7) {
    if (hw_value_io_7 < -999 || hw_value_io_7 > 9999) hw_display_io_7.clear();
    else hw_display_io_7.showNumberDec((int)hw_value_io_7, false, 4, 0);
    hw_display_previous_io_7 = hw_value_io_7;
  }
  pinMode(2, OUTPUT);
  digitalWrite(2, hw_value_io_8 ? HIGH : LOW);
  pinMode(8, OUTPUT);
  digitalWrite(8, LOW);
  pinMode(9, INPUT);
  pinMode(10, OUTPUT);
  digitalWrite(10, LOW);
  pinMode(11, INPUT);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  uint32_t hw_ultrasonic_now_0 = (uint32_t)millis();
  if (!hw_ultrasonic_attempted_0 || (uint32_t)(hw_ultrasonic_now_0 - hw_ultrasonic_sampled_0) >= 60UL) {
    hw_ultrasonic_attempted_0 = true; hw_ultrasonic_sampled_0 = hw_ultrasonic_now_0;
    digitalWrite(8, LOW); delayMicroseconds(2);
    digitalWrite(8, HIGH); delayMicroseconds(10); digitalWrite(8, LOW);
    unsigned long hw_ultrasonic_pulse_0 = pulseIn(9, HIGH, 25000UL);
    hw_value_io_1 = (hw_ultrasonic_pulse_0 >= 116UL && hw_ultrasonic_pulse_0 <= 23200UL);
    if (hw_value_io_1) hw_value_io_0 = hw_ultrasonic_pulse_0 / 58.0;
  }
  uint32_t hw_ultrasonic_now_1 = (uint32_t)millis();
  if (!hw_ultrasonic_attempted_1 || (uint32_t)(hw_ultrasonic_now_1 - hw_ultrasonic_sampled_1) >= 60UL) {
    hw_ultrasonic_attempted_1 = true; hw_ultrasonic_sampled_1 = hw_ultrasonic_now_1;
    digitalWrite(10, LOW); delayMicroseconds(2);
    digitalWrite(10, HIGH); delayMicroseconds(10); digitalWrite(10, LOW);
    unsigned long hw_ultrasonic_pulse_1 = pulseIn(11, HIGH, 25000UL);
    hw_value_io_3 = (hw_ultrasonic_pulse_1 >= 116UL && hw_ultrasonic_pulse_1 <= 23200UL);
    if (hw_value_io_3) hw_value_io_2 = hw_ultrasonic_pulse_1 / 58.0;
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_io_7, &hw_value_io_8, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0);
  digitalWrite(12, hw_value_io_4 ? HIGH : LOW);
  digitalWrite(13, hw_value_io_5 ? HIGH : LOW);
  int32_t hw_tone_command_io_6 = (hw_value_io_6 >= 31 && hw_value_io_6 <= 65535) ? hw_value_io_6 : 0;
  if (hw_tone_command_io_6 != hw_tone_io_6) {
    if (hw_tone_command_io_6 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_6);
    hw_tone_io_6 = hw_tone_command_io_6;
  }
  if (hw_value_io_7 != hw_display_previous_io_7) {
    if (hw_value_io_7 < -999 || hw_value_io_7 > 9999) hw_display_io_7.clear();
    else hw_display_io_7.showNumberDec((int)hw_value_io_7, false, 4, 0);
    hw_display_previous_io_7 = hw_value_io_7;
  }
  digitalWrite(2, hw_value_io_8 ? HIGH : LOW);
  if (hw_value_serial_0_trigger) { Serial.print("danger_event_duration_ms"); Serial.println(hw_value_serial_0_value_0); }
}
