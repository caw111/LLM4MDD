#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision10\\cases\\embedbench_102\\arduino\\sketch\\sketch.ino"
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
extern int32_t sysml_state_elapsedAccumMs;
/* Global Variable define of  */
extern int32_t sysml_state_displayedNumber;
/* Global Variable define of  */
extern int32_t sysml_state_tensDigit;
/* Global Variable define of  */
extern int32_t sysml_state_unitsDigit;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_7, bool* io_0, bool* io_1, bool* io_2, int32_t* io_3, double* io_4, double* io_5, double* io_6);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_elapsedAccumMs = 0;
/* Global Variable define of  */
int32_t sysml_state_displayedNumber = 0;
/* Global Variable define of  */
int32_t sysml_state_tensDigit = 0;
/* Global Variable define of  */
int32_t sysml_state_unitsDigit = 0;
/* Init function of CompositeActor named "ProgramPage1" */
#line 207 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision10\\cases\\embedbench_102\\arduino\\sketch\\sketch.ino"
static void hw_shift595_io_3(int32_t value);
#line 251 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision10\\cases\\embedbench_102\\arduino\\sketch\\sketch.ino"
void setup();
#line 288 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision10\\cases\\embedbench_102\\arduino\\sketch\\sketch.ino"
void loop();
#line 41 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision10\\cases\\embedbench_102\\arduino\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_elapsedAccumMs = 0;
    sysml_state_displayedNumber = 0;
    sysml_state_tensDigit = 0;
    sysml_state_unitsDigit = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_7, bool* io_0, bool* io_1, bool* io_2, int32_t* io_3, double* io_4, double* io_5, double* io_6) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int32_t sysml_conditional_0 = 0;
    int32_t sysml_conditional_1 = 0;
    int32_t sysml_arithmetic_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_conditional_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_conditional_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_conditional_8 = 0;
    double sysml_conditional_9 = 0;
    bool sysml_lazy_10 = 0;
    double sysml_conditional_11 = 0;
    double sysml_conditional_12 = 0;
    bool sysml_lazy_13 = 0;
    double sysml_conditional_14 = 0;
    double sysml_conditional_15 = 0;
    bool sysml_lazy_16 = 0;
    double sysml_conditional_17 = 0;
    sysml_state_elapsedAccumMs = (sysml_state_elapsedAccumMs + io_7);
    if ((sysml_state_elapsedAccumMs >= 2000)) {
        sysml_state_elapsedAccumMs = (sysml_state_elapsedAccumMs - 2000);
        sysml_state_displayedNumber = (sysml_state_displayedNumber + 1);
        if ((sysml_state_displayedNumber > 99)) {
            sysml_state_displayedNumber = 0;
        }
    }
    if ((((double)sysml_state_displayedNumber) / 10.0) < ((double)((int32_t)(((double)sysml_state_displayedNumber) / 10.0)))) {
        sysml_conditional_0 = 1;
    }
    else {
        sysml_conditional_0 = 0;
    }
    sysml_state_tensDigit = ((int32_t)(((double)sysml_state_displayedNumber) / 10.0)) - sysml_conditional_0;
    if ((((double)sysml_state_displayedNumber) / 10.0) < ((double)((int32_t)(((double)sysml_state_displayedNumber) / 10.0)))) {
        sysml_conditional_1 = 1;
    }
    else {
        sysml_conditional_1 = 0;
    }
    sysml_arithmetic_2 = ((int32_t)(((double)sysml_state_displayedNumber) / 10.0)) - sysml_conditional_1;
    sysml_state_unitsDigit = sysml_state_displayedNumber - (10 * sysml_arithmetic_2);
    bool sysml_value_0 = 0;
    sysml_lazy_3 = sysml_state_unitsDigit >= 0;
    if (sysml_lazy_3) {
        sysml_lazy_3 = sysml_state_unitsDigit <= 2;
    }
    if (sysml_lazy_3) {
        sysml_conditional_4 = true;
    }
    else {
        sysml_conditional_4 = false;
    }
    sysml_value_0 = sysml_conditional_4;
    bool sysml_value_1 = 0;
    sysml_lazy_5 = sysml_state_unitsDigit >= 3;
    if (sysml_lazy_5) {
        sysml_lazy_5 = sysml_state_unitsDigit <= 5;
    }
    if (sysml_lazy_5) {
        sysml_conditional_6 = true;
    }
    else {
        sysml_conditional_6 = false;
    }
    sysml_value_1 = sysml_conditional_6;
    bool sysml_value_2 = 0;
    sysml_lazy_7 = sysml_state_unitsDigit >= 6;
    if (sysml_lazy_7) {
        sysml_lazy_7 = sysml_state_unitsDigit <= 9;
    }
    if (sysml_lazy_7) {
        sysml_conditional_8 = true;
    }
    else {
        sysml_conditional_8 = false;
    }
    sysml_value_2 = sysml_conditional_8;
    int32_t sysml_value_3 = 0;
    sysml_value_3 = sysml_state_displayedNumber;
    double sysml_value_4 = 0;
    if (sysml_state_tensDigit <= 2) {
        sysml_conditional_9 = 30.0;
    }
    else {
        sysml_lazy_10 = sysml_state_tensDigit >= 3;
        if (sysml_lazy_10) {
            sysml_lazy_10 = sysml_state_tensDigit <= 5;
        }
        if (sysml_lazy_10) {
            sysml_conditional_11 = 60.0;
        }
        else {
            sysml_conditional_11 = 90.0;
        }
        sysml_conditional_9 = sysml_conditional_11;
    }
    sysml_value_4 = sysml_conditional_9;
    double sysml_value_5 = 0;
    if (sysml_state_tensDigit <= 2) {
        sysml_conditional_12 = 60.0;
    }
    else {
        sysml_lazy_13 = sysml_state_tensDigit >= 3;
        if (sysml_lazy_13) {
            sysml_lazy_13 = sysml_state_tensDigit <= 5;
        }
        if (sysml_lazy_13) {
            sysml_conditional_14 = 90.0;
        }
        else {
            sysml_conditional_14 = 120.0;
        }
        sysml_conditional_12 = sysml_conditional_14;
    }
    sysml_value_5 = sysml_conditional_12;
    double sysml_value_6 = 0;
    if (sysml_state_tensDigit <= 2) {
        sysml_conditional_15 = 90.0;
    }
    else {
        sysml_lazy_16 = sysml_state_tensDigit >= 3;
        if (sysml_lazy_16) {
            sysml_lazy_16 = sysml_state_tensDigit <= 5;
        }
        if (sysml_lazy_16) {
            sysml_conditional_17 = 120.0;
        }
        else {
            sysml_conditional_17 = 150.0;
        }
        sysml_conditional_15 = sysml_conditional_17;
    }
    sysml_value_6 = sysml_conditional_15;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
    *io_4 = sysml_value_4;
    *io_5 = sysml_value_5;
    *io_6 = sysml_value_6;
}
// END COMPILED SYSML MODEL

