#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_pwmDuty = 0.0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_pwmDuty = 0.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_1, bool io_1_valid, double* io_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    if (io_1_valid) {
        sysml_state_pwmDuty = io_1;
    }
    double sysml_value_0 = 0;
    sysml_value_0 = sysml_state_pwmDuty;
    *io_0 = sysml_value_0;
}