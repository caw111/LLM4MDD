#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_count = 0;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
bool sysml_state_debouncedPressed = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_count = 0;
    sysml_state_previousPressed = false;
    sysml_state_debouncedPressed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_1, int* io_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    int sysml_conditional_2 = 0;
    sysml_lazy_0 = io_1;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        sysml_state_debouncedPressed = true;
    }
    sysml_lazy_1 = !io_1;
    if (sysml_lazy_1) {
        sysml_lazy_1 = sysml_state_previousPressed;
    }
    if (sysml_lazy_1) {
        sysml_state_debouncedPressed = false;
    }
    if (sysml_state_debouncedPressed) {
        if (sysml_state_count == 9) {
            sysml_conditional_2 = 0;
        }
        else {
            sysml_conditional_2 = sysml_state_count + 3;
        }
        sysml_state_count = sysml_conditional_2;
        sysml_state_debouncedPressed = false;
    }
    sysml_state_previousPressed = io_1;
    int sysml_value_0 = 0;
    sysml_value_0 = sysml_state_count;
    *io_0 = sysml_value_0;
}