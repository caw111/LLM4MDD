#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_89\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
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
extern int32_t sysml_state_count;
/* Global Variable define of  */
extern bool sysml_state_prevBtn0;
/* Global Variable define of  */
extern bool sysml_state_prevBtn1;
/* Global Variable define of  */
extern bool sysml_state_prevBtn2;
/* Global Variable define of  */
extern bool sysml_state_debounceStable0;
/* Global Variable define of  */
extern bool sysml_state_debounceStable1;
/* Global Variable define of  */
extern bool sysml_state_debounceStable2;
/* Global Variable define of  */
extern bool sysml_state_debounceCand0;
/* Global Variable define of  */
extern bool sysml_state_debounceCand1;
/* Global Variable define of  */
extern bool sysml_state_debounceCand2;
/* Global Variable define of  */
extern int32_t sysml_state_debounceCnt0;
/* Global Variable define of  */
extern int32_t sysml_state_debounceCnt1;
/* Global Variable define of  */
extern int32_t sysml_state_debounceCnt2;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, double* io_3, double* io_4, double* io_5, double* io_6, double* io_7, double* io_8, int32_t* io_9);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_count = 0;
/* Global Variable define of  */
bool sysml_state_prevBtn0 = false;
/* Global Variable define of  */
bool sysml_state_prevBtn1 = false;
/* Global Variable define of  */
bool sysml_state_prevBtn2 = false;
/* Global Variable define of  */
bool sysml_state_debounceStable0 = false;
/* Global Variable define of  */
bool sysml_state_debounceStable1 = false;
/* Global Variable define of  */
bool sysml_state_debounceStable2 = false;
/* Global Variable define of  */
bool sysml_state_debounceCand0 = false;
/* Global Variable define of  */
bool sysml_state_debounceCand1 = false;
/* Global Variable define of  */
bool sysml_state_debounceCand2 = false;
/* Global Variable define of  */
int32_t sysml_state_debounceCnt0 = 0;
/* Global Variable define of  */
int32_t sysml_state_debounceCnt1 = 0;
/* Global Variable define of  */
int32_t sysml_state_debounceCnt2 = 0;
/* Init function of CompositeActor named "ProgramPage1" */
#line 469 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_89\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
static void hw_shift595_io_9(int32_t value);
#line 490 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_89\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void setup();
#line 515 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_89\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void loop();
#line 77 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_89\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_count = 0;
    sysml_state_prevBtn0 = false;
    sysml_state_prevBtn1 = false;
    sysml_state_prevBtn2 = false;
    sysml_state_debounceStable0 = false;
    sysml_state_debounceStable1 = false;
    sysml_state_debounceStable2 = false;
    sysml_state_debounceCand0 = false;
    sysml_state_debounceCand1 = false;
    sysml_state_debounceCand2 = false;
    sysml_state_debounceCnt0 = 0;
    sysml_state_debounceCnt1 = 0;
    sysml_state_debounceCnt2 = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, double* io_3, double* io_4, double* io_5, double* io_6, double* io_7, double* io_8, int32_t* io_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int32_t sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    int32_t sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    int32_t sysml_conditional_5 = 0;
    double sysml_conditional_6 = 0;
    bool sysml_lazy_7 = 0;
    double sysml_conditional_8 = 0;
    bool sysml_lazy_9 = 0;
    double sysml_conditional_10 = 0;
    bool sysml_lazy_11 = 0;
    double sysml_conditional_12 = 0;
    bool sysml_lazy_13 = 0;
    double sysml_conditional_14 = 0;
    double sysml_conditional_15 = 0;
    bool sysml_lazy_16 = 0;
    double sysml_conditional_17 = 0;
    double sysml_conditional_18 = 0;
    bool sysml_lazy_19 = 0;
    double sysml_conditional_20 = 0;
    bool sysml_lazy_21 = 0;
    double sysml_conditional_22 = 0;
    double sysml_conditional_23 = 0;
    bool sysml_lazy_24 = 0;
    double sysml_conditional_25 = 0;
    bool sysml_lazy_26 = 0;
    double sysml_conditional_27 = 0;
    double sysml_conditional_28 = 0;
    bool sysml_lazy_29 = 0;
    double sysml_conditional_30 = 0;
    bool sysml_lazy_31 = 0;
    double sysml_conditional_32 = 0;
    int32_t sysml_conditional_33 = 0;
    if (io_0) {
        if (sysml_state_debounceCand0) {
            if ((sysml_state_debounceStable0 == true)) {
            }
        }
        else {
            sysml_state_debounceCand0 = true;
            sysml_state_debounceCnt0 = 0;
        }
    }
    else {
        if ((!sysml_state_debounceCand0)) {
            if ((sysml_state_debounceStable0 == true)) {
            }
        }
        else {
            sysml_state_debounceCand0 = false;
            sysml_state_debounceCnt0 = 0;
        }
    }
    if (sysml_state_debounceCand0) {
        if ((sysml_state_debounceStable0 == false)) {
            sysml_state_debounceCnt0 = (sysml_state_debounceCnt0 + 1);
        }
    }
    else {
        if ((sysml_state_debounceStable0 == true)) {
            sysml_state_debounceCnt0 = (sysml_state_debounceCnt0 + 1);
        }
    }
    if ((sysml_state_debounceCnt0 >= 2)) {
        sysml_state_debounceStable0 = sysml_state_debounceCand0;
        sysml_state_debounceCnt0 = 0;
    }
    if (io_1) {
        if (sysml_state_debounceCand1) {
        }
        else {
            sysml_state_debounceCand1 = true;
            sysml_state_debounceCnt1 = 0;
        }
    }
    else {
        if ((!sysml_state_debounceCand1)) {
        }
        else {
            sysml_state_debounceCand1 = false;
            sysml_state_debounceCnt1 = 0;
        }
    }
    if (sysml_state_debounceCand1) {
        if ((sysml_state_debounceStable1 == false)) {
            sysml_state_debounceCnt1 = (sysml_state_debounceCnt1 + 1);
        }
    }
    else {
        if ((sysml_state_debounceStable1 == true)) {
            sysml_state_debounceCnt1 = (sysml_state_debounceCnt1 + 1);
        }
    }
    if ((sysml_state_debounceCnt1 >= 2)) {
        sysml_state_debounceStable1 = sysml_state_debounceCand1;
        sysml_state_debounceCnt1 = 0;
    }
    if (io_2) {
        if (sysml_state_debounceCand2) {
        }
        else {
            sysml_state_debounceCand2 = true;
            sysml_state_debounceCnt2 = 0;
        }
    }
    else {
        if ((!sysml_state_debounceCand2)) {
        }
        else {
            sysml_state_debounceCand2 = false;
            sysml_state_debounceCnt2 = 0;
        }
    }
    if (sysml_state_debounceCand2) {
        if ((sysml_state_debounceStable2 == false)) {
            sysml_state_debounceCnt2 = (sysml_state_debounceCnt2 + 1);
        }
    }
    else {
        if ((sysml_state_debounceStable2 == true)) {
            sysml_state_debounceCnt2 = (sysml_state_debounceCnt2 + 1);
        }
    }
    if ((sysml_state_debounceCnt2 >= 2)) {
        sysml_state_debounceStable2 = sysml_state_debounceCand2;
        sysml_state_debounceCnt2 = 0;
    }
    sysml_lazy_0 = sysml_state_debounceStable0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_prevBtn0;
    }
    if (sysml_lazy_0) {
        if ((sysml_state_count > 15)) {
            sysml_state_count = 0;
        }
        if ((sysml_state_count + 1) > 15) {
            sysml_conditional_1 = 0;
        }
        else {
            sysml_conditional_1 = sysml_state_count + 1;
        }
        sysml_state_count = sysml_conditional_1;
    }
    sysml_lazy_2 = sysml_state_debounceStable1;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_prevBtn1;
    }
    if (sysml_lazy_2) {
        if ((sysml_state_count > 15)) {
            sysml_state_count = 0;
        }
        if ((sysml_state_count + 1) > 15) {
            sysml_conditional_3 = 0;
        }
        else {
            sysml_conditional_3 = sysml_state_count + 1;
        }
        sysml_state_count = sysml_conditional_3;
    }
    sysml_lazy_4 = sysml_state_debounceStable2;
    if (sysml_lazy_4) {
        sysml_lazy_4 = !sysml_state_prevBtn2;
    }
    if (sysml_lazy_4) {
        if ((sysml_state_count > 15)) {
            sysml_state_count = 0;
        }
        if ((sysml_state_count + 1) > 15) {
            sysml_conditional_5 = 0;
        }
        else {
            sysml_conditional_5 = sysml_state_count + 1;
        }
        sysml_state_count = sysml_conditional_5;
    }
    sysml_state_prevBtn0 = sysml_state_debounceStable0;
    sysml_state_prevBtn1 = sysml_state_debounceStable1;
    sysml_state_prevBtn2 = sysml_state_debounceStable2;
    double sysml_value_0 = 0;
    if (sysml_state_count < 5) {
        sysml_conditional_6 = 1.0;
    }
    else {
        sysml_lazy_7 = sysml_state_count >= 5;
        if (sysml_lazy_7) {
            sysml_lazy_7 = sysml_state_count <= 9;
        }
        if (sysml_lazy_7) {
            sysml_conditional_8 = 0.0;
        }
        else {
            sysml_lazy_9 = sysml_state_count >= 10;
            if (sysml_lazy_9) {
                sysml_lazy_9 = sysml_state_count <= 15;
            }
            if (sysml_lazy_9) {
                sysml_conditional_10 = 1.0;
            }
            else {
                sysml_conditional_10 = 0.0;
            }
            sysml_conditional_8 = sysml_conditional_10;
        }
        sysml_conditional_6 = sysml_conditional_8;
    }
    sysml_value_0 = sysml_conditional_6;
    double sysml_value_1 = 0;
    sysml_lazy_11 = sysml_state_count >= 5;
    if (sysml_lazy_11) {
        sysml_lazy_11 = sysml_state_count <= 9;
    }
    if (sysml_lazy_11) {
        sysml_conditional_12 = 1.0;
    }
    else {
        sysml_lazy_13 = sysml_state_count >= 10;
        if (sysml_lazy_13) {
            sysml_lazy_13 = sysml_state_count <= 15;
        }
        if (sysml_lazy_13) {
            sysml_conditional_14 = 1.0;
        }
        else {
            sysml_conditional_14 = 0.0;
        }
        sysml_conditional_12 = sysml_conditional_14;
    }
    sysml_value_1 = sysml_conditional_12;
    double sysml_value_2 = 0;
    if (sysml_state_count < 5) {
        sysml_conditional_15 = 1.0;
    }
    else {
        sysml_lazy_16 = sysml_state_count >= 10;
        if (sysml_lazy_16) {
            sysml_lazy_16 = sysml_state_count <= 15;
        }
        if (sysml_lazy_16) {
            sysml_conditional_17 = 1.0;
        }
        else {
            sysml_conditional_17 = 0.0;
        }
        sysml_conditional_15 = sysml_conditional_17;
    }
    sysml_value_2 = sysml_conditional_15;
    double sysml_value_3 = 0;
    if (sysml_state_count < 5) {
        sysml_conditional_18 = 0.0;
    }
    else {
        sysml_lazy_19 = sysml_state_count >= 5;
        if (sysml_lazy_19) {
            sysml_lazy_19 = sysml_state_count <= 9;
        }
        if (sysml_lazy_19) {
            sysml_conditional_20 = 1.0;
        }
        else {
            sysml_lazy_21 = sysml_state_count >= 10;
            if (sysml_lazy_21) {
                sysml_lazy_21 = sysml_state_count <= 15;
            }
            if (sysml_lazy_21) {
                sysml_conditional_22 = 0.0;
            }
            else {
                sysml_conditional_22 = 0.0;
            }
            sysml_conditional_20 = sysml_conditional_22;
        }
        sysml_conditional_18 = sysml_conditional_20;
    }
    sysml_value_3 = sysml_conditional_18;
    double sysml_value_4 = 0;
    if (sysml_state_count < 5) {
        sysml_conditional_23 = 0.0;
    }
    else {
        sysml_lazy_24 = sysml_state_count >= 5;
        if (sysml_lazy_24) {
            sysml_lazy_24 = sysml_state_count <= 9;
        }
        if (sysml_lazy_24) {
            sysml_conditional_25 = 1.0;
        }
        else {
            sysml_lazy_26 = sysml_state_count >= 10;
            if (sysml_lazy_26) {
                sysml_lazy_26 = sysml_state_count <= 15;
            }
            if (sysml_lazy_26) {
                sysml_conditional_27 = 1.0;
            }
            else {
                sysml_conditional_27 = 0.0;
            }
            sysml_conditional_25 = sysml_conditional_27;
        }
        sysml_conditional_23 = sysml_conditional_25;
    }
    sysml_value_4 = sysml_conditional_23;
    double sysml_value_5 = 0;
    if (sysml_state_count < 5) {
        sysml_conditional_28 = 1.0;
    }
    else {
        sysml_lazy_29 = sysml_state_count >= 5;
        if (sysml_lazy_29) {
            sysml_lazy_29 = sysml_state_count <= 9;
        }
        if (sysml_lazy_29) {
            sysml_conditional_30 = 0.0;
        }
        else {
            sysml_lazy_31 = sysml_state_count >= 10;
            if (sysml_lazy_31) {
                sysml_lazy_31 = sysml_state_count <= 15;
            }
            if (sysml_lazy_31) {
                sysml_conditional_32 = 1.0;
            }
            else {
                sysml_conditional_32 = 0.0;
            }
            sysml_conditional_30 = sysml_conditional_32;
        }
        sysml_conditional_28 = sysml_conditional_30;
    }
    sysml_value_5 = sysml_conditional_28;
    int32_t sysml_value_6 = 0;
    if (sysml_state_count > 15) {
        sysml_conditional_33 = 0;
    }
    else {
        sysml_conditional_33 = sysml_state_count;
    }
    sysml_value_6 = sysml_conditional_33;
    *io_3 = sysml_value_0;
    *io_4 = sysml_value_1;
    *io_5 = sysml_value_2;
    *io_6 = sysml_value_3;
    *io_7 = sysml_value_4;
    *io_8 = sysml_value_5;
    *io_9 = sysml_value_6;
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
static bool hw_value_io_2 = false;
static uint32_t hw_sampled_io_2 = 0;
static bool hw_attempted_io_2 = false;
static double hw_value_io_3 = 0;
static double hw_value_io_4 = 0;
static double hw_value_io_5 = 0;
static double hw_value_io_6 = 0;
static double hw_value_io_7 = 0;
static double hw_value_io_8 = 0;
static int32_t hw_value_io_9 = 0;
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

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, OUTPUT);
  analogWrite(5, (int)constrain((hw_value_io_3 * 1 + 0), 0.0, 255.0));
  pinMode(6, OUTPUT);
  analogWrite(6, (int)constrain((hw_value_io_4 * 1 + 0), 0.0, 255.0));
  pinMode(7, OUTPUT);
  analogWrite(7, (int)constrain((hw_value_io_5 * 1 + 0), 0.0, 255.0));
  pinMode(8, OUTPUT);
  analogWrite(8, (int)constrain((hw_value_io_6 * 1 + 0), 0.0, 255.0));
  pinMode(9, OUTPUT);
  analogWrite(9, (int)constrain((hw_value_io_7 * 1 + 0), 0.0, 255.0));
  pinMode(10, OUTPUT);
  analogWrite(10, (int)constrain((hw_value_io_8 * 1 + 0), 0.0, 255.0));
  pinMode(A0, OUTPUT); digitalWrite(A0, LOW);
  pinMode(A2, OUTPUT); digitalWrite(A2, LOW);
  pinMode(A1, OUTPUT); digitalWrite(A1, LOW);
  if (hw_value_io_9 != hw_shift595_previous_io_9) {
    hw_shift595_io_9(hw_value_io_9); hw_shift595_previous_io_9 = hw_value_io_9;
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
    hw_value_io_2 = (digitalRead(4) == LOW);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_io_7, &hw_value_io_8, &hw_value_io_9);
  analogWrite(5, (int)constrain((hw_value_io_3 * 1 + 0), 0.0, 255.0));
  analogWrite(6, (int)constrain((hw_value_io_4 * 1 + 0), 0.0, 255.0));
  analogWrite(7, (int)constrain((hw_value_io_5 * 1 + 0), 0.0, 255.0));
  analogWrite(8, (int)constrain((hw_value_io_6 * 1 + 0), 0.0, 255.0));
  analogWrite(9, (int)constrain((hw_value_io_7 * 1 + 0), 0.0, 255.0));
  analogWrite(10, (int)constrain((hw_value_io_8 * 1 + 0), 0.0, 255.0));
  if (hw_value_io_9 != hw_shift595_previous_io_9) {
    hw_shift595_io_9(hw_value_io_9); hw_shift595_previous_io_9 = hw_value_io_9;
  }
}

