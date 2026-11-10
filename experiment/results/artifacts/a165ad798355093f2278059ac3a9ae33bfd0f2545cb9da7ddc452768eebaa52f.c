#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_modeAuto = true;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
double sysml_state_manualThresholdCm = 15.0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_modeAuto = true;
    sysml_state_previousPressed = false;
    sysml_state_manualThresholdCm = 15.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, double io_3, bool io_4, int* io_2, int* io_5) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    double sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    int sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    int sysml_conditional_8 = 0;
    int sysml_conditional_9 = 0;
    int sysml_conditional_10 = 0;
    int sysml_conditional_11 = 0;
    sysml_lazy_0 = io_1;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        sysml_state_modeAuto = (!sysml_state_modeAuto);
        if (io_0 <= 15.0) {
            sysml_conditional_1 = 15.0;
        }
        else {
            if ((((double)(io_0 * 400.0)) / 1023.0) < ((double)((int)(((double)(io_0 * 400.0)) / 1023.0)))) {
                sysml_conditional_2 = 1;
            }
            else {
                sysml_conditional_2 = 0;
            }
            sysml_conditional_1 = ((int)(((double)(io_0 * 400.0)) / 1023.0)) - sysml_conditional_2;
        }
        sysml_state_manualThresholdCm = sysml_conditional_1;
    }
    sysml_state_previousPressed = io_1;
    int sysml_value_0 = 0;
    sysml_lazy_3 = sysml_state_modeAuto;
    if (sysml_lazy_3) {
        sysml_lazy_3 = io_4;
    }
    sysml_lazy_4 = sysml_lazy_3;
    if (sysml_lazy_4) {
        sysml_lazy_4 = io_3 < 15.0;
    }
    if (sysml_lazy_4) {
        sysml_conditional_5 = 1000;
    }
    else {
        sysml_lazy_6 = !sysml_state_modeAuto;
        if (sysml_lazy_6) {
            sysml_lazy_6 = io_4;
        }
        sysml_lazy_7 = sysml_lazy_6;
        if (sysml_lazy_7) {
            sysml_lazy_7 = io_3 < sysml_state_manualThresholdCm;
        }
        if (sysml_lazy_7) {
            sysml_conditional_8 = 1000;
        }
        else {
            sysml_conditional_8 = 0;
        }
        sysml_conditional_5 = sysml_conditional_8;
    }
    sysml_value_0 = sysml_conditional_5;
    int sysml_value_1 = 0;
    if (sysml_state_modeAuto) {
        sysml_conditional_9 = 15;
    }
    else {
        if (sysml_state_manualThresholdCm <= 0.0) {
            sysml_conditional_10 = 0;
        }
        else {
            if (sysml_state_manualThresholdCm < ((double)((int)sysml_state_manualThresholdCm))) {
                sysml_conditional_11 = 1;
            }
            else {
                sysml_conditional_11 = 0;
            }
            sysml_conditional_10 = ((int)sysml_state_manualThresholdCm) - sysml_conditional_11;
        }
        sysml_conditional_9 = sysml_conditional_10;
    }
    sysml_value_1 = sysml_conditional_9;
    *io_2 = sysml_value_0;
    *io_5 = sysml_value_1;
}