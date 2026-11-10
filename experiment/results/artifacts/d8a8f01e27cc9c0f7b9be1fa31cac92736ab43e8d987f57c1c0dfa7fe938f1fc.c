#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_repairAccMs = 0;
/* Global Variable define of  */
bool sysml_state_repairLedLit = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_repairAccMs = 0;
    sysml_state_repairLedLit = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_1, bool* io_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    sysml_state_repairAccMs = (sysml_state_repairAccMs + io_1);
    if ((sysml_state_repairAccMs >= 500)) {
        sysml_state_repairAccMs = (sysml_state_repairAccMs - 500);
        sysml_state_repairLedLit = (!sysml_state_repairLedLit);
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_repairLedLit;
    *io_0 = sysml_value_0;
}