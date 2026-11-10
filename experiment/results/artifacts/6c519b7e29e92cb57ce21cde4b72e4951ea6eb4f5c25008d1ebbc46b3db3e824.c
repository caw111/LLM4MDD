#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_prevAngle = 0.0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_prevAngle = 0.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_3, bool* io_0, bool* io_1, double* io_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_conditional_0 = 0;
    double sysml_conditional_1 = 0;
    bool sysml_conditional_2 = 0;
    double sysml_conditional_3 = 0;
    bool sysml_conditional_4 = 0;
    double sysml_conditional_5 = 0;
    double sysml_conditional_6 = 0;
    double sysml_conditional_7 = 0;
    if ((io_3 * 180.0) >= 1023.0) {
        sysml_conditional_0 = 180.0;
    }
    else {
        sysml_conditional_0 = ((double)(io_3 * 180.0)) / 1023.0;
    }
    sysml_state_prevAngle = sysml_conditional_0;
    bool sysml_value_0 = 0;
    if ((io_3 * 180.0) >= 1023.0) {
        sysml_conditional_1 = 180.0;
    }
    else {
        sysml_conditional_1 = ((double)(io_3 * 180.0)) / 1023.0;
    }
    if (sysml_conditional_1 > sysml_state_prevAngle) {
        sysml_conditional_2 = true;
    }
    else {
        sysml_conditional_2 = false;
    }
    sysml_value_0 = sysml_conditional_2;
    bool sysml_value_1 = 0;
    if ((io_3 * 180.0) >= 1023.0) {
        sysml_conditional_3 = 180.0;
    }
    else {
        sysml_conditional_3 = ((double)(io_3 * 180.0)) / 1023.0;
    }
    if (sysml_conditional_3 < sysml_state_prevAngle) {
        sysml_conditional_4 = true;
    }
    else {
        sysml_conditional_4 = false;
    }
    sysml_value_1 = sysml_conditional_4;
    double sysml_value_2 = 0;
    if (io_3 >= 1023.0) {
        sysml_conditional_5 = 180.0;
    }
    else {
        sysml_conditional_5 = ((double)(io_3 * 180.0)) / 1023.0;
    }
    if (sysml_conditional_5 >= 0.0) {
        if (io_3 >= 1023.0) {
            sysml_conditional_7 = 180.0;
        }
        else {
            sysml_conditional_7 = ((double)(io_3 * 180.0)) / 1023.0;
        }
        sysml_conditional_6 = sysml_conditional_7;
    }
    else {
        sysml_conditional_6 = 0.0;
    }
    sysml_value_2 = sysml_conditional_6;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
}