#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_multiplexer_potentiometer\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
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
extern int32_t sysml_state_framePhase;
/* Global Variable define of  */
extern int32_t sysml_state_pot0;
/* Global Variable define of  */
extern int32_t sysml_state_pot1;
/* Global Variable define of  */
extern int32_t sysml_state_pot2;
/* Global Variable define of  */
extern int32_t sysml_state_pot3;
/* Global Variable define of  */
extern int32_t sysml_state_pot4;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_4, bool* io_0, bool* io_1, bool* io_2, bool* io_3, bool* serial_0_trigger, int32_t* serial_0_value_0, int32_t* serial_0_value_1, int32_t* serial_0_value_2, int32_t* serial_0_value_3, int32_t* serial_0_value_4);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_framePhase = 0;
/* Global Variable define of  */
int32_t sysml_state_pot0 = 0;
/* Global Variable define of  */
int32_t sysml_state_pot1 = 0;
/* Global Variable define of  */
int32_t sysml_state_pot2 = 0;
/* Global Variable define of  */
int32_t sysml_state_pot3 = 0;
/* Global Variable define of  */
int32_t sysml_state_pot4 = 0;
/* Init function of CompositeActor named "ProgramPage1" */
#line 191 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_multiplexer_potentiometer\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 205 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_multiplexer_potentiometer\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 49 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_multiplexer_potentiometer\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_framePhase = 0;
    sysml_state_pot0 = 0;
    sysml_state_pot1 = 0;
    sysml_state_pot2 = 0;
    sysml_state_pot3 = 0;
    sysml_state_pot4 = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_4, bool* io_0, bool* io_1, bool* io_2, bool* io_3, bool* serial_0_trigger, int32_t* serial_0_value_0, int32_t* serial_0_value_1, int32_t* serial_0_value_2, int32_t* serial_0_value_3, int32_t* serial_0_value_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int32_t sysml_conditional_0 = 0;
    int32_t sysml_conditional_1 = 0;
    int32_t sysml_conditional_2 = 0;
    int32_t sysml_conditional_3 = 0;
    int32_t sysml_conditional_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    if ((sysml_state_framePhase == 0)) {
        if (io_4 < ((double)((int32_t)io_4))) {
            sysml_conditional_0 = 1;
        }
        else {
            sysml_conditional_0 = 0;
        }
        sysml_state_pot0 = ((int32_t)io_4) - sysml_conditional_0;
        sysml_state_framePhase = 1;
    }
    if ((sysml_state_framePhase == 1)) {
        if (io_4 < ((double)((int32_t)io_4))) {
            sysml_conditional_1 = 1;
        }
        else {
            sysml_conditional_1 = 0;
        }
        sysml_state_pot1 = ((int32_t)io_4) - sysml_conditional_1;
        sysml_state_framePhase = 2;
    }
    if ((sysml_state_framePhase == 2)) {
        if (io_4 < ((double)((int32_t)io_4))) {
            sysml_conditional_2 = 1;
        }
        else {
            sysml_conditional_2 = 0;
        }
        sysml_state_pot2 = ((int32_t)io_4) - sysml_conditional_2;
        sysml_state_framePhase = 3;
    }
    if ((sysml_state_framePhase == 3)) {
        if (io_4 < ((double)((int32_t)io_4))) {
            sysml_conditional_3 = 1;
        }
        else {
            sysml_conditional_3 = 0;
        }
        sysml_state_pot3 = ((int32_t)io_4) - sysml_conditional_3;
        sysml_state_framePhase = 4;
    }
    if ((sysml_state_framePhase == 4)) {
        if (io_4 < ((double)((int32_t)io_4))) {
            sysml_conditional_4 = 1;
        }
        else {
            sysml_conditional_4 = 0;
        }
        sysml_state_pot4 = ((int32_t)io_4) - sysml_conditional_4;
        sysml_state_framePhase = 0;
    }
    bool sysml_value_0 = 0;
    sysml_lazy_5 = sysml_state_framePhase == 0;
    if (!sysml_lazy_5) {
        sysml_lazy_5 = sysml_state_framePhase == 2;
    }
    sysml_lazy_6 = sysml_lazy_5;
    if (!sysml_lazy_6) {
        sysml_lazy_6 = sysml_state_framePhase == 4;
    }
    sysml_value_0 = sysml_lazy_6;
    bool sysml_value_1 = 0;
    sysml_lazy_7 = sysml_state_framePhase == 1;
    if (!sysml_lazy_7) {
        sysml_lazy_7 = sysml_state_framePhase == 2;
    }
    sysml_value_1 = sysml_lazy_7;
    bool sysml_value_2 = 0;
    sysml_lazy_8 = sysml_state_framePhase == 3;
    if (!sysml_lazy_8) {
        sysml_lazy_8 = sysml_state_framePhase == 4;
    }
    sysml_value_2 = sysml_lazy_8;
    bool sysml_value_3 = 0;
    sysml_value_3 = false;
    bool sysml_value_4 = 0;
    sysml_value_4 = (sysml_state_framePhase == 4);
    int32_t sysml_value_5 = 0;
    sysml_value_5 = sysml_state_pot0;
    int32_t sysml_value_6 = 0;
    sysml_value_6 = sysml_state_pot1;
    int32_t sysml_value_7 = 0;
    sysml_value_7 = sysml_state_pot2;
    int32_t sysml_value_8 = 0;
    sysml_value_8 = sysml_state_pot3;
    int32_t sysml_value_9 = 0;
    sysml_value_9 = sysml_state_pot4;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
    *serial_0_trigger = sysml_value_4;
    *serial_0_value_0 = sysml_value_5;
    *serial_0_value_1 = sysml_value_6;
    *serial_0_value_2 = sysml_value_7;
    *serial_0_value_3 = sysml_value_8;
    *serial_0_value_4 = sysml_value_9;
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
static bool hw_value_io_1 = false;
static bool hw_value_io_2 = false;
static bool hw_value_io_3 = false;
static double hw_value_io_4 = 0;
static uint32_t hw_sampled_io_4 = 0;
static bool hw_attempted_io_4 = false;
static bool hw_value_serial_0_trigger = false;
static int32_t hw_value_serial_0_value_0 = 0;
static int32_t hw_value_serial_0_value_1 = 0;
static int32_t hw_value_serial_0_value_2 = 0;
static int32_t hw_value_serial_0_value_3 = 0;
static int32_t hw_value_serial_0_value_4 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  digitalWrite(2, hw_value_io_0 ? HIGH : LOW);
  pinMode(3, OUTPUT);
  digitalWrite(3, hw_value_io_1 ? HIGH : LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, hw_value_io_2 ? HIGH : LOW);
  pinMode(5, OUTPUT);
  digitalWrite(5, hw_value_io_3 ? HIGH : LOW);
  pinMode(A0, INPUT);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_4 || (uint32_t)(hw_now - hw_sampled_io_4) >= 10UL) {
    hw_attempted_io_4 = true; hw_sampled_io_4 = hw_now;
    hw_value_io_4 = (analogRead(A0) * 1.0 + 0.0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_4, &hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0, &hw_value_serial_0_value_1, &hw_value_serial_0_value_2, &hw_value_serial_0_value_3, &hw_value_serial_0_value_4);
  digitalWrite(2, hw_value_io_0 ? HIGH : LOW);
  digitalWrite(3, hw_value_io_1 ? HIGH : LOW);
  digitalWrite(4, hw_value_io_2 ? HIGH : LOW);
  digitalWrite(5, hw_value_io_3 ? HIGH : LOW);
  if (hw_value_serial_0_trigger) { Serial.print("Sensor 0"); Serial.println(hw_value_serial_0_value_0); }
  if (hw_value_serial_0_trigger) { Serial.print("Sensor 1"); Serial.println(hw_value_serial_0_value_1); }
  if (hw_value_serial_0_trigger) { Serial.print("Sensor 2"); Serial.println(hw_value_serial_0_value_2); }
  if (hw_value_serial_0_trigger) { Serial.print("Sensor 3"); Serial.println(hw_value_serial_0_value_3); }
  if (hw_value_serial_0_trigger) { Serial.print("Sensor 4"); Serial.println(hw_value_serial_0_value_4); }
}

