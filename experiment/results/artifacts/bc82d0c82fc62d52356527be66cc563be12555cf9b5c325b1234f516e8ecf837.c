#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_prev_valid = false;
/* Global Variable define of  */
double sysml_state_serial_value_hold = 0.0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_prev_valid = false;
    sysml_state_serial_value_hold = 0.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, bool* serial_0_trigger, double* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    if (io_1) {
        sysml_state_serial_value_hold = io_0;
    }
    sysml_state_prev_valid = io_1;
    bool sysml_value_0 = 0;
    sysml_lazy_0 = io_1;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_prev_valid;
    }
    sysml_value_0 = sysml_lazy_0;
    double sysml_value_1 = 0;
    sysml_value_1 = sysml_state_serial_value_hold;
    *serial_0_trigger = sysml_value_0;
    *serial_0_value_0 = sysml_value_1;
}