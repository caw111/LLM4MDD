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
extern bool sysml_state_locked;
/* Global Variable define of  */
extern bool sysml_state_previousPressedAny;
/* Global Variable define of  */
extern bool sysml_state_previousPressedP1;
/* Global Variable define of  */
extern bool sysml_state_previousPressedP2;
/* Global Variable define of  */
extern bool sysml_state_previousPressedP3;
/* Global Variable define of  */
extern bool sysml_state_previousPressedP4;
/* Global Variable define of  */
extern bool sysml_state_resetHeld;
/* Global Variable define of  */
extern bool sysml_state_resetLatch;
/* Global Variable define of  */
extern int32_t sysml_state_playerId;
/* Global Variable define of  */
extern int32_t sysml_state_buzzerTone;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, bool io_3, bool io_4, bool* io_5, int32_t* io_6, int32_t* io_7, bool* serial_0_trigger, int32_t* serial_0_value_0, int32_t* serial_0_value_1);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_locked = false;
/* Global Variable define of  */
bool sysml_state_previousPressedAny = false;
/* Global Variable define of  */
bool sysml_state_previousPressedP1 = false;
/* Global Variable define of  */
bool sysml_state_previousPressedP2 = false;
/* Global Variable define of  */
bool sysml_state_previousPressedP3 = false;
/* Global Variable define of  */
bool sysml_state_previousPressedP4 = false;
/* Global Variable define of  */
bool sysml_state_resetHeld = false;
/* Global Variable define of  */
bool sysml_state_resetLatch = false;
/* Global Variable define of  */
int32_t sysml_state_playerId = 0;
/* Global Variable define of  */
int32_t sysml_state_buzzerTone = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_locked = false;
    sysml_state_previousPressedAny = false;
    sysml_state_previousPressedP1 = false;
    sysml_state_previousPressedP2 = false;
    sysml_state_previousPressedP3 = false;
    sysml_state_previousPressedP4 = false;
    sysml_state_resetHeld = false;
    sysml_state_resetLatch = false;
    sysml_state_playerId = 0;
    sysml_state_buzzerTone = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, bool io_3, bool io_4, bool* io_5, int32_t* io_6, int32_t* io_7, bool* serial_0_trigger, int32_t* serial_0_value_0, int32_t* serial_0_value_1) {
    static bool previous[4]={},clearDone=false,beeping=false;
    static uint32_t wins[4]={},resetStart=0,winTime=0;
    bool buttons[]={io_0,io_1,io_2,io_3};
    uint32_t now=millis();
    *serial_0_trigger=false;
    if (io_4) {
        if (!sysml_state_resetHeld) {
            resetStart=now;
            sysml_state_playerId=0;
            beeping=false;
            clearDone=false;
        }
        if (!clearDone && (uint32_t)(now-resetStart)>=3000) {
            for (uint8_t i=0; i<4; ++i) wins[i]=0;
            Serial.println("stats cleared");
            clearDone=true;
        }
    } else if (sysml_state_playerId==0) {
        for (uint8_t i=0; i<4; ++i) if (buttons[i] && !previous[i]) {
            sysml_state_playerId=i+1;
            ++wins[i];
            Serial.print("player wins: ");
            Serial.println(wins[i]);
            winTime=now;
            beeping=true;
            *serial_0_trigger=true;
            break;
        }
    }
    for (uint8_t i=0; i<4; ++i) previous[i]=buttons[i];
    sysml_state_resetHeld=io_4;
    if (beeping && (uint32_t)(now-winTime)>=200) beeping=false;
    *io_5=sysml_state_playerId!=0;
    *io_6=beeping ? 1000 : 0;
    *io_7=sysml_state_playerId;
    *serial_0_value_0=(int32_t)winTime;
    *serial_0_value_1=sysml_state_playerId;
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
static uint32_t hw_sampled_io_3 = 0;
static bool hw_attempted_io_3 = false;
static bool hw_value_io_4 = false;
static uint32_t hw_sampled_io_4 = 0;
static bool hw_attempted_io_4 = false;
static bool hw_value_io_5 = false;
static int32_t hw_value_io_6 = 0;
static int32_t hw_tone_io_6 = -1;
static int32_t hw_value_io_7 = 0;
TM1637Display hw_display_io_7(4, 5);
static int32_t hw_display_previous_io_7 = INT32_MIN;
static bool hw_value_serial_0_trigger = false;
static int32_t hw_value_serial_0_value_0 = 0;
static int32_t hw_value_serial_0_value_1 = 0;

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
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
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
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 5UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = manual_debounce_input((digitalRead(2) == LOW), 0);
  }
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 5UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = manual_debounce_input((digitalRead(3) == LOW), 1);
  }
  if (!hw_attempted_io_2 || (uint32_t)(hw_now - hw_sampled_io_2) >= 5UL) {
    hw_attempted_io_2 = true; hw_sampled_io_2 = hw_now;
    hw_value_io_2 = manual_debounce_input((digitalRead(7) == LOW), 2);
  }
  if (!hw_attempted_io_3 || (uint32_t)(hw_now - hw_sampled_io_3) >= 5UL) {
    hw_attempted_io_3 = true; hw_sampled_io_3 = hw_now;
    hw_value_io_3 = manual_debounce_input((digitalRead(8) == LOW), 3);
  }
  if (!hw_attempted_io_4 || (uint32_t)(hw_now - hw_sampled_io_4) >= 5UL) {
    hw_attempted_io_4 = true; hw_sampled_io_4 = hw_now;
    hw_value_io_4 = manual_debounce_input((digitalRead(9) == LOW), 4);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, hw_value_io_3, hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_io_7, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0, &hw_value_serial_0_value_1);
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
  if (hw_value_serial_0_trigger) { Serial.print("timestamp_ms"); Serial.println(hw_value_serial_0_value_0); }
  if (hw_value_serial_0_trigger) { Serial.print("player_id"); Serial.println(hw_value_serial_0_value_1); }
}
