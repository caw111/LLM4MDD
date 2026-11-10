#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_accumulatedMs = 0;
/* Global Variable define of  */
int sysml_state_seed = 1;
/* Global Variable define of  */
int sysml_state_ledCount = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_accumulatedMs = 0;
    sysml_state_seed = 1;
    sysml_state_ledCount = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_4, bool* io_0, bool* io_1, bool* io_2, int* io_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    int sysml_arithmetic_1 = 0;
    int sysml_conditional_2 = 0;
    int sysml_arithmetic_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_arithmetic_5 = 0;
    int sysml_conditional_6 = 0;
    int sysml_conditional_7 = 0;
    int sysml_arithmetic_8 = 0;
    int sysml_conditional_9 = 0;
    int sysml_arithmetic_10 = 0;
    int sysml_conditional_11 = 0;
    int sysml_arithmetic_12 = 0;
    int sysml_conditional_13 = 0;
    int sysml_conditional_14 = 0;
    int sysml_arithmetic_15 = 0;
    int sysml_conditional_16 = 0;
    int sysml_arithmetic_17 = 0;
    int sysml_conditional_18 = 0;
    int sysml_arithmetic_19 = 0;
    int sysml_conditional_20 = 0;
    int sysml_conditional_21 = 0;
    int sysml_arithmetic_22 = 0;
    int sysml_conditional_23 = 0;
    int sysml_arithmetic_24 = 0;
    int sysml_conditional_25 = 0;
    int sysml_arithmetic_26 = 0;
    int sysml_conditional_27 = 0;
    int sysml_arithmetic_28 = 0;
    int sysml_conditional_29 = 0;
    int sysml_arithmetic_30 = 0;
    int sysml_conditional_31 = 0;
    int sysml_arithmetic_32 = 0;
    sysml_state_accumulatedMs = (sysml_state_accumulatedMs + io_4);
    if ((sysml_state_accumulatedMs >= 2000)) {
        sysml_state_accumulatedMs = (sysml_state_accumulatedMs - 2000);
        sysml_state_seed = ((sysml_state_seed * 1103515245) + 12345);
        if ((((double)sysml_state_seed) / 4) < ((double)((int)(((double)sysml_state_seed) / 4)))) {
            sysml_conditional_0 = 1;
        }
        else {
            sysml_conditional_0 = 0;
        }
        sysml_arithmetic_1 = ((int)(((double)sysml_state_seed) / 4)) - sysml_conditional_0;
        if ((((double)sysml_state_seed) / 4) < ((double)((int)(((double)sysml_state_seed) / 4)))) {
            sysml_conditional_2 = 1;
        }
        else {
            sysml_conditional_2 = 0;
        }
        sysml_arithmetic_3 = ((int)(((double)sysml_state_seed) / 4)) - sysml_conditional_2;
        if ((((double)sysml_state_seed) / 4) < ((double)((int)(((double)sysml_state_seed) / 4)))) {
            sysml_conditional_4 = 1;
        }
        else {
            sysml_conditional_4 = 0;
        }
        sysml_arithmetic_5 = ((int)(((double)sysml_state_seed) / 4)) - sysml_conditional_4;
        if ((((double)(sysml_state_seed - (4 * sysml_arithmetic_3))) / 1) < ((double)((int)(((double)(sysml_state_seed - (4 * sysml_arithmetic_5))) / 1)))) {
            sysml_conditional_6 = 1;
        }
        else {
            sysml_conditional_6 = 0;
        }
        if ((((double)sysml_state_seed) / 4) < ((double)((int)(((double)sysml_state_seed) / 4)))) {
            sysml_conditional_7 = 1;
        }
        else {
            sysml_conditional_7 = 0;
        }
        sysml_arithmetic_8 = ((int)(((double)sysml_state_seed) / 4)) - sysml_conditional_7;
        if ((((double)sysml_state_seed) / 4) < ((double)((int)(((double)sysml_state_seed) / 4)))) {
            sysml_conditional_9 = 1;
        }
        else {
            sysml_conditional_9 = 0;
        }
        sysml_arithmetic_10 = ((int)(((double)sysml_state_seed) / 4)) - sysml_conditional_9;
        if ((((double)sysml_state_seed) / 4) < ((double)((int)(((double)sysml_state_seed) / 4)))) {
            sysml_conditional_11 = 1;
        }
        else {
            sysml_conditional_11 = 0;
        }
        sysml_arithmetic_12 = ((int)(((double)sysml_state_seed) / 4)) - sysml_conditional_11;
        if ((((double)(sysml_state_seed - (4 * sysml_arithmetic_10))) / 1) < ((double)((int)(((double)(sysml_state_seed - (4 * sysml_arithmetic_12))) / 1)))) {
            sysml_conditional_13 = 1;
        }
        else {
            sysml_conditional_13 = 0;
        }
        if ((((double)sysml_state_seed) / 4) < ((double)((int)(((double)sysml_state_seed) / 4)))) {
            sysml_conditional_14 = 1;
        }
        else {
            sysml_conditional_14 = 0;
        }
        sysml_arithmetic_15 = ((int)(((double)sysml_state_seed) / 4)) - sysml_conditional_14;
        if ((((double)sysml_state_seed) / 4) < ((double)((int)(((double)sysml_state_seed) / 4)))) {
            sysml_conditional_16 = 1;
        }
        else {
            sysml_conditional_16 = 0;
        }
        sysml_arithmetic_17 = ((int)(((double)sysml_state_seed) / 4)) - sysml_conditional_16;
        if ((((double)sysml_state_seed) / 4) < ((double)((int)(((double)sysml_state_seed) / 4)))) {
            sysml_conditional_18 = 1;
        }
        else {
            sysml_conditional_18 = 0;
        }
        sysml_arithmetic_19 = ((int)(((double)sysml_state_seed) / 4)) - sysml_conditional_18;
        if ((((double)(sysml_state_seed - (4 * sysml_arithmetic_17))) / 1) < ((double)((int)(((double)(sysml_state_seed - (4 * sysml_arithmetic_19))) / 1)))) {
            sysml_conditional_20 = 1;
        }
        else {
            sysml_conditional_20 = 0;
        }
        if ((((double)(((int)(((double)(sysml_state_seed - (4 * sysml_arithmetic_8))) / 1)) - sysml_conditional_13)) / 2) < ((double)((int)(((double)(((int)(((double)(sysml_state_seed - (4 * sysml_arithmetic_15))) / 1)) - sysml_conditional_20)) / 2)))) {
            sysml_conditional_21 = 1;
        }
        else {
            sysml_conditional_21 = 0;
        }
        sysml_arithmetic_22 = ((int)(((double)(((int)(((double)(sysml_state_seed - (4 * sysml_arithmetic_1))) / 1)) - sysml_conditional_6)) / 2)) - sysml_conditional_21;
        if ((((double)sysml_state_seed) / 4) < ((double)((int)(((double)sysml_state_seed) / 4)))) {
            sysml_conditional_23 = 1;
        }
        else {
            sysml_conditional_23 = 0;
        }
        sysml_arithmetic_24 = ((int)(((double)sysml_state_seed) / 4)) - sysml_conditional_23;
        if ((((double)sysml_state_seed) / 4) < ((double)((int)(((double)sysml_state_seed) / 4)))) {
            sysml_conditional_25 = 1;
        }
        else {
            sysml_conditional_25 = 0;
        }
        sysml_arithmetic_26 = ((int)(((double)sysml_state_seed) / 4)) - sysml_conditional_25;
        if ((((double)sysml_state_seed) / 4) < ((double)((int)(((double)sysml_state_seed) / 4)))) {
            sysml_conditional_27 = 1;
        }
        else {
            sysml_conditional_27 = 0;
        }
        sysml_arithmetic_28 = ((int)(((double)sysml_state_seed) / 4)) - sysml_conditional_27;
        if ((((double)sysml_state_seed) / 4) < ((double)((int)(((double)sysml_state_seed) / 4)))) {
            sysml_conditional_29 = 1;
        }
        else {
            sysml_conditional_29 = 0;
        }
        sysml_arithmetic_30 = ((int)(((double)sysml_state_seed) / 4)) - sysml_conditional_29;
        if ((((double)(sysml_state_seed - (4 * sysml_arithmetic_28))) / 2) < ((double)((int)(((double)(sysml_state_seed - (4 * sysml_arithmetic_30))) / 2)))) {
            sysml_conditional_31 = 1;
        }
        else {
            sysml_conditional_31 = 0;
        }
        sysml_arithmetic_32 = ((int)(((double)(sysml_state_seed - (4 * sysml_arithmetic_26))) / 2)) - sysml_conditional_31;
        sysml_state_ledCount = ((2 * sysml_arithmetic_22) + (sysml_state_seed - (4 * sysml_arithmetic_24))) - (2 * sysml_arithmetic_32);
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = (sysml_state_ledCount >= 1);
    bool sysml_value_1 = 0;
    sysml_value_1 = (sysml_state_ledCount >= 2);
    bool sysml_value_2 = 0;
    sysml_value_2 = (sysml_state_ledCount >= 3);
    int sysml_value_3 = 0;
    sysml_value_3 = sysml_state_ledCount;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
}