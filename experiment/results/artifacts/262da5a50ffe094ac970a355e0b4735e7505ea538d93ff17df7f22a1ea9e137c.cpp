#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_79\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
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
extern bool sysml_state_prevBtn3;
/* Global Variable define of  */
extern int32_t sysml_state_accumMs;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, double io_3, int32_t io_4, int32_t* io_5);
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
bool sysml_state_prevBtn3 = false;
/* Global Variable define of  */
int32_t sysml_state_accumMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
#line 208 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_79\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
static void hw_shift595_io_5(int32_t value);
#line 242 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_79\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 256 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_79\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 45 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_79\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_number = 0;
    sysml_state_prevBtn1 = false;
    sysml_state_prevBtn2 = false;
    sysml_state_prevBtn3 = false;
    sysml_state_accumMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, double io_3, int32_t io_4, int32_t* io_5) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    int32_t sysml_conditional_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    int32_t sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    bool sysml_lazy_9 = 0;
    int32_t sysml_conditional_10 = 0;
    int32_t sysml_conditional_11 = 0;
    bool sysml_lazy_12 = 0;
    bool sysml_lazy_13 = 0;
    int32_t sysml_conditional_14 = 0;
    int32_t sysml_conditional_15 = 0;
    if (io_2) {
        sysml_state_number = 0;
        sysml_state_accumMs = 0;
    }
    sysml_lazy_0 = !io_2;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_0;
    }
    sysml_lazy_1 = sysml_lazy_0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_prevBtn1;
    }
    if (sysml_lazy_1) {
        if (sysml_state_number >= 99) {
            sysml_conditional_2 = 0;
        }
        else {
            sysml_conditional_2 = sysml_state_number + 1;
        }
        sysml_state_number = sysml_conditional_2;
        sysml_state_accumMs = 0;
    }
    sysml_lazy_3 = !io_2;
    if (sysml_lazy_3) {
        sysml_lazy_3 = io_1;
    }
    sysml_lazy_4 = sysml_lazy_3;
    if (sysml_lazy_4) {
        sysml_lazy_4 = !sysml_state_prevBtn2;
    }
    if (sysml_lazy_4) {
        if (sysml_state_number == 0) {
            sysml_conditional_5 = 99;
        }
        else {
            sysml_conditional_5 = sysml_state_number - 1;
        }
        sysml_state_number = sysml_conditional_5;
        sysml_state_accumMs = 0;
    }
    sysml_lazy_6 = !io_2;
    if (sysml_lazy_6) {
        sysml_lazy_6 = !io_0;
    }
    sysml_lazy_7 = sysml_lazy_6;
    if (sysml_lazy_7) {
        sysml_lazy_7 = !io_1;
    }
    if (sysml_lazy_7) {
        sysml_state_accumMs = 0;
    }
    sysml_lazy_8 = !io_2;
    if (sysml_lazy_8) {
        sysml_lazy_8 = io_0;
    }
    sysml_lazy_9 = sysml_lazy_8;
    if (sysml_lazy_9) {
        sysml_lazy_9 = !io_1;
    }
    if (sysml_lazy_9) {
        sysml_state_accumMs = (sysml_state_accumMs + io_4);
        if ((((double)(io_3 * 900.0)) / 1023.0) < ((double)((int32_t)(((double)(io_3 * 900.0)) / 1023.0)))) {
            sysml_conditional_10 = 1;
        }
        else {
            sysml_conditional_10 = 0;
        }
        if (sysml_state_accumMs >= (100 + (((int32_t)(((double)(io_3 * 900.0)) / 1023.0)) - sysml_conditional_10))) {
            if (sysml_state_number >= 99) {
                sysml_conditional_11 = 0;
            }
            else {
                sysml_conditional_11 = sysml_state_number + 1;
            }
            sysml_state_number = sysml_conditional_11;
            sysml_state_accumMs = 0;
        }
    }
    sysml_lazy_12 = !io_2;
    if (sysml_lazy_12) {
        sysml_lazy_12 = io_1;
    }
    sysml_lazy_13 = sysml_lazy_12;
    if (sysml_lazy_13) {
        sysml_lazy_13 = !io_0;
    }
    if (sysml_lazy_13) {
        sysml_state_accumMs = (sysml_state_accumMs + io_4);
        if ((((double)(io_3 * 900.0)) / 1023.0) < ((double)((int32_t)(((double)(io_3 * 900.0)) / 1023.0)))) {
            sysml_conditional_14 = 1;
        }
        else {
            sysml_conditional_14 = 0;
        }
        if (sysml_state_accumMs >= (100 + (((int32_t)(((double)(io_3 * 900.0)) / 1023.0)) - sysml_conditional_14))) {
            if (sysml_state_number == 0) {
                sysml_conditional_15 = 99;
            }
            else {
                sysml_conditional_15 = sysml_state_number - 1;
            }
            sysml_state_number = sysml_conditional_15;
            sysml_state_accumMs = 0;
        }
    }
    sysml_state_prevBtn1 = io_0;
    sysml_state_prevBtn2 = io_1;
    sysml_state_prevBtn3 = io_2;
    int32_t sysml_value_0 = 0;
    sysml_value_0 = sysml_state_number;
    *io_5 = sysml_value_0;
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
static uint32_t hw_sampled_io_3 = 0;
static bool hw_attempted_io_3 = false;
static int32_t hw_value_io_4 = 0;
static int32_t hw_value_io_5 = 0;
static void hw_shift595_io_5(int32_t value) {
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
static int32_t hw_shift595_previous_io_5 = INT32_MIN;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(A3, INPUT);
  pinMode(A0, OUTPUT); digitalWrite(A0, LOW);
  pinMode(A2, OUTPUT); digitalWrite(A2, LOW);
  pinMode(A1, OUTPUT); digitalWrite(A1, LOW);
  if (hw_value_io_5 != hw_shift595_previous_io_5) {
    hw_shift595_io_5(hw_value_io_5); hw_shift595_previous_io_5 = hw_value_io_5;
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
    hw_value_io_2 = (digitalRead(4) == LOW);
  }
  if (!hw_attempted_io_3 || (uint32_t)(hw_now - hw_sampled_io_3) >= 10UL) {
    hw_attempted_io_3 = true; hw_sampled_io_3 = hw_now;
    hw_value_io_3 = (analogRead(A3) * 900 + 100);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_4 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, hw_value_io_3, hw_value_io_4, &hw_value_io_5);
  if (hw_value_io_5 != hw_shift595_previous_io_5) {
    hw_shift595_io_5(hw_value_io_5); hw_shift595_previous_io_5 = hw_value_io_5;
  }
}

