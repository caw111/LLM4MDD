#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_value = 0;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_value = 0;
    sysml_state_previousPressed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, int* io_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        if ((sysml_state_value + 37) > 99) {
            sysml_conditional_1 = 0;
        }
        else {
            sysml_conditional_1 = sysml_state_value + 37;
        }
        sysml_state_value = sysml_conditional_1;
    }
    sysml_state_previousPressed = io_0;
    int sysml_value_0 = 0;
    if (sysml_state_value < 0) {
        sysml_conditional_2 = 0;
    }
    else {
        sysml_conditional_2 = sysml_state_value;
    }
    sysml_value_0 = sysml_conditional_2;
    *io_1 = sysml_value_0;
}