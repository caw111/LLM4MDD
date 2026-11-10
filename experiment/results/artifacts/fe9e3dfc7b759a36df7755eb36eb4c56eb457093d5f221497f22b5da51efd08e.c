#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_timerSeconds = 0;
/* Global Variable define of  */
int sysml_state_accumulatedMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_timerSeconds = 0;
    sysml_state_accumulatedMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_0, int* io_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_arithmetic_2 = 0;
    if (true) {
        sysml_state_accumulatedMs = (sysml_state_accumulatedMs + io_0);
    }
    if ((sysml_state_accumulatedMs >= 1000)) {
        sysml_state_accumulatedMs = (sysml_state_accumulatedMs - 1000);
        sysml_state_timerSeconds = (sysml_state_timerSeconds + 2);
    }
    int sysml_value_0 = 0;
    if (sysml_state_timerSeconds <= 10) {
        sysml_conditional_0 = sysml_state_timerSeconds;
    }
    else {
        if ((((double)(sysml_state_timerSeconds - 10)) / 10) < ((double)((int)(((double)(sysml_state_timerSeconds - 10)) / 10)))) {
            sysml_conditional_1 = 1;
        }
        else {
            sysml_conditional_1 = 0;
        }
        sysml_arithmetic_2 = ((int)(((double)(sysml_state_timerSeconds - 10)) / 10)) - sysml_conditional_1;
        sysml_conditional_0 = (sysml_state_timerSeconds - 10) - (10 * sysml_arithmetic_2);
    }
    sysml_value_0 = sysml_conditional_0;
    *io_1 = sysml_value_0;
}