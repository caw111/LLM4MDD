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
extern bool sysml_state_processComplete;
/* Global Variable define of  */
extern int32_t sysml_state_elapsed_stage1_ms;
/* Global Variable define of  */
extern int32_t sysml_state_elapsed_stage2_ms;
/* Global Variable define of  */
extern int32_t sysml_state_elapsed_stage3_ms;
/* Global Variable define of  */
extern int32_t sysml_state_countdown_display;
/* Global Variable define of  */
extern int32_t sysml_state_buzzer_hz;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_1, bool io_2, bool io_3, int32_t* io_0, int32_t* io_4, bool* io_5, bool* serial_0_trigger, int32_t* serial_0_value_0, int32_t* serial_0_value_1, int32_t* serial_0_value_2);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_processComplete = false;
/* Global Variable define of  */
int32_t sysml_state_elapsed_stage1_ms = 0;
/* Global Variable define of  */
int32_t sysml_state_elapsed_stage2_ms = 0;
/* Global Variable define of  */
int32_t sysml_state_elapsed_stage3_ms = 0;
/* Global Variable define of  */
int32_t sysml_state_countdown_display = 0;
/* Global Variable define of  */
int32_t sysml_state_buzzer_hz = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_processComplete = false;
    sysml_state_elapsed_stage1_ms = 0;
    sysml_state_elapsed_stage2_ms = 0;
    sysml_state_elapsed_stage3_ms = 0;
    sysml_state_countdown_display = 0;
    sysml_state_buzzer_hz = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_1, bool io_2, bool io_3, int32_t* io_0, int32_t* io_4, bool* io_5, bool* serial_0_trigger, int32_t* serial_0_value_0, int32_t* serial_0_value_1, int32_t* serial_0_value_2) {
    static bool previous[3]={},running=false,active=false,complete=false;
    static uint8_t selected=0,stage=0,pulses=0;
    static uint32_t duration[3]={2000,2000,2000},measured[3]={},last=0,beganBeep=0;
    uint32_t now=millis();
    uint32_t elapsed=(uint32_t)(now-last);
    last=now;
    bool report=false;
    if (running) {
        measured[stage]+=elapsed;
        if (measured[stage]>=duration[stage]) {
            pulses=stage+1;
            beganBeep=now;
            if (++stage==3) {
                active=running=false;
                complete=report=true;
            }
        }
    }
    if (!active) {
        if (io_1 && !previous[0] && duration[selected]<9999000UL) {duration[selected]+=1000;complete=false;}
        if (io_2 && !previous[1]) {selected=(selected+1)%3;complete=false;}
    }
    if (io_3 && !previous[2]) {
        if (!active) {
            for (uint8_t i=0;i<3;++i) measured[i]=0;
            stage=0;
            complete=false;
            active=running=true;
        } else running=!running;
    }
    previous[0]=io_1;
    previous[1]=io_2;
    previous[2]=io_3;
    uint32_t beepTime=(uint32_t)(now-beganBeep);
    if (beepTime>=pulses*300UL) pulses=0;
    *io_0=complete ? 0 : (int32_t)((active ? duration[stage]-measured[stage]+999 : duration[selected])/1000);
    *io_4=pulses && beepTime%300<150 ? 1000 : 0;
    *io_5=complete;
    *serial_0_trigger=report;
    if (report) Serial.println("process complete");
    *serial_0_value_0=measured[0];
    *serial_0_value_1=measured[1];
    *serial_0_value_2=measured[2];
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
static int32_t hw_value_io_0 = 0;
TM1637Display hw_display_io_0(4, 5);
static int32_t hw_display_previous_io_0 = INT32_MIN;
static bool hw_value_io_1 = false;
static uint32_t hw_sampled_io_1 = 0;
static bool hw_attempted_io_1 = false;
static bool hw_value_io_2 = false;
static uint32_t hw_sampled_io_2 = 0;
static bool hw_attempted_io_2 = false;
static bool hw_value_io_3 = false;
static uint32_t hw_sampled_io_3 = 0;
static bool hw_attempted_io_3 = false;
static int32_t hw_value_io_4 = 0;
static int32_t hw_tone_io_4 = -1;
static bool hw_value_io_5 = false;
static bool hw_value_serial_0_trigger = false;
static int32_t hw_value_serial_0_value_0 = 0;
static int32_t hw_value_serial_0_value_1 = 0;
static int32_t hw_value_serial_0_value_2 = 0;

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
  hw_display_io_0.setBrightness(7, true);
  if (hw_value_io_0 != hw_display_previous_io_0) {
    if (hw_value_io_0 < -999 || hw_value_io_0 > 9999) hw_display_io_0.clear();
    else hw_display_io_0.showNumberDec((int)hw_value_io_0, false, 4, 0);
    hw_display_previous_io_0 = hw_value_io_0;
  }
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(6, OUTPUT);
  int32_t hw_tone_command_io_4 = (hw_value_io_4 >= 31 && hw_value_io_4 <= 65535) ? hw_value_io_4 : 0;
  if (hw_tone_command_io_4 != hw_tone_io_4) {
    if (hw_tone_command_io_4 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_4);
    hw_tone_io_4 = hw_tone_command_io_4;
  }
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_5 ? HIGH : LOW);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 10UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = manual_debounce_input((digitalRead(2) == LOW), 0);
  }
  if (!hw_attempted_io_2 || (uint32_t)(hw_now - hw_sampled_io_2) >= 10UL) {
    hw_attempted_io_2 = true; hw_sampled_io_2 = hw_now;
    hw_value_io_2 = manual_debounce_input((digitalRead(3) == LOW), 1);
  }
  if (!hw_attempted_io_3 || (uint32_t)(hw_now - hw_sampled_io_3) >= 10UL) {
    hw_attempted_io_3 = true; hw_sampled_io_3 = hw_now;
    hw_value_io_3 = manual_debounce_input((digitalRead(7) == LOW), 2);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_1, hw_value_io_2, hw_value_io_3, &hw_value_io_0, &hw_value_io_4, &hw_value_io_5, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0, &hw_value_serial_0_value_1, &hw_value_serial_0_value_2);
  if (hw_value_io_0 != hw_display_previous_io_0) {
    if (hw_value_io_0 < -999 || hw_value_io_0 > 9999) hw_display_io_0.clear();
    else hw_display_io_0.showNumberDec((int)hw_value_io_0, false, 4, 0);
    hw_display_previous_io_0 = hw_value_io_0;
  }
  int32_t hw_tone_command_io_4 = (hw_value_io_4 >= 31 && hw_value_io_4 <= 65535) ? hw_value_io_4 : 0;
  if (hw_tone_command_io_4 != hw_tone_io_4) {
    if (hw_tone_command_io_4 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_4);
    hw_tone_io_4 = hw_tone_command_io_4;
  }
  digitalWrite(13, hw_value_io_5 ? HIGH : LOW);
  if (hw_value_serial_0_trigger) { Serial.print("stage1_elapsed_ms"); Serial.println(hw_value_serial_0_value_0); }
  if (hw_value_serial_0_trigger) { Serial.print("stage2_elapsed_ms"); Serial.println(hw_value_serial_0_value_1); }
  if (hw_value_serial_0_trigger) { Serial.print("stage3_elapsed_ms"); Serial.println(hw_value_serial_0_value_2); }
}
