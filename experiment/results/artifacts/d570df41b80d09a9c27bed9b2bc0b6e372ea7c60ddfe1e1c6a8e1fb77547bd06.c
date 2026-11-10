#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_manualFrozen = false;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
int sysml_state_frozenDisplayValue = 0;
/* Global Variable define of  */
int sysml_state_frozenPwmValue = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_manualFrozen = false;
    sysml_state_previousPressed = false;
    sysml_state_frozenDisplayValue = 0;
    sysml_state_frozenPwmValue = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_2, double* io_1, bool* io_3, int* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    int sysml_conditional_6 = 0;
    sysml_lazy_0 = io_2;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        sysml_state_manualFrozen = (!sysml_state_manualFrozen);
    }
    if ((!sysml_state_manualFrozen)) {
        if (io_0 < ((double)((int)io_0))) {
            sysml_conditional_1 = 1;
        }
        else {
            sysml_conditional_1 = 0;
        }
        sysml_state_frozenDisplayValue = ((int)io_0) - sysml_conditional_1;
        if (io_0 < ((double)((int)io_0))) {
            sysml_conditional_2 = 1;
        }
        else {
            sysml_conditional_2 = 0;
        }
        sysml_state_frozenPwmValue = ((int)io_0) - sysml_conditional_2;
    }
    sysml_state_previousPressed = io_2;
    double sysml_value_0 = 0;
    if (sysml_state_manualFrozen) {
        sysml_conditional_3 = sysml_state_frozenPwmValue;
    }
    else {
        if (io_0 < ((double)((int)io_0))) {
            sysml_conditional_4 = 1;
        }
        else {
            sysml_conditional_4 = 0;
        }
        sysml_conditional_3 = ((int)io_0) - sysml_conditional_4;
    }
    sysml_value_0 = sysml_conditional_3;
    bool sysml_value_1 = 0;
    sysml_value_1 = sysml_state_manualFrozen;
    int sysml_value_2 = 0;
    if (sysml_state_manualFrozen) {
        sysml_conditional_5 = sysml_state_frozenDisplayValue;
    }
    else {
        if (io_0 < ((double)((int)io_0))) {
            sysml_conditional_6 = 1;
        }
        else {
            sysml_conditional_6 = 0;
        }
        sysml_conditional_5 = ((int)io_0) - sysml_conditional_6;
    }
    sysml_value_2 = sysml_conditional_5;
    *io_1 = sysml_value_0;
    *io_3 = sysml_value_1;
    *io_4 = sysml_value_2;
}