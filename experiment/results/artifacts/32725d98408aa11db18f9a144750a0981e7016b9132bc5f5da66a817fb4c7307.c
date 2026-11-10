#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_stepIndex = 0;
/* Global Variable define of  */
int sysml_state_remainingMs = 2000;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_stepIndex = 0;
    sysml_state_remainingMs = 2000;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_3, double* io_0, double* io_1, double* io_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    double sysml_conditional_1 = 0;
    double sysml_conditional_2 = 0;
    double sysml_conditional_3 = 0;
    double sysml_conditional_4 = 0;
    double sysml_conditional_5 = 0;
    double sysml_conditional_6 = 0;
    double sysml_conditional_7 = 0;
    double sysml_conditional_8 = 0;
    double sysml_conditional_9 = 0;
    double sysml_conditional_10 = 0;
    double sysml_conditional_11 = 0;
    double sysml_conditional_12 = 0;
    double sysml_conditional_13 = 0;
    double sysml_conditional_14 = 0;
    double sysml_conditional_15 = 0;
    sysml_state_remainingMs = (sysml_state_remainingMs - io_3);
    if ((sysml_state_remainingMs <= 0)) {
        if (sysml_state_stepIndex == 5) {
            sysml_conditional_0 = 0;
        }
        else {
            sysml_conditional_0 = sysml_state_stepIndex + 1;
        }
        sysml_state_stepIndex = sysml_conditional_0;
        sysml_state_remainingMs = 2000;
    }
    double sysml_value_0 = 0;
    if (sysml_state_stepIndex == 0) {
        sysml_conditional_1 = 0.0;
    }
    else {
        if (sysml_state_stepIndex == 1) {
            sysml_conditional_2 = 0.0;
        }
        else {
            if (sysml_state_stepIndex == 2) {
                sysml_conditional_3 = 45.0;
            }
            else {
                if (sysml_state_stepIndex == 3) {
                    sysml_conditional_4 = 90.0;
                }
                else {
                    if (sysml_state_stepIndex == 4) {
                        sysml_conditional_5 = 135.0;
                    }
                    else {
                        sysml_conditional_5 = 180.0;
                    }
                    sysml_conditional_4 = sysml_conditional_5;
                }
                sysml_conditional_3 = sysml_conditional_4;
            }
            sysml_conditional_2 = sysml_conditional_3;
        }
        sysml_conditional_1 = sysml_conditional_2;
    }
    sysml_value_0 = sysml_conditional_1;
    double sysml_value_1 = 0;
    if (sysml_state_stepIndex == 0) {
        sysml_conditional_6 = 0.0;
    }
    else {
        if (sysml_state_stepIndex == 1) {
            sysml_conditional_7 = 45.0;
        }
        else {
            if (sysml_state_stepIndex == 2) {
                sysml_conditional_8 = 90.0;
            }
            else {
                if (sysml_state_stepIndex == 3) {
                    sysml_conditional_9 = 135.0;
                }
                else {
                    if (sysml_state_stepIndex == 4) {
                        sysml_conditional_10 = 180.0;
                    }
                    else {
                        sysml_conditional_10 = 0.0;
                    }
                    sysml_conditional_9 = sysml_conditional_10;
                }
                sysml_conditional_8 = sysml_conditional_9;
            }
            sysml_conditional_7 = sysml_conditional_8;
        }
        sysml_conditional_6 = sysml_conditional_7;
    }
    sysml_value_1 = sysml_conditional_6;
    double sysml_value_2 = 0;
    if (sysml_state_stepIndex == 0) {
        sysml_conditional_11 = 0.0;
    }
    else {
        if (sysml_state_stepIndex == 1) {
            sysml_conditional_12 = 90.0;
        }
        else {
            if (sysml_state_stepIndex == 2) {
                sysml_conditional_13 = 135.0;
            }
            else {
                if (sysml_state_stepIndex == 3) {
                    sysml_conditional_14 = 180.0;
                }
                else {
                    if (sysml_state_stepIndex == 4) {
                        sysml_conditional_15 = 0.0;
                    }
                    else {
                        sysml_conditional_15 = 45.0;
                    }
                    sysml_conditional_14 = sysml_conditional_15;
                }
                sysml_conditional_13 = sysml_conditional_14;
            }
            sysml_conditional_12 = sysml_conditional_13;
        }
        sysml_conditional_11 = sysml_conditional_12;
    }
    sysml_value_2 = sysml_conditional_11;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
}