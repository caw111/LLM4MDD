#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, int* io_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_value_0 = 0;
    int sysml_conditional_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    int sysml_conditional_6 = 0;
    int sysml_conditional_7 = 0;
    int sysml_conditional_8 = 0;
    int sysml_conditional_9 = 0;
    int sysml_conditional_10 = 0;
    int sysml_conditional_11 = 0;
    int sysml_conditional_12 = 0;
    int sysml_conditional_13 = 0;
    if ((((double)(io_0 * 9.0)) / 1023.0) < ((double)((int)(((double)(io_0 * 9.0)) / 1023.0)))) {
        sysml_conditional_0 = 1;
    }
    else {
        sysml_conditional_0 = 0;
    }
    if ((((double)(io_1 * 9.0)) / 1023.0) < ((double)((int)(((double)(io_1 * 9.0)) / 1023.0)))) {
        sysml_conditional_1 = 1;
    }
    else {
        sysml_conditional_1 = 0;
    }
    if (((((int)(((double)(io_0 * 9.0)) / 1023.0)) - sysml_conditional_0) + (((int)(((double)(io_1 * 9.0)) / 1023.0)) - sysml_conditional_1)) >= 10.0) {
        if ((((double)(io_0 * 9.0)) / 1023.0) < ((double)((int)(((double)(io_0 * 9.0)) / 1023.0)))) {
            sysml_conditional_3 = 1;
        }
        else {
            sysml_conditional_3 = 0;
        }
        if ((((double)(io_1 * 9.0)) / 1023.0) < ((double)((int)(((double)(io_1 * 9.0)) / 1023.0)))) {
            sysml_conditional_4 = 1;
        }
        else {
            sysml_conditional_4 = 0;
        }
        if ((((double)(io_0 * 9.0)) / 1023.0) < ((double)((int)(((double)(io_0 * 9.0)) / 1023.0)))) {
            sysml_conditional_5 = 1;
        }
        else {
            sysml_conditional_5 = 0;
        }
        if ((((double)(io_1 * 9.0)) / 1023.0) < ((double)((int)(((double)(io_1 * 9.0)) / 1023.0)))) {
            sysml_conditional_6 = 1;
        }
        else {
            sysml_conditional_6 = 0;
        }
        if ((((double)(io_0 * 9.0)) / 1023.0) < ((double)((int)(((double)(io_0 * 9.0)) / 1023.0)))) {
            sysml_conditional_7 = 1;
        }
        else {
            sysml_conditional_7 = 0;
        }
        if ((((double)(io_1 * 9.0)) / 1023.0) < ((double)((int)(((double)(io_1 * 9.0)) / 1023.0)))) {
            sysml_conditional_8 = 1;
        }
        else {
            sysml_conditional_8 = 0;
        }
        if ((((double)(io_0 * 9.0)) / 1023.0) < ((double)((int)(((double)(io_0 * 9.0)) / 1023.0)))) {
            sysml_conditional_9 = 1;
        }
        else {
            sysml_conditional_9 = 0;
        }
        if ((((double)(io_1 * 9.0)) / 1023.0) < ((double)((int)(((double)(io_1 * 9.0)) / 1023.0)))) {
            sysml_conditional_10 = 1;
        }
        else {
            sysml_conditional_10 = 0;
        }
        if ((((double)((((int)(((double)(io_0 * 9.0)) / 1023.0)) - sysml_conditional_7) + (((int)(((double)(io_1 * 9.0)) / 1023.0)) - sysml_conditional_8))) / 10.0) < ((double)((int)(((double)((((int)(((double)(io_0 * 9.0)) / 1023.0)) - sysml_conditional_9) + (((int)(((double)(io_1 * 9.0)) / 1023.0)) - sysml_conditional_10))) / 10.0)))) {
            sysml_conditional_11 = 1;
        }
        else {
            sysml_conditional_11 = 0;
        }
        sysml_conditional_2 = ((((int)(((double)(io_0 * 9.0)) / 1023.0)) - sysml_conditional_3) + (((int)(((double)(io_1 * 9.0)) / 1023.0)) - sysml_conditional_4)) - (((int)(((double)((((int)(((double)(io_0 * 9.0)) / 1023.0)) - sysml_conditional_5) + (((int)(((double)(io_1 * 9.0)) / 1023.0)) - sysml_conditional_6))) / 10.0)) - sysml_conditional_11);
    }
    else {
        if ((((double)(io_0 * 9.0)) / 1023.0) < ((double)((int)(((double)(io_0 * 9.0)) / 1023.0)))) {
            sysml_conditional_12 = 1;
        }
        else {
            sysml_conditional_12 = 0;
        }
        if ((((double)(io_1 * 9.0)) / 1023.0) < ((double)((int)(((double)(io_1 * 9.0)) / 1023.0)))) {
            sysml_conditional_13 = 1;
        }
        else {
            sysml_conditional_13 = 0;
        }
        sysml_conditional_2 = (((int)(((double)(io_0 * 9.0)) / 1023.0)) - sysml_conditional_12) + (((int)(((double)(io_1 * 9.0)) / 1023.0)) - sysml_conditional_13);
    }
    sysml_value_0 = sysml_conditional_2;
    *io_2 = sysml_value_0;
}