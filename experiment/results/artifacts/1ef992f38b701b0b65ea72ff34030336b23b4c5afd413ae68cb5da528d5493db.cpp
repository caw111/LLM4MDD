#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision17\\cases\\embedbench_78\\arduino\\sketch\\sketch.ino"
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
extern bool sysml_state_initialized;
/* Global Variable define of  */
extern int32_t sysml_state_displayNumber;
/* Global Variable define of  */
extern int32_t sysml_state_previousTensDigit;
/* Global Variable define of  */
extern int32_t sysml_state_previousUnitsDigit;
/* Global Variable define of  */
extern bool sysml_state_previousIncrementPressed;
/* Global Variable define of  */
extern bool sysml_state_previousDecrementPressed;
/* Global Variable define of  */
extern double sysml_state_incrementElapsed;
/* Global Variable define of  */
extern double sysml_state_decrementElapsed;
/* Global Variable define of  */
extern int32_t sysml_state_incrementSteps;
/* Global Variable define of  */
extern int32_t sysml_state_decrementSteps;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double io_2, double io_3, int32_t io_5, int32_t* io_4);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_initialized = false;
/* Global Variable define of  */
int32_t sysml_state_displayNumber = 0;
/* Global Variable define of  */
int32_t sysml_state_previousTensDigit = 0;
/* Global Variable define of  */
int32_t sysml_state_previousUnitsDigit = 0;
/* Global Variable define of  */
bool sysml_state_previousIncrementPressed = false;
/* Global Variable define of  */
bool sysml_state_previousDecrementPressed = false;
/* Global Variable define of  */
double sysml_state_incrementElapsed = 0.0;
/* Global Variable define of  */
double sysml_state_decrementElapsed = 0.0;
/* Global Variable define of  */
int32_t sysml_state_incrementSteps = 0;
/* Global Variable define of  */
int32_t sysml_state_decrementSteps = 0;
/* Init function of CompositeActor named "ProgramPage1" */
#line 116 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision17\\cases\\embedbench_78\\arduino\\sketch\\sketch.ino"
static void hw_shift595_io_4(int32_t value);
#line 151 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision17\\cases\\embedbench_78\\arduino\\sketch\\sketch.ino"
static bool manual_debounce_input(bool raw, uint8_t index);
#line 161 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision17\\cases\\embedbench_78\\arduino\\sketch\\sketch.ino"
void setup();
#line 175 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision17\\cases\\embedbench_78\\arduino\\sketch\\sketch.ino"
void loop();
#line 65 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision17\\cases\\embedbench_78\\arduino\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_initialized = false;
    sysml_state_displayNumber = 0;
    sysml_state_previousTensDigit = 0;
    sysml_state_previousUnitsDigit = 0;
    sysml_state_previousIncrementPressed = false;
    sysml_state_previousDecrementPressed = false;
    sysml_state_incrementElapsed = 0.0;
    sysml_state_decrementElapsed = 0.0;
    sysml_state_incrementSteps = 0;
    sysml_state_decrementSteps = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double io_2, double io_3, int32_t io_5, int32_t* io_4) {
    static int number=(int)(io_2*9.0/1023.0)*10+(int)(io_3*9.0/1023.0);
    static int previous=0;
    static uint32_t last=0;
    uint32_t now=millis();
    int direction=(int)io_0-(int)io_1;
    uint32_t interval=100+(uint32_t)((direction>0 ? io_2 : io_3)*900.0/1023.0);
    if (direction && (direction!=previous || (uint32_t)(now-last)>=interval)) {
        number=(number+direction+100)%100;
        last=now;
    }
    previous=direction;
    *io_4=number;
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
static uint32_t hw_sampled_io_1 = 0;
static bool hw_attempted_io_1 = false;
static double hw_value_io_2 = 0;
static uint32_t hw_sampled_io_2 = 0;
static bool hw_attempted_io_2 = false;
static double hw_value_io_3 = 0;
static uint32_t hw_sampled_io_3 = 0;
static bool hw_attempted_io_3 = false;
static int32_t hw_value_io_4 = 0;
static void hw_shift595_io_4(int32_t value) {
  static const uint8_t digits[10] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
  bool valid = value >= 0 && value < 100L;
  digitalWrite(A1, LOW);
  {
    uint8_t segments = valid && (true) ? digits[(value / 1L) % 10] : 0;
    uint8_t bits = 0;
    if (segments & 1) bits |= 1;
    if (segments & 2) bits |= 2;
    if (segments & 4) bits |= 4;
    if (segments & 8) bits |= 8;
    if (segments & 16) bits |= 16;
    if (segments & 32) bits |= 32;
    if (segments & 64) bits |= 64;
    bits = (uint8_t)~bits;
    shiftOut(A0, A2, MSBFIRST, bits);
  }
  {
    uint8_t segments = valid && (true) ? digits[(value / 10L) % 10] : 0;
    uint8_t bits = 0;
    if (segments & 1) bits |= 1;
    if (segments & 2) bits |= 2;
    if (segments & 4) bits |= 4;
    if (segments & 8) bits |= 8;
    if (segments & 16) bits |= 16;
    if (segments & 32) bits |= 32;
    if (segments & 64) bits |= 64;
    bits = (uint8_t)~bits;
    shiftOut(A0, A2, MSBFIRST, bits);
  }
  digitalWrite(A1, HIGH);
}
static int32_t hw_shift595_previous_io_4 = INT32_MIN;
static int32_t hw_value_io_5 = 0;

static bool manual_debounce_input(bool raw, uint8_t index) {
  static bool previous[8]={}, stable[8]={};
  static uint32_t changed[8]={};
  uint32_t now=millis();
  if(raw!=previous[index]) {previous[index]=raw;changed[index]=now;}
  if((uint32_t)(now-changed[index])>=25UL) stable[index]=raw;
  if(!raw) stable[index]=false;
  return stable[index];
}

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A0, OUTPUT); digitalWrite(A0, LOW);
  pinMode(A2, OUTPUT); digitalWrite(A2, LOW);
  pinMode(A1, OUTPUT); digitalWrite(A1, LOW);
  if (hw_value_io_4 != hw_shift595_previous_io_4) {
    hw_shift595_io_4(hw_value_io_4); hw_shift595_previous_io_4 = hw_value_io_4;
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
    hw_value_io_2 = (analogRead(A3) * 1.0 + 0.0);
  }
  if (!hw_attempted_io_3 || (uint32_t)(hw_now - hw_sampled_io_3) >= 5UL) {
    hw_attempted_io_3 = true; hw_sampled_io_3 = hw_now;
    hw_value_io_3 = (analogRead(A4) * 1.0 + 0.0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_5 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, hw_value_io_3, hw_value_io_5, &hw_value_io_4);
  if (hw_value_io_4 != hw_shift595_previous_io_4) {
    hw_shift595_io_4(hw_value_io_4); hw_shift595_previous_io_4 = hw_value_io_4;
  }
}

