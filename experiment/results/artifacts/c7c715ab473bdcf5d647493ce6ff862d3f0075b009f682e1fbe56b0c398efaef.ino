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
extern int32_t sysml_state_mode;
/* Global Variable define of  */
extern bool sysml_state_previousPressed;
/* Global Variable define of  */
extern int32_t sysml_state_modeAgeMs;
/* Global Variable define of  */
extern int32_t sysml_state_rainbowPhaseMs;
/* Global Variable define of  */
extern int32_t sysml_state_led1SeqStage;
/* Global Variable define of  */
extern int32_t sysml_state_led3SeqStage;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool* io_1, bool* io_2, bool* io_3, double* io_4, double* io_5, double* io_6, double* io_7, double* io_8, double* io_9, double* io_10, double* io_11, double* io_12);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_mode = 1;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
int32_t sysml_state_modeAgeMs = 0;
/* Global Variable define of  */
int32_t sysml_state_rainbowPhaseMs = 0;
/* Global Variable define of  */
int32_t sysml_state_led1SeqStage = 0;
/* Global Variable define of  */
int32_t sysml_state_led3SeqStage = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_mode = 1;
    sysml_state_previousPressed = false;
    sysml_state_modeAgeMs = 0;
    sysml_state_rainbowPhaseMs = 0;
    sysml_state_led1SeqStage = 0;
    sysml_state_led3SeqStage = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool* io_1, bool* io_2, bool* io_3, double* io_4, double* io_5, double* io_6, double* io_7, double* io_8, double* io_9, double* io_10, double* io_11, double* io_12) {
    static bool previous=false,started=false;
    static uint8_t mode=0;
    static uint32_t changed=0;
    uint32_t now=millis();
    if (io_0 && !previous && (!started || (uint32_t)(now-changed)>=2000)) {
        mode=(mode+1)%4;
        started=true;
        changed=now;
    }
    previous=io_0;
    *io_1=mode==1;
    *io_2=mode==2;
    *io_3=mode==3;
    double *out[]={io_4,io_5,io_6,io_7,io_8,io_9,io_10,io_11,io_12};
    const uint8_t rainbow[]={3,6,5};
    uint32_t time=(uint32_t)(now-changed);
    for (uint8_t led=0;led<3;++led) {
        for (uint8_t channel=0;channel<3;++channel) {
            double value=0;
            if (mode==1 && channel==led) value=255;
            if (mode==3) value=192;
            if (mode==2) {
                unsigned phase=(time/1000+led)%3;
                double from=(rainbow[phase]&(1U<<channel)) ? 255 : 0;
                double to=(rainbow[(phase+1)%3]&(1U<<channel)) ? 255 : 0;
                value=from+(to-from)*(time%1000)/1000.0;
            }
            *out[3*led+channel]=value;
        }
    }
}
// END COMPILED SYSML MODEL

// LLM4MDD target-board: arduino:avr:mega
#include <Arduino.h>
#include <math.h>
#if defined(ARDUINO) && !defined(ARDUINO_AVR_MEGA2560)
#error "Selected Arduino board differs from the interface contract"
#endif
static uint32_t hw_last_step = 0;
static bool hw_started = false;
static bool hw_value_io_0 = false;
static uint32_t hw_sampled_io_0 = 0;
static bool hw_attempted_io_0 = false;
static bool hw_value_io_1 = false;
static bool hw_value_io_2 = false;
static bool hw_value_io_3 = false;
static double hw_value_io_4 = 0;
static double hw_value_io_5 = 0;
static double hw_value_io_6 = 0;
static double hw_value_io_7 = 0;
static double hw_value_io_8 = 0;
static double hw_value_io_9 = 0;
static double hw_value_io_10 = 0;
static double hw_value_io_11 = 0;
static double hw_value_io_12 = 0;

static bool manual_debounce_input(bool raw, uint8_t index) {
  static bool previous[8]={}, stable[8]={};
  static uint32_t changed[8]={};
  uint32_t now=millis();
  if(raw!=previous[index]) {previous[index]=raw;changed[index]=now;}
  if((uint32_t)(now-changed[index])>=25UL) stable[index]=raw;
  return stable[index];
}

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(5, INPUT_PULLUP);
  pinMode(2, OUTPUT);
  digitalWrite(2, hw_value_io_1 ? HIGH : LOW);
  pinMode(3, OUTPUT);
  digitalWrite(3, hw_value_io_2 ? HIGH : LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, hw_value_io_3 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  analogWrite(6, (int)constrain((hw_value_io_4 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(7, OUTPUT);
  analogWrite(7, (int)constrain((hw_value_io_5 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(8, OUTPUT);
  analogWrite(8, (int)constrain((hw_value_io_6 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(9, OUTPUT);
  analogWrite(9, (int)constrain((hw_value_io_7 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(10, OUTPUT);
  analogWrite(10, (int)constrain((hw_value_io_8 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(11, OUTPUT);
  analogWrite(11, (int)constrain((hw_value_io_9 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(12, OUTPUT);
  analogWrite(12, (int)constrain((hw_value_io_10 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(13, OUTPUT);
  analogWrite(13, (int)constrain((hw_value_io_11 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(44, OUTPUT);
  analogWrite(44, (int)constrain((hw_value_io_12 * 1.0 + 0.0), 0.0, 255.0));
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 5UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = manual_debounce_input((digitalRead(5) == LOW), 0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_io_7, &hw_value_io_8, &hw_value_io_9, &hw_value_io_10, &hw_value_io_11, &hw_value_io_12);
  digitalWrite(2, hw_value_io_1 ? HIGH : LOW);
  digitalWrite(3, hw_value_io_2 ? HIGH : LOW);
  digitalWrite(4, hw_value_io_3 ? HIGH : LOW);
  analogWrite(6, (int)constrain((hw_value_io_4 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(7, (int)constrain((hw_value_io_5 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(8, (int)constrain((hw_value_io_6 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(9, (int)constrain((hw_value_io_7 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(10, (int)constrain((hw_value_io_8 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(11, (int)constrain((hw_value_io_9 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(12, (int)constrain((hw_value_io_10 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(13, (int)constrain((hw_value_io_11 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(44, (int)constrain((hw_value_io_12 * 1.0 + 0.0), 0.0, 255.0));
}
