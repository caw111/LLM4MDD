#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_modeManual = false;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_modeManual = false;
    sysml_state_previousPressed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, bool io_2, bool io_4, double* io_3, bool* io_5) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    double sysml_conditional_3 = 0;
    bool sysml_conditional_4 = 0;
    bool sysml_lazy_5 = 0;
    sysml_lazy_0 = io_4;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        sysml_state_modeManual = (!sysml_state_modeManual);
    }
    sysml_state_previousPressed = io_4;
    double sysml_value_0 = 0;
    sysml_lazy_1 = sysml_state_modeManual;
    if (!sysml_lazy_1) {
        sysml_lazy_2 = io_0 > 0.0;
        if (sysml_lazy_2) {
            sysml_lazy_2 = io_2;
        }
        sysml_lazy_1 = sysml_lazy_2;
    }
    if (sysml_lazy_1) {
        sysml_conditional_3 = 255.0;
    }
    else {
        sysml_conditional_3 = 0.0;
    }
    sysml_value_0 = sysml_conditional_3;
    bool sysml_value_1 = 0;
    if (sysml_state_modeManual) {
        sysml_conditional_4 = true;
    }
    else {
        sysml_lazy_5 = io_0 > 0.0;
        if (sysml_lazy_5) {
            sysml_lazy_5 = io_2;
        }
        sysml_conditional_4 = sysml_lazy_5;
    }
    sysml_value_1 = sysml_conditional_4;
    *io_3 = sysml_value_0;
    *io_5 = sysml_value_1;
}