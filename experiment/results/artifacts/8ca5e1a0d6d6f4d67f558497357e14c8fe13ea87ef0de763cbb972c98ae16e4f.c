#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
int sysml_state_displayMode = 0;
/* Global Variable define of  */
int sysml_state_accumulatedMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousPressed = false;
    sysml_state_displayMode = 0;
    sysml_state_accumulatedMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, double io_2, double io_3, bool io_4, int io_6, int* io_5, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, double* serial_0_value_2, bool* serial_1_trigger, bool* serial_1_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_lazy_10 = 0;
    sysml_lazy_0 = io_1;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        if ((sysml_state_displayMode == 2)) {
            sysml_state_displayMode = 0;
        }
        else {
            sysml_state_displayMode = (sysml_state_displayMode + 1);
        }
    }
    sysml_state_accumulatedMs = (sysml_state_accumulatedMs + io_6);
    if ((sysml_state_accumulatedMs >= 2000)) {
        sysml_state_accumulatedMs = (sysml_state_accumulatedMs - 2000);
    }
    sysml_state_previousPressed = io_1;
    int sysml_value_0 = 0;
    if (sysml_state_displayMode == 0) {
        if (io_0 < ((double)((int)io_0))) {
            sysml_conditional_2 = 1;
        }
        else {
            sysml_conditional_2 = 0;
        }
        sysml_conditional_1 = ((int)io_0) - sysml_conditional_2;
    }
    else {
        if (sysml_state_displayMode == 1) {
            if (io_2 < ((double)((int)io_2))) {
                sysml_conditional_4 = 1;
            }
            else {
                sysml_conditional_4 = 0;
            }
            sysml_conditional_3 = ((int)io_2) - sysml_conditional_4;
        }
        else {
            if (io_3 < ((double)((int)io_3))) {
                sysml_conditional_5 = 1;
            }
            else {
                sysml_conditional_5 = 0;
            }
            sysml_conditional_3 = ((int)io_3) - sysml_conditional_5;
        }
        sysml_conditional_1 = sysml_conditional_3;
    }
    sysml_value_0 = sysml_conditional_1;
    bool sysml_value_1 = 0;
    sysml_lazy_6 = sysml_state_accumulatedMs >= 0;
    if (sysml_lazy_6) {
        sysml_lazy_6 = sysml_state_accumulatedMs < 10;
    }
    sysml_value_1 = sysml_lazy_6;
    double sysml_value_2 = 0;
    sysml_value_2 = io_0;
    double sysml_value_3 = 0;
    sysml_value_3 = io_2;
    double sysml_value_4 = 0;
    sysml_value_4 = io_3;
    bool sysml_value_5 = 0;
    sysml_lazy_7 = io_4;
    if (sysml_lazy_7) {
        sysml_lazy_8 = io_0 <= 300.0;
        if (sysml_lazy_8) {
            sysml_lazy_8 = io_2 >= 35.0;
        }
        sysml_lazy_7 = sysml_lazy_8;
    }
    sysml_value_5 = sysml_lazy_7;
    bool sysml_value_6 = 0;
    sysml_lazy_9 = io_4;
    if (sysml_lazy_9) {
        sysml_lazy_10 = io_0 <= 300.0;
        if (sysml_lazy_10) {
            sysml_lazy_10 = io_2 >= 35.0;
        }
        sysml_lazy_9 = sysml_lazy_10;
    }
    sysml_value_6 = sysml_lazy_9;
    *io_5 = sysml_value_0;
    *serial_0_trigger = sysml_value_1;
    *serial_0_value_0 = sysml_value_2;
    *serial_0_value_1 = sysml_value_3;
    *serial_0_value_2 = sysml_value_4;
    *serial_1_trigger = sysml_value_5;
    *serial_1_value_0 = sysml_value_6;
}