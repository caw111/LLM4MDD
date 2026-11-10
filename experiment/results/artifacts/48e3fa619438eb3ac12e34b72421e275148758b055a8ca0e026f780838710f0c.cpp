#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_115\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
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
extern int32_t sysml_state_countdownValue;
/* Global Variable define of  */
extern int32_t sysml_state_elapsedAccumMs;
/* Global Variable define of  */
extern int32_t sysml_state_ledStepIndex;
/* Global Variable define of  */
extern int32_t sysml_state_rgbColorIndex;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_0, bool* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7, bool* io_8, bool* io_9, int32_t* io_10);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_countdownValue = 9;
/* Global Variable define of  */
int32_t sysml_state_elapsedAccumMs = 0;
/* Global Variable define of  */
int32_t sysml_state_ledStepIndex = 0;
/* Global Variable define of  */
int32_t sysml_state_rgbColorIndex = 0;
/* Init function of CompositeActor named "ProgramPage1" */
#line 294 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_115\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
static void hw_shift595_io_10(int32_t value);
#line 328 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_115\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void setup();
#line 356 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_115\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void loop();
#line 41 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_115\\repeat_1\\blind_resampling\\final\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_countdownValue = 9;
    sysml_state_elapsedAccumMs = 0;
    sysml_state_ledStepIndex = 0;
    sysml_state_rgbColorIndex = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_0, bool* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7, bool* io_8, bool* io_9, int32_t* io_10) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int32_t sysml_conditional_0 = 0;
    int32_t sysml_conditional_1 = 0;
    int32_t sysml_conditional_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_conditional_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_conditional_6 = 0;
    bool sysml_lazy_7 = 0;
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
    int32_t sysml_conditional_21 = 0;
    int32_t sysml_conditional_22 = 0;
    int32_t sysml_conditional_23 = 0;
    bool sysml_lazy_24 = 0;
    int32_t sysml_conditional_25 = 0;
    int32_t sysml_arithmetic_26 = 0;
    if ((io_0 >= 1)) {
        sysml_state_elapsedAccumMs = (sysml_state_elapsedAccumMs + io_0);
        if ((sysml_state_elapsedAccumMs >= 2000)) {
            sysml_state_elapsedAccumMs = (sysml_state_elapsedAccumMs - 2000);
            if (sysml_state_countdownValue == 0) {
                sysml_conditional_0 = 9;
            }
            else {
                sysml_conditional_0 = sysml_state_countdownValue - 1;
            }
            sysml_state_countdownValue = sysml_conditional_0;
            if (sysml_state_countdownValue == 0) {
                sysml_conditional_1 = 0;
            }
            else {
                sysml_conditional_1 = sysml_state_ledStepIndex + 1;
            }
            sysml_state_ledStepIndex = sysml_conditional_1;
            if (sysml_state_countdownValue == 0) {
                sysml_conditional_2 = 0;
            }
            else {
                sysml_conditional_2 = sysml_state_rgbColorIndex + 1;
            }
            sysml_state_rgbColorIndex = sysml_conditional_2;
            if ((sysml_state_ledStepIndex >= 3)) {
                sysml_state_ledStepIndex = 0;
            }
            if ((sysml_state_rgbColorIndex >= 3)) {
                sysml_state_rgbColorIndex = 0;
            }
        }
    }
    bool sysml_value_0 = 0;
    sysml_lazy_3 = sysml_state_countdownValue != 0;
    if (sysml_lazy_3) {
        sysml_lazy_3 = sysml_state_ledStepIndex == 0;
    }
    if (sysml_lazy_3) {
        sysml_conditional_4 = true;
    }
    else {
        sysml_conditional_4 = false;
    }
    sysml_value_0 = sysml_conditional_4;
    bool sysml_value_1 = 0;
    sysml_lazy_5 = sysml_state_countdownValue != 0;
    if (sysml_lazy_5) {
        sysml_lazy_5 = sysml_state_ledStepIndex == 1;
    }
    if (sysml_lazy_5) {
        sysml_conditional_6 = true;
    }
    else {
        sysml_conditional_6 = false;
    }
    sysml_value_1 = sysml_conditional_6;
    bool sysml_value_2 = 0;
    sysml_lazy_7 = sysml_state_countdownValue != 0;
    if (sysml_lazy_7) {
        sysml_lazy_7 = sysml_state_ledStepIndex == 2;
    }
    if (sysml_lazy_7) {
        sysml_conditional_8 = true;
    }
    else {
        sysml_conditional_8 = false;
    }
    sysml_value_2 = sysml_conditional_8;
    bool sysml_value_3 = 0;
    if (sysml_state_countdownValue == 0) {
        sysml_conditional_9 = true;
    }
    else {
        if (sysml_state_rgbColorIndex == 0) {
            sysml_conditional_10 = true;
        }
        else {
            sysml_conditional_10 = false;
        }
        sysml_conditional_9 = sysml_conditional_10;
    }
    sysml_value_3 = sysml_conditional_9;
    bool sysml_value_4 = 0;
    if (sysml_state_countdownValue == 0) {
        sysml_conditional_11 = true;
    }
    else {
        if (sysml_state_rgbColorIndex == 1) {
            sysml_conditional_12 = true;
        }
        else {
            sysml_conditional_12 = false;
        }
        sysml_conditional_11 = sysml_conditional_12;
    }
    sysml_value_4 = sysml_conditional_11;
    bool sysml_value_5 = 0;
    if (sysml_state_countdownValue == 0) {
        sysml_conditional_13 = true;
    }
    else {
        if (sysml_state_rgbColorIndex == 2) {
            sysml_conditional_14 = true;
        }
        else {
            sysml_conditional_14 = false;
        }
        sysml_conditional_13 = sysml_conditional_14;
    }
    sysml_value_5 = sysml_conditional_13;
    bool sysml_value_6 = 0;
    if (sysml_state_countdownValue == 0) {
        sysml_conditional_15 = true;
    }
    else {
        if (sysml_state_rgbColorIndex == 0) {
            sysml_conditional_16 = true;
        }
        else {
            sysml_conditional_16 = false;
        }
        sysml_conditional_15 = sysml_conditional_16;
    }
    sysml_value_6 = sysml_conditional_15;
    bool sysml_value_7 = 0;
    if (sysml_state_countdownValue == 0) {
        sysml_conditional_17 = true;
    }
    else {
        if (sysml_state_rgbColorIndex == 1) {
            sysml_conditional_18 = true;
        }
        else {
            sysml_conditional_18 = false;
        }
        sysml_conditional_17 = sysml_conditional_18;
    }
    sysml_value_7 = sysml_conditional_17;
    bool sysml_value_8 = 0;
    if (sysml_state_countdownValue == 0) {
        sysml_conditional_19 = true;
    }
    else {
        if (sysml_state_rgbColorIndex == 2) {
            sysml_conditional_20 = true;
        }
        else {
            sysml_conditional_20 = false;
        }
        sysml_conditional_19 = sysml_conditional_20;
    }
    sysml_value_8 = sysml_conditional_19;
    int32_t sysml_value_9 = 0;
    if (sysml_state_countdownValue == 8) {
        sysml_conditional_21 = 88;
    }
    else {
        if (sysml_state_countdownValue == 0) {
            sysml_conditional_22 = 0;
        }
        else {
            if (sysml_state_countdownValue == 9) {
                sysml_conditional_23 = 99;
            }
            else {
                sysml_lazy_24 = sysml_state_countdownValue >= 1;
                if (sysml_lazy_24) {
                    sysml_lazy_24 = sysml_state_countdownValue <= 7;
                }
                if (sysml_lazy_24) {
                    sysml_arithmetic_26 = (10 * 0) + 1;
                    sysml_conditional_25 = (((10 * sysml_arithmetic_26) * 0) + (10 * 0)) + 0;
                }
                else {
                    sysml_conditional_25 = 0;
                }
                sysml_conditional_23 = sysml_conditional_25;
            }
            sysml_conditional_22 = sysml_conditional_23;
        }
        sysml_conditional_21 = sysml_conditional_22;
    }
    sysml_value_9 = sysml_conditional_21;
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
#include <Arduino.h>
#include <math.h>
#if defined(ARDUINO) && !defined(ARDUINO_AVR_MEGA2560)
#error "Selected Arduino board differs from the interface contract"
#endif
static uint32_t hw_last_step = 0;
static bool hw_started = false;
static int32_t hw_value_io_0 = 0;
static bool hw_value_io_1 = false;
static bool hw_value_io_2 = false;
static bool hw_value_io_3 = false;
static bool hw_value_io_4 = false;
static bool hw_value_io_5 = false;
static bool hw_value_io_6 = false;
static bool hw_value_io_7 = false;
static bool hw_value_io_8 = false;
static bool hw_value_io_9 = false;
static int32_t hw_value_io_10 = 99;
static void hw_shift595_io_10(int32_t value) {
  static const uint8_t digits[10] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
  bool valid = value >= 0 && value < 100L;
  digitalWrite(A1, LOW);
  {
    uint8_t segments = valid && (value >= 10L) ? digits[(value / 10L) % 10] : 0;
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
static int32_t hw_shift595_previous_io_10 = INT32_MIN;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, OUTPUT);
  digitalWrite(2, hw_value_io_1 ? HIGH : LOW);
  pinMode(3, OUTPUT);
  digitalWrite(3, hw_value_io_2 ? HIGH : LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, hw_value_io_3 ? HIGH : LOW);
  pinMode(5, OUTPUT);
  digitalWrite(5, hw_value_io_4 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  digitalWrite(6, hw_value_io_5 ? HIGH : LOW);
  pinMode(7, OUTPUT);
  digitalWrite(7, hw_value_io_6 ? HIGH : LOW);
  pinMode(8, OUTPUT);
  digitalWrite(8, hw_value_io_7 ? HIGH : LOW);
  pinMode(9, OUTPUT);
  digitalWrite(9, hw_value_io_8 ? HIGH : LOW);
  pinMode(10, OUTPUT);
  digitalWrite(10, hw_value_io_9 ? HIGH : LOW);
  pinMode(A0, OUTPUT); digitalWrite(A0, LOW);
  pinMode(A2, OUTPUT); digitalWrite(A2, LOW);
  pinMode(A1, OUTPUT); digitalWrite(A1, LOW);
  if (hw_value_io_10 != hw_shift595_previous_io_10) {
    hw_shift595_io_10(hw_value_io_10); hw_shift595_previous_io_10 = hw_value_io_10;
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
  digitalWrite(2, hw_value_io_1 ? HIGH : LOW);
  digitalWrite(3, hw_value_io_2 ? HIGH : LOW);
  digitalWrite(4, hw_value_io_3 ? HIGH : LOW);
  digitalWrite(5, hw_value_io_4 ? HIGH : LOW);
  digitalWrite(6, hw_value_io_5 ? HIGH : LOW);
  digitalWrite(7, hw_value_io_6 ? HIGH : LOW);
  digitalWrite(8, hw_value_io_7 ? HIGH : LOW);
  digitalWrite(9, hw_value_io_8 ? HIGH : LOW);
  digitalWrite(10, hw_value_io_9 ? HIGH : LOW);
  if (hw_value_io_10 != hw_shift595_previous_io_10) {
    hw_shift595_io_10(hw_value_io_10); hw_shift595_previous_io_10 = hw_value_io_10;
  }
}

