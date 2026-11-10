#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
int sysml_state_debounceCounter = 0;
/* Global Variable define of  */
bool sysml_state_stablePressed = false;
/* Global Variable define of  */
bool sysml_state_toggleState = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousPressed = false;
    sysml_state_debounceCounter = 0;
    sysml_state_stablePressed = false;
    sysml_state_toggleState = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, double* io_1, double* io_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    double sysml_conditional_3 = 0;
    double sysml_conditional_4 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        sysml_state_debounceCounter = 0;
    }
    if (io_0) {
        sysml_state_debounceCounter = (sysml_state_debounceCounter + 1);
    }
    sysml_lazy_1 = sysml_state_stablePressed;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !io_0;
    }
    if (sysml_lazy_1) {
        sysml_state_debounceCounter = 0;
    }
    if ((!io_0)) {
        sysml_state_debounceCounter = (sysml_state_debounceCounter + 1);
    }
    if ((sysml_state_debounceCounter >= 2)) {
        sysml_state_stablePressed = io_0;
    }
    sysml_lazy_2 = sysml_state_stablePressed;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_2) {
        sysml_state_toggleState = (!sysml_state_toggleState);
    }
    sysml_state_previousPressed = sysml_state_stablePressed;
    double sysml_value_0 = 0;
    if (sysml_state_toggleState) {
        sysml_conditional_3 = 180.0;
    }
    else {
        sysml_conditional_3 = 0.0;
    }
    sysml_value_0 = sysml_conditional_3;
    double sysml_value_1 = 0;
    if (sysml_state_toggleState) {
        sysml_conditional_4 = 0.0;
    }
    else {
        sysml_conditional_4 = 180.0;
    }
    sysml_value_1 = sysml_conditional_4;
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
}