#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_count = 0;
/* Global Variable define of  */
int sysml_state_barTensLit = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_count = 0;
    sysml_state_barTensLit = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double* io_0, double* io_1, double* io_2, double* io_3, double* io_4, double* io_5, bool* io_6_segment_0, bool* io_6_segment_1, bool* io_6_segment_2, bool* io_6_segment_3, bool* io_6_segment_4, bool* io_6_segment_5, bool* io_6_segment_6, bool* io_6_segment_7, int* io_7) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    double sysml_conditional_4 = 0;
    bool sysml_lazy_5 = 0;
    double sysml_conditional_6 = 0;
    double sysml_conditional_7 = 0;
    double sysml_conditional_8 = 0;
    bool sysml_lazy_9 = 0;
    double sysml_conditional_10 = 0;
    double sysml_conditional_11 = 0;
    if (sysml_state_count >= 99) {
        sysml_conditional_0 = 0;
    }
    else {
        sysml_conditional_0 = sysml_state_count + 1;
    }
    sysml_state_count = sysml_conditional_0;
    if ((((double)sysml_state_count) / 10.0) < ((double)((int)(((double)sysml_state_count) / 10.0)))) {
        sysml_conditional_1 = 1;
    }
    else {
        sysml_conditional_1 = 0;
    }
    if ((((int)(((double)sysml_state_count) / 10.0)) - sysml_conditional_1) >= 8.0) {
        sysml_conditional_2 = 8;
    }
    else {
        if ((((double)sysml_state_count) / 10.0) < ((double)((int)(((double)sysml_state_count) / 10.0)))) {
            sysml_conditional_3 = 1;
        }
        else {
            sysml_conditional_3 = 0;
        }
        sysml_conditional_2 = ((int)(((double)sysml_state_count) / 10.0)) - sysml_conditional_3;
    }
    sysml_state_barTensLit = sysml_conditional_2;
    double sysml_value_0 = 0;
    if (sysml_state_count <= 5) {
        sysml_conditional_4 = 255.0;
    }
    else {
        sysml_conditional_4 = 0.0;
    }
    sysml_value_0 = sysml_conditional_4;
    double sysml_value_1 = 0;
    sysml_lazy_5 = sysml_state_count >= 6;
    if (sysml_lazy_5) {
        sysml_lazy_5 = sysml_state_count <= 9;
    }
    if (sysml_lazy_5) {
        sysml_conditional_6 = 255.0;
    }
    else {
        sysml_conditional_6 = 0.0;
    }
    sysml_value_1 = sysml_conditional_6;
    double sysml_value_2 = 0;
    if (sysml_state_count >= 10) {
        sysml_conditional_7 = 255.0;
    }
    else {
        sysml_conditional_7 = 0.0;
    }
    sysml_value_2 = sysml_conditional_7;
    double sysml_value_3 = 0;
    if (sysml_state_count <= 5) {
        sysml_conditional_8 = 255.0;
    }
    else {
        sysml_conditional_8 = 0.0;
    }
    sysml_value_3 = sysml_conditional_8;
    double sysml_value_4 = 0;
    sysml_lazy_9 = sysml_state_count >= 6;
    if (sysml_lazy_9) {
        sysml_lazy_9 = sysml_state_count <= 9;
    }
    if (sysml_lazy_9) {
        sysml_conditional_10 = 255.0;
    }
    else {
        sysml_conditional_10 = 0.0;
    }
    sysml_value_4 = sysml_conditional_10;
    double sysml_value_5 = 0;
    if (sysml_state_count >= 10) {
        sysml_conditional_11 = 255.0;
    }
    else {
        sysml_conditional_11 = 0.0;
    }
    sysml_value_5 = sysml_conditional_11;
    bool sysml_value_6 = 0;
    sysml_value_6 = (sysml_state_barTensLit >= 1);
    bool sysml_value_7 = 0;
    sysml_value_7 = (sysml_state_barTensLit >= 2);
    bool sysml_value_8 = 0;
    sysml_value_8 = (sysml_state_barTensLit >= 3);
    bool sysml_value_9 = 0;
    sysml_value_9 = (sysml_state_barTensLit >= 4);
    bool sysml_value_10 = 0;
    sysml_value_10 = (sysml_state_barTensLit >= 5);
    bool sysml_value_11 = 0;
    sysml_value_11 = (sysml_state_barTensLit >= 6);
    bool sysml_value_12 = 0;
    sysml_value_12 = (sysml_state_barTensLit >= 7);
    bool sysml_value_13 = 0;
    sysml_value_13 = (sysml_state_barTensLit >= 8);
    int sysml_value_14 = 0;
    sysml_value_14 = sysml_state_count;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
    *io_4 = sysml_value_4;
    *io_5 = sysml_value_5;
    *io_6_segment_0 = sysml_value_6;
    *io_6_segment_1 = sysml_value_7;
    *io_6_segment_2 = sysml_value_8;
    *io_6_segment_3 = sysml_value_9;
    *io_6_segment_4 = sysml_value_10;
    *io_6_segment_5 = sysml_value_11;
    *io_6_segment_6 = sysml_value_12;
    *io_6_segment_7 = sysml_value_13;
    *io_7 = sysml_value_14;
}