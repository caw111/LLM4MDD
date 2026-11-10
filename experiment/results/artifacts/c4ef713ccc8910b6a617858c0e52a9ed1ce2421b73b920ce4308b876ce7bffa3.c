#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_2, bool io_3, bool* io_0, int* io_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_value_0 = 0;
    bool sysml_lazy_0 = 0;
    bool sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    int sysml_conditional_3 = 0;
    sysml_lazy_0 = io_3;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_2 < 20.0;
    }
    if (sysml_lazy_0) {
        sysml_conditional_1 = true;
    }
    else {
        sysml_conditional_1 = false;
    }
    sysml_value_0 = sysml_conditional_1;
    int sysml_value_1 = 0;
    sysml_lazy_2 = io_3;
    if (sysml_lazy_2) {
        sysml_lazy_2 = io_2 < 20.0;
    }
    if (sysml_lazy_2) {
        sysml_conditional_3 = 1000;
    }
    else {
        sysml_conditional_3 = 0;
    }
    sysml_value_1 = sysml_conditional_3;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
}