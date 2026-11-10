#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_119\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
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
extern double sysml_state_servo1Angle;
/* Global Variable define of  */
extern double sysml_state_servo2Angle;
/* Global Variable define of  */
extern bool sysml_state_ledOn;
/* Global Variable define of  */
extern bool sysml_state_previousBtn1;
/* Global Variable define of  */
extern bool sysml_state_previousBtn2;
/* Global Variable define of  */
extern bool sysml_state_btn1Latch;
/* Global Variable define of  */
extern bool sysml_state_btn2Latch;
/* Global Variable define of  */
extern int32_t sysml_state_maintainUntilMs;
/* Global Variable define of  */
extern int32_t sysml_state_flashAccumulatedMs;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_3, bool io_4, int32_t io_5, double* io_0, double* io_1, bool* io_2);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_mode = 0;
/* Global Variable define of  */
double sysml_state_servo1Angle = 0.0;
/* Global Variable define of  */
double sysml_state_servo2Angle = 0.0;
/* Global Variable define of  */
bool sysml_state_ledOn = false;
/* Global Variable define of  */
bool sysml_state_previousBtn1 = false;
/* Global Variable define of  */
bool sysml_state_previousBtn2 = false;
/* Global Variable define of  */
bool sysml_state_btn1Latch = false;
/* Global Variable define of  */
bool sysml_state_btn2Latch = false;
/* Global Variable define of  */
int32_t sysml_state_maintainUntilMs = 0;
/* Global Variable define of  */
int32_t sysml_state_flashAccumulatedMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
#line 323 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_119\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 345 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_119\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 65 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_119\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_mode = 0;
    sysml_state_servo1Angle = 0.0;
    sysml_state_servo2Angle = 0.0;
    sysml_state_ledOn = false;
    sysml_state_previousBtn1 = false;
    sysml_state_previousBtn2 = false;
    sysml_state_btn1Latch = false;
    sysml_state_btn2Latch = false;
    sysml_state_maintainUntilMs = 0;
    sysml_state_flashAccumulatedMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_3, bool io_4, int32_t io_5, double* io_0, double* io_1, bool* io_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    int32_t sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_conditional_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_lazy_10 = 0;
    bool sysml_lazy_11 = 0;
    bool sysml_lazy_12 = 0;
    bool sysml_lazy_13 = 0;
    sysml_lazy_0 = sysml_state_btn1Latch;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_btn2Latch;
    }
    sysml_lazy_1 = sysml_lazy_0;
    if (sysml_lazy_1) {
        sysml_lazy_2 = sysml_state_btn2Latch;
        if (sysml_lazy_2) {
            sysml_lazy_2 = !sysml_state_btn1Latch;
        }
        sysml_lazy_1 = !sysml_lazy_2;
    }
    if (sysml_lazy_1) {
        if ((sysml_state_maintainUntilMs <= 0)) {
            sysml_state_mode = 1;
            sysml_state_servo1Angle = 90.0;
            sysml_state_servo2Angle = 0.0;
            sysml_state_ledOn = true;
        }
    }
    sysml_lazy_3 = sysml_state_btn2Latch;
    if (sysml_lazy_3) {
        sysml_lazy_3 = !sysml_state_btn1Latch;
    }
    if (sysml_lazy_3) {
        if ((sysml_state_maintainUntilMs <= 0)) {
            sysml_state_mode = 1;
            sysml_state_servo1Angle = 0.0;
            sysml_state_servo2Angle = 180.0;
            sysml_state_ledOn = false;
        }
    }
    sysml_lazy_4 = sysml_state_btn1Latch;
    if (sysml_lazy_4) {
        sysml_lazy_4 = sysml_state_btn2Latch;
    }
    if (sysml_lazy_4) {
        if ((sysml_state_maintainUntilMs <= 0)) {
            sysml_state_mode = 2;
            sysml_state_servo1Angle = 0.0;
            sysml_state_servo2Angle = 0.0;
        }
    }
    if (sysml_state_maintainUntilMs > 0) {
        sysml_conditional_5 = sysml_state_maintainUntilMs - io_5;
    }
    else {
        sysml_conditional_5 = 0;
    }
    sysml_state_maintainUntilMs = sysml_conditional_5;
    if ((sysml_state_mode == 0)) {
        sysml_state_servo1Angle = 0.0;
        sysml_state_servo2Angle = 0.0;
        sysml_state_ledOn = false;
        sysml_state_flashAccumulatedMs = 0;
    }
    if ((sysml_state_mode == 1)) {
        sysml_lazy_6 = sysml_state_btn1Latch;
        if (sysml_lazy_6) {
            sysml_lazy_6 = !sysml_state_btn2Latch;
        }
        if (sysml_lazy_6) {
            sysml_state_servo1Angle = 90.0;
            sysml_state_servo2Angle = 0.0;
            sysml_state_ledOn = true;
        }
        else {
            sysml_lazy_7 = sysml_state_btn2Latch;
            if (sysml_lazy_7) {
                sysml_lazy_7 = !sysml_state_btn1Latch;
            }
            if (sysml_lazy_7) {
                sysml_state_servo1Angle = 0.0;
                sysml_state_servo2Angle = 180.0;
                sysml_state_ledOn = false;
            }
            else {
                sysml_state_servo1Angle = 0.0;
                sysml_state_servo2Angle = 0.0;
                sysml_state_ledOn = false;
            }
        }
    }
    if ((sysml_state_mode == 2)) {
        sysml_state_servo1Angle = 0.0;
        sysml_state_servo2Angle = 0.0;
        sysml_state_flashAccumulatedMs = (sysml_state_flashAccumulatedMs + io_5);
        if ((sysml_state_flashAccumulatedMs >= 4000)) {
            sysml_state_flashAccumulatedMs = (sysml_state_flashAccumulatedMs - 4000);
        }
        if (sysml_state_flashAccumulatedMs < 2000) {
            sysml_conditional_8 = true;
        }
        else {
            sysml_conditional_8 = false;
        }
        sysml_state_ledOn = sysml_conditional_8;
    }
    sysml_lazy_9 = io_3;
    if (sysml_lazy_9) {
        sysml_lazy_9 = !sysml_state_previousBtn1;
    }
    if (sysml_lazy_9) {
        if (io_4) {
            sysml_state_btn1Latch = true;
            sysml_state_btn2Latch = true;
            sysml_state_mode = 2;
            sysml_state_servo1Angle = 0.0;
            sysml_state_servo2Angle = 0.0;
            sysml_state_ledOn = true;
            sysml_state_flashAccumulatedMs = 0;
            sysml_state_maintainUntilMs = 2000;
        }
        else {
            sysml_state_btn1Latch = true;
            sysml_state_mode = 1;
            sysml_state_servo1Angle = 90.0;
            sysml_state_servo2Angle = 0.0;
            sysml_state_ledOn = true;
            sysml_state_flashAccumulatedMs = 0;
            sysml_state_maintainUntilMs = 2000;
        }
    }
    sysml_lazy_10 = io_4;
    if (sysml_lazy_10) {
        sysml_lazy_10 = !sysml_state_previousBtn2;
    }
    if (sysml_lazy_10) {
        if (io_3) {
            sysml_state_btn1Latch = true;
            sysml_state_btn2Latch = true;
            sysml_state_mode = 2;
            sysml_state_servo1Angle = 0.0;
            sysml_state_servo2Angle = 0.0;
            sysml_state_ledOn = true;
            sysml_state_flashAccumulatedMs = 0;
            sysml_state_maintainUntilMs = 2000;
        }
        else {
            sysml_state_btn2Latch = true;
            sysml_state_mode = 1;
            sysml_state_servo1Angle = 0.0;
            sysml_state_servo2Angle = 180.0;
            sysml_state_ledOn = false;
            sysml_state_flashAccumulatedMs = 0;
            sysml_state_maintainUntilMs = 2000;
        }
    }
    if ((sysml_state_maintainUntilMs <= 0)) {
        sysml_lazy_11 = sysml_state_btn1Latch;
        if (sysml_lazy_11) {
            sysml_lazy_11 = sysml_state_btn2Latch;
        }
        if (sysml_lazy_11) {
            sysml_state_mode = 2;
            sysml_state_servo1Angle = 0.0;
            sysml_state_servo2Angle = 0.0;
        }
        else {
            sysml_lazy_12 = sysml_state_btn1Latch;
            if (sysml_lazy_12) {
                sysml_lazy_12 = !sysml_state_btn2Latch;
            }
            if (sysml_lazy_12) {
                sysml_state_mode = 1;
                sysml_state_servo1Angle = 90.0;
                sysml_state_servo2Angle = 0.0;
                sysml_state_ledOn = true;
            }
            else {
                sysml_lazy_13 = sysml_state_btn2Latch;
                if (sysml_lazy_13) {
                    sysml_lazy_13 = !sysml_state_btn1Latch;
                }
                if (sysml_lazy_13) {
                    sysml_state_mode = 1;
                    sysml_state_servo1Angle = 0.0;
                    sysml_state_servo2Angle = 180.0;
                    sysml_state_ledOn = false;
                }
                else {
                    sysml_state_mode = 0;
                    sysml_state_servo1Angle = 0.0;
                    sysml_state_servo2Angle = 0.0;
                    sysml_state_ledOn = false;
                    sysml_state_flashAccumulatedMs = 0;
                }
            }
        }
    }
    sysml_state_previousBtn1 = io_3;
    sysml_state_previousBtn2 = io_4;
    double sysml_value_0 = 0;
    sysml_value_0 = sysml_state_servo1Angle;
    double sysml_value_1 = 0;
    sysml_value_1 = sysml_state_servo2Angle;
    bool sysml_value_2 = 0;
    sysml_value_2 = sysml_state_ledOn;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
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
static double hw_value_io_0 = 0;
Servo hw_servo_io_0;
static int hw_servo_previous_io_0 = -1;
static double hw_value_io_1 = 0;
Servo hw_servo_io_1;
static int hw_servo_previous_io_1 = -1;
static bool hw_value_io_2 = false;
static bool hw_value_io_3 = false;
static uint32_t hw_sampled_io_3 = 0;
static bool hw_attempted_io_3 = false;
static bool hw_value_io_4 = false;
static uint32_t hw_sampled_io_4 = 0;
static bool hw_attempted_io_4 = false;
static int32_t hw_value_io_5 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  hw_servo_io_0.attach(5);
  if (isfinite((double)hw_value_io_0) && hw_value_io_0 >= 0 && hw_value_io_0 <= 180) {
    int command = (int)hw_value_io_0;
    if (command != hw_servo_previous_io_0) {
      hw_servo_io_0.write(command); hw_servo_previous_io_0 = command;
    }
  }
  hw_servo_io_1.attach(6);
  if (isfinite((double)hw_value_io_1) && hw_value_io_1 >= 0 && hw_value_io_1 <= 180) {
    int command = (int)hw_value_io_1;
    if (command != hw_servo_previous_io_1) {
      hw_servo_io_1.write(command); hw_servo_previous_io_1 = command;
    }
  }
  pinMode(2, OUTPUT);
  digitalWrite(2, hw_value_io_2 ? HIGH : LOW);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_3 || (uint32_t)(hw_now - hw_sampled_io_3) >= 10UL) {
    hw_attempted_io_3 = true; hw_sampled_io_3 = hw_now;
    hw_value_io_3 = (digitalRead(3) == HIGH);
  }
  if (!hw_attempted_io_4 || (uint32_t)(hw_now - hw_sampled_io_4) >= 10UL) {
    hw_attempted_io_4 = true; hw_sampled_io_4 = hw_now;
    hw_value_io_4 = (digitalRead(4) == HIGH);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_5 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_3, hw_value_io_4, hw_value_io_5, &hw_value_io_0, &hw_value_io_1, &hw_value_io_2);
  if (isfinite((double)hw_value_io_0) && hw_value_io_0 >= 0 && hw_value_io_0 <= 180) {
    int command = (int)hw_value_io_0;
    if (command != hw_servo_previous_io_0) {
      hw_servo_io_0.write(command); hw_servo_previous_io_0 = command;
    }
  }
  if (isfinite((double)hw_value_io_1) && hw_value_io_1 >= 0 && hw_value_io_1 <= 180) {
    int command = (int)hw_value_io_1;
    if (command != hw_servo_previous_io_1) {
      hw_servo_io_1.write(command); hw_servo_previous_io_1 = command;
    }
  }
  digitalWrite(2, hw_value_io_2 ? HIGH : LOW);
}

