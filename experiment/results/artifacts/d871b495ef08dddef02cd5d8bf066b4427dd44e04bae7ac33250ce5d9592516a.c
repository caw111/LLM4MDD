#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
int sysml_state_currentState = 1;
/* Global Variable define of  */
int sysml_state_stateEnteredAtMs = 0;
/* Global Variable define of  */
bool sysml_state_pendingAdvance = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousPressed = false;
    sysml_state_currentState = 1;
    sysml_state_stateEnteredAtMs = 0;
    sysml_state_pendingAdvance = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, double* io_1, double* io_2, double* io_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    int sysml_conditional_3 = 0;
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
    sysml_lazy_0 = sysml_state_pendingAdvance;
    if (!sysml_lazy_0) {
        sysml_lazy_1 = io_0;
        if (sysml_lazy_1) {
            sysml_lazy_1 = !sysml_state_previousPressed;
        }
        sysml_lazy_0 = sysml_lazy_1;
    }
    sysml_state_pendingAdvance = sysml_lazy_0;
    sysml_state_previousPressed = io_0;
    sysml_lazy_2 = sysml_state_pendingAdvance;
    if (sysml_lazy_2) {
        sysml_lazy_2 = sysml_state_stateEnteredAtMs >= 2000;
    }
    if (sysml_lazy_2) {
        sysml_state_pendingAdvance = false;
        if (sysml_state_currentState == 5) {
            sysml_conditional_3 = 1;
        }
        else {
            sysml_conditional_3 = sysml_state_currentState + 1;
        }
        sysml_state_currentState = sysml_conditional_3;
        sysml_state_stateEnteredAtMs = 0;
    }
    if ((sysml_state_stateEnteredAtMs > 2000)) {
        sysml_state_stateEnteredAtMs = sysml_state_stateEnteredAtMs;
    }
    double sysml_value_0 = 0;
    if (sysml_state_currentState == 1) {
        sysml_conditional_4 = 0.0;
    }
    else {
        if (sysml_state_currentState == 2) {
            sysml_conditional_5 = 90.0;
        }
        else {
            if (sysml_state_currentState == 3) {
                sysml_conditional_6 = 180.0;
            }
            else {
                if (sysml_state_currentState == 4) {
                    sysml_conditional_7 = 45.0;
                }
                else {
                    sysml_conditional_7 = 0.0;
                }
                sysml_conditional_6 = sysml_conditional_7;
            }
            sysml_conditional_5 = sysml_conditional_6;
        }
        sysml_conditional_4 = sysml_conditional_5;
    }
    sysml_value_0 = sysml_conditional_4;
    double sysml_value_1 = 0;
    if (sysml_state_currentState == 1) {
        sysml_conditional_8 = 0.0;
    }
    else {
        if (sysml_state_currentState == 2) {
            sysml_conditional_9 = 45.0;
        }
        else {
            if (sysml_state_currentState == 3) {
                sysml_conditional_10 = 90.0;
            }
            else {
                if (sysml_state_currentState == 4) {
                    sysml_conditional_11 = 135.0;
                }
                else {
                    sysml_conditional_11 = 0.0;
                }
                sysml_conditional_10 = sysml_conditional_11;
            }
            sysml_conditional_9 = sysml_conditional_10;
        }
        sysml_conditional_8 = sysml_conditional_9;
    }
    sysml_value_1 = sysml_conditional_8;
    double sysml_value_2 = 0;
    if (sysml_state_currentState == 1) {
        sysml_conditional_12 = 0.0;
    }
    else {
        if (sysml_state_currentState == 2) {
            sysml_conditional_13 = 135.0;
        }
        else {
            if (sysml_state_currentState == 3) {
                sysml_conditional_14 = 0.0;
            }
            else {
                if (sysml_state_currentState == 4) {
                    sysml_conditional_15 = 90.0;
                }
                else {
                    sysml_conditional_15 = 0.0;
                }
                sysml_conditional_14 = sysml_conditional_15;
            }
            sysml_conditional_13 = sysml_conditional_14;
        }
        sysml_conditional_12 = sysml_conditional_13;
    }
    sysml_value_2 = sysml_conditional_12;
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
    *io_3 = sysml_value_2;
}