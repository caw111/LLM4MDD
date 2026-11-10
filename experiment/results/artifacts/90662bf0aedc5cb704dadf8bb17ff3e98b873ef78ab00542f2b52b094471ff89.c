#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_blinkAccumulatorMs = 0;
/* Global Variable define of  */
bool sysml_state_blinkOn = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_blinkAccumulatorMs = 0;
    sysml_state_blinkOn = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_0, double io_2, double* io_1, bool* io_3_segment_0, bool* io_3_segment_1, bool* io_3_segment_2, bool* io_3_segment_3, bool* io_3_segment_4, bool* io_3_segment_5, bool* io_3_segment_6, bool* io_3_segment_7, bool* io_3_segment_8, bool* io_3_segment_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_conditional_0 = 0;
    double sysml_conditional_1 = 0;
    bool sysml_conditional_2 = 0;
    bool sysml_conditional_3 = 0;
    bool sysml_conditional_4 = 0;
    bool sysml_conditional_5 = 0;
    bool sysml_conditional_6 = 0;
    bool sysml_conditional_7 = 0;
    bool sysml_conditional_8 = 0;
    bool sysml_conditional_9 = 0;
    bool sysml_conditional_10 = 0;
    bool sysml_conditional_11 = 0;
    if ((io_2 == 1023.0)) {
        sysml_state_blinkAccumulatorMs = (sysml_state_blinkAccumulatorMs + io_0);
        if ((sysml_state_blinkAccumulatorMs >= 2000)) {
            sysml_state_blinkAccumulatorMs = (sysml_state_blinkAccumulatorMs - 2000);
            sysml_state_blinkOn = (!sysml_state_blinkOn);
        }
        if ((sysml_state_blinkAccumulatorMs >= 2000)) {
            sysml_state_blinkAccumulatorMs = (sysml_state_blinkAccumulatorMs - 2000);
            sysml_state_blinkOn = (!sysml_state_blinkOn);
        }
    }
    else {
        sysml_state_blinkAccumulatorMs = 0;
        sysml_state_blinkOn = false;
    }
    double sysml_value_0 = 0;
    if (io_2 == 1023.0) {
        if (sysml_state_blinkOn) {
            sysml_conditional_1 = 1023.0;
        }
        else {
            sysml_conditional_1 = 0.0;
        }
        sysml_conditional_0 = sysml_conditional_1;
    }
    else {
        sysml_conditional_0 = io_2;
    }
    sysml_value_0 = sysml_conditional_0;
    bool sysml_value_1 = 0;
    if (io_2 <= 102.0) {
        sysml_conditional_2 = false;
    }
    else {
        sysml_conditional_2 = false;
    }
    sysml_value_1 = sysml_conditional_2;
    bool sysml_value_2 = 0;
    if (io_2 >= 103.0) {
        sysml_conditional_3 = io_2 <= 204.0;
    }
    else {
        sysml_conditional_3 = false;
    }
    sysml_value_2 = sysml_conditional_3;
    bool sysml_value_3 = 0;
    if (io_2 >= 205.0) {
        sysml_conditional_4 = io_2 <= 306.0;
    }
    else {
        sysml_conditional_4 = false;
    }
    sysml_value_3 = sysml_conditional_4;
    bool sysml_value_4 = 0;
    if (io_2 >= 307.0) {
        sysml_conditional_5 = io_2 <= 408.0;
    }
    else {
        sysml_conditional_5 = false;
    }
    sysml_value_4 = sysml_conditional_5;
    bool sysml_value_5 = 0;
    if (io_2 >= 409.0) {
        sysml_conditional_6 = io_2 <= 510.0;
    }
    else {
        sysml_conditional_6 = false;
    }
    sysml_value_5 = sysml_conditional_6;
    bool sysml_value_6 = 0;
    if (io_2 >= 511.0) {
        sysml_conditional_7 = io_2 <= 612.0;
    }
    else {
        sysml_conditional_7 = false;
    }
    sysml_value_6 = sysml_conditional_7;
    bool sysml_value_7 = 0;
    if (io_2 >= 613.0) {
        sysml_conditional_8 = io_2 <= 714.0;
    }
    else {
        sysml_conditional_8 = false;
    }
    sysml_value_7 = sysml_conditional_8;
    bool sysml_value_8 = 0;
    if (io_2 >= 715.0) {
        sysml_conditional_9 = io_2 <= 816.0;
    }
    else {
        sysml_conditional_9 = false;
    }
    sysml_value_8 = sysml_conditional_9;
    bool sysml_value_9 = 0;
    if (io_2 >= 817.0) {
        sysml_conditional_10 = io_2 <= 918.0;
    }
    else {
        sysml_conditional_10 = false;
    }
    sysml_value_9 = sysml_conditional_10;
    bool sysml_value_10 = 0;
    if (io_2 >= 919.0) {
        sysml_conditional_11 = io_2 <= 1023.0;
    }
    else {
        sysml_conditional_11 = false;
    }
    sysml_value_10 = sysml_conditional_11;
    *io_1 = sysml_value_0;
    *io_3_segment_0 = sysml_value_1;
    *io_3_segment_1 = sysml_value_2;
    *io_3_segment_2 = sysml_value_3;
    *io_3_segment_3 = sysml_value_4;
    *io_3_segment_4 = sysml_value_5;
    *io_3_segment_5 = sysml_value_6;
    *io_3_segment_6 = sysml_value_7;
    *io_3_segment_7 = sysml_value_8;
    *io_3_segment_8 = sysml_value_9;
    *io_3_segment_9 = sysml_value_10;
}