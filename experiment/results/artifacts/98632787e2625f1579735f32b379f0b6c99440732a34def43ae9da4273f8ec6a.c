#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_accumMs = 0;
/* Global Variable define of  */
int sysml_state_displayNumber = 0;
/* Global Variable define of  */
bool sysml_state_ledPhase = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_accumMs = 0;
    sysml_state_displayNumber = 0;
    sysml_state_ledPhase = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_0, int* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7, bool* io_8, bool* io_9, bool* io_10, bool* io_11, bool* io_12, bool* io_13, bool* io_14) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    int sysml_conditional_1 = 0;
    bool sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    bool sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    bool sysml_conditional_6 = 0;
    int sysml_conditional_7 = 0;
    bool sysml_conditional_8 = 0;
    int sysml_conditional_9 = 0;
    bool sysml_conditional_10 = 0;
    int sysml_conditional_11 = 0;
    bool sysml_conditional_12 = 0;
    int sysml_conditional_13 = 0;
    bool sysml_conditional_14 = 0;
    int sysml_conditional_15 = 0;
    bool sysml_conditional_16 = 0;
    int sysml_conditional_17 = 0;
    bool sysml_conditional_18 = 0;
    int sysml_conditional_19 = 0;
    bool sysml_conditional_20 = 0;
    bool sysml_conditional_21 = 0;
    sysml_state_accumMs = (sysml_state_accumMs + io_0);
    if ((sysml_state_accumMs >= 2000)) {
        sysml_state_accumMs = (sysml_state_accumMs - 2000);
        if (sysml_state_displayNumber >= 99) {
            sysml_conditional_0 = 0;
        }
        else {
            sysml_conditional_0 = sysml_state_displayNumber + 1;
        }
        sysml_state_displayNumber = sysml_conditional_0;
        sysml_state_ledPhase = (!sysml_state_ledPhase);
    }
    int sysml_value_0 = 0;
    sysml_value_0 = sysml_state_displayNumber;
    bool sysml_value_1 = 0;
    if ((((double)sysml_state_displayNumber) / 10) < ((double)((int)(((double)sysml_state_displayNumber) / 10)))) {
        sysml_conditional_1 = 1;
    }
    else {
        sysml_conditional_1 = 0;
    }
    if (((((int)(((double)sysml_state_displayNumber) / 10)) - sysml_conditional_1) + 1) >= 1) {
        sysml_conditional_2 = true;
    }
    else {
        sysml_conditional_2 = false;
    }
    sysml_value_1 = sysml_conditional_2;
    bool sysml_value_2 = 0;
    if ((((double)sysml_state_displayNumber) / 10) < ((double)((int)(((double)sysml_state_displayNumber) / 10)))) {
        sysml_conditional_3 = 1;
    }
    else {
        sysml_conditional_3 = 0;
    }
    if (((((int)(((double)sysml_state_displayNumber) / 10)) - sysml_conditional_3) + 1) >= 2) {
        sysml_conditional_4 = true;
    }
    else {
        sysml_conditional_4 = false;
    }
    sysml_value_2 = sysml_conditional_4;
    bool sysml_value_3 = 0;
    if ((((double)sysml_state_displayNumber) / 10) < ((double)((int)(((double)sysml_state_displayNumber) / 10)))) {
        sysml_conditional_5 = 1;
    }
    else {
        sysml_conditional_5 = 0;
    }
    if (((((int)(((double)sysml_state_displayNumber) / 10)) - sysml_conditional_5) + 1) >= 3) {
        sysml_conditional_6 = true;
    }
    else {
        sysml_conditional_6 = false;
    }
    sysml_value_3 = sysml_conditional_6;
    bool sysml_value_4 = 0;
    if ((((double)sysml_state_displayNumber) / 10) < ((double)((int)(((double)sysml_state_displayNumber) / 10)))) {
        sysml_conditional_7 = 1;
    }
    else {
        sysml_conditional_7 = 0;
    }
    if (((((int)(((double)sysml_state_displayNumber) / 10)) - sysml_conditional_7) + 1) >= 4) {
        sysml_conditional_8 = true;
    }
    else {
        sysml_conditional_8 = false;
    }
    sysml_value_4 = sysml_conditional_8;
    bool sysml_value_5 = 0;
    if ((((double)sysml_state_displayNumber) / 10) < ((double)((int)(((double)sysml_state_displayNumber) / 10)))) {
        sysml_conditional_9 = 1;
    }
    else {
        sysml_conditional_9 = 0;
    }
    if (((((int)(((double)sysml_state_displayNumber) / 10)) - sysml_conditional_9) + 1) >= 5) {
        sysml_conditional_10 = true;
    }
    else {
        sysml_conditional_10 = false;
    }
    sysml_value_5 = sysml_conditional_10;
    bool sysml_value_6 = 0;
    if ((((double)sysml_state_displayNumber) / 10) < ((double)((int)(((double)sysml_state_displayNumber) / 10)))) {
        sysml_conditional_11 = 1;
    }
    else {
        sysml_conditional_11 = 0;
    }
    if (((((int)(((double)sysml_state_displayNumber) / 10)) - sysml_conditional_11) + 1) >= 6) {
        sysml_conditional_12 = true;
    }
    else {
        sysml_conditional_12 = false;
    }
    sysml_value_6 = sysml_conditional_12;
    bool sysml_value_7 = 0;
    if ((((double)sysml_state_displayNumber) / 10) < ((double)((int)(((double)sysml_state_displayNumber) / 10)))) {
        sysml_conditional_13 = 1;
    }
    else {
        sysml_conditional_13 = 0;
    }
    if (((((int)(((double)sysml_state_displayNumber) / 10)) - sysml_conditional_13) + 1) >= 7) {
        sysml_conditional_14 = true;
    }
    else {
        sysml_conditional_14 = false;
    }
    sysml_value_7 = sysml_conditional_14;
    bool sysml_value_8 = 0;
    if ((((double)sysml_state_displayNumber) / 10) < ((double)((int)(((double)sysml_state_displayNumber) / 10)))) {
        sysml_conditional_15 = 1;
    }
    else {
        sysml_conditional_15 = 0;
    }
    if (((((int)(((double)sysml_state_displayNumber) / 10)) - sysml_conditional_15) + 1) >= 8) {
        sysml_conditional_16 = true;
    }
    else {
        sysml_conditional_16 = false;
    }
    sysml_value_8 = sysml_conditional_16;
    bool sysml_value_9 = 0;
    if ((((double)sysml_state_displayNumber) / 10) < ((double)((int)(((double)sysml_state_displayNumber) / 10)))) {
        sysml_conditional_17 = 1;
    }
    else {
        sysml_conditional_17 = 0;
    }
    if (((((int)(((double)sysml_state_displayNumber) / 10)) - sysml_conditional_17) + 1) >= 9) {
        sysml_conditional_18 = true;
    }
    else {
        sysml_conditional_18 = false;
    }
    sysml_value_9 = sysml_conditional_18;
    bool sysml_value_10 = 0;
    if ((((double)sysml_state_displayNumber) / 10) < ((double)((int)(((double)sysml_state_displayNumber) / 10)))) {
        sysml_conditional_19 = 1;
    }
    else {
        sysml_conditional_19 = 0;
    }
    if (((((int)(((double)sysml_state_displayNumber) / 10)) - sysml_conditional_19) + 1) >= 10) {
        sysml_conditional_20 = true;
    }
    else {
        sysml_conditional_20 = false;
    }
    sysml_value_10 = sysml_conditional_20;
    bool sysml_value_11 = 0;
    sysml_value_11 = sysml_state_ledPhase;
    bool sysml_value_12 = 0;
    if (sysml_state_ledPhase) {
        sysml_conditional_21 = false;
    }
    else {
        sysml_conditional_21 = true;
    }
    sysml_value_12 = sysml_conditional_21;
    bool sysml_value_13 = 0;
    sysml_value_13 = false;
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
    *io_3 = sysml_value_2;
    *io_4 = sysml_value_3;
    *io_5 = sysml_value_4;
    *io_6 = sysml_value_5;
    *io_7 = sysml_value_6;
    *io_8 = sysml_value_7;
    *io_9 = sysml_value_8;
    *io_10 = sysml_value_9;
    *io_11 = sysml_value_10;
    *io_12 = sysml_value_11;
    *io_13 = sysml_value_12;
    *io_14 = sysml_value_13;
}