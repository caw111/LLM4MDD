#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_buzzRemainingMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_buzzRemainingMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_1, bool io_1_valid, int* io_0, bool* serial_0_trigger, int* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    int sysml_conditional_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    sysml_lazy_0 = sysml_state_buzzRemainingMs > 0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_1_valid;
    }
    if (sysml_lazy_0) {
        sysml_state_buzzRemainingMs = 0;
    }
    else {
        if ((sysml_state_buzzRemainingMs > 0)) {
            sysml_state_buzzRemainingMs = 0;
        }
        sysml_lazy_1 = io_1_valid;
        if (sysml_lazy_1) {
            sysml_lazy_1 = io_1 == 1;
        }
        if (sysml_lazy_1) {
            sysml_state_buzzRemainingMs = 500;
        }
    }
    int sysml_value_0 = 0;
    if (sysml_state_buzzRemainingMs > 0) {
        sysml_conditional_2 = 1000;
    }
    else {
        sysml_conditional_2 = 0;
    }
    sysml_value_0 = sysml_conditional_2;
    bool sysml_value_1 = 0;
    sysml_lazy_3 = io_1_valid;
    if (sysml_lazy_3) {
        sysml_lazy_3 = io_1 == 1;
    }
    if (sysml_lazy_3) {
        sysml_conditional_4 = true;
    }
    else {
        sysml_conditional_4 = false;
    }
    sysml_value_1 = sysml_conditional_4;
    int sysml_value_2 = 0;
    if (sysml_state_buzzRemainingMs > 0) {
        sysml_conditional_5 = 1000;
    }
    else {
        sysml_conditional_5 = 0;
    }
    sysml_value_2 = sysml_conditional_5;
    *io_0 = sysml_value_0;
    *serial_0_trigger = sysml_value_1;
    *serial_0_value_0 = sysml_value_2;
}