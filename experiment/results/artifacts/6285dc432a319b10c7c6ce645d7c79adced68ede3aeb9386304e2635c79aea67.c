#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_mode1Index = 0;
/* Global Variable define of  */
int sysml_state_mode2Index = 0;
/* Global Variable define of  */
bool sysml_state_previousPressedBtn1 = false;
/* Global Variable define of  */
bool sysml_state_previousPressedBtn2 = false;
/* Global Variable define of  */
bool sysml_state_isSecondMode = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_mode1Index = 0;
    sysml_state_mode2Index = 0;
    sysml_state_previousPressedBtn1 = false;
    sysml_state_previousPressedBtn2 = false;
    sysml_state_isSecondMode = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double io_2, double* io_3, double* io_4, double* io_5) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    int sysml_conditional_3 = 0;
    double sysml_conditional_4 = 0;
    double sysml_conditional_5 = 0;
    int sysml_conditional_6 = 0;
    double sysml_conditional_7 = 0;
    double sysml_conditional_8 = 0;
    int sysml_conditional_9 = 0;
    double sysml_conditional_10 = 0;
    int sysml_conditional_11 = 0;
    double sysml_conditional_12 = 0;
    double sysml_conditional_13 = 0;
    double sysml_conditional_14 = 0;
    double sysml_conditional_15 = 0;
    int sysml_conditional_16 = 0;
    double sysml_conditional_17 = 0;
    int sysml_conditional_18 = 0;
    double sysml_conditional_19 = 0;
    double sysml_conditional_20 = 0;
    double sysml_conditional_21 = 0;
    int sysml_conditional_22 = 0;
    double sysml_conditional_23 = 0;
    double sysml_conditional_24 = 0;
    double sysml_conditional_25 = 0;
    double sysml_conditional_26 = 0;
    double sysml_conditional_27 = 0;
    int sysml_conditional_28 = 0;
    double sysml_conditional_29 = 0;
    double sysml_conditional_30 = 0;
    double sysml_conditional_31 = 0;
    int sysml_conditional_32 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressedBtn1;
    }
    if (sysml_lazy_0) {
        if (sysml_state_isSecondMode) {
            sysml_state_isSecondMode = false;
        }
        else {
            if (sysml_state_mode1Index == 2) {
                sysml_conditional_1 = 0;
            }
            else {
                sysml_conditional_1 = sysml_state_mode1Index + 1;
            }
            sysml_state_mode1Index = sysml_conditional_1;
        }
    }
    sysml_lazy_2 = io_1;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_previousPressedBtn2;
    }
    if (sysml_lazy_2) {
        if (sysml_state_isSecondMode) {
            if (sysml_state_mode2Index == 2) {
                sysml_conditional_3 = 0;
            }
            else {
                sysml_conditional_3 = sysml_state_mode2Index + 1;
            }
            sysml_state_mode2Index = sysml_conditional_3;
        }
        else {
            sysml_state_isSecondMode = true;
        }
    }
    sysml_state_previousPressedBtn1 = io_0;
    sysml_state_previousPressedBtn2 = io_1;
    double sysml_value_0 = 0;
    if (sysml_state_isSecondMode) {
        if (sysml_state_mode2Index == 0) {
            if ((((double)(io_2 * 255.0)) / 1023.0) < ((double)((int)(((double)(io_2 * 255.0)) / 1023.0)))) {
                sysml_conditional_6 = 1;
            }
            else {
                sysml_conditional_6 = 0;
            }
            sysml_conditional_5 = ((int)(((double)(io_2 * 255.0)) / 1023.0)) - sysml_conditional_6;
        }
        else {
            if (sysml_state_mode2Index == 1) {
                sysml_conditional_7 = 0.0;
            }
            else {
                if (sysml_state_mode2Index == 2) {
                    if ((((double)(io_2 * 255.0)) / 1023.0) < ((double)((int)(((double)(io_2 * 255.0)) / 1023.0)))) {
                        sysml_conditional_9 = 1;
                    }
                    else {
                        sysml_conditional_9 = 0;
                    }
                    sysml_conditional_8 = ((int)(((double)(io_2 * 255.0)) / 1023.0)) - sysml_conditional_9;
                }
                else {
                    sysml_conditional_8 = 0.0;
                }
                sysml_conditional_7 = sysml_conditional_8;
            }
            sysml_conditional_5 = sysml_conditional_7;
        }
        sysml_conditional_4 = sysml_conditional_5;
    }
    else {
        if (sysml_state_mode1Index == 0) {
            if ((((double)(io_2 * 255.0)) / 1023.0) < ((double)((int)(((double)(io_2 * 255.0)) / 1023.0)))) {
                sysml_conditional_11 = 1;
            }
            else {
                sysml_conditional_11 = 0;
            }
            sysml_conditional_10 = ((int)(((double)(io_2 * 255.0)) / 1023.0)) - sysml_conditional_11;
        }
        else {
            if (sysml_state_mode1Index == 1) {
                sysml_conditional_12 = 0.0;
            }
            else {
                if (sysml_state_mode1Index == 2) {
                    sysml_conditional_13 = 0.0;
                }
                else {
                    sysml_conditional_13 = 0.0;
                }
                sysml_conditional_12 = sysml_conditional_13;
            }
            sysml_conditional_10 = sysml_conditional_12;
        }
        sysml_conditional_4 = sysml_conditional_10;
    }
    sysml_value_0 = sysml_conditional_4;
    double sysml_value_1 = 0;
    if (sysml_state_isSecondMode) {
        if (sysml_state_mode2Index == 0) {
            if ((((double)(io_2 * 255.0)) / 1023.0) < ((double)((int)(((double)(io_2 * 255.0)) / 1023.0)))) {
                sysml_conditional_16 = 1;
            }
            else {
                sysml_conditional_16 = 0;
            }
            sysml_conditional_15 = ((int)(((double)(io_2 * 255.0)) / 1023.0)) - sysml_conditional_16;
        }
        else {
            if (sysml_state_mode2Index == 1) {
                if ((((double)(io_2 * 255.0)) / 1023.0) < ((double)((int)(((double)(io_2 * 255.0)) / 1023.0)))) {
                    sysml_conditional_18 = 1;
                }
                else {
                    sysml_conditional_18 = 0;
                }
                sysml_conditional_17 = ((int)(((double)(io_2 * 255.0)) / 1023.0)) - sysml_conditional_18;
            }
            else {
                if (sysml_state_mode2Index == 2) {
                    sysml_conditional_19 = 0.0;
                }
                else {
                    sysml_conditional_19 = 0.0;
                }
                sysml_conditional_17 = sysml_conditional_19;
            }
            sysml_conditional_15 = sysml_conditional_17;
        }
        sysml_conditional_14 = sysml_conditional_15;
    }
    else {
        if (sysml_state_mode1Index == 0) {
            sysml_conditional_20 = 0.0;
        }
        else {
            if (sysml_state_mode1Index == 1) {
                if ((((double)(io_2 * 255.0)) / 1023.0) < ((double)((int)(((double)(io_2 * 255.0)) / 1023.0)))) {
                    sysml_conditional_22 = 1;
                }
                else {
                    sysml_conditional_22 = 0;
                }
                sysml_conditional_21 = ((int)(((double)(io_2 * 255.0)) / 1023.0)) - sysml_conditional_22;
            }
            else {
                if (sysml_state_mode1Index == 2) {
                    sysml_conditional_23 = 0.0;
                }
                else {
                    sysml_conditional_23 = 0.0;
                }
                sysml_conditional_21 = sysml_conditional_23;
            }
            sysml_conditional_20 = sysml_conditional_21;
        }
        sysml_conditional_14 = sysml_conditional_20;
    }
    sysml_value_1 = sysml_conditional_14;
    double sysml_value_2 = 0;
    if (sysml_state_isSecondMode) {
        if (sysml_state_mode2Index == 0) {
            sysml_conditional_25 = 0.0;
        }
        else {
            if (sysml_state_mode2Index == 1) {
                sysml_conditional_26 = 0.0;
            }
            else {
                if (sysml_state_mode2Index == 2) {
                    if ((((double)(io_2 * 255.0)) / 1023.0) < ((double)((int)(((double)(io_2 * 255.0)) / 1023.0)))) {
                        sysml_conditional_28 = 1;
                    }
                    else {
                        sysml_conditional_28 = 0;
                    }
                    sysml_conditional_27 = ((int)(((double)(io_2 * 255.0)) / 1023.0)) - sysml_conditional_28;
                }
                else {
                    sysml_conditional_27 = 0.0;
                }
                sysml_conditional_26 = sysml_conditional_27;
            }
            sysml_conditional_25 = sysml_conditional_26;
        }
        sysml_conditional_24 = sysml_conditional_25;
    }
    else {
        if (sysml_state_mode1Index == 0) {
            sysml_conditional_29 = 0.0;
        }
        else {
            if (sysml_state_mode1Index == 1) {
                sysml_conditional_30 = 0.0;
            }
            else {
                if (sysml_state_mode1Index == 2) {
                    if ((((double)(io_2 * 255.0)) / 1023.0) < ((double)((int)(((double)(io_2 * 255.0)) / 1023.0)))) {
                        sysml_conditional_32 = 1;
                    }
                    else {
                        sysml_conditional_32 = 0;
                    }
                    sysml_conditional_31 = ((int)(((double)(io_2 * 255.0)) / 1023.0)) - sysml_conditional_32;
                }
                else {
                    sysml_conditional_31 = 0.0;
                }
                sysml_conditional_30 = sysml_conditional_31;
            }
            sysml_conditional_29 = sysml_conditional_30;
        }
        sysml_conditional_24 = sysml_conditional_29;
    }
    sysml_value_2 = sysml_conditional_24;
    *io_3 = sysml_value_0;
    *io_4 = sysml_value_1;
    *io_5 = sysml_value_2;
}