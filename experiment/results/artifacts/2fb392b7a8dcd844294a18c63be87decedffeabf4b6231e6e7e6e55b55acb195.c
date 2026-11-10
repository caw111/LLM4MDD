#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_phaseIsRed = true;
/* Global Variable define of  */
int sysml_state_elapsedInPhaseMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_phaseIsRed = true;
    sysml_state_elapsedInPhaseMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool* io_0, bool* io_1, bool* io_2, bool* io_3, bool* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_conditional_0 = 0;
    bool sysml_conditional_1 = 0;
    bool sysml_conditional_2 = 0;
    if ((sysml_state_elapsedInPhaseMs >= 2000)) {
        sysml_state_phaseIsRed = (!sysml_state_phaseIsRed);
        sysml_state_elapsedInPhaseMs = 0;
    }
    else {
        sysml_state_elapsedInPhaseMs = (sysml_state_elapsedInPhaseMs + 0);
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_phaseIsRed;
    bool sysml_value_1 = 0;
    if (sysml_state_phaseIsRed) {
        sysml_conditional_0 = true;
    }
    else {
        sysml_conditional_0 = false;
    }
    sysml_value_1 = sysml_conditional_0;
    bool sysml_value_2 = 0;
    sysml_value_2 = false;
    bool sysml_value_3 = 0;
    if (sysml_state_phaseIsRed) {
        sysml_conditional_1 = false;
    }
    else {
        sysml_conditional_1 = true;
    }
    sysml_value_3 = sysml_conditional_1;
    bool sysml_value_4 = 0;
    if (sysml_state_phaseIsRed) {
        sysml_conditional_2 = false;
    }
    else {
        sysml_conditional_2 = true;
    }
    sysml_value_4 = sysml_conditional_2;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
    *io_4 = sysml_value_4;
}