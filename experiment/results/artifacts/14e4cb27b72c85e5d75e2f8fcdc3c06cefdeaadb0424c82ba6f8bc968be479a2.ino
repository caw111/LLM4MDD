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
extern int32_t sysml_state_phaseMs;
/* Global Variable define of  */
extern int32_t sysml_state_colorIndex;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool* io_0, bool* io_1, bool* io_2, double* io_3, double* io_4, double* io_5);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_phaseMs = 0;
/* Global Variable define of  */
int32_t sysml_state_colorIndex = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_phaseMs = 0;
    sysml_state_colorIndex = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool* io_0, bool* io_1, bool* io_2, double* io_3, double* io_4, double* io_5) {
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
    if ((sysml_state_phaseMs >= 2000)) {
        sysml_state_phaseMs = (sysml_state_phaseMs - 2000);
        if (sysml_state_colorIndex == 6) {
            sysml_conditional_0 = 0;
        }
        else {
            sysml_conditional_0 = sysml_state_colorIndex + 1;
        }
        sysml_state_colorIndex = sysml_conditional_0;
    }
    else {
        sysml_state_phaseMs = (sysml_state_phaseMs + 10);
    }
    bool sysml_value_0 = 0;
    if (sysml_state_colorIndex == 0) {
        sysml_conditional_1 = true;
    }
    else {
        if (sysml_state_colorIndex == 1) {
            sysml_conditional_2 = false;
        }
        else {
            if (sysml_state_colorIndex == 3) {
                sysml_conditional_3 = true;
            }
            else {
                if (sysml_state_colorIndex == 5) {
                    sysml_conditional_4 = true;
                }
                else {
                    if (sysml_state_colorIndex == 6) {
                        sysml_conditional_5 = true;
                    }
                    else {
                        sysml_conditional_5 = false;
                    }
                    sysml_conditional_4 = sysml_conditional_5;
                }
                sysml_conditional_3 = sysml_conditional_4;
            }
            sysml_conditional_2 = sysml_conditional_3;
        }
        sysml_conditional_1 = sysml_conditional_2;
    }
    sysml_value_0 = sysml_conditional_1;
    bool sysml_value_1 = 0;
    if (sysml_state_colorIndex == 0) {
        sysml_conditional_6 = false;
    }
    else {
        if (sysml_state_colorIndex == 1) {
            sysml_conditional_7 = true;
        }
        else {
            if (sysml_state_colorIndex == 3) {
                sysml_conditional_8 = true;
            }
            else {
                if (sysml_state_colorIndex == 4) {
                    sysml_conditional_9 = true;
                }
                else {
                    if (sysml_state_colorIndex == 6) {
                        sysml_conditional_10 = true;
                    }
                    else {
                        sysml_conditional_10 = false;
                    }
                    sysml_conditional_9 = sysml_conditional_10;
                }
                sysml_conditional_8 = sysml_conditional_9;
            }
            sysml_conditional_7 = sysml_conditional_8;
        }
        sysml_conditional_6 = sysml_conditional_7;
    }
    sysml_value_1 = sysml_conditional_6;
    bool sysml_value_2 = 0;
    if (sysml_state_colorIndex == 0) {
        sysml_conditional_11 = false;
    }
    else {
        if (sysml_state_colorIndex == 2) {
            sysml_conditional_12 = true;
        }
        else {
            if (sysml_state_colorIndex == 3) {
                sysml_conditional_13 = false;
            }
            else {
                if (sysml_state_colorIndex == 4) {
                    sysml_conditional_14 = true;
                }
                else {
                    if (sysml_state_colorIndex == 5) {
                        sysml_conditional_15 = true;
                    }
                    else {
                        if (sysml_state_colorIndex == 6) {
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
            sysml_conditional_12 = sysml_conditional_13;
        }
        sysml_conditional_11 = sysml_conditional_12;
    }
    sysml_value_2 = sysml_conditional_11;
    double sysml_value_3 = 0;
    if (sysml_state_colorIndex == 0) {
        sysml_conditional_17 = 0.0;
    }
    else {
        if (sysml_state_colorIndex == 1) {
            sysml_conditional_18 = 90.0;
        }
        else {
            if (sysml_state_colorIndex == 2) {
                sysml_conditional_19 = 180.0;
            }
            else {
                if (sysml_state_colorIndex == 3) {
                    sysml_conditional_20 = 45.0;
                }
                else {
                    if (sysml_state_colorIndex == 4) {
                        sysml_conditional_21 = 135.0;
                    }
                    else {
                        if (sysml_state_colorIndex == 5) {
                            sysml_conditional_22 = 180.0;
                        }
                        else {
                            sysml_conditional_22 = 0.0;
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
    if (sysml_state_colorIndex == 0) {
        sysml_conditional_23 = 90.0;
    }
    else {
        if (sysml_state_colorIndex == 1) {
            sysml_conditional_24 = 180.0;
        }
        else {
            if (sysml_state_colorIndex == 2) {
                sysml_conditional_25 = 0.0;
            }
            else {
                if (sysml_state_colorIndex == 3) {
                    sysml_conditional_26 = 135.0;
                }
                else {
                    if (sysml_state_colorIndex == 4) {
                        sysml_conditional_27 = 180.0;
                    }
                    else {
                        if (sysml_state_colorIndex == 5) {
                            sysml_conditional_28 = 45.0;
                        }
                        else {
                            sysml_conditional_28 = 90.0;
                        }
                        sysml_conditional_27 = sysml_conditional_28;
                    }
                    sysml_conditional_26 = sysml_conditional_27;
                }
                sysml_conditional_25 = sysml_conditional_26;
            }
            sysml_conditional_24 = sysml_conditional_25;
        }
        sysml_conditional_23 = sysml_conditional_24;
    }
    sysml_value_4 = sysml_conditional_23;
    double sysml_value_5 = 0;
    if (sysml_state_colorIndex == 0) {
        sysml_conditional_29 = 180.0;
    }
    else {
        if (sysml_state_colorIndex == 1) {
            sysml_conditional_30 = 0.0;
        }
        else {
            if (sysml_state_colorIndex == 2) {
                sysml_conditional_31 = 90.0;
            }
            else {
                if (sysml_state_colorIndex == 3) {
                    sysml_conditional_32 = 180.0;
                }
                else {
                    if (sysml_state_colorIndex == 4) {
                        sysml_conditional_33 = 45.0;
                    }
                    else {
                        if (sysml_state_colorIndex == 5) {
                            sysml_conditional_34 = 135.0;
                        }
                        else {
                            sysml_conditional_34 = 180.0;
                        }
                        sysml_conditional_33 = sysml_conditional_34;
                    }
                    sysml_conditional_32 = sysml_conditional_33;
                }
                sysml_conditional_31 = sysml_conditional_32;
            }
            sysml_conditional_30 = sysml_conditional_31;
        }
        sysml_conditional_29 = sysml_conditional_30;
    }
    sysml_value_5 = sysml_conditional_29;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
    *io_4 = sysml_value_4;
    *io_5 = sysml_value_5;
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
static double hw_value_io_3 = 0;
Servo hw_servo_io_3;
static int hw_servo_previous_io_3 = -1;
static double hw_value_io_4 = 90;
Servo hw_servo_io_4;
static int hw_servo_previous_io_4 = -1;
static double hw_value_io_5 = 180;
Servo hw_servo_io_5;
static int hw_servo_previous_io_5 = -1;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, OUTPUT);
  digitalWrite(2, hw_value_io_0 ? HIGH : LOW);
  pinMode(3, OUTPUT);
  digitalWrite(3, hw_value_io_1 ? HIGH : LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, hw_value_io_2 ? HIGH : LOW);
  hw_servo_io_3.attach(5);
  if (isfinite((double)hw_value_io_3) && hw_value_io_3 >= 0 && hw_value_io_3 <= 180) {
    int command = (int)hw_value_io_3;
    if (command != hw_servo_previous_io_3) {
      hw_servo_io_3.write(command); hw_servo_previous_io_3 = command;
    }
  }
  hw_servo_io_4.attach(6);
  if (isfinite((double)hw_value_io_4) && hw_value_io_4 >= 0 && hw_value_io_4 <= 180) {
    int command = (int)hw_value_io_4;
    if (command != hw_servo_previous_io_4) {
      hw_servo_io_4.write(command); hw_servo_previous_io_4 = command;
    }
  }
  hw_servo_io_5.attach(7);
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
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(&hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5);
  digitalWrite(2, hw_value_io_0 ? HIGH : LOW);
  digitalWrite(3, hw_value_io_1 ? HIGH : LOW);
  digitalWrite(4, hw_value_io_2 ? HIGH : LOW);
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
  if (isfinite((double)hw_value_io_5) && hw_value_io_5 >= 0 && hw_value_io_5 <= 180) {
    int command = (int)hw_value_io_5;
    if (command != hw_servo_previous_io_5) {
      hw_servo_io_5.write(command); hw_servo_previous_io_5 = command;
    }
  }
}
