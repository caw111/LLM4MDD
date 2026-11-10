#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, int* io_1, bool* io_2, bool* io_3, bool* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_value_0 = 0;
    int sysml_conditional_0 = 0;
    int sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    int sysml_conditional_3 = 0;
    bool sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    int sysml_conditional_7 = 0;
    bool sysml_conditional_8 = 0;
    int sysml_conditional_9 = 0;
    bool sysml_lazy_10 = 0;
    int sysml_conditional_11 = 0;
    bool sysml_conditional_12 = 0;
    if ((((double)(io_0 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_0 * 99.0)) / 1023.0)))) {
        sysml_conditional_0 = 1;
    }
    else {
        sysml_conditional_0 = 0;
    }
    sysml_value_0 = ((int)(((double)(io_0 * 99.0)) / 1023.0)) - sysml_conditional_0;
    bool sysml_value_1 = 0;
    if ((((double)(io_0 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_0 * 99.0)) / 1023.0)))) {
        sysml_conditional_1 = 1;
    }
    else {
        sysml_conditional_1 = 0;
    }
    sysml_lazy_2 = (((int)(((double)(io_0 * 99.0)) / 1023.0)) - sysml_conditional_1) <= 33;
    if (sysml_lazy_2) {
        if ((((double)(io_0 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_0 * 99.0)) / 1023.0)))) {
            sysml_conditional_3 = 1;
        }
        else {
            sysml_conditional_3 = 0;
        }
        sysml_lazy_2 = (((int)(((double)(io_0 * 99.0)) / 1023.0)) - sysml_conditional_3) >= 0;
    }
    if (sysml_lazy_2) {
        sysml_conditional_4 = true;
    }
    else {
        sysml_conditional_4 = false;
    }
    sysml_value_1 = sysml_conditional_4;
    bool sysml_value_2 = 0;
    if ((((double)(io_0 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_0 * 99.0)) / 1023.0)))) {
        sysml_conditional_5 = 1;
    }
    else {
        sysml_conditional_5 = 0;
    }
    sysml_lazy_6 = (((int)(((double)(io_0 * 99.0)) / 1023.0)) - sysml_conditional_5) <= 66;
    if (sysml_lazy_6) {
        if ((((double)(io_0 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_0 * 99.0)) / 1023.0)))) {
            sysml_conditional_7 = 1;
        }
        else {
            sysml_conditional_7 = 0;
        }
        sysml_lazy_6 = (((int)(((double)(io_0 * 99.0)) / 1023.0)) - sysml_conditional_7) >= 34;
    }
    if (sysml_lazy_6) {
        sysml_conditional_8 = true;
    }
    else {
        sysml_conditional_8 = false;
    }
    sysml_value_2 = sysml_conditional_8;
    bool sysml_value_3 = 0;
    if ((((double)(io_0 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_0 * 99.0)) / 1023.0)))) {
        sysml_conditional_9 = 1;
    }
    else {
        sysml_conditional_9 = 0;
    }
    sysml_lazy_10 = (((int)(((double)(io_0 * 99.0)) / 1023.0)) - sysml_conditional_9) <= 99;
    if (sysml_lazy_10) {
        if ((((double)(io_0 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_0 * 99.0)) / 1023.0)))) {
            sysml_conditional_11 = 1;
        }
        else {
            sysml_conditional_11 = 0;
        }
        sysml_lazy_10 = (((int)(((double)(io_0 * 99.0)) / 1023.0)) - sysml_conditional_11) >= 67;
    }
    if (sysml_lazy_10) {
        sysml_conditional_12 = true;
    }
    else {
        sysml_conditional_12 = false;
    }
    sysml_value_3 = sysml_conditional_12;
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
    *io_3 = sysml_value_2;
    *io_4 = sysml_value_3;
}