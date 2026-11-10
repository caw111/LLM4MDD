#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_elapsedAccumMs = 0;
/* Global Variable define of  */
int sysml_state_cycleAccumMs = 0;
/* Global Variable define of  */
bool sysml_state_previousBecameActive = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_elapsedAccumMs = 0;
    sysml_state_cycleAccumMs = 0;
    sysml_state_previousBecameActive = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_0, int* io_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    int sysml_conditional_3 = 0;
    sysml_state_elapsedAccumMs = (sysml_state_elapsedAccumMs + io_0);
    sysml_state_cycleAccumMs = (sysml_state_cycleAccumMs + io_0);
    sysml_lazy_0 = sysml_state_cycleAccumMs >= 2000;
    if (!sysml_lazy_0) {
        sysml_lazy_0 = sysml_state_elapsedAccumMs >= 2000;
    }
    if (sysml_lazy_0) {
        sysml_state_cycleAccumMs = (sysml_state_cycleAccumMs - 2000);
    }
    sysml_lazy_1 = sysml_state_cycleAccumMs < 200;
    if (sysml_lazy_1) {
        sysml_lazy_1 = sysml_state_cycleAccumMs >= 0;
    }
    if (sysml_lazy_1) {
        sysml_state_previousBecameActive = true;
    }
    else {
        sysml_state_previousBecameActive = false;
    }
    int sysml_value_0 = 0;
    sysml_lazy_2 = sysml_state_cycleAccumMs < 200;
    if (sysml_lazy_2) {
        sysml_lazy_2 = sysml_state_cycleAccumMs >= 0;
    }
    if (sysml_lazy_2) {
        sysml_conditional_3 = 31;
    }
    else {
        sysml_conditional_3 = 0;
    }
    sysml_value_0 = sysml_conditional_3;
    *io_1 = sysml_value_0;
}