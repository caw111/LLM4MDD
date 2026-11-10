#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_thresholdDegC = 0.0;
/* Global Variable define of  */
bool sysml_state_prevTempValid = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_thresholdDegC = 0.0;
    sysml_state_prevTempValid = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_3, double io_4, bool io_5, bool* io_1, int* io_2, int* io_6) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_conditional_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    bool sysml_lazy_3 = 0;
    int sysml_conditional_4 = 0;
    sysml_state_thresholdDegC = io_0;
    if (io_5) {
        sysml_state_prevTempValid = true;
    }
    else {
        sysml_state_prevTempValid = false;
    }
    bool sysml_value_0 = 0;
    if (io_5) {
        sysml_conditional_0 = io_3 > sysml_state_thresholdDegC;
    }
    else {
        sysml_conditional_0 = false;
    }
    sysml_value_0 = sysml_conditional_0;
    int sysml_value_1 = 0;
    if (io_5) {
        if (io_3 > sysml_state_thresholdDegC) {
            sysml_conditional_2 = 1000;
        }
        else {
            sysml_conditional_2 = 0;
        }
        sysml_conditional_1 = sysml_conditional_2;
    }
    else {
        sysml_conditional_1 = 0;
    }
    sysml_value_1 = sysml_conditional_1;
    int sysml_value_2 = 0;
    sysml_lazy_3 = io_5;
    if (sysml_lazy_3) {
        sysml_lazy_3 = io_3 > sysml_state_thresholdDegC;
    }
    if (sysml_lazy_3) {
        sysml_conditional_4 = 1;
    }
    else {
        sysml_conditional_4 = 0;
    }
    sysml_value_2 = sysml_conditional_4;
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
    *io_6 = sysml_value_2;
}