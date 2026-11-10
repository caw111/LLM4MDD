#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_mode1Index = 0;
/* Global Variable define of  */
int sysml_state_mode2Index = 0;
/* Global Variable define of  */
bool sysml_state_rgbOff = false;
/* Global Variable define of  */
bool sysml_state_prevIo0 = false;
/* Global Variable define of  */
bool sysml_state_prevIo1 = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_mode1Index = 0;
    sysml_state_mode2Index = 0;
    sysml_state_rgbOff = false;
    sysml_state_prevIo0 = false;
    sysml_state_prevIo1 = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double io_2, bool* io_3, bool* io_4, bool* io_5) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    bool sysml_conditional_9 = 0;
    bool sysml_conditional_10 = 0;
    bool sysml_conditional_11 = 0;
    bool sysml_conditional_12 = 0;
    bool sysml_conditional_13 = 0;
    bool sysml_conditional_14 = 0;
    bool sysml_conditional_15 = 0;
    bool sysml_conditional_16 = 0;
    bool sysml_conditional_17 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_prevIo0;
    }
    if (sysml_lazy_0) {
        if ((!sysml_state_rgbOff)) {
            if ((sysml_state_mode1Index == 0)) {
                sysml_state_mode1Index = 1;
            }
            else {
                if ((sysml_state_mode1Index == 1)) {
                    sysml_state_mode1Index = 2;
                }
                else {
                    sysml_state_mode1Index = 0;
                }
            }
        }
        else {
            sysml_state_mode1Index = 0;
        }
    }
    sysml_lazy_1 = io_1;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_prevIo1;
    }
    if (sysml_lazy_1) {
        if ((!sysml_state_rgbOff)) {
            if ((sysml_state_mode2Index == 0)) {
                sysml_state_mode2Index = 1;
            }
            else {
                if ((sysml_state_mode2Index == 1)) {
                    sysml_state_mode2Index = 2;
                }
                else {
                    sysml_state_mode2Index = 0;
                }
            }
        }
        else {
            sysml_state_mode2Index = 0;
        }
    }
    sysml_lazy_2 = !sysml_state_rgbOff;
    if (sysml_lazy_2) {
        sysml_lazy_2 = io_1;
    }
    if (sysml_lazy_2) {
        sysml_lazy_3 = sysml_state_mode1Index == 1;
        if (!sysml_lazy_3) {
            sysml_lazy_3 = sysml_state_mode1Index == 2;
        }
        if (sysml_lazy_3) {
        }
        else {
            if ((sysml_state_mode1Index == 0)) {
                sysml_state_rgbOff = true;
            }
        }
    }
    sysml_lazy_4 = !sysml_state_rgbOff;
    if (sysml_lazy_4) {
        sysml_lazy_4 = io_0;
    }
    if (sysml_lazy_4) {
        sysml_lazy_5 = sysml_state_mode2Index == 0;
        if (!sysml_lazy_5) {
            sysml_lazy_5 = sysml_state_mode2Index == 1;
        }
        sysml_lazy_6 = sysml_lazy_5;
        if (!sysml_lazy_6) {
            sysml_lazy_6 = sysml_state_mode2Index == 2;
        }
        if (sysml_lazy_6) {
            if ((sysml_state_mode2Index == 0)) {
                sysml_state_rgbOff = true;
            }
        }
    }
    sysml_lazy_7 = sysml_state_rgbOff;
    if (sysml_lazy_7) {
        sysml_lazy_8 = io_0;
        if (!sysml_lazy_8) {
            sysml_lazy_8 = io_1;
        }
        sysml_lazy_7 = sysml_lazy_8;
    }
    if (sysml_lazy_7) {
        sysml_state_mode1Index = 0;
        sysml_state_mode2Index = 0;
    }
    sysml_state_prevIo0 = io_0;
    sysml_state_prevIo1 = io_1;
    bool sysml_value_0 = 0;
    if (sysml_state_rgbOff) {
        sysml_conditional_9 = false;
    }
    else {
        if (sysml_state_mode2Index == 0) {
            sysml_conditional_10 = true;
        }
        else {
            if (sysml_state_mode1Index == 0) {
                sysml_conditional_11 = true;
            }
            else {
                sysml_conditional_11 = false;
            }
            sysml_conditional_10 = sysml_conditional_11;
        }
        sysml_conditional_9 = sysml_conditional_10;
    }
    sysml_value_0 = sysml_conditional_9;
    bool sysml_value_1 = 0;
    if (sysml_state_rgbOff) {
        sysml_conditional_12 = false;
    }
    else {
        if (sysml_state_mode2Index == 1) {
            sysml_conditional_13 = true;
        }
        else {
            if (sysml_state_mode1Index == 1) {
                sysml_conditional_14 = true;
            }
            else {
                sysml_conditional_14 = false;
            }
            sysml_conditional_13 = sysml_conditional_14;
        }
        sysml_conditional_12 = sysml_conditional_13;
    }
    sysml_value_1 = sysml_conditional_12;
    bool sysml_value_2 = 0;
    if (sysml_state_rgbOff) {
        sysml_conditional_15 = false;
    }
    else {
        if (sysml_state_mode2Index == 2) {
            sysml_conditional_16 = true;
        }
        else {
            if (sysml_state_mode1Index == 2) {
                sysml_conditional_17 = true;
            }
            else {
                sysml_conditional_17 = false;
            }
            sysml_conditional_16 = sysml_conditional_17;
        }
        sysml_conditional_15 = sysml_conditional_16;
    }
    sysml_value_2 = sysml_conditional_15;
    *io_3 = sysml_value_0;
    *io_4 = sysml_value_1;
    *io_5 = sysml_value_2;
}