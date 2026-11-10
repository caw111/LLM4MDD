#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double* io_1, double* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7, bool* io_8, bool* io_9, bool* io_10, bool* io_11, bool* io_12) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_value_0 = 0;
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
    sysml_value_0 = ((double)((io_0 * 180.0)) / 1023.0);
    double sysml_value_1 = 0;
    sysml_value_1 = (180.0 - ((double)((io_0 * 180.0)) / 1023.0));
    bool sysml_value_2 = 0;
    if ((((double)(io_0 * 10.0)) / 1023.0) < ((double)((int)(((double)(io_0 * 10.0)) / 1023.0)))) {
        sysml_conditional_0 = 1;
    }
    else {
        sysml_conditional_0 = 0;
    }
    sysml_value_2 = (((int)(((double)(io_0 * 10.0)) / 1023.0)) - sysml_conditional_0) >= 1;
    bool sysml_value_3 = 0;
    if ((((double)(io_0 * 10.0)) / 1023.0) < ((double)((int)(((double)(io_0 * 10.0)) / 1023.0)))) {
        sysml_conditional_1 = 1;
    }
    else {
        sysml_conditional_1 = 0;
    }
    sysml_value_3 = (((int)(((double)(io_0 * 10.0)) / 1023.0)) - sysml_conditional_1) >= 2;
    bool sysml_value_4 = 0;
    if ((((double)(io_0 * 10.0)) / 1023.0) < ((double)((int)(((double)(io_0 * 10.0)) / 1023.0)))) {
        sysml_conditional_2 = 1;
    }
    else {
        sysml_conditional_2 = 0;
    }
    sysml_value_4 = (((int)(((double)(io_0 * 10.0)) / 1023.0)) - sysml_conditional_2) >= 3;
    bool sysml_value_5 = 0;
    if ((((double)(io_0 * 10.0)) / 1023.0) < ((double)((int)(((double)(io_0 * 10.0)) / 1023.0)))) {
        sysml_conditional_3 = 1;
    }
    else {
        sysml_conditional_3 = 0;
    }
    sysml_value_5 = (((int)(((double)(io_0 * 10.0)) / 1023.0)) - sysml_conditional_3) >= 4;
    bool sysml_value_6 = 0;
    if ((((double)(io_0 * 10.0)) / 1023.0) < ((double)((int)(((double)(io_0 * 10.0)) / 1023.0)))) {
        sysml_conditional_4 = 1;
    }
    else {
        sysml_conditional_4 = 0;
    }
    sysml_value_6 = (((int)(((double)(io_0 * 10.0)) / 1023.0)) - sysml_conditional_4) >= 5;
    bool sysml_value_7 = 0;
    if ((((double)(io_0 * 10.0)) / 1023.0) < ((double)((int)(((double)(io_0 * 10.0)) / 1023.0)))) {
        sysml_conditional_5 = 1;
    }
    else {
        sysml_conditional_5 = 0;
    }
    sysml_value_7 = (((int)(((double)(io_0 * 10.0)) / 1023.0)) - sysml_conditional_5) >= 6;
    bool sysml_value_8 = 0;
    if ((((double)(io_0 * 10.0)) / 1023.0) < ((double)((int)(((double)(io_0 * 10.0)) / 1023.0)))) {
        sysml_conditional_6 = 1;
    }
    else {
        sysml_conditional_6 = 0;
    }
    sysml_value_8 = (((int)(((double)(io_0 * 10.0)) / 1023.0)) - sysml_conditional_6) >= 7;
    bool sysml_value_9 = 0;
    if ((((double)(io_0 * 10.0)) / 1023.0) < ((double)((int)(((double)(io_0 * 10.0)) / 1023.0)))) {
        sysml_conditional_7 = 1;
    }
    else {
        sysml_conditional_7 = 0;
    }
    sysml_value_9 = (((int)(((double)(io_0 * 10.0)) / 1023.0)) - sysml_conditional_7) >= 8;
    bool sysml_value_10 = 0;
    if ((((double)(io_0 * 10.0)) / 1023.0) < ((double)((int)(((double)(io_0 * 10.0)) / 1023.0)))) {
        sysml_conditional_8 = 1;
    }
    else {
        sysml_conditional_8 = 0;
    }
    sysml_value_10 = (((int)(((double)(io_0 * 10.0)) / 1023.0)) - sysml_conditional_8) >= 9;
    bool sysml_value_11 = 0;
    if ((((double)(io_0 * 10.0)) / 1023.0) < ((double)((int)(((double)(io_0 * 10.0)) / 1023.0)))) {
        sysml_conditional_9 = 1;
    }
    else {
        sysml_conditional_9 = 0;
    }
    sysml_value_11 = (((int)(((double)(io_0 * 10.0)) / 1023.0)) - sysml_conditional_9) >= 10;
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
    *io_3 = sysml_value_2;
    *io_4 = sysml_value_3;
    *io_5 = sysml_value_4;
    *io_6 = sysml_value_5;
    *io_7 = sysml_value_6;
    *io_8 = sysml_value_7;
    *io_9 = sysml_value_8;
    *io_10 = sysml_value_9;
    *io_11 = sysml_value_10;
    *io_12 = sysml_value_11;
}