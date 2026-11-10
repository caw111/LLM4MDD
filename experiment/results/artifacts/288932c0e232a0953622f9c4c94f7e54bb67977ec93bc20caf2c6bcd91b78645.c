#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_aboveThreshold = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_aboveThreshold = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, bool io_2, int io_3, bool io_3_valid, bool* serial_0_trigger, bool* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    if (io_2) {
        sysml_state_aboveThreshold = (io_0 > 30.0);
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_aboveThreshold;
    bool sysml_value_1 = 0;
    sysml_value_1 = true;
    *serial_0_trigger = sysml_value_0;
    *serial_0_value_0 = sysml_value_1;
}