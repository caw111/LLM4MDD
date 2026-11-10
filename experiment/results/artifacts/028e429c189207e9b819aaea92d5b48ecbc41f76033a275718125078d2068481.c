#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_displayMode = 0;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
bool sysml_state_alarmState = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_displayMode = 0;
    sysml_state_previousPressed = false;
    sysml_state_alarmState = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, double io_2, bool io_7, double* io_3, double* io_4, bool* io_5, int* io_6, int* io_8) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    double sysml_conditional_1 = 0;
    double sysml_conditional_2 = 0;
    double sysml_conditional_3 = 0;
    double sysml_conditional_4 = 0;
    bool sysml_conditional_5 = 0;
    int sysml_conditional_6 = 0;
    int sysml_conditional_7 = 0;
    int sysml_conditional_8 = 0;
    int sysml_conditional_9 = 0;
    int sysml_conditional_10 = 0;
    int sysml_conditional_11 = 0;
    sysml_lazy_0 = io_7;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        sysml_state_displayMode = (sysml_state_displayMode + 1);
    }
    if ((sysml_state_displayMode == 3)) {
        sysml_state_displayMode = 0;
    }
    sysml_state_previousPressed = io_7;
    if ((io_2 < 300.0)) {
        sysml_state_alarmState = true;
    }
    else {
        sysml_state_alarmState = false;
    }
    double sysml_value_0 = 0;
    if (sysml_state_alarmState) {
        sysml_conditional_1 = 0.0;
    }
    else {
        if (io_0 < 250.0) {
            sysml_conditional_2 = 255.0;
        }
        else {
            sysml_conditional_2 = 0.0;
        }
        sysml_conditional_1 = sysml_conditional_2;
    }
    sysml_value_0 = sysml_conditional_1;
    double sysml_value_1 = 0;
    if (sysml_state_alarmState) {
        sysml_conditional_3 = 0.0;
    }
    else {
        if (io_1 < 300.0) {
            sysml_conditional_4 = 200.0;
        }
        else {
            sysml_conditional_4 = 0.0;
        }
        sysml_conditional_3 = sysml_conditional_4;
    }
    sysml_value_1 = sysml_conditional_3;
    bool sysml_value_2 = 0;
    if (sysml_state_alarmState) {
        sysml_conditional_5 = true;
    }
    else {
        sysml_conditional_5 = false;
    }
    sysml_value_2 = sysml_conditional_5;
    int sysml_value_3 = 0;
    if (sysml_state_alarmState) {
        sysml_conditional_6 = 1000;
    }
    else {
        sysml_conditional_6 = 0;
    }
    sysml_value_3 = sysml_conditional_6;
    int sysml_value_4 = 0;
    if (sysml_state_displayMode == 0) {
        if (io_0 < ((double)((int)io_0))) {
            sysml_conditional_8 = 1;
        }
        else {
            sysml_conditional_8 = 0;
        }
        sysml_conditional_7 = ((int)io_0) - sysml_conditional_8;
    }
    else {
        if (sysml_state_displayMode == 1) {
            if (io_1 < ((double)((int)io_1))) {
                sysml_conditional_10 = 1;
            }
            else {
                sysml_conditional_10 = 0;
            }
            sysml_conditional_9 = ((int)io_1) - sysml_conditional_10;
        }
        else {
            if (io_2 < ((double)((int)io_2))) {
                sysml_conditional_11 = 1;
            }
            else {
                sysml_conditional_11 = 0;
            }
            sysml_conditional_9 = ((int)io_2) - sysml_conditional_11;
        }
        sysml_conditional_7 = sysml_conditional_9;
    }
    sysml_value_4 = sysml_conditional_7;
    *io_3 = sysml_value_0;
    *io_4 = sysml_value_1;
    *io_5 = sysml_value_2;
    *io_6 = sysml_value_3;
    *io_8 = sysml_value_4;
}