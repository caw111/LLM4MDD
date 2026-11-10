#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_colorIndex = 0;
/* Global Variable define of  */
bool sysml_state_debouncedBtn1 = false;
/* Global Variable define of  */
bool sysml_state_debouncedBtn2 = false;
/* Global Variable define of  */
bool sysml_state_previousDebouncedBtn1 = false;
/* Global Variable define of  */
bool sysml_state_previousDebouncedBtn2 = false;
/* Global Variable define of  */
int sysml_state_stableCounter1 = 0;
/* Global Variable define of  */
int sysml_state_stableCounter2 = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_colorIndex = 0;
    sysml_state_debouncedBtn1 = false;
    sysml_state_debouncedBtn2 = false;
    sysml_state_previousDebouncedBtn1 = false;
    sysml_state_previousDebouncedBtn2 = false;
    sysml_state_stableCounter1 = 0;
    sysml_state_stableCounter2 = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool* io_2, bool* io_3, bool* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    int sysml_conditional_5 = 0;
    int sysml_conditional_6 = 0;
    bool sysml_lazy_7 = 0;
    int sysml_conditional_8 = 0;
    int sysml_conditional_9 = 0;
    bool sysml_lazy_10 = 0;
    bool sysml_lazy_11 = 0;
    bool sysml_conditional_12 = 0;
    bool sysml_lazy_13 = 0;
    bool sysml_lazy_14 = 0;
    bool sysml_lazy_15 = 0;
    bool sysml_conditional_16 = 0;
    bool sysml_lazy_17 = 0;
    bool sysml_lazy_18 = 0;
    bool sysml_lazy_19 = 0;
    bool sysml_conditional_20 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_debouncedBtn1;
    }
    if (sysml_lazy_0) {
        sysml_state_stableCounter1 = (sysml_state_stableCounter1 + 1);
        if (((sysml_state_stableCounter1 + 1) >= 2)) {
            sysml_state_debouncedBtn1 = true;
        }
    }
    else {
        sysml_lazy_1 = !io_0;
        if (sysml_lazy_1) {
            sysml_lazy_1 = sysml_state_debouncedBtn1;
        }
        if (sysml_lazy_1) {
            sysml_state_stableCounter1 = (sysml_state_stableCounter1 + 1);
            if (((sysml_state_stableCounter1 + 1) >= 2)) {
                sysml_state_debouncedBtn1 = false;
            }
        }
        else {
            sysml_state_stableCounter1 = 0;
        }
    }
    sysml_lazy_2 = io_1;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_debouncedBtn2;
    }
    if (sysml_lazy_2) {
        sysml_state_stableCounter2 = (sysml_state_stableCounter2 + 1);
        if (((sysml_state_stableCounter2 + 1) >= 2)) {
            sysml_state_debouncedBtn2 = true;
        }
    }
    else {
        sysml_lazy_3 = !io_1;
        if (sysml_lazy_3) {
            sysml_lazy_3 = sysml_state_debouncedBtn2;
        }
        if (sysml_lazy_3) {
            sysml_state_stableCounter2 = (sysml_state_stableCounter2 + 1);
            if (((sysml_state_stableCounter2 + 1) >= 2)) {
                sysml_state_debouncedBtn2 = false;
            }
        }
        else {
            sysml_state_stableCounter2 = 0;
        }
    }
    sysml_lazy_4 = sysml_state_debouncedBtn1;
    if (sysml_lazy_4) {
        sysml_lazy_4 = !sysml_state_previousDebouncedBtn1;
    }
    if (sysml_lazy_4) {
        if (sysml_state_colorIndex == 7) {
            sysml_conditional_5 = 0;
        }
        else {
            if (sysml_state_colorIndex == 0) {
                sysml_conditional_6 = 1;
            }
            else {
                sysml_conditional_6 = sysml_state_colorIndex + 1;
            }
            sysml_conditional_5 = sysml_conditional_6;
        }
        sysml_state_colorIndex = sysml_conditional_5;
    }
    sysml_lazy_7 = sysml_state_debouncedBtn2;
    if (sysml_lazy_7) {
        sysml_lazy_7 = !sysml_state_previousDebouncedBtn2;
    }
    if (sysml_lazy_7) {
        if (sysml_state_colorIndex == 1) {
            sysml_conditional_8 = 0;
        }
        else {
            if (sysml_state_colorIndex == 0) {
                sysml_conditional_9 = 7;
            }
            else {
                sysml_conditional_9 = sysml_state_colorIndex - 1;
            }
            sysml_conditional_8 = sysml_conditional_9;
        }
        sysml_state_colorIndex = sysml_conditional_8;
    }
    sysml_state_previousDebouncedBtn1 = sysml_state_debouncedBtn1;
    sysml_state_previousDebouncedBtn2 = sysml_state_debouncedBtn2;
    if ((sysml_state_colorIndex == 8)) {
        sysml_state_colorIndex = 0;
    }
    bool sysml_value_0 = 0;
    sysml_lazy_10 = sysml_state_colorIndex == 1;
    if (!sysml_lazy_10) {
        sysml_lazy_10 = sysml_state_colorIndex == 5;
    }
    sysml_lazy_11 = sysml_lazy_10;
    if (!sysml_lazy_11) {
        sysml_lazy_11 = sysml_state_colorIndex == 7;
    }
    if (sysml_lazy_11) {
        sysml_conditional_12 = true;
    }
    else {
        sysml_conditional_12 = false;
    }
    sysml_value_0 = sysml_conditional_12;
    bool sysml_value_1 = 0;
    sysml_lazy_13 = sysml_state_colorIndex == 2;
    if (!sysml_lazy_13) {
        sysml_lazy_13 = sysml_state_colorIndex == 4;
    }
    sysml_lazy_14 = sysml_lazy_13;
    if (!sysml_lazy_14) {
        sysml_lazy_14 = sysml_state_colorIndex == 5;
    }
    sysml_lazy_15 = sysml_lazy_14;
    if (!sysml_lazy_15) {
        sysml_lazy_15 = sysml_state_colorIndex == 7;
    }
    if (sysml_lazy_15) {
        sysml_conditional_16 = true;
    }
    else {
        sysml_conditional_16 = false;
    }
    sysml_value_1 = sysml_conditional_16;
    bool sysml_value_2 = 0;
    sysml_lazy_17 = sysml_state_colorIndex == 3;
    if (!sysml_lazy_17) {
        sysml_lazy_17 = sysml_state_colorIndex == 4;
    }
    sysml_lazy_18 = sysml_lazy_17;
    if (!sysml_lazy_18) {
        sysml_lazy_18 = sysml_state_colorIndex == 6;
    }
    sysml_lazy_19 = sysml_lazy_18;
    if (!sysml_lazy_19) {
        sysml_lazy_19 = sysml_state_colorIndex == 7;
    }
    if (sysml_lazy_19) {
        sysml_conditional_20 = true;
    }
    else {
        sysml_conditional_20 = false;
    }
    sysml_value_2 = sysml_conditional_20;
    *io_2 = sysml_value_0;
    *io_3 = sysml_value_1;
    *io_4 = sysml_value_2;
}