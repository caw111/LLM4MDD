#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_displayNumber = 0;
/* Global Variable define of  */
int sysml_state_accumulatedMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_displayNumber = 0;
    sysml_state_accumulatedMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_7, bool* io_0, bool* io_1, bool* io_2, int* io_3, double* io_4, double* io_5, double* io_6) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_arithmetic_2 = 0;
    bool sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_arithmetic_5 = 0;
    bool sysml_lazy_6 = 0;
    int sysml_conditional_7 = 0;
    int sysml_arithmetic_8 = 0;
    bool sysml_conditional_9 = 0;
    int sysml_conditional_10 = 0;
    int sysml_arithmetic_11 = 0;
    bool sysml_lazy_12 = 0;
    int sysml_conditional_13 = 0;
    int sysml_arithmetic_14 = 0;
    bool sysml_conditional_15 = 0;
    int sysml_conditional_16 = 0;
    double sysml_conditional_17 = 0;
    int sysml_conditional_18 = 0;
    double sysml_conditional_19 = 0;
    int sysml_conditional_20 = 0;
    double sysml_conditional_21 = 0;
    int sysml_conditional_22 = 0;
    double sysml_conditional_23 = 0;
    int sysml_conditional_24 = 0;
    double sysml_conditional_25 = 0;
    int sysml_conditional_26 = 0;
    double sysml_conditional_27 = 0;
    sysml_state_accumulatedMs = (sysml_state_accumulatedMs + io_7);
    if ((sysml_state_accumulatedMs >= 2000)) {
        sysml_state_accumulatedMs = (sysml_state_accumulatedMs - 2000);
        if ((sysml_state_displayNumber + 1) >= 100) {
            sysml_conditional_0 = 0;
        }
        else {
            sysml_conditional_0 = sysml_state_displayNumber + 1;
        }
        sysml_state_displayNumber = sysml_conditional_0;
    }
    bool sysml_value_0 = 0;
    if ((((double)sysml_state_displayNumber) / 10) < ((double)((int)(((double)sysml_state_displayNumber) / 10)))) {
        sysml_conditional_1 = 1;
    }
    else {
        sysml_conditional_1 = 0;
    }
    sysml_arithmetic_2 = ((int)(((double)sysml_state_displayNumber) / 10)) - sysml_conditional_1;
    if ((sysml_state_displayNumber - (10 * sysml_arithmetic_2)) <= 2) {
        sysml_conditional_3 = true;
    }
    else {
        sysml_conditional_3 = false;
    }
    sysml_value_0 = sysml_conditional_3;
    bool sysml_value_1 = 0;
    if ((((double)sysml_state_displayNumber) / 10) < ((double)((int)(((double)sysml_state_displayNumber) / 10)))) {
        sysml_conditional_4 = 1;
    }
    else {
        sysml_conditional_4 = 0;
    }
    sysml_arithmetic_5 = ((int)(((double)sysml_state_displayNumber) / 10)) - sysml_conditional_4;
    sysml_lazy_6 = (sysml_state_displayNumber - (10 * sysml_arithmetic_5)) >= 3;
    if (sysml_lazy_6) {
        if ((((double)sysml_state_displayNumber) / 10) < ((double)((int)(((double)sysml_state_displayNumber) / 10)))) {
            sysml_conditional_7 = 1;
        }
        else {
            sysml_conditional_7 = 0;
        }
        sysml_arithmetic_8 = ((int)(((double)sysml_state_displayNumber) / 10)) - sysml_conditional_7;
        sysml_lazy_6 = (sysml_state_displayNumber - (10 * sysml_arithmetic_8)) <= 5;
    }
    if (sysml_lazy_6) {
        sysml_conditional_9 = true;
    }
    else {
        sysml_conditional_9 = false;
    }
    sysml_value_1 = sysml_conditional_9;
    bool sysml_value_2 = 0;
    if ((((double)sysml_state_displayNumber) / 10) < ((double)((int)(((double)sysml_state_displayNumber) / 10)))) {
        sysml_conditional_10 = 1;
    }
    else {
        sysml_conditional_10 = 0;
    }
    sysml_arithmetic_11 = ((int)(((double)sysml_state_displayNumber) / 10)) - sysml_conditional_10;
    sysml_lazy_12 = (sysml_state_displayNumber - (10 * sysml_arithmetic_11)) >= 6;
    if (sysml_lazy_12) {
        if ((((double)sysml_state_displayNumber) / 10) < ((double)((int)(((double)sysml_state_displayNumber) / 10)))) {
            sysml_conditional_13 = 1;
        }
        else {
            sysml_conditional_13 = 0;
        }
        sysml_arithmetic_14 = ((int)(((double)sysml_state_displayNumber) / 10)) - sysml_conditional_13;
        sysml_lazy_12 = (sysml_state_displayNumber - (10 * sysml_arithmetic_14)) <= 9;
    }
    if (sysml_lazy_12) {
        sysml_conditional_15 = true;
    }
    else {
        sysml_conditional_15 = false;
    }
    sysml_value_2 = sysml_conditional_15;
    int sysml_value_3 = 0;
    sysml_value_3 = sysml_state_displayNumber;
    double sysml_value_4 = 0;
    if ((((double)sysml_state_displayNumber) / 10) < ((double)((int)(((double)sysml_state_displayNumber) / 10)))) {
        sysml_conditional_16 = 1;
    }
    else {
        sysml_conditional_16 = 0;
    }
    if ((((int)(((double)sysml_state_displayNumber) / 10)) - sysml_conditional_16) <= 2) {
        sysml_conditional_17 = 30.0;
    }
    else {
        if ((((double)sysml_state_displayNumber) / 10) < ((double)((int)(((double)sysml_state_displayNumber) / 10)))) {
            sysml_conditional_18 = 1;
        }
        else {
            sysml_conditional_18 = 0;
        }
        if ((((int)(((double)sysml_state_displayNumber) / 10)) - sysml_conditional_18) <= 5) {
            sysml_conditional_19 = 60.0;
        }
        else {
            sysml_conditional_19 = 90.0;
        }
        sysml_conditional_17 = sysml_conditional_19;
    }
    sysml_value_4 = sysml_conditional_17;
    double sysml_value_5 = 0;
    if ((((double)sysml_state_displayNumber) / 10) < ((double)((int)(((double)sysml_state_displayNumber) / 10)))) {
        sysml_conditional_20 = 1;
    }
    else {
        sysml_conditional_20 = 0;
    }
    if ((((int)(((double)sysml_state_displayNumber) / 10)) - sysml_conditional_20) <= 2) {
        sysml_conditional_21 = 60.0;
    }
    else {
        if ((((double)sysml_state_displayNumber) / 10) < ((double)((int)(((double)sysml_state_displayNumber) / 10)))) {
            sysml_conditional_22 = 1;
        }
        else {
            sysml_conditional_22 = 0;
        }
        if ((((int)(((double)sysml_state_displayNumber) / 10)) - sysml_conditional_22) <= 5) {
            sysml_conditional_23 = 90.0;
        }
        else {
            sysml_conditional_23 = 120.0;
        }
        sysml_conditional_21 = sysml_conditional_23;
    }
    sysml_value_5 = sysml_conditional_21;
    double sysml_value_6 = 0;
    if ((((double)sysml_state_displayNumber) / 10) < ((double)((int)(((double)sysml_state_displayNumber) / 10)))) {
        sysml_conditional_24 = 1;
    }
    else {
        sysml_conditional_24 = 0;
    }
    if ((((int)(((double)sysml_state_displayNumber) / 10)) - sysml_conditional_24) <= 2) {
        sysml_conditional_25 = 90.0;
    }
    else {
        if ((((double)sysml_state_displayNumber) / 10) < ((double)((int)(((double)sysml_state_displayNumber) / 10)))) {
            sysml_conditional_26 = 1;
        }
        else {
            sysml_conditional_26 = 0;
        }
        if ((((int)(((double)sysml_state_displayNumber) / 10)) - sysml_conditional_26) <= 5) {
            sysml_conditional_27 = 120.0;
        }
        else {
            sysml_conditional_27 = 150.0;
        }
        sysml_conditional_25 = sysml_conditional_27;
    }
    sysml_value_6 = sysml_conditional_25;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
    *io_4 = sysml_value_4;
    *io_5 = sysml_value_5;
    *io_6 = sysml_value_6;
}