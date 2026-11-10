#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_45\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
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
extern int32_t sysml_state_colorIndex;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double* io_0, double* io_1, double* io_2, double* io_3, double* io_4, double* io_5, double* io_6, double* io_7, double* io_8, double* io_9, double* io_10, double* io_11);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_colorIndex = 0;
/* Init function of CompositeActor named "ProgramPage1" */
#line 531 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_45\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void setup();
#line 574 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_45\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void loop();
#line 29 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_45\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_colorIndex = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double* io_0, double* io_1, double* io_2, double* io_3, double* io_4, double* io_5, double* io_6, double* io_7, double* io_8, double* io_9, double* io_10, double* io_11) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int32_t sysml_conditional_0 = 0;
    double sysml_conditional_1 = 0;
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
    double sysml_conditional_48 = 0;
    double sysml_conditional_49 = 0;
    double sysml_conditional_50 = 0;
    double sysml_conditional_51 = 0;
    double sysml_conditional_52 = 0;
    double sysml_conditional_53 = 0;
    double sysml_conditional_54 = 0;
    double sysml_conditional_55 = 0;
    double sysml_conditional_56 = 0;
    double sysml_conditional_57 = 0;
    double sysml_conditional_58 = 0;
    double sysml_conditional_59 = 0;
    double sysml_conditional_60 = 0;
    if (sysml_state_colorIndex >= 6) {
        sysml_conditional_0 = 0;
    }
    else {
        sysml_conditional_0 = sysml_state_colorIndex + 1;
    }
    sysml_state_colorIndex = sysml_conditional_0;
    double sysml_value_0 = 0;
    if (sysml_state_colorIndex == 0) {
        sysml_conditional_1 = 255.0;
    }
    else {
        if (sysml_state_colorIndex == 3) {
            sysml_conditional_2 = 255.0;
        }
        else {
            if (sysml_state_colorIndex == 5) {
                sysml_conditional_3 = 255.0;
            }
            else {
                if (sysml_state_colorIndex == 6) {
                    sysml_conditional_4 = 255.0;
                }
                else {
                    sysml_conditional_4 = 0.0;
                }
                sysml_conditional_3 = sysml_conditional_4;
            }
            sysml_conditional_2 = sysml_conditional_3;
        }
        sysml_conditional_1 = sysml_conditional_2;
    }
    sysml_value_0 = sysml_conditional_1;
    double sysml_value_1 = 0;
    if (sysml_state_colorIndex == 1) {
        sysml_conditional_5 = 255.0;
    }
    else {
        if (sysml_state_colorIndex == 3) {
            sysml_conditional_6 = 255.0;
        }
        else {
            if (sysml_state_colorIndex == 4) {
                sysml_conditional_7 = 255.0;
            }
            else {
                if (sysml_state_colorIndex == 5) {
                    sysml_conditional_8 = 255.0;
                }
                else {
                    if (sysml_state_colorIndex == 6) {
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
    sysml_value_1 = sysml_conditional_5;
    double sysml_value_2 = 0;
    if (sysml_state_colorIndex == 2) {
        sysml_conditional_10 = 255.0;
    }
    else {
        if (sysml_state_colorIndex == 3) {
            sysml_conditional_11 = 0.0;
        }
        else {
            if (sysml_state_colorIndex == 4) {
                sysml_conditional_12 = 255.0;
            }
            else {
                if (sysml_state_colorIndex == 5) {
                    sysml_conditional_13 = 255.0;
                }
                else {
                    if (sysml_state_colorIndex == 6) {
                        sysml_conditional_14 = 255.0;
                    }
                    else {
                        sysml_conditional_14 = 0.0;
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
    if (sysml_state_colorIndex == 0) {
        sysml_conditional_15 = 255.0;
    }
    else {
        if (sysml_state_colorIndex == 3) {
            sysml_conditional_16 = 255.0;
        }
        else {
            if (sysml_state_colorIndex == 5) {
                sysml_conditional_17 = 255.0;
            }
            else {
                if (sysml_state_colorIndex == 6) {
                    sysml_conditional_18 = 255.0;
                }
                else {
                    sysml_conditional_18 = 0.0;
                }
                sysml_conditional_17 = sysml_conditional_18;
            }
            sysml_conditional_16 = sysml_conditional_17;
        }
        sysml_conditional_15 = sysml_conditional_16;
    }
    sysml_value_3 = sysml_conditional_15;
    double sysml_value_4 = 0;
    if (sysml_state_colorIndex == 1) {
        sysml_conditional_19 = 255.0;
    }
    else {
        if (sysml_state_colorIndex == 3) {
            sysml_conditional_20 = 255.0;
        }
        else {
            if (sysml_state_colorIndex == 4) {
                sysml_conditional_21 = 255.0;
            }
            else {
                if (sysml_state_colorIndex == 5) {
                    sysml_conditional_22 = 255.0;
                }
                else {
                    if (sysml_state_colorIndex == 6) {
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
    sysml_value_4 = sysml_conditional_19;
    double sysml_value_5 = 0;
    if (sysml_state_colorIndex == 2) {
        sysml_conditional_24 = 255.0;
    }
    else {
        if (sysml_state_colorIndex == 3) {
            sysml_conditional_25 = 0.0;
        }
        else {
            if (sysml_state_colorIndex == 4) {
                sysml_conditional_26 = 255.0;
            }
            else {
                if (sysml_state_colorIndex == 5) {
                    sysml_conditional_27 = 255.0;
                }
                else {
                    if (sysml_state_colorIndex == 6) {
                        sysml_conditional_28 = 255.0;
                    }
                    else {
                        sysml_conditional_28 = 0.0;
                    }
                    sysml_conditional_27 = sysml_conditional_28;
                }
                sysml_conditional_26 = sysml_conditional_27;
            }
            sysml_conditional_25 = sysml_conditional_26;
        }
        sysml_conditional_24 = sysml_conditional_25;
    }
    sysml_value_5 = sysml_conditional_24;
    double sysml_value_6 = 0;
    if (sysml_state_colorIndex == 0) {
        sysml_conditional_29 = 255.0;
    }
    else {
        if (sysml_state_colorIndex == 3) {
            sysml_conditional_30 = 255.0;
        }
        else {
            if (sysml_state_colorIndex == 5) {
                sysml_conditional_31 = 255.0;
            }
            else {
                if (sysml_state_colorIndex == 6) {
                    sysml_conditional_32 = 255.0;
                }
                else {
                    sysml_conditional_32 = 0.0;
                }
                sysml_conditional_31 = sysml_conditional_32;
            }
            sysml_conditional_30 = sysml_conditional_31;
        }
        sysml_conditional_29 = sysml_conditional_30;
    }
    sysml_value_6 = sysml_conditional_29;
    double sysml_value_7 = 0;
    if (sysml_state_colorIndex == 1) {
        sysml_conditional_33 = 255.0;
    }
    else {
        if (sysml_state_colorIndex == 3) {
            sysml_conditional_34 = 255.0;
        }
        else {
            if (sysml_state_colorIndex == 4) {
                sysml_conditional_35 = 255.0;
            }
            else {
                if (sysml_state_colorIndex == 5) {
                    sysml_conditional_36 = 255.0;
                }
                else {
                    if (sysml_state_colorIndex == 6) {
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
    sysml_value_7 = sysml_conditional_33;
    double sysml_value_8 = 0;
    if (sysml_state_colorIndex == 2) {
        sysml_conditional_38 = 255.0;
    }
    else {
        if (sysml_state_colorIndex == 3) {
            sysml_conditional_39 = 0.0;
        }
        else {
            if (sysml_state_colorIndex == 4) {
                sysml_conditional_40 = 255.0;
            }
            else {
                if (sysml_state_colorIndex == 5) {
                    sysml_conditional_41 = 255.0;
                }
                else {
                    if (sysml_state_colorIndex == 6) {
                        sysml_conditional_42 = 255.0;
                    }
                    else {
                        sysml_conditional_42 = 0.0;
                    }
                    sysml_conditional_41 = sysml_conditional_42;
                }
                sysml_conditional_40 = sysml_conditional_41;
            }
            sysml_conditional_39 = sysml_conditional_40;
        }
        sysml_conditional_38 = sysml_conditional_39;
    }
    sysml_value_8 = sysml_conditional_38;
    double sysml_value_9 = 0;
    if (sysml_state_colorIndex == 0) {
        sysml_conditional_43 = 0.0;
    }
    else {
        if (sysml_state_colorIndex == 1) {
            sysml_conditional_44 = 45.0;
        }
        else {
            if (sysml_state_colorIndex == 2) {
                sysml_conditional_45 = 90.0;
            }
            else {
                if (sysml_state_colorIndex == 3) {
                    sysml_conditional_46 = 135.0;
                }
                else {
                    if (sysml_state_colorIndex == 4) {
                        sysml_conditional_47 = 180.0;
                    }
                    else {
                        if (sysml_state_colorIndex == 5) {
                            sysml_conditional_48 = 0.0;
                        }
                        else {
                            sysml_conditional_48 = 90.0;
                        }
                        sysml_conditional_47 = sysml_conditional_48;
                    }
                    sysml_conditional_46 = sysml_conditional_47;
                }
                sysml_conditional_45 = sysml_conditional_46;
            }
            sysml_conditional_44 = sysml_conditional_45;
        }
        sysml_conditional_43 = sysml_conditional_44;
    }
    sysml_value_9 = sysml_conditional_43;
    double sysml_value_10 = 0;
    if (sysml_state_colorIndex == 0) {
        sysml_conditional_49 = 45.0;
    }
    else {
        if (sysml_state_colorIndex == 1) {
            sysml_conditional_50 = 90.0;
        }
        else {
            if (sysml_state_colorIndex == 2) {
                sysml_conditional_51 = 135.0;
            }
            else {
                if (sysml_state_colorIndex == 3) {
                    sysml_conditional_52 = 180.0;
                }
                else {
                    if (sysml_state_colorIndex == 4) {
                        sysml_conditional_53 = 0.0;
                    }
                    else {
                        if (sysml_state_colorIndex == 5) {
                            sysml_conditional_54 = 45.0;
                        }
                        else {
                            sysml_conditional_54 = 135.0;
                        }
                        sysml_conditional_53 = sysml_conditional_54;
                    }
                    sysml_conditional_52 = sysml_conditional_53;
                }
                sysml_conditional_51 = sysml_conditional_52;
            }
            sysml_conditional_50 = sysml_conditional_51;
        }
        sysml_conditional_49 = sysml_conditional_50;
    }
    sysml_value_10 = sysml_conditional_49;
    double sysml_value_11 = 0;
    if (sysml_state_colorIndex == 0) {
        sysml_conditional_55 = 90.0;
    }
    else {
        if (sysml_state_colorIndex == 1) {
            sysml_conditional_56 = 135.0;
        }
        else {
            if (sysml_state_colorIndex == 2) {
                sysml_conditional_57 = 180.0;
            }
            else {
                if (sysml_state_colorIndex == 3) {
                    sysml_conditional_58 = 0.0;
                }
                else {
                    if (sysml_state_colorIndex == 4) {
                        sysml_conditional_59 = 45.0;
                    }
                    else {
                        if (sysml_state_colorIndex == 5) {
                            sysml_conditional_60 = 90.0;
                        }
                        else {
                            sysml_conditional_60 = 180.0;
                        }
                        sysml_conditional_59 = sysml_conditional_60;
                    }
                    sysml_conditional_58 = sysml_conditional_59;
                }
                sysml_conditional_57 = sysml_conditional_58;
            }
            sysml_conditional_56 = sysml_conditional_57;
        }
        sysml_conditional_55 = sysml_conditional_56;
    }
    sysml_value_11 = sysml_conditional_55;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
    *io_4 = sysml_value_4;
    *io_5 = sysml_value_5;
    *io_6 = sysml_value_6;
    *io_7 = sysml_value_7;
    *io_8 = sysml_value_8;
    *io_9 = sysml_value_9;
    *io_10 = sysml_value_10;
    *io_11 = sysml_value_11;
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
static double hw_value_io_1 = 0;
static double hw_value_io_2 = 0;
static double hw_value_io_3 = 0;
static double hw_value_io_4 = 0;
static double hw_value_io_5 = 0;
static double hw_value_io_6 = 0;
static double hw_value_io_7 = 0;
static double hw_value_io_8 = 0;
static double hw_value_io_9 = 0;
Servo hw_servo_io_9;
static int hw_servo_previous_io_9 = -1;
static double hw_value_io_10 = 0;
Servo hw_servo_io_10;
static int hw_servo_previous_io_10 = -1;
static double hw_value_io_11 = 0;
Servo hw_servo_io_11;
static int hw_servo_previous_io_11 = -1;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, OUTPUT);
  analogWrite(2, (int)constrain((hw_value_io_0 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(3, OUTPUT);
  analogWrite(3, (int)constrain((hw_value_io_1 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(4, OUTPUT);
  analogWrite(4, (int)constrain((hw_value_io_2 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(5, OUTPUT);
  analogWrite(5, (int)constrain((hw_value_io_3 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(6, OUTPUT);
  analogWrite(6, (int)constrain((hw_value_io_4 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(7, OUTPUT);
  analogWrite(7, (int)constrain((hw_value_io_5 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(8, OUTPUT);
  analogWrite(8, (int)constrain((hw_value_io_6 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(9, OUTPUT);
  analogWrite(9, (int)constrain((hw_value_io_7 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(10, OUTPUT);
  analogWrite(10, (int)constrain((hw_value_io_8 * 1.0 + 0.0), 0.0, 255.0));
  hw_servo_io_9.attach(11);
  if (isfinite((double)hw_value_io_9) && hw_value_io_9 >= 0 && hw_value_io_9 <= 180) {
    int command = (int)hw_value_io_9;
    if (command != hw_servo_previous_io_9) {
      hw_servo_io_9.write(command); hw_servo_previous_io_9 = command;
    }
  }
  hw_servo_io_10.attach(12);
  if (isfinite((double)hw_value_io_10) && hw_value_io_10 >= 0 && hw_value_io_10 <= 180) {
    int command = (int)hw_value_io_10;
    if (command != hw_servo_previous_io_10) {
      hw_servo_io_10.write(command); hw_servo_previous_io_10 = command;
    }
  }
  hw_servo_io_11.attach(13);
  if (isfinite((double)hw_value_io_11) && hw_value_io_11 >= 0 && hw_value_io_11 <= 180) {
    int command = (int)hw_value_io_11;
    if (command != hw_servo_previous_io_11) {
      hw_servo_io_11.write(command); hw_servo_previous_io_11 = command;
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
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(&hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_io_7, &hw_value_io_8, &hw_value_io_9, &hw_value_io_10, &hw_value_io_11);
  analogWrite(2, (int)constrain((hw_value_io_0 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(3, (int)constrain((hw_value_io_1 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(4, (int)constrain((hw_value_io_2 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(5, (int)constrain((hw_value_io_3 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(6, (int)constrain((hw_value_io_4 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(7, (int)constrain((hw_value_io_5 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(8, (int)constrain((hw_value_io_6 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(9, (int)constrain((hw_value_io_7 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(10, (int)constrain((hw_value_io_8 * 1.0 + 0.0), 0.0, 255.0));
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
  if (isfinite((double)hw_value_io_11) && hw_value_io_11 >= 0 && hw_value_io_11 <= 180) {
    int command = (int)hw_value_io_11;
    if (command != hw_servo_previous_io_11) {
      hw_servo_io_11.write(command); hw_servo_previous_io_11 = command;
    }
  }
}

