#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_007\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
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
extern bool sysml_state_ledState;
/* Global Variable define of  */
extern int32_t sysml_state_pwmValue;
/* Global Variable define of  */
extern int32_t sysml_state_buzzerHz;
/* Global Variable define of  */
extern bool sysml_state_prevPressed;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_3, bool* io_0, double* io_1, int32_t* io_2, bool* serial_0_trigger, bool* serial_0_value_0, int32_t* serial_0_value_1, int32_t* serial_0_value_2);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_ledState = false;
/* Global Variable define of  */
int32_t sysml_state_pwmValue = 0;
/* Global Variable define of  */
int32_t sysml_state_buzzerHz = 0;
/* Global Variable define of  */
bool sysml_state_prevPressed = false;
/* Init function of CompositeActor named "ProgramPage1" */
#line 118 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_007\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void setup();
#line 135 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_007\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void loop();
#line 41 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_007\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_ledState = false;
    sysml_state_pwmValue = 0;
    sysml_state_buzzerHz = 0;
    sysml_state_prevPressed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_3, bool* io_0, double* io_1, int32_t* io_2, bool* serial_0_trigger, bool* serial_0_value_0, int32_t* serial_0_value_1, int32_t* serial_0_value_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    sysml_lazy_0 = io_3;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_prevPressed;
    }
    if (sysml_lazy_0) {
        sysml_state_prevPressed = true;
    }
    else {
        sysml_state_prevPressed = false;
    }
    sysml_lazy_1 = io_3;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_prevPressed;
    }
    if (!sysml_lazy_1) {
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_ledState;
    double sysml_value_1 = 0;
    sysml_value_1 = (sysml_state_pwmValue * 1.0);
    int32_t sysml_value_2 = 0;
    sysml_value_2 = sysml_state_buzzerHz;
    bool sysml_value_3 = 0;
    sysml_lazy_2 = io_3;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_prevPressed;
    }
    sysml_value_3 = sysml_lazy_2;
    bool sysml_value_4 = 0;
    sysml_value_4 = sysml_state_ledState;
    int32_t sysml_value_5 = 0;
    sysml_value_5 = sysml_state_pwmValue;
    int32_t sysml_value_6 = 0;
    sysml_value_6 = sysml_state_buzzerHz;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *serial_0_trigger = sysml_value_3;
    *serial_0_value_0 = sysml_value_4;
    *serial_0_value_1 = sysml_value_5;
    *serial_0_value_2 = sysml_value_6;
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
static double hw_value_io_1 = 0;
static int32_t hw_value_io_2 = 0;
static int32_t hw_tone_io_2 = -1;
static bool hw_value_io_3 = false;
static uint32_t hw_sampled_io_3 = 0;
static bool hw_attempted_io_3 = false;
static bool hw_value_serial_0_trigger = false;
static bool hw_value_serial_0_value_0 = false;
static int32_t hw_value_serial_0_value_1 = 0;
static int32_t hw_value_serial_0_value_2 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_0 ? HIGH : LOW);
  pinMode(9, OUTPUT);
  analogWrite(9, (int)constrain((hw_value_io_1 * 255 + 0), 0.0, 255.0));
  pinMode(6, OUTPUT);
  int32_t hw_tone_command_io_2 = (hw_value_io_2 >= 31 && hw_value_io_2 <= 65535) ? hw_value_io_2 : 0;
  if (hw_tone_command_io_2 != hw_tone_io_2) {
    if (hw_tone_command_io_2 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_2);
    hw_tone_io_2 = hw_tone_command_io_2;
  }
  pinMode(2, INPUT_PULLUP);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_3 || (uint32_t)(hw_now - hw_sampled_io_3) >= 5UL) {
    hw_attempted_io_3 = true; hw_sampled_io_3 = hw_now;
    hw_value_io_3 = (digitalRead(2) == HIGH);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_3, &hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0, &hw_value_serial_0_value_1, &hw_value_serial_0_value_2);
  digitalWrite(13, hw_value_io_0 ? HIGH : LOW);
  analogWrite(9, (int)constrain((hw_value_io_1 * 255 + 0), 0.0, 255.0));
  int32_t hw_tone_command_io_2 = (hw_value_io_2 >= 31 && hw_value_io_2 <= 65535) ? hw_value_io_2 : 0;
  if (hw_tone_command_io_2 != hw_tone_io_2) {
    if (hw_tone_command_io_2 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_2);
    hw_tone_io_2 = hw_tone_command_io_2;
  }
  if (hw_value_serial_0_trigger) { Serial.print("LED"); Serial.println(hw_value_serial_0_value_0); }
  if (hw_value_serial_0_trigger) { Serial.print("PWM"); Serial.println(hw_value_serial_0_value_1); }
  if (hw_value_serial_0_trigger) { Serial.print("BUZZ"); Serial.println(hw_value_serial_0_value_2); }
}

