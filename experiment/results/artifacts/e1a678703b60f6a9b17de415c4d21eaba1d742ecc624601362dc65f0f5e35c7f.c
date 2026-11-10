#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_1, bool io_1_valid, double* io_0, bool* serial_0_trigger, int* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_value_0 = 0;
    double sysml_conditional_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    if (io_1_valid) {
        if ((io_1 * 1.0) < ((double)((int)(io_1 * 1.0)))) {
            sysml_conditional_1 = 1;
        }
        else {
            sysml_conditional_1 = 0;
        }
        sysml_conditional_0 = ((int)(io_1 * 1.0)) - sysml_conditional_1;
    }
    else {
        sysml_conditional_0 = 0.0;
    }
    sysml_value_0 = sysml_conditional_0;
    bool sysml_value_1 = 0;
    sysml_value_1 = io_1_valid;
    int sysml_value_2 = 0;
    if (io_1_valid) {
        sysml_conditional_2 = io_1;
    }
    else {
        sysml_conditional_2 = 0;
    }
    sysml_value_2 = sysml_conditional_2;
    *io_0 = sysml_value_0;
    *serial_0_trigger = sysml_value_1;
    *serial_0_value_0 = sysml_value_2;
}