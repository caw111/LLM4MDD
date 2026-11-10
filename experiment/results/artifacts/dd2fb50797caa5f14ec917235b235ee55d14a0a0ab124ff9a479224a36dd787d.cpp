#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_71\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
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
extern bool sysml_state_previousBtn1Pressed;
/* Global Variable define of  */
extern bool sysml_state_previousBtn2Pressed;
/* Global Variable define of  */
extern bool sysml_state_btn1StablePressed;
/* Global Variable define of  */
extern bool sysml_state_btn2StablePressed;
/* Global Variable define of  */
extern int32_t sysml_state_btn1DebounceCounter;
/* Global Variable define of  */
extern int32_t sysml_state_btn2DebounceCounter;
/* Global Variable define of  */
extern double sysml_state_servo1Angle;
/* Global Variable define of  */
extern double sysml_state_servo2Angle;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double io_2, double io_3, double* io_4, double* io_5);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousBtn1Pressed = false;
/* Global Variable define of  */
bool sysml_state_previousBtn2Pressed = false;
/* Global Variable define of  */
bool sysml_state_btn1StablePressed = false;
/* Global Variable define of  */
bool sysml_state_btn2StablePressed = false;
/* Global Variable define of  */
int32_t sysml_state_btn1DebounceCounter = 0;
/* Global Variable define of  */
int32_t sysml_state_btn2DebounceCounter = 0;
/* Global Variable define of  */
double sysml_state_servo1Angle = 0.0;
/* Global Variable define of  */
double sysml_state_servo2Angle = 0.0;
/* Init function of CompositeActor named "ProgramPage1" */
#line 210 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_71\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void setup();
#line 232 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_71\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void loop();
#line 57 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_71\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousBtn1Pressed = false;
    sysml_state_previousBtn2Pressed = false;
    sysml_state_btn1StablePressed = false;
    sysml_state_btn2StablePressed = false;
    sysml_state_btn1DebounceCounter = 0;
    sysml_state_btn2DebounceCounter = 0;
    sysml_state_servo1Angle = 0.0;
    sysml_state_servo2Angle = 0.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double io_2, double io_3, double* io_4, double* io_5) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    double sysml_conditional_4 = 0;
    double sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    bool sysml_lazy_9 = 0;
    double sysml_conditional_10 = 0;
    double sysml_conditional_11 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousBtn1Pressed;
    }
    if (sysml_lazy_0) {
        sysml_state_btn1DebounceCounter = 5;
    }
    sysml_lazy_1 = io_0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = sysml_state_btn1DebounceCounter > 0;
    }
    if (sysml_lazy_1) {
        sysml_state_btn1DebounceCounter = (sysml_state_btn1DebounceCounter - 1);
    }
    if ((!io_0)) {
        sysml_state_btn1DebounceCounter = 0;
        sysml_state_btn1StablePressed = false;
    }
    sysml_lazy_2 = io_0;
    if (sysml_lazy_2) {
        sysml_lazy_2 = sysml_state_btn1DebounceCounter == 0;
    }
    if (sysml_lazy_2) {
        sysml_state_btn1StablePressed = true;
    }
    sysml_lazy_3 = sysml_state_btn1StablePressed;
    if (sysml_lazy_3) {
        sysml_lazy_3 = !sysml_state_previousBtn1Pressed;
    }
    if (sysml_lazy_3) {
        if (io_2 <= 0.0) {
            sysml_conditional_4 = 0.0;
        }
        else {
            if (io_2 >= 1023.0) {
                sysml_conditional_5 = 180.0;
            }
            else {
                sysml_conditional_5 = ((double)(io_2 * 180.0)) / 1023.0;
            }
            sysml_conditional_4 = sysml_conditional_5;
        }
        sysml_state_servo1Angle = sysml_conditional_4;
    }
    sysml_state_previousBtn1Pressed = io_0;
    sysml_lazy_6 = io_1;
    if (sysml_lazy_6) {
        sysml_lazy_6 = !sysml_state_previousBtn2Pressed;
    }
    if (sysml_lazy_6) {
        sysml_state_btn2DebounceCounter = 5;
    }
    sysml_lazy_7 = io_1;
    if (sysml_lazy_7) {
        sysml_lazy_7 = sysml_state_btn2DebounceCounter > 0;
    }
    if (sysml_lazy_7) {
        sysml_state_btn2DebounceCounter = (sysml_state_btn2DebounceCounter - 1);
    }
    if ((!io_1)) {
        sysml_state_btn2DebounceCounter = 0;
        sysml_state_btn2StablePressed = false;
    }
    sysml_lazy_8 = io_1;
    if (sysml_lazy_8) {
        sysml_lazy_8 = sysml_state_btn2DebounceCounter == 0;
    }
    if (sysml_lazy_8) {
        sysml_state_btn2StablePressed = true;
    }
    sysml_lazy_9 = sysml_state_btn2StablePressed;
    if (sysml_lazy_9) {
        sysml_lazy_9 = !sysml_state_previousBtn2Pressed;
    }
    if (sysml_lazy_9) {
        if (io_3 <= 0.0) {
            sysml_conditional_10 = 0.0;
        }
        else {
            if (io_3 >= 1023.0) {
                sysml_conditional_11 = 180.0;
            }
            else {
                sysml_conditional_11 = ((double)(io_3 * 180.0)) / 1023.0;
            }
            sysml_conditional_10 = sysml_conditional_11;
        }
        sysml_state_servo2Angle = sysml_conditional_10;
    }
    sysml_state_previousBtn2Pressed = io_1;
    double sysml_value_0 = 0;
    sysml_value_0 = sysml_state_servo1Angle;
    double sysml_value_1 = 0;
    sysml_value_1 = sysml_state_servo2Angle;
    *io_4 = sysml_value_0;
    *io_5 = sysml_value_1;
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
static double hw_value_io_4 = 0;
Servo hw_servo_io_4;
static int hw_servo_previous_io_4 = -1;
static double hw_value_io_5 = 0;
Servo hw_servo_io_5;
static int hw_servo_previous_io_5 = -1;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  hw_servo_io_4.attach(4);
  if (isfinite((double)hw_value_io_4) && hw_value_io_4 >= 0 && hw_value_io_4 <= 180) {
    int command = (int)hw_value_io_4;
    if (command != hw_servo_previous_io_4) {
      hw_servo_io_4.write(command); hw_servo_previous_io_4 = command;
    }
  }
  hw_servo_io_5.attach(5);
  if (isfinite((double)hw_value_io_5) && hw_value_io_5 >= 0 && hw_value_io_5 <= 180) {
    int command = (int)hw_value_io_5;
    if (command != hw_servo_previous_io_5) {
      hw_servo_io_5.write(command); hw_servo_previous_io_5 = command;
    }
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
  if (!hw_attempted_io_2 || (uint32_t)(hw_now - hw_sampled_io_2) >= 5UL) {
    hw_attempted_io_2 = true; hw_sampled_io_2 = hw_now;
    hw_value_io_2 = (analogRead(A3) * 1 + 0.0);
  }
  if (!hw_attempted_io_3 || (uint32_t)(hw_now - hw_sampled_io_3) >= 5UL) {
    hw_attempted_io_3 = true; hw_sampled_io_3 = hw_now;
    hw_value_io_3 = (analogRead(A4) * 1 + 0.0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, hw_value_io_3, &hw_value_io_4, &hw_value_io_5);
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
}

