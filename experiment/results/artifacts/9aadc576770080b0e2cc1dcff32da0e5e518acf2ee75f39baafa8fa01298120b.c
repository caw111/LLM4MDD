#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_servoPhaseMs = 0;
/* Global Variable define of  */
int sysml_state_servoPhaseIndex = 0;
/* Global Variable define of  */
int sysml_state_ledPhaseMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_servoPhaseMs = 0;
    sysml_state_servoPhaseIndex = 0;
    sysml_state_ledPhaseMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_5, bool* io_0, bool* io_1, double* io_2, double* io_3, double* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_conditional_0 = 0;
    bool sysml_conditional_1 = 0;
    double sysml_conditional_2 = 0;
    double sysml_conditional_3 = 0;
    double sysml_conditional_4 = 0;
    sysml_state_servoPhaseMs = (sysml_state_servoPhaseMs + io_5);
    sysml_state_ledPhaseMs = (sysml_state_ledPhaseMs + io_5);
    if ((sysml_state_servoPhaseMs >= 2000)) {
        sysml_state_servoPhaseMs = (sysml_state_servoPhaseMs - 2000);
        sysml_state_servoPhaseIndex = (sysml_state_servoPhaseIndex + 1);
    }
    if ((sysml_state_servoPhaseIndex >= 2)) {
        sysml_state_servoPhaseIndex = (sysml_state_servoPhaseIndex - 2);
    }
    if ((sysml_state_ledPhaseMs >= 2000)) {
        sysml_state_ledPhaseMs = (sysml_state_ledPhaseMs - 2000);
    }
    bool sysml_value_0 = 0;
    if (sysml_state_ledPhaseMs < 1000) {
        sysml_conditional_0 = false;
    }
    else {
        sysml_conditional_0 = true;
    }
    sysml_value_0 = sysml_conditional_0;
    bool sysml_value_1 = 0;
    if (sysml_state_ledPhaseMs < 1000) {
        sysml_conditional_1 = true;
    }
    else {
        sysml_conditional_1 = false;
    }
    sysml_value_1 = sysml_conditional_1;
    double sysml_value_2 = 0;
    if (sysml_state_servoPhaseIndex == 0) {
        sysml_conditional_2 = 0.0;
    }
    else {
        sysml_conditional_2 = 180.0;
    }
    sysml_value_2 = sysml_conditional_2;
    double sysml_value_3 = 0;
    if (sysml_state_servoPhaseIndex == 0) {
        sysml_conditional_3 = 90.0;
    }
    else {
        sysml_conditional_3 = 0.0;
    }
    sysml_value_3 = sysml_conditional_3;
    double sysml_value_4 = 0;
    if (sysml_state_servoPhaseIndex == 0) {
        sysml_conditional_4 = 180.0;
    }
    else {
        sysml_conditional_4 = 90.0;
    }
    sysml_value_4 = sysml_conditional_4;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
    *io_4 = sysml_value_4;
}