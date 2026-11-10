#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_accumulatedMs = 0;
/* Global Variable define of  */
int sysml_state_numberValue = 0;
/* Global Variable define of  */
bool sysml_state_ledPhase = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_accumulatedMs = 0;
    sysml_state_numberValue = 0;
    sysml_state_ledPhase = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_4, int* io_0, bool* io_1_segment_0, bool* io_1_segment_1, bool* io_1_segment_2, bool* io_1_segment_3, bool* io_1_segment_4, bool* io_1_segment_5, bool* io_1_segment_6, bool* io_1_segment_7, bool* io_1_segment_8, bool* io_1_segment_9, bool* io_2, bool* io_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_conditional_0 = 0;
    int sysml_conditional_1 = 0;
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
    bool sysml_conditional_12 = 0;
    bool sysml_conditional_13 = 0;
    bool sysml_conditional_14 = 0;
    bool sysml_conditional_15 = 0;
    bool sysml_conditional_16 = 0;
    bool sysml_conditional_17 = 0;
    bool sysml_conditional_18 = 0;
    bool sysml_conditional_19 = 0;
    bool sysml_conditional_20 = 0;
    bool sysml_conditional_21 = 0;
    bool sysml_conditional_22 = 0;
    bool sysml_conditional_23 = 0;
    bool sysml_conditional_24 = 0;
    bool sysml_conditional_25 = 0;
    sysml_state_accumulatedMs = (sysml_state_accumulatedMs + io_4);
    if ((sysml_state_accumulatedMs >= 2000)) {
        sysml_state_accumulatedMs = (sysml_state_accumulatedMs - 2000);
        if (sysml_state_numberValue >= 99) {
            sysml_conditional_0 = true;
        }
        else {
            sysml_conditional_0 = false;
        }
        if (sysml_conditional_0) {
            sysml_conditional_1 = 0;
        }
        else {
            sysml_conditional_1 = sysml_state_numberValue + 1;
        }
        sysml_state_numberValue = sysml_conditional_1;
        sysml_state_ledPhase = (!sysml_state_ledPhase);
    }
    int sysml_value_0 = 0;
    sysml_value_0 = sysml_state_numberValue;
    bool sysml_value_1 = 0;
    if (sysml_state_numberValue <= 19) {
        sysml_conditional_2 = true;
    }
    else {
        sysml_conditional_2 = false;
    }
    if (sysml_conditional_2) {
        sysml_conditional_3 = true;
    }
    else {
        sysml_conditional_3 = false;
    }
    sysml_value_1 = sysml_conditional_3;
    bool sysml_value_2 = 0;
    if ((((double)sysml_state_numberValue) / 10) >= 1) {
        sysml_conditional_4 = true;
    }
    else {
        sysml_conditional_4 = false;
    }
    if (sysml_conditional_4) {
        sysml_conditional_5 = true;
    }
    else {
        sysml_conditional_5 = false;
    }
    sysml_value_2 = sysml_conditional_5;
    bool sysml_value_3 = 0;
    if ((((double)sysml_state_numberValue) / 10) >= 2) {
        sysml_conditional_6 = true;
    }
    else {
        sysml_conditional_6 = false;
    }
    if (sysml_conditional_6) {
        sysml_conditional_7 = true;
    }
    else {
        sysml_conditional_7 = false;
    }
    sysml_value_3 = sysml_conditional_7;
    bool sysml_value_4 = 0;
    if ((((double)sysml_state_numberValue) / 10) >= 3) {
        sysml_conditional_8 = true;
    }
    else {
        sysml_conditional_8 = false;
    }
    if (sysml_conditional_8) {
        sysml_conditional_9 = true;
    }
    else {
        sysml_conditional_9 = false;
    }
    sysml_value_4 = sysml_conditional_9;
    bool sysml_value_5 = 0;
    if ((((double)sysml_state_numberValue) / 10) >= 4) {
        sysml_conditional_10 = true;
    }
    else {
        sysml_conditional_10 = false;
    }
    if (sysml_conditional_10) {
        sysml_conditional_11 = true;
    }
    else {
        sysml_conditional_11 = false;
    }
    sysml_value_5 = sysml_conditional_11;
    bool sysml_value_6 = 0;
    if ((((double)sysml_state_numberValue) / 10) >= 5) {
        sysml_conditional_12 = true;
    }
    else {
        sysml_conditional_12 = false;
    }
    if (sysml_conditional_12) {
        sysml_conditional_13 = true;
    }
    else {
        sysml_conditional_13 = false;
    }
    sysml_value_6 = sysml_conditional_13;
    bool sysml_value_7 = 0;
    if ((((double)sysml_state_numberValue) / 10) >= 6) {
        sysml_conditional_14 = true;
    }
    else {
        sysml_conditional_14 = false;
    }
    if (sysml_conditional_14) {
        sysml_conditional_15 = true;
    }
    else {
        sysml_conditional_15 = false;
    }
    sysml_value_7 = sysml_conditional_15;
    bool sysml_value_8 = 0;
    if ((((double)sysml_state_numberValue) / 10) >= 7) {
        sysml_conditional_16 = true;
    }
    else {
        sysml_conditional_16 = false;
    }
    if (sysml_conditional_16) {
        sysml_conditional_17 = true;
    }
    else {
        sysml_conditional_17 = false;
    }
    sysml_value_8 = sysml_conditional_17;
    bool sysml_value_9 = 0;
    if ((((double)sysml_state_numberValue) / 10) >= 8) {
        sysml_conditional_18 = true;
    }
    else {
        sysml_conditional_18 = false;
    }
    if (sysml_conditional_18) {
        sysml_conditional_19 = true;
    }
    else {
        sysml_conditional_19 = false;
    }
    sysml_value_9 = sysml_conditional_19;
    bool sysml_value_10 = 0;
    if ((((double)sysml_state_numberValue) / 10) >= 9) {
        sysml_conditional_20 = true;
    }
    else {
        sysml_conditional_20 = false;
    }
    if (sysml_conditional_20) {
        sysml_conditional_21 = true;
    }
    else {
        sysml_conditional_21 = false;
    }
    sysml_value_10 = sysml_conditional_21;
    bool sysml_value_11 = 0;
    if (sysml_state_ledPhase) {
        sysml_conditional_22 = true;
    }
    else {
        sysml_conditional_22 = false;
    }
    if (sysml_conditional_22) {
        sysml_conditional_23 = true;
    }
    else {
        sysml_conditional_23 = false;
    }
    sysml_value_11 = sysml_conditional_23;
    bool sysml_value_12 = 0;
    if (sysml_state_ledPhase) {
        sysml_conditional_24 = true;
    }
    else {
        sysml_conditional_24 = false;
    }
    if (sysml_conditional_24) {
        sysml_conditional_25 = false;
    }
    else {
        sysml_conditional_25 = true;
    }
    sysml_value_12 = sysml_conditional_25;
    *io_0 = sysml_value_0;
    *io_1_segment_0 = sysml_value_1;
    *io_1_segment_1 = sysml_value_2;
    *io_1_segment_2 = sysml_value_3;
    *io_1_segment_3 = sysml_value_4;
    *io_1_segment_4 = sysml_value_5;
    *io_1_segment_5 = sysml_value_6;
    *io_1_segment_6 = sysml_value_7;
    *io_1_segment_7 = sysml_value_8;
    *io_1_segment_8 = sysml_value_9;
    *io_1_segment_9 = sysml_value_10;
    *io_2 = sysml_value_11;
    *io_3 = sysml_value_12;
}