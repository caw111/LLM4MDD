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
extern int32_t sysml_state_elapsedTotalMs;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_8, bool* io_0, bool* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_elapsedTotalMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_elapsedTotalMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_8, bool* io_0, bool* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_conditional_7 = 0;
    bool sysml_lazy_8 = 0;
    bool sysml_conditional_9 = 0;
    bool sysml_lazy_10 = 0;
    bool sysml_conditional_11 = 0;
    bool sysml_lazy_12 = 0;
    bool sysml_conditional_13 = 0;
    bool sysml_lazy_14 = 0;
    bool sysml_conditional_15 = 0;
    bool sysml_lazy_16 = 0;
    bool sysml_conditional_17 = 0;
    bool sysml_lazy_18 = 0;
    bool sysml_conditional_19 = 0;
    bool sysml_lazy_20 = 0;
    bool sysml_conditional_21 = 0;
    bool sysml_lazy_22 = 0;
    bool sysml_conditional_23 = 0;
    bool sysml_lazy_24 = 0;
    bool sysml_conditional_25 = 0;
    bool sysml_lazy_26 = 0;
    bool sysml_conditional_27 = 0;
    bool sysml_lazy_28 = 0;
    bool sysml_conditional_29 = 0;
    bool sysml_lazy_30 = 0;
    bool sysml_conditional_31 = 0;
    bool sysml_lazy_32 = 0;
    bool sysml_conditional_33 = 0;
    bool sysml_lazy_34 = 0;
    bool sysml_conditional_35 = 0;
    bool sysml_lazy_36 = 0;
    bool sysml_conditional_37 = 0;
    bool sysml_lazy_38 = 0;
    bool sysml_conditional_39 = 0;
    bool sysml_lazy_40 = 0;
    bool sysml_conditional_41 = 0;
    bool sysml_lazy_42 = 0;
    bool sysml_conditional_43 = 0;
    bool sysml_lazy_44 = 0;
    bool sysml_conditional_45 = 0;
    bool sysml_lazy_46 = 0;
    bool sysml_conditional_47 = 0;
    sysml_state_elapsedTotalMs = (sysml_state_elapsedTotalMs + io_8);
    bool sysml_value_0 = 0;
    sysml_lazy_0 = sysml_state_elapsedTotalMs >= 1000;
    if (sysml_lazy_0) {
        sysml_lazy_0 = sysml_state_elapsedTotalMs < 2000;
    }
    if (sysml_lazy_0) {
        sysml_conditional_1 = true;
    }
    else {
        sysml_lazy_2 = sysml_state_elapsedTotalMs >= 2000;
        if (sysml_lazy_2) {
            sysml_lazy_2 = sysml_state_elapsedTotalMs < 3000;
        }
        if (sysml_lazy_2) {
            sysml_conditional_3 = true;
        }
        else {
            sysml_lazy_4 = sysml_state_elapsedTotalMs >= 3000;
            if (sysml_lazy_4) {
                sysml_lazy_4 = sysml_state_elapsedTotalMs < 4000;
            }
            if (sysml_lazy_4) {
                sysml_conditional_5 = true;
            }
            else {
                sysml_conditional_5 = false;
            }
            sysml_conditional_3 = sysml_conditional_5;
        }
        sysml_conditional_1 = sysml_conditional_3;
    }
    sysml_value_0 = sysml_conditional_1;
    bool sysml_value_1 = 0;
    sysml_lazy_6 = sysml_state_elapsedTotalMs >= 1000;
    if (sysml_lazy_6) {
        sysml_lazy_6 = sysml_state_elapsedTotalMs < 2000;
    }
    if (sysml_lazy_6) {
        sysml_conditional_7 = true;
    }
    else {
        sysml_lazy_8 = sysml_state_elapsedTotalMs >= 2000;
        if (sysml_lazy_8) {
            sysml_lazy_8 = sysml_state_elapsedTotalMs < 3000;
        }
        if (sysml_lazy_8) {
            sysml_conditional_9 = false;
        }
        else {
            sysml_lazy_10 = sysml_state_elapsedTotalMs >= 3000;
            if (sysml_lazy_10) {
                sysml_lazy_10 = sysml_state_elapsedTotalMs < 4000;
            }
            if (sysml_lazy_10) {
                sysml_conditional_11 = false;
            }
            else {
                sysml_conditional_11 = false;
            }
            sysml_conditional_9 = sysml_conditional_11;
        }
        sysml_conditional_7 = sysml_conditional_9;
    }
    sysml_value_1 = sysml_conditional_7;
    bool sysml_value_2 = 0;
    sysml_lazy_12 = sysml_state_elapsedTotalMs >= 1000;
    if (sysml_lazy_12) {
        sysml_lazy_12 = sysml_state_elapsedTotalMs < 2000;
    }
    if (sysml_lazy_12) {
        sysml_conditional_13 = true;
    }
    else {
        sysml_lazy_14 = sysml_state_elapsedTotalMs >= 2000;
        if (sysml_lazy_14) {
            sysml_lazy_14 = sysml_state_elapsedTotalMs < 3000;
        }
        if (sysml_lazy_14) {
            sysml_conditional_15 = false;
        }
        else {
            sysml_lazy_16 = sysml_state_elapsedTotalMs >= 3000;
            if (sysml_lazy_16) {
                sysml_lazy_16 = sysml_state_elapsedTotalMs < 4000;
            }
            if (sysml_lazy_16) {
                sysml_conditional_17 = false;
            }
            else {
                sysml_conditional_17 = false;
            }
            sysml_conditional_15 = sysml_conditional_17;
        }
        sysml_conditional_13 = sysml_conditional_15;
    }
    sysml_value_2 = sysml_conditional_13;
    bool sysml_value_3 = 0;
    sysml_lazy_18 = sysml_state_elapsedTotalMs >= 1000;
    if (sysml_lazy_18) {
        sysml_lazy_18 = sysml_state_elapsedTotalMs < 2000;
    }
    if (sysml_lazy_18) {
        sysml_conditional_19 = true;
    }
    else {
        sysml_lazy_20 = sysml_state_elapsedTotalMs >= 2000;
        if (sysml_lazy_20) {
            sysml_lazy_20 = sysml_state_elapsedTotalMs < 3000;
        }
        if (sysml_lazy_20) {
            sysml_conditional_21 = true;
        }
        else {
            sysml_lazy_22 = sysml_state_elapsedTotalMs >= 3000;
            if (sysml_lazy_22) {
                sysml_lazy_22 = sysml_state_elapsedTotalMs < 4000;
            }
            if (sysml_lazy_22) {
                sysml_conditional_23 = true;
            }
            else {
                sysml_conditional_23 = false;
            }
            sysml_conditional_21 = sysml_conditional_23;
        }
        sysml_conditional_19 = sysml_conditional_21;
    }
    sysml_value_3 = sysml_conditional_19;
    bool sysml_value_4 = 0;
    sysml_lazy_24 = sysml_state_elapsedTotalMs >= 1000;
    if (sysml_lazy_24) {
        sysml_lazy_24 = sysml_state_elapsedTotalMs < 2000;
    }
    if (sysml_lazy_24) {
        sysml_conditional_25 = true;
    }
    else {
        sysml_lazy_26 = sysml_state_elapsedTotalMs >= 2000;
        if (sysml_lazy_26) {
            sysml_lazy_26 = sysml_state_elapsedTotalMs < 3000;
        }
        if (sysml_lazy_26) {
            sysml_conditional_27 = false;
        }
        else {
            sysml_lazy_28 = sysml_state_elapsedTotalMs >= 3000;
            if (sysml_lazy_28) {
                sysml_lazy_28 = sysml_state_elapsedTotalMs < 4000;
            }
            if (sysml_lazy_28) {
                sysml_conditional_29 = false;
            }
            else {
                sysml_conditional_29 = false;
            }
            sysml_conditional_27 = sysml_conditional_29;
        }
        sysml_conditional_25 = sysml_conditional_27;
    }
    sysml_value_4 = sysml_conditional_25;
    bool sysml_value_5 = 0;
    sysml_lazy_30 = sysml_state_elapsedTotalMs >= 1000;
    if (sysml_lazy_30) {
        sysml_lazy_30 = sysml_state_elapsedTotalMs < 2000;
    }
    if (sysml_lazy_30) {
        sysml_conditional_31 = false;
    }
    else {
        sysml_lazy_32 = sysml_state_elapsedTotalMs >= 2000;
        if (sysml_lazy_32) {
            sysml_lazy_32 = sysml_state_elapsedTotalMs < 3000;
        }
        if (sysml_lazy_32) {
            sysml_conditional_33 = false;
        }
        else {
            sysml_lazy_34 = sysml_state_elapsedTotalMs >= 3000;
            if (sysml_lazy_34) {
                sysml_lazy_34 = sysml_state_elapsedTotalMs < 4000;
            }
            if (sysml_lazy_34) {
                sysml_conditional_35 = false;
            }
            else {
                sysml_conditional_35 = false;
            }
            sysml_conditional_33 = sysml_conditional_35;
        }
        sysml_conditional_31 = sysml_conditional_33;
    }
    sysml_value_5 = sysml_conditional_31;
    bool sysml_value_6 = 0;
    sysml_lazy_36 = sysml_state_elapsedTotalMs >= 1000;
    if (sysml_lazy_36) {
        sysml_lazy_36 = sysml_state_elapsedTotalMs < 2000;
    }
    if (sysml_lazy_36) {
        sysml_conditional_37 = false;
    }
    else {
        sysml_lazy_38 = sysml_state_elapsedTotalMs >= 2000;
        if (sysml_lazy_38) {
            sysml_lazy_38 = sysml_state_elapsedTotalMs < 3000;
        }
        if (sysml_lazy_38) {
            sysml_conditional_39 = false;
        }
        else {
            sysml_lazy_40 = sysml_state_elapsedTotalMs >= 3000;
            if (sysml_lazy_40) {
                sysml_lazy_40 = sysml_state_elapsedTotalMs < 4000;
            }
            if (sysml_lazy_40) {
                sysml_conditional_41 = false;
            }
            else {
                sysml_conditional_41 = false;
            }
            sysml_conditional_39 = sysml_conditional_41;
        }
        sysml_conditional_37 = sysml_conditional_39;
    }
    sysml_value_6 = sysml_conditional_37;
    bool sysml_value_7 = 0;
    sysml_lazy_42 = sysml_state_elapsedTotalMs >= 1000;
    if (sysml_lazy_42) {
        sysml_lazy_42 = sysml_state_elapsedTotalMs < 2000;
    }
    if (sysml_lazy_42) {
        sysml_conditional_43 = false;
    }
    else {
        sysml_lazy_44 = sysml_state_elapsedTotalMs >= 2000;
        if (sysml_lazy_44) {
            sysml_lazy_44 = sysml_state_elapsedTotalMs < 3000;
        }
        if (sysml_lazy_44) {
            sysml_conditional_45 = false;
        }
        else {
            sysml_lazy_46 = sysml_state_elapsedTotalMs >= 3000;
            if (sysml_lazy_46) {
                sysml_lazy_46 = sysml_state_elapsedTotalMs < 4000;
            }
            if (sysml_lazy_46) {
                sysml_conditional_47 = false;
            }
            else {
                sysml_conditional_47 = false;
            }
            sysml_conditional_45 = sysml_conditional_47;
        }
        sysml_conditional_43 = sysml_conditional_45;
    }
    sysml_value_7 = sysml_conditional_43;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
    *io_4 = sysml_value_4;
    *io_5 = sysml_value_5;
    *io_6 = sysml_value_6;
    *io_7 = sysml_value_7;
}
// END COMPILED SYSML MODEL

