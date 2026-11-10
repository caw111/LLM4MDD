#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_lastIo0Value = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_lastIo0Value = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_0, bool io_0_valid, int io_2, bool* io_1, bool* serial_0_trigger, int* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    if (io_0_valid) {
        sysml_state_lastIo0Value = io_0;
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = io_0_valid;
    bool sysml_value_1 = 0;
    sysml_value_1 = io_0_valid;
    int sysml_value_2 = 0;
    sysml_value_2 = sysml_state_lastIo0Value;
    *io_1 = sysml_value_0;
    *serial_0_trigger = sysml_value_1;
    *serial_0_value_0 = sysml_value_2;
}