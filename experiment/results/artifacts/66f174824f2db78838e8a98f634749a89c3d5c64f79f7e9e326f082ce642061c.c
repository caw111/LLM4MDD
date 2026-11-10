#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_accumulatedMs = 0;
/* Global Variable define of  */
double sysml_state_lastValidTemp = 0.0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_accumulatedMs = 0;
    sysml_state_lastValidTemp = 0.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_0, double io_1, bool io_2, bool* serial_0_trigger, double* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_conditional_0 = 0;
    if (io_2) {
        sysml_state_lastValidTemp = io_1;
    }
    sysml_state_accumulatedMs = (sysml_state_accumulatedMs + io_0);
    if ((sysml_state_accumulatedMs >= 5000)) {
        sysml_state_accumulatedMs = (sysml_state_accumulatedMs - 5000);
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = (sysml_state_accumulatedMs >= 5000);
    double sysml_value_1 = 0;
    if (io_2) {
        sysml_conditional_0 = io_1;
    }
    else {
        sysml_conditional_0 = sysml_state_lastValidTemp;
    }
    sysml_value_1 = sysml_conditional_0;
    *serial_0_trigger = sysml_value_0;
    *serial_0_value_0 = sysml_value_1;
}