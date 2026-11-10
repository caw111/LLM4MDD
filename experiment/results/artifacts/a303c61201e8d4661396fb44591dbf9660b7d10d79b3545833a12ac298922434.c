#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_accumulatedMs = 0;
/* Global Variable define of  */
int sysml_state_ledCount = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_accumulatedMs = 0;
    sysml_state_ledCount = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_4, bool* io_0, bool* io_1, bool* io_2, int* io_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_conditional_0 = 0;
    bool sysml_conditional_1 = 0;
    bool sysml_conditional_2 = 0;
    if ((io_4 >= 2000)) {
        sysml_state_accumulatedMs = (io_4 - 2000);
        sysml_state_ledCount = 0;
    }
    else {
        sysml_state_accumulatedMs = (sysml_state_accumulatedMs + io_4);
        if (((sysml_state_accumulatedMs + io_4) >= 2000)) {
            sysml_state_accumulatedMs = ((sysml_state_accumulatedMs + io_4) - 2000);
            sysml_state_ledCount = 0;
        }
    }
    bool sysml_value_0 = 0;
    if (sysml_state_ledCount >= 1) {
        sysml_conditional_0 = true;
    }
    else {
        sysml_conditional_0 = false;
    }
    sysml_value_0 = sysml_conditional_0;
    bool sysml_value_1 = 0;
    if (sysml_state_ledCount >= 2) {
        sysml_conditional_1 = true;
    }
    else {
        sysml_conditional_1 = false;
    }
    sysml_value_1 = sysml_conditional_1;
    bool sysml_value_2 = 0;
    if (sysml_state_ledCount >= 3) {
        sysml_conditional_2 = true;
    }
    else {
        sysml_conditional_2 = false;
    }
    sysml_value_2 = sysml_conditional_2;
    int sysml_value_3 = 0;
    sysml_value_3 = sysml_state_ledCount;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
}