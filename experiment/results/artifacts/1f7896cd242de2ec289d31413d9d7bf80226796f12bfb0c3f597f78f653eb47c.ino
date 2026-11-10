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
extern int32_t sysml_state_phaseIndex;
/* Global Variable define of  */
extern int32_t sysml_state_phaseTimeMs;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool* io_0, bool* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7, bool* io_8, double* io_9, double* io_10, double* io_11);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_phaseIndex = 0;
/* Global Variable define of  */
int32_t sysml_state_phaseTimeMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_phaseIndex = 0;
    sysml_state_phaseTimeMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool* io_0, bool* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7, bool* io_8, double* io_9, double* io_10, double* io_11) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int32_t sysml_conditional_0 = 0;
    bool sysml_conditional_1 = 0;
    bool sysml_conditional_2 = 0;
    bool sysml_conditional_3 = 0;
    bool sysml_conditional_4 = 0;
    bool sysml_conditional_5 = 0;
    bool sysml_conditional_6 = 0;
    bool sysml_conditional_7 = 0;
    bool sysml_conditional_8 = 0;
    bool sysml_conditional_9 = 0;
    bool sysml_conditional_10 = 0;
    bool sysml_conditional_11 = 0;
    bool sysml_conditional_12 = 0;
    bool sysml_conditional_13 = 0;
    bool sysml_conditional_14 = 0;
    bool sysml_conditional_15 = 0;
    bool sysml_conditional_16 = 0;
    bool sysml_conditional_17 = 0;
    bool sysml_conditional_18 = 0;
    bool sysml_conditional_19 = 0;
    bool sysml_conditional_20 = 0;
    bool sysml_conditional_21 = 0;
    bool sysml_conditional_22 = 0;
    bool sysml_conditional_23 = 0;
    bool sysml_conditional_24 = 0;
    bool sysml_conditional_25 = 0;
    bool sysml_conditional_26 = 0;
    bool sysml_conditional_27 = 0;
    bool sysml_conditional_28 = 0;
    bool sysml_conditional_29 = 0;
    bool sysml_conditional_30 = 0;
    bool sysml_conditional_31 = 0;
    bool sysml_conditional_32 = 0;
    bool sysml_conditional_33 = 0;
    bool sysml_conditional_34 = 0;
    bool sysml_conditional_35 = 0;
    bool sysml_conditional_36 = 0;
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
    if (((sysml_state_phaseTimeMs + 10) >= 2000)) {
        sysml_state_phaseTimeMs = 0;
        if (sysml_state_phaseIndex == 6) {
            sysml_conditional_0 = 0;
        }
        else {
            sysml_conditional_0 = sysml_state_phaseIndex + 1;
        }
        sysml_state_phaseIndex = sysml_conditional_0;
    }
    else {
        sysml_state_phaseTimeMs = (sysml_state_phaseTimeMs + 10);
    }
    bool sysml_value_0 = 0;
    if (sysml_state_phaseIndex == 0) {
        sysml_conditional_1 = true;
    }
    else {
        if (sysml_state_phaseIndex == 3) {
            sysml_conditional_2 = true;
        }
        else {
            if (sysml_state_phaseIndex == 5) {
                sysml_conditional_3 = true;
            }
            else {
                if (sysml_state_phaseIndex == 6) {
                    sysml_conditional_4 = true;
                }
                else {
                    sysml_conditional_4 = false;
                }
                sysml_conditional_3 = sysml_conditional_4;
            }
            sysml_conditional_2 = sysml_conditional_3;
        }
        sysml_conditional_1 = sysml_conditional_2;
    }
    sysml_value_0 = sysml_conditional_1;
    bool sysml_value_1 = 0;
    if (sysml_state_phaseIndex == 1) {
        sysml_conditional_5 = true;
    }
    else {
        if (sysml_state_phaseIndex == 3) {
            sysml_conditional_6 = true;
        }
        else {
            if (sysml_state_phaseIndex == 4) {
                sysml_conditional_7 = true;
            }
            else {
                if (sysml_state_phaseIndex == 6) {
                    sysml_conditional_8 = true;
                }
                else {
                    sysml_conditional_8 = false;
                }
                sysml_conditional_7 = sysml_conditional_8;
            }
            sysml_conditional_6 = sysml_conditional_7;
        }
        sysml_conditional_5 = sysml_conditional_6;
    }
    sysml_value_1 = sysml_conditional_5;
    bool sysml_value_2 = 0;
    if (sysml_state_phaseIndex == 2) {
        sysml_conditional_9 = true;
    }
    else {
        if (sysml_state_phaseIndex == 4) {
            sysml_conditional_10 = true;
        }
        else {
            if (sysml_state_phaseIndex == 5) {
                sysml_conditional_11 = true;
            }
            else {
                if (sysml_state_phaseIndex == 6) {
                    sysml_conditional_12 = true;
                }
                else {
                    sysml_conditional_12 = false;
                }
                sysml_conditional_11 = sysml_conditional_12;
            }
            sysml_conditional_10 = sysml_conditional_11;
        }
        sysml_conditional_9 = sysml_conditional_10;
    }
    sysml_value_2 = sysml_conditional_9;
    bool sysml_value_3 = 0;
    if (sysml_state_phaseIndex == 0) {
        sysml_conditional_13 = true;
    }
    else {
        if (sysml_state_phaseIndex == 3) {
            sysml_conditional_14 = true;
        }
        else {
            if (sysml_state_phaseIndex == 5) {
                sysml_conditional_15 = true;
            }
            else {
                if (sysml_state_phaseIndex == 6) {
                    sysml_conditional_16 = true;
                }
                else {
                    sysml_conditional_16 = false;
                }
                sysml_conditional_15 = sysml_conditional_16;
            }
            sysml_conditional_14 = sysml_conditional_15;
        }
        sysml_conditional_13 = sysml_conditional_14;
    }
    sysml_value_3 = sysml_conditional_13;
    bool sysml_value_4 = 0;
    if (sysml_state_phaseIndex == 1) {
        sysml_conditional_17 = true;
    }
    else {
        if (sysml_state_phaseIndex == 3) {
            sysml_conditional_18 = true;
        }
        else {
            if (sysml_state_phaseIndex == 4) {
                sysml_conditional_19 = true;
            }
            else {
                if (sysml_state_phaseIndex == 6) {
                    sysml_conditional_20 = true;
                }
                else {
                    sysml_conditional_20 = false;
                }
                sysml_conditional_19 = sysml_conditional_20;
            }
            sysml_conditional_18 = sysml_conditional_19;
        }
        sysml_conditional_17 = sysml_conditional_18;
    }
    sysml_value_4 = sysml_conditional_17;
    bool sysml_value_5 = 0;
    if (sysml_state_phaseIndex == 2) {
        sysml_conditional_21 = true;
    }
    else {
        if (sysml_state_phaseIndex == 4) {
            sysml_conditional_22 = true;
        }
        else {
            if (sysml_state_phaseIndex == 5) {
                sysml_conditional_23 = true;
            }
            else {
                if (sysml_state_phaseIndex == 6) {
                    sysml_conditional_24 = true;
                }
                else {
                    sysml_conditional_24 = false;
                }
                sysml_conditional_23 = sysml_conditional_24;
            }
            sysml_conditional_22 = sysml_conditional_23;
        }
        sysml_conditional_21 = sysml_conditional_22;
    }
    sysml_value_5 = sysml_conditional_21;
    bool sysml_value_6 = 0;
    if (sysml_state_phaseIndex == 0) {
        sysml_conditional_25 = true;
    }
    else {
        if (sysml_state_phaseIndex == 3) {
            sysml_conditional_26 = true;
        }
        else {
            if (sysml_state_phaseIndex == 5) {
                sysml_conditional_27 = true;
            }
            else {
                if (sysml_state_phaseIndex == 6) {
                    sysml_conditional_28 = true;
                }
                else {
                    sysml_conditional_28 = false;
                }
                sysml_conditional_27 = sysml_conditional_28;
            }
            sysml_conditional_26 = sysml_conditional_27;
        }
        sysml_conditional_25 = sysml_conditional_26;
    }
    sysml_value_6 = sysml_conditional_25;
    bool sysml_value_7 = 0;
    if (sysml_state_phaseIndex == 1) {
        sysml_conditional_29 = true;
    }
    else {
        if (sysml_state_phaseIndex == 3) {
            sysml_conditional_30 = true;
        }
        else {
            if (sysml_state_phaseIndex == 4) {
                sysml_conditional_31 = true;
            }
            else {
                if (sysml_state_phaseIndex == 6) {
                    sysml_conditional_32 = true;
                }
                else {
                    sysml_conditional_32 = false;
                }
                sysml_conditional_31 = sysml_conditional_32;
            }
            sysml_conditional_30 = sysml_conditional_31;
        }
        sysml_conditional_29 = sysml_conditional_30;
    }
    sysml_value_7 = sysml_conditional_29;
    bool sysml_value_8 = 0;
    if (sysml_state_phaseIndex == 2) {
        sysml_conditional_33 = true;
    }
    else {
        if (sysml_state_phaseIndex == 4) {
            sysml_conditional_34 = true;
        }
        else {
            if (sysml_state_phaseIndex == 5) {
                sysml_conditional_35 = true;
            }
            else {
                if (sysml_state_phaseIndex == 6) {
                    sysml_conditional_36 = true;
                }
                else {
                    sysml_conditional_36 = false;
                }
                sysml_conditional_35 = sysml_conditional_36;
            }
            sysml_conditional_34 = sysml_conditional_35;
        }
        sysml_conditional_33 = sysml_conditional_34;
    }
    sysml_value_8 = sysml_conditional_33;
    double sysml_value_9 = 0;
    if (sysml_state_phaseIndex == 0) {
        sysml_conditional_37 = 0.0;
    }
    else {
        if (sysml_state_phaseIndex == 1) {
            sysml_conditional_38 = 45.0;
        }
        else {
            if (sysml_state_phaseIndex == 2) {
                sysml_conditional_39 = 90.0;
            }
            else {
                if (sysml_state_phaseIndex == 3) {
                    sysml_conditional_40 = 135.0;
                }
                else {
                    if (sysml_state_phaseIndex == 4) {
                        sysml_conditional_41 = 180.0;
                    }
                    else {
                        if (sysml_state_phaseIndex == 5) {
                            sysml_conditional_42 = 0.0;
                        }
                        else {
                            sysml_conditional_42 = 90.0;
                        }
                        sysml_conditional_41 = sysml_conditional_42;
                    }
                    sysml_conditional_40 = sysml_conditional_41;
                }
                sysml_conditional_39 = sysml_conditional_40;
            }
            sysml_conditional_38 = sysml_conditional_39;
        }
        sysml_conditional_37 = sysml_conditional_38;
    }
    sysml_value_9 = sysml_conditional_37;
    double sysml_value_10 = 0;
    if (sysml_state_phaseIndex == 0) {
        sysml_conditional_43 = 45.0;
    }
    else {
        if (sysml_state_phaseIndex == 1) {
            sysml_conditional_44 = 90.0;
        }
        else {
            if (sysml_state_phaseIndex == 2) {
                sysml_conditional_45 = 135.0;
            }
            else {
                if (sysml_state_phaseIndex == 3) {
                    sysml_conditional_46 = 180.0;
                }
                else {
                    if (sysml_state_phaseIndex == 4) {
                        sysml_conditional_47 = 0.0;
                    }
                    else {
                        if (sysml_state_phaseIndex == 5) {
                            sysml_conditional_48 = 45.0;
                        }
                        else {
                            sysml_conditional_48 = 135.0;
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
    sysml_value_10 = sysml_conditional_43;
    double sysml_value_11 = 0;
    if (sysml_state_phaseIndex == 0) {
        sysml_conditional_49 = 90.0;
    }
    else {
        if (sysml_state_phaseIndex == 1) {
            sysml_conditional_50 = 135.0;
        }
        else {
            if (sysml_state_phaseIndex == 2) {
                sysml_conditional_51 = 180.0;
            }
            else {
                if (sysml_state_phaseIndex == 3) {
                    sysml_conditional_52 = 0.0;
                }
                else {
                    if (sysml_state_phaseIndex == 4) {
                        sysml_conditional_53 = 45.0;
                    }
                    else {
                        if (sysml_state_phaseIndex == 5) {
                            sysml_conditional_54 = 90.0;
                        }
                        else {
                            sysml_conditional_54 = 180.0;
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
    sysml_value_11 = sysml_conditional_49;
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
static bool hw_value_io_0 = false;
static bool hw_value_io_1 = false;
static bool hw_value_io_2 = false;
static bool hw_value_io_3 = false;
static bool hw_value_io_4 = false;
static bool hw_value_io_5 = false;
static bool hw_value_io_6 = false;
static bool hw_value_io_7 = false;
static bool hw_value_io_8 = false;
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
  digitalWrite(2, hw_value_io_0 ? HIGH : LOW);
  pinMode(3, OUTPUT);
  digitalWrite(3, hw_value_io_1 ? HIGH : LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, hw_value_io_2 ? HIGH : LOW);
  pinMode(5, OUTPUT);
  digitalWrite(5, hw_value_io_3 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  digitalWrite(6, hw_value_io_4 ? HIGH : LOW);
  pinMode(7, OUTPUT);
  digitalWrite(7, hw_value_io_5 ? HIGH : LOW);
  pinMode(8, OUTPUT);
  digitalWrite(8, hw_value_io_6 ? HIGH : LOW);
  pinMode(9, OUTPUT);
  digitalWrite(9, hw_value_io_7 ? HIGH : LOW);
  pinMode(10, OUTPUT);
  digitalWrite(10, hw_value_io_8 ? HIGH : LOW);
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
  digitalWrite(2, hw_value_io_0 ? HIGH : LOW);
  digitalWrite(3, hw_value_io_1 ? HIGH : LOW);
  digitalWrite(4, hw_value_io_2 ? HIGH : LOW);
  digitalWrite(5, hw_value_io_3 ? HIGH : LOW);
  digitalWrite(6, hw_value_io_4 ? HIGH : LOW);
  digitalWrite(7, hw_value_io_5 ? HIGH : LOW);
  digitalWrite(8, hw_value_io_6 ? HIGH : LOW);
  digitalWrite(9, hw_value_io_7 ? HIGH : LOW);
  digitalWrite(10, hw_value_io_8 ? HIGH : LOW);
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
