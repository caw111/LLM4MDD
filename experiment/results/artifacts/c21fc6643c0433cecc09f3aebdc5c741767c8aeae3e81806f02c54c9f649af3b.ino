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
extern int32_t sysml_state_stageIndex;
/* Global Variable define of  */
extern int32_t sysml_state_elapsedStageMs;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool* io_0, bool* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7, bool* io_8, bool* io_9, bool* io_10_segment_0, bool* io_10_segment_1, bool* io_10_segment_2, bool* io_10_segment_3);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_stageIndex = 1;
/* Global Variable define of  */
int32_t sysml_state_elapsedStageMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_stageIndex = 1;
    sysml_state_elapsedStageMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool* io_0, bool* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7, bool* io_8, bool* io_9, bool* io_10_segment_0, bool* io_10_segment_1, bool* io_10_segment_2, bool* io_10_segment_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int32_t sysml_conditional_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_conditional_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_conditional_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_conditional_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_conditional_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_conditional_10 = 0;
    bool sysml_lazy_11 = 0;
    bool sysml_conditional_12 = 0;
    bool sysml_lazy_13 = 0;
    bool sysml_conditional_14 = 0;
    bool sysml_lazy_15 = 0;
    bool sysml_conditional_16 = 0;
    bool sysml_lazy_17 = 0;
    bool sysml_conditional_18 = 0;
    bool sysml_lazy_19 = 0;
    bool sysml_conditional_20 = 0;
    bool sysml_lazy_21 = 0;
    bool sysml_conditional_22 = 0;
    bool sysml_lazy_23 = 0;
    bool sysml_conditional_24 = 0;
    bool sysml_lazy_25 = 0;
    bool sysml_conditional_26 = 0;
    bool sysml_lazy_27 = 0;
    bool sysml_conditional_28 = 0;
    bool sysml_lazy_29 = 0;
    bool sysml_conditional_30 = 0;
    bool sysml_lazy_31 = 0;
    bool sysml_conditional_32 = 0;
    bool sysml_lazy_33 = 0;
    bool sysml_conditional_34 = 0;
    bool sysml_lazy_35 = 0;
    bool sysml_conditional_36 = 0;
    bool sysml_lazy_37 = 0;
    bool sysml_conditional_38 = 0;
    bool sysml_lazy_39 = 0;
    bool sysml_conditional_40 = 0;
    bool sysml_lazy_41 = 0;
    bool sysml_conditional_42 = 0;
    bool sysml_lazy_43 = 0;
    bool sysml_conditional_44 = 0;
    bool sysml_lazy_45 = 0;
    bool sysml_conditional_46 = 0;
    bool sysml_lazy_47 = 0;
    bool sysml_conditional_48 = 0;
    bool sysml_lazy_49 = 0;
    bool sysml_conditional_50 = 0;
    bool sysml_lazy_51 = 0;
    bool sysml_conditional_52 = 0;
    bool sysml_lazy_53 = 0;
    bool sysml_conditional_54 = 0;
    bool sysml_lazy_55 = 0;
    bool sysml_conditional_56 = 0;
    bool sysml_lazy_57 = 0;
    bool sysml_conditional_58 = 0;
    bool sysml_lazy_59 = 0;
    bool sysml_conditional_60 = 0;
    bool sysml_lazy_61 = 0;
    bool sysml_conditional_62 = 0;
    bool sysml_lazy_63 = 0;
    bool sysml_conditional_64 = 0;
    bool sysml_lazy_65 = 0;
    bool sysml_conditional_66 = 0;
    bool sysml_lazy_67 = 0;
    bool sysml_conditional_68 = 0;
    bool sysml_lazy_69 = 0;
    bool sysml_conditional_70 = 0;
    if ((sysml_state_elapsedStageMs >= 2000)) {
        sysml_state_elapsedStageMs = 0;
        if (sysml_state_stageIndex >= 4) {
            sysml_conditional_0 = 1;
        }
        else {
            sysml_conditional_0 = sysml_state_stageIndex + 1;
        }
        sysml_state_stageIndex = sysml_conditional_0;
    }
    if ((sysml_state_elapsedStageMs < 2000)) {
        sysml_state_elapsedStageMs = (sysml_state_elapsedStageMs + 10);
    }
    bool sysml_value_0 = 0;
    sysml_lazy_1 = sysml_state_stageIndex == 2;
    if (sysml_lazy_1) {
        sysml_lazy_1 = true;
    }
    if (sysml_lazy_1) {
        sysml_conditional_2 = true;
    }
    else {
        sysml_conditional_2 = false;
    }
    sysml_value_0 = sysml_conditional_2;
    bool sysml_value_1 = 0;
    sysml_lazy_3 = sysml_state_stageIndex == 1;
    if (sysml_lazy_3) {
        sysml_lazy_3 = true;
    }
    if (sysml_lazy_3) {
        sysml_conditional_4 = true;
    }
    else {
        sysml_lazy_5 = sysml_state_stageIndex == 2;
        if (sysml_lazy_5) {
            sysml_lazy_5 = true;
        }
        if (sysml_lazy_5) {
            sysml_conditional_6 = false;
        }
        else {
            sysml_lazy_7 = sysml_state_stageIndex == 3;
            if (sysml_lazy_7) {
                sysml_lazy_7 = true;
            }
            if (sysml_lazy_7) {
                sysml_conditional_8 = false;
            }
            else {
                sysml_lazy_9 = sysml_state_stageIndex == 4;
                if (sysml_lazy_9) {
                    sysml_lazy_9 = true;
                }
                if (sysml_lazy_9) {
                    sysml_conditional_10 = true;
                }
                else {
                    sysml_conditional_10 = false;
                }
                sysml_conditional_8 = sysml_conditional_10;
            }
            sysml_conditional_6 = sysml_conditional_8;
        }
        sysml_conditional_4 = sysml_conditional_6;
    }
    sysml_value_1 = sysml_conditional_4;
    bool sysml_value_2 = 0;
    sysml_lazy_11 = sysml_state_stageIndex == 2;
    if (sysml_lazy_11) {
        sysml_lazy_11 = true;
    }
    if (sysml_lazy_11) {
        sysml_conditional_12 = true;
    }
    else {
        sysml_lazy_13 = sysml_state_stageIndex == 3;
        if (sysml_lazy_13) {
            sysml_lazy_13 = true;
        }
        if (sysml_lazy_13) {
            sysml_conditional_14 = false;
        }
        else {
            sysml_lazy_15 = sysml_state_stageIndex == 4;
            if (sysml_lazy_15) {
                sysml_lazy_15 = true;
            }
            if (sysml_lazy_15) {
                sysml_conditional_16 = true;
            }
            else {
                sysml_conditional_16 = false;
            }
            sysml_conditional_14 = sysml_conditional_16;
        }
        sysml_conditional_12 = sysml_conditional_14;
    }
    sysml_value_2 = sysml_conditional_12;
    bool sysml_value_3 = 0;
    sysml_lazy_17 = sysml_state_stageIndex == 1;
    if (sysml_lazy_17) {
        sysml_lazy_17 = true;
    }
    if (sysml_lazy_17) {
        sysml_conditional_18 = false;
    }
    else {
        sysml_lazy_19 = sysml_state_stageIndex == 3;
        if (sysml_lazy_19) {
            sysml_lazy_19 = true;
        }
        if (sysml_lazy_19) {
            sysml_conditional_20 = true;
        }
        else {
            sysml_lazy_21 = sysml_state_stageIndex == 4;
            if (sysml_lazy_21) {
                sysml_lazy_21 = true;
            }
            if (sysml_lazy_21) {
                sysml_conditional_22 = true;
            }
            else {
                sysml_conditional_22 = false;
            }
            sysml_conditional_20 = sysml_conditional_22;
        }
        sysml_conditional_18 = sysml_conditional_20;
    }
    sysml_value_3 = sysml_conditional_18;
    bool sysml_value_4 = 0;
    sysml_lazy_23 = sysml_state_stageIndex == 3;
    if (sysml_lazy_23) {
        sysml_lazy_23 = true;
    }
    if (sysml_lazy_23) {
        sysml_conditional_24 = true;
    }
    else {
        sysml_lazy_25 = sysml_state_stageIndex == 4;
        if (sysml_lazy_25) {
            sysml_lazy_25 = true;
        }
        if (sysml_lazy_25) {
            sysml_conditional_26 = true;
        }
        else {
            sysml_conditional_26 = false;
        }
        sysml_conditional_24 = sysml_conditional_26;
    }
    sysml_value_4 = sysml_conditional_24;
    bool sysml_value_5 = 0;
    sysml_lazy_27 = sysml_state_stageIndex == 1;
    if (sysml_lazy_27) {
        sysml_lazy_27 = true;
    }
    if (sysml_lazy_27) {
        sysml_conditional_28 = false;
    }
    else {
        sysml_lazy_29 = sysml_state_stageIndex == 3;
        if (sysml_lazy_29) {
            sysml_lazy_29 = true;
        }
        if (sysml_lazy_29) {
            sysml_conditional_30 = false;
        }
        else {
            sysml_lazy_31 = sysml_state_stageIndex == 4;
            if (sysml_lazy_31) {
                sysml_lazy_31 = true;
            }
            if (sysml_lazy_31) {
                sysml_conditional_32 = true;
            }
            else {
                sysml_lazy_33 = sysml_state_stageIndex == 2;
                if (sysml_lazy_33) {
                    sysml_lazy_33 = true;
                }
                if (sysml_lazy_33) {
                    sysml_conditional_34 = false;
                }
                else {
                    sysml_conditional_34 = false;
                }
                sysml_conditional_32 = sysml_conditional_34;
            }
            sysml_conditional_30 = sysml_conditional_32;
        }
        sysml_conditional_28 = sysml_conditional_30;
    }
    sysml_value_5 = sysml_conditional_28;
    bool sysml_value_6 = 0;
    sysml_lazy_35 = sysml_state_stageIndex == 2;
    if (sysml_lazy_35) {
        sysml_lazy_35 = true;
    }
    if (sysml_lazy_35) {
        sysml_conditional_36 = true;
    }
    else {
        sysml_lazy_37 = sysml_state_stageIndex == 3;
        if (sysml_lazy_37) {
            sysml_lazy_37 = true;
        }
        if (sysml_lazy_37) {
            sysml_conditional_38 = false;
        }
        else {
            sysml_lazy_39 = sysml_state_stageIndex == 4;
            if (sysml_lazy_39) {
                sysml_lazy_39 = true;
            }
            if (sysml_lazy_39) {
                sysml_conditional_40 = true;
            }
            else {
                sysml_conditional_40 = false;
            }
            sysml_conditional_38 = sysml_conditional_40;
        }
        sysml_conditional_36 = sysml_conditional_38;
    }
    sysml_value_6 = sysml_conditional_36;
    bool sysml_value_7 = 0;
    sysml_lazy_41 = sysml_state_stageIndex == 1;
    if (sysml_lazy_41) {
        sysml_lazy_41 = true;
    }
    if (sysml_lazy_41) {
        sysml_conditional_42 = false;
    }
    else {
        sysml_lazy_43 = sysml_state_stageIndex == 2;
        if (sysml_lazy_43) {
            sysml_lazy_43 = true;
        }
        if (sysml_lazy_43) {
            sysml_conditional_44 = false;
        }
        else {
            sysml_lazy_45 = sysml_state_stageIndex == 3;
            if (sysml_lazy_45) {
                sysml_lazy_45 = true;
            }
            if (sysml_lazy_45) {
                sysml_conditional_46 = true;
            }
            else {
                sysml_lazy_47 = sysml_state_stageIndex == 4;
                if (sysml_lazy_47) {
                    sysml_lazy_47 = true;
                }
                if (sysml_lazy_47) {
                    sysml_conditional_48 = true;
                }
                else {
                    sysml_conditional_48 = false;
                }
                sysml_conditional_46 = sysml_conditional_48;
            }
            sysml_conditional_44 = sysml_conditional_46;
        }
        sysml_conditional_42 = sysml_conditional_44;
    }
    sysml_value_7 = sysml_conditional_42;
    bool sysml_value_8 = 0;
    sysml_lazy_49 = sysml_state_stageIndex == 4;
    if (sysml_lazy_49) {
        sysml_lazy_49 = true;
    }
    if (sysml_lazy_49) {
        sysml_conditional_50 = true;
    }
    else {
        sysml_lazy_51 = sysml_state_stageIndex == 3;
        if (sysml_lazy_51) {
            sysml_lazy_51 = true;
        }
        if (sysml_lazy_51) {
            sysml_conditional_52 = true;
        }
        else {
            sysml_lazy_53 = sysml_state_stageIndex == 2;
            if (sysml_lazy_53) {
                sysml_lazy_53 = true;
            }
            if (sysml_lazy_53) {
                sysml_conditional_54 = false;
            }
            else {
                sysml_conditional_54 = false;
            }
            sysml_conditional_52 = sysml_conditional_54;
        }
        sysml_conditional_50 = sysml_conditional_52;
    }
    sysml_value_8 = sysml_conditional_50;
    bool sysml_value_9 = 0;
    sysml_lazy_55 = sysml_state_stageIndex == 4;
    if (sysml_lazy_55) {
        sysml_lazy_55 = true;
    }
    if (sysml_lazy_55) {
        sysml_conditional_56 = true;
    }
    else {
        sysml_lazy_57 = sysml_state_stageIndex == 2;
        if (sysml_lazy_57) {
            sysml_lazy_57 = true;
        }
        if (sysml_lazy_57) {
            sysml_conditional_58 = false;
        }
        else {
            sysml_lazy_59 = sysml_state_stageIndex == 1;
            if (sysml_lazy_59) {
                sysml_lazy_59 = true;
            }
            if (sysml_lazy_59) {
                sysml_conditional_60 = false;
            }
            else {
                sysml_lazy_61 = sysml_state_stageIndex == 3;
                if (sysml_lazy_61) {
                    sysml_lazy_61 = true;
                }
                if (sysml_lazy_61) {
                    sysml_conditional_62 = false;
                }
                else {
                    sysml_conditional_62 = false;
                }
                sysml_conditional_60 = sysml_conditional_62;
            }
            sysml_conditional_58 = sysml_conditional_60;
        }
        sysml_conditional_56 = sysml_conditional_58;
    }
    sysml_value_9 = sysml_conditional_56;
    bool sysml_value_10 = 0;
    sysml_lazy_63 = sysml_state_stageIndex >= 1;
    if (sysml_lazy_63) {
        sysml_lazy_63 = true;
    }
    if (sysml_lazy_63) {
        sysml_conditional_64 = true;
    }
    else {
        sysml_conditional_64 = false;
    }
    sysml_value_10 = sysml_conditional_64;
    bool sysml_value_11 = 0;
    sysml_lazy_65 = sysml_state_stageIndex >= 2;
    if (sysml_lazy_65) {
        sysml_lazy_65 = true;
    }
    if (sysml_lazy_65) {
        sysml_conditional_66 = true;
    }
    else {
        sysml_conditional_66 = false;
    }
    sysml_value_11 = sysml_conditional_66;
    bool sysml_value_12 = 0;
    sysml_lazy_67 = sysml_state_stageIndex >= 3;
    if (sysml_lazy_67) {
        sysml_lazy_67 = true;
    }
    if (sysml_lazy_67) {
        sysml_conditional_68 = true;
    }
    else {
        sysml_conditional_68 = false;
    }
    sysml_value_12 = sysml_conditional_68;
    bool sysml_value_13 = 0;
    sysml_lazy_69 = sysml_state_stageIndex == 4;
    if (sysml_lazy_69) {
        sysml_lazy_69 = true;
    }
    if (sysml_lazy_69) {
        sysml_conditional_70 = true;
    }
    else {
        sysml_conditional_70 = false;
    }
    sysml_value_13 = sysml_conditional_70;
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
    *io_10_segment_0 = sysml_value_10;
    *io_10_segment_1 = sysml_value_11;
    *io_10_segment_2 = sysml_value_12;
    *io_10_segment_3 = sysml_value_13;
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
static bool hw_value_io_4 = false;
static bool hw_value_io_5 = false;
static bool hw_value_io_6 = false;
static bool hw_value_io_7 = false;
static bool hw_value_io_8 = false;
static bool hw_value_io_9 = false;
static bool hw_value_io_10_segment_0 = false;
static bool hw_value_io_10_segment_1 = false;
static bool hw_value_io_10_segment_2 = false;
static bool hw_value_io_10_segment_3 = false;

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
  pinMode(11, OUTPUT);
  digitalWrite(11, hw_value_io_9 ? HIGH : LOW);
  pinMode(12, OUTPUT);
  digitalWrite(12, hw_value_io_10_segment_0 ? HIGH : LOW);
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_10_segment_1 ? HIGH : LOW);
  pinMode(44, OUTPUT);
  digitalWrite(44, hw_value_io_10_segment_2 ? HIGH : LOW);
  pinMode(45, OUTPUT);
  digitalWrite(45, hw_value_io_10_segment_3 ? HIGH : LOW);
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
  ProgramPage1(&hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_io_7, &hw_value_io_8, &hw_value_io_9, &hw_value_io_10_segment_0, &hw_value_io_10_segment_1, &hw_value_io_10_segment_2, &hw_value_io_10_segment_3);
  digitalWrite(2, hw_value_io_0 ? HIGH : LOW);
  digitalWrite(3, hw_value_io_1 ? HIGH : LOW);
  digitalWrite(4, hw_value_io_2 ? HIGH : LOW);
  digitalWrite(5, hw_value_io_3 ? HIGH : LOW);
  digitalWrite(6, hw_value_io_4 ? HIGH : LOW);
  digitalWrite(7, hw_value_io_5 ? HIGH : LOW);
  digitalWrite(8, hw_value_io_6 ? HIGH : LOW);
  digitalWrite(9, hw_value_io_7 ? HIGH : LOW);
  digitalWrite(10, hw_value_io_8 ? HIGH : LOW);
  digitalWrite(11, hw_value_io_9 ? HIGH : LOW);
  digitalWrite(12, hw_value_io_10_segment_0 ? HIGH : LOW);
  digitalWrite(13, hw_value_io_10_segment_1 ? HIGH : LOW);
  digitalWrite(44, hw_value_io_10_segment_2 ? HIGH : LOW);
  digitalWrite(45, hw_value_io_10_segment_3 ? HIGH : LOW);
}
