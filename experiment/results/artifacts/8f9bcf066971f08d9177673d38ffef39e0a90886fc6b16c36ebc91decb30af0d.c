#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool* io_1, int* io_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_value_0 = 0;
    int sysml_conditional_0 = 0;
    sysml_value_0 = (io_0 >= 512.0);
    int sysml_value_1 = 0;
    if ((((double)(io_0 * 7.0)) / 1023.0) < ((double)((int)(((double)(io_0 * 7.0)) / 1023.0)))) {
        sysml_conditional_0 = 1;
    }
    else {
        sysml_conditional_0 = 0;
    }
    sysml_value_1 = ((int)(((double)(io_0 * 7.0)) / 1023.0)) - sysml_conditional_0;
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
}