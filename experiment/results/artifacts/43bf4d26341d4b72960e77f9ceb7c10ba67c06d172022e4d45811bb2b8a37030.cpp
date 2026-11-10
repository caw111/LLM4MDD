#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision10\\cases\\advanced_024\\arduino\\sketch\\sketch.ino"
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
extern bool sysml_state_prevIo7Pressed;
/* Global Variable define of  */
extern int32_t sysml_state_countReusable;
/* Global Variable define of  */
extern int32_t sysml_state_countOther;
/* Global Variable define of  */
extern bool sysml_state_alarmLatched;
/* Global Variable define of  */
extern bool sysml_state_prevFullState;
/* Global Variable define of  */
extern double sysml_state_lastValidDistance;
/* Global Variable define of  */
extern bool sysml_state_hasEverValidDistance;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double io_2, bool io_3, bool io_7, int32_t* io_4, int32_t* io_5, bool* io_6, bool* serial_0_trigger, bool* serial_0_value_0);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_prevIo7Pressed = false;
/* Global Variable define of  */
int32_t sysml_state_countReusable = 0;
/* Global Variable define of  */
int32_t sysml_state_countOther = 0;
/* Global Variable define of  */
bool sysml_state_alarmLatched = false;
/* Global Variable define of  */
bool sysml_state_prevFullState = false;
/* Global Variable define of  */
double sysml_state_lastValidDistance = 0.0;
/* Global Variable define of  */
bool sysml_state_hasEverValidDistance = false;
/* Init function of CompositeActor named "ProgramPage1" */
#line 187 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision10\\cases\\advanced_024\\arduino\\sketch\\sketch.ino"
void setup();
#line 213 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision10\\cases\\advanced_024\\arduino\\sketch\\sketch.ino"
void loop();
#line 53 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision10\\cases\\advanced_024\\arduino\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_prevIo7Pressed = false;
    sysml_state_countReusable = 0;
    sysml_state_countOther = 0;
    sysml_state_alarmLatched = false;
    sysml_state_prevFullState = false;
    sysml_state_lastValidDistance = 0.0;
    sysml_state_hasEverValidDistance = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double io_2, bool io_3, bool io_7, int32_t* io_4, int32_t* io_5, bool* io_6, bool* serial_0_trigger, bool* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_conditional_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    int32_t sysml_conditional_8 = 0;
    if (io_3) {
        sysml_state_lastValidDistance = io_2;
        sysml_state_hasEverValidDistance = true;
    }

    if (sysml_state_hasEverValidDistance) {
        sysml_conditional_0 = sysml_state_lastValidDistance >= 20.0;
    }
    else {
        sysml_conditional_0 = false;
    }
    sysml_state_prevFullState = sysml_conditional_0;
    sysml_lazy_1 = !sysml_state_prevIo7Pressed;
    if (sysml_lazy_1) {
        sysml_lazy_1 = io_7;
    }
    if (sysml_lazy_1) {
        sysml_state_countReusable = 0;
        sysml_state_countOther = 0;
        sysml_state_alarmLatched = false;
        sysml_state_prevFullState = false;
    }
    sysml_lazy_2 = !sysml_state_alarmLatched;
    if (sysml_lazy_2) {
        sysml_lazy_2 = sysml_state_hasEverValidDistance;
    }
    sysml_lazy_3 = sysml_lazy_2;
    if (sysml_lazy_3) {
        sysml_lazy_3 = sysml_state_lastValidDistance >= 20.0;
    }
    if (sysml_lazy_3) {
        sysml_state_alarmLatched = true;
    }
    sysml_lazy_4 = !sysml_state_alarmLatched;
    if (sysml_lazy_4) {
        sysml_lazy_4 = io_0;
    }
    sysml_lazy_5 = sysml_lazy_4;
    if (sysml_lazy_5) {
        sysml_lazy_5 = !(sysml_state_lastValidDistance >= 20.0);
    }
    if (sysml_lazy_5) {
        sysml_state_countReusable = (sysml_state_countReusable + 1);
    }
    sysml_lazy_6 = !sysml_state_alarmLatched;
    if (sysml_lazy_6) {
        sysml_lazy_6 = io_1;
    }
    sysml_lazy_7 = sysml_lazy_6;
    if (sysml_lazy_7) {
        sysml_lazy_7 = !(sysml_state_lastValidDistance >= 20.0);
    }
    if (sysml_lazy_7) {
        sysml_state_countOther = (sysml_state_countOther + 1);
    }
    int32_t sysml_value_0 = 0;
    sysml_value_0 = (sysml_state_countReusable + sysml_state_countOther);
    int32_t sysml_value_1 = 0;
    if (sysml_state_alarmLatched) {
        sysml_conditional_8 = 2000;
    }
    else {
        sysml_conditional_8 = 0;
    }
    sysml_value_1 = sysml_conditional_8;
    bool sysml_value_2 = 0;
    sysml_value_2 = sysml_state_alarmLatched;
    bool sysml_value_3 = 0;
    sysml_value_3 = sysml_state_alarmLatched;
    bool sysml_value_4 = 0;
    sysml_value_4 = sysml_state_alarmLatched;
    *io_4 = sysml_value_0;
    *io_5 = sysml_value_1;
    *io_6 = sysml_value_2;
    *serial_0_trigger = sysml_value_3;
    *serial_0_value_0 = sysml_value_4;

    sysml_state_prevIo7Pressed = io_7;
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
static bool hw_value_io_3 = false;
static int32_t hw_value_io_4 = 0;
TM1637Display hw_display_io_4(4, 5);
static int32_t hw_display_previous_io_4 = INT32_MIN;
static int32_t hw_value_io_5 = 0;
static int32_t hw_tone_io_5 = -1;
static bool hw_value_io_6 = false;
static bool hw_value_io_7 = false;
static uint32_t hw_sampled_io_7 = 0;
static bool hw_attempted_io_7 = false;
static bool hw_value_serial_0_trigger = false;
static bool hw_value_serial_0_value_0 = false;
static uint32_t hw_ultrasonic_sampled_0 = 0;
static bool hw_ultrasonic_attempted_0 = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
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
  pinMode(7, INPUT_PULLUP);
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
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 5UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = (digitalRead(3) == HIGH);
  }
  if (!hw_attempted_io_7 || (uint32_t)(hw_now - hw_sampled_io_7) >= 5UL) {
    hw_attempted_io_7 = true; hw_sampled_io_7 = hw_now;
    hw_value_io_7 = (digitalRead(7) == LOW);
  }
  uint32_t hw_ultrasonic_now_0 = (uint32_t)millis();
  if (!hw_ultrasonic_attempted_0 || (uint32_t)(hw_ultrasonic_now_0 - hw_ultrasonic_sampled_0) >= 250UL) {
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
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, hw_value_io_3, hw_value_io_7, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0);
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
  if (hw_value_serial_0_trigger) { Serial.print("empty bin needed"); Serial.println(hw_value_serial_0_value_0); }
}

