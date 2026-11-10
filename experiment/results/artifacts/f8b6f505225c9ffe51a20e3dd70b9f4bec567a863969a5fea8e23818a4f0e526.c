#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool* serial_0_trigger, bool* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_value_0 = 0;
    sysml_value_0 = true;
    bool sysml_value_1 = 0;
    sysml_value_1 = io_0;
    *serial_0_trigger = sysml_value_0;
    *serial_0_value_0 = sysml_value_1;
}