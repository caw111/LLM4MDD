#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousPressed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool* serial_0_trigger, bool* serial_0_value_0, bool* serial_1_trigger, bool* serial_1_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    sysml_state_previousPressed = io_0;
    bool sysml_value_0 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    sysml_value_0 = sysml_lazy_0;
    bool sysml_value_1 = 0;
    sysml_value_1 = io_0;
    bool sysml_value_2 = 0;
    sysml_lazy_1 = !io_0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = sysml_state_previousPressed;
    }
    sysml_value_2 = sysml_lazy_1;
    bool sysml_value_3 = 0;
    sysml_value_3 = (!io_0);
    *serial_0_trigger = sysml_value_0;
    *serial_0_value_0 = sysml_value_1;
    *serial_1_trigger = sysml_value_2;
    *serial_1_value_0 = sysml_value_3;
}