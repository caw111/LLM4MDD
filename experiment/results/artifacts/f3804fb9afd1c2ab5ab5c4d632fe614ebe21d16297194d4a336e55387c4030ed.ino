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
extern int32_t sysml_state_countdown;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double* io_0, double* io_1, double* io_2, double* io_3, double* io_4, double* io_5, double* io_6, double* io_7, double* io_8, int32_t* io_9, double* io_10, double* io_11, double* io_12);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_countdown = 9;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_countdown = 9;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double* io_0, double* io_1, double* io_2, double* io_3, double* io_4, double* io_5, double* io_6, double* io_7, double* io_8, int32_t* io_9, double* io_10, double* io_11, double* io_12) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_value_0 = 0;
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    double sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_lazy_10 = 0;
    bool sysml_lazy_11 = 0;
    double sysml_conditional_12 = 0;
    bool sysml_lazy_13 = 0;
    bool sysml_lazy_14 = 0;
    bool sysml_lazy_15 = 0;
    bool sysml_lazy_16 = 0;
    double sysml_conditional_17 = 0;
    bool sysml_lazy_18 = 0;
    bool sysml_lazy_19 = 0;
    bool sysml_lazy_20 = 0;
    bool sysml_lazy_21 = 0;
    bool sysml_lazy_22 = 0;
    double sysml_conditional_23 = 0;
    bool sysml_lazy_24 = 0;
    bool sysml_lazy_25 = 0;
    bool sysml_lazy_26 = 0;
    bool sysml_lazy_27 = 0;
    bool sysml_lazy_28 = 0;
    bool sysml_lazy_29 = 0;
    double sysml_conditional_30 = 0;
    bool sysml_lazy_31 = 0;
    bool sysml_lazy_32 = 0;
    bool sysml_lazy_33 = 0;
    bool sysml_lazy_34 = 0;
    double sysml_conditional_35 = 0;
    bool sysml_lazy_36 = 0;
    bool sysml_lazy_37 = 0;
    bool sysml_lazy_38 = 0;
    bool sysml_lazy_39 = 0;
    bool sysml_lazy_40 = 0;
    double sysml_conditional_41 = 0;
    bool sysml_lazy_42 = 0;
    bool sysml_lazy_43 = 0;
    bool sysml_lazy_44 = 0;
    bool sysml_lazy_45 = 0;
    bool sysml_lazy_46 = 0;
    bool sysml_lazy_47 = 0;
    double sysml_conditional_48 = 0;
    bool sysml_lazy_49 = 0;
    bool sysml_lazy_50 = 0;
    bool sysml_lazy_51 = 0;
    bool sysml_lazy_52 = 0;
    double sysml_conditional_53 = 0;
    bool sysml_lazy_54 = 0;
    double sysml_conditional_55 = 0;
    bool sysml_lazy_56 = 0;
    double sysml_conditional_57 = 0;
    bool sysml_lazy_58 = 0;
    double sysml_conditional_59 = 0;
    bool sysml_lazy_60 = 0;
    double sysml_conditional_61 = 0;
    bool sysml_lazy_62 = 0;
    double sysml_conditional_63 = 0;
    bool sysml_lazy_64 = 0;
    double sysml_conditional_65 = 0;
    bool sysml_lazy_66 = 0;
    double sysml_conditional_67 = 0;
    bool sysml_lazy_68 = 0;
    double sysml_conditional_69 = 0;
    bool sysml_lazy_70 = 0;
    double sysml_conditional_71 = 0;
    sysml_lazy_0 = sysml_state_countdown == 9;
    if (!sysml_lazy_0) {
        sysml_lazy_0 = sysml_state_countdown == 8;
    }
    sysml_lazy_1 = sysml_lazy_0;
    if (!sysml_lazy_1) {
        sysml_lazy_1 = sysml_state_countdown == 1;
    }
    sysml_lazy_2 = sysml_lazy_1;
    if (!sysml_lazy_2) {
        sysml_lazy_2 = sysml_state_countdown == 0;
    }
    sysml_lazy_3 = sysml_lazy_2;
    if (!sysml_lazy_3) {
        sysml_lazy_3 = sysml_state_countdown == 3;
    }
    sysml_lazy_4 = sysml_lazy_3;
    if (!sysml_lazy_4) {
        sysml_lazy_4 = sysml_state_countdown == 2;
    }
    if (sysml_lazy_4) {
        sysml_conditional_5 = 255.0;
    }
    else {
        sysml_conditional_5 = 0.0;
    }
    sysml_value_0 = sysml_conditional_5;
    double sysml_value_1 = 0;
    sysml_lazy_6 = sysml_state_countdown == 9;
    if (!sysml_lazy_6) {
        sysml_lazy_6 = sysml_state_countdown == 8;
    }
    sysml_lazy_7 = sysml_lazy_6;
    if (!sysml_lazy_7) {
        sysml_lazy_7 = sysml_state_countdown == 1;
    }
    sysml_lazy_8 = sysml_lazy_7;
    if (!sysml_lazy_8) {
        sysml_lazy_8 = sysml_state_countdown == 0;
    }
    sysml_lazy_9 = sysml_lazy_8;
    if (!sysml_lazy_9) {
        sysml_lazy_9 = sysml_state_countdown == 4;
    }
    sysml_lazy_10 = sysml_lazy_9;
    if (!sysml_lazy_10) {
        sysml_lazy_10 = sysml_state_countdown == 5;
    }
    sysml_lazy_11 = sysml_lazy_10;
    if (!sysml_lazy_11) {
        sysml_lazy_11 = sysml_state_countdown == 6;
    }
    if (sysml_lazy_11) {
        sysml_conditional_12 = 255.0;
    }
    else {
        sysml_conditional_12 = 0.0;
    }
    sysml_value_1 = sysml_conditional_12;
    double sysml_value_2 = 0;
    sysml_lazy_13 = sysml_state_countdown == 7;
    if (!sysml_lazy_13) {
        sysml_lazy_13 = sysml_state_countdown == 4;
    }
    sysml_lazy_14 = sysml_lazy_13;
    if (!sysml_lazy_14) {
        sysml_lazy_14 = sysml_state_countdown == 0;
    }
    sysml_lazy_15 = sysml_lazy_14;
    if (!sysml_lazy_15) {
        sysml_lazy_15 = sysml_state_countdown == 3;
    }
    sysml_lazy_16 = sysml_lazy_15;
    if (!sysml_lazy_16) {
        sysml_lazy_16 = sysml_state_countdown == 5;
    }
    if (sysml_lazy_16) {
        sysml_conditional_17 = 255.0;
    }
    else {
        sysml_conditional_17 = 0.0;
    }
    sysml_value_2 = sysml_conditional_17;
    double sysml_value_3 = 0;
    sysml_lazy_18 = sysml_state_countdown == 9;
    if (!sysml_lazy_18) {
        sysml_lazy_18 = sysml_state_countdown == 8;
    }
    sysml_lazy_19 = sysml_lazy_18;
    if (!sysml_lazy_19) {
        sysml_lazy_19 = sysml_state_countdown == 1;
    }
    sysml_lazy_20 = sysml_lazy_19;
    if (!sysml_lazy_20) {
        sysml_lazy_20 = sysml_state_countdown == 0;
    }
    sysml_lazy_21 = sysml_lazy_20;
    if (!sysml_lazy_21) {
        sysml_lazy_21 = sysml_state_countdown == 3;
    }
    sysml_lazy_22 = sysml_lazy_21;
    if (!sysml_lazy_22) {
        sysml_lazy_22 = sysml_state_countdown == 2;
    }
    if (sysml_lazy_22) {
        sysml_conditional_23 = 255.0;
    }
    else {
        sysml_conditional_23 = 0.0;
    }
    sysml_value_3 = sysml_conditional_23;
    double sysml_value_4 = 0;
    sysml_lazy_24 = sysml_state_countdown == 9;
    if (!sysml_lazy_24) {
        sysml_lazy_24 = sysml_state_countdown == 8;
    }
    sysml_lazy_25 = sysml_lazy_24;
    if (!sysml_lazy_25) {
        sysml_lazy_25 = sysml_state_countdown == 1;
    }
    sysml_lazy_26 = sysml_lazy_25;
    if (!sysml_lazy_26) {
        sysml_lazy_26 = sysml_state_countdown == 0;
    }
    sysml_lazy_27 = sysml_lazy_26;
    if (!sysml_lazy_27) {
        sysml_lazy_27 = sysml_state_countdown == 4;
    }
    sysml_lazy_28 = sysml_lazy_27;
    if (!sysml_lazy_28) {
        sysml_lazy_28 = sysml_state_countdown == 5;
    }
    sysml_lazy_29 = sysml_lazy_28;
    if (!sysml_lazy_29) {
        sysml_lazy_29 = sysml_state_countdown == 6;
    }
    if (sysml_lazy_29) {
        sysml_conditional_30 = 255.0;
    }
    else {
        sysml_conditional_30 = 0.0;
    }
    sysml_value_4 = sysml_conditional_30;
    double sysml_value_5 = 0;
    sysml_lazy_31 = sysml_state_countdown == 7;
    if (!sysml_lazy_31) {
        sysml_lazy_31 = sysml_state_countdown == 4;
    }
    sysml_lazy_32 = sysml_lazy_31;
    if (!sysml_lazy_32) {
        sysml_lazy_32 = sysml_state_countdown == 0;
    }
    sysml_lazy_33 = sysml_lazy_32;
    if (!sysml_lazy_33) {
        sysml_lazy_33 = sysml_state_countdown == 3;
    }
    sysml_lazy_34 = sysml_lazy_33;
    if (!sysml_lazy_34) {
        sysml_lazy_34 = sysml_state_countdown == 5;
    }
    if (sysml_lazy_34) {
        sysml_conditional_35 = 255.0;
    }
    else {
        sysml_conditional_35 = 0.0;
    }
    sysml_value_5 = sysml_conditional_35;
    double sysml_value_6 = 0;
    sysml_lazy_36 = sysml_state_countdown == 9;
    if (!sysml_lazy_36) {
        sysml_lazy_36 = sysml_state_countdown == 8;
    }
    sysml_lazy_37 = sysml_lazy_36;
    if (!sysml_lazy_37) {
        sysml_lazy_37 = sysml_state_countdown == 1;
    }
    sysml_lazy_38 = sysml_lazy_37;
    if (!sysml_lazy_38) {
        sysml_lazy_38 = sysml_state_countdown == 0;
    }
    sysml_lazy_39 = sysml_lazy_38;
    if (!sysml_lazy_39) {
        sysml_lazy_39 = sysml_state_countdown == 3;
    }
    sysml_lazy_40 = sysml_lazy_39;
    if (!sysml_lazy_40) {
        sysml_lazy_40 = sysml_state_countdown == 2;
    }
    if (sysml_lazy_40) {
        sysml_conditional_41 = 255.0;
    }
    else {
        sysml_conditional_41 = 0.0;
    }
    sysml_value_6 = sysml_conditional_41;
    double sysml_value_7 = 0;
    sysml_lazy_42 = sysml_state_countdown == 9;
    if (!sysml_lazy_42) {
        sysml_lazy_42 = sysml_state_countdown == 8;
    }
    sysml_lazy_43 = sysml_lazy_42;
    if (!sysml_lazy_43) {
        sysml_lazy_43 = sysml_state_countdown == 1;
    }
    sysml_lazy_44 = sysml_lazy_43;
    if (!sysml_lazy_44) {
        sysml_lazy_44 = sysml_state_countdown == 0;
    }
    sysml_lazy_45 = sysml_lazy_44;
    if (!sysml_lazy_45) {
        sysml_lazy_45 = sysml_state_countdown == 4;
    }
    sysml_lazy_46 = sysml_lazy_45;
    if (!sysml_lazy_46) {
        sysml_lazy_46 = sysml_state_countdown == 5;
    }
    sysml_lazy_47 = sysml_lazy_46;
    if (!sysml_lazy_47) {
        sysml_lazy_47 = sysml_state_countdown == 6;
    }
    if (sysml_lazy_47) {
        sysml_conditional_48 = 255.0;
    }
    else {
        sysml_conditional_48 = 0.0;
    }
    sysml_value_7 = sysml_conditional_48;
    double sysml_value_8 = 0;
    sysml_lazy_49 = sysml_state_countdown == 7;
    if (!sysml_lazy_49) {
        sysml_lazy_49 = sysml_state_countdown == 4;
    }
    sysml_lazy_50 = sysml_lazy_49;
    if (!sysml_lazy_50) {
        sysml_lazy_50 = sysml_state_countdown == 0;
    }
    sysml_lazy_51 = sysml_lazy_50;
    if (!sysml_lazy_51) {
        sysml_lazy_51 = sysml_state_countdown == 3;
    }
    sysml_lazy_52 = sysml_lazy_51;
    if (!sysml_lazy_52) {
        sysml_lazy_52 = sysml_state_countdown == 5;
    }
    if (sysml_lazy_52) {
        sysml_conditional_53 = 255.0;
    }
    else {
        sysml_conditional_53 = 0.0;
    }
    sysml_value_8 = sysml_conditional_53;
    int32_t sysml_value_9 = 0;
    sysml_value_9 = sysml_state_countdown;
    double sysml_value_10 = 0;
    sysml_lazy_54 = sysml_state_countdown >= 7;
    if (sysml_lazy_54) {
        sysml_lazy_54 = sysml_state_countdown <= 9;
    }
    if (sysml_lazy_54) {
        sysml_conditional_55 = 0.0;
    }
    else {
        sysml_lazy_56 = sysml_state_countdown >= 4;
        if (sysml_lazy_56) {
            sysml_lazy_56 = sysml_state_countdown <= 6;
        }
        if (sysml_lazy_56) {
            sysml_conditional_57 = 60.0;
        }
        else {
            sysml_lazy_58 = sysml_state_countdown >= 1;
            if (sysml_lazy_58) {
                sysml_lazy_58 = sysml_state_countdown <= 3;
            }
            if (sysml_lazy_58) {
                sysml_conditional_59 = 120.0;
            }
            else {
                sysml_conditional_59 = 180.0;
            }
            sysml_conditional_57 = sysml_conditional_59;
        }
        sysml_conditional_55 = sysml_conditional_57;
    }
    sysml_value_10 = sysml_conditional_55;
    double sysml_value_11 = 0;
    sysml_lazy_60 = sysml_state_countdown >= 7;
    if (sysml_lazy_60) {
        sysml_lazy_60 = sysml_state_countdown <= 9;
    }
    if (sysml_lazy_60) {
        sysml_conditional_61 = 0.0;
    }
    else {
        sysml_lazy_62 = sysml_state_countdown >= 4;
        if (sysml_lazy_62) {
            sysml_lazy_62 = sysml_state_countdown <= 6;
        }
        if (sysml_lazy_62) {
            sysml_conditional_63 = 60.0;
        }
        else {
            sysml_lazy_64 = sysml_state_countdown >= 1;
            if (sysml_lazy_64) {
                sysml_lazy_64 = sysml_state_countdown <= 3;
            }
            if (sysml_lazy_64) {
                sysml_conditional_65 = 120.0;
            }
            else {
                sysml_conditional_65 = 180.0;
            }
            sysml_conditional_63 = sysml_conditional_65;
        }
        sysml_conditional_61 = sysml_conditional_63;
    }
    sysml_value_11 = sysml_conditional_61;
    double sysml_value_12 = 0;
    sysml_lazy_66 = sysml_state_countdown >= 7;
    if (sysml_lazy_66) {
        sysml_lazy_66 = sysml_state_countdown <= 9;
    }
    if (sysml_lazy_66) {
        sysml_conditional_67 = 0.0;
    }
    else {
        sysml_lazy_68 = sysml_state_countdown >= 4;
        if (sysml_lazy_68) {
            sysml_lazy_68 = sysml_state_countdown <= 6;
        }
        if (sysml_lazy_68) {
            sysml_conditional_69 = 60.0;
        }
        else {
            sysml_lazy_70 = sysml_state_countdown >= 1;
            if (sysml_lazy_70) {
                sysml_lazy_70 = sysml_state_countdown <= 3;
            }
            if (sysml_lazy_70) {
                sysml_conditional_71 = 120.0;
            }
            else {
                sysml_conditional_71 = 180.0;
            }
            sysml_conditional_69 = sysml_conditional_71;
        }
        sysml_conditional_67 = sysml_conditional_69;
    }
    sysml_value_12 = sysml_conditional_67;
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
    *io_12 = sysml_value_12;
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
static int32_t hw_value_io_9 = 9;
static void hw_shift595_io_9(int32_t value) {
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
static int32_t hw_shift595_previous_io_9 = INT32_MIN;
static double hw_value_io_10 = 0;
Servo hw_servo_io_10;
static int hw_servo_previous_io_10 = -1;
static double hw_value_io_11 = 0;
Servo hw_servo_io_11;
static int hw_servo_previous_io_11 = -1;
static double hw_value_io_12 = 0;
Servo hw_servo_io_12;
static int hw_servo_previous_io_12 = -1;

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
  pinMode(A0, OUTPUT); digitalWrite(A0, LOW);
  pinMode(A2, OUTPUT); digitalWrite(A2, LOW);
  pinMode(A1, OUTPUT); digitalWrite(A1, LOW);
  if (hw_value_io_9 != hw_shift595_previous_io_9) {
    hw_shift595_io_9(hw_value_io_9); hw_shift595_previous_io_9 = hw_value_io_9;
  }
  hw_servo_io_10.attach(11);
  if (isfinite((double)hw_value_io_10) && hw_value_io_10 >= 0 && hw_value_io_10 <= 180) {
    int command = (int)hw_value_io_10;
    if (command != hw_servo_previous_io_10) {
      hw_servo_io_10.write(command); hw_servo_previous_io_10 = command;
    }
  }
  hw_servo_io_11.attach(12);
  if (isfinite((double)hw_value_io_11) && hw_value_io_11 >= 0 && hw_value_io_11 <= 180) {
    int command = (int)hw_value_io_11;
    if (command != hw_servo_previous_io_11) {
      hw_servo_io_11.write(command); hw_servo_previous_io_11 = command;
    }
  }
  hw_servo_io_12.attach(13);
  if (isfinite((double)hw_value_io_12) && hw_value_io_12 >= 0 && hw_value_io_12 <= 180) {
    int command = (int)hw_value_io_12;
    if (command != hw_servo_previous_io_12) {
      hw_servo_io_12.write(command); hw_servo_previous_io_12 = command;
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
  ProgramPage1(&hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_io_7, &hw_value_io_8, &hw_value_io_9, &hw_value_io_10, &hw_value_io_11, &hw_value_io_12);
  analogWrite(2, (int)constrain((hw_value_io_0 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(3, (int)constrain((hw_value_io_1 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(4, (int)constrain((hw_value_io_2 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(5, (int)constrain((hw_value_io_3 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(6, (int)constrain((hw_value_io_4 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(7, (int)constrain((hw_value_io_5 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(8, (int)constrain((hw_value_io_6 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(9, (int)constrain((hw_value_io_7 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(10, (int)constrain((hw_value_io_8 * 1.0 + 0.0), 0.0, 255.0));
  if (hw_value_io_9 != hw_shift595_previous_io_9) {
    hw_shift595_io_9(hw_value_io_9); hw_shift595_previous_io_9 = hw_value_io_9;
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
  if (isfinite((double)hw_value_io_12) && hw_value_io_12 >= 0 && hw_value_io_12 <= 180) {
    int command = (int)hw_value_io_12;
    if (command != hw_servo_previous_io_12) {
      hw_servo_io_12.write(command); hw_servo_previous_io_12 = command;
    }
  }
}
