#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_3, double io_4, double* io_0, double* io_1, double* io_2, int* io_5) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_value_0 = 0;
    bool sysml_lazy_0 = 0;
    double sysml_conditional_1 = 0;
    double sysml_conditional_2 = 0;
    bool sysml_lazy_3 = 0;
    double sysml_conditional_4 = 0;
    double sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    double sysml_conditional_7 = 0;
    double sysml_conditional_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_lazy_10 = 0;
    bool sysml_lazy_11 = 0;
    int sysml_conditional_12 = 0;
    int sysml_conditional_13 = 0;
    bool sysml_lazy_14 = 0;
    int sysml_conditional_15 = 0;
    sysml_lazy_0 = io_3 >= 0.0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_3 <= 1023.0;
    }
    if (sysml_lazy_0) {
        sysml_conditional_1 = ((double)(io_3 * 255.0)) / 1023.0;
    }
    else {
        if (io_3 < 0.0) {
            sysml_conditional_2 = 0.0;
        }
        else {
            sysml_conditional_2 = 255.0;
        }
        sysml_conditional_1 = sysml_conditional_2;
    }
    sysml_value_0 = sysml_conditional_1;
    double sysml_value_1 = 0;
    sysml_lazy_3 = io_3 >= 0.0;
    if (sysml_lazy_3) {
        sysml_lazy_3 = io_3 <= 1023.0;
    }
    if (sysml_lazy_3) {
        sysml_conditional_4 = ((double)(io_3 * 255.0)) / 1023.0;
    }
    else {
        if (io_3 < 0.0) {
            sysml_conditional_5 = 0.0;
        }
        else {
            sysml_conditional_5 = 255.0;
        }
        sysml_conditional_4 = sysml_conditional_5;
    }
    sysml_value_1 = sysml_conditional_4;
    double sysml_value_2 = 0;
    sysml_lazy_6 = io_4 >= 0.0;
    if (sysml_lazy_6) {
        sysml_lazy_6 = io_4 <= 1023.0;
    }
    if (sysml_lazy_6) {
        sysml_conditional_7 = ((double)(io_4 * 255.0)) / 1023.0;
    }
    else {
        if (io_4 < 0.0) {
            sysml_conditional_8 = 0.0;
        }
        else {
            sysml_conditional_8 = 255.0;
        }
        sysml_conditional_7 = sysml_conditional_8;
    }
    sysml_value_2 = sysml_conditional_7;
    int sysml_value_3 = 0;
    sysml_lazy_9 = io_3 >= 0.0;
    if (sysml_lazy_9) {
        sysml_lazy_9 = io_3 <= 1023.0;
    }
    sysml_lazy_10 = sysml_lazy_9;
    if (sysml_lazy_10) {
        sysml_lazy_10 = io_4 >= 0.0;
    }
    sysml_lazy_11 = sysml_lazy_10;
    if (sysml_lazy_11) {
        sysml_lazy_11 = io_4 <= 1023.0;
    }
    if (sysml_lazy_11) {
        if ((((double)((((double)((((double)(io_3 * 255.0)) / 1023.0) + (((double)(io_3 * 255.0)) / 1023.0))) / 3.0) + (((double)(((double)(io_4 * 255.0)) / 1023.0)) / 3.0))) / 28.33) < ((double)((int)(((double)((((double)((((double)(io_3 * 255.0)) / 1023.0) + (((double)(io_3 * 255.0)) / 1023.0))) / 3.0) + (((double)(((double)(io_4 * 255.0)) / 1023.0)) / 3.0))) / 28.33)))) {
            sysml_conditional_13 = 1;
        }
        else {
            sysml_conditional_13 = 0;
        }
        sysml_conditional_12 = ((int)(((double)((((double)((((double)(io_3 * 255.0)) / 1023.0) + (((double)(io_3 * 255.0)) / 1023.0))) / 3.0) + (((double)(((double)(io_4 * 255.0)) / 1023.0)) / 3.0))) / 28.33)) - sysml_conditional_13;
    }
    else {
        sysml_lazy_14 = io_3 < 0.0;
        if (!sysml_lazy_14) {
            sysml_lazy_14 = io_4 < 0.0;
        }
        if (sysml_lazy_14) {
            sysml_conditional_15 = 0;
        }
        else {
            sysml_conditional_15 = 9;
        }
        sysml_conditional_12 = sysml_conditional_15;
    }
    sysml_value_3 = sysml_conditional_12;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_5 = sysml_value_3;
}