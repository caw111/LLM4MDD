#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, bool io_3, int* io_4, int* io_5, int* io_6, int* io_7) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_value_0 = 0;
    int sysml_conditional_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    if (io_0) {
        sysml_conditional_0 = 262;
    }
    else {
        sysml_conditional_0 = 0;
    }
    sysml_value_0 = sysml_conditional_0;
    int sysml_value_1 = 0;
    if (io_1) {
        sysml_conditional_1 = 294;
    }
    else {
        sysml_conditional_1 = 0;
    }
    sysml_value_1 = sysml_conditional_1;
    int sysml_value_2 = 0;
    if (io_2) {
        sysml_conditional_2 = 330;
    }
    else {
        sysml_conditional_2 = 0;
    }
    sysml_value_2 = sysml_conditional_2;
    int sysml_value_3 = 0;
    if (io_3) {
        sysml_conditional_3 = 349;
    }
    else {
        sysml_conditional_3 = 0;
    }
    sysml_value_3 = sysml_conditional_3;
    *io_4 = sysml_value_0;
    *io_5 = sysml_value_1;
    *io_6 = sysml_value_2;
    *io_7 = sysml_value_3;
}