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
extern bool sysml_state_bit0;
/* Global Variable define of  */
extern bool sysml_state_bit1;
/* Global Variable define of  */
extern bool sysml_state_bit2;
/* Global Variable define of  */
extern bool sysml_state_prevBtn1;
/* Global Variable define of  */
extern bool sysml_state_prevBtn2;
/* Global Variable define of  */
extern bool sysml_state_prevBtn3;
/* Global Variable define of  */
extern int32_t sysml_state_btn1HeldMs;
/* Global Variable define of  */
extern int32_t sysml_state_btn2HeldMs;
/* Global Variable define of  */
extern int32_t sysml_state_btn3HeldMs;
/* Global Variable define of  */
extern bool sysml_state_latchedBtn1;
/* Global Variable define of  */
extern bool sysml_state_latchedBtn2;
/* Global Variable define of  */
extern bool sysml_state_latchedBtn3;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_3, bool io_4, bool io_5, bool* io_0, bool* io_1, bool* io_2, bool* io_6_segment_0, bool* io_6_segment_1, bool* io_6_segment_2, bool* io_6_segment_3, bool* io_6_segment_4, bool* io_6_segment_5, bool* io_6_segment_6, bool* io_6_segment_7, bool* io_7, bool* io_8, bool* io_9);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_bit0 = false;
/* Global Variable define of  */
bool sysml_state_bit1 = false;
/* Global Variable define of  */
bool sysml_state_bit2 = false;
/* Global Variable define of  */
bool sysml_state_prevBtn1 = false;
/* Global Variable define of  */
bool sysml_state_prevBtn2 = false;
/* Global Variable define of  */
bool sysml_state_prevBtn3 = false;
/* Global Variable define of  */
int32_t sysml_state_btn1HeldMs = 0;
/* Global Variable define of  */
int32_t sysml_state_btn2HeldMs = 0;
/* Global Variable define of  */
int32_t sysml_state_btn3HeldMs = 0;
/* Global Variable define of  */
bool sysml_state_latchedBtn1 = false;
/* Global Variable define of  */
bool sysml_state_latchedBtn2 = false;
/* Global Variable define of  */
bool sysml_state_latchedBtn3 = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_bit0 = false;
    sysml_state_bit1 = false;
    sysml_state_bit2 = false;
    sysml_state_prevBtn1 = false;
    sysml_state_prevBtn2 = false;
    sysml_state_prevBtn3 = false;
    sysml_state_btn1HeldMs = 0;
    sysml_state_btn2HeldMs = 0;
    sysml_state_btn3HeldMs = 0;
    sysml_state_latchedBtn1 = false;
    sysml_state_latchedBtn2 = false;
    sysml_state_latchedBtn3 = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_3, bool io_4, bool io_5, bool* io_0, bool* io_1, bool* io_2, bool* io_6_segment_0, bool* io_6_segment_1, bool* io_6_segment_2, bool* io_6_segment_3, bool* io_6_segment_4, bool* io_6_segment_5, bool* io_6_segment_6, bool* io_6_segment_7, bool* io_7, bool* io_8, bool* io_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int32_t sysml_conditional_0 = 0;
    int32_t sysml_conditional_1 = 0;
    int32_t sysml_conditional_2 = 0;
    bool sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_conditional_5 = 0;
    bool sysml_conditional_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_conditional_8 = 0;
    bool sysml_conditional_9 = 0;
    bool sysml_lazy_10 = 0;
    bool sysml_conditional_11 = 0;
    bool sysml_lazy_12 = 0;
    bool sysml_lazy_13 = 0;
    bool sysml_lazy_14 = 0;
    bool sysml_lazy_15 = 0;
    bool sysml_lazy_16 = 0;
    bool sysml_lazy_17 = 0;
    bool sysml_lazy_18 = 0;
    bool sysml_lazy_19 = 0;
    bool sysml_lazy_20 = 0;
    bool sysml_lazy_21 = 0;
    bool sysml_lazy_22 = 0;
    bool sysml_lazy_23 = 0;
    bool sysml_lazy_24 = 0;
    bool sysml_lazy_25 = 0;
    bool sysml_lazy_26 = 0;
    bool sysml_lazy_27 = 0;
    bool sysml_lazy_28 = 0;
    bool sysml_lazy_29 = 0;
    bool sysml_lazy_30 = 0;
    bool sysml_lazy_31 = 0;
    bool sysml_lazy_32 = 0;
    bool sysml_lazy_33 = 0;
    bool sysml_lazy_34 = 0;
    bool sysml_lazy_35 = 0;
    bool sysml_lazy_36 = 0;
    bool sysml_lazy_37 = 0;
    bool sysml_lazy_38 = 0;
    bool sysml_lazy_39 = 0;
    bool sysml_lazy_40 = 0;
    if (io_3) {
        sysml_conditional_0 = sysml_state_btn1HeldMs + 5;
    }
    else {
        sysml_conditional_0 = 0;
    }
    sysml_state_btn1HeldMs = sysml_conditional_0;
    if (io_4) {
        sysml_conditional_1 = sysml_state_btn2HeldMs + 5;
    }
    else {
        sysml_conditional_1 = 0;
    }
    sysml_state_btn2HeldMs = sysml_conditional_1;
    if (io_5) {
        sysml_conditional_2 = sysml_state_btn3HeldMs + 5;
    }
    else {
        sysml_conditional_2 = 0;
    }
    sysml_state_btn3HeldMs = sysml_conditional_2;
    if (io_3) {
        sysml_lazy_4 = sysml_state_latchedBtn1;
        if (!sysml_lazy_4) {
            sysml_lazy_4 = sysml_state_btn1HeldMs >= 150;
        }
        if (sysml_lazy_4) {
            sysml_conditional_5 = true;
        }
        else {
            sysml_conditional_5 = false;
        }
        sysml_conditional_3 = sysml_conditional_5;
    }
    else {
        sysml_conditional_3 = false;
    }
    sysml_state_latchedBtn1 = sysml_conditional_3;
    if (io_4) {
        sysml_lazy_7 = sysml_state_latchedBtn2;
        if (!sysml_lazy_7) {
            sysml_lazy_7 = sysml_state_btn2HeldMs >= 150;
        }
        if (sysml_lazy_7) {
            sysml_conditional_8 = true;
        }
        else {
            sysml_conditional_8 = false;
        }
        sysml_conditional_6 = sysml_conditional_8;
    }
    else {
        sysml_conditional_6 = false;
    }
    sysml_state_latchedBtn2 = sysml_conditional_6;
    if (io_5) {
        sysml_lazy_10 = sysml_state_latchedBtn3;
        if (!sysml_lazy_10) {
            sysml_lazy_10 = sysml_state_btn3HeldMs >= 150;
        }
        if (sysml_lazy_10) {
            sysml_conditional_11 = true;
        }
        else {
            sysml_conditional_11 = false;
        }
        sysml_conditional_9 = sysml_conditional_11;
    }
    else {
        sysml_conditional_9 = false;
    }
    sysml_state_latchedBtn3 = sysml_conditional_9;
    sysml_lazy_12 = io_3;
    if (sysml_lazy_12) {
        sysml_lazy_13 = !sysml_state_prevBtn1;
        if (sysml_lazy_13) {
            sysml_lazy_13 = sysml_state_latchedBtn1;
        }
        sysml_lazy_12 = sysml_lazy_13;
    }
    if (sysml_lazy_12) {
        sysml_state_bit0 = (!sysml_state_bit0);
    }
    sysml_lazy_14 = io_4;
    if (sysml_lazy_14) {
        sysml_lazy_15 = !sysml_state_prevBtn2;
        if (sysml_lazy_15) {
            sysml_lazy_15 = sysml_state_latchedBtn2;
        }
        sysml_lazy_14 = sysml_lazy_15;
    }
    if (sysml_lazy_14) {
        sysml_state_bit1 = (!sysml_state_bit1);
    }
    sysml_lazy_16 = io_5;
    if (sysml_lazy_16) {
        sysml_lazy_17 = !sysml_state_prevBtn3;
        if (sysml_lazy_17) {
            sysml_lazy_17 = sysml_state_latchedBtn3;
        }
        sysml_lazy_16 = sysml_lazy_17;
    }
    if (sysml_lazy_16) {
        sysml_state_bit2 = (!sysml_state_bit2);
    }
    sysml_state_prevBtn1 = io_3;
    sysml_state_prevBtn2 = io_4;
    sysml_state_prevBtn3 = io_5;
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_bit0;
    bool sysml_value_1 = 0;
    sysml_value_1 = sysml_state_bit1;
    bool sysml_value_2 = 0;
    sysml_value_2 = sysml_state_bit2;
    bool sysml_value_3 = 0;
    sysml_lazy_18 = sysml_state_bit0;
    if (!sysml_lazy_18) {
        sysml_lazy_19 = sysml_state_bit1;
        if (sysml_lazy_19) {
            sysml_lazy_19 = sysml_state_bit0;
        }
        sysml_lazy_18 = sysml_lazy_19;
    }
    sysml_lazy_20 = sysml_lazy_18;
    if (!sysml_lazy_20) {
        sysml_lazy_21 = sysml_state_bit2;
        if (sysml_lazy_21) {
            sysml_lazy_22 = sysml_state_bit0;
            if (!sysml_lazy_22) {
                sysml_lazy_22 = sysml_state_bit1;
            }
            sysml_lazy_21 = sysml_lazy_22;
        }
        sysml_lazy_20 = sysml_lazy_21;
    }
    sysml_value_3 = !sysml_lazy_20;
    bool sysml_value_4 = 0;
    sysml_lazy_23 = !sysml_state_bit1;
    if (sysml_lazy_23) {
        sysml_lazy_23 = !sysml_state_bit2;
    }
    sysml_lazy_24 = sysml_lazy_23;
    if (sysml_lazy_24) {
        sysml_lazy_24 = sysml_state_bit0;
    }
    sysml_value_4 = sysml_lazy_24;
    bool sysml_value_5 = 0;
    sysml_lazy_25 = sysml_state_bit1;
    if (sysml_lazy_25) {
        sysml_lazy_25 = !sysml_state_bit2;
    }
    sysml_lazy_26 = sysml_lazy_25;
    if (sysml_lazy_26) {
        sysml_lazy_26 = !sysml_state_bit0;
    }
    sysml_value_5 = sysml_lazy_26;
    bool sysml_value_6 = 0;
    sysml_lazy_27 = sysml_state_bit1;
    if (sysml_lazy_27) {
        sysml_lazy_27 = !sysml_state_bit2;
    }
    sysml_lazy_28 = sysml_lazy_27;
    if (sysml_lazy_28) {
        sysml_lazy_28 = sysml_state_bit0;
    }
    sysml_value_6 = sysml_lazy_28;
    bool sysml_value_7 = 0;
    sysml_lazy_29 = !sysml_state_bit1;
    if (sysml_lazy_29) {
        sysml_lazy_29 = sysml_state_bit2;
    }
    sysml_lazy_30 = sysml_lazy_29;
    if (sysml_lazy_30) {
        sysml_lazy_30 = !sysml_state_bit0;
    }
    sysml_value_7 = sysml_lazy_30;
    bool sysml_value_8 = 0;
    sysml_lazy_31 = !sysml_state_bit1;
    if (sysml_lazy_31) {
        sysml_lazy_31 = sysml_state_bit2;
    }
    sysml_lazy_32 = sysml_lazy_31;
    if (sysml_lazy_32) {
        sysml_lazy_32 = sysml_state_bit0;
    }
    sysml_value_8 = sysml_lazy_32;
    bool sysml_value_9 = 0;
    sysml_lazy_33 = sysml_state_bit1;
    if (sysml_lazy_33) {
        sysml_lazy_33 = sysml_state_bit2;
    }
    sysml_lazy_34 = sysml_lazy_33;
    if (sysml_lazy_34) {
        sysml_lazy_34 = !sysml_state_bit0;
    }
    sysml_value_9 = sysml_lazy_34;
    bool sysml_value_10 = 0;
    sysml_lazy_35 = sysml_state_bit1;
    if (sysml_lazy_35) {
        sysml_lazy_35 = sysml_state_bit2;
    }
    sysml_lazy_36 = sysml_lazy_35;
    if (sysml_lazy_36) {
        sysml_lazy_36 = sysml_state_bit0;
    }
    sysml_value_10 = sysml_lazy_36;
    bool sysml_value_11 = 0;
    sysml_value_11 = false;
    bool sysml_value_12 = 0;
    sysml_lazy_37 = sysml_state_bit2;
    if (sysml_lazy_37) {
        sysml_lazy_37 = sysml_state_bit0;
    }
    sysml_lazy_38 = sysml_lazy_37;
    if (sysml_lazy_38) {
        sysml_lazy_38 = !sysml_state_bit1;
    }
    sysml_value_12 = sysml_lazy_38;
    bool sysml_value_13 = 0;
    sysml_lazy_39 = sysml_state_bit2;
    if (sysml_lazy_39) {
        sysml_lazy_39 = sysml_state_bit1;
    }
    sysml_lazy_40 = sysml_lazy_39;
    if (sysml_lazy_40) {
        sysml_lazy_40 = sysml_state_bit0;
    }
    sysml_value_13 = sysml_lazy_40;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_6_segment_0 = sysml_value_3;
    *io_6_segment_1 = sysml_value_4;
    *io_6_segment_2 = sysml_value_5;
    *io_6_segment_3 = sysml_value_6;
    *io_6_segment_4 = sysml_value_7;
    *io_6_segment_5 = sysml_value_8;
    *io_6_segment_6 = sysml_value_9;
    *io_6_segment_7 = sysml_value_10;
    *io_7 = sysml_value_11;
    *io_8 = sysml_value_12;
    *io_9 = sysml_value_13;
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
static bool hw_value_io_1 = false;
static bool hw_value_io_2 = false;
static bool hw_value_io_3 = false;
static uint32_t hw_sampled_io_3 = 0;
static bool hw_attempted_io_3 = false;
static bool hw_value_io_4 = false;
static uint32_t hw_sampled_io_4 = 0;
static bool hw_attempted_io_4 = false;
static bool hw_value_io_5 = false;
static uint32_t hw_sampled_io_5 = 0;
static bool hw_attempted_io_5 = false;
static bool hw_value_io_6_segment_0 = false;
static bool hw_value_io_6_segment_1 = false;
static bool hw_value_io_6_segment_2 = false;
static bool hw_value_io_6_segment_3 = false;
static bool hw_value_io_6_segment_4 = false;
static bool hw_value_io_6_segment_5 = false;
static bool hw_value_io_6_segment_6 = false;
static bool hw_value_io_6_segment_7 = false;
static bool hw_value_io_7 = false;
static bool hw_value_io_8 = false;
static bool hw_value_io_9 = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, OUTPUT);
  digitalWrite(2, hw_value_io_0 ? HIGH : LOW);
  pinMode(3, OUTPUT);
  digitalWrite(3, hw_value_io_1 ? HIGH : LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, hw_value_io_2 ? HIGH : LOW);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, OUTPUT);
  digitalWrite(8, hw_value_io_6_segment_0 ? HIGH : LOW);
  pinMode(9, OUTPUT);
  digitalWrite(9, hw_value_io_6_segment_1 ? HIGH : LOW);
  pinMode(10, OUTPUT);
  digitalWrite(10, hw_value_io_6_segment_2 ? HIGH : LOW);
  pinMode(11, OUTPUT);
  digitalWrite(11, hw_value_io_6_segment_3 ? HIGH : LOW);
  pinMode(12, OUTPUT);
  digitalWrite(12, hw_value_io_6_segment_4 ? HIGH : LOW);
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_6_segment_5 ? HIGH : LOW);
  pinMode(44, OUTPUT);
  digitalWrite(44, hw_value_io_6_segment_6 ? HIGH : LOW);
  pinMode(45, OUTPUT);
  digitalWrite(45, hw_value_io_6_segment_7 ? HIGH : LOW);
  pinMode(A1, OUTPUT);
  digitalWrite(A1, hw_value_io_7 ? HIGH : LOW);
  pinMode(A5, OUTPUT);
  digitalWrite(A5, hw_value_io_8 ? HIGH : LOW);
  pinMode(A7, OUTPUT);
  digitalWrite(A7, hw_value_io_9 ? HIGH : LOW);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_3 || (uint32_t)(hw_now - hw_sampled_io_3) >= 5UL) {
    hw_attempted_io_3 = true; hw_sampled_io_3 = hw_now;
    hw_value_io_3 = (digitalRead(5) == LOW);
  }
  if (!hw_attempted_io_4 || (uint32_t)(hw_now - hw_sampled_io_4) >= 5UL) {
    hw_attempted_io_4 = true; hw_sampled_io_4 = hw_now;
    hw_value_io_4 = (digitalRead(6) == LOW);
  }
  if (!hw_attempted_io_5 || (uint32_t)(hw_now - hw_sampled_io_5) >= 5UL) {
    hw_attempted_io_5 = true; hw_sampled_io_5 = hw_now;
    hw_value_io_5 = (digitalRead(7) == LOW);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_3, hw_value_io_4, hw_value_io_5, &hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_6_segment_0, &hw_value_io_6_segment_1, &hw_value_io_6_segment_2, &hw_value_io_6_segment_3, &hw_value_io_6_segment_4, &hw_value_io_6_segment_5, &hw_value_io_6_segment_6, &hw_value_io_6_segment_7, &hw_value_io_7, &hw_value_io_8, &hw_value_io_9);
  digitalWrite(2, hw_value_io_0 ? HIGH : LOW);
  digitalWrite(3, hw_value_io_1 ? HIGH : LOW);
  digitalWrite(4, hw_value_io_2 ? HIGH : LOW);
  digitalWrite(8, hw_value_io_6_segment_0 ? HIGH : LOW);
  digitalWrite(9, hw_value_io_6_segment_1 ? HIGH : LOW);
  digitalWrite(10, hw_value_io_6_segment_2 ? HIGH : LOW);
  digitalWrite(11, hw_value_io_6_segment_3 ? HIGH : LOW);
  digitalWrite(12, hw_value_io_6_segment_4 ? HIGH : LOW);
  digitalWrite(13, hw_value_io_6_segment_5 ? HIGH : LOW);
  digitalWrite(44, hw_value_io_6_segment_6 ? HIGH : LOW);
  digitalWrite(45, hw_value_io_6_segment_7 ? HIGH : LOW);
  digitalWrite(A1, hw_value_io_7 ? HIGH : LOW);
  digitalWrite(A5, hw_value_io_8 ? HIGH : LOW);
  digitalWrite(A7, hw_value_io_9 ? HIGH : LOW);
}
