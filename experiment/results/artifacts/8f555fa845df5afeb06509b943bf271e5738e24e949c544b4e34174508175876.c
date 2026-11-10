#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_digit = 0;
/* Global Variable define of  */
bool sysml_state_prevBtn1 = true;
/* Global Variable define of  */
bool sysml_state_prevBtn2 = true;
/* Global Variable define of  */
bool sysml_state_prevBtn3 = true;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_digit = 0;
    sysml_state_prevBtn1 = true;
    sysml_state_prevBtn2 = true;
    sysml_state_prevBtn3 = true;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, int* io_3, double* io_4, double* io_5) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    int sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    int sysml_conditional_5 = 0;
    double sysml_conditional_6 = 0;
    double sysml_conditional_7 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_prevBtn1;
    }
    if (sysml_lazy_0) {
        if ((sysml_state_digit + 1) > 9) {
            sysml_conditional_1 = 0;
        }
        else {
            sysml_conditional_1 = sysml_state_digit + 1;
        }
        sysml_state_digit = sysml_conditional_1;
    }
    sysml_lazy_2 = io_1;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_prevBtn2;
    }
    if (sysml_lazy_2) {
        if ((sysml_state_digit + 2) > 9) {
            sysml_conditional_3 = 0;
        }
        else {
            sysml_conditional_3 = sysml_state_digit + 2;
        }
        sysml_state_digit = sysml_conditional_3;
    }
    sysml_lazy_4 = io_2;
    if (sysml_lazy_4) {
        sysml_lazy_4 = !sysml_state_prevBtn3;
    }
    if (sysml_lazy_4) {
        if ((sysml_state_digit + 3) > 9) {
            sysml_conditional_5 = 0;
        }
        else {
            sysml_conditional_5 = sysml_state_digit + 3;
        }
        sysml_state_digit = sysml_conditional_5;
    }
    sysml_state_prevBtn1 = io_0;
    sysml_state_prevBtn2 = io_1;
    sysml_state_prevBtn3 = io_2;
    int sysml_value_0 = 0;
    sysml_value_0 = sysml_state_digit;
    double sysml_value_1 = 0;
    if (sysml_state_digit >= 5) {
        sysml_conditional_6 = 90.0;
    }
    else {
        sysml_conditional_6 = 0.0;
    }
    sysml_value_1 = sysml_conditional_6;
    double sysml_value_2 = 0;
    if (sysml_state_digit >= 5) {
        sysml_conditional_7 = 180.0;
    }
    else {
        sysml_conditional_7 = 90.0;
    }
    sysml_value_2 = sysml_conditional_7;
    *io_3 = sysml_value_0;
    *io_4 = sysml_value_1;
    *io_5 = sysml_value_2;
}