#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_colorIndex = 0;
/* Global Variable define of  */
int sysml_state_segValue = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_colorIndex = 0;
    sysml_state_segValue = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double* io_0, double* io_1, double* io_2, double* io_3, double* io_4, double* io_5, int* io_6) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_value_0 = 0;
    double sysml_conditional_0 = 0;
    double sysml_conditional_1 = 0;
    double sysml_conditional_2 = 0;
    double sysml_conditional_3 = 0;
    double sysml_conditional_4 = 0;
    double sysml_conditional_5 = 0;
    if (sysml_state_colorIndex == 0) {
        sysml_conditional_0 = 255.0;
    }
    else {
        sysml_conditional_0 = 0.0;
    }
    sysml_value_0 = sysml_conditional_0;
    double sysml_value_1 = 0;
    if (sysml_state_colorIndex == 0) {
        sysml_conditional_1 = 0.0;
    }
    else {
        sysml_conditional_1 = 0.0;
    }
    sysml_value_1 = sysml_conditional_1;
    double sysml_value_2 = 0;
    if (sysml_state_colorIndex == 0) {
        sysml_conditional_2 = 0.0;
    }
    else {
        sysml_conditional_2 = 0.0;
    }
    sysml_value_2 = sysml_conditional_2;
    double sysml_value_3 = 0;
    if (sysml_state_colorIndex == 0) {
        sysml_conditional_3 = 255.0;
    }
    else {
        sysml_conditional_3 = 0.0;
    }
    sysml_value_3 = sysml_conditional_3;
    double sysml_value_4 = 0;
    if (sysml_state_colorIndex == 0) {
        sysml_conditional_4 = 0.0;
    }
    else {
        sysml_conditional_4 = 0.0;
    }
    sysml_value_4 = sysml_conditional_4;
    double sysml_value_5 = 0;
    if (sysml_state_colorIndex == 0) {
        sysml_conditional_5 = 0.0;
    }
    else {
        sysml_conditional_5 = 0.0;
    }
    sysml_value_5 = sysml_conditional_5;
    int sysml_value_6 = 0;
    sysml_value_6 = sysml_state_segValue;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
    *io_4 = sysml_value_4;
    *io_5 = sysml_value_5;
    *io_6 = sysml_value_6;
}