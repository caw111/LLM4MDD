#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_io_0_clamped = 0.0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_io_0_clamped = 0.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double* io_1, bool* io_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_conditional_0 = 0;
    double sysml_conditional_1 = 0;
    double sysml_conditional_2 = 0;
    double sysml_conditional_3 = 0;
    if (io_0 < 0.0) {
        sysml_conditional_0 = 0.0;
    }
    else {
        if (io_0 > 1023.0) {
            sysml_conditional_1 = 1023.0;
        }
        else {
            sysml_conditional_1 = io_0;
        }
        sysml_conditional_0 = sysml_conditional_1;
    }
    sysml_state_io_0_clamped = sysml_conditional_0;
    double sysml_value_0 = 0;
    if (sysml_state_io_0_clamped == 0.0) {
        sysml_conditional_2 = 1023.0;
    }
    else {
        if (sysml_state_io_0_clamped == 1023.0) {
            sysml_conditional_3 = 0.0;
        }
        else {
            sysml_conditional_3 = 1023.0 - sysml_state_io_0_clamped;
        }
        sysml_conditional_2 = sysml_conditional_3;
    }
    sysml_value_0 = sysml_conditional_2;
    bool sysml_value_1 = 0;
    sysml_value_1 = (io_0 <= 512.0);
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
}