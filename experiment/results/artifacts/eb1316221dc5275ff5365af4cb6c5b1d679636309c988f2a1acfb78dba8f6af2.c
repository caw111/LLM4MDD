#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, bool* io_2, bool* io_3, bool* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_value_0 = 0;
    bool sysml_conditional_0 = 0;
    bool sysml_conditional_1 = 0;
    bool sysml_conditional_2 = 0;
    if (io_1) {
        sysml_conditional_0 = io_0 < 50.0;
    }
    else {
        sysml_conditional_0 = false;
    }
    sysml_value_0 = sysml_conditional_0;
    bool sysml_value_1 = 0;
    if (io_1) {
        sysml_conditional_1 = io_0 >= 50.0;
    }
    else {
        sysml_conditional_1 = false;
    }
    sysml_value_1 = sysml_conditional_1;
    bool sysml_value_2 = 0;
    if (io_1) {
        sysml_conditional_2 = false;
    }
    else {
        sysml_conditional_2 = false;
    }
    sysml_value_2 = sysml_conditional_2;
    *io_2 = sysml_value_0;
    *io_3 = sysml_value_1;
    *io_4 = sysml_value_2;
}