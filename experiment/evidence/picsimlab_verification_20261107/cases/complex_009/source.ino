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
extern bool sysml_state_running;
/* Global Variable define of  */
extern bool sysml_state_previousStartPausePressed;
/* Global Variable define of  */
extern bool sysml_state_previousResetPressed;
/* Global Variable define of  */
extern int32_t sysml_state_seconds;
/* Global Variable define of  */
extern int32_t sysml_state_beepRemainingSeconds;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, int32_t* io_2, int32_t* io_3);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_running = false;
/* Global Variable define of  */
bool sysml_state_previousStartPausePressed = false;
/* Global Variable define of  */
bool sysml_state_previousResetPressed = false;
/* Global Variable define of  */
int32_t sysml_state_seconds = 0;
/* Global Variable define of  */
int32_t sysml_state_beepRemainingSeconds = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_running = false;
    sysml_state_previousStartPausePressed = false;
    sysml_state_previousResetPressed = false;
    sysml_state_seconds = 0;
    sysml_state_beepRemainingSeconds = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, int32_t* io_2, int32_t* io_3) {
    static uint32_t elapsed=0,last=millis(),beepStart=0;
    static bool running=false,previous[2]={},beeping=false;
    uint32_t now=millis(),dt=now-last;last=now;
    uint32_t old=elapsed;
    if(running) elapsed+=dt;
    if(old<60000UL && elapsed>=60000UL) {beeping=true;beepStart=now;}
    if(io_0&&!previous[0]) running=!running;
    if(io_1&&!previous[1]) {elapsed=0;running=false;beeping=false;}
    if(beeping && (uint32_t)(now-beepStart)>=1000) beeping=false;
    *io_2=beeping ? 1000 : 0;*io_3=elapsed/1000UL;
    previous[0]=io_0;previous[1]=io_1;
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
static int32_t hw_value_io_2 = 0;
static int32_t hw_tone_io_2 = -1;
static int32_t hw_value_io_3 = 0;
TM1637Display hw_display_io_3(4, 5);
static int32_t hw_display_previous_io_3 = INT32_MIN;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(6, OUTPUT);
  int32_t hw_tone_command_io_2 = (hw_value_io_2 >= 31 && hw_value_io_2 <= 65535) ? hw_value_io_2 : 0;
  if (hw_tone_command_io_2 != hw_tone_io_2) {
    if (hw_tone_command_io_2 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_2);
    hw_tone_io_2 = hw_tone_command_io_2;
  }
  hw_display_io_3.setBrightness(7, true);
  if (hw_value_io_3 != hw_display_previous_io_3) {
    if (hw_value_io_3 < -999 || hw_value_io_3 > 9999) hw_display_io_3.clear();
    else hw_display_io_3.showNumberDec((int)hw_value_io_3, false, 4, 0);
    hw_display_previous_io_3 = hw_value_io_3;
  }
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 5UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (digitalRead(2) == LOW);
  }
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 5UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = (digitalRead(3) == LOW);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, &hw_value_io_2, &hw_value_io_3);
  int32_t hw_tone_command_io_2 = (hw_value_io_2 >= 31 && hw_value_io_2 <= 65535) ? hw_value_io_2 : 0;
  if (hw_tone_command_io_2 != hw_tone_io_2) {
    if (hw_tone_command_io_2 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_2);
    hw_tone_io_2 = hw_tone_command_io_2;
  }
  if (hw_value_io_3 != hw_display_previous_io_3) {
    if (hw_value_io_3 < -999 || hw_value_io_3 > 9999) hw_display_io_3.clear();
    else hw_display_io_3.showNumberDec((int)hw_value_io_3, false, 4, 0);
    hw_display_previous_io_3 = hw_value_io_3;
  }
}
