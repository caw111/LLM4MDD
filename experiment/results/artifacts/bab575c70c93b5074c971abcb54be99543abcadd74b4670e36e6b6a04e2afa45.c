#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_serialTick = 0;
/* Global Variable define of  */
bool sysml_state_initialized = false;
/* Global Variable define of  */
double sysml_state_lastValue = 0.0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_serialTick = 0;
    sysml_state_initialized = false;
    sysml_state_lastValue = 0.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool* serial_0_trigger, double* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    if ((!sysml_state_initialized)) {
        sysml_state_initialized = true;
        sysml_state_serialTick = 0;
        sysml_state_lastValue = io_0;
    }
    sysml_state_serialTick = (sysml_state_serialTick + 1);
    if (true) {
        sysml_state_lastValue = io_0;
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = true;
    double sysml_value_1 = 0;
    sysml_value_1 = sysml_state_lastValue;
    *serial_0_trigger = sysml_value_0;
    *serial_0_value_0 = sysml_value_1;
}