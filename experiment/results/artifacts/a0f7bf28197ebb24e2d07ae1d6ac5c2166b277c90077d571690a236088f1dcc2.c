#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_io0Prev = 0.0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_io0Prev = 0.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_1, bool* io_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    sysml_state_io0Prev = io_1;
    bool sysml_value_0 = 0;
    sysml_value_0 = false;
    *io_0 = sysml_value_0;
}