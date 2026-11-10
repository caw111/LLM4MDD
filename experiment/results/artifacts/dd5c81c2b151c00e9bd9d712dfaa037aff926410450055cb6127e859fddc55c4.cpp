#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_110\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
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
extern int32_t sysml_state_ledAccumMs;
/* Global Variable define of  */
extern int32_t sysml_state_rainbowAccumMs;
/* Global Variable define of  */
extern int32_t sysml_state_servoAccumMs;
/* Global Variable define of  */
extern int32_t sysml_state_servoPhase;
/* Global Variable define of  */
extern int32_t sysml_state_rainbowStep;
/* Global Variable define of  */
extern bool sysml_state_ledOn;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_0, double* io_1, double* io_2, double* io_3, double* io_4, double* io_5, double* io_6, double* io_7, double* io_8, double* io_9, double* io_10);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_ledAccumMs = 0;
/* Global Variable define of  */
int32_t sysml_state_rainbowAccumMs = 0;
/* Global Variable define of  */
int32_t sysml_state_servoAccumMs = 0;
/* Global Variable define of  */
int32_t sysml_state_servoPhase = 0;
/* Global Variable define of  */
int32_t sysml_state_rainbowStep = 0;
/* Global Variable define of  */
bool sysml_state_ledOn = false;
/* Init function of CompositeActor named "ProgramPage1" */
#line 472 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_110\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 511 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_110\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 49 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_110\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_ledAccumMs = 0;
    sysml_state_rainbowAccumMs = 0;
    sysml_state_servoAccumMs = 0;
    sysml_state_servoPhase = 0;
    sysml_state_rainbowStep = 0;
    sysml_state_ledOn = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_0, double* io_1, double* io_2, double* io_3, double* io_4, double* io_5, double* io_6, double* io_7, double* io_8, double* io_9, double* io_10) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int32_t sysml_conditional_0 = 0;
    int32_t sysml_conditional_1 = 0;
    double sysml_conditional_2 = 0;
    double sysml_conditional_3 = 0;
    double sysml_conditional_4 = 0;
    double sysml_conditional_5 = 0;
    double sysml_conditional_6 = 0;
    double sysml_conditional_7 = 0;
    double sysml_conditional_8 = 0;
    double sysml_conditional_9 = 0;
    double sysml_conditional_10 = 0;
    double sysml_conditional_11 = 0;
    double sysml_conditional_12 = 0;
    double sysml_conditional_13 = 0;
    double sysml_conditional_14 = 0;
    double sysml_conditional_15 = 0;
    double sysml_conditional_16 = 0;
    double sysml_conditional_17 = 0;
    double sysml_conditional_18 = 0;
    double sysml_conditional_19 = 0;
    double sysml_conditional_20 = 0;
    double sysml_conditional_21 = 0;
    double sysml_conditional_22 = 0;
    double sysml_conditional_23 = 0;
    double sysml_conditional_24 = 0;
    double sysml_conditional_25 = 0;
    double sysml_conditional_26 = 0;
    double sysml_conditional_27 = 0;
    double sysml_conditional_28 = 0;
    double sysml_conditional_29 = 0;
    double sysml_conditional_30 = 0;
    double sysml_conditional_31 = 0;
    double sysml_conditional_32 = 0;
    double sysml_conditional_33 = 0;
    double sysml_conditional_34 = 0;
    double sysml_conditional_35 = 0;
    double sysml_conditional_36 = 0;
    double sysml_conditional_37 = 0;
    double sysml_conditional_38 = 0;
    double sysml_conditional_39 = 0;
    double sysml_conditional_40 = 0;
    double sysml_conditional_41 = 0;
    double sysml_conditional_42 = 0;
    double sysml_conditional_43 = 0;
    double sysml_conditional_44 = 0;
    double sysml_conditional_45 = 0;
    double sysml_conditional_46 = 0;
    double sysml_conditional_47 = 0;
    sysml_state_ledAccumMs = (sysml_state_ledAccumMs + io_0);
    sysml_state_rainbowAccumMs = (sysml_state_rainbowAccumMs + io_0);
    sysml_state_servoAccumMs = (sysml_state_servoAccumMs + io_0);
    if ((sysml_state_ledAccumMs >= 2000)) {
        sysml_state_ledAccumMs = (sysml_state_ledAccumMs - 2000);
        sysml_state_ledOn = (!sysml_state_ledOn);
    }
    if ((sysml_state_rainbowAccumMs >= 2000)) {
        sysml_state_rainbowAccumMs = (sysml_state_rainbowAccumMs - 2000);
        if (sysml_state_rainbowStep >= 7) {
            sysml_conditional_0 = 0;
        }
        else {
            sysml_conditional_0 = sysml_state_rainbowStep + 1;
        }
        sysml_state_rainbowStep = sysml_conditional_0;
    }
    if ((sysml_state_servoAccumMs >= 2000)) {
        sysml_state_servoAccumMs = (sysml_state_servoAccumMs - 2000);
        if (sysml_state_servoPhase >= 1) {
            sysml_conditional_1 = 0;
        }
        else {
            sysml_conditional_1 = sysml_state_servoPhase + 1;
        }
        sysml_state_servoPhase = sysml_conditional_1;
    }
    double sysml_value_0 = 0;
    if (sysml_state_ledOn) {
        sysml_conditional_2 = 255.0;
    }
    else {
        sysml_conditional_2 = 0.0;
    }
    sysml_value_0 = sysml_conditional_2;
    double sysml_value_1 = 0;
    if (sysml_state_rainbowStep == 0) {
        sysml_conditional_3 = 255.0;
    }
    else {
        if (sysml_state_rainbowStep == 1) {
            sysml_conditional_4 = 0.0;
        }
        else {
            if (sysml_state_rainbowStep == 2) {
                sysml_conditional_5 = 0.0;
            }
            else {
                if (sysml_state_rainbowStep == 3) {
                    sysml_conditional_6 = 255.0;
                }
                else {
                    if (sysml_state_rainbowStep == 4) {
                        sysml_conditional_7 = 0.0;
                    }
                    else {
                        if (sysml_state_rainbowStep == 5) {
                            sysml_conditional_8 = 255.0;
                        }
                        else {
                            if (sysml_state_rainbowStep == 6) {
                                sysml_conditional_9 = 255.0;
                            }
                            else {
                                sysml_conditional_9 = 0.0;
                            }
                            sysml_conditional_8 = sysml_conditional_9;
                        }
                        sysml_conditional_7 = sysml_conditional_8;
                    }
                    sysml_conditional_6 = sysml_conditional_7;
                }
                sysml_conditional_5 = sysml_conditional_6;
            }
            sysml_conditional_4 = sysml_conditional_5;
        }
        sysml_conditional_3 = sysml_conditional_4;
    }
    sysml_value_1 = sysml_conditional_3;
    double sysml_value_2 = 0;
    if (sysml_state_rainbowStep == 0) {
        sysml_conditional_10 = 0.0;
    }
    else {
        if (sysml_state_rainbowStep == 1) {
            sysml_conditional_11 = 255.0;
        }
        else {
            if (sysml_state_rainbowStep == 2) {
                sysml_conditional_12 = 0.0;
            }
            else {
                if (sysml_state_rainbowStep == 3) {
                    sysml_conditional_13 = 255.0;
                }
                else {
                    if (sysml_state_rainbowStep == 4) {
                        sysml_conditional_14 = 255.0;
                    }
                    else {
                        if (sysml_state_rainbowStep == 5) {
                            sysml_conditional_15 = 0.0;
                        }
                        else {
                            if (sysml_state_rainbowStep == 6) {
                                sysml_conditional_16 = 255.0;
                            }
                            else {
                                sysml_conditional_16 = 0.0;
                            }
                            sysml_conditional_15 = sysml_conditional_16;
                        }
                        sysml_conditional_14 = sysml_conditional_15;
                    }
                    sysml_conditional_13 = sysml_conditional_14;
                }
                sysml_conditional_12 = sysml_conditional_13;
            }
            sysml_conditional_11 = sysml_conditional_12;
        }
        sysml_conditional_10 = sysml_conditional_11;
    }
    sysml_value_2 = sysml_conditional_10;
    double sysml_value_3 = 0;
    if (sysml_state_rainbowStep == 0) {
        sysml_conditional_17 = 0.0;
    }
    else {
        if (sysml_state_rainbowStep == 1) {
            sysml_conditional_18 = 0.0;
        }
        else {
            if (sysml_state_rainbowStep == 2) {
                sysml_conditional_19 = 255.0;
            }
            else {
                if (sysml_state_rainbowStep == 3) {
                    sysml_conditional_20 = 0.0;
                }
                else {
                    if (sysml_state_rainbowStep == 4) {
                        sysml_conditional_21 = 255.0;
                    }
                    else {
                        if (sysml_state_rainbowStep == 5) {
                            sysml_conditional_22 = 255.0;
                        }
                        else {
                            if (sysml_state_rainbowStep == 6) {
                                sysml_conditional_23 = 255.0;
                            }
                            else {
                                sysml_conditional_23 = 0.0;
                            }
                            sysml_conditional_22 = sysml_conditional_23;
                        }
                        sysml_conditional_21 = sysml_conditional_22;
                    }
                    sysml_conditional_20 = sysml_conditional_21;
                }
                sysml_conditional_19 = sysml_conditional_20;
            }
            sysml_conditional_18 = sysml_conditional_19;
        }
        sysml_conditional_17 = sysml_conditional_18;
    }
    sysml_value_3 = sysml_conditional_17;
    double sysml_value_4 = 0;
    if (sysml_state_rainbowStep == 0) {
        sysml_conditional_24 = 255.0;
    }
    else {
        if (sysml_state_rainbowStep == 1) {
            sysml_conditional_25 = 0.0;
        }
        else {
            if (sysml_state_rainbowStep == 2) {
                sysml_conditional_26 = 0.0;
            }
            else {
                if (sysml_state_rainbowStep == 3) {
                    sysml_conditional_27 = 255.0;
                }
                else {
                    if (sysml_state_rainbowStep == 4) {
                        sysml_conditional_28 = 0.0;
                    }
                    else {
                        if (sysml_state_rainbowStep == 5) {
                            sysml_conditional_29 = 255.0;
                        }
                        else {
                            if (sysml_state_rainbowStep == 6) {
                                sysml_conditional_30 = 255.0;
                            }
                            else {
                                sysml_conditional_30 = 0.0;
                            }
                            sysml_conditional_29 = sysml_conditional_30;
                        }
                        sysml_conditional_28 = sysml_conditional_29;
                    }
                    sysml_conditional_27 = sysml_conditional_28;
                }
                sysml_conditional_26 = sysml_conditional_27;
            }
            sysml_conditional_25 = sysml_conditional_26;
        }
        sysml_conditional_24 = sysml_conditional_25;
    }
    sysml_value_4 = sysml_conditional_24;
    double sysml_value_5 = 0;
    if (sysml_state_rainbowStep == 0) {
        sysml_conditional_31 = 0.0;
    }
    else {
        if (sysml_state_rainbowStep == 1) {
            sysml_conditional_32 = 255.0;
        }
        else {
            if (sysml_state_rainbowStep == 2) {
                sysml_conditional_33 = 0.0;
            }
            else {
                if (sysml_state_rainbowStep == 3) {
                    sysml_conditional_34 = 255.0;
                }
                else {
                    if (sysml_state_rainbowStep == 4) {
                        sysml_conditional_35 = 255.0;
                    }
                    else {
                        if (sysml_state_rainbowStep == 5) {
                            sysml_conditional_36 = 0.0;
                        }
                        else {
                            if (sysml_state_rainbowStep == 6) {
                                sysml_conditional_37 = 255.0;
                            }
                            else {
                                sysml_conditional_37 = 0.0;
                            }
                            sysml_conditional_36 = sysml_conditional_37;
                        }
                        sysml_conditional_35 = sysml_conditional_36;
                    }
                    sysml_conditional_34 = sysml_conditional_35;
                }
                sysml_conditional_33 = sysml_conditional_34;
            }
            sysml_conditional_32 = sysml_conditional_33;
        }
        sysml_conditional_31 = sysml_conditional_32;
    }
    sysml_value_5 = sysml_conditional_31;
    double sysml_value_6 = 0;
    if (sysml_state_rainbowStep == 0) {
        sysml_conditional_38 = 0.0;
    }
    else {
        if (sysml_state_rainbowStep == 1) {
            sysml_conditional_39 = 0.0;
        }
        else {
            if (sysml_state_rainbowStep == 2) {
                sysml_conditional_40 = 255.0;
            }
            else {
                if (sysml_state_rainbowStep == 3) {
                    sysml_conditional_41 = 0.0;
                }
                else {
                    if (sysml_state_rainbowStep == 4) {
                        sysml_conditional_42 = 255.0;
                    }
                    else {
                        if (sysml_state_rainbowStep == 5) {
                            sysml_conditional_43 = 255.0;
                        }
                        else {
                            if (sysml_state_rainbowStep == 6) {
                                sysml_conditional_44 = 255.0;
                            }
                            else {
                                sysml_conditional_44 = 0.0;
                            }
                            sysml_conditional_43 = sysml_conditional_44;
                        }
                        sysml_conditional_42 = sysml_conditional_43;
                    }
                    sysml_conditional_41 = sysml_conditional_42;
                }
                sysml_conditional_40 = sysml_conditional_41;
            }
            sysml_conditional_39 = sysml_conditional_40;
        }
        sysml_conditional_38 = sysml_conditional_39;
    }
    sysml_value_6 = sysml_conditional_38;
    double sysml_value_7 = 0;
    if (sysml_state_servoPhase == 0) {
        sysml_conditional_45 = 0.0;
    }
    else {
        sysml_conditional_45 = 180.0;
    }
    sysml_value_7 = sysml_conditional_45;
    double sysml_value_8 = 0;
    if (sysml_state_servoPhase == 0) {
        sysml_conditional_46 = 180.0;
    }
    else {
        sysml_conditional_46 = 0.0;
    }
    sysml_value_8 = sysml_conditional_46;
    double sysml_value_9 = 0;
    if (sysml_state_servoPhase == 0) {
        sysml_conditional_47 = 90.0;
    }
    else {
        sysml_conditional_47 = 135.0;
    }
    sysml_value_9 = sysml_conditional_47;
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
static double hw_value_io_2 = 255;
static double hw_value_io_3 = 0;
static double hw_value_io_4 = 0;
static double hw_value_io_5 = 255;
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

