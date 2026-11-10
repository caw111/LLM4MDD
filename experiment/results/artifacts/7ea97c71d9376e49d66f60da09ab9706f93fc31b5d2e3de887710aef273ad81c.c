#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_servoStep = 1;
/* Global Variable define of  */
bool sysml_state_prevPressed = false;
/* Global Variable define of  */
int sysml_state_stateHoldMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_servoStep = 1;
    sysml_state_prevPressed = false;
    sysml_state_stateHoldMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, double* io_1, double* io_2, double* io_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
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
    double sysml_conditional_16 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_prevPressed;
    }
    if (sysml_lazy_0) {
        if (sysml_state_servoStep == 1) {
            sysml_conditional_1 = 2;
        }
        else {
            if (sysml_state_servoStep == 2) {
                sysml_conditional_2 = 3;
            }
            else {
                if (sysml_state_servoStep == 3) {
                    sysml_conditional_3 = 4;
                }
                else {
                    if (sysml_state_servoStep == 4) {
                        sysml_conditional_4 = 5;
                    }
                    else {
                        sysml_conditional_4 = 1;
                    }
                    sysml_conditional_3 = sysml_conditional_4;
                }
                sysml_conditional_2 = sysml_conditional_3;
            }
            sysml_conditional_1 = sysml_conditional_2;
        }
        sysml_state_servoStep = sysml_conditional_1;
        sysml_state_stateHoldMs = 0;
    }
    sysml_state_prevPressed = io_0;
    double sysml_value_0 = 0;
    if (sysml_state_servoStep == 1) {
        sysml_conditional_5 = 0.0;
    }
    else {
        if (sysml_state_servoStep == 2) {
            sysml_conditional_6 = 90.0;
        }
        else {
            if (sysml_state_servoStep == 3) {
                sysml_conditional_7 = 180.0;
            }
            else {
                if (sysml_state_servoStep == 4) {
                    sysml_conditional_8 = 45.0;
                }
                else {
                    sysml_conditional_8 = 0.0;
                }
                sysml_conditional_7 = sysml_conditional_8;
            }
            sysml_conditional_6 = sysml_conditional_7;
        }
        sysml_conditional_5 = sysml_conditional_6;
    }
    sysml_value_0 = sysml_conditional_5;
    double sysml_value_1 = 0;
    if (sysml_state_servoStep == 1) {
        sysml_conditional_9 = 0.0;
    }
    else {
        if (sysml_state_servoStep == 2) {
            sysml_conditional_10 = 45.0;
        }
        else {
            if (sysml_state_servoStep == 3) {
                sysml_conditional_11 = 90.0;
            }
            else {
                if (sysml_state_servoStep == 4) {
                    sysml_conditional_12 = 135.0;
                }
                else {
                    sysml_conditional_12 = 0.0;
                }
                sysml_conditional_11 = sysml_conditional_12;
            }
            sysml_conditional_10 = sysml_conditional_11;
        }
        sysml_conditional_9 = sysml_conditional_10;
    }
    sysml_value_1 = sysml_conditional_9;
    double sysml_value_2 = 0;
    if (sysml_state_servoStep == 1) {
        sysml_conditional_13 = 0.0;
    }
    else {
        if (sysml_state_servoStep == 2) {
            sysml_conditional_14 = 135.0;
        }
        else {
            if (sysml_state_servoStep == 3) {
                sysml_conditional_15 = 0.0;
            }
            else {
                if (sysml_state_servoStep == 4) {
                    sysml_conditional_16 = 90.0;
                }
                else {
                    sysml_conditional_16 = 0.0;
                }
                sysml_conditional_15 = sysml_conditional_16;
            }
            sysml_conditional_14 = sysml_conditional_15;
        }
        sysml_conditional_13 = sysml_conditional_14;
    }
    sysml_value_2 = sysml_conditional_13;
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
    *io_3 = sysml_value_2;
}