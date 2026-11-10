#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, double io_2, double* io_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_value_0 = 0;
    double sysml_conditional_0 = 0;
    double sysml_arithmetic_1 = 0;
    double sysml_arithmetic_2 = 0;
    if (io_0 >= io_1) {
        sysml_arithmetic_1 = io_0 - io_1;
        sysml_arithmetic_2 = io_0 - io_1;
        sysml_conditional_0 = (((double)((io_2 - io_1) * sysml_arithmetic_1)) / sysml_arithmetic_2) + io_1;
    }
    else {
        sysml_conditional_0 = 0.0;
    }
    sysml_value_0 = sysml_conditional_0;
    *io_3 = sysml_value_0;
}