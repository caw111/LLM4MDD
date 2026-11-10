#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_number = 0;
/* Global Variable define of  */
int sysml_state_accumulatedMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_number = 0;
    sysml_state_accumulatedMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_5, double* io_0, double* io_1, double* io_2, int* io_3, bool* io_4_segment_0, bool* io_4_segment_1, bool* io_4_segment_2, bool* io_4_segment_3, bool* io_4_segment_4, bool* io_4_segment_5, bool* io_4_segment_6, bool* io_4_segment_7, bool* io_4_segment_8, bool* io_4_segment_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    double sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    double sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    double sysml_conditional_5 = 0;
    if ((io_5 > 0)) {
        sysml_state_accumulatedMs = (sysml_state_accumulatedMs + io_5);
    }
    if ((sysml_state_accumulatedMs >= 2000)) {
        sysml_state_accumulatedMs = (sysml_state_accumulatedMs - 2000);
        if (sysml_state_number == 9) {
            sysml_conditional_0 = 0;
        }
        else {
            sysml_conditional_0 = sysml_state_number + 1;
        }
        sysml_state_number = sysml_conditional_0;
    }
    double sysml_value_0 = 0;
    if (sysml_state_number <= 3) {
        sysml_conditional_1 = 255.0;
    }
    else {
        sysml_conditional_1 = 0.0;
    }
    sysml_value_0 = sysml_conditional_1;
    double sysml_value_1 = 0;
    sysml_lazy_2 = sysml_state_number >= 4;
    if (sysml_lazy_2) {
        sysml_lazy_2 = sysml_state_number <= 6;
    }
    if (sysml_lazy_2) {
        sysml_conditional_3 = 255.0;
    }
    else {
        sysml_conditional_3 = 0.0;
    }
    sysml_value_1 = sysml_conditional_3;
    double sysml_value_2 = 0;
    sysml_lazy_4 = sysml_state_number >= 7;
    if (sysml_lazy_4) {
        sysml_lazy_4 = sysml_state_number <= 9;
    }
    if (sysml_lazy_4) {
        sysml_conditional_5 = 255.0;
    }
    else {
        sysml_conditional_5 = 0.0;
    }
    sysml_value_2 = sysml_conditional_5;
    int sysml_value_3 = 0;
    sysml_value_3 = sysml_state_number;
    bool sysml_value_4 = 0;
    sysml_value_4 = (sysml_state_number >= 1);
    bool sysml_value_5 = 0;
    sysml_value_5 = (sysml_state_number >= 2);
    bool sysml_value_6 = 0;
    sysml_value_6 = (sysml_state_number >= 3);
    bool sysml_value_7 = 0;
    sysml_value_7 = (sysml_state_number >= 4);
    bool sysml_value_8 = 0;
    sysml_value_8 = (sysml_state_number >= 5);
    bool sysml_value_9 = 0;
    sysml_value_9 = (sysml_state_number >= 6);
    bool sysml_value_10 = 0;
    sysml_value_10 = (sysml_state_number >= 7);
    bool sysml_value_11 = 0;
    sysml_value_11 = (sysml_state_number >= 8);
    bool sysml_value_12 = 0;
    sysml_value_12 = (sysml_state_number >= 9);
    bool sysml_value_13 = 0;
    sysml_value_13 = false;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
    *io_4_segment_0 = sysml_value_4;
    *io_4_segment_1 = sysml_value_5;
    *io_4_segment_2 = sysml_value_6;
    *io_4_segment_3 = sysml_value_7;
    *io_4_segment_4 = sysml_value_8;
    *io_4_segment_5 = sysml_value_9;
    *io_4_segment_6 = sysml_value_10;
    *io_4_segment_7 = sysml_value_11;
    *io_4_segment_8 = sysml_value_12;
    *io_4_segment_9 = sysml_value_13;
}