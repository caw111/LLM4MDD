#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_counter = 0;
/* Global Variable define of  */
bool sysml_state_previousIo0 = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_counter = 0;
    sysml_state_previousIo0 = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, int io_1, bool* serial_0_trigger, int* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousIo0;
    }
    if (sysml_lazy_0) {
        sysml_state_counter = (sysml_state_counter + 1);
        sysml_state_previousIo0 = io_0;
    }
    else {
        sysml_state_previousIo0 = io_0;
    }
    bool sysml_value_0 = 0;
    sysml_lazy_1 = io_0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_previousIo0;
    }
    sysml_value_0 = sysml_lazy_1;
    int sysml_value_1 = 0;
    sysml_value_1 = sysml_state_counter;
    *serial_0_trigger = sysml_value_0;
    *serial_0_value_0 = sysml_value_1;
}