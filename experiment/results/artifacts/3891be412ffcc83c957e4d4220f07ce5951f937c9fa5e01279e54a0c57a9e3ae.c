#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, double* io_3, double* io_4, double* io_5) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_value_0 = 0;
    double sysml_conditional_0 = 0;
    double sysml_conditional_1 = 0;
    double sysml_conditional_2 = 0;
    if (io_0) {
        sysml_conditional_0 = 255.0;
    }
    else {
        sysml_conditional_0 = 0.0;
    }
    sysml_value_0 = sysml_conditional_0;
    double sysml_value_1 = 0;
    if (io_1) {
        sysml_conditional_1 = 255.0;
    }
    else {
        sysml_conditional_1 = 0.0;
    }
    sysml_value_1 = sysml_conditional_1;
    double sysml_value_2 = 0;
    if (io_2) {
        sysml_conditional_2 = 255.0;
    }
    else {
        sysml_conditional_2 = 0.0;
    }
    sysml_value_2 = sysml_conditional_2;
    *io_3 = sysml_value_0;
    *io_4 = sysml_value_1;
    *io_5 = sysml_value_2;
}