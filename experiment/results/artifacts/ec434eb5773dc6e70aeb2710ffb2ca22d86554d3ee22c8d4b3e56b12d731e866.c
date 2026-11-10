#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_elapsedAccMs = 0;
/* Global Variable define of  */
bool sysml_state_pendingTrigger = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_elapsedAccMs = 0;
    sysml_state_pendingTrigger = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_0, bool* io_1, bool* serial_0_trigger, bool* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    sysml_state_elapsedAccMs = (sysml_state_elapsedAccMs + io_0);
    if ((sysml_state_elapsedAccMs >= 2000)) {
        sysml_state_elapsedAccMs = (sysml_state_elapsedAccMs - 2000);
        sysml_state_pendingTrigger = true;
    }
    else {
        sysml_state_pendingTrigger = false;
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_pendingTrigger;
    bool sysml_value_1 = 0;
    sysml_value_1 = sysml_state_pendingTrigger;
    bool sysml_value_2 = 0;
    sysml_value_2 = sysml_state_pendingTrigger;
    *io_1 = sysml_value_0;
    *serial_0_trigger = sysml_value_1;
    *serial_0_value_0 = sysml_value_2;
}