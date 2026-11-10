#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_manualMode = false;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_manualMode = false;
    sysml_state_previousPressed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, bool io_2, bool io_4, double* io_3, bool* io_5) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    double sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    double sysml_conditional_4 = 0;
    sysml_lazy_0 = io_4;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        sysml_state_manualMode = (!sysml_state_manualMode);
    }
    sysml_state_previousPressed = io_4;
    double sysml_value_0 = 0;
    if (sysml_state_manualMode) {
        sysml_conditional_1 = 1023.0;
    }
    else {
        sysml_lazy_2 = io_0 < 512.0;
        if (sysml_lazy_2) {
            sysml_lazy_2 = io_2;
        }
        sysml_lazy_3 = sysml_lazy_2;
        if (sysml_lazy_3) {
            sysml_lazy_3 = true == true;
        }
        if (sysml_lazy_3) {
            sysml_conditional_4 = 1023.0;
        }
        else {
            sysml_conditional_4 = 0.0;
        }
        sysml_conditional_1 = sysml_conditional_4;
    }
    sysml_value_0 = sysml_conditional_1;
    bool sysml_value_1 = 0;
    sysml_value_1 = sysml_state_manualMode;
    *io_3 = sysml_value_0;
    *io_5 = sysml_value_1;
}