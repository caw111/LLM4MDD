#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_reportAccumMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_reportAccumMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, bool io_2, int io_3, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, bool* serial_0_value_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_conditional_0 = 0;
    bool sysml_lazy_1 = 0;
    double sysml_conditional_2 = 0;
    double sysml_conditional_3 = 0;
    sysml_state_reportAccumMs = (sysml_state_reportAccumMs + io_3);
    if ((sysml_state_reportAccumMs >= 1000)) {
        sysml_state_reportAccumMs = (sysml_state_reportAccumMs - 1000);
    }
    bool sysml_value_0 = 0;
    if (sysml_state_reportAccumMs >= 0) {
        sysml_lazy_1 = sysml_state_reportAccumMs >= 1000;
        if (sysml_lazy_1) {
            sysml_lazy_1 = io_2;
        }
        sysml_conditional_0 = sysml_lazy_1;
    }
    else {
        sysml_conditional_0 = false;
    }
    sysml_value_0 = sysml_conditional_0;
    double sysml_value_1 = 0;
    if (io_2) {
        sysml_conditional_2 = io_0;
    }
    else {
        sysml_conditional_2 = io_0;
    }
    sysml_value_1 = sysml_conditional_2;
    double sysml_value_2 = 0;
    if (io_2) {
        sysml_conditional_3 = io_1;
    }
    else {
        sysml_conditional_3 = io_1;
    }
    sysml_value_2 = sysml_conditional_3;
    bool sysml_value_3 = 0;
    sysml_value_3 = io_2;
    *serial_0_trigger = sysml_value_0;
    *serial_0_value_0 = sysml_value_1;
    *serial_0_value_1 = sysml_value_2;
    *serial_0_value_2 = sysml_value_3;
}