#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_accumulatedMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_accumulatedMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_2, bool* io_0, bool* io_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_conditional_5 = 0;
    sysml_state_accumulatedMs = (sysml_state_accumulatedMs + io_2);
    if ((sysml_state_accumulatedMs >= 20000)) {
        sysml_state_accumulatedMs = (sysml_state_accumulatedMs - 20000);
    }
    bool sysml_value_0 = 0;
    sysml_lazy_0 = sysml_state_accumulatedMs >= 5000;
    if (sysml_lazy_0) {
        sysml_lazy_0 = sysml_state_accumulatedMs < 10000;
    }
    if (sysml_lazy_0) {
        sysml_conditional_1 = true;
    }
    else {
        sysml_lazy_2 = sysml_state_accumulatedMs >= 10000;
        if (sysml_lazy_2) {
            sysml_lazy_2 = sysml_state_accumulatedMs < 15000;
        }
        if (sysml_lazy_2) {
            sysml_conditional_3 = false;
        }
        else {
            sysml_conditional_3 = false;
        }
        sysml_conditional_1 = sysml_conditional_3;
    }
    sysml_value_0 = sysml_conditional_1;
    bool sysml_value_1 = 0;
    sysml_lazy_4 = sysml_state_accumulatedMs >= 10000;
    if (sysml_lazy_4) {
        sysml_lazy_4 = sysml_state_accumulatedMs < 15000;
    }
    if (sysml_lazy_4) {
        sysml_conditional_5 = true;
    }
    else {
        sysml_conditional_5 = false;
    }
    sysml_value_1 = sysml_conditional_5;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
}