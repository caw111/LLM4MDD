#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_3, double io_4, double io_5, double* io_0, double* io_1, bool* io_2_segment_0, bool* io_2_segment_1, bool* io_2_segment_2, bool* io_2_segment_3, bool* io_2_segment_4, bool* io_2_segment_5, bool* io_2_segment_6, bool* io_2_segment_7, bool* io_2_segment_8, bool* io_2_segment_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_value_0 = 0;
    bool sysml_conditional_0 = 0;
    bool sysml_conditional_1 = 0;
    bool sysml_conditional_2 = 0;
    bool sysml_conditional_3 = 0;
    bool sysml_conditional_4 = 0;
    bool sysml_conditional_5 = 0;
    bool sysml_conditional_6 = 0;
    bool sysml_conditional_7 = 0;
    bool sysml_conditional_8 = 0;
    bool sysml_conditional_9 = 0;
    sysml_value_0 = ((double)((io_3 * 255.0)) / 1023.0);
    double sysml_value_1 = 0;
    sysml_value_1 = ((double)((io_4 * 255.0)) / 1023.0);
    bool sysml_value_2 = 0;
    if (io_5 >= 1.0) {
        sysml_conditional_0 = true;
    }
    else {
        sysml_conditional_0 = false;
    }
    sysml_value_2 = sysml_conditional_0;
    bool sysml_value_3 = 0;
    if (io_5 >= 2.0) {
        sysml_conditional_1 = true;
    }
    else {
        sysml_conditional_1 = false;
    }
    sysml_value_3 = sysml_conditional_1;
    bool sysml_value_4 = 0;
    if (io_5 >= 3.0) {
        sysml_conditional_2 = true;
    }
    else {
        sysml_conditional_2 = false;
    }
    sysml_value_4 = sysml_conditional_2;
    bool sysml_value_5 = 0;
    if (io_5 >= 4.0) {
        sysml_conditional_3 = true;
    }
    else {
        sysml_conditional_3 = false;
    }
    sysml_value_5 = sysml_conditional_3;
    bool sysml_value_6 = 0;
    if (io_5 >= 5.0) {
        sysml_conditional_4 = true;
    }
    else {
        sysml_conditional_4 = false;
    }
    sysml_value_6 = sysml_conditional_4;
    bool sysml_value_7 = 0;
    if (io_5 >= 6.0) {
        sysml_conditional_5 = true;
    }
    else {
        sysml_conditional_5 = false;
    }
    sysml_value_7 = sysml_conditional_5;
    bool sysml_value_8 = 0;
    if (io_5 >= 7.0) {
        sysml_conditional_6 = true;
    }
    else {
        sysml_conditional_6 = false;
    }
    sysml_value_8 = sysml_conditional_6;
    bool sysml_value_9 = 0;
    if (io_5 >= 8.0) {
        sysml_conditional_7 = true;
    }
    else {
        sysml_conditional_7 = false;
    }
    sysml_value_9 = sysml_conditional_7;
    bool sysml_value_10 = 0;
    if (io_5 >= 9.0) {
        sysml_conditional_8 = true;
    }
    else {
        sysml_conditional_8 = false;
    }
    sysml_value_10 = sysml_conditional_8;
    bool sysml_value_11 = 0;
    if (io_5 >= 10.0) {
        sysml_conditional_9 = true;
    }
    else {
        sysml_conditional_9 = false;
    }
    sysml_value_11 = sysml_conditional_9;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2_segment_0 = sysml_value_2;
    *io_2_segment_1 = sysml_value_3;
    *io_2_segment_2 = sysml_value_4;
    *io_2_segment_3 = sysml_value_5;
    *io_2_segment_4 = sysml_value_6;
    *io_2_segment_5 = sysml_value_7;
    *io_2_segment_6 = sysml_value_8;
    *io_2_segment_7 = sysml_value_9;
    *io_2_segment_8 = sysml_value_10;
    *io_2_segment_9 = sysml_value_11;
}