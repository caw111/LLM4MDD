#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, int* io_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_value_0 = 0;
    int sysml_conditional_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    if (io_0 <= 0.0) {
        sysml_conditional_0 = 0;
    }
    else {
        if (io_0 > 1023.0) {
            sysml_conditional_1 = 2000;
        }
        else {
            if (((((double)io_0) / 1023.0) * 1999) < ((double)((int)((((double)io_0) / 1023.0) * 1999)))) {
                sysml_conditional_2 = 1;
            }
            else {
                sysml_conditional_2 = 0;
            }
            sysml_conditional_1 = ((int)((((double)io_0) / 1023.0) * 1999)) - sysml_conditional_2;
        }
        sysml_conditional_0 = sysml_conditional_1;
    }
    sysml_value_0 = sysml_conditional_0;
    *io_1 = sysml_value_0;
}