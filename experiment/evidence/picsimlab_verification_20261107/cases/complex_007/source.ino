#ifdef ARDUINO
#include <Arduino.h>
#include <string.h>
#include <stdlib.h>
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
extern double sysml_state_pwmValue;
/* Global Variable define of  */
extern int32_t sysml_state_buzzerHz;
/* Global Variable define of  */
extern bool sysml_state_prevPressed;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_3, bool* io_0, double* io_1, int32_t* io_2, bool* serial_0_trigger, bool* serial_0_value_0, int32_t* serial_0_value_1);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_ledState = false;
/* Global Variable define of  */
double sysml_state_pwmValue = 0.0;
/* Global Variable define of  */
int32_t sysml_state_buzzerHz = 0;
/* Global Variable define of  */
bool sysml_state_prevPressed = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_ledState = false;
    sysml_state_pwmValue = 0.0;
    sysml_state_buzzerHz = 0;
    sysml_state_prevPressed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_3, bool* io_0, double* io_1, int32_t* io_2, bool* serial_0_trigger, bool* serial_0_value_0, int32_t* serial_0_value_1) {
    static char command[24];
    static uint8_t used=0,mode=0;
    static bool overflow=false,buzzing=false;
    static uint32_t blinkStart=0,buzzStart=0,lastByte=0;
    uint32_t now=millis();
    bool report=io_3 && !sysml_state_prevPressed;
    sysml_state_prevPressed=io_3;
    for (uint8_t budget=0; budget<64; ++budget) {
        char c='\n';
        if (Serial.available()) { c=(char)Serial.read(); lastByte=now; }
        else if (!(used || overflow) || (uint32_t)(now-lastByte)<20) break;
        if (c!='\n' && c!='\r') {
            if (used<sizeof(command)-1) command[used++]=c;
            else overflow=true;
            continue;
        }
        command[used]='\0';
        if (!overflow) {
            if (strcmp(command,"ON")==0) mode=1;
            else if (strcmp(command,"OFF")==0) mode=0;
            else if (strcmp(command,"BLINK")==0) { mode=2; blinkStart=now; }
            else if (strcmp(command,"BUZZ")==0) { buzzing=true; buzzStart=now; }
            else if (strcmp(command,"STATUS")==0) report=true;
            else if (strncmp(command,"PWM:",4)==0) {
                char *end;
                long value=strtol(command+4,&end,10);
                if (end!=command+4 && *end=='\0' && value>=0 && value<=255) sysml_state_pwmValue=value;
            }
        }
        used=0;
        overflow=false;
    }
    if (buzzing && (uint32_t)(now-buzzStart)>=250) buzzing=false;
    *io_0=mode==1 || (mode==2 && (uint32_t)(now-blinkStart)%1000<500);
    *io_1=sysml_state_pwmValue;
    *io_2=buzzing ? 1000 : 0;
    *serial_0_trigger=report;
    *serial_0_value_0=*io_0;
    *serial_0_value_1=(int32_t)*io_1;
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
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_0 ? HIGH : LOW);
  pinMode(9, OUTPUT);
  analogWrite(9, (int)constrain((hw_value_io_1 * 1 + 0), 0.0, 255.0));
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
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_3 || (uint32_t)(hw_now - hw_sampled_io_3) >= 10UL) {
    hw_attempted_io_3 = true; hw_sampled_io_3 = hw_now;
    hw_value_io_3 = manual_debounce_input((digitalRead(2) == LOW), 0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_3, &hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0, &hw_value_serial_0_value_1);
  digitalWrite(13, hw_value_io_0 ? HIGH : LOW);
  analogWrite(9, (int)constrain((hw_value_io_1 * 1 + 0), 0.0, 255.0));
  int32_t hw_tone_command_io_2 = (hw_value_io_2 >= 31 && hw_value_io_2 <= 65535) ? hw_value_io_2 : 0;
  if (hw_tone_command_io_2 != hw_tone_io_2) {
    if (hw_tone_command_io_2 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_2);
    hw_tone_io_2 = hw_tone_command_io_2;
  }
  if (hw_value_serial_0_trigger) { Serial.print("LED_STATE"); Serial.println(hw_value_serial_0_value_0); }
  if (hw_value_serial_0_trigger) { Serial.print("PWM_VALUE"); Serial.println(hw_value_serial_0_value_1); }
}
