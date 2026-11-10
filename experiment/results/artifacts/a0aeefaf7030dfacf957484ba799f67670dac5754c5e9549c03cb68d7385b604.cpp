#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision19\\cases\\complex_022\\arduino\\sketch\\sketch.ino"
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
extern int32_t sysml_state_brightness;
/* Global Variable define of  */
extern int32_t sysml_state_breathSpeed;
/* Global Variable define of  */
extern int32_t sysml_state_accumMs;
/* Global Variable define of  */
extern bool sysml_state_prevModeBtnPressed;
/* Global Variable define of  */
extern bool sysml_state_prevParamBtnPressed;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, int32_t io_5, double* io_2, int32_t* io_3, bool* io_4);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_mode = 0;
/* Global Variable define of  */
int32_t sysml_state_brightness = 255;
/* Global Variable define of  */
int32_t sysml_state_breathSpeed = 2000;
/* Global Variable define of  */
int32_t sysml_state_accumMs = 0;
/* Global Variable define of  */
bool sysml_state_prevModeBtnPressed = false;
/* Global Variable define of  */
bool sysml_state_prevParamBtnPressed = false;
/* Init function of CompositeActor named "ProgramPage1" */
#line 102 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision19\\cases\\complex_022\\arduino\\sketch\\sketch.ino"
static bool manual_debounce_input(bool raw, uint8_t index);
#line 111 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision19\\cases\\complex_022\\arduino\\sketch\\sketch.ino"
void setup();
#line 127 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision19\\cases\\complex_022\\arduino\\sketch\\sketch.ino"
void loop();
#line 49 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision19\\cases\\complex_022\\arduino\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_mode = 0;
    sysml_state_brightness = 255;
    sysml_state_breathSpeed = 2000;
    sysml_state_accumMs = 0;
    sysml_state_prevModeBtnPressed = false;
    sysml_state_prevParamBtnPressed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, int32_t io_5, double* io_2, int32_t* io_3, bool* io_4) {
    static bool prevMode=false,prevParameter=false;
    static uint8_t mode=0,brightness=64;
    static uint16_t period=2000;
    static uint32_t started=0;
    uint32_t now=millis();
    if (io_0 && !prevMode) { mode=(mode+1)%3; started=now; }
    if (io_1 && !prevParameter) {
        if (mode==1) brightness=brightness>=240 ? 16 : brightness+16;
        if (mode==2) { period=period<=500 ? 2000 : period-500; started=now; }
    }
    prevMode=io_0;
    prevParameter=io_1;
    unsigned phase=(uint32_t)(now-started)%period;
    unsigned ramp=phase<=period/2 ? phase : period-phase;
    *io_2=mode==0 ? 0 : (mode==1 ? brightness : ramp*510UL/period);
    *io_3=mode;
    *io_4=mode!=0;
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
static int32_t hw_value_io_3 = 0;
TM1637Display hw_display_io_3(4, 5);
static int32_t hw_display_previous_io_3 = INT32_MIN;
static bool hw_value_io_4 = false;
static int32_t hw_value_io_5 = 0;

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
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(9, OUTPUT);
  analogWrite(9, (int)constrain((hw_value_io_2 * 1.0 + 0.0), 0.0, 255.0));
  hw_display_io_3.setBrightness(7, true);
  if (hw_value_io_3 != hw_display_previous_io_3) {
    if (hw_value_io_3 < -999 || hw_value_io_3 > 9999) hw_display_io_3.clear();
    else hw_display_io_3.showNumberDec((int)hw_value_io_3, false, 4, 0);
    hw_display_previous_io_3 = hw_value_io_3;
  }
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_4 ? HIGH : LOW);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 10UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = manual_debounce_input((digitalRead(2) == LOW), 0);
  }
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 10UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = manual_debounce_input((digitalRead(3) == LOW), 1);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_5 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_5, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4);
  analogWrite(9, (int)constrain((hw_value_io_2 * 1.0 + 0.0), 0.0, 255.0));
  if (hw_value_io_3 != hw_display_previous_io_3) {
    if (hw_value_io_3 < -999 || hw_value_io_3 > 9999) hw_display_io_3.clear();
    else hw_display_io_3.showNumberDec((int)hw_value_io_3, false, 4, 0);
    hw_display_previous_io_3 = hw_value_io_3;
  }
  digitalWrite(13, hw_value_io_4 ? HIGH : LOW);
}

