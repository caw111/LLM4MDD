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
extern int32_t sysml_state_mode;
/* Global Variable define of  */
extern bool sysml_state_previousPressed;
/* Global Variable define of  */
extern int32_t sysml_state_modeAgeMs;
/* Global Variable define of  */
extern int32_t sysml_state_rainbowPhaseMs;
/* Global Variable define of  */
extern int32_t sysml_state_led1SeqStage;
/* Global Variable define of  */
extern int32_t sysml_state_led3SeqStage;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool* io_1, bool* io_2, bool* io_3, double* io_4, double* io_5, double* io_6, double* io_7, double* io_8, double* io_9, double* io_10, double* io_11, double* io_12);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_mode = 1;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
int32_t sysml_state_modeAgeMs = 0;
/* Global Variable define of  */
int32_t sysml_state_rainbowPhaseMs = 0;
/* Global Variable define of  */
int32_t sysml_state_led1SeqStage = 0;
/* Global Variable define of  */
int32_t sysml_state_led3SeqStage = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_mode = 1;
    sysml_state_previousPressed = false;
    sysml_state_modeAgeMs = 0;
    sysml_state_rainbowPhaseMs = 0;
    sysml_state_led1SeqStage = 0;
    sysml_state_led3SeqStage = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool* io_1, bool* io_2, bool* io_3, double* io_4, double* io_5, double* io_6, double* io_7, double* io_8, double* io_9, double* io_10, double* io_11, double* io_12) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int32_t sysml_conditional_1 = 0;
    int32_t sysml_conditional_2 = 0;
    int32_t sysml_conditional_3 = 0;
    int32_t sysml_conditional_4 = 0;
    int32_t sysml_conditional_5 = 0;
    int32_t sysml_conditional_6 = 0;
    int32_t sysml_conditional_7 = 0;
    int32_t sysml_conditional_8 = 0;
    int32_t sysml_conditional_9 = 0;
    bool sysml_conditional_10 = 0;
    bool sysml_conditional_11 = 0;
    bool sysml_conditional_12 = 0;
    bool sysml_conditional_13 = 0;
    double sysml_conditional_14 = 0;
    double sysml_conditional_15 = 0;
    bool sysml_lazy_16 = 0;
    double sysml_conditional_17 = 0;
    bool sysml_lazy_18 = 0;
    double sysml_conditional_19 = 0;
    double sysml_conditional_20 = 0;
    double sysml_conditional_21 = 0;
    double sysml_conditional_22 = 0;
    bool sysml_lazy_23 = 0;
    double sysml_conditional_24 = 0;
    bool sysml_lazy_25 = 0;
    double sysml_conditional_26 = 0;
    double sysml_conditional_27 = 0;
    double sysml_conditional_28 = 0;
    double sysml_conditional_29 = 0;
    bool sysml_lazy_30 = 0;
    double sysml_conditional_31 = 0;
    bool sysml_lazy_32 = 0;
    double sysml_conditional_33 = 0;
    double sysml_conditional_34 = 0;
    double sysml_conditional_35 = 0;
    double sysml_conditional_36 = 0;
    bool sysml_lazy_37 = 0;
    double sysml_conditional_38 = 0;
    bool sysml_lazy_39 = 0;
    double sysml_conditional_40 = 0;
    double sysml_conditional_41 = 0;
    double sysml_conditional_42 = 0;
    double sysml_conditional_43 = 0;
    bool sysml_lazy_44 = 0;
    double sysml_conditional_45 = 0;
    bool sysml_lazy_46 = 0;
    double sysml_conditional_47 = 0;
    double sysml_conditional_48 = 0;
    double sysml_conditional_49 = 0;
    double sysml_conditional_50 = 0;
    bool sysml_lazy_51 = 0;
    double sysml_conditional_52 = 0;
    bool sysml_lazy_53 = 0;
    double sysml_conditional_54 = 0;
    double sysml_conditional_55 = 0;
    double sysml_conditional_56 = 0;
    double sysml_conditional_57 = 0;
    bool sysml_lazy_58 = 0;
    double sysml_conditional_59 = 0;
    bool sysml_lazy_60 = 0;
    double sysml_conditional_61 = 0;
    double sysml_conditional_62 = 0;
    double sysml_conditional_63 = 0;
    double sysml_conditional_64 = 0;
    bool sysml_lazy_65 = 0;
    double sysml_conditional_66 = 0;
    bool sysml_lazy_67 = 0;
    double sysml_conditional_68 = 0;
    double sysml_conditional_69 = 0;
    double sysml_conditional_70 = 0;
    double sysml_conditional_71 = 0;
    bool sysml_lazy_72 = 0;
    double sysml_conditional_73 = 0;
    bool sysml_lazy_74 = 0;
    double sysml_conditional_75 = 0;
    double sysml_conditional_76 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        if ((sysml_state_modeAgeMs >= 2000)) {
            if (sysml_state_mode == 1) {
                sysml_conditional_1 = 2;
            }
            else {
                if (sysml_state_mode == 2) {
                    sysml_conditional_2 = 3;
                }
                else {
                    if (sysml_state_mode == 3) {
                        sysml_conditional_3 = 4;
                    }
                    else {
                        sysml_conditional_3 = 1;
                    }
                    sysml_conditional_2 = sysml_conditional_3;
                }
                sysml_conditional_1 = sysml_conditional_2;
            }
            sysml_state_mode = sysml_conditional_1;
            sysml_state_modeAgeMs = 0;
            sysml_state_rainbowPhaseMs = 0;
            sysml_state_led1SeqStage = 0;
            sysml_state_led3SeqStage = 0;
        }
    }
    if ((sysml_state_mode == 2)) {
        sysml_state_rainbowPhaseMs = (sysml_state_rainbowPhaseMs + 5);
    }
    sysml_state_modeAgeMs = (sysml_state_modeAgeMs + 5);
    if ((sysml_state_mode == 1)) {
        if (sysml_state_modeAgeMs < 0) {
            sysml_conditional_4 = 0;
        }
        else {
            if (sysml_state_modeAgeMs < 20) {
                sysml_conditional_5 = 1;
            }
            else {
                if (sysml_state_modeAgeMs < 40) {
                    sysml_conditional_6 = 2;
                }
                else {
                    sysml_conditional_6 = 3;
                }
                sysml_conditional_5 = sysml_conditional_6;
            }
            sysml_conditional_4 = sysml_conditional_5;
        }
        sysml_state_led1SeqStage = sysml_conditional_4;
    }
    else {
        sysml_state_led1SeqStage = 0;
    }
    if ((sysml_state_mode == 3)) {
        if (sysml_state_modeAgeMs < 0) {
            sysml_conditional_7 = 0;
        }
        else {
            if (sysml_state_modeAgeMs < 20) {
                sysml_conditional_8 = 1;
            }
            else {
                if (sysml_state_modeAgeMs < 40) {
                    sysml_conditional_9 = 2;
                }
                else {
                    sysml_conditional_9 = 3;
                }
                sysml_conditional_8 = sysml_conditional_9;
            }
            sysml_conditional_7 = sysml_conditional_8;
        }
        sysml_state_led3SeqStage = sysml_conditional_7;
    }
    else {
        sysml_state_led3SeqStage = 0;
    }
    sysml_state_previousPressed = io_0;
    bool sysml_value_0 = 0;
    if (sysml_state_mode == 1) {
        if (sysml_state_led1SeqStage >= 1) {
            sysml_conditional_11 = true;
        }
        else {
            sysml_conditional_11 = false;
        }
        sysml_conditional_10 = sysml_conditional_11;
    }
    else {
        sysml_conditional_10 = false;
    }
    sysml_value_0 = sysml_conditional_10;
    bool sysml_value_1 = 0;
    sysml_value_1 = (sysml_state_mode == 2);
    bool sysml_value_2 = 0;
    if (sysml_state_mode == 3) {
        if (sysml_state_led3SeqStage >= 1) {
            sysml_conditional_13 = true;
        }
        else {
            sysml_conditional_13 = false;
        }
        sysml_conditional_12 = sysml_conditional_13;
    }
    else {
        sysml_conditional_12 = false;
    }
    sysml_value_2 = sysml_conditional_12;
    double sysml_value_3 = 0;
    if (sysml_state_mode == 1) {
        sysml_conditional_14 = 255.0;
    }
    else {
        if (sysml_state_mode == 2) {
            sysml_lazy_16 = sysml_state_rainbowPhaseMs >= 0;
            if (sysml_lazy_16) {
                sysml_lazy_16 = sysml_state_rainbowPhaseMs < 1000;
            }
            if (sysml_lazy_16) {
                sysml_conditional_17 = 255.0;
            }
            else {
                sysml_lazy_18 = sysml_state_rainbowPhaseMs >= 1000;
                if (sysml_lazy_18) {
                    sysml_lazy_18 = sysml_state_rainbowPhaseMs < 2000;
                }
                if (sysml_lazy_18) {
                    sysml_conditional_19 = 255.0;
                }
                else {
                    sysml_conditional_19 = 0.0;
                }
                sysml_conditional_17 = sysml_conditional_19;
            }
            sysml_conditional_15 = sysml_conditional_17;
        }
        else {
            if (sysml_state_mode == 3) {
                sysml_conditional_20 = 255.0;
            }
            else {
                sysml_conditional_20 = 0.0;
            }
            sysml_conditional_15 = sysml_conditional_20;
        }
        sysml_conditional_14 = sysml_conditional_15;
    }
    sysml_value_3 = sysml_conditional_14;
    double sysml_value_4 = 0;
    if (sysml_state_mode == 1) {
        sysml_conditional_21 = 255.0;
    }
    else {
        if (sysml_state_mode == 2) {
            sysml_lazy_23 = sysml_state_rainbowPhaseMs >= 0;
            if (sysml_lazy_23) {
                sysml_lazy_23 = sysml_state_rainbowPhaseMs < 1000;
            }
            if (sysml_lazy_23) {
                sysml_conditional_24 = 255.0;
            }
            else {
                sysml_lazy_25 = sysml_state_rainbowPhaseMs >= 1000;
                if (sysml_lazy_25) {
                    sysml_lazy_25 = sysml_state_rainbowPhaseMs < 2000;
                }
                if (sysml_lazy_25) {
                    sysml_conditional_26 = 0.0;
                }
                else {
                    sysml_conditional_26 = 255.0;
                }
                sysml_conditional_24 = sysml_conditional_26;
            }
            sysml_conditional_22 = sysml_conditional_24;
        }
        else {
            if (sysml_state_mode == 3) {
                sysml_conditional_27 = 255.0;
            }
            else {
                sysml_conditional_27 = 0.0;
            }
            sysml_conditional_22 = sysml_conditional_27;
        }
        sysml_conditional_21 = sysml_conditional_22;
    }
    sysml_value_4 = sysml_conditional_21;
    double sysml_value_5 = 0;
    if (sysml_state_mode == 1) {
        sysml_conditional_28 = 255.0;
    }
    else {
        if (sysml_state_mode == 2) {
            sysml_lazy_30 = sysml_state_rainbowPhaseMs >= 0;
            if (sysml_lazy_30) {
                sysml_lazy_30 = sysml_state_rainbowPhaseMs < 1000;
            }
            if (sysml_lazy_30) {
                sysml_conditional_31 = 0.0;
            }
            else {
                sysml_lazy_32 = sysml_state_rainbowPhaseMs >= 1000;
                if (sysml_lazy_32) {
                    sysml_lazy_32 = sysml_state_rainbowPhaseMs < 2000;
                }
                if (sysml_lazy_32) {
                    sysml_conditional_33 = 255.0;
                }
                else {
                    sysml_conditional_33 = 255.0;
                }
                sysml_conditional_31 = sysml_conditional_33;
            }
            sysml_conditional_29 = sysml_conditional_31;
        }
        else {
            if (sysml_state_mode == 3) {
                sysml_conditional_34 = 255.0;
            }
            else {
                sysml_conditional_34 = 0.0;
            }
            sysml_conditional_29 = sysml_conditional_34;
        }
        sysml_conditional_28 = sysml_conditional_29;
    }
    sysml_value_5 = sysml_conditional_28;
    double sysml_value_6 = 0;
    if (sysml_state_mode == 1) {
        sysml_conditional_35 = 255.0;
    }
    else {
        if (sysml_state_mode == 2) {
            sysml_lazy_37 = sysml_state_rainbowPhaseMs >= 0;
            if (sysml_lazy_37) {
                sysml_lazy_37 = sysml_state_rainbowPhaseMs < 1000;
            }
            if (sysml_lazy_37) {
                sysml_conditional_38 = 255.0;
            }
            else {
                sysml_lazy_39 = sysml_state_rainbowPhaseMs >= 1000;
                if (sysml_lazy_39) {
                    sysml_lazy_39 = sysml_state_rainbowPhaseMs < 2000;
                }
                if (sysml_lazy_39) {
                    sysml_conditional_40 = 255.0;
                }
                else {
                    sysml_conditional_40 = 0.0;
                }
                sysml_conditional_38 = sysml_conditional_40;
            }
            sysml_conditional_36 = sysml_conditional_38;
        }
        else {
            if (sysml_state_mode == 3) {
                sysml_conditional_41 = 255.0;
            }
            else {
                sysml_conditional_41 = 0.0;
            }
            sysml_conditional_36 = sysml_conditional_41;
        }
        sysml_conditional_35 = sysml_conditional_36;
    }
    sysml_value_6 = sysml_conditional_35;
    double sysml_value_7 = 0;
    if (sysml_state_mode == 1) {
        sysml_conditional_42 = 255.0;
    }
    else {
        if (sysml_state_mode == 2) {
            sysml_lazy_44 = sysml_state_rainbowPhaseMs >= 0;
            if (sysml_lazy_44) {
                sysml_lazy_44 = sysml_state_rainbowPhaseMs < 1000;
            }
            if (sysml_lazy_44) {
                sysml_conditional_45 = 255.0;
            }
            else {
                sysml_lazy_46 = sysml_state_rainbowPhaseMs >= 1000;
                if (sysml_lazy_46) {
                    sysml_lazy_46 = sysml_state_rainbowPhaseMs < 2000;
                }
                if (sysml_lazy_46) {
                    sysml_conditional_47 = 0.0;
                }
                else {
                    sysml_conditional_47 = 255.0;
                }
                sysml_conditional_45 = sysml_conditional_47;
            }
            sysml_conditional_43 = sysml_conditional_45;
        }
        else {
            if (sysml_state_mode == 3) {
                sysml_conditional_48 = 255.0;
            }
            else {
                sysml_conditional_48 = 0.0;
            }
            sysml_conditional_43 = sysml_conditional_48;
        }
        sysml_conditional_42 = sysml_conditional_43;
    }
    sysml_value_7 = sysml_conditional_42;
    double sysml_value_8 = 0;
    if (sysml_state_mode == 1) {
        sysml_conditional_49 = 255.0;
    }
    else {
        if (sysml_state_mode == 2) {
            sysml_lazy_51 = sysml_state_rainbowPhaseMs >= 0;
            if (sysml_lazy_51) {
                sysml_lazy_51 = sysml_state_rainbowPhaseMs < 1000;
            }
            if (sysml_lazy_51) {
                sysml_conditional_52 = 0.0;
            }
            else {
                sysml_lazy_53 = sysml_state_rainbowPhaseMs >= 1000;
                if (sysml_lazy_53) {
                    sysml_lazy_53 = sysml_state_rainbowPhaseMs < 2000;
                }
                if (sysml_lazy_53) {
                    sysml_conditional_54 = 255.0;
                }
                else {
                    sysml_conditional_54 = 255.0;
                }
                sysml_conditional_52 = sysml_conditional_54;
            }
            sysml_conditional_50 = sysml_conditional_52;
        }
        else {
            if (sysml_state_mode == 3) {
                sysml_conditional_55 = 255.0;
            }
            else {
                sysml_conditional_55 = 0.0;
            }
            sysml_conditional_50 = sysml_conditional_55;
        }
        sysml_conditional_49 = sysml_conditional_50;
    }
    sysml_value_8 = sysml_conditional_49;
    double sysml_value_9 = 0;
    if (sysml_state_mode == 1) {
        sysml_conditional_56 = 255.0;
    }
    else {
        if (sysml_state_mode == 2) {
            sysml_lazy_58 = sysml_state_rainbowPhaseMs >= 0;
            if (sysml_lazy_58) {
                sysml_lazy_58 = sysml_state_rainbowPhaseMs < 1000;
            }
            if (sysml_lazy_58) {
                sysml_conditional_59 = 255.0;
            }
            else {
                sysml_lazy_60 = sysml_state_rainbowPhaseMs >= 1000;
                if (sysml_lazy_60) {
                    sysml_lazy_60 = sysml_state_rainbowPhaseMs < 2000;
                }
                if (sysml_lazy_60) {
                    sysml_conditional_61 = 255.0;
                }
                else {
                    sysml_conditional_61 = 0.0;
                }
                sysml_conditional_59 = sysml_conditional_61;
            }
            sysml_conditional_57 = sysml_conditional_59;
        }
        else {
            if (sysml_state_mode == 3) {
                sysml_conditional_62 = 255.0;
            }
            else {
                sysml_conditional_62 = 0.0;
            }
            sysml_conditional_57 = sysml_conditional_62;
        }
        sysml_conditional_56 = sysml_conditional_57;
    }
    sysml_value_9 = sysml_conditional_56;
    double sysml_value_10 = 0;
    if (sysml_state_mode == 1) {
        sysml_conditional_63 = 255.0;
    }
    else {
        if (sysml_state_mode == 2) {
            sysml_lazy_65 = sysml_state_rainbowPhaseMs >= 0;
            if (sysml_lazy_65) {
                sysml_lazy_65 = sysml_state_rainbowPhaseMs < 1000;
            }
            if (sysml_lazy_65) {
                sysml_conditional_66 = 255.0;
            }
            else {
                sysml_lazy_67 = sysml_state_rainbowPhaseMs >= 1000;
                if (sysml_lazy_67) {
                    sysml_lazy_67 = sysml_state_rainbowPhaseMs < 2000;
                }
                if (sysml_lazy_67) {
                    sysml_conditional_68 = 0.0;
                }
                else {
                    sysml_conditional_68 = 255.0;
                }
                sysml_conditional_66 = sysml_conditional_68;
            }
            sysml_conditional_64 = sysml_conditional_66;
        }
        else {
            if (sysml_state_mode == 3) {
                sysml_conditional_69 = 255.0;
            }
            else {
                sysml_conditional_69 = 0.0;
            }
            sysml_conditional_64 = sysml_conditional_69;
        }
        sysml_conditional_63 = sysml_conditional_64;
    }
    sysml_value_10 = sysml_conditional_63;
    double sysml_value_11 = 0;
    if (sysml_state_mode == 1) {
        sysml_conditional_70 = 255.0;
    }
    else {
        if (sysml_state_mode == 2) {
            sysml_lazy_72 = sysml_state_rainbowPhaseMs >= 0;
            if (sysml_lazy_72) {
                sysml_lazy_72 = sysml_state_rainbowPhaseMs < 1000;
            }
            if (sysml_lazy_72) {
                sysml_conditional_73 = 0.0;
            }
            else {
                sysml_lazy_74 = sysml_state_rainbowPhaseMs >= 1000;
                if (sysml_lazy_74) {
                    sysml_lazy_74 = sysml_state_rainbowPhaseMs < 2000;
                }
                if (sysml_lazy_74) {
                    sysml_conditional_75 = 255.0;
                }
                else {
                    sysml_conditional_75 = 255.0;
                }
                sysml_conditional_73 = sysml_conditional_75;
            }
            sysml_conditional_71 = sysml_conditional_73;
        }
        else {
            if (sysml_state_mode == 3) {
                sysml_conditional_76 = 255.0;
            }
            else {
                sysml_conditional_76 = 0.0;
            }
            sysml_conditional_71 = sysml_conditional_76;
        }
        sysml_conditional_70 = sysml_conditional_71;
    }
    sysml_value_11 = sysml_conditional_70;
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
    *io_11 = sysml_value_10;
    *io_12 = sysml_value_11;
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
static bool hw_value_io_2 = false;
static bool hw_value_io_3 = false;
static double hw_value_io_4 = 0;
static double hw_value_io_5 = 0;
static double hw_value_io_6 = 0;
static double hw_value_io_7 = 0;
static double hw_value_io_8 = 0;
static double hw_value_io_9 = 0;
static double hw_value_io_10 = 0;
static double hw_value_io_11 = 0;
static double hw_value_io_12 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(5, INPUT);
  pinMode(2, OUTPUT);
  digitalWrite(2, hw_value_io_1 ? HIGH : LOW);
  pinMode(3, OUTPUT);
  digitalWrite(3, hw_value_io_2 ? HIGH : LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, hw_value_io_3 ? HIGH : LOW);
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
  pinMode(11, OUTPUT);
  analogWrite(11, (int)constrain((hw_value_io_9 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(12, OUTPUT);
  analogWrite(12, (int)constrain((hw_value_io_10 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(13, OUTPUT);
  analogWrite(13, (int)constrain((hw_value_io_11 * 1.0 + 0.0), 0.0, 255.0));
  pinMode(44, OUTPUT);
  analogWrite(44, (int)constrain((hw_value_io_12 * 1.0 + 0.0), 0.0, 255.0));
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 5UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (digitalRead(5) == HIGH);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_3, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_io_7, &hw_value_io_8, &hw_value_io_9, &hw_value_io_10, &hw_value_io_11, &hw_value_io_12);
  digitalWrite(2, hw_value_io_1 ? HIGH : LOW);
  digitalWrite(3, hw_value_io_2 ? HIGH : LOW);
  digitalWrite(4, hw_value_io_3 ? HIGH : LOW);
  analogWrite(6, (int)constrain((hw_value_io_4 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(7, (int)constrain((hw_value_io_5 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(8, (int)constrain((hw_value_io_6 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(9, (int)constrain((hw_value_io_7 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(10, (int)constrain((hw_value_io_8 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(11, (int)constrain((hw_value_io_9 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(12, (int)constrain((hw_value_io_10 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(13, (int)constrain((hw_value_io_11 * 1.0 + 0.0), 0.0, 255.0));
  analogWrite(44, (int)constrain((hw_value_io_12 * 1.0 + 0.0), 0.0, 255.0));
}
