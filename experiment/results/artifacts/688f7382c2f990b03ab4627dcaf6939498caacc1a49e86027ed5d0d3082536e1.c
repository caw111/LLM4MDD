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
void ProgramPage1(int io_1, bool* io_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_conditional_0 = 0;
    bool sysml_conditional_1 = 0;
    bool sysml_conditional_2 = 0;
    sysml_state_accumulatedMs = (sysml_state_accumulatedMs + io_1);
    if ((sysml_state_accumulatedMs >= 9000)) {
        sysml_state_accumulatedMs = (sysml_state_accumulatedMs - 9000);
    }
    bool sysml_value_0 = 0;
    if (sysml_state_accumulatedMs < 1000) {
        sysml_conditional_0 = true;
    }
    else {
        if (sysml_state_accumulatedMs < 3000) {
            sysml_conditional_1 = false;
        }
        else {
            if (sysml_state_accumulatedMs < 6000) {
                sysml_conditional_2 = true;
            }
            else {
                sysml_conditional_2 = false;
            }
            sysml_conditional_1 = sysml_conditional_2;
        }
        sysml_conditional_0 = sysml_conditional_1;
    }
    sysml_value_0 = sysml_conditional_0;
    *io_0 = sysml_value_0;
}