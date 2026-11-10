#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_accumulatedMs = 0;
/* Global Variable define of  */
int sysml_state_currentNumber = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_accumulatedMs = 0;
    sysml_state_currentNumber = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_5, double* io_0, double* io_1, double* io_2, int* io_3, bool* io_4_segment_0, bool* io_4_segment_1, bool* io_4_segment_2, bool* io_4_segment_3, bool* io_4_segment_4, bool* io_4_segment_5, bool* io_4_segment_6, bool* io_4_segment_7, bool* io_4_segment_8, bool* io_4_segment_9, bool* io_6) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    double sysml_conditional_4 = 0;
    bool sysml_lazy_5 = 0;
    double sysml_conditional_6 = 0;
    bool sysml_lazy_7 = 0;
    double sysml_conditional_8 = 0;
    bool sysml_conditional_9 = 0;
    bool sysml_conditional_10 = 0;
    bool sysml_conditional_11 = 0;
    bool sysml_conditional_12 = 0;
    bool sysml_conditional_13 = 0;
    bool sysml_conditional_14 = 0;
    bool sysml_conditional_15 = 0;
    bool sysml_conditional_16 = 0;
    bool sysml_conditional_17 = 0;
    sysml_state_accumulatedMs = (sysml_state_accumulatedMs + io_5);
    if ((sysml_state_accumulatedMs >= 2000)) {
        sysml_state_accumulatedMs = (sysml_state_accumulatedMs - 2000);
        if (sysml_state_currentNumber >= 9) {
            sysml_conditional_0 = 0;
        }
        else {
            sysml_conditional_0 = sysml_state_currentNumber + 1;
        }
        sysml_state_currentNumber = sysml_conditional_0;
    }
    double sysml_value_0 = 0;
    sysml_lazy_1 = sysml_state_currentNumber == 0;
    if (!sysml_lazy_1) {
        sysml_lazy_1 = sysml_state_currentNumber == 1;
    }
    sysml_lazy_2 = sysml_lazy_1;
    if (!sysml_lazy_2) {
        sysml_lazy_2 = sysml_state_currentNumber == 2;
    }
    sysml_lazy_3 = sysml_lazy_2;
    if (!sysml_lazy_3) {
        sysml_lazy_3 = sysml_state_currentNumber == 3;
    }
    if (sysml_lazy_3) {
        sysml_conditional_4 = 1.0;
    }
    else {
        sysml_conditional_4 = 0.0;
    }
    sysml_value_0 = sysml_conditional_4;
    double sysml_value_1 = 0;
    sysml_lazy_5 = sysml_state_currentNumber >= 4;
    if (sysml_lazy_5) {
        sysml_lazy_5 = sysml_state_currentNumber <= 6;
    }
    if (sysml_lazy_5) {
        sysml_conditional_6 = 1.0;
    }
    else {
        sysml_conditional_6 = 0.0;
    }
    sysml_value_1 = sysml_conditional_6;
    double sysml_value_2 = 0;
    sysml_lazy_7 = sysml_state_currentNumber >= 7;
    if (sysml_lazy_7) {
        sysml_lazy_7 = sysml_state_currentNumber <= 9;
    }
    if (sysml_lazy_7) {
        sysml_conditional_8 = 1.0;
    }
    else {
        sysml_conditional_8 = 0.0;
    }
    sysml_value_2 = sysml_conditional_8;
    int sysml_value_3 = 0;
    sysml_value_3 = sysml_state_currentNumber;
    bool sysml_value_4 = 0;
    if (sysml_state_currentNumber >= 1) {
        sysml_conditional_9 = true;
    }
    else {
        sysml_conditional_9 = false;
    }
    sysml_value_4 = sysml_conditional_9;
    bool sysml_value_5 = 0;
    if (sysml_state_currentNumber >= 2) {
        sysml_conditional_10 = true;
    }
    else {
        sysml_conditional_10 = false;
    }
    sysml_value_5 = sysml_conditional_10;
    bool sysml_value_6 = 0;
    if (sysml_state_currentNumber >= 3) {
        sysml_conditional_11 = true;
    }
    else {
        sysml_conditional_11 = false;
    }
    sysml_value_6 = sysml_conditional_11;
    bool sysml_value_7 = 0;
    if (sysml_state_currentNumber >= 4) {
        sysml_conditional_12 = true;
    }
    else {
        sysml_conditional_12 = false;
    }
    sysml_value_7 = sysml_conditional_12;
    bool sysml_value_8 = 0;
    if (sysml_state_currentNumber >= 5) {
        sysml_conditional_13 = true;
    }
    else {
        sysml_conditional_13 = false;
    }
    sysml_value_8 = sysml_conditional_13;
    bool sysml_value_9 = 0;
    if (sysml_state_currentNumber >= 6) {
        sysml_conditional_14 = true;
    }
    else {
        sysml_conditional_14 = false;
    }
    sysml_value_9 = sysml_conditional_14;
    bool sysml_value_10 = 0;
    if (sysml_state_currentNumber >= 7) {
        sysml_conditional_15 = true;
    }
    else {
        sysml_conditional_15 = false;
    }
    sysml_value_10 = sysml_conditional_15;
    bool sysml_value_11 = 0;
    if (sysml_state_currentNumber >= 8) {
        sysml_conditional_16 = true;
    }
    else {
        sysml_conditional_16 = false;
    }
    sysml_value_11 = sysml_conditional_16;
    bool sysml_value_12 = 0;
    if (sysml_state_currentNumber >= 9) {
        sysml_conditional_17 = true;
    }
    else {
        sysml_conditional_17 = false;
    }
    sysml_value_12 = sysml_conditional_17;
    bool sysml_value_13 = 0;
    sysml_value_13 = false;
    bool sysml_value_14 = 0;
    sysml_value_14 = false;
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
    *io_6 = sysml_value_14;
}