// LLM4MDD target-board: arduino:avr:mega
#include <Servo.h>
#include <Arduino.h>
#include <math.h>
#if defined(ARDUINO) && !defined(ARDUINO_AVR_MEGA2560)
#error "Selected Arduino board differs from the interface contract"
#endif
static uint32_t hw_last_step = 0;
static bool hw_started = false;
static bool hw_value_io_0 = false;
static bool hw_value_io_1 = false;
static bool hw_value_io_2 = false;
static int32_t hw_value_io_3 = 0;
static void hw_shift595_io_3(int32_t value) {
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
static int32_t hw_shift595_previous_io_3 = INT32_MIN;
static double hw_value_io_4 = 0;
Servo hw_servo_io_4;
static int hw_servo_previous_io_4 = -1;
static double hw_value_io_5 = 0;
Servo hw_servo_io_5;
static int hw_servo_previous_io_5 = -1;
static double hw_value_io_6 = 0;
Servo hw_servo_io_6;
static int hw_servo_previous_io_6 = -1;
static int32_t hw_value_io_7 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, OUTPUT);
  digitalWrite(2, hw_value_io_0 ? HIGH : LOW);
  pinMode(3, OUTPUT);
  digitalWrite(3, hw_value_io_1 ? HIGH : LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, hw_value_io_2 ? HIGH : LOW);
  pinMode(A0, OUTPUT); digitalWrite(A0, LOW);
  pinMode(A2, OUTPUT); digitalWrite(A2, LOW);
  pinMode(A1, OUTPUT); digitalWrite(A1, LOW);
  if (hw_value_io_3 != hw_shift595_previous_io_3) {
    hw_shift595_io_3(hw_value_io_3); hw_shift595_previous_io_3 = hw_value_io_3;
  }
  hw_servo_io_4.attach(5);
  if (isfinite((double)hw_value_io_4) && hw_value_io_4 >= 0 && hw_value_io_4 <= 180) {
    int command = (int)hw_value_io_4;
    if (command != hw_servo_previous_io_4) {
      hw_servo_io_4.write(command); hw_servo_previous_io_4 = command;
    }
  }
  hw_servo_io_5.attach(6);
  if (isfinite((double)hw_value_io_5) && hw_value_io_5 >= 0 && hw_value_io_5 <= 180) {
    int command = (int)hw_value_io_5;
    if (command != hw_servo_previous_io_5) {
      hw_servo_io_5.write(command); hw_servo_previous_io_5 = command;
    }
  }
  hw_servo_io_6.attach(7);
  if (isfinite((double)hw_value_io_6) && hw_value_io_6 >= 0 && hw_value_io_6 <= 180) {
    int command = (int)hw_value_io_6;
    if (command != hw_servo_previous_io_6) {
      hw_servo_io_6.write(command); hw_servo_previous_io_6 = command;
    }
  }
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_7 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_7, &hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6);
  digitalWrite(2, hw_value_io_0 ? HIGH : LOW);
  digitalWrite(3, hw_value_io_1 ? HIGH : LOW);
  digitalWrite(4, hw_value_io_2 ? HIGH : LOW);
  if (hw_value_io_3 != hw_shift595_previous_io_3) {
    hw_shift595_io_3(hw_value_io_3); hw_shift595_previous_io_3 = hw_value_io_3;
  }
  if (isfinite((double)hw_value_io_4) && hw_value_io_4 >= 0 && hw_value_io_4 <= 180) {
    int command = (int)hw_value_io_4;
    if (command != hw_servo_previous_io_4) {
      hw_servo_io_4.write(command); hw_servo_previous_io_4 = command;
    }
  }
  if (isfinite((double)hw_value_io_5) && hw_value_io_5 >= 0 && hw_value_io_5 <= 180) {
    int command = (int)hw_value_io_5;
    if (command != hw_servo_previous_io_5) {
      hw_servo_io_5.write(command); hw_servo_previous_io_5 = command;
    }
  }
  if (isfinite((double)hw_value_io_6) && hw_value_io_6 >= 0 && hw_value_io_6 <= 180) {
    int command = (int)hw_value_io_6;
    if (command != hw_servo_previous_io_6) {
      hw_servo_io_6.write(command); hw_servo_previous_io_6 = command;
    }
  }
}

