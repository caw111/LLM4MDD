#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_countdownValue = 9;
/* Global Variable define of  */
int sysml_state_elapsedAccumMs = 0;
/* Global Variable define of  */
int sysml_state_ledStepIndex = 0;
/* Global Variable define of  */
int sysml_state_rgbColorIndex = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_countdownValue = 9;
    sysml_state_elapsedAccumMs = 0;
    sysml_state_ledStepIndex = 0;
    sysml_state_rgbColorIndex = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_0, bool* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7, bool* io_8, bool* io_9, int* io_10) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
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
    int sysml_conditional_21 = 0;
    int sysml_conditional_22 = 0;
    int sysml_conditional_23 = 0;
    bool sysml_lazy_24 = 0;
    int sysml_conditional_25 = 0;
    int sysml_arithmetic_26 = 0;
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
    int sysml_value_9 = 0;
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