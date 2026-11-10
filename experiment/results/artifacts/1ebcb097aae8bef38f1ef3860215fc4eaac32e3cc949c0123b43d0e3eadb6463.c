#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_lastValidDistanceCm = 400.0;
/* Global Variable define of  */
bool sysml_state_distanceValidHeld = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_lastValidDistanceCm = 400.0;
    sysml_state_distanceValidHeld = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, int io_2, bool* serial_0_trigger, double* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_conditional_0 = 0;
    if (io_1) {
        sysml_state_lastValidDistanceCm = io_0;
        sysml_state_distanceValidHeld = true;
    }
    else {
        sysml_state_distanceValidHeld = false;
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = true;
    double sysml_value_1 = 0;
    if (sysml_state_distanceValidHeld) {
        sysml_conditional_0 = sysml_state_lastValidDistanceCm;
    }
    else {
        sysml_conditional_0 = sysml_state_lastValidDistanceCm;
    }
    sysml_value_1 = sysml_conditional_0;
    *serial_0_trigger = sysml_value_0;
    *serial_0_value_0 = sysml_value_1;
}