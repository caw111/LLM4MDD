#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_number = 0;
/* Global Variable define of  */
bool sysml_state_prevBtn1 = false;
/* Global Variable define of  */
bool sysml_state_prevBtn2 = false;
/* Global Variable define of  */
bool sysml_state_prevBtn3 = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_number = 0;
    sysml_state_prevBtn1 = false;
    sysml_state_prevBtn2 = false;
    sysml_state_prevBtn3 = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_3, bool io_4, bool io_5, int* io_0, double* io_1, double* io_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    int sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    int sysml_conditional_5 = 0;
    double sysml_conditional_6 = 0;
    double sysml_conditional_7 = 0;
    sysml_lazy_0 = io_3;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_prevBtn1;
    }
    if (sysml_lazy_0) {
        if ((sysml_state_number + 1) >= 10) {
            sysml_conditional_1 = 0;
        }
        else {
            sysml_conditional_1 = sysml_state_number + 1;
        }
        sysml_state_number = sysml_conditional_1;
    }
    sysml_lazy_2 = io_4;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_prevBtn2;
    }
    if (sysml_lazy_2) {
        if ((sysml_state_number + 2) >= 10) {
            sysml_conditional_3 = 0;
        }
        else {
            sysml_conditional_3 = sysml_state_number + 2;
        }
        sysml_state_number = sysml_conditional_3;
    }
    sysml_lazy_4 = io_5;
    if (sysml_lazy_4) {
        sysml_lazy_4 = !sysml_state_prevBtn3;
    }
    if (sysml_lazy_4) {
        if ((sysml_state_number + 3) >= 10) {
            sysml_conditional_5 = 0;
        }
        else {
            sysml_conditional_5 = sysml_state_number + 3;
        }
        sysml_state_number = sysml_conditional_5;
    }
    sysml_state_prevBtn1 = io_3;
    sysml_state_prevBtn2 = io_4;
    sysml_state_prevBtn3 = io_5;
    int sysml_value_0 = 0;
    sysml_value_0 = sysml_state_number;
    double sysml_value_1 = 0;
    if (sysml_state_number >= 5) {
        sysml_conditional_6 = 90.0;
    }
    else {
        sysml_conditional_6 = 0.0;
    }
    sysml_value_1 = sysml_conditional_6;
    double sysml_value_2 = 0;
    if (sysml_state_number >= 5) {
        sysml_conditional_7 = 180.0;
    }
    else {
        sysml_conditional_7 = 90.0;
    }
    sysml_value_2 = sysml_conditional_7;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
}