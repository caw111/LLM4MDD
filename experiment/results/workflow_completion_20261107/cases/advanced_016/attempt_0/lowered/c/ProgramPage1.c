#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_displayPhaseMs = 0.0;
/* Global Variable define of  */
bool sysml_state_previousVibration = false;
/* Global Variable define of  */
bool sysml_state_faultEntry = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_displayPhaseMs = 0.0;
    sysml_state_previousVibration = false;
    sysml_state_faultEntry = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, bool io_2, int io_7, double* io_3, int* io_4, bool* io_5, int* io_6, bool* serial_0_trigger, int* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_arithmetic_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    int sysml_conditional_6 = 0;
    int sysml_conditional_7 = 0;
    double sysml_conditional_8 = 0;
    int sysml_conditional_9 = 0;
    int sysml_conditional_10 = 0;
    int sysml_conditional_11 = 0;
    int sysml_conditional_12 = 0;
    sysml_lazy_0 = io_2;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousVibration;
    }
    sysml_state_faultEntry = sysml_lazy_0;
    sysml_state_displayPhaseMs = (sysml_state_displayPhaseMs + io_7);
    if ((((double)sysml_state_displayPhaseMs) / 6000.0) < ((double)((int)(((double)sysml_state_displayPhaseMs) / 6000.0)))) {
        sysml_conditional_1 = 1;
    }
    else {
        sysml_conditional_1 = 0;
    }
    sysml_arithmetic_2 = ((int)(((double)sysml_state_displayPhaseMs) / 6000.0)) - sysml_conditional_1;
    sysml_state_displayPhaseMs = sysml_state_displayPhaseMs - (6000.0 * sysml_arithmetic_2);
    sysml_state_previousVibration = io_2;
    double sysml_value_0 = 0;
    sysml_value_0 = ((double)((io_0 * 10.0)) / 1023.0);
    double sysml_value_1 = 0;
    sysml_value_1 = ((double)((io_1 * 500.0)) / 1023.0);
    int sysml_value_2 = 0;
    if (io_2) {
        sysml_conditional_3 = 2000;
    }
    else {
        sysml_conditional_3 = 0;
    }
    sysml_value_2 = sysml_conditional_3;
    bool sysml_value_3 = 0;
    sysml_value_3 = io_2;
    bool sysml_value_4 = 0;
    sysml_value_4 = sysml_state_faultEntry;
    int sysml_value_5 = 0;
    if (io_2) {
        sysml_conditional_4 = 3;
    }
    else {
        sysml_conditional_4 = 0;
    }
    sysml_value_5 = sysml_conditional_4;
    int sysml_value_6 = 0;
    if (io_2) {
        sysml_conditional_5 = 3;
    }
    else {
        if (sysml_value_1 > 60.0) {
            sysml_conditional_6 = 2;
        }
        else {
            if (sysml_value_0 > 7.0) {
                sysml_conditional_7 = 1;
            }
            else {
                sysml_conditional_7 = 0;
            }
            sysml_conditional_6 = sysml_conditional_7;
        }
        sysml_conditional_5 = sysml_conditional_6;
    }
    sysml_value_6 = sysml_conditional_5;
    double sysml_value_7 = 0;
    if (sysml_value_1 > 60.0) {
        sysml_conditional_8 = 255.0;
    }
    else {
        sysml_conditional_8 = 0.0;
    }
    sysml_value_7 = sysml_conditional_8;
    int sysml_value_8 = 0;
    if (sysml_state_displayPhaseMs < 2000.0) {
        if (sysml_value_0 < ((double)((int)sysml_value_0))) {
            sysml_conditional_10 = 1;
        }
        else {
            sysml_conditional_10 = 0;
        }
        sysml_conditional_9 = ((int)sysml_value_0) - sysml_conditional_10;
    }
    else {
        if (sysml_state_displayPhaseMs < 4000.0) {
            if (sysml_value_1 < ((double)((int)sysml_value_1))) {
                sysml_conditional_12 = 1;
            }
            else {
                sysml_conditional_12 = 0;
            }
            sysml_conditional_11 = ((int)sysml_value_1) - sysml_conditional_12;
        }
        else {
            sysml_conditional_11 = sysml_value_6;
        }
        sysml_conditional_9 = sysml_conditional_11;
    }
    sysml_value_8 = sysml_conditional_9;
    *io_3 = sysml_value_7;
    *io_4 = sysml_value_2;
    *io_5 = sysml_value_3;
    *io_6 = sysml_value_8;
    *serial_0_trigger = sysml_value_4;
    *serial_0_value_0 = sysml_value_5;
}