#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_modeCycling = true;
/* Global Variable define of  */
bool sysml_state_modeOff = false;
/* Global Variable define of  */
int sysml_state_currentColorIndex = 0;
/* Global Variable define of  */
bool sysml_state_previousK1 = false;
/* Global Variable define of  */
int sysml_state_accumulatedMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_modeCycling = true;
    sysml_state_modeOff = false;
    sysml_state_currentColorIndex = 0;
    sysml_state_previousK1 = false;
    sysml_state_accumulatedMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_3, bool io_4, bool io_5, int io_6, double* io_0, double* io_1, double* io_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    int sysml_conditional_4 = 0;
    double sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    double sysml_conditional_7 = 0;
    bool sysml_lazy_8 = 0;
    double sysml_conditional_9 = 0;
    double sysml_conditional_10 = 0;
    bool sysml_lazy_11 = 0;
    double sysml_conditional_12 = 0;
    bool sysml_lazy_13 = 0;
    double sysml_conditional_14 = 0;
    double sysml_conditional_15 = 0;
    bool sysml_lazy_16 = 0;
    double sysml_conditional_17 = 0;
    bool sysml_lazy_18 = 0;
    double sysml_conditional_19 = 0;
    if (io_5) {
        sysml_state_modeOff = true;
        sysml_state_modeCycling = false;
    }
    sysml_lazy_0 = !io_5;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_4;
    }
    sysml_lazy_1 = sysml_lazy_0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_modeOff;
    }
    if (sysml_lazy_1) {
        sysml_state_modeCycling = true;
        sysml_state_modeOff = false;
    }
    sysml_lazy_2 = io_3;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_previousK1;
    }
    if (sysml_lazy_2) {
        sysml_state_modeCycling = false;
    }
    sysml_lazy_3 = sysml_state_modeCycling;
    if (sysml_lazy_3) {
        sysml_lazy_3 = !sysml_state_modeOff;
    }
    if (sysml_lazy_3) {
        sysml_state_accumulatedMs = (sysml_state_accumulatedMs + io_6);
        if ((sysml_state_accumulatedMs >= 2000)) {
            sysml_state_accumulatedMs = (sysml_state_accumulatedMs - 2000);
            if (sysml_state_currentColorIndex == 2) {
                sysml_conditional_4 = 0;
            }
            else {
                sysml_conditional_4 = sysml_state_currentColorIndex + 1;
            }
            sysml_state_currentColorIndex = sysml_conditional_4;
        }
    }
    sysml_state_previousK1 = io_3;
    double sysml_value_0 = 0;
    if (sysml_state_modeOff) {
        sysml_conditional_5 = 0.0;
    }
    else {
        sysml_lazy_6 = sysml_state_modeCycling;
        if (sysml_lazy_6) {
            sysml_lazy_6 = sysml_state_currentColorIndex == 0;
        }
        if (sysml_lazy_6) {
            sysml_conditional_7 = 255.0;
        }
        else {
            sysml_lazy_8 = !sysml_state_modeCycling;
            if (sysml_lazy_8) {
                sysml_lazy_8 = sysml_state_currentColorIndex == 0;
            }
            if (sysml_lazy_8) {
                sysml_conditional_9 = 255.0;
            }
            else {
                sysml_conditional_9 = 0.0;
            }
            sysml_conditional_7 = sysml_conditional_9;
        }
        sysml_conditional_5 = sysml_conditional_7;
    }
    sysml_value_0 = sysml_conditional_5;
    double sysml_value_1 = 0;
    if (sysml_state_modeOff) {
        sysml_conditional_10 = 0.0;
    }
    else {
        sysml_lazy_11 = sysml_state_modeCycling;
        if (sysml_lazy_11) {
            sysml_lazy_11 = sysml_state_currentColorIndex == 1;
        }
        if (sysml_lazy_11) {
            sysml_conditional_12 = 255.0;
        }
        else {
            sysml_lazy_13 = !sysml_state_modeCycling;
            if (sysml_lazy_13) {
                sysml_lazy_13 = sysml_state_currentColorIndex == 1;
            }
            if (sysml_lazy_13) {
                sysml_conditional_14 = 255.0;
            }
            else {
                sysml_conditional_14 = 0.0;
            }
            sysml_conditional_12 = sysml_conditional_14;
        }
        sysml_conditional_10 = sysml_conditional_12;
    }
    sysml_value_1 = sysml_conditional_10;
    double sysml_value_2 = 0;
    if (sysml_state_modeOff) {
        sysml_conditional_15 = 0.0;
    }
    else {
        sysml_lazy_16 = sysml_state_modeCycling;
        if (sysml_lazy_16) {
            sysml_lazy_16 = sysml_state_currentColorIndex == 2;
        }
        if (sysml_lazy_16) {
            sysml_conditional_17 = 255.0;
        }
        else {
            sysml_lazy_18 = !sysml_state_modeCycling;
            if (sysml_lazy_18) {
                sysml_lazy_18 = sysml_state_currentColorIndex == 2;
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
    sysml_value_2 = sysml_conditional_15;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
}