#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_mappedTens = 0;
/* Global Variable define of  */
int sysml_state_mappedUnits = 0;
/* Global Variable define of  */
int sysml_state_sumDigits = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_mappedTens = 0;
    sysml_state_mappedUnits = 0;
    sysml_state_sumDigits = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, int* io_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    int sysml_arithmetic_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_arithmetic_5 = 0;
    int sysml_conditional_6 = 0;
    int sysml_arithmetic_7 = 0;
    int sysml_conditional_8 = 0;
    if ((((double)(io_0 * 9.0)) / 1023.0) < ((double)((int)(((double)(io_0 * 9.0)) / 1023.0)))) {
        sysml_conditional_0 = 1;
    }
    else {
        sysml_conditional_0 = 0;
    }
    sysml_state_mappedTens = ((int)(((double)(io_0 * 9.0)) / 1023.0)) - sysml_conditional_0;
    if ((((double)(io_1 * 9.0)) / 1023.0) < ((double)((int)(((double)(io_1 * 9.0)) / 1023.0)))) {
        sysml_conditional_1 = 1;
    }
    else {
        sysml_conditional_1 = 0;
    }
    sysml_state_mappedUnits = ((int)(((double)(io_1 * 9.0)) / 1023.0)) - sysml_conditional_1;
    if ((((double)(sysml_state_mappedTens + sysml_state_mappedUnits)) / 10.0) < ((double)((int)(((double)(sysml_state_mappedTens + sysml_state_mappedUnits)) / 10.0)))) {
        sysml_conditional_2 = 1;
    }
    else {
        sysml_conditional_2 = 0;
    }
    sysml_arithmetic_3 = ((int)(((double)(sysml_state_mappedTens + sysml_state_mappedUnits)) / 10.0)) - sysml_conditional_2;
    if ((((double)(sysml_state_mappedTens + sysml_state_mappedUnits)) / 10.0) < ((double)((int)(((double)(sysml_state_mappedTens + sysml_state_mappedUnits)) / 10.0)))) {
        sysml_conditional_4 = 1;
    }
    else {
        sysml_conditional_4 = 0;
    }
    sysml_arithmetic_5 = ((int)(((double)(sysml_state_mappedTens + sysml_state_mappedUnits)) / 10.0)) - sysml_conditional_4;
    if ((((double)(sysml_state_mappedTens + sysml_state_mappedUnits)) / 10.0) < ((double)((int)(((double)(sysml_state_mappedTens + sysml_state_mappedUnits)) / 10.0)))) {
        sysml_conditional_6 = 1;
    }
    else {
        sysml_conditional_6 = 0;
    }
    sysml_arithmetic_7 = ((int)(((double)(sysml_state_mappedTens + sysml_state_mappedUnits)) / 10.0)) - sysml_conditional_6;
    if (((sysml_state_mappedTens + sysml_state_mappedUnits) - (10.0 * sysml_arithmetic_5)) < ((double)((int)((sysml_state_mappedTens + sysml_state_mappedUnits) - (10.0 * sysml_arithmetic_7))))) {
        sysml_conditional_8 = 1;
    }
    else {
        sysml_conditional_8 = 0;
    }
    sysml_state_sumDigits = ((int)((sysml_state_mappedTens + sysml_state_mappedUnits) - (10.0 * sysml_arithmetic_3))) - sysml_conditional_8;
    int sysml_value_0 = 0;
    sysml_value_0 = sysml_state_sumDigits;
    *io_2 = sysml_value_0;
}