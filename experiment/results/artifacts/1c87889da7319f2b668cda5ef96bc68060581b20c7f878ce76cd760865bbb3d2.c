#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_mode = 0;
/* Global Variable define of  */
int sysml_state_param = 128;
/* Global Variable define of  */
double sysml_state_brightnessGate = 0.0;
/* Global Variable define of  */
int sysml_state_accumulatedMs = 0;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_mode = 0;
    sysml_state_param = 128;
    sysml_state_brightnessGate = 0.0;
    sysml_state_accumulatedMs = 0;
    sysml_state_previousPressed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, int io_5, double* io_2, bool* io_3, int* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    double sysml_conditional_5 = 0;
    double sysml_conditional_6 = 0;
    double sysml_conditional_7 = 0;
    double sysml_arithmetic_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_lazy_10 = 0;
    int sysml_conditional_11 = 0;
    int sysml_conditional_12 = 0;
    sysml_state_previousPressed = io_0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        if (sysml_state_mode == 2) {
            sysml_conditional_1 = 0;
        }
        else {
            sysml_conditional_1 = sysml_state_mode + 1;
        }
        sysml_state_mode = sysml_conditional_1;
    }
    sysml_lazy_2 = io_1;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !(sysml_state_mode == 2);
    }
    if (sysml_lazy_2) {
        if (sysml_state_param == 255) {
            sysml_conditional_3 = 0;
        }
        else {
            sysml_conditional_3 = sysml_state_param + 16;
        }
        sysml_state_param = sysml_conditional_3;
    }
    else {
        if (sysml_state_param == 5000) {
            sysml_conditional_4 = 500;
        }
        else {
            sysml_conditional_4 = sysml_state_param + 500;
        }
        sysml_state_param = sysml_conditional_4;
    }
    if ((sysml_state_mode == 0)) {
        sysml_state_brightnessGate = 0.0;
    }
    if ((sysml_state_mode == 1)) {
        if (sysml_state_param >= 255) {
            sysml_conditional_5 = 255.0;
        }
        else {
            sysml_conditional_5 = sysml_state_param * 1.0;
        }
        sysml_state_brightnessGate = sysml_conditional_5;
    }
    if ((sysml_state_mode == 2)) {
        sysml_state_accumulatedMs = (sysml_state_accumulatedMs + io_5);
        if (sysml_state_accumulatedMs < (sysml_state_param * 1)) {
            if (sysml_state_accumulatedMs >= (((double)sysml_state_param) / 2)) {
                sysml_conditional_7 = ((double)(sysml_state_param - (((double)(sysml_state_accumulatedMs - (((double)sysml_state_param) / 2))) / 1))) / 1;
            }
            else {
                sysml_arithmetic_8 = ((double)sysml_state_param) / 255;
                sysml_conditional_7 = ((double)sysml_state_accumulatedMs) / sysml_arithmetic_8;
            }
            sysml_conditional_6 = sysml_conditional_7;
        }
        else {
            sysml_conditional_6 = 0.0;
        }
        sysml_state_brightnessGate = sysml_conditional_6;
    }
    sysml_lazy_9 = sysml_state_mode == 2;
    if (sysml_lazy_9) {
        sysml_lazy_9 = sysml_state_accumulatedMs >= (sysml_state_param * 2);
    }
    if (sysml_lazy_9) {
        sysml_state_accumulatedMs = 0;
    }
    double sysml_value_0 = 0;
    sysml_value_0 = sysml_state_brightnessGate;
    bool sysml_value_1 = 0;
    sysml_value_1 = (sysml_state_mode != 0);
    int sysml_value_2 = 0;
    sysml_lazy_10 = sysml_state_mode == 0;
    if (!sysml_lazy_10) {
        sysml_lazy_10 = sysml_state_mode == 1;
    }
    if (sysml_lazy_10) {
        sysml_conditional_11 = sysml_state_mode;
    }
    else {
        if (sysml_state_param < ((double)((int)sysml_state_param))) {
            sysml_conditional_12 = 1;
        }
        else {
            sysml_conditional_12 = 0;
        }
        sysml_conditional_11 = ((int)sysml_state_param) - sysml_conditional_12;
    }
    sysml_value_2 = sysml_conditional_11;
    *io_2 = sysml_value_0;
    *io_3 = sysml_value_1;
    *io_4 = sysml_value_2;
}