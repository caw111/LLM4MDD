#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_78\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
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
extern int32_t sysml_state_number;
/* Global Variable define of  */
extern bool sysml_state_prevBtn1;
/* Global Variable define of  */
extern bool sysml_state_prevBtn2;
/* Global Variable define of  */
extern int32_t sysml_state_accIncMs;
/* Global Variable define of  */
extern int32_t sysml_state_accDecMs;
/* Global Variable define of  */
extern bool sysml_state_initialized;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double io_2, double io_3, int32_t io_5, int32_t* io_4);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_number = 0;
/* Global Variable define of  */
bool sysml_state_prevBtn1 = false;
/* Global Variable define of  */
bool sysml_state_prevBtn2 = false;
/* Global Variable define of  */
int32_t sysml_state_accIncMs = 0;
/* Global Variable define of  */
int32_t sysml_state_accDecMs = 0;
/* Global Variable define of  */
bool sysml_state_initialized = false;
/* Init function of CompositeActor named "ProgramPage1" */
#line 443 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_78\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
static void hw_shift595_io_4(int32_t value);
#line 478 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_78\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 492 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_78\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 49 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_78\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_number = 0;
    sysml_state_prevBtn1 = false;
    sysml_state_prevBtn2 = false;
    sysml_state_accIncMs = 0;
    sysml_state_accDecMs = 0;
    sysml_state_initialized = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double io_2, double io_3, int32_t io_5, int32_t* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int32_t sysml_conditional_0 = 0;
    int32_t sysml_conditional_1 = 0;
    int32_t sysml_conditional_2 = 0;
    bool sysml_lazy_3 = 0;
    int32_t sysml_conditional_4 = 0;
    int32_t sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    int32_t sysml_conditional_7 = 0;
    int32_t sysml_conditional_8 = 0;
    bool sysml_lazy_9 = 0;
    int32_t sysml_conditional_10 = 0;
    bool sysml_lazy_11 = 0;
    int32_t sysml_conditional_12 = 0;
    int32_t sysml_conditional_13 = 0;
    int32_t sysml_conditional_14 = 0;
    int32_t sysml_conditional_15 = 0;
    int32_t sysml_conditional_16 = 0;
    int32_t sysml_conditional_17 = 0;
    int32_t sysml_conditional_18 = 0;
    int32_t sysml_conditional_19 = 0;
    int32_t sysml_conditional_20 = 0;
    int32_t sysml_conditional_21 = 0;
    int32_t sysml_conditional_22 = 0;
    int32_t sysml_conditional_23 = 0;
    int32_t sysml_conditional_24 = 0;
    int32_t sysml_conditional_25 = 0;
    int32_t sysml_conditional_26 = 0;
    int32_t sysml_conditional_27 = 0;
    int32_t sysml_conditional_28 = 0;
    bool sysml_lazy_29 = 0;
    int32_t sysml_conditional_30 = 0;
    bool sysml_lazy_31 = 0;
    int32_t sysml_conditional_32 = 0;
    int32_t sysml_conditional_33 = 0;
    int32_t sysml_conditional_34 = 0;
    int32_t sysml_conditional_35 = 0;
    int32_t sysml_conditional_36 = 0;
    int32_t sysml_conditional_37 = 0;
    int32_t sysml_conditional_38 = 0;
    int32_t sysml_conditional_39 = 0;
    int32_t sysml_conditional_40 = 0;
    int32_t sysml_conditional_41 = 0;
    int32_t sysml_conditional_42 = 0;
    int32_t sysml_conditional_43 = 0;
    int32_t sysml_conditional_44 = 0;
    int32_t sysml_conditional_45 = 0;
    int32_t sysml_conditional_46 = 0;
    int32_t sysml_conditional_47 = 0;
    int32_t sysml_conditional_48 = 0;
    if ((!sysml_state_initialized)) {
        if (io_2 >= 0.0) {
            if ((((double)(io_3 * 99.0)) / 1023.0) < ((double)((int32_t)(((double)(io_3 * 99.0)) / 1023.0)))) {
                sysml_conditional_1 = 1;
            }
            else {
                sysml_conditional_1 = 0;
            }
            if ((((double)(io_2 * 99.0)) / 1023.0) < ((double)((int32_t)(((double)(io_2 * 99.0)) / 1023.0)))) {
                sysml_conditional_2 = 1;
            }
            else {
                sysml_conditional_2 = 0;
            }
            sysml_conditional_0 = ((((int32_t)(((double)(io_3 * 99.0)) / 1023.0)) - sysml_conditional_1) * 10) + (((int32_t)(((double)(io_2 * 99.0)) / 1023.0)) - sysml_conditional_2);
        }
        else {
            sysml_conditional_0 = 0;
        }
        sysml_state_number = sysml_conditional_0;
        sysml_state_initialized = true;
        sysml_state_prevBtn1 = io_0;
        sysml_state_prevBtn2 = io_1;
    }
    sysml_lazy_3 = io_0;
    if (sysml_lazy_3) {
        sysml_lazy_3 = !sysml_state_prevBtn1;
    }
    if (sysml_lazy_3) {
        sysml_conditional_4 = 0;
    }
    else {
        if (io_0) {
            sysml_conditional_5 = sysml_state_accIncMs + io_5;
        }
        else {
            sysml_conditional_5 = 0;
        }
        sysml_conditional_4 = sysml_conditional_5;
    }
    sysml_state_accIncMs = sysml_conditional_4;
    sysml_lazy_6 = io_1;
    if (sysml_lazy_6) {
        sysml_lazy_6 = !sysml_state_prevBtn2;
    }
    if (sysml_lazy_6) {
        sysml_conditional_7 = 0;
    }
    else {
        if (io_1) {
            sysml_conditional_8 = sysml_state_accDecMs + io_5;
        }
        else {
            sysml_conditional_8 = 0;
        }
        sysml_conditional_7 = sysml_conditional_8;
    }
    sysml_state_accDecMs = sysml_conditional_7;
    sysml_lazy_9 = io_0;
    if (sysml_lazy_9) {
        sysml_lazy_9 = !sysml_state_prevBtn1;
    }
    if (sysml_lazy_9) {
        if (sysml_state_number == 99) {
            sysml_conditional_10 = 0;
        }
        else {
            sysml_conditional_10 = sysml_state_number + 1;
        }
        sysml_state_number = sysml_conditional_10;
    }
    sysml_lazy_11 = io_0;
    if (sysml_lazy_11) {
        if ((((double)(io_3 * 99.0)) / 1023.0) < ((double)((int32_t)(((double)(io_3 * 99.0)) / 1023.0)))) {
            sysml_conditional_12 = 1;
        }
        else {
            sysml_conditional_12 = 0;
        }
        if ((((int32_t)(((double)(io_3 * 99.0)) / 1023.0)) - sysml_conditional_12) == 0) {
            sysml_conditional_13 = 100;
        }
        else {
            if ((((double)(io_3 * 99.0)) / 1023.0) < ((double)((int32_t)(((double)(io_3 * 99.0)) / 1023.0)))) {
                sysml_conditional_14 = 1;
            }
            else {
                sysml_conditional_14 = 0;
            }
            if ((((int32_t)(((double)(io_3 * 99.0)) / 1023.0)) - sysml_conditional_14) >= 99) {
                sysml_conditional_15 = 1000;
            }
            else {
                if ((((double)(io_3 * 900.0)) / 1023.0) < ((double)((int32_t)(((double)(io_3 * 900.0)) / 1023.0)))) {
                    sysml_conditional_16 = 1;
                }
                else {
                    sysml_conditional_16 = 0;
                }
                if ((((double)(io_3 * 900.0)) / 1023.0) < ((double)((int32_t)(((double)(io_3 * 900.0)) / 1023.0)))) {
                    sysml_conditional_17 = 1;
                }
                else {
                    sysml_conditional_17 = 0;
                }
                if ((((double)(io_3 * 900.0)) / 1023.0) < ((double)((int32_t)(((double)(io_3 * 900.0)) / 1023.0)))) {
                    sysml_conditional_18 = 1;
                }
                else {
                    sysml_conditional_18 = 0;
                }
                if ((100 + ((((int32_t)(((double)(io_3 * 900.0)) / 1023.0)) - sysml_conditional_17) * 1.0)) < ((double)((int32_t)(100 + ((((int32_t)(((double)(io_3 * 900.0)) / 1023.0)) - sysml_conditional_18) * 1.0))))) {
                    sysml_conditional_19 = 1;
                }
                else {
                    sysml_conditional_19 = 0;
                }
                sysml_conditional_15 = ((int32_t)(100 + ((((int32_t)(((double)(io_3 * 900.0)) / 1023.0)) - sysml_conditional_16) * 1.0))) - sysml_conditional_19;
            }
            sysml_conditional_13 = sysml_conditional_15;
        }
        sysml_lazy_11 = sysml_state_accIncMs >= sysml_conditional_13;
    }
    if (sysml_lazy_11) {
        if ((((double)(io_3 * 99.0)) / 1023.0) < ((double)((int32_t)(((double)(io_3 * 99.0)) / 1023.0)))) {
            sysml_conditional_20 = 1;
        }
        else {
            sysml_conditional_20 = 0;
        }
        if ((((int32_t)(((double)(io_3 * 99.0)) / 1023.0)) - sysml_conditional_20) == 0) {
            sysml_conditional_21 = 100;
        }
        else {
            if ((((double)(io_3 * 99.0)) / 1023.0) < ((double)((int32_t)(((double)(io_3 * 99.0)) / 1023.0)))) {
                sysml_conditional_22 = 1;
            }
            else {
                sysml_conditional_22 = 0;
            }
            if ((((int32_t)(((double)(io_3 * 99.0)) / 1023.0)) - sysml_conditional_22) >= 99) {
                sysml_conditional_23 = 1000;
            }
            else {
                if ((((double)(io_3 * 900.0)) / 1023.0) < ((double)((int32_t)(((double)(io_3 * 900.0)) / 1023.0)))) {
                    sysml_conditional_24 = 1;
                }
                else {
                    sysml_conditional_24 = 0;
                }
                if ((((double)(io_3 * 900.0)) / 1023.0) < ((double)((int32_t)(((double)(io_3 * 900.0)) / 1023.0)))) {
                    sysml_conditional_25 = 1;
                }
                else {
                    sysml_conditional_25 = 0;
                }
                if ((((double)(io_3 * 900.0)) / 1023.0) < ((double)((int32_t)(((double)(io_3 * 900.0)) / 1023.0)))) {
                    sysml_conditional_26 = 1;
                }
                else {
                    sysml_conditional_26 = 0;
                }
                if ((100 + ((((int32_t)(((double)(io_3 * 900.0)) / 1023.0)) - sysml_conditional_25) * 1.0)) < ((double)((int32_t)(100 + ((((int32_t)(((double)(io_3 * 900.0)) / 1023.0)) - sysml_conditional_26) * 1.0))))) {
                    sysml_conditional_27 = 1;
                }
                else {
                    sysml_conditional_27 = 0;
                }
                sysml_conditional_23 = ((int32_t)(100 + ((((int32_t)(((double)(io_3 * 900.0)) / 1023.0)) - sysml_conditional_24) * 1.0))) - sysml_conditional_27;
            }
            sysml_conditional_21 = sysml_conditional_23;
        }
        sysml_state_accIncMs = sysml_state_accIncMs - sysml_conditional_21;
        if (sysml_state_number == 99) {
            sysml_conditional_28 = 0;
        }
        else {
            sysml_conditional_28 = sysml_state_number + 1;
        }
        sysml_state_number = sysml_conditional_28;
    }
    sysml_lazy_29 = io_1;
    if (sysml_lazy_29) {
        sysml_lazy_29 = !sysml_state_prevBtn2;
    }
    if (sysml_lazy_29) {
        if (sysml_state_number == 0) {
            sysml_conditional_30 = 99;
        }
        else {
            sysml_conditional_30 = sysml_state_number - 1;
        }
        sysml_state_number = sysml_conditional_30;
    }
    sysml_lazy_31 = io_1;
    if (sysml_lazy_31) {
        if ((((double)(io_2 * 99.0)) / 1023.0) < ((double)((int32_t)(((double)(io_2 * 99.0)) / 1023.0)))) {
            sysml_conditional_32 = 1;
        }
        else {
            sysml_conditional_32 = 0;
        }
        if ((((int32_t)(((double)(io_2 * 99.0)) / 1023.0)) - sysml_conditional_32) == 0) {
            sysml_conditional_33 = 100;
        }
        else {
            if ((((double)(io_2 * 99.0)) / 1023.0) < ((double)((int32_t)(((double)(io_2 * 99.0)) / 1023.0)))) {
                sysml_conditional_34 = 1;
            }
            else {
                sysml_conditional_34 = 0;
            }
            if ((((int32_t)(((double)(io_2 * 99.0)) / 1023.0)) - sysml_conditional_34) >= 99) {
                sysml_conditional_35 = 1000;
            }
            else {
                if ((((double)(io_2 * 900.0)) / 1023.0) < ((double)((int32_t)(((double)(io_2 * 900.0)) / 1023.0)))) {
                    sysml_conditional_36 = 1;
                }
                else {
                    sysml_conditional_36 = 0;
                }
                if ((((double)(io_2 * 900.0)) / 1023.0) < ((double)((int32_t)(((double)(io_2 * 900.0)) / 1023.0)))) {
                    sysml_conditional_37 = 1;
                }
                else {
                    sysml_conditional_37 = 0;
                }
                if ((((double)(io_2 * 900.0)) / 1023.0) < ((double)((int32_t)(((double)(io_2 * 900.0)) / 1023.0)))) {
                    sysml_conditional_38 = 1;
                }
                else {
                    sysml_conditional_38 = 0;
                }
                if ((100 + ((((int32_t)(((double)(io_2 * 900.0)) / 1023.0)) - sysml_conditional_37) * 1.0)) < ((double)((int32_t)(100 + ((((int32_t)(((double)(io_2 * 900.0)) / 1023.0)) - sysml_conditional_38) * 1.0))))) {
                    sysml_conditional_39 = 1;
                }
                else {
                    sysml_conditional_39 = 0;
                }
                sysml_conditional_35 = ((int32_t)(100 + ((((int32_t)(((double)(io_2 * 900.0)) / 1023.0)) - sysml_conditional_36) * 1.0))) - sysml_conditional_39;
            }
            sysml_conditional_33 = sysml_conditional_35;
        }
        sysml_lazy_31 = sysml_state_accDecMs >= sysml_conditional_33;
    }
    if (sysml_lazy_31) {
        if ((((double)(io_2 * 99.0)) / 1023.0) < ((double)((int32_t)(((double)(io_2 * 99.0)) / 1023.0)))) {
            sysml_conditional_40 = 1;
        }
        else {
            sysml_conditional_40 = 0;
        }
        if ((((int32_t)(((double)(io_2 * 99.0)) / 1023.0)) - sysml_conditional_40) == 0) {
            sysml_conditional_41 = 100;
        }
        else {
            if ((((double)(io_2 * 99.0)) / 1023.0) < ((double)((int32_t)(((double)(io_2 * 99.0)) / 1023.0)))) {
                sysml_conditional_42 = 1;
            }
            else {
                sysml_conditional_42 = 0;
            }
            if ((((int32_t)(((double)(io_2 * 99.0)) / 1023.0)) - sysml_conditional_42) >= 99) {
                sysml_conditional_43 = 1000;
            }
            else {
                if ((((double)(io_2 * 900.0)) / 1023.0) < ((double)((int32_t)(((double)(io_2 * 900.0)) / 1023.0)))) {
                    sysml_conditional_44 = 1;
                }
                else {
                    sysml_conditional_44 = 0;
                }
                if ((((double)(io_2 * 900.0)) / 1023.0) < ((double)((int32_t)(((double)(io_2 * 900.0)) / 1023.0)))) {
                    sysml_conditional_45 = 1;
                }
                else {
                    sysml_conditional_45 = 0;
                }
                if ((((double)(io_2 * 900.0)) / 1023.0) < ((double)((int32_t)(((double)(io_2 * 900.0)) / 1023.0)))) {
                    sysml_conditional_46 = 1;
                }
                else {
                    sysml_conditional_46 = 0;
                }
                if ((100 + ((((int32_t)(((double)(io_2 * 900.0)) / 1023.0)) - sysml_conditional_45) * 1.0)) < ((double)((int32_t)(100 + ((((int32_t)(((double)(io_2 * 900.0)) / 1023.0)) - sysml_conditional_46) * 1.0))))) {
                    sysml_conditional_47 = 1;
                }
                else {
                    sysml_conditional_47 = 0;
                }
                sysml_conditional_43 = ((int32_t)(100 + ((((int32_t)(((double)(io_2 * 900.0)) / 1023.0)) - sysml_conditional_44) * 1.0))) - sysml_conditional_47;
            }
            sysml_conditional_41 = sysml_conditional_43;
        }
        sysml_state_accDecMs = sysml_state_accDecMs - sysml_conditional_41;
        if (sysml_state_number == 0) {
            sysml_conditional_48 = 99;
        }
        else {
            sysml_conditional_48 = sysml_state_number - 1;
        }
        sysml_state_number = sysml_conditional_48;
    }
    sysml_state_prevBtn1 = io_0;
    sysml_state_prevBtn2 = io_1;
    int32_t sysml_value_0 = 0;
    sysml_value_0 = sysml_state_number;
    *io_4 = sysml_value_0;
}
// END COMPILED SYSML MODEL

