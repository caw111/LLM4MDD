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
void ProgramPage1(bool io_3, double* io_0, double* io_1, double* io_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int sysml_conditional_1 = 0;
    double sysml_conditional_2 = 0;
    double sysml_conditional_3 = 0;
    double sysml_conditional_4 = 0;
    sysml_lazy_0 = io_3;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        if (sysml_state_colorIndex == 3) {
            sysml_conditional_1 = 0;
        }
        else {
            sysml_conditional_1 = sysml_state_colorIndex + 1;
        }
        sysml_state_colorIndex = sysml_conditional_1;
    }
    sysml_state_previousPressed = io_3;
    double sysml_value_0 = 0;
    if (sysml_state_colorIndex == 0) {
        sysml_conditional_2 = 1.0;
    }
    else {
        sysml_conditional_2 = 0.0;
    }
    sysml_value_0 = sysml_conditional_2;
    double sysml_value_1 = 0;
    if (sysml_state_colorIndex == 1) {
        sysml_conditional_3 = 1.0;
    }
    else {
        sysml_conditional_3 = 0.0;
    }
    sysml_value_1 = sysml_conditional_3;
    double sysml_value_2 = 0;
    if (sysml_state_colorIndex == 2) {
        sysml_conditional_4 = 1.0;
    }
    else {
        sysml_conditional_4 = 0.0;
    }
    sysml_value_2 = sysml_conditional_4;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
}