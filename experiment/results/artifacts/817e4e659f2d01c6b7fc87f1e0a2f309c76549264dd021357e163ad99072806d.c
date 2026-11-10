#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_count = 0;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
bool sysml_state_pendingReport = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_count = 0;
    sysml_state_previousPressed = false;
    sysml_state_pendingReport = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, int io_1, int io_2, bool io_2_valid, bool* serial_0_trigger, int* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        sysml_state_count = (sysml_state_count + 1);
        sysml_state_pendingReport = true;
    }
    else {
        sysml_state_pendingReport = false;
    }
    sysml_state_previousPressed = io_0;
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_pendingReport;
    int sysml_value_1 = 0;
    sysml_value_1 = sysml_state_count;
    *serial_0_trigger = sysml_value_0;
    *serial_0_value_0 = sysml_value_1;
}