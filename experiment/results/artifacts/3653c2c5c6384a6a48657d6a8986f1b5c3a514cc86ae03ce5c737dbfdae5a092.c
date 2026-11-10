#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_displayValue = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_displayValue = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_5, bool io_5_valid, bool* io_0, bool* io_1, bool* io_2, bool* io_3, bool* io_4, bool* serial_0_trigger, int* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    int sysml_arithmetic_1 = 0;
    int sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    int sysml_conditional_6 = 0;
    int sysml_arithmetic_7 = 0;
    int sysml_conditional_8 = 0;
    int sysml_conditional_9 = 0;
    int sysml_conditional_10 = 0;
    int sysml_conditional_11 = 0;
    int sysml_conditional_12 = 0;
    int sysml_arithmetic_13 = 0;
    int sysml_conditional_14 = 0;
    int sysml_conditional_15 = 0;
    int sysml_conditional_16 = 0;
    int sysml_conditional_17 = 0;
    int sysml_conditional_18 = 0;
    int sysml_arithmetic_19 = 0;
    int sysml_conditional_20 = 0;
    int sysml_conditional_21 = 0;
    int sysml_conditional_22 = 0;
    int sysml_conditional_23 = 0;
    int sysml_conditional_24 = 0;
    int sysml_arithmetic_25 = 0;
    if (io_5_valid) {
        sysml_state_displayValue = io_5;
    }
    bool sysml_value_0 = 0;
    if ((((double)sysml_state_displayValue) / 2) < ((double)((int)(((double)sysml_state_displayValue) / 2)))) {
        sysml_conditional_0 = 1;
    }
    else {
        sysml_conditional_0 = 0;
    }
    sysml_arithmetic_1 = ((int)(((double)sysml_state_displayValue) / 2)) - sysml_conditional_0;
    sysml_value_0 = (sysml_state_displayValue - (1 * sysml_arithmetic_1)) == 1;
    bool sysml_value_1 = 0;
    if ((((double)sysml_state_displayValue) / 2) < ((double)((int)(((double)sysml_state_displayValue) / 2)))) {
        sysml_conditional_2 = 1;
    }
    else {
        sysml_conditional_2 = 0;
    }
    if ((((double)sysml_state_displayValue) / 2) < ((double)((int)(((double)sysml_state_displayValue) / 2)))) {
        sysml_conditional_3 = 1;
    }
    else {
        sysml_conditional_3 = 0;
    }
    if ((((double)sysml_state_displayValue) / 2) < ((double)((int)(((double)sysml_state_displayValue) / 2)))) {
        sysml_conditional_4 = 1;
    }
    else {
        sysml_conditional_4 = 0;
    }
    if ((((double)sysml_state_displayValue) / 2) < ((double)((int)(((double)sysml_state_displayValue) / 2)))) {
        sysml_conditional_5 = 1;
    }
    else {
        sysml_conditional_5 = 0;
    }
    if ((((double)(((int)(((double)sysml_state_displayValue) / 2)) - sysml_conditional_4)) / 2) < ((double)((int)(((double)(((int)(((double)sysml_state_displayValue) / 2)) - sysml_conditional_5)) / 2)))) {
        sysml_conditional_6 = 1;
    }
    else {
        sysml_conditional_6 = 0;
    }
    sysml_arithmetic_7 = ((int)(((double)(((int)(((double)sysml_state_displayValue) / 2)) - sysml_conditional_3)) / 2)) - sysml_conditional_6;
    sysml_value_1 = ((((int)(((double)sysml_state_displayValue) / 2)) - sysml_conditional_2) - (1 * sysml_arithmetic_7)) == 1;
    bool sysml_value_2 = 0;
    if ((((double)sysml_state_displayValue) / 4) < ((double)((int)(((double)sysml_state_displayValue) / 4)))) {
        sysml_conditional_8 = 1;
    }
    else {
        sysml_conditional_8 = 0;
    }
    if ((((double)sysml_state_displayValue) / 4) < ((double)((int)(((double)sysml_state_displayValue) / 4)))) {
        sysml_conditional_9 = 1;
    }
    else {
        sysml_conditional_9 = 0;
    }
    if ((((double)sysml_state_displayValue) / 4) < ((double)((int)(((double)sysml_state_displayValue) / 4)))) {
        sysml_conditional_10 = 1;
    }
    else {
        sysml_conditional_10 = 0;
    }
    if ((((double)sysml_state_displayValue) / 4) < ((double)((int)(((double)sysml_state_displayValue) / 4)))) {
        sysml_conditional_11 = 1;
    }
    else {
        sysml_conditional_11 = 0;
    }
    if ((((double)(((int)(((double)sysml_state_displayValue) / 4)) - sysml_conditional_10)) / 2) < ((double)((int)(((double)(((int)(((double)sysml_state_displayValue) / 4)) - sysml_conditional_11)) / 2)))) {
        sysml_conditional_12 = 1;
    }
    else {
        sysml_conditional_12 = 0;
    }
    sysml_arithmetic_13 = ((int)(((double)(((int)(((double)sysml_state_displayValue) / 4)) - sysml_conditional_9)) / 2)) - sysml_conditional_12;
    sysml_value_2 = ((((int)(((double)sysml_state_displayValue) / 4)) - sysml_conditional_8) - (1 * sysml_arithmetic_13)) == 1;
    bool sysml_value_3 = 0;
    if ((((double)sysml_state_displayValue) / 8) < ((double)((int)(((double)sysml_state_displayValue) / 8)))) {
        sysml_conditional_14 = 1;
    }
    else {
        sysml_conditional_14 = 0;
    }
    if ((((double)sysml_state_displayValue) / 8) < ((double)((int)(((double)sysml_state_displayValue) / 8)))) {
        sysml_conditional_15 = 1;
    }
    else {
        sysml_conditional_15 = 0;
    }
    if ((((double)sysml_state_displayValue) / 8) < ((double)((int)(((double)sysml_state_displayValue) / 8)))) {
        sysml_conditional_16 = 1;
    }
    else {
        sysml_conditional_16 = 0;
    }
    if ((((double)sysml_state_displayValue) / 8) < ((double)((int)(((double)sysml_state_displayValue) / 8)))) {
        sysml_conditional_17 = 1;
    }
    else {
        sysml_conditional_17 = 0;
    }
    if ((((double)(((int)(((double)sysml_state_displayValue) / 8)) - sysml_conditional_16)) / 2) < ((double)((int)(((double)(((int)(((double)sysml_state_displayValue) / 8)) - sysml_conditional_17)) / 2)))) {
        sysml_conditional_18 = 1;
    }
    else {
        sysml_conditional_18 = 0;
    }
    sysml_arithmetic_19 = ((int)(((double)(((int)(((double)sysml_state_displayValue) / 8)) - sysml_conditional_15)) / 2)) - sysml_conditional_18;
    sysml_value_3 = ((((int)(((double)sysml_state_displayValue) / 8)) - sysml_conditional_14) - (1 * sysml_arithmetic_19)) == 1;
    bool sysml_value_4 = 0;
    if ((((double)sysml_state_displayValue) / 16) < ((double)((int)(((double)sysml_state_displayValue) / 16)))) {
        sysml_conditional_20 = 1;
    }
    else {
        sysml_conditional_20 = 0;
    }
    if ((((double)sysml_state_displayValue) / 16) < ((double)((int)(((double)sysml_state_displayValue) / 16)))) {
        sysml_conditional_21 = 1;
    }
    else {
        sysml_conditional_21 = 0;
    }
    if ((((double)sysml_state_displayValue) / 16) < ((double)((int)(((double)sysml_state_displayValue) / 16)))) {
        sysml_conditional_22 = 1;
    }
    else {
        sysml_conditional_22 = 0;
    }
    if ((((double)sysml_state_displayValue) / 16) < ((double)((int)(((double)sysml_state_displayValue) / 16)))) {
        sysml_conditional_23 = 1;
    }
    else {
        sysml_conditional_23 = 0;
    }
    if ((((double)(((int)(((double)sysml_state_displayValue) / 16)) - sysml_conditional_22)) / 2) < ((double)((int)(((double)(((int)(((double)sysml_state_displayValue) / 16)) - sysml_conditional_23)) / 2)))) {
        sysml_conditional_24 = 1;
    }
    else {
        sysml_conditional_24 = 0;
    }
    sysml_arithmetic_25 = ((int)(((double)(((int)(((double)sysml_state_displayValue) / 16)) - sysml_conditional_21)) / 2)) - sysml_conditional_24;
    sysml_value_4 = ((((int)(((double)sysml_state_displayValue) / 16)) - sysml_conditional_20) - (1 * sysml_arithmetic_25)) == 1;
    bool sysml_value_5 = 0;
    sysml_value_5 = io_5_valid;
    int sysml_value_6 = 0;
    sysml_value_6 = sysml_state_displayValue;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
    *io_4 = sysml_value_4;
    *serial_0_trigger = sysml_value_5;
    *serial_0_value_0 = sysml_value_6;
}