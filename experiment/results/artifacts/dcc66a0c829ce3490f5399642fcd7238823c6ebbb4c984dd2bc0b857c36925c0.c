#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousPressed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, double* io_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_conditional_0 = 0;
    int sysml_conditional_1 = 0;
    if (io_1) {
        sysml_state_previousPressed = io_1;
    }
    else {
        sysml_state_previousPressed = false;
    }
    double sysml_value_0 = 0;
    if (io_1) {
        if ((((double)(io_0 * 255.0)) / 1023.0) < ((double)((int)(((double)(io_0 * 255.0)) / 1023.0)))) {
            sysml_conditional_1 = 1;
        }
        else {
            sysml_conditional_1 = 0;
        }
        sysml_conditional_0 = ((int)(((double)(io_0 * 255.0)) / 1023.0)) - sysml_conditional_1;
    }
    else {
        sysml_conditional_0 = 0.0;
    }
    sysml_value_0 = sysml_conditional_0;
    *io_2 = sysml_value_0;
}