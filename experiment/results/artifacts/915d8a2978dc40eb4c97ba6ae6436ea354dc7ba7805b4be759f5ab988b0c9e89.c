#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_warningLevel = 0;
/* Global Variable define of  */
bool sysml_state_prevFlame = false;
/* Global Variable define of  */
bool sysml_state_toneRunning = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_warningLevel = 0;
    sysml_state_prevFlame = false;
    sysml_state_toneRunning = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, bool io_2, int* io_3, bool* io_4, bool* io_5, int* io_6, bool* serial_0_trigger, int* serial_0_value_0, double* serial_0_value_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_conditional_0 = 0;
    bool sysml_lazy_1 = 0;
    double sysml_conditional_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    int sysml_conditional_8 = 0;
    int sysml_conditional_9 = 0;
    bool sysml_conditional_10 = 0;
    bool sysml_conditional_11 = 0;
    double sysml_conditional_12 = 0;
    double sysml_conditional_13 = 0;
    sysml_state_warningLevel = 0;
    if (io_0 >= 0.0) {
        sysml_conditional_0 = io_0;
    }
    else {
        sysml_conditional_0 = 0.0;
    }
    sysml_lazy_1 = ((sysml_conditional_0 * 1.0) + 0.0) >= 60.0;
    if (!sysml_lazy_1) {
        if (io_1 >= 0.0) {
            sysml_conditional_2 = io_1;
        }
        else {
            sysml_conditional_2 = 0.0;
        }
        sysml_lazy_1 = sysml_conditional_2 >= 550.0;
    }
    if (sysml_lazy_1) {
        sysml_state_warningLevel = 1;
    }
    sysml_lazy_3 = io_0 >= 80.0;
    if (!sysml_lazy_3) {
        sysml_lazy_3 = io_1 >= 750.0;
    }
    sysml_lazy_4 = sysml_lazy_3;
    if (!sysml_lazy_4) {
        sysml_lazy_5 = io_2;
        if (sysml_lazy_5) {
            sysml_lazy_6 = io_0 >= 60.0;
            if (!sysml_lazy_6) {
                sysml_lazy_6 = io_1 >= 550.0;
            }
            sysml_lazy_5 = sysml_lazy_6;
        }
        sysml_lazy_4 = sysml_lazy_5;
    }
    sysml_lazy_7 = sysml_lazy_4;
    if (!sysml_lazy_7) {
        sysml_lazy_7 = io_2;
    }
    if (sysml_lazy_7) {
        sysml_state_warningLevel = 2;
    }
    if (sysml_state_warningLevel == 2) {
        sysml_conditional_8 = 1;
    }
    else {
        sysml_conditional_8 = 0;
    }
    if (sysml_conditional_8 == 1) {
        sysml_state_toneRunning = true;
    }
    else {
        sysml_state_toneRunning = false;
    }
    sysml_state_prevFlame = io_2;
    int sysml_value_0 = 0;
    if (sysml_state_toneRunning) {
        sysml_conditional_9 = 2000;
    }
    else {
        sysml_conditional_9 = 0;
    }
    sysml_value_0 = sysml_conditional_9;
    bool sysml_value_1 = 0;
    if (sysml_state_warningLevel == 2) {
        sysml_conditional_10 = true;
    }
    else {
        sysml_conditional_10 = false;
    }
    sysml_value_1 = sysml_conditional_10;
    bool sysml_value_2 = 0;
    if (sysml_state_warningLevel == 1) {
        sysml_conditional_11 = true;
    }
    else {
        sysml_conditional_11 = false;
    }
    sysml_value_2 = sysml_conditional_11;
    int sysml_value_3 = 0;
    sysml_value_3 = sysml_state_warningLevel;
    bool sysml_value_4 = 0;
    sysml_value_4 = (sysml_state_warningLevel == 2);
    int sysml_value_5 = 0;
    sysml_value_5 = sysml_state_warningLevel;
    double sysml_value_6 = 0;
    if (sysml_state_warningLevel == 2) {
        sysml_conditional_12 = io_0;
    }
    else {
        if (sysml_state_warningLevel == 1) {
            sysml_conditional_13 = io_0;
        }
        else {
            sysml_conditional_13 = 0.0;
        }
        sysml_conditional_12 = sysml_conditional_13;
    }
    sysml_value_6 = sysml_conditional_12;
    *io_3 = sysml_value_0;
    *io_4 = sysml_value_1;
    *io_5 = sysml_value_2;
    *io_6 = sysml_value_3;
    *serial_0_trigger = sysml_value_4;
    *serial_0_value_0 = sysml_value_5;
    *serial_0_value_1 = sysml_value_6;
}