#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_prevMinDeg = 0.0;
/* Global Variable define of  */
double sysml_state_prevMaxDeg = 0.0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_prevMinDeg = 0.0;
    sysml_state_prevMaxDeg = 0.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, double io_2, double* io_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_conditional_0 = 0;
    double sysml_conditional_1 = 0;
    double sysml_conditional_2 = 0;
    double sysml_arithmetic_3 = 0;
    if (io_0 > 0.0) {
        sysml_conditional_0 = ((double)(io_0 * 90.0)) / 1023.0;
    }
    else {
        sysml_conditional_0 = 0.0;
    }
    sysml_state_prevMinDeg = sysml_conditional_0;
    if (io_1 > 0.0) {
        sysml_conditional_1 = 90.0 + (((double)(io_1 * 90.0)) / 1023.0);
    }
    else {
        sysml_conditional_1 = 90.0;
    }
    sysml_state_prevMaxDeg = sysml_conditional_1;
    if ((sysml_state_prevMinDeg > sysml_state_prevMaxDeg)) {
        sysml_state_prevMinDeg = sysml_state_prevMaxDeg;
        sysml_state_prevMaxDeg = sysml_state_prevMinDeg;
    }
    double sysml_value_0 = 0;
    if (sysml_state_prevMaxDeg > sysml_state_prevMinDeg) {
        sysml_arithmetic_3 = sysml_state_prevMaxDeg - sysml_state_prevMinDeg;
        sysml_conditional_2 = sysml_state_prevMinDeg + (((double)(io_2 * sysml_arithmetic_3)) / 1023.0);
    }
    else {
        sysml_conditional_2 = sysml_state_prevMinDeg;
    }
    sysml_value_0 = sysml_conditional_2;
    *io_3 = sysml_value_0;
}