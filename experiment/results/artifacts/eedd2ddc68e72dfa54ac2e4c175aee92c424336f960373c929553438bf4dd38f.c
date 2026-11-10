#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_peopleCount = 0;
/* Global Variable define of  */
bool sysml_state_showTempMode = false;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
double sysml_state_tempHold = 0.0;
/* Global Variable define of  */
bool sysml_state_lastEntranceValid = false;
/* Global Variable define of  */
bool sysml_state_lastExitValid = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_peopleCount = 0;
    sysml_state_showTempMode = false;
    sysml_state_previousPressed = false;
    sysml_state_tempHold = 0.0;
    sysml_state_lastEntranceValid = false;
    sysml_state_lastExitValid = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, double io_2, bool io_3, double io_4, double io_5, bool io_6, bool io_9, double* io_7, int* io_8, bool* serial_0_trigger, int* serial_0_value_0, double* serial_0_value_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    double sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    if (io_6) {
        sysml_state_tempHold = io_4;
    }
    sysml_lazy_0 = io_9;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        sysml_state_showTempMode = (!sysml_state_showTempMode);
    }
    sysml_state_previousPressed = io_9;
    sysml_lazy_1 = io_1;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_lastEntranceValid;
    }
    if (sysml_lazy_1) {
        sysml_state_lastEntranceValid = true;
        sysml_state_lastExitValid = false;
    }
    sysml_lazy_2 = io_3;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_lastExitValid;
    }
    if (sysml_lazy_2) {
        if (sysml_state_lastEntranceValid) {
            sysml_state_peopleCount = (sysml_state_peopleCount + 1);
        }
        else {
            sysml_state_peopleCount = (sysml_state_peopleCount - 1);
        }
        sysml_state_lastExitValid = true;
        sysml_state_lastEntranceValid = false;
    }
    if ((!io_1)) {
        sysml_state_lastEntranceValid = false;
    }
    if ((!io_3)) {
        sysml_state_lastExitValid = false;
    }
    if ((sysml_state_peopleCount < 0)) {
        sysml_state_peopleCount = 0;
    }
    double sysml_value_0 = 0;
    if (sysml_state_peopleCount == 0) {
        sysml_conditional_3 = 0.0;
    }
    else {
        sysml_conditional_3 = 100.0;
    }
    sysml_value_0 = sysml_conditional_3;
    int sysml_value_1 = 0;
    if (sysml_state_showTempMode) {
        if (sysml_state_tempHold < ((double)((int)sysml_state_tempHold))) {
            sysml_conditional_5 = 1;
        }
        else {
            sysml_conditional_5 = 0;
        }
        sysml_conditional_4 = ((int)sysml_state_tempHold) - sysml_conditional_5;
    }
    else {
        sysml_conditional_4 = sysml_state_peopleCount;
    }
    sysml_value_1 = sysml_conditional_4;
    bool sysml_value_2 = 0;
    sysml_value_2 = (sysml_state_peopleCount != 0);
    int sysml_value_3 = 0;
    sysml_value_3 = sysml_state_peopleCount;
    double sysml_value_4 = 0;
    sysml_value_4 = sysml_state_tempHold;
    *io_7 = sysml_value_0;
    *io_8 = sysml_value_1;
    *serial_0_trigger = sysml_value_2;
    *serial_0_value_0 = sysml_value_3;
    *serial_0_value_1 = sysml_value_4;
}