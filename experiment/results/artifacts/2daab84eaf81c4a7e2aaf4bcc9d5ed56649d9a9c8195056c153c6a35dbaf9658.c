#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_elapsedAccumMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_elapsedAccumMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, int elapsedMilliseconds, bool* serial_0_trigger, double* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_conditional_0 = 0;
    sysml_state_elapsedAccumMs = (sysml_state_elapsedAccumMs + elapsedMilliseconds);
    if ((sysml_state_elapsedAccumMs >= 1000)) {
        sysml_state_elapsedAccumMs = (sysml_state_elapsedAccumMs - 1000);
    }
    bool sysml_value_0 = 0;
    if ((sysml_state_elapsedAccumMs + elapsedMilliseconds) >= 1000) {
        sysml_conditional_0 = true;
    }
    else {
        sysml_conditional_0 = false;
    }
    sysml_value_0 = sysml_conditional_0;
    double sysml_value_1 = 0;
    sysml_value_1 = io_0;
    *serial_0_trigger = sysml_value_0;
    *serial_0_value_0 = sysml_value_1;
}