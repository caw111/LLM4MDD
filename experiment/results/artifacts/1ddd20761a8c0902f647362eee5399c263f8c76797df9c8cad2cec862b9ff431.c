#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_displayedValue = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_displayedValue = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_0, bool io_0_valid, bool* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    int sysml_conditional_2 = 0;
    int sysml_arithmetic_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    int sysml_arithmetic_6 = 0;
    int sysml_conditional_7 = 0;
    int sysml_conditional_8 = 0;
    int sysml_arithmetic_9 = 0;
    int sysml_conditional_10 = 0;
    int sysml_conditional_11 = 0;
    int sysml_arithmetic_12 = 0;
    sysml_lazy_0 = io_0_valid;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_0 >= 0;
    }
    sysml_lazy_1 = sysml_lazy_0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = io_0 <= 31;
    }
    if (sysml_lazy_1) {
        sysml_state_displayedValue = io_0;
    }
    bool sysml_value_0 = 0;
    if ((((double)sysml_state_displayedValue) / 2) < ((double)((int)(((double)sysml_state_displayedValue) / 2)))) {
        sysml_conditional_2 = 1;
    }
    else {
        sysml_conditional_2 = 0;
    }
    sysml_arithmetic_3 = ((int)(((double)sysml_state_displayedValue) / 2)) - sysml_conditional_2;
    sysml_value_0 = sysml_state_displayedValue != (2 * sysml_arithmetic_3);
    bool sysml_value_1 = 0;
    if ((((double)sysml_state_displayedValue) / 2) < ((double)((int)(((double)sysml_state_displayedValue) / 2)))) {
        sysml_conditional_4 = 1;
    }
    else {
        sysml_conditional_4 = 0;
    }
    if ((((double)sysml_state_displayedValue) / 4) < ((double)((int)(((double)sysml_state_displayedValue) / 4)))) {
        sysml_conditional_5 = 1;
    }
    else {
        sysml_conditional_5 = 0;
    }
    sysml_arithmetic_6 = ((int)(((double)sysml_state_displayedValue) / 4)) - sysml_conditional_5;
    sysml_value_1 = (((int)(((double)sysml_state_displayedValue) / 2)) - sysml_conditional_4) != (2 * sysml_arithmetic_6);
    bool sysml_value_2 = 0;
    if ((((double)sysml_state_displayedValue) / 4) < ((double)((int)(((double)sysml_state_displayedValue) / 4)))) {
        sysml_conditional_7 = 1;
    }
    else {
        sysml_conditional_7 = 0;
    }
    if ((((double)sysml_state_displayedValue) / 8) < ((double)((int)(((double)sysml_state_displayedValue) / 8)))) {
        sysml_conditional_8 = 1;
    }
    else {
        sysml_conditional_8 = 0;
    }
    sysml_arithmetic_9 = ((int)(((double)sysml_state_displayedValue) / 8)) - sysml_conditional_8;
    sysml_value_2 = (((int)(((double)sysml_state_displayedValue) / 4)) - sysml_conditional_7) != (2 * sysml_arithmetic_9);
    bool sysml_value_3 = 0;
    if ((((double)sysml_state_displayedValue) / 8) < ((double)((int)(((double)sysml_state_displayedValue) / 8)))) {
        sysml_conditional_10 = 1;
    }
    else {
        sysml_conditional_10 = 0;
    }
    if ((((double)sysml_state_displayedValue) / 16) < ((double)((int)(((double)sysml_state_displayedValue) / 16)))) {
        sysml_conditional_11 = 1;
    }
    else {
        sysml_conditional_11 = 0;
    }
    sysml_arithmetic_12 = ((int)(((double)sysml_state_displayedValue) / 16)) - sysml_conditional_11;
    sysml_value_3 = (((int)(((double)sysml_state_displayedValue) / 8)) - sysml_conditional_10) != (2 * sysml_arithmetic_12);
    bool sysml_value_4 = 0;
    sysml_value_4 = (sysml_state_displayedValue >= 16);
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
    *io_3 = sysml_value_2;
    *io_4 = sysml_value_3;
    *io_5 = sysml_value_4;
}