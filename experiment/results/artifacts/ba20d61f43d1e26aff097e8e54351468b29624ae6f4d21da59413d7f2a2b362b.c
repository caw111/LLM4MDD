#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_lastDistanceCm = 0.0;
/* Global Variable define of  */
bool sysml_state_hasLastValid = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_lastDistanceCm = 0.0;
    sysml_state_hasLastValid = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, int io_2, bool io_2_valid, bool* serial_0_trigger, double* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    double sysml_conditional_2 = 0;
    if (io_1) {
        sysml_state_lastDistanceCm = io_0;
        sysml_state_hasLastValid = true;
    }
    sysml_lazy_0 = io_2_valid;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_2 == 1;
    }
    if (sysml_lazy_0) {
    }
    bool sysml_value_0 = 0;
    sysml_lazy_1 = io_2_valid;
    if (sysml_lazy_1) {
        sysml_lazy_1 = io_2 == 1;
    }
    sysml_value_0 = sysml_lazy_1;
    double sysml_value_1 = 0;
    if (sysml_state_hasLastValid) {
        sysml_conditional_2 = sysml_state_lastDistanceCm;
    }
    else {
        sysml_conditional_2 = 0.0;
    }
    sysml_value_1 = sysml_conditional_2;
    *serial_0_trigger = sysml_value_0;
    *serial_0_value_0 = sysml_value_1;
}