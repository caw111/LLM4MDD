#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_103\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
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
extern int32_t sysml_state_ledPhaseMs;
/* Global Variable define of  */
extern int32_t sysml_state_sevsegPhaseMs;
/* Global Variable define of  */
extern int32_t sysml_state_sevsegDigit;
/* Global Variable define of  */
extern int32_t sysml_state_servo1PhaseMs;
/* Global Variable define of  */
extern int32_t sysml_state_servo2PhaseMs;
/* Global Variable define of  */
extern bool sysml_state_servo1AtEndpoint;
/* Global Variable define of  */
extern bool sysml_state_servo2AtEndpoint;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t elapsedMilliseconds, bool* io_0, bool* io_1, int32_t* io_2, double* io_3, double* io_4);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_ledPhaseMs = 0;
/* Global Variable define of  */
int32_t sysml_state_sevsegPhaseMs = 0;
/* Global Variable define of  */
int32_t sysml_state_sevsegDigit = 0;
/* Global Variable define of  */
int32_t sysml_state_servo1PhaseMs = 0;
/* Global Variable define of  */
int32_t sysml_state_servo2PhaseMs = 0;
/* Global Variable define of  */
bool sysml_state_servo1AtEndpoint = false;
/* Global Variable define of  */
bool sysml_state_servo2AtEndpoint = true;
/* Init function of CompositeActor named "ProgramPage1" */
#line 227 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_103\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
static void hw_shift595_io_2(int32_t value);
#line 255 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_103\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 283 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_103\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 53 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_103\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_ledPhaseMs = 0;
    sysml_state_sevsegPhaseMs = 0;
    sysml_state_sevsegDigit = 0;
    sysml_state_servo1PhaseMs = 0;
    sysml_state_servo2PhaseMs = 0;
    sysml_state_servo1AtEndpoint = false;
    sysml_state_servo2AtEndpoint = true;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t elapsedMilliseconds, bool* io_0, bool* io_1, int32_t* io_2, double* io_3, double* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int32_t sysml_conditional_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_conditional_5 = 0;
    bool sysml_conditional_6 = 0;
    int32_t sysml_conditional_7 = 0;
    double sysml_conditional_8 = 0;
    double sysml_conditional_9 = 0;
    double sysml_conditional_10 = 0;
    double sysml_conditional_11 = 0;
    double sysml_conditional_12 = 0;
    double sysml_conditional_13 = 0;
    double sysml_conditional_14 = 0;
    double sysml_conditional_15 = 0;
    sysml_state_ledPhaseMs = (sysml_state_ledPhaseMs + elapsedMilliseconds);
    if ((sysml_state_ledPhaseMs >= 2000)) {
        sysml_state_ledPhaseMs = (sysml_state_ledPhaseMs - 2000);
    }
    sysml_state_sevsegPhaseMs = (sysml_state_sevsegPhaseMs + elapsedMilliseconds);
    if ((sysml_state_sevsegPhaseMs >= 2000)) {
        sysml_state_sevsegPhaseMs = (sysml_state_sevsegPhaseMs - 2000);
        if (sysml_state_sevsegDigit == 9) {
            sysml_conditional_0 = 0;
        }
        else {
            sysml_conditional_0 = sysml_state_sevsegDigit + 1;
        }
        sysml_state_sevsegDigit = sysml_conditional_0;
    }
    sysml_state_servo1PhaseMs = (sysml_state_servo1PhaseMs + elapsedMilliseconds);
    sysml_state_servo2PhaseMs = (sysml_state_servo2PhaseMs + elapsedMilliseconds);
    sysml_lazy_1 = sysml_state_servo1AtEndpoint;
    if (sysml_lazy_1) {
        sysml_lazy_1 = sysml_state_servo1PhaseMs >= 2000;
    }
    if (sysml_lazy_1) {
        sysml_state_servo1PhaseMs = (sysml_state_servo1PhaseMs - 2000);
        sysml_state_servo1AtEndpoint = false;
    }
    sysml_lazy_2 = !sysml_state_servo1AtEndpoint;
    if (sysml_lazy_2) {
        sysml_lazy_2 = sysml_state_servo1PhaseMs >= 2000;
    }
    if (sysml_lazy_2) {
        sysml_state_servo1PhaseMs = (sysml_state_servo1PhaseMs - 2000);
        sysml_state_servo1AtEndpoint = true;
    }
    sysml_lazy_3 = sysml_state_servo2AtEndpoint;
    if (sysml_lazy_3) {
        sysml_lazy_3 = sysml_state_servo2PhaseMs >= 2000;
    }
    if (sysml_lazy_3) {
        sysml_state_servo2PhaseMs = (sysml_state_servo2PhaseMs - 2000);
        sysml_state_servo2AtEndpoint = false;
    }
    sysml_lazy_4 = !sysml_state_servo2AtEndpoint;
    if (sysml_lazy_4) {
        sysml_lazy_4 = sysml_state_servo2PhaseMs >= 2000;
    }
    if (sysml_lazy_4) {
        sysml_state_servo2PhaseMs = (sysml_state_servo2PhaseMs - 2000);
        sysml_state_servo2AtEndpoint = true;
    }
    bool sysml_value_0 = 0;
    if (sysml_state_ledPhaseMs >= 1000) {
        sysml_conditional_5 = true;
    }
    else {
        sysml_conditional_5 = false;
    }
    sysml_value_0 = sysml_conditional_5;
    bool sysml_value_1 = 0;
    if (sysml_state_ledPhaseMs >= 1000) {
        sysml_conditional_6 = false;
    }
    else {
        sysml_conditional_6 = true;
    }
    sysml_value_1 = sysml_conditional_6;
    int32_t sysml_value_2 = 0;
    if (sysml_state_sevsegDigit >= 0) {
        sysml_conditional_7 = sysml_state_sevsegDigit;
    }
    else {
        sysml_conditional_7 = 0;
    }
    sysml_value_2 = sysml_conditional_7;
    double sysml_value_3 = 0;
    if (sysml_state_servo1AtEndpoint) {
        if (sysml_state_servo1PhaseMs < 1) {
            sysml_conditional_9 = 0.0;
        }
        else {
            sysml_conditional_9 = 180.0;
        }
        sysml_conditional_8 = sysml_conditional_9;
    }
    else {
        if (sysml_state_servo1PhaseMs < 1000) {
            if (sysml_state_servo1PhaseMs == 0) {
                sysml_conditional_11 = 0.0;
            }
            else {
                sysml_conditional_11 = (((double)sysml_state_servo1PhaseMs) / 1000.0) * 180.0;
            }
            sysml_conditional_10 = sysml_conditional_11;
        }
        else {
            sysml_conditional_10 = (((double)(sysml_state_servo1PhaseMs - 1000)) / 1000.0) * 180.0;
        }
        sysml_conditional_8 = sysml_conditional_10;
    }
    sysml_value_3 = sysml_conditional_8;
    double sysml_value_4 = 0;
    if (sysml_state_servo2AtEndpoint) {
        if (sysml_state_servo2PhaseMs < 1) {
            sysml_conditional_13 = 180.0;
        }
        else {
            sysml_conditional_13 = 0.0;
        }
        sysml_conditional_12 = sysml_conditional_13;
    }
    else {
        if (sysml_state_servo2PhaseMs < 1000) {
            if (sysml_state_servo2PhaseMs == 0) {
                sysml_conditional_15 = 180.0;
            }
            else {
                sysml_conditional_15 = 180.0 - ((((double)sysml_state_servo2PhaseMs) / 1000.0) * 180.0);
            }
            sysml_conditional_14 = sysml_conditional_15;
        }
        else {
            sysml_conditional_14 = 180.0 - ((((double)(sysml_state_servo2PhaseMs - 1000)) / 1000.0) * 180.0);
        }
        sysml_conditional_12 = sysml_conditional_14;
    }
    sysml_value_4 = sysml_conditional_12;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
    *io_4 = sysml_value_4;
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
static bool hw_value_io_1 = true;
static int32_t hw_value_io_2 = 0;
static void hw_shift595_io_2(int32_t value) {
  static const uint8_t digits[10] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
  bool valid = value >= 0 && value < 10L;
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
  digitalWrite(A1, HIGH);
}
static int32_t hw_shift595_previous_io_2 = INT32_MIN;
static double hw_value_io_3 = 0;
Servo hw_servo_io_3;
static int hw_servo_previous_io_3 = -1;
static double hw_value_io_4 = 180;
Servo hw_servo_io_4;
static int hw_servo_previous_io_4 = -1;
static int32_t hw_value_elapsedMilliseconds = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, OUTPUT);
  digitalWrite(2, hw_value_io_0 ? HIGH : LOW);
  pinMode(3, OUTPUT);
  digitalWrite(3, hw_value_io_1 ? HIGH : LOW);
  pinMode(A0, OUTPUT); digitalWrite(A0, LOW);
  pinMode(A2, OUTPUT); digitalWrite(A2, LOW);
  pinMode(A1, OUTPUT); digitalWrite(A1, LOW);
  if (hw_value_io_2 != hw_shift595_previous_io_2) {
    hw_shift595_io_2(hw_value_io_2); hw_shift595_previous_io_2 = hw_value_io_2;
  }
  hw_servo_io_3.attach(4);
  if (isfinite((double)hw_value_io_3) && hw_value_io_3 >= 0 && hw_value_io_3 <= 180) {
    int command = (int)hw_value_io_3;
    if (command != hw_servo_previous_io_3) {
      hw_servo_io_3.write(command); hw_servo_previous_io_3 = command;
    }
  }
  hw_servo_io_4.attach(5);
  if (isfinite((double)hw_value_io_4) && hw_value_io_4 >= 0 && hw_value_io_4 <= 180) {
    int command = (int)hw_value_io_4;
    if (command != hw_servo_previous_io_4) {
      hw_servo_io_4.write(command); hw_servo_previous_io_4 = command;
    }
  }
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_elapsedMilliseconds = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_elapsedMilliseconds, &hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4);
  digitalWrite(2, hw_value_io_0 ? HIGH : LOW);
  digitalWrite(3, hw_value_io_1 ? HIGH : LOW);
  if (hw_value_io_2 != hw_shift595_previous_io_2) {
    hw_shift595_io_2(hw_value_io_2); hw_shift595_previous_io_2 = hw_value_io_2;
  }
  if (isfinite((double)hw_value_io_3) && hw_value_io_3 >= 0 && hw_value_io_3 <= 180) {
    int command = (int)hw_value_io_3;
    if (command != hw_servo_previous_io_3) {
      hw_servo_io_3.write(command); hw_servo_previous_io_3 = command;
    }
  }
  if (isfinite((double)hw_value_io_4) && hw_value_io_4 >= 0 && hw_value_io_4 <= 180) {
    int command = (int)hw_value_io_4;
    if (command != hw_servo_previous_io_4) {
      hw_servo_io_4.write(command); hw_servo_previous_io_4 = command;
    }
  }
}

