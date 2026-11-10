#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_119\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
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
extern bool sysml_state_previousBtn1;
/* Global Variable define of  */
extern bool sysml_state_previousBtn2;
/* Global Variable define of  */
extern int32_t sysml_state_mode;
/* Global Variable define of  */
extern int32_t sysml_state_maintenanceElapsedMs;
/* Global Variable define of  */
extern int32_t sysml_state_flashAccumMs;
/* Global Variable define of  */
extern bool sysml_state_ledFlashOn;
/* Global Variable define of  */
extern int32_t sysml_state_btn1HoldMs;
/* Global Variable define of  */
extern int32_t sysml_state_btn2HoldMs;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_3, bool io_4, int32_t io_5, double* io_0, double* io_1, bool* io_2);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousBtn1 = false;
/* Global Variable define of  */
bool sysml_state_previousBtn2 = false;
/* Global Variable define of  */
int32_t sysml_state_mode = 0;
/* Global Variable define of  */
int32_t sysml_state_maintenanceElapsedMs = 0;
/* Global Variable define of  */
int32_t sysml_state_flashAccumMs = 0;
/* Global Variable define of  */
bool sysml_state_ledFlashOn = false;
/* Global Variable define of  */
int32_t sysml_state_btn1HoldMs = 0;
/* Global Variable define of  */
int32_t sysml_state_btn2HoldMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
#line 305 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_119\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void setup();
#line 327 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_119\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void loop();
#line 57 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_119\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousBtn1 = false;
    sysml_state_previousBtn2 = false;
    sysml_state_mode = 0;
    sysml_state_maintenanceElapsedMs = 0;
    sysml_state_flashAccumMs = 0;
    sysml_state_ledFlashOn = false;
    sysml_state_btn1HoldMs = 0;
    sysml_state_btn2HoldMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_3, bool io_4, int32_t io_5, double* io_0, double* io_1, bool* io_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_lazy_10 = 0;
    bool sysml_lazy_11 = 0;
    double sysml_conditional_12 = 0;
    double sysml_conditional_13 = 0;
    double sysml_conditional_14 = 0;
    double sysml_conditional_15 = 0;
    bool sysml_conditional_16 = 0;
    bool sysml_conditional_17 = 0;
    sysml_lazy_0 = io_3;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousBtn1;
    }
    if (sysml_lazy_0) {
        if (io_4) {
            sysml_state_mode = 2;
            sysml_state_maintenanceElapsedMs = 0;
            sysml_state_flashAccumMs = 0;
            sysml_state_ledFlashOn = true;
            sysml_state_btn1HoldMs = 0;
            sysml_state_btn2HoldMs = 0;
        }
    }
    sysml_lazy_1 = io_4;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_previousBtn2;
    }
    if (sysml_lazy_1) {
        if (io_3) {
            sysml_state_mode = 2;
            sysml_state_maintenanceElapsedMs = 0;
            sysml_state_flashAccumMs = 0;
            sysml_state_ledFlashOn = true;
            sysml_state_btn1HoldMs = 0;
            sysml_state_btn2HoldMs = 0;
        }
        else {
            sysml_state_mode = 1;
            sysml_state_maintenanceElapsedMs = 0;
            sysml_state_flashAccumMs = 0;
            sysml_state_ledFlashOn = false;
            sysml_state_btn1HoldMs = 0;
            sysml_state_btn2HoldMs = 0;
        }
    }
    else {
        sysml_lazy_2 = !io_3;
        if (sysml_lazy_2) {
            sysml_lazy_2 = io_4;
        }
        sysml_lazy_3 = sysml_lazy_2;
        if (sysml_lazy_3) {
            sysml_lazy_3 = !sysml_state_previousBtn2;
        }
        if (sysml_lazy_3) {
            sysml_state_mode = 1;
            sysml_state_maintenanceElapsedMs = 0;
            sysml_state_flashAccumMs = 0;
            sysml_state_ledFlashOn = false;
            sysml_state_btn1HoldMs = 0;
            sysml_state_btn2HoldMs = 0;
        }
    }
    sysml_lazy_4 = io_3;
    if (sysml_lazy_4) {
        sysml_lazy_4 = !sysml_state_previousBtn1;
    }
    sysml_lazy_5 = sysml_lazy_4;
    if (sysml_lazy_5) {
        sysml_lazy_5 = !io_4;
    }
    if (sysml_lazy_5) {
        sysml_state_mode = 0;
        sysml_state_maintenanceElapsedMs = 0;
        sysml_state_flashAccumMs = 0;
        sysml_state_ledFlashOn = true;
        sysml_state_btn1HoldMs = 0;
        sysml_state_btn2HoldMs = 0;
    }
    sysml_lazy_6 = sysml_state_mode == 2;
    if (sysml_lazy_6) {
        sysml_lazy_7 = io_3;
        if (!sysml_lazy_7) {
            sysml_lazy_7 = io_4;
        }
        sysml_lazy_6 = sysml_lazy_7;
    }
    if (sysml_lazy_6) {
        sysml_lazy_8 = io_3;
        if (sysml_lazy_8) {
            sysml_lazy_8 = !io_4;
        }
        if (sysml_lazy_8) {
            sysml_state_mode = 0;
            sysml_state_maintenanceElapsedMs = 0;
            sysml_state_flashAccumMs = 0;
            sysml_state_ledFlashOn = true;
            sysml_state_btn1HoldMs = 0;
            sysml_state_btn2HoldMs = 0;
        }
        else {
            sysml_lazy_9 = io_4;
            if (sysml_lazy_9) {
                sysml_lazy_9 = !io_3;
            }
            if (sysml_lazy_9) {
                sysml_state_mode = 1;
                sysml_state_maintenanceElapsedMs = 0;
                sysml_state_flashAccumMs = 0;
                sysml_state_ledFlashOn = false;
                sysml_state_btn1HoldMs = 0;
                sysml_state_btn2HoldMs = 0;
            }
        }
    }
    if ((sysml_state_mode == 0)) {
        sysml_state_maintenanceElapsedMs = (sysml_state_maintenanceElapsedMs + io_5);
        sysml_state_btn1HoldMs = (sysml_state_btn1HoldMs + io_5);
        sysml_state_btn2HoldMs = 0;
    }
    else {
        if ((sysml_state_mode == 1)) {
            sysml_state_maintenanceElapsedMs = (sysml_state_maintenanceElapsedMs + io_5);
            sysml_state_btn2HoldMs = (sysml_state_btn2HoldMs + io_5);
            sysml_state_btn1HoldMs = 0;
        }
        else {
            if ((sysml_state_mode == 2)) {
                sysml_state_maintenanceElapsedMs = (sysml_state_maintenanceElapsedMs + io_5);
                sysml_state_flashAccumMs = (sysml_state_flashAccumMs + io_5);
                sysml_lazy_10 = sysml_state_ledFlashOn;
                if (sysml_lazy_10) {
                    sysml_lazy_10 = sysml_state_flashAccumMs >= 2000;
                }
                if (sysml_lazy_10) {
                    sysml_state_flashAccumMs = (sysml_state_flashAccumMs - 2000);
                    sysml_state_ledFlashOn = false;
                }
                sysml_lazy_11 = !sysml_state_ledFlashOn;
                if (sysml_lazy_11) {
                    sysml_lazy_11 = sysml_state_flashAccumMs >= 2000;
                }
                if (sysml_lazy_11) {
                    sysml_state_flashAccumMs = (sysml_state_flashAccumMs - 2000);
                    sysml_state_ledFlashOn = true;
                }
                sysml_state_btn1HoldMs = 0;
                sysml_state_btn2HoldMs = 0;
            }
        }
    }
    sysml_state_previousBtn1 = io_3;
    sysml_state_previousBtn2 = io_4;
    double sysml_value_0 = 0;
    if (sysml_state_mode == 0) {
        sysml_conditional_12 = 90.0;
    }
    else {
        if (sysml_state_mode == 2) {
            sysml_conditional_13 = 0.0;
        }
        else {
            sysml_conditional_13 = 0.0;
        }
        sysml_conditional_12 = sysml_conditional_13;
    }
    sysml_value_0 = sysml_conditional_12;
    double sysml_value_1 = 0;
    if (sysml_state_mode == 1) {
        sysml_conditional_14 = 180.0;
    }
    else {
        if (sysml_state_mode == 2) {
            sysml_conditional_15 = 0.0;
        }
        else {
            sysml_conditional_15 = 0.0;
        }
        sysml_conditional_14 = sysml_conditional_15;
    }
    sysml_value_1 = sysml_conditional_14;
    bool sysml_value_2 = 0;
    if (sysml_state_mode == 0) {
        sysml_conditional_16 = true;
    }
    else {
        if (sysml_state_mode == 1) {
            sysml_conditional_17 = false;
        }
        else {
            sysml_conditional_17 = sysml_state_ledFlashOn;
        }
        sysml_conditional_16 = sysml_conditional_17;
    }
    sysml_value_2 = sysml_conditional_16;
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
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_3 || (uint32_t)(hw_now - hw_sampled_io_3) >= 5UL) {
    hw_attempted_io_3 = true; hw_sampled_io_3 = hw_now;
    hw_value_io_3 = (digitalRead(3) == LOW);
  }
  if (!hw_attempted_io_4 || (uint32_t)(hw_now - hw_sampled_io_4) >= 5UL) {
    hw_attempted_io_4 = true; hw_sampled_io_4 = hw_now;
    hw_value_io_4 = (digitalRead(4) == LOW);
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

