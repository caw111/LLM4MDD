#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision33\\cases\\advanced_026\\arduino\\sketch\\sketch.ino"
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
extern bool sysml_state_previousAnyPIR;
/* Global Variable define of  */
extern bool sysml_state_isLighting;
/* Global Variable define of  */
extern int32_t sysml_state_todayCount;
/* Global Variable define of  */
extern int32_t sysml_state_illuminatedOnTimeMs;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double io_2, double* io_3, int32_t* io_4, double* io_5, bool* serial_0_trigger, int32_t* serial_0_value_0);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousAnyPIR = false;
/* Global Variable define of  */
bool sysml_state_isLighting = false;
/* Global Variable define of  */
int32_t sysml_state_todayCount = 0;
/* Global Variable define of  */
int32_t sysml_state_illuminatedOnTimeMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
#line 103 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision33\\cases\\advanced_026\\arduino\\sketch\\sketch.ino"
void setup();
#line 121 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision33\\cases\\advanced_026\\arduino\\sketch\\sketch.ino"
void loop();
#line 41 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision33\\cases\\advanced_026\\arduino\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousAnyPIR = false;
    sysml_state_isLighting = false;
    sysml_state_todayCount = 0;
    sysml_state_illuminatedOnTimeMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double io_2, double* io_3, int32_t* io_4, double* io_5, bool* serial_0_trigger, int32_t* serial_0_value_0) {
    static bool lastLeft=false,lastRight=false,wasLit=false,detected=false;
    static uint8_t from=0;
    static uint32_t since=0,motion=0,last=0,lastReport=0,total=0;
    static int count=0;
    uint32_t now=millis();
    if (wasLit) total+=(uint32_t)(now-last);
    last=now;
    bool left=io_0 && !lastLeft,right=io_1 && !lastRight;
    lastLeft=io_0;
    lastRight=io_1;
    if (from && (uint32_t)(now-since)>4000) from=0;
    if (left!=right) {
        uint8_t side=left ? 1 : 2;
        if (from && from!=side) {if (count<9999) ++count;from=0;}
        else {from=side;since=now;}
    }
    if (io_0 || io_1) {detected=true;motion=now;}
    wasLit=io_2<350 && detected && (uint32_t)(now-motion)<3000;
    *io_3=0;
    *io_4=count;
    *io_5=wasLit ? 255 : 0;
    *serial_0_trigger=(uint32_t)(now-lastReport)>=5000;
    if (*serial_0_trigger) lastReport=now;
    *serial_0_value_0=(int32_t)total;
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
static uint32_t hw_sampled_io_2 = 0;
static bool hw_attempted_io_2 = false;
static double hw_value_io_3 = 0;
static int32_t hw_value_io_4 = 0;
TM1637Display hw_display_io_4(4, 5);
static int32_t hw_display_previous_io_4 = INT32_MIN;
static double hw_value_io_5 = 0;
static bool hw_value_serial_0_trigger = false;
static int32_t hw_value_serial_0_value_0 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(A0, INPUT);
  pinMode(6, OUTPUT);
  analogWrite(6, (int)constrain((hw_value_io_3 * 1.0 + 0.0), 0.0, 255.0));
  hw_display_io_4.setBrightness(7, true);
  if (hw_value_io_4 != hw_display_previous_io_4) {
    if (hw_value_io_4 < -999 || hw_value_io_4 > 9999) hw_display_io_4.clear();
    else hw_display_io_4.showNumberDec((int)hw_value_io_4, false, 4, 0);
    hw_display_previous_io_4 = hw_value_io_4;
  }
  pinMode(9, OUTPUT);
  analogWrite(9, (int)constrain((hw_value_io_5 * 1.0 + 0.0), 0.0, 255.0));
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 10UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (digitalRead(2) == HIGH);
  }
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 10UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = (digitalRead(3) == HIGH);
  }
  if (!hw_attempted_io_2 || (uint32_t)(hw_now - hw_sampled_io_2) >= 100UL) {
    hw_attempted_io_2 = true; hw_sampled_io_2 = hw_now;
    hw_value_io_2 = (analogRead(A0) * 1.0 + 0.0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0);
  analogWrite(6, (int)constrain((hw_value_io_3 * 1.0 + 0.0), 0.0, 255.0));
  if (hw_value_io_4 != hw_display_previous_io_4) {
    if (hw_value_io_4 < -999 || hw_value_io_4 > 9999) hw_display_io_4.clear();
    else hw_display_io_4.showNumberDec((int)hw_value_io_4, false, 4, 0);
    hw_display_previous_io_4 = hw_value_io_4;
  }
  analogWrite(9, (int)constrain((hw_value_io_5 * 1.0 + 0.0), 0.0, 255.0));
  if (hw_value_serial_0_trigger) { Serial.print("illuminated_on_time_ms"); Serial.println(hw_value_serial_0_value_0); }
}

