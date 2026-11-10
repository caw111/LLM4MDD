#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_accumulatedMs = 0;
/* Global Variable define of  */
bool sysml_state_ledOn = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_accumulatedMs = 0;
    sysml_state_ledOn = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_1, bool* io_0, bool* serial_0_trigger, bool* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    sysml_state_accumulatedMs = (sysml_state_accumulatedMs + io_1);
    if ((sysml_state_accumulatedMs >= 1000)) {
        sysml_state_accumulatedMs = (sysml_state_accumulatedMs - 1000);
        sysml_state_ledOn = (!sysml_state_ledOn);
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_ledOn;
    bool sysml_value_1 = 0;
    sysml_value_1 = true;
    bool sysml_value_2 = 0;
    sysml_value_2 = false;
    *io_0 = sysml_value_0;
    *serial_0_trigger = sysml_value_1;
    *serial_0_value_0 = sysml_value_2;
}