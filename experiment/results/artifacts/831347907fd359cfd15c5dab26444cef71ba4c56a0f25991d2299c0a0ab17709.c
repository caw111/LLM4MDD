#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, double* io_2, double* io_3, int* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_value_0 = 0;
    double sysml_conditional_0 = 0;
    double sysml_conditional_1 = 0;
    double sysml_conditional_2 = 0;
    double sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    int sysml_conditional_6 = 0;
    int sysml_arithmetic_7 = 0;
    int sysml_conditional_8 = 0;
    int sysml_arithmetic_9 = 0;
    int sysml_conditional_10 = 0;
    int sysml_arithmetic_11 = 0;
    int sysml_conditional_12 = 0;
    int sysml_arithmetic_13 = 0;
    int sysml_conditional_14 = 0;
    if (io_0 >= 0.0) {
        if (io_0 <= 99.0) {
            sysml_conditional_1 = io_0;
        }
        else {
            sysml_conditional_1 = 99.0;
        }
        sysml_conditional_0 = sysml_conditional_1;
    }
    else {
        sysml_conditional_0 = 0.0;
    }
    sysml_value_0 = sysml_conditional_0;
    double sysml_value_1 = 0;
    if (io_1 >= 0.0) {
        if (io_1 <= 99.0) {
            sysml_conditional_3 = io_1;
        }
        else {
            sysml_conditional_3 = 99.0;
        }
        sysml_conditional_2 = sysml_conditional_3;
    }
    else {
        sysml_conditional_2 = 0.0;
    }
    sysml_value_1 = sysml_conditional_2;
    int sysml_value_2 = 0;
    if (io_0 >= 0.0) {
        if (io_0 <= 99.0) {
            if ((((double)io_0) / 10.0) < ((double)((int)(((double)io_0) / 10.0)))) {
                sysml_conditional_6 = 1;
            }
            else {
                sysml_conditional_6 = 0;
            }
            sysml_arithmetic_7 = ((int)(((double)io_0) / 10.0)) - sysml_conditional_6;
            if ((((double)io_0) / 10.0) < ((double)((int)(((double)io_0) / 10.0)))) {
                sysml_conditional_8 = 1;
            }
            else {
                sysml_conditional_8 = 0;
            }
            sysml_arithmetic_9 = ((int)(((double)io_0) / 10.0)) - sysml_conditional_8;
            if ((((double)io_0) / 10.0) < ((double)((int)(((double)io_0) / 10.0)))) {
                sysml_conditional_10 = 1;
            }
            else {
                sysml_conditional_10 = 0;
            }
            sysml_arithmetic_11 = ((int)(((double)io_0) / 10.0)) - sysml_conditional_10;
            if ((((double)io_0) / 10.0) < ((double)((int)(((double)io_0) / 10.0)))) {
                sysml_conditional_12 = 1;
            }
            else {
                sysml_conditional_12 = 0;
            }
            sysml_arithmetic_13 = ((int)(((double)io_0) / 10.0)) - sysml_conditional_12;
            if ((io_0 - (10.0 * sysml_arithmetic_11)) < ((double)((int)(io_0 - (10.0 * sysml_arithmetic_13))))) {
                sysml_conditional_14 = 1;
            }
            else {
                sysml_conditional_14 = 0;
            }
            sysml_conditional_5 = (2 * sysml_arithmetic_7) + (((int)(io_0 - (10.0 * sysml_arithmetic_9))) - sysml_conditional_14);
        }
        else {
            sysml_conditional_5 = 0;
        }
        sysml_conditional_4 = sysml_conditional_5;
    }
    else {
        sysml_conditional_4 = 0;
    }
    sysml_value_2 = sysml_conditional_4;
    *io_2 = sysml_value_0;
    *io_3 = sysml_value_1;
    *io_4 = sysml_value_2;
}