// LLM4MDD target-board: arduino:avr:mega
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
static int32_t hw_value_io_4 = 0;
static void hw_shift595_io_4(int32_t value) {
  static const uint8_t digits[10] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
  bool valid = value >= 0 && value < 100L;
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
  {
    uint8_t segments = valid && (true) ? digits[(value / 10L) % 10] : 0;
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
static int32_t hw_shift595_previous_io_4 = INT32_MIN;
static int32_t hw_value_io_5 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A0, OUTPUT); digitalWrite(A0, LOW);
  pinMode(A2, OUTPUT); digitalWrite(A2, LOW);
  pinMode(A1, OUTPUT); digitalWrite(A1, LOW);
  if (hw_value_io_4 != hw_shift595_previous_io_4) {
    hw_shift595_io_4(hw_value_io_4); hw_shift595_previous_io_4 = hw_value_io_4;
  }
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 10UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (digitalRead(2) == LOW);
  }
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 10UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = (digitalRead(3) == LOW);
  }
  if (!hw_attempted_io_2 || (uint32_t)(hw_now - hw_sampled_io_2) >= 10UL) {
    hw_attempted_io_2 = true; hw_sampled_io_2 = hw_now;
    hw_value_io_2 = (analogRead(A3) * 1 + 0);
  }
  if (!hw_attempted_io_3 || (uint32_t)(hw_now - hw_sampled_io_3) >= 10UL) {
    hw_attempted_io_3 = true; hw_sampled_io_3 = hw_now;
    hw_value_io_3 = (analogRead(A4) * 1 + 0);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_5 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, hw_value_io_3, hw_value_io_5, &hw_value_io_4);
  if (hw_value_io_4 != hw_shift595_previous_io_4) {
    hw_shift595_io_4(hw_value_io_4); hw_shift595_previous_io_4 = hw_value_io_4;
  }
}

