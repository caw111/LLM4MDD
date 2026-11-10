#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_accumulated = 0;
/* Global Variable define of  */
bool sysml_state_serialTrig = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_accumulated = 0;
    sysml_state_serialTrig = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, int io_1, bool* serial_0_trigger, double* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    sysml_state_accumulated = (sysml_state_accumulated + io_1);
    if ((sysml_state_accumulated >= 1000)) {
        sysml_state_accumulated = (sysml_state_accumulated - 1000);
        sysml_state_serialTrig = true;
    }
    else {
        sysml_state_serialTrig = false;
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_serialTrig;
    double sysml_value_1 = 0;
    sysml_value_1 = io_0;
    *serial_0_trigger = sysml_value_0;
    *serial_0_value_0 = sysml_value_1;
}