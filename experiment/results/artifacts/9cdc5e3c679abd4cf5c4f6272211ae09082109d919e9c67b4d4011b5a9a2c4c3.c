#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_maxTempDegC = 0.0;
/* Global Variable define of  */
bool sysml_state_showMaxActive = false;
/* Global Variable define of  */
int sysml_state_showMaxRemainingMs = 0;
/* Global Variable define of  */
bool sysml_state_previousButtonPressed = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_maxTempDegC = 0.0;
    sysml_state_showMaxActive = false;
    sysml_state_showMaxRemainingMs = 0;
    sysml_state_previousButtonPressed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, int* io_2, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    double sysml_conditional_5 = 0;
    sysml_lazy_0 = io_1;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousButtonPressed;
    }
    if (sysml_lazy_0) {
        sysml_state_showMaxActive = true;
        sysml_state_showMaxRemainingMs = 3000;
    }
    if (sysml_state_showMaxActive) {
        if (sysml_state_showMaxRemainingMs <= 0) {
            sysml_conditional_1 = 0;
        }
        else {
            sysml_conditional_1 = sysml_state_showMaxRemainingMs;
        }
        sysml_state_showMaxRemainingMs = sysml_conditional_1;
        if ((sysml_state_showMaxRemainingMs <= 0)) {
            sysml_state_showMaxActive = false;
        }
    }
    sysml_state_previousButtonPressed = io_1;
    if ((io_0 > sysml_state_maxTempDegC)) {
        sysml_state_maxTempDegC = io_0;
    }
    int sysml_value_0 = 0;
    if (sysml_state_showMaxActive) {
        if (sysml_state_maxTempDegC < ((double)((int)sysml_state_maxTempDegC))) {
            sysml_conditional_3 = 1;
        }
        else {
            sysml_conditional_3 = 0;
        }
        sysml_conditional_2 = ((int)sysml_state_maxTempDegC) - sysml_conditional_3;
    }
    else {
        if (io_0 < ((double)((int)io_0))) {
            sysml_conditional_4 = 1;
        }
        else {
            sysml_conditional_4 = 0;
        }
        sysml_conditional_2 = ((int)io_0) - sysml_conditional_4;
    }
    sysml_value_0 = sysml_conditional_2;
    bool sysml_value_1 = 0;
    sysml_value_1 = true;
    double sysml_value_2 = 0;
    if (sysml_state_showMaxActive) {
        sysml_conditional_5 = io_0;
    }
    else {
        sysml_conditional_5 = io_0;
    }
    sysml_value_2 = sysml_conditional_5;
    double sysml_value_3 = 0;
    sysml_value_3 = sysml_state_maxTempDegC;
    *io_2 = sysml_value_0;
    *serial_0_trigger = sysml_value_1;
    *serial_0_value_0 = sysml_value_2;
    *serial_0_value_1 = sysml_value_3;
}