#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_modeOff = false;
/* Global Variable define of  */
bool sysml_state_modeHold = false;
/* Global Variable define of  */
int sysml_state_colorIndex = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_modeOff = false;
    sysml_state_modeHold = false;
    sysml_state_colorIndex = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_3, bool io_4, bool io_5, double* io_0, double* io_1, double* io_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    double sysml_conditional_3 = 0;
    double sysml_conditional_4 = 0;
    double sysml_conditional_5 = 0;
    double sysml_conditional_6 = 0;
    double sysml_conditional_7 = 0;
    double sysml_conditional_8 = 0;
    if (io_5) {
        sysml_state_modeOff = true;
        sysml_state_modeHold = false;
    }
    else {
        if (io_3) {
            sysml_state_modeOff = false;
            sysml_state_modeHold = true;
        }
        else {
            if (io_4) {
                sysml_state_modeOff = false;
                sysml_state_modeHold = false;
            }
        }
    }
    sysml_lazy_0 = !sysml_state_modeOff;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_modeHold;
    }
    if (sysml_lazy_0) {
        if (sysml_state_colorIndex == 0) {
            sysml_conditional_1 = 1;
        }
        else {
            if (sysml_state_colorIndex == 1) {
                sysml_conditional_2 = 2;
            }
            else {
                sysml_conditional_2 = 0;
            }
            sysml_conditional_1 = sysml_conditional_2;
        }
        sysml_state_colorIndex = sysml_conditional_1;
    }
    double sysml_value_0 = 0;
    if (sysml_state_modeOff) {
        sysml_conditional_3 = 0.0;
    }
    else {
        if (sysml_state_colorIndex == 0) {
            sysml_conditional_4 = 255.0;
        }
        else {
            sysml_conditional_4 = 0.0;
        }
        sysml_conditional_3 = sysml_conditional_4;
    }
    sysml_value_0 = sysml_conditional_3;
    double sysml_value_1 = 0;
    if (sysml_state_modeOff) {
        sysml_conditional_5 = 0.0;
    }
    else {
        if (sysml_state_colorIndex == 1) {
            sysml_conditional_6 = 255.0;
        }
        else {
            sysml_conditional_6 = 0.0;
        }
        sysml_conditional_5 = sysml_conditional_6;
    }
    sysml_value_1 = sysml_conditional_5;
    double sysml_value_2 = 0;
    if (sysml_state_modeOff) {
        sysml_conditional_7 = 0.0;
    }
    else {
        if (sysml_state_colorIndex == 2) {
            sysml_conditional_8 = 255.0;
        }
        else {
            sysml_conditional_8 = 0.0;
        }
        sysml_conditional_7 = sysml_conditional_8;
    }
    sysml_value_2 = sysml_conditional_7;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
}