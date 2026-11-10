#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_accumulatedMs = 0;
/* Global Variable define of  */
int sysml_state_stepIndex = 0;
/* Global Variable define of  */
bool sysml_state_direction = true;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_accumulatedMs = 0;
    sysml_state_stepIndex = 0;
    sysml_state_direction = true;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_1, double* io_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_conditional_0 = 0;
    int sysml_arithmetic_1 = 0;
    int sysml_arithmetic_2 = 0;
    sysml_state_accumulatedMs = (sysml_state_accumulatedMs + io_1);
    if ((sysml_state_accumulatedMs >= 2000)) {
        sysml_state_accumulatedMs = (sysml_state_accumulatedMs - 2000);
        if (sysml_state_direction) {
            if ((sysml_state_stepIndex >= 18)) {
                sysml_state_direction = false;
                sysml_state_stepIndex = 17;
            }
            if ((sysml_state_stepIndex < 18)) {
                sysml_state_stepIndex = (sysml_state_stepIndex + 1);
            }
        }
        else {
            if ((sysml_state_stepIndex <= 0)) {
                sysml_state_direction = true;
                sysml_state_stepIndex = 1;
            }
            if ((sysml_state_stepIndex > 0)) {
                sysml_state_stepIndex = (sysml_state_stepIndex - 1);
            }
        }
    }
    double sysml_value_0 = 0;
    if (sysml_state_direction) {
        sysml_arithmetic_1 = 2 * sysml_state_stepIndex;
        sysml_conditional_0 = 5.0 * sysml_arithmetic_1;
    }
    else {
        sysml_arithmetic_2 = 2 * sysml_state_stepIndex;
        sysml_conditional_0 = 5.0 * sysml_arithmetic_2;
    }
    sysml_value_0 = sysml_conditional_0;
    *io_0 = sysml_value_0;
}