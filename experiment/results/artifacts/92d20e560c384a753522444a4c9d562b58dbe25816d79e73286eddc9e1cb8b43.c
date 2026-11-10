#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_maxAdc = 0.0;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
bool sysml_state_showMax = false;
/* Global Variable define of  */
int sysml_state_displayMaxRemaining = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_maxAdc = 0.0;
    sysml_state_previousPressed = false;
    sysml_state_showMax = false;
    sysml_state_displayMaxRemaining = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, int* io_2, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_conditional_0 = 0;
    bool sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    int sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    int sysml_conditional_8 = 0;
    int sysml_conditional_9 = 0;
    int sysml_conditional_10 = 0;
    double sysml_conditional_11 = 0;
    double sysml_conditional_12 = 0;
    if ((io_0 > sysml_state_maxAdc)) {
        sysml_state_maxAdc = io_0;
    }
    if (sysml_state_showMax) {
        if (sysml_state_displayMaxRemaining <= 1) {
            sysml_conditional_1 = false;
        }
        else {
            sysml_conditional_1 = true;
        }
        sysml_conditional_0 = sysml_conditional_1;
    }
    else {
        sysml_conditional_0 = sysml_state_showMax;
    }
    sysml_state_showMax = sysml_conditional_0;
    if (sysml_state_showMax) {
        if (sysml_state_displayMaxRemaining <= 1) {
            sysml_conditional_3 = 0;
        }
        else {
            sysml_conditional_3 = sysml_state_displayMaxRemaining - 1;
        }
        sysml_conditional_2 = sysml_conditional_3;
    }
    else {
        sysml_lazy_4 = io_1;
        if (sysml_lazy_4) {
            sysml_lazy_4 = !sysml_state_previousPressed;
        }
        if (sysml_lazy_4) {
            sysml_conditional_5 = 3000;
        }
        else {
            sysml_conditional_5 = sysml_state_displayMaxRemaining;
        }
        sysml_conditional_2 = sysml_conditional_5;
    }
    sysml_state_displayMaxRemaining = sysml_conditional_2;
    sysml_lazy_6 = !sysml_state_showMax;
    if (sysml_lazy_6) {
        sysml_lazy_7 = sysml_state_displayMaxRemaining >= 1;
        if (sysml_lazy_7) {
            sysml_lazy_7 = !io_1;
        }
        sysml_lazy_6 = sysml_lazy_7;
    }
    if (sysml_lazy_6) {
        sysml_state_displayMaxRemaining = 0;
    }
    sysml_state_previousPressed = io_1;
    int sysml_value_0 = 0;
    if (sysml_state_showMax) {
        if (sysml_state_maxAdc < ((double)((int)sysml_state_maxAdc))) {
            sysml_conditional_9 = 1;
        }
        else {
            sysml_conditional_9 = 0;
        }
        sysml_conditional_8 = ((int)sysml_state_maxAdc) - sysml_conditional_9;
    }
    else {
        if (io_0 < ((double)((int)io_0))) {
            sysml_conditional_10 = 1;
        }
        else {
            sysml_conditional_10 = 0;
        }
        sysml_conditional_8 = ((int)io_0) - sysml_conditional_10;
    }
    sysml_value_0 = sysml_conditional_8;
    bool sysml_value_1 = 0;
    sysml_value_1 = true;
    double sysml_value_2 = 0;
    if (true) {
        sysml_conditional_11 = io_0;
    }
    else {
        sysml_conditional_11 = 0.0;
    }
    sysml_value_2 = sysml_conditional_11;
    double sysml_value_3 = 0;
    if (true) {
        sysml_conditional_12 = sysml_state_maxAdc;
    }
    else {
        sysml_conditional_12 = 0.0;
    }
    sysml_value_3 = sysml_conditional_12;
    *io_2 = sysml_value_0;
    *serial_0_trigger = sysml_value_1;
    *serial_0_value_0 = sysml_value_2;
    *serial_0_value_1 = sysml_value_3;
}