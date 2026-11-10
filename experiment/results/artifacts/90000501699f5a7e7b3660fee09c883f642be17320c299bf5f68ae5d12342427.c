#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_fanLevel = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_fanLevel = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double* io_1, int* io_2, int* io_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_conditional_0 = 0;
    double sysml_conditional_1 = 0;
    double sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    if ((io_0 > 300.0)) {
        sysml_state_fanLevel = 3;
    }
    else {
        if ((io_0 > 200.0)) {
            sysml_state_fanLevel = 2;
        }
        else {
            if ((io_0 > 100.0)) {
                sysml_state_fanLevel = 1;
            }
            else {
                sysml_state_fanLevel = 0;
            }
        }
    }
    double sysml_value_0 = 0;
    if (sysml_state_fanLevel == 0) {
        sysml_conditional_0 = 0.0;
    }
    else {
        if (sysml_state_fanLevel == 1) {
            sysml_conditional_1 = 85.0;
        }
        else {
            if (sysml_state_fanLevel == 2) {
                sysml_conditional_2 = 170.0;
            }
            else {
                sysml_conditional_2 = 255.0;
            }
            sysml_conditional_1 = sysml_conditional_2;
        }
        sysml_conditional_0 = sysml_conditional_1;
    }
    sysml_value_0 = sysml_conditional_0;
    int sysml_value_1 = 0;
    if (sysml_state_fanLevel == 3) {
        sysml_conditional_3 = 2000;
    }
    else {
        sysml_conditional_3 = 0;
    }
    sysml_value_1 = sysml_conditional_3;
    int sysml_value_2 = 0;
    sysml_value_2 = sysml_state_fanLevel;
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
    *io_3 = sysml_value_2;
}