#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_currentNumber = 0;
/* Global Variable define of  */
int sysml_state_accumulatedMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_currentNumber = 0;
    sysml_state_accumulatedMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_4, bool* io_0, bool* io_1, int* io_2, bool* io_3_segment_0, bool* io_3_segment_1, bool* io_3_segment_2, bool* io_3_segment_3, bool* io_3_segment_4, bool* io_3_segment_5, bool* io_3_segment_6, bool* io_3_segment_7, bool* io_3_segment_8, bool* io_3_segment_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_arithmetic_2 = 0;
    bool sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_arithmetic_5 = 0;
    bool sysml_conditional_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_conditional_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_conditional_10 = 0;
    bool sysml_lazy_11 = 0;
    bool sysml_conditional_12 = 0;
    bool sysml_lazy_13 = 0;
    bool sysml_conditional_14 = 0;
    bool sysml_lazy_15 = 0;
    bool sysml_conditional_16 = 0;
    bool sysml_lazy_17 = 0;
    bool sysml_conditional_18 = 0;
    bool sysml_lazy_19 = 0;
    bool sysml_conditional_20 = 0;
    bool sysml_lazy_21 = 0;
    bool sysml_conditional_22 = 0;
    bool sysml_lazy_23 = 0;
    bool sysml_conditional_24 = 0;
    bool sysml_lazy_25 = 0;
    bool sysml_conditional_26 = 0;
    if ((sysml_state_accumulatedMs >= 2000)) {
        sysml_state_accumulatedMs = (sysml_state_accumulatedMs - 2000);
        if (sysml_state_currentNumber == 9) {
            sysml_conditional_0 = 0;
        }
        else {
            sysml_conditional_0 = sysml_state_currentNumber + 1;
        }
        sysml_state_currentNumber = sysml_conditional_0;
    }
    sysml_state_accumulatedMs = (sysml_state_accumulatedMs + io_4);
    bool sysml_value_0 = 0;
    if ((((double)sysml_state_currentNumber) / 2) < ((double)((int)(((double)sysml_state_currentNumber) / 2)))) {
        sysml_conditional_1 = 1;
    }
    else {
        sysml_conditional_1 = 0;
    }
    sysml_arithmetic_2 = ((int)(((double)sysml_state_currentNumber) / 2)) - sysml_conditional_1;
    if (sysml_state_currentNumber == (2 * sysml_arithmetic_2)) {
        sysml_conditional_3 = true;
    }
    else {
        sysml_conditional_3 = false;
    }
    sysml_value_0 = sysml_conditional_3;
    bool sysml_value_1 = 0;
    if ((((double)sysml_state_currentNumber) / 2) < ((double)((int)(((double)sysml_state_currentNumber) / 2)))) {
        sysml_conditional_4 = 1;
    }
    else {
        sysml_conditional_4 = 0;
    }
    sysml_arithmetic_5 = ((int)(((double)sysml_state_currentNumber) / 2)) - sysml_conditional_4;
    if (sysml_state_currentNumber != (2 * sysml_arithmetic_5)) {
        sysml_conditional_6 = true;
    }
    else {
        sysml_conditional_6 = false;
    }
    sysml_value_1 = sysml_conditional_6;
    int sysml_value_2 = 0;
    sysml_value_2 = sysml_state_currentNumber;
    bool sysml_value_3 = 0;
    sysml_lazy_7 = sysml_state_currentNumber >= 1;
    if (sysml_lazy_7) {
        sysml_lazy_7 = sysml_state_currentNumber <= 1;
    }
    if (sysml_lazy_7) {
        sysml_conditional_8 = true;
    }
    else {
        sysml_conditional_8 = false;
    }
    sysml_value_3 = sysml_conditional_8;
    bool sysml_value_4 = 0;
    sysml_lazy_9 = sysml_state_currentNumber >= 2;
    if (sysml_lazy_9) {
        sysml_lazy_9 = sysml_state_currentNumber <= 2;
    }
    if (sysml_lazy_9) {
        sysml_conditional_10 = true;
    }
    else {
        sysml_conditional_10 = false;
    }
    sysml_value_4 = sysml_conditional_10;
    bool sysml_value_5 = 0;
    sysml_lazy_11 = sysml_state_currentNumber >= 3;
    if (sysml_lazy_11) {
        sysml_lazy_11 = sysml_state_currentNumber <= 3;
    }
    if (sysml_lazy_11) {
        sysml_conditional_12 = true;
    }
    else {
        sysml_conditional_12 = false;
    }
    sysml_value_5 = sysml_conditional_12;
    bool sysml_value_6 = 0;
    sysml_lazy_13 = sysml_state_currentNumber >= 4;
    if (sysml_lazy_13) {
        sysml_lazy_13 = sysml_state_currentNumber <= 4;
    }
    if (sysml_lazy_13) {
        sysml_conditional_14 = true;
    }
    else {
        sysml_conditional_14 = false;
    }
    sysml_value_6 = sysml_conditional_14;
    bool sysml_value_7 = 0;
    sysml_lazy_15 = sysml_state_currentNumber >= 5;
    if (sysml_lazy_15) {
        sysml_lazy_15 = sysml_state_currentNumber <= 5;
    }
    if (sysml_lazy_15) {
        sysml_conditional_16 = true;
    }
    else {
        sysml_conditional_16 = false;
    }
    sysml_value_7 = sysml_conditional_16;
    bool sysml_value_8 = 0;
    sysml_lazy_17 = sysml_state_currentNumber >= 6;
    if (sysml_lazy_17) {
        sysml_lazy_17 = sysml_state_currentNumber <= 6;
    }
    if (sysml_lazy_17) {
        sysml_conditional_18 = true;
    }
    else {
        sysml_conditional_18 = false;
    }
    sysml_value_8 = sysml_conditional_18;
    bool sysml_value_9 = 0;
    sysml_lazy_19 = sysml_state_currentNumber >= 7;
    if (sysml_lazy_19) {
        sysml_lazy_19 = sysml_state_currentNumber <= 7;
    }
    if (sysml_lazy_19) {
        sysml_conditional_20 = true;
    }
    else {
        sysml_conditional_20 = false;
    }
    sysml_value_9 = sysml_conditional_20;
    bool sysml_value_10 = 0;
    sysml_lazy_21 = sysml_state_currentNumber >= 8;
    if (sysml_lazy_21) {
        sysml_lazy_21 = sysml_state_currentNumber <= 8;
    }
    if (sysml_lazy_21) {
        sysml_conditional_22 = true;
    }
    else {
        sysml_conditional_22 = false;
    }
    sysml_value_10 = sysml_conditional_22;
    bool sysml_value_11 = 0;
    sysml_lazy_23 = sysml_state_currentNumber >= 9;
    if (sysml_lazy_23) {
        sysml_lazy_23 = sysml_state_currentNumber <= 9;
    }
    if (sysml_lazy_23) {
        sysml_conditional_24 = true;
    }
    else {
        sysml_conditional_24 = false;
    }
    sysml_value_11 = sysml_conditional_24;
    bool sysml_value_12 = 0;
    sysml_lazy_25 = sysml_state_currentNumber >= 10;
    if (sysml_lazy_25) {
        sysml_lazy_25 = sysml_state_currentNumber <= 10;
    }
    if (sysml_lazy_25) {
        sysml_conditional_26 = true;
    }
    else {
        sysml_conditional_26 = false;
    }
    sysml_value_12 = sysml_conditional_26;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3_segment_0 = sysml_value_3;
    *io_3_segment_1 = sysml_value_4;
    *io_3_segment_2 = sysml_value_5;
    *io_3_segment_3 = sysml_value_6;
    *io_3_segment_4 = sysml_value_7;
    *io_3_segment_5 = sysml_value_8;
    *io_3_segment_6 = sysml_value_9;
    *io_3_segment_7 = sysml_value_10;
    *io_3_segment_8 = sysml_value_11;
    *io_3_segment_9 = sysml_value_12;
}