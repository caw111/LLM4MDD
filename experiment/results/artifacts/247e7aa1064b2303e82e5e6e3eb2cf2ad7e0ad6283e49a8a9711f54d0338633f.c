#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_shift595_number = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_shift595_number = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, int* io_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    sysml_lazy_0 = io_0 >= 0.0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_0 <= 99.0;
    }
    if (sysml_lazy_0) {
        if (((((double)io_0) / 1.0) * 1.0) < ((double)((int)((((double)io_0) / 1.0) * 1.0)))) {
            sysml_conditional_2 = 1;
        }
        else {
            sysml_conditional_2 = 0;
        }
        sysml_conditional_1 = ((int)((((double)io_0) / 1.0) * 1.0)) - sysml_conditional_2;
    }
    else {
        sysml_conditional_1 = 0;
    }
    sysml_state_shift595_number = sysml_conditional_1;
    int sysml_value_0 = 0;
    sysml_value_0 = sysml_state_shift595_number;
    *io_1 = sysml_value_0;
}