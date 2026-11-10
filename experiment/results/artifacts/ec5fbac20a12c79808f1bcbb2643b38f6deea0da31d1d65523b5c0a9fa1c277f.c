#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_blinkPhaseMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_blinkPhaseMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_1, int io_3, double* io_0, bool* io_2_segment_0, bool* io_2_segment_1, bool* io_2_segment_2, bool* io_2_segment_3, bool* io_2_segment_4, bool* io_2_segment_5, bool* io_2_segment_6, bool* io_2_segment_7, bool* io_2_segment_8, bool* io_2_segment_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    double sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    double sysml_conditional_3 = 0;
    double sysml_conditional_4 = 0;
    bool sysml_conditional_5 = 0;
    bool sysml_conditional_6 = 0;
    bool sysml_conditional_7 = 0;
    bool sysml_conditional_8 = 0;
    bool sysml_conditional_9 = 0;
    bool sysml_conditional_10 = 0;
    bool sysml_conditional_11 = 0;
    bool sysml_conditional_12 = 0;
    bool sysml_conditional_13 = 0;
    bool sysml_conditional_14 = 0;
    if (io_1 == 1023.0) {
        sysml_conditional_0 = (sysml_state_blinkPhaseMs + io_3) - 0;
    }
    else {
        sysml_conditional_0 = 0;
    }
    sysml_state_blinkPhaseMs = sysml_conditional_0;
    if ((sysml_state_blinkPhaseMs >= 4000)) {
        sysml_state_blinkPhaseMs = (sysml_state_blinkPhaseMs - 4000);
    }
    double sysml_value_0 = 0;
    if (io_1 == 1023.0) {
        sysml_lazy_2 = sysml_state_blinkPhaseMs < 2000;
        if (sysml_lazy_2) {
            sysml_lazy_2 = sysml_state_blinkPhaseMs >= 0;
        }
        if (sysml_lazy_2) {
            sysml_conditional_3 = 1023.0;
        }
        else {
            sysml_conditional_3 = 0.0;
        }
        sysml_conditional_1 = sysml_conditional_3;
    }
    else {
        if (io_1 == 1023.0) {
            sysml_conditional_4 = 1023.0;
        }
        else {
            sysml_conditional_4 = io_1;
        }
        sysml_conditional_1 = sysml_conditional_4;
    }
    sysml_value_0 = sysml_conditional_1;
    bool sysml_value_1 = 0;
    if (io_1 >= 103.0) {
        sysml_conditional_5 = true;
    }
    else {
        sysml_conditional_5 = false;
    }
    sysml_value_1 = sysml_conditional_5;
    bool sysml_value_2 = 0;
    if (io_1 >= 204.0) {
        sysml_conditional_6 = true;
    }
    else {
        sysml_conditional_6 = false;
    }
    sysml_value_2 = sysml_conditional_6;
    bool sysml_value_3 = 0;
    if (io_1 >= 307.0) {
        sysml_conditional_7 = true;
    }
    else {
        sysml_conditional_7 = false;
    }
    sysml_value_3 = sysml_conditional_7;
    bool sysml_value_4 = 0;
    if (io_1 >= 410.0) {
        sysml_conditional_8 = true;
    }
    else {
        sysml_conditional_8 = false;
    }
    sysml_value_4 = sysml_conditional_8;
    bool sysml_value_5 = 0;
    if (io_1 >= 513.0) {
        sysml_conditional_9 = true;
    }
    else {
        sysml_conditional_9 = false;
    }
    sysml_value_5 = sysml_conditional_9;
    bool sysml_value_6 = 0;
    if (io_1 >= 616.0) {
        sysml_conditional_10 = true;
    }
    else {
        sysml_conditional_10 = false;
    }
    sysml_value_6 = sysml_conditional_10;
    bool sysml_value_7 = 0;
    if (io_1 >= 719.0) {
        sysml_conditional_11 = true;
    }
    else {
        sysml_conditional_11 = false;
    }
    sysml_value_7 = sysml_conditional_11;
    bool sysml_value_8 = 0;
    if (io_1 >= 822.0) {
        sysml_conditional_12 = true;
    }
    else {
        sysml_conditional_12 = false;
    }
    sysml_value_8 = sysml_conditional_12;
    bool sysml_value_9 = 0;
    if (io_1 >= 925.0) {
        sysml_conditional_13 = true;
    }
    else {
        sysml_conditional_13 = false;
    }
    sysml_value_9 = sysml_conditional_13;
    bool sysml_value_10 = 0;
    if (io_1 >= 1028.0) {
        sysml_conditional_14 = true;
    }
    else {
        sysml_conditional_14 = false;
    }
    sysml_value_10 = sysml_conditional_14;
    *io_0 = sysml_value_0;
    *io_2_segment_0 = sysml_value_1;
    *io_2_segment_1 = sysml_value_2;
    *io_2_segment_2 = sysml_value_3;
    *io_2_segment_3 = sysml_value_4;
    *io_2_segment_4 = sysml_value_5;
    *io_2_segment_5 = sysml_value_6;
    *io_2_segment_6 = sysml_value_7;
    *io_2_segment_7 = sysml_value_8;
    *io_2_segment_8 = sysml_value_9;
    *io_2_segment_9 = sysml_value_10;
}