#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_lastHumidity = 0.0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_lastHumidity = 0.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, bool* serial_0_trigger, double* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    if (io_1) {
        sysml_state_lastHumidity = io_0;
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = io_1;
    double sysml_value_1 = 0;
    sysml_value_1 = sysml_state_lastHumidity;
    *serial_0_trigger = sysml_value_0;
    *serial_0_value_0 = sysml_value_1;
}