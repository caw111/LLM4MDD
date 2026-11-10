#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
bool sysml_state_releaseDelayActive = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousPressed = false;
    sysml_state_releaseDelayActive = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, int* io_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    if (io_0) {
        sysml_state_releaseDelayActive = false;
    }
    sysml_lazy_0 = !io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        sysml_state_releaseDelayActive = true;
    }
    sysml_state_previousPressed = io_0;
    int sysml_value_0 = 0;
    if (io_0) {
        sysml_conditional_1 = 440;
    }
    else {
        if (sysml_state_releaseDelayActive) {
            sysml_conditional_2 = 0;
        }
        else {
            sysml_conditional_2 = 0;
        }
        sysml_conditional_1 = sysml_conditional_2;
    }
    sysml_value_0 = sysml_conditional_1;
    *io_1 = sysml_value_0;
}