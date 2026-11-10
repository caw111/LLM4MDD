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
extern bool sysml_state_servo3Phase;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, double* io_2, double* io_3, bool* io_4, double* io_5, double* io_6, bool* io_7, double* io_8, double* io_9, bool* io_10, double* io_11, double* io_12, double* io_13);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_servo3Phase = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_servo3Phase = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, double* io_2, double* io_3, bool* io_4, double* io_5, double* io_6, bool* io_7, double* io_8, double* io_9, bool* io_10, double* io_11, double* io_12, double* io_13) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_conditional_0 = 0;
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
    sysml_state_servo3Phase = sysml_state_servo3Phase;
    double sysml_value_0 = 0;
    if (true) {
        if (io_0 <= 255.0) {
            sysml_conditional_1 = io_0;
        }
        else {
            if (io_0 <= 1023.0) {
                sysml_conditional_2 = ((double)(io_0 * 255.0)) / 1023.0;
            }
            else {
                sysml_conditional_2 = 255.0;
            }
            sysml_conditional_1 = sysml_conditional_2;
        }
        sysml_conditional_0 = sysml_conditional_1;
    }
    else {
        sysml_conditional_0 = 0.0;
    }
    sysml_value_0 = sysml_conditional_0;
    double sysml_value_1 = 0;
    if (true) {
        if (io_0 <= 255.0) {
            sysml_conditional_4 = io_0;
        }
        else {
            if (io_0 <= 1023.0) {
                sysml_conditional_5 = ((double)(io_0 * 255.0)) / 1023.0;
            }
            else {
                sysml_conditional_5 = 255.0;
            }
            sysml_conditional_4 = sysml_conditional_5;
        }
        sysml_conditional_3 = sysml_conditional_4;
    }
    else {
        sysml_conditional_3 = 0.0;
    }
    sysml_value_1 = sysml_conditional_3;
    bool sysml_value_2 = 0;
    sysml_value_2 = false;
    double sysml_value_3 = 0;
    if (true) {
        if (io_1 <= 255.0) {
            sysml_conditional_7 = io_1;
        }
        else {
            if (io_1 <= 1023.0) {
                sysml_conditional_8 = ((double)(io_1 * 255.0)) / 1023.0;
            }
            else {
                sysml_conditional_8 = 255.0;
            }
            sysml_conditional_7 = sysml_conditional_8;
        }
        sysml_conditional_6 = sysml_conditional_7;
    }
    else {
        sysml_conditional_6 = 0.0;
    }
    sysml_value_3 = sysml_conditional_6;
    double sysml_value_4 = 0;
    if (true) {
        if (io_1 <= 255.0) {
            sysml_conditional_10 = io_1;
        }
        else {
            if (io_1 <= 1023.0) {
                sysml_conditional_11 = ((double)(io_1 * 255.0)) / 1023.0;
            }
            else {
                sysml_conditional_11 = 255.0;
            }
            sysml_conditional_10 = sysml_conditional_11;
        }
        sysml_conditional_9 = sysml_conditional_10;
    }
    else {
        sysml_conditional_9 = 0.0;
    }
    sysml_value_4 = sysml_conditional_9;
    bool sysml_value_5 = 0;
    sysml_value_5 = false;
    double sysml_value_6 = 0;
    if (true) {
        if (io_0 <= 255.0) {
            sysml_conditional_13 = io_0;
        }
        else {
            if (io_0 <= 1023.0) {
                sysml_conditional_14 = ((double)(io_0 * 255.0)) / 1023.0;
            }
            else {
                sysml_conditional_14 = 255.0;
            }
            sysml_conditional_13 = sysml_conditional_14;
        }
        sysml_conditional_12 = sysml_conditional_13;
    }
    else {
        sysml_conditional_12 = 0.0;
    }
    sysml_value_6 = sysml_conditional_12;
    double sysml_value_7 = 0;
    if (true) {
        if (io_1 <= 255.0) {
            sysml_conditional_16 = io_1;
        }
        else {
            if (io_1 <= 1023.0) {
                sysml_conditional_17 = ((double)(io_1 * 255.0)) / 1023.0;
            }
            else {
                sysml_conditional_17 = 255.0;
            }
            sysml_conditional_16 = sysml_conditional_17;
        }
        sysml_conditional_15 = sysml_conditional_16;
    }
    else {
        sysml_conditional_15 = 0.0;
    }
    sysml_value_7 = sysml_conditional_15;
    bool sysml_value_8 = 0;
    sysml_value_8 = false;
    double sysml_value_9 = 0;
    if (io_0 <= 180.0) {
        sysml_conditional_18 = io_0;
    }
    else {
        if (io_0 <= 1023.0) {
            sysml_conditional_19 = ((double)(io_0 * 180.0)) / 1023.0;
        }
        else {
            sysml_conditional_19 = 180.0;
        }
        sysml_conditional_18 = sysml_conditional_19;
    }
    sysml_value_9 = sysml_conditional_18;
    double sysml_value_10 = 0;
    if (io_1 <= 180.0) {
        sysml_conditional_20 = io_1;
    }
    else {
        if (io_1 <= 1023.0) {
            sysml_conditional_21 = ((double)(io_1 * 180.0)) / 1023.0;
        }
        else {
            sysml_conditional_21 = 180.0;
        }
        sysml_conditional_20 = sysml_conditional_21;
    }
    sysml_value_10 = sysml_conditional_20;
    double sysml_value_11 = 0;
    if (sysml_state_servo3Phase) {
        sysml_conditional_22 = 180.0;
    }
    else {
        sysml_conditional_22 = 0.0;
    }
    sysml_value_11 = sysml_conditional_22;
    *io_2 = sysml_value_0;
    *io_3 = sysml_value_1;
    *io_4 = sysml_value_2;
    *io_5 = sysml_value_3;
    *io_6 = sysml_value_4;
    *io_7 = sysml_value_5;
    *io_8 = sysml_value_6;
    *io_9 = sysml_value_7;
    *io_10 = sysml_value_8;
    *io_11 = sysml_value_9;
    *io_12 = sysml_value_10;
    *io_13 = sysml_value_11;
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
static uint32_t hw_sampled_io_0 = 0;
static bool hw_attempted_io_0 = false;
static double hw_value_io_1 = 0;
static uint32_t hw_sampled_io_1 = 0;
static bool hw_attempted_io_1 = false;
static double hw_value_io_2 = 0;
static double hw_value_io_3 = 0;
static bool hw_value_io_4 = false;
static double hw_value_io_5 = 0;
static double hw_value_io_6 = 0;
static bool hw_value_io_7 = false;
static double hw_value_io_8 = 0;
static double hw_value_io_9 = 0;
static bool hw_value_io_10 = false;
static double hw_value_io_11 = 0;
Servo hw_servo_io_11;
static int hw_servo_previous_io_11 = -1;
static double hw_value_io_12 = 0;
Servo hw_servo_io_12;
static int hw_servo_previous_io_12 = -1;
static double hw_value_io_13 = 0;
Servo hw_servo_io_13;
static int hw_servo_previous_io_13 = -1;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(2, OUTPUT);
  analogWrite(2, (int)constrain((hw_value_io_2 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(3, OUTPUT);
  analogWrite(3, (int)constrain((hw_value_io_3 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(4, OUTPUT);
  digitalWrite(4, hw_value_io_4 ? HIGH : LOW);
  pinMode(5, OUTPUT);
  analogWrite(5, (int)constrain((hw_value_io_5 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(6, OUTPUT);
  analogWrite(6, (int)constrain((hw_value_io_6 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(7, OUTPUT);
  digitalWrite(7, hw_value_io_7 ? HIGH : LOW);
  pinMode(8, OUTPUT);
  analogWrite(8, (int)constrain((hw_value_io_8 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(9, OUTPUT);
  analogWrite(9, (int)constrain((hw_value_io_9 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(10, OUTPUT);
  digitalWrite(10, hw_value_io_10 ? HIGH : LOW);
  hw_servo_io_11.attach(11);
  if (isfinite((double)hw_value_io_11) && hw_value_io_11 >= 0 && hw_value_io_11 <= 180) {
    int command = (int)hw_value_io_11;
    if (command != hw_servo_previous_io_11) {
      hw_servo_io_11.write(command); hw_servo_previous_io_11 = command;
    }
  }
  hw_servo_io_12.attach(12);
  if (isfinite((double)hw_value_io_12) && hw_value_io_12 >= 0 && hw_value_io_12 <= 180) {
    int command = (int)hw_value_io_12;
    if (command != hw_servo_previous_io_12) {
      hw_servo_io_12.write(command); hw_servo_previous_io_12 = command;
    }
  }
  hw_servo_io_13.attach(13);
  if (isfinite((double)hw_value_io_13) && hw_value_io_13 >= 0 && hw_value_io_13 <= 180) {
    int command = (int)hw_value_io_13;
    if (command != hw_servo_previous_io_13) {
      hw_servo_io_13.write(command); hw_servo_previous_io_13 = command;
    }
  }
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 10UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (analogRead(A3) * 1.0 + 0.0);
  }
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 10UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = (analogRead(A4) * 1.0 + 0.0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_io_7, &hw_value_io_8, &hw_value_io_9, &hw_value_io_10, &hw_value_io_11, &hw_value_io_12, &hw_value_io_13);
  analogWrite(2, (int)constrain((hw_value_io_2 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(3, (int)constrain((hw_value_io_3 * 1.0 + 0.0), 0.0, 255.0));
  digitalWrite(4, hw_value_io_4 ? HIGH : LOW);
  analogWrite(5, (int)constrain((hw_value_io_5 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(6, (int)constrain((hw_value_io_6 * 1.0 + 0.0), 0.0, 255.0));
  digitalWrite(7, hw_value_io_7 ? HIGH : LOW);
  analogWrite(8, (int)constrain((hw_value_io_8 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(9, (int)constrain((hw_value_io_9 * 1.0 + 0.0), 0.0, 255.0));
  digitalWrite(10, hw_value_io_10 ? HIGH : LOW);
  if (isfinite((double)hw_value_io_11) && hw_value_io_11 >= 0 && hw_value_io_11 <= 180) {
    int command = (int)hw_value_io_11;
    if (command != hw_servo_previous_io_11) {
      hw_servo_io_11.write(command); hw_servo_previous_io_11 = command;
    }
  }
  if (isfinite((double)hw_value_io_12) && hw_value_io_12 >= 0 && hw_value_io_12 <= 180) {
    int command = (int)hw_value_io_12;
    if (command != hw_servo_previous_io_12) {
      hw_servo_io_12.write(command); hw_servo_previous_io_12 = command;
    }
  }
  if (isfinite((double)hw_value_io_13) && hw_value_io_13 >= 0 && hw_value_io_13 <= 180) {
    int command = (int)hw_value_io_13;
    if (command != hw_servo_previous_io_13) {
      hw_servo_io_13.write(command); hw_servo_previous_io_13 = command;
    }
  }
}
