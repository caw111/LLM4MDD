#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_elapsedAccumMs = 0;
/* Global Variable define of  */
int sysml_state_displayNumber = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_elapsedAccumMs = 0;
    sysml_state_displayNumber = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_4, int* io_0, double* io_1, double* io_2, double* io_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    if ((io_4 >= 2000)) {
        sysml_state_elapsedAccumMs = ((sysml_state_elapsedAccumMs + io_4) - 2000);
        if (sysml_state_displayNumber >= 99) {
            sysml_conditional_0 = 0;
        }
        else {
            sysml_conditional_0 = sysml_state_displayNumber + 1;
        }
        sysml_state_displayNumber = sysml_conditional_0;
    }
    else {
        sysml_state_elapsedAccumMs = (sysml_state_elapsedAccumMs + io_4);
    }
    int sysml_value_0 = 0;
    sysml_value_0 = sysml_state_displayNumber;
    double sysml_value_1 = 0;
    sysml_value_1 = (sysml_state_displayNumber * 1.8);
    double sysml_value_2 = 0;
    sysml_value_2 = (sysml_state_displayNumber * 1.2);
    double sysml_value_3 = 0;
    sysml_value_3 = (sysml_state_displayNumber * 0.6);
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
}