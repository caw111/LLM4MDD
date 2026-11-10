#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, int* io_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_value_0 = 0;
    int sysml_conditional_0 = 0;
    if (io_0) {
        sysml_conditional_0 = 1111;
    }
    else {
        sysml_conditional_0 = 0;
    }
    sysml_value_0 = sysml_conditional_0;
    *io_1 = sysml_value_0;
}