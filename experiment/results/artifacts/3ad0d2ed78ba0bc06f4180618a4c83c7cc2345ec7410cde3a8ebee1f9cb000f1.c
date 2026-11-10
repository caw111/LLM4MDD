#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_buzzActive = false;
/* Global Variable define of  */
int sysml_state_buzzElapsedMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_buzzActive = false;
    sysml_state_buzzElapsedMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_1, bool io_1_valid, int* io_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int sysml_conditional_1 = 0;
    sysml_lazy_0 = io_1_valid;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_1 == 1;
    }
    if (sysml_lazy_0) {
        sysml_state_buzzActive = true;
        sysml_state_buzzElapsedMs = 0;
    }
    if (sysml_state_buzzActive) {
        sysml_state_buzzElapsedMs = sysml_state_buzzElapsedMs;
        if ((sysml_state_buzzElapsedMs >= 500)) {
            sysml_state_buzzActive = false;
        }
    }
    int sysml_value_0 = 0;
    if (sysml_state_buzzActive) {
        sysml_conditional_1 = 500;
    }
    else {
        sysml_conditional_1 = 0;
    }
    sysml_value_0 = sysml_conditional_1;
    *io_0 = sysml_value_0;
}