// LLM4MDD target-board: arduino:avr:uno
#include <Arduino.h>
#include <math.h>
#if defined(ARDUINO) && !defined(ARDUINO_AVR_UNO)
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
static int32_t hw_value_io_8 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, OUTPUT);
  digitalWrite(2, hw_value_io_0 ? LOW : HIGH);
  pinMode(3, OUTPUT);
  digitalWrite(3, hw_value_io_1 ? LOW : HIGH);
  pinMode(4, OUTPUT);
  digitalWrite(4, hw_value_io_2 ? LOW : HIGH);
  pinMode(5, OUTPUT);
  digitalWrite(5, hw_value_io_3 ? LOW : HIGH);
  pinMode(6, OUTPUT);
  digitalWrite(6, hw_value_io_4 ? LOW : HIGH);
  pinMode(7, OUTPUT);
  digitalWrite(7, hw_value_io_5 ? LOW : HIGH);
  pinMode(8, OUTPUT);
  digitalWrite(8, hw_value_io_6 ? LOW : HIGH);
  pinMode(9, OUTPUT);
  digitalWrite(9, hw_value_io_7 ? LOW : HIGH);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_8 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_8, &hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_io_7);
  digitalWrite(2, hw_value_io_0 ? LOW : HIGH);
  digitalWrite(3, hw_value_io_1 ? LOW : HIGH);
  digitalWrite(4, hw_value_io_2 ? LOW : HIGH);
  digitalWrite(5, hw_value_io_3 ? LOW : HIGH);
  digitalWrite(6, hw_value_io_4 ? LOW : HIGH);
  digitalWrite(7, hw_value_io_5 ? LOW : HIGH);
  digitalWrite(8, hw_value_io_6 ? LOW : HIGH);
  digitalWrite(9, hw_value_io_7 ? LOW : HIGH);
}
