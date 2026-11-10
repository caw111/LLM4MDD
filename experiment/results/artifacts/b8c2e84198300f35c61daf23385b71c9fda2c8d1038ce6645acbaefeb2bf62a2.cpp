#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_110\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
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
extern int32_t sysml_state_elapsedTotalMs;
/* Global Variable define of  */
extern int32_t sysml_state_rainbowIndex;
/* Global Variable define of  */
extern bool sysml_state_ledOn;
/* Global Variable define of  */
extern bool sysml_state_servo1At180;
/* Global Variable define of  */
extern bool sysml_state_servo2At0;
/* Global Variable define of  */
extern bool sysml_state_servo3At135;
/* Global Variable define of  */
extern bool sysml_state_phaseStepDone;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_0, double* io_1, double* io_2, double* io_3, double* io_4, double* io_5, double* io_6, double* io_7, double* io_8, double* io_9, double* io_10);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_elapsedTotalMs = 0;
/* Global Variable define of  */
int32_t sysml_state_rainbowIndex = 0;
/* Global Variable define of  */
bool sysml_state_ledOn = false;
/* Global Variable define of  */
bool sysml_state_servo1At180 = false;
/* Global Variable define of  */
bool sysml_state_servo2At0 = false;
/* Global Variable define of  */
bool sysml_state_servo3At135 = false;
/* Global Variable define of  */
bool sysml_state_phaseStepDone = false;
/* Init function of CompositeActor named "ProgramPage1" */
#line 308 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_110\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void setup();
#line 347 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_110\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void loop();
#line 53 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_110\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_elapsedTotalMs = 0;
    sysml_state_rainbowIndex = 0;
    sysml_state_ledOn = false;
    sysml_state_servo1At180 = false;
    sysml_state_servo2At0 = false;
    sysml_state_servo3At135 = false;
    sysml_state_phaseStepDone = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_0, double* io_1, double* io_2, double* io_3, double* io_4, double* io_5, double* io_6, double* io_7, double* io_8, double* io_9, double* io_10) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int32_t sysml_conditional_1 = 0;
    double sysml_conditional_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    double sysml_conditional_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    bool sysml_lazy_9 = 0;
    double sysml_conditional_10 = 0;
    bool sysml_lazy_11 = 0;
    bool sysml_lazy_12 = 0;
    bool sysml_lazy_13 = 0;
    double sysml_conditional_14 = 0;
    bool sysml_lazy_15 = 0;
    bool sysml_lazy_16 = 0;
    bool sysml_lazy_17 = 0;
    double sysml_conditional_18 = 0;
    bool sysml_lazy_19 = 0;
    bool sysml_lazy_20 = 0;
    bool sysml_lazy_21 = 0;
    double sysml_conditional_22 = 0;
    bool sysml_lazy_23 = 0;
    bool sysml_lazy_24 = 0;
    bool sysml_lazy_25 = 0;
    double sysml_conditional_26 = 0;
    double sysml_conditional_27 = 0;
    double sysml_conditional_28 = 0;
    double sysml_conditional_29 = 0;
    sysml_state_elapsedTotalMs = (sysml_state_elapsedTotalMs + io_0);
    sysml_state_phaseStepDone = (io_0 > (-1));
    sysml_lazy_0 = sysml_state_phaseStepDone;
    if (sysml_lazy_0) {
        sysml_lazy_0 = sysml_state_elapsedTotalMs >= 2000;
    }
    if (sysml_lazy_0) {
        sysml_state_elapsedTotalMs = (sysml_state_elapsedTotalMs - 2000);
        if (sysml_state_rainbowIndex >= 6) {
            sysml_conditional_1 = 0;
        }
        else {
            sysml_conditional_1 = sysml_state_rainbowIndex + 1;
        }
        sysml_state_rainbowIndex = sysml_conditional_1;
        sysml_state_ledOn = (!sysml_state_ledOn);
        sysml_state_servo1At180 = (!sysml_state_servo1At180);
        sysml_state_servo2At0 = (!sysml_state_servo2At0);
        sysml_state_servo3At135 = (!sysml_state_servo3At135);
    }
    double sysml_value_0 = 0;
    if (sysml_state_ledOn) {
        sysml_conditional_2 = 1.0;
    }
    else {
        sysml_conditional_2 = 0.0;
    }
    sysml_value_0 = sysml_conditional_2;
    double sysml_value_1 = 0;
    sysml_lazy_3 = sysml_state_rainbowIndex == 0;
    if (!sysml_lazy_3) {
        sysml_lazy_3 = sysml_state_rainbowIndex == 3;
    }
    sysml_lazy_4 = sysml_lazy_3;
    if (!sysml_lazy_4) {
        sysml_lazy_4 = sysml_state_rainbowIndex == 5;
    }
    sysml_lazy_5 = sysml_lazy_4;
    if (!sysml_lazy_5) {
        sysml_lazy_5 = sysml_state_rainbowIndex == 6;
    }
    if (sysml_lazy_5) {
        sysml_conditional_6 = 1.0;
    }
    else {
        sysml_conditional_6 = 0.0;
    }
    sysml_value_1 = sysml_conditional_6;
    double sysml_value_2 = 0;
    sysml_lazy_7 = sysml_state_rainbowIndex == 1;
    if (!sysml_lazy_7) {
        sysml_lazy_7 = sysml_state_rainbowIndex == 3;
    }
    sysml_lazy_8 = sysml_lazy_7;
    if (!sysml_lazy_8) {
        sysml_lazy_8 = sysml_state_rainbowIndex == 4;
    }
    sysml_lazy_9 = sysml_lazy_8;
    if (!sysml_lazy_9) {
        sysml_lazy_9 = sysml_state_rainbowIndex == 6;
    }
    if (sysml_lazy_9) {
        sysml_conditional_10 = 1.0;
    }
    else {
        sysml_conditional_10 = 0.0;
    }
    sysml_value_2 = sysml_conditional_10;
    double sysml_value_3 = 0;
    sysml_lazy_11 = sysml_state_rainbowIndex == 2;
    if (!sysml_lazy_11) {
        sysml_lazy_11 = sysml_state_rainbowIndex == 4;
    }
    sysml_lazy_12 = sysml_lazy_11;
    if (!sysml_lazy_12) {
        sysml_lazy_12 = sysml_state_rainbowIndex == 5;
    }
    sysml_lazy_13 = sysml_lazy_12;
    if (!sysml_lazy_13) {
        sysml_lazy_13 = sysml_state_rainbowIndex == 6;
    }
    if (sysml_lazy_13) {
        sysml_conditional_14 = 1.0;
    }
    else {
        sysml_conditional_14 = 0.0;
    }
    sysml_value_3 = sysml_conditional_14;
    double sysml_value_4 = 0;
    sysml_lazy_15 = sysml_state_rainbowIndex == 0;
    if (!sysml_lazy_15) {
        sysml_lazy_15 = sysml_state_rainbowIndex == 3;
    }
    sysml_lazy_16 = sysml_lazy_15;
    if (!sysml_lazy_16) {
        sysml_lazy_16 = sysml_state_rainbowIndex == 5;
    }
    sysml_lazy_17 = sysml_lazy_16;
    if (!sysml_lazy_17) {
        sysml_lazy_17 = sysml_state_rainbowIndex == 6;
    }
    if (sysml_lazy_17) {
        sysml_conditional_18 = 1.0;
    }
    else {
        sysml_conditional_18 = 0.0;
    }
    sysml_value_4 = sysml_conditional_18;
    double sysml_value_5 = 0;
    sysml_lazy_19 = sysml_state_rainbowIndex == 1;
    if (!sysml_lazy_19) {
        sysml_lazy_19 = sysml_state_rainbowIndex == 3;
    }
    sysml_lazy_20 = sysml_lazy_19;
    if (!sysml_lazy_20) {
        sysml_lazy_20 = sysml_state_rainbowIndex == 4;
    }
    sysml_lazy_21 = sysml_lazy_20;
    if (!sysml_lazy_21) {
        sysml_lazy_21 = sysml_state_rainbowIndex == 6;
    }
    if (sysml_lazy_21) {
        sysml_conditional_22 = 1.0;
    }
    else {
        sysml_conditional_22 = 0.0;
    }
    sysml_value_5 = sysml_conditional_22;
    double sysml_value_6 = 0;
    sysml_lazy_23 = sysml_state_rainbowIndex == 2;
    if (!sysml_lazy_23) {
        sysml_lazy_23 = sysml_state_rainbowIndex == 4;
    }
    sysml_lazy_24 = sysml_lazy_23;
    if (!sysml_lazy_24) {
        sysml_lazy_24 = sysml_state_rainbowIndex == 5;
    }
    sysml_lazy_25 = sysml_lazy_24;
    if (!sysml_lazy_25) {
        sysml_lazy_25 = sysml_state_rainbowIndex == 6;
    }
    if (sysml_lazy_25) {
        sysml_conditional_26 = 1.0;
    }
    else {
        sysml_conditional_26 = 0.0;
    }
    sysml_value_6 = sysml_conditional_26;
    double sysml_value_7 = 0;
    if (sysml_state_servo1At180) {
        sysml_conditional_27 = 180.0;
    }
    else {
        sysml_conditional_27 = 0.0;
    }
    sysml_value_7 = sysml_conditional_27;
    double sysml_value_8 = 0;
    if (sysml_state_servo2At0) {
        sysml_conditional_28 = 0.0;
    }
    else {
        sysml_conditional_28 = 180.0;
    }
    sysml_value_8 = sysml_conditional_28;
    double sysml_value_9 = 0;
    if (sysml_state_servo3At135) {
        sysml_conditional_29 = 135.0;
    }
    else {
        sysml_conditional_29 = 90.0;
    }
    sysml_value_9 = sysml_conditional_29;
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
    *io_3 = sysml_value_2;
    *io_4 = sysml_value_3;
    *io_5 = sysml_value_4;
    *io_6 = sysml_value_5;
    *io_7 = sysml_value_6;
    *io_8 = sysml_value_7;
    *io_9 = sysml_value_8;
    *io_10 = sysml_value_9;
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
static int32_t hw_value_io_0 = 0;
static double hw_value_io_1 = 0;
static double hw_value_io_2 = 1;
static double hw_value_io_3 = 0;
static double hw_value_io_4 = 0;
static double hw_value_io_5 = 1;
static double hw_value_io_6 = 0;
static double hw_value_io_7 = 0;
static double hw_value_io_8 = 0;
Servo hw_servo_io_8;
static int hw_servo_previous_io_8 = -1;
static double hw_value_io_9 = 180;
Servo hw_servo_io_9;
static int hw_servo_previous_io_9 = -1;
static double hw_value_io_10 = 90;
Servo hw_servo_io_10;
static int hw_servo_previous_io_10 = -1;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, OUTPUT);
  analogWrite(2, (int)constrain((hw_value_io_1 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(3, OUTPUT);
  analogWrite(3, (int)constrain((hw_value_io_2 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(4, OUTPUT);
  analogWrite(4, (int)constrain((hw_value_io_3 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(5, OUTPUT);
  analogWrite(5, (int)constrain((hw_value_io_4 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(6, OUTPUT);
  analogWrite(6, (int)constrain((hw_value_io_5 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(7, OUTPUT);
  analogWrite(7, (int)constrain((hw_value_io_6 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(8, OUTPUT);
  analogWrite(8, (int)constrain((hw_value_io_7 * 1.0 + 0.0), 0.0, 255.0));
  hw_servo_io_8.attach(9);
  if (isfinite((double)hw_value_io_8) && hw_value_io_8 >= 0 && hw_value_io_8 <= 180) {
    int command = (int)hw_value_io_8;
    if (command != hw_servo_previous_io_8) {
      hw_servo_io_8.write(command); hw_servo_previous_io_8 = command;
    }
  }
  hw_servo_io_9.attach(10);
  if (isfinite((double)hw_value_io_9) && hw_value_io_9 >= 0 && hw_value_io_9 <= 180) {
    int command = (int)hw_value_io_9;
    if (command != hw_servo_previous_io_9) {
      hw_servo_io_9.write(command); hw_servo_previous_io_9 = command;
    }
  }
  hw_servo_io_10.attach(11);
  if (isfinite((double)hw_value_io_10) && hw_value_io_10 >= 0 && hw_value_io_10 <= 180) {
    int command = (int)hw_value_io_10;
    if (command != hw_servo_previous_io_10) {
      hw_servo_io_10.write(command); hw_servo_previous_io_10 = command;
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
  hw_value_io_0 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_io_7, &hw_value_io_8, &hw_value_io_9, &hw_value_io_10);
  analogWrite(2, (int)constrain((hw_value_io_1 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(3, (int)constrain((hw_value_io_2 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(4, (int)constrain((hw_value_io_3 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(5, (int)constrain((hw_value_io_4 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(6, (int)constrain((hw_value_io_5 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(7, (int)constrain((hw_value_io_6 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(8, (int)constrain((hw_value_io_7 * 1.0 + 0.0), 0.0, 255.0));
  if (isfinite((double)hw_value_io_8) && hw_value_io_8 >= 0 && hw_value_io_8 <= 180) {
    int command = (int)hw_value_io_8;
    if (command != hw_servo_previous_io_8) {
      hw_servo_io_8.write(command); hw_servo_previous_io_8 = command;
    }
  }
  if (isfinite((double)hw_value_io_9) && hw_value_io_9 >= 0 && hw_value_io_9 <= 180) {
    int command = (int)hw_value_io_9;
    if (command != hw_servo_previous_io_9) {
      hw_servo_io_9.write(command); hw_servo_previous_io_9 = command;
    }
  }
  if (isfinite((double)hw_value_io_10) && hw_value_io_10 >= 0 && hw_value_io_10 <= 180) {
    int command = (int)hw_value_io_10;
    if (command != hw_servo_previous_io_10) {
      hw_servo_io_10.write(command); hw_servo_previous_io_10 = command;
    }
  }
}

