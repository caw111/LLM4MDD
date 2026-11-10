#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_countSeconds = 0;
/* Global Variable define of  */
int sysml_state_accumulatedMs = 0;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_countSeconds = 0;
    sysml_state_accumulatedMs = 0;
    sysml_state_previousPressed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, int elapsedMilliseconds, int* io_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        sysml_state_countSeconds = 0;
        sysml_state_accumulatedMs = 0;
    }
    sysml_lazy_1 = io_0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_previousPressed;
    }
    if (!sysml_lazy_1) {
        sysml_state_accumulatedMs = (sysml_state_accumulatedMs + elapsedMilliseconds);
        if ((sysml_state_accumulatedMs >= 1000)) {
            sysml_state_accumulatedMs = (sysml_state_accumulatedMs - 1000);
            sysml_state_countSeconds = (sysml_state_countSeconds + 1);
        }
    }
    sysml_state_previousPressed = io_0;
    int sysml_value_0 = 0;
    sysml_value_0 = sysml_state_countSeconds;
    *io_1 = sysml_value_0;
}