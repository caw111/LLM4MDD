#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_1, double* io_0, bool* serial_0_trigger, int* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_value_0 = 0;
    double sysml_conditional_0 = 0;
    int sysml_conditional_1 = 0;
    if (io_1) {
        sysml_conditional_0 = 180.0;
    }
    else {
        sysml_conditional_0 = 90.0;
    }
    sysml_value_0 = sysml_conditional_0;
    bool sysml_value_1 = 0;
    sysml_value_1 = true;
    int sysml_value_2 = 0;
    if (io_1) {
        sysml_conditional_1 = 180;
    }
    else {
        sysml_conditional_1 = 90;
    }
    sysml_value_2 = sysml_conditional_1;
    *io_0 = sysml_value_0;
    *serial_0_trigger = sysml_value_1;
    *serial_0_value_0 = sysml_value_2;
}