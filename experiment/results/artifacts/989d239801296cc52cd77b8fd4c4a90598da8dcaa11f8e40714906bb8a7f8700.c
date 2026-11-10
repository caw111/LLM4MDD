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
void ProgramPage1(double io_0, bool io_1, int io_2, bool* serial_0_trigger, double* serial_0_value_0, bool* serial_0_value_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    sysml_state_accumulatedMs = (sysml_state_accumulatedMs + io_2);
    if ((sysml_state_accumulatedMs >= 1000)) {
        sysml_state_accumulatedMs = (sysml_state_accumulatedMs - 1000);
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = (sysml_state_accumulatedMs >= 1000);
    double sysml_value_1 = 0;
    sysml_value_1 = io_0;
    bool sysml_value_2 = 0;
    sysml_value_2 = io_1;
    *serial_0_trigger = sysml_value_0;
    *serial_0_value_0 = sysml_value_1;
    *serial_0_value_1 = sysml_value_2;
}