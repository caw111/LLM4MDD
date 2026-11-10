#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_elapsedAccumMs = 0;
/* Global Variable define of  */
int sysml_state_displayedNumber = 0;
/* Global Variable define of  */
int sysml_state_tensDigit = 0;
/* Global Variable define of  */
int sysml_state_unitsDigit = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_elapsedAccumMs = 0;
    sysml_state_displayedNumber = 0;
    sysml_state_tensDigit = 0;
    sysml_state_unitsDigit = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_7, bool* io_0, bool* io_1, bool* io_2, int* io_3, double* io_4, double* io_5, double* io_6) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_arithmetic_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_conditional_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_conditional_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_conditional_8 = 0;
    double sysml_conditional_9 = 0;
    bool sysml_lazy_10 = 0;
    double sysml_conditional_11 = 0;
    double sysml_conditional_12 = 0;
    bool sysml_lazy_13 = 0;
    double sysml_conditional_14 = 0;
    double sysml_conditional_15 = 0;
    bool sysml_lazy_16 = 0;
    double sysml_conditional_17 = 0;
    sysml_state_elapsedAccumMs = (sysml_state_elapsedAccumMs + io_7);
    if ((sysml_state_elapsedAccumMs >= 2000)) {
        sysml_state_elapsedAccumMs = (sysml_state_elapsedAccumMs - 2000);
        sysml_state_displayedNumber = (sysml_state_displayedNumber + 1);
        if ((sysml_state_displayedNumber > 99)) {
            sysml_state_displayedNumber = 0;
        }
    }
    if ((((double)sysml_state_displayedNumber) / 10.0) < ((double)((int)(((double)sysml_state_displayedNumber) / 10.0)))) {
        sysml_conditional_0 = 1;
    }
    else {
        sysml_conditional_0 = 0;
    }
    sysml_state_tensDigit = ((int)(((double)sysml_state_displayedNumber) / 10.0)) - sysml_conditional_0;
    if ((((double)sysml_state_displayedNumber) / 10.0) < ((double)((int)(((double)sysml_state_displayedNumber) / 10.0)))) {
        sysml_conditional_1 = 1;
    }
    else {
        sysml_conditional_1 = 0;
    }
    sysml_arithmetic_2 = ((int)(((double)sysml_state_displayedNumber) / 10.0)) - sysml_conditional_1;
    sysml_state_unitsDigit = sysml_state_displayedNumber - (10 * sysml_arithmetic_2);
    bool sysml_value_0 = 0;
    sysml_lazy_3 = sysml_state_unitsDigit >= 0;
    if (sysml_lazy_3) {
        sysml_lazy_3 = sysml_state_unitsDigit <= 2;
    }
    if (sysml_lazy_3) {
        sysml_conditional_4 = true;
    }
    else {
        sysml_conditional_4 = false;
    }
    sysml_value_0 = sysml_conditional_4;
    bool sysml_value_1 = 0;
    sysml_lazy_5 = sysml_state_unitsDigit >= 3;
    if (sysml_lazy_5) {
        sysml_lazy_5 = sysml_state_unitsDigit <= 5;
    }
    if (sysml_lazy_5) {
        sysml_conditional_6 = true;
    }
    else {
        sysml_conditional_6 = false;
    }
    sysml_value_1 = sysml_conditional_6;
    bool sysml_value_2 = 0;
    sysml_lazy_7 = sysml_state_unitsDigit >= 6;
    if (sysml_lazy_7) {
        sysml_lazy_7 = sysml_state_unitsDigit <= 9;
    }
    if (sysml_lazy_7) {
        sysml_conditional_8 = true;
    }
    else {
        sysml_conditional_8 = false;
    }
    sysml_value_2 = sysml_conditional_8;
    int sysml_value_3 = 0;
    sysml_value_3 = sysml_state_displayedNumber;
    double sysml_value_4 = 0;
    if (sysml_state_tensDigit <= 2) {
        sysml_conditional_9 = 30.0;
    }
    else {
        sysml_lazy_10 = sysml_state_tensDigit >= 3;
        if (sysml_lazy_10) {
            sysml_lazy_10 = sysml_state_tensDigit <= 5;
        }
        if (sysml_lazy_10) {
            sysml_conditional_11 = 60.0;
        }
        else {
            sysml_conditional_11 = 90.0;
        }
        sysml_conditional_9 = sysml_conditional_11;
    }
    sysml_value_4 = sysml_conditional_9;
    double sysml_value_5 = 0;
    if (sysml_state_tensDigit <= 2) {
        sysml_conditional_12 = 60.0;
    }
    else {
        sysml_lazy_13 = sysml_state_tensDigit >= 3;
        if (sysml_lazy_13) {
            sysml_lazy_13 = sysml_state_tensDigit <= 5;
        }
        if (sysml_lazy_13) {
            sysml_conditional_14 = 90.0;
        }
        else {
            sysml_conditional_14 = 120.0;
        }
        sysml_conditional_12 = sysml_conditional_14;
    }
    sysml_value_5 = sysml_conditional_12;
    double sysml_value_6 = 0;
    if (sysml_state_tensDigit <= 2) {
        sysml_conditional_15 = 90.0;
    }
    else {
        sysml_lazy_16 = sysml_state_tensDigit >= 3;
        if (sysml_lazy_16) {
            sysml_lazy_16 = sysml_state_tensDigit <= 5;
        }
        if (sysml_lazy_16) {
            sysml_conditional_17 = 120.0;
        }
        else {
            sysml_conditional_17 = 150.0;
        }
        sysml_conditional_15 = sysml_conditional_17;
    }
    sysml_value_6 = sysml_conditional_15;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
    *io_4 = sysml_value_4;
    *io_5 = sysml_value_5;
    *io_6 = sysml_value_6;
}