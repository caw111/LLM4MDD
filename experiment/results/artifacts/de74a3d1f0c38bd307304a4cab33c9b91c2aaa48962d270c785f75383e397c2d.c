#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_colorIndex = 0;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_colorIndex = 0;
    sysml_state_previousPressed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, double* io_1, double* io_2, double* io_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    double sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    double sysml_conditional_9 = 0;
    bool sysml_lazy_10 = 0;
    bool sysml_lazy_11 = 0;
    bool sysml_lazy_12 = 0;
    double sysml_conditional_13 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        if (sysml_state_colorIndex >= 7) {
            sysml_conditional_1 = 0;
        }
        else {
            sysml_conditional_1 = sysml_state_colorIndex + 1;
        }
        sysml_state_colorIndex = sysml_conditional_1;
        sysml_state_previousPressed = true;
    }
    else {
        sysml_state_previousPressed = io_0;
    }
    double sysml_value_0 = 0;
    sysml_lazy_2 = sysml_state_colorIndex == 0;
    if (!sysml_lazy_2) {
        sysml_lazy_2 = sysml_state_colorIndex == 3;
    }
    sysml_lazy_3 = sysml_lazy_2;
    if (!sysml_lazy_3) {
        sysml_lazy_3 = sysml_state_colorIndex == 5;
    }
    sysml_lazy_4 = sysml_lazy_3;
    if (!sysml_lazy_4) {
        sysml_lazy_4 = sysml_state_colorIndex == 6;
    }
    if (sysml_lazy_4) {
        sysml_conditional_5 = 1.0;
    }
    else {
        sysml_conditional_5 = 0.0;
    }
    sysml_value_0 = sysml_conditional_5;
    double sysml_value_1 = 0;
    sysml_lazy_6 = sysml_state_colorIndex == 1;
    if (!sysml_lazy_6) {
        sysml_lazy_6 = sysml_state_colorIndex == 3;
    }
    sysml_lazy_7 = sysml_lazy_6;
    if (!sysml_lazy_7) {
        sysml_lazy_7 = sysml_state_colorIndex == 4;
    }
    sysml_lazy_8 = sysml_lazy_7;
    if (!sysml_lazy_8) {
        sysml_lazy_8 = sysml_state_colorIndex == 6;
    }
    if (sysml_lazy_8) {
        sysml_conditional_9 = 1.0;
    }
    else {
        sysml_conditional_9 = 0.0;
    }
    sysml_value_1 = sysml_conditional_9;
    double sysml_value_2 = 0;
    sysml_lazy_10 = sysml_state_colorIndex == 2;
    if (!sysml_lazy_10) {
        sysml_lazy_10 = sysml_state_colorIndex == 4;
    }
    sysml_lazy_11 = sysml_lazy_10;
    if (!sysml_lazy_11) {
        sysml_lazy_11 = sysml_state_colorIndex == 5;
    }
    sysml_lazy_12 = sysml_lazy_11;
    if (!sysml_lazy_12) {
        sysml_lazy_12 = sysml_state_colorIndex == 6;
    }
    if (sysml_lazy_12) {
        sysml_conditional_13 = 1.0;
    }
    else {
        sysml_conditional_13 = 0.0;
    }
    sysml_value_2 = sysml_conditional_13;
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
    *io_3 = sysml_value_2;
}