#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_lastValidDistanceCm = 0.0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_lastValidDistanceCm = 0.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, int* io_2, bool* io_3, int* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    if (io_1) {
        sysml_state_lastValidDistanceCm = io_0;
    }
    int sysml_value_0 = 0;
    if (sysml_state_lastValidDistanceCm < ((double)((int)sysml_state_lastValidDistanceCm))) {
        sysml_conditional_0 = 1;
    }
    else {
        sysml_conditional_0 = 0;
    }
    sysml_value_0 = ((int)sysml_state_lastValidDistanceCm) - sysml_conditional_0;
    bool sysml_value_1 = 0;
    sysml_value_1 = (sysml_state_lastValidDistanceCm < 30.0);
    int sysml_value_2 = 0;
    if (sysml_state_lastValidDistanceCm < 8.0) {
        sysml_conditional_1 = 220;
    }
    else {
        if (sysml_state_lastValidDistanceCm < 15.0) {
            sysml_conditional_2 = 140;
        }
        else {
            sysml_conditional_2 = 0;
        }
        sysml_conditional_1 = sysml_conditional_2;
    }
    sysml_value_2 = sysml_conditional_1;
    *io_2 = sysml_value_0;
    *io_3 = sysml_value_1;
    *io_4 = sysml_value_2;
}