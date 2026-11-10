#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_0, bool io_1, int* io_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_value_0 = 0;
    bool sysml_lazy_0 = 0;
    int sysml_conditional_1 = 0;
    sysml_lazy_0 = io_1;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_0 >= 1732423740;
    }
    if (sysml_lazy_0) {
        sysml_conditional_1 = 1000;
    }
    else {
        sysml_conditional_1 = 0;
    }
    sysml_value_0 = sysml_conditional_1;
    *io_2 = sysml_value_0;
}