#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_countdownValue = 9;
/* Global Variable define of  */
int sysml_state_elapsedInStepMs = 0;
/* Global Variable define of  */
int sysml_state_ledIndex = 1;
/* Global Variable define of  */
int sysml_state_rgbPhase = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_countdownValue = 9;
    sysml_state_elapsedInStepMs = 0;
    sysml_state_ledIndex = 1;
    sysml_state_rgbPhase = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_10, int* io_0, bool* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7, bool* io_8, bool* io_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_conditional_2 = 0;
    bool sysml_conditional_3 = 0;
    bool sysml_conditional_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_conditional_6 = 0;
    bool sysml_conditional_7 = 0;
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
    sysml_lazy_0 = sysml_state_elapsedInStepMs >= 2000;
    if (sysml_lazy_0) {
        sysml_lazy_0 = sysml_state_countdownValue != 0;
    }
    if (sysml_lazy_0) {
        sysml_state_elapsedInStepMs = (sysml_state_elapsedInStepMs - 2000);
        sysml_state_countdownValue = (sysml_state_countdownValue - 1);
        sysml_state_ledIndex = (sysml_state_ledIndex + 1);
        sysml_state_rgbPhase = (sysml_state_rgbPhase + 1);
        if ((sysml_state_ledIndex >= 4)) {
            sysml_state_ledIndex = 1;
        }
        if ((sysml_state_rgbPhase >= 3)) {
            sysml_state_rgbPhase = 0;
        }
    }
    sysml_lazy_1 = sysml_state_countdownValue != 0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = sysml_state_elapsedInStepMs < 2000;
    }
    if (sysml_lazy_1) {
        sysml_state_elapsedInStepMs = (sysml_state_elapsedInStepMs + io_10);
    }
    if ((sysml_state_countdownValue == 0)) {
        sysml_state_elapsedInStepMs = 0;
        sysml_state_countdownValue = 9;
        sysml_state_ledIndex = 1;
        sysml_state_rgbPhase = 0;
    }
    int sysml_value_0 = 0;
    sysml_value_0 = sysml_state_countdownValue;
    bool sysml_value_1 = 0;
    if (sysml_state_ledIndex == 1) {
        sysml_conditional_2 = true;
    }
    else {
        sysml_conditional_2 = false;
    }
    sysml_value_1 = sysml_conditional_2;
    bool sysml_value_2 = 0;
    if (sysml_state_ledIndex == 2) {
        sysml_conditional_3 = true;
    }
    else {
        sysml_conditional_3 = false;
    }
    sysml_value_2 = sysml_conditional_3;
    bool sysml_value_3 = 0;
    if (sysml_state_ledIndex == 3) {
        sysml_conditional_4 = true;
    }
    else {
        sysml_conditional_4 = false;
    }
    sysml_value_3 = sysml_conditional_4;
    bool sysml_value_4 = 0;
    sysml_lazy_5 = sysml_state_countdownValue == 0;
    if (sysml_lazy_5) {
        sysml_lazy_5 = true;
    }
    if (sysml_lazy_5) {
        sysml_conditional_6 = true;
    }
    else {
        if (sysml_state_rgbPhase == 0) {
            sysml_conditional_7 = true;
        }
        else {
            sysml_conditional_7 = false;
        }
        sysml_conditional_6 = sysml_conditional_7;
    }
    sysml_value_4 = sysml_conditional_6;
    bool sysml_value_5 = 0;
    if (sysml_state_countdownValue == 0) {
        sysml_conditional_8 = true;
    }
    else {
        if (sysml_state_rgbPhase == 1) {
            sysml_conditional_9 = true;
        }
        else {
            sysml_conditional_9 = false;
        }
        sysml_conditional_8 = sysml_conditional_9;
    }
    sysml_value_5 = sysml_conditional_8;
    bool sysml_value_6 = 0;
    if (sysml_state_countdownValue == 0) {
        sysml_conditional_10 = true;
    }
    else {
        if (sysml_state_rgbPhase == 2) {
            sysml_conditional_11 = true;
        }
        else {
            sysml_conditional_11 = false;
        }
        sysml_conditional_10 = sysml_conditional_11;
    }
    sysml_value_6 = sysml_conditional_10;
    bool sysml_value_7 = 0;
    if (sysml_state_countdownValue == 0) {
        sysml_conditional_12 = true;
    }
    else {
        if (sysml_state_rgbPhase == 0) {
            sysml_conditional_13 = true;
        }
        else {
            sysml_conditional_13 = false;
        }
        sysml_conditional_12 = sysml_conditional_13;
    }
    sysml_value_7 = sysml_conditional_12;
    bool sysml_value_8 = 0;
    if (sysml_state_countdownValue == 0) {
        sysml_conditional_14 = true;
    }
    else {
        if (sysml_state_rgbPhase == 1) {
            sysml_conditional_15 = true;
        }
        else {
            sysml_conditional_15 = false;
        }
        sysml_conditional_14 = sysml_conditional_15;
    }
    sysml_value_8 = sysml_conditional_14;
    bool sysml_value_9 = 0;
    if (sysml_state_countdownValue == 0) {
        sysml_conditional_16 = true;
    }
    else {
        if (sysml_state_rgbPhase == 2) {
            sysml_conditional_17 = true;
        }
        else {
            sysml_conditional_17 = false;
        }
        sysml_conditional_16 = sysml_conditional_17;
    }
    sysml_value_9 = sysml_conditional_16;
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
}