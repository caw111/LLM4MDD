#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_brightnessLevel = 0;
/* Global Variable define of  */
int sysml_state_displayNumber = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_brightnessLevel = 0;
    sysml_state_displayNumber = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, int* io_1, bool* io_2, bool* serial_0_trigger, int* serial_0_value_0, int* serial_0_value_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    bool sysml_conditional_5 = 0;
    bool sysml_conditional_6 = 0;
    sysml_lazy_0 = io_0 >= 0.0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_0 <= 1023.0;
    }
    if (sysml_lazy_0) {
        if (io_0 < ((double)((int)io_0))) {
            sysml_conditional_2 = 1;
        }
        else {
            sysml_conditional_2 = 0;
        }
        sysml_conditional_1 = ((int)io_0) - sysml_conditional_2;
    }
    else {
        if (io_0 < 0.0) {
            sysml_conditional_3 = 0;
        }
        else {
            sysml_conditional_3 = 1023;
        }
        sysml_conditional_1 = sysml_conditional_3;
    }
    sysml_state_brightnessLevel = sysml_conditional_1;
    if (sysml_state_brightnessLevel >= 0) {
        sysml_conditional_4 = sysml_state_brightnessLevel;
    }
    else {
        sysml_conditional_4 = 0;
    }
    sysml_state_displayNumber = sysml_conditional_4;
    int sysml_value_0 = 0;
    sysml_value_0 = sysml_state_displayNumber;
    bool sysml_value_1 = 0;
    if (sysml_state_brightnessLevel > 0) {
        sysml_conditional_5 = true;
    }
    else {
        sysml_conditional_5 = false;
    }
    sysml_value_1 = sysml_conditional_5;
    bool sysml_value_2 = 0;
    if (sysml_state_brightnessLevel != 0) {
        sysml_conditional_6 = true;
    }
    else {
        sysml_conditional_6 = false;
    }
    sysml_value_2 = sysml_conditional_6;
    int sysml_value_3 = 0;
    sysml_value_3 = sysml_state_brightnessLevel;
    int sysml_value_4 = 0;
    sysml_value_4 = sysml_state_displayNumber;
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
    *serial_0_trigger = sysml_value_2;
    *serial_0_value_0 = sysml_value_3;
    *serial_0_value_1 = sysml_value_4;
}