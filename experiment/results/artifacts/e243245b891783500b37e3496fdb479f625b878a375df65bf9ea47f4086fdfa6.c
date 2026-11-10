#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_targetCm = 400;
/* Global Variable define of  */
int sysml_state_lastDistanceCm = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_targetCm = 400;
    sysml_state_lastDistanceCm = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, int io_2, bool io_2_valid, bool* serial_0_trigger, int* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    bool sysml_lazy_1 = 0;
    int sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    if (io_2_valid) {
        sysml_state_targetCm = io_2;
    }
    if (io_1) {
        if (io_0 < ((double)((int)io_0))) {
            sysml_conditional_0 = 1;
        }
        else {
            sysml_conditional_0 = 0;
        }
        sysml_state_lastDistanceCm = ((int)io_0) - sysml_conditional_0;
    }
    bool sysml_value_0 = 0;
    sysml_lazy_1 = io_1;
    if (sysml_lazy_1) {
        sysml_lazy_1 = io_2_valid;
    }
    sysml_value_0 = sysml_lazy_1;
    int sysml_value_1 = 0;
    if (sysml_state_lastDistanceCm > sysml_state_targetCm) {
        sysml_conditional_2 = 1;
    }
    else {
        if (sysml_state_lastDistanceCm == sysml_state_targetCm) {
            sysml_conditional_3 = 2;
        }
        else {
            sysml_conditional_3 = 3;
        }
        sysml_conditional_2 = sysml_conditional_3;
    }
    sysml_value_1 = sysml_conditional_2;
    *serial_0_trigger = sysml_value_0;
    *serial_0_value_0 = sysml_value_1;
}