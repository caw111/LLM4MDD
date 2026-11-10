#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_accumulatedMs = 0;
/* Global Variable define of  */
bool sysml_state_ledToggle = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_accumulatedMs = 0;
    sysml_state_ledToggle = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_2, bool* io_0, bool* io_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    sysml_state_accumulatedMs = (sysml_state_accumulatedMs + io_2);
    if ((sysml_state_accumulatedMs >= 1000)) {
        sysml_state_accumulatedMs = (sysml_state_accumulatedMs - 1000);
        sysml_state_ledToggle = (!sysml_state_ledToggle);
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_ledToggle;
    bool sysml_value_1 = 0;
    sysml_value_1 = (!sysml_state_ledToggle);
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
}