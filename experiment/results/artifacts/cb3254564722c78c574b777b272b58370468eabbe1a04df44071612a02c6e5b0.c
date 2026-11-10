#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_tMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_tMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_1, double* io_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    double sysml_conditional_1 = 0;
    if ((sysml_state_tMs + io_1) >= 2000) {
        sysml_conditional_0 = (sysml_state_tMs + io_1) - 2000;
    }
    else {
        sysml_conditional_0 = sysml_state_tMs + io_1;
    }
    sysml_state_tMs = sysml_conditional_0;
    double sysml_value_0 = 0;
    if (sysml_state_tMs < 1000) {
        sysml_conditional_1 = (((double)sysml_state_tMs) / 1000.0) * 255.0;
    }
    else {
        sysml_conditional_1 = (((double)(2000 - sysml_state_tMs)) / 1000.0) * 255.0;
    }
    sysml_value_0 = sysml_conditional_1;
    *io_0 = sysml_value_0;
}