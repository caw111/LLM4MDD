#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_mode = 0;
/* Global Variable define of  */
int sysml_state_stepIndex = 0;
/* Global Variable define of  */
int sysml_state_phaseMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_mode = 0;
    sysml_state_stepIndex = 0;
    sysml_state_phaseMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool* io_0, bool* io_1, bool* io_2, double* io_3, double* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    int sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    int sysml_conditional_5 = 0;
    int sysml_conditional_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_conditional_8 = 0;
    bool sysml_conditional_9 = 0;
    bool sysml_conditional_10 = 0;
    double sysml_conditional_11 = 0;
    double sysml_conditional_12 = 0;
    double sysml_conditional_13 = 0;
    double sysml_conditional_14 = 0;
    double sysml_conditional_15 = 0;
    sysml_lazy_0 = sysml_state_mode == 0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = sysml_state_phaseMs >= 2000;
    }
    if (sysml_lazy_0) {
        sysml_state_mode = 1;
        sysml_state_phaseMs = 0;
        sysml_state_stepIndex = 0;
    }
    sysml_lazy_1 = sysml_state_mode == 1;
    if (sysml_lazy_1) {
        sysml_lazy_1 = sysml_state_phaseMs >= 2000;
    }
    if (sysml_lazy_1) {
        sysml_state_phaseMs = 0;
        if (sysml_state_stepIndex < 5) {
            sysml_conditional_2 = sysml_state_stepIndex + 1;
        }
        else {
            sysml_conditional_2 = 5;
        }
        sysml_state_stepIndex = sysml_conditional_2;
        if (sysml_state_stepIndex == 5) {
            sysml_conditional_3 = 2;
        }
        else {
            sysml_conditional_3 = 1;
        }
        sysml_state_mode = sysml_conditional_3;
    }
    sysml_lazy_4 = sysml_state_mode == 2;
    if (sysml_lazy_4) {
        sysml_lazy_4 = sysml_state_phaseMs >= 2000;
    }
    if (sysml_lazy_4) {
        sysml_state_phaseMs = 0;
        if (sysml_state_stepIndex < 5) {
            sysml_conditional_5 = sysml_state_stepIndex + 1;
        }
        else {
            sysml_conditional_5 = 5;
        }
        sysml_state_stepIndex = sysml_conditional_5;
        if (sysml_state_stepIndex == 5) {
            sysml_conditional_6 = 3;
        }
        else {
            sysml_conditional_6 = 2;
        }
        sysml_state_mode = sysml_conditional_6;
    }
    sysml_lazy_7 = sysml_state_mode == 3;
    if (sysml_lazy_7) {
        sysml_lazy_7 = sysml_state_phaseMs >= 2000;
    }
    if (sysml_lazy_7) {
        sysml_state_mode = 1;
        sysml_state_phaseMs = 0;
        sysml_state_stepIndex = 0;
    }
    bool sysml_value_0 = 0;
    if (sysml_state_mode == 1) {
        sysml_conditional_8 = true;
    }
    else {
        sysml_conditional_8 = false;
    }
    sysml_value_0 = sysml_conditional_8;
    bool sysml_value_1 = 0;
    if (sysml_state_mode == 2) {
        sysml_conditional_9 = true;
    }
    else {
        sysml_conditional_9 = false;
    }
    sysml_value_1 = sysml_conditional_9;
    bool sysml_value_2 = 0;
    if (sysml_state_mode == 3) {
        sysml_conditional_10 = true;
    }
    else {
        sysml_conditional_10 = false;
    }
    sysml_value_2 = sysml_conditional_10;
    double sysml_value_3 = 0;
    if (sysml_state_mode == 0) {
        sysml_conditional_11 = 0.0;
    }
    else {
        if (sysml_state_mode == 1) {
            sysml_conditional_12 = ((sysml_state_stepIndex + 0) * 30) + 0;
        }
        else {
            sysml_conditional_12 = 180.0;
        }
        sysml_conditional_11 = sysml_conditional_12;
    }
    sysml_value_3 = sysml_conditional_11;
    double sysml_value_4 = 0;
    if (sysml_state_mode == 0) {
        sysml_conditional_13 = 180.0;
    }
    else {
        if (sysml_state_mode == 2) {
            sysml_conditional_14 = 180.0 - ((sysml_state_stepIndex + 0) * 30);
        }
        else {
            if (sysml_state_mode == 1) {
                sysml_conditional_15 = 180.0;
            }
            else {
                sysml_conditional_15 = 0.0;
            }
            sysml_conditional_14 = sysml_conditional_15;
        }
        sysml_conditional_13 = sysml_conditional_14;
    }
    sysml_value_4 = sysml_conditional_13;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
    *io_4 = sysml_value_4;
}