#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_digit = 0;
/* Global Variable define of  */
bool sysml_state_prevBtn1 = false;
/* Global Variable define of  */
bool sysml_state_prevBtn2 = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_digit = 0;
    sysml_state_prevBtn1 = false;
    sysml_state_prevBtn2 = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_1, bool io_2, int* io_0, bool* io_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_arithmetic_5 = 0;
    sysml_lazy_0 = io_1;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_prevBtn1;
    }
    if (sysml_lazy_0) {
        if (sysml_state_digit == 9) {
            sysml_conditional_1 = 0;
        }
        else {
            sysml_conditional_1 = sysml_state_digit + 1;
        }
        sysml_state_digit = sysml_conditional_1;
    }
    sysml_lazy_2 = io_2;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_prevBtn2;
    }
    if (sysml_lazy_2) {
        if (sysml_state_digit == 0) {
            sysml_conditional_3 = 9;
        }
        else {
            sysml_conditional_3 = sysml_state_digit - 1;
        }
        sysml_state_digit = sysml_conditional_3;
    }
    sysml_state_prevBtn1 = io_1;
    sysml_state_prevBtn2 = io_2;
    int sysml_value_0 = 0;
    sysml_value_0 = sysml_state_digit;
    bool sysml_value_1 = 0;
    if ((((double)sysml_state_digit) / 2) < ((double)((int)(((double)sysml_state_digit) / 2)))) {
        sysml_conditional_4 = 1;
    }
    else {
        sysml_conditional_4 = 0;
    }
    sysml_arithmetic_5 = ((int)(((double)sysml_state_digit) / 2)) - sysml_conditional_4;
    sysml_value_1 = sysml_state_digit == (2 * sysml_arithmetic_5);
    *io_0 = sysml_value_0;
    *io_3 = sysml_value_1;
}