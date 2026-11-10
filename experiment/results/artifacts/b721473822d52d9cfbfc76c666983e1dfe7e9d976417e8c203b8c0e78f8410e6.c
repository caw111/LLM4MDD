#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_serial0ValidPrev = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_serial0ValidPrev = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool* io_0, bool* serial_0_trigger, int* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    sysml_state_serial0ValidPrev = true;
    bool sysml_value_0 = 0;
    sysml_value_0 = true;
    bool sysml_value_1 = 0;
    sysml_value_1 = true;
    int sysml_value_2 = 0;
    sysml_value_2 = 0;
    *io_0 = sysml_value_0;
    *serial_0_trigger = sysml_value_1;
    *serial_0_value_0 = sysml_value_2;
}