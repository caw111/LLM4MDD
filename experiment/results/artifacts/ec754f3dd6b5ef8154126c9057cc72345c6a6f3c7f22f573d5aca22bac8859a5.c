#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_currentNumber = 0;
/* Global Variable define of  */
bool sysml_state_prevBtn1Pressed = false;
/* Global Variable define of  */
bool sysml_state_prevBtn2Pressed = false;
/* Global Variable define of  */
bool sysml_state_prevBtn3Pressed = false;
/* Global Variable define of  */
bool sysml_state_btn1Stable = false;
/* Global Variable define of  */
bool sysml_state_btn2Stable = false;
/* Global Variable define of  */
bool sysml_state_btn3Stable = false;
/* Global Variable define of  */
int sysml_state_btn1ConfirmCounter = 0;
/* Global Variable define of  */
int sysml_state_btn2ConfirmCounter = 0;
/* Global Variable define of  */
int sysml_state_btn3ConfirmCounter = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_currentNumber = 0;
    sysml_state_prevBtn1Pressed = false;
    sysml_state_prevBtn2Pressed = false;
    sysml_state_prevBtn3Pressed = false;
    sysml_state_btn1Stable = false;
    sysml_state_btn2Stable = false;
    sysml_state_btn3Stable = false;
    sysml_state_btn1ConfirmCounter = 0;
    sysml_state_btn2ConfirmCounter = 0;
    sysml_state_btn3ConfirmCounter = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, int* io_3, bool* io_4_segment_0, bool* io_4_segment_1, bool* io_4_segment_2, bool* io_4_segment_3, bool* io_4_segment_4, bool* io_4_segment_5, bool* io_4_segment_6, bool* io_4_segment_7, bool* io_4_segment_8, bool* io_4_segment_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    bool sysml_lazy_9 = 0;
    int sysml_conditional_10 = 0;
    bool sysml_lazy_11 = 0;
    bool sysml_lazy_12 = 0;
    int sysml_conditional_13 = 0;
    int sysml_conditional_14 = 0;
    int sysml_arithmetic_15 = 0;
    bool sysml_lazy_16 = 0;
    int sysml_conditional_17 = 0;
    bool sysml_conditional_18 = 0;
    int sysml_conditional_19 = 0;
    bool sysml_conditional_20 = 0;
    int sysml_conditional_21 = 0;
    bool sysml_conditional_22 = 0;
    int sysml_conditional_23 = 0;
    bool sysml_conditional_24 = 0;
    int sysml_conditional_25 = 0;
    bool sysml_conditional_26 = 0;
    int sysml_conditional_27 = 0;
    bool sysml_conditional_28 = 0;
    int sysml_conditional_29 = 0;
    bool sysml_conditional_30 = 0;
    int sysml_conditional_31 = 0;
    bool sysml_conditional_32 = 0;
    int sysml_conditional_33 = 0;
    bool sysml_conditional_34 = 0;
    int sysml_conditional_35 = 0;
    bool sysml_conditional_36 = 0;
    sysml_lazy_0 = io_2;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_prevBtn3Pressed;
    }
    if (sysml_lazy_0) {
        sysml_state_btn3ConfirmCounter = 1;
    }
    if ((!io_2)) {
        sysml_state_btn3ConfirmCounter = 0;
        sysml_state_btn3Stable = false;
    }
    sysml_lazy_1 = io_2;
    if (sysml_lazy_1) {
        sysml_lazy_1 = sysml_state_btn3ConfirmCounter < 3;
    }
    if (sysml_lazy_1) {
        sysml_state_btn3ConfirmCounter = (sysml_state_btn3ConfirmCounter + 1);
    }
    sysml_lazy_2 = io_2;
    if (sysml_lazy_2) {
        sysml_lazy_2 = sysml_state_btn3ConfirmCounter >= 3;
    }
    if (sysml_lazy_2) {
        sysml_state_btn3Stable = true;
    }
    if (sysml_state_btn3Stable) {
        sysml_state_currentNumber = 0;
    }
    sysml_lazy_3 = io_0;
    if (sysml_lazy_3) {
        sysml_lazy_3 = !sysml_state_prevBtn1Pressed;
    }
    if (sysml_lazy_3) {
        sysml_state_btn1ConfirmCounter = 1;
    }
    if ((!io_0)) {
        sysml_state_btn1ConfirmCounter = 0;
        sysml_state_btn1Stable = false;
    }
    sysml_lazy_4 = io_0;
    if (sysml_lazy_4) {
        sysml_lazy_4 = sysml_state_btn1ConfirmCounter < 3;
    }
    if (sysml_lazy_4) {
        sysml_state_btn1ConfirmCounter = (sysml_state_btn1ConfirmCounter + 1);
    }
    sysml_lazy_5 = io_0;
    if (sysml_lazy_5) {
        sysml_lazy_5 = sysml_state_btn1ConfirmCounter >= 3;
    }
    if (sysml_lazy_5) {
        sysml_state_btn1Stable = true;
    }
    sysml_lazy_6 = io_1;
    if (sysml_lazy_6) {
        sysml_lazy_6 = !sysml_state_prevBtn2Pressed;
    }
    if (sysml_lazy_6) {
        sysml_state_btn2ConfirmCounter = 1;
    }
    if ((!io_1)) {
        sysml_state_btn2ConfirmCounter = 0;
        sysml_state_btn2Stable = false;
    }
    sysml_lazy_7 = io_1;
    if (sysml_lazy_7) {
        sysml_lazy_7 = sysml_state_btn2ConfirmCounter < 3;
    }
    if (sysml_lazy_7) {
        sysml_state_btn2ConfirmCounter = (sysml_state_btn2ConfirmCounter + 1);
    }
    sysml_lazy_8 = io_1;
    if (sysml_lazy_8) {
        sysml_lazy_8 = sysml_state_btn2ConfirmCounter >= 3;
    }
    if (sysml_lazy_8) {
        sysml_state_btn2Stable = true;
    }
    sysml_lazy_9 = !sysml_state_btn3Stable;
    if (sysml_lazy_9) {
        sysml_lazy_9 = sysml_state_btn1Stable;
    }
    if (sysml_lazy_9) {
        if (sysml_state_currentNumber >= 99) {
            sysml_conditional_10 = 0;
        }
        else {
            sysml_conditional_10 = sysml_state_currentNumber + 1;
        }
        sysml_state_currentNumber = sysml_conditional_10;
    }
    sysml_lazy_11 = !sysml_state_btn3Stable;
    if (sysml_lazy_11) {
        sysml_lazy_11 = !sysml_state_btn1Stable;
    }
    sysml_lazy_12 = sysml_lazy_11;
    if (sysml_lazy_12) {
        sysml_lazy_12 = sysml_state_btn2Stable;
    }
    if (sysml_lazy_12) {
        if (sysml_state_currentNumber >= 95) {
            sysml_conditional_13 = 0;
        }
        else {
            sysml_conditional_13 = sysml_state_currentNumber + 5;
        }
        sysml_state_currentNumber = sysml_conditional_13;
    }
    sysml_state_prevBtn1Pressed = io_0;
    sysml_state_prevBtn2Pressed = io_1;
    sysml_state_prevBtn3Pressed = io_2;
    int sysml_value_0 = 0;
    sysml_value_0 = sysml_state_currentNumber;
    bool sysml_value_1 = 0;
    if ((((double)sysml_state_currentNumber) / 10) < ((double)((int)(((double)sysml_state_currentNumber) / 10)))) {
        sysml_conditional_14 = 1;
    }
    else {
        sysml_conditional_14 = 0;
    }
    sysml_arithmetic_15 = ((int)(((double)sysml_state_currentNumber) / 10)) - sysml_conditional_14;
    sysml_lazy_16 = sysml_state_currentNumber == (10 * sysml_arithmetic_15);
    if (sysml_lazy_16) {
        if ((((double)sysml_state_currentNumber) / 10) < ((double)((int)(((double)sysml_state_currentNumber) / 10)))) {
            sysml_conditional_17 = 1;
        }
        else {
            sysml_conditional_17 = 0;
        }
        sysml_lazy_16 = (((int)(((double)sysml_state_currentNumber) / 10)) - sysml_conditional_17) >= 1;
    }
    if (sysml_lazy_16) {
        sysml_conditional_18 = true;
    }
    else {
        sysml_conditional_18 = false;
    }
    sysml_value_1 = sysml_conditional_18;
    bool sysml_value_2 = 0;
    if ((((double)sysml_state_currentNumber) / 10) < ((double)((int)(((double)sysml_state_currentNumber) / 10)))) {
        sysml_conditional_19 = 1;
    }
    else {
        sysml_conditional_19 = 0;
    }
    if ((((int)(((double)sysml_state_currentNumber) / 10)) - sysml_conditional_19) >= 2) {
        sysml_conditional_20 = true;
    }
    else {
        sysml_conditional_20 = false;
    }
    sysml_value_2 = sysml_conditional_20;
    bool sysml_value_3 = 0;
    if ((((double)sysml_state_currentNumber) / 10) < ((double)((int)(((double)sysml_state_currentNumber) / 10)))) {
        sysml_conditional_21 = 1;
    }
    else {
        sysml_conditional_21 = 0;
    }
    if ((((int)(((double)sysml_state_currentNumber) / 10)) - sysml_conditional_21) >= 3) {
        sysml_conditional_22 = true;
    }
    else {
        sysml_conditional_22 = false;
    }
    sysml_value_3 = sysml_conditional_22;
    bool sysml_value_4 = 0;
    if ((((double)sysml_state_currentNumber) / 10) < ((double)((int)(((double)sysml_state_currentNumber) / 10)))) {
        sysml_conditional_23 = 1;
    }
    else {
        sysml_conditional_23 = 0;
    }
    if ((((int)(((double)sysml_state_currentNumber) / 10)) - sysml_conditional_23) >= 4) {
        sysml_conditional_24 = true;
    }
    else {
        sysml_conditional_24 = false;
    }
    sysml_value_4 = sysml_conditional_24;
    bool sysml_value_5 = 0;
    if ((((double)sysml_state_currentNumber) / 10) < ((double)((int)(((double)sysml_state_currentNumber) / 10)))) {
        sysml_conditional_25 = 1;
    }
    else {
        sysml_conditional_25 = 0;
    }
    if ((((int)(((double)sysml_state_currentNumber) / 10)) - sysml_conditional_25) >= 5) {
        sysml_conditional_26 = true;
    }
    else {
        sysml_conditional_26 = false;
    }
    sysml_value_5 = sysml_conditional_26;
    bool sysml_value_6 = 0;
    if ((((double)sysml_state_currentNumber) / 10) < ((double)((int)(((double)sysml_state_currentNumber) / 10)))) {
        sysml_conditional_27 = 1;
    }
    else {
        sysml_conditional_27 = 0;
    }
    if ((((int)(((double)sysml_state_currentNumber) / 10)) - sysml_conditional_27) >= 6) {
        sysml_conditional_28 = true;
    }
    else {
        sysml_conditional_28 = false;
    }
    sysml_value_6 = sysml_conditional_28;
    bool sysml_value_7 = 0;
    if ((((double)sysml_state_currentNumber) / 10) < ((double)((int)(((double)sysml_state_currentNumber) / 10)))) {
        sysml_conditional_29 = 1;
    }
    else {
        sysml_conditional_29 = 0;
    }
    if ((((int)(((double)sysml_state_currentNumber) / 10)) - sysml_conditional_29) >= 7) {
        sysml_conditional_30 = true;
    }
    else {
        sysml_conditional_30 = false;
    }
    sysml_value_7 = sysml_conditional_30;
    bool sysml_value_8 = 0;
    if ((((double)sysml_state_currentNumber) / 10) < ((double)((int)(((double)sysml_state_currentNumber) / 10)))) {
        sysml_conditional_31 = 1;
    }
    else {
        sysml_conditional_31 = 0;
    }
    if ((((int)(((double)sysml_state_currentNumber) / 10)) - sysml_conditional_31) >= 8) {
        sysml_conditional_32 = true;
    }
    else {
        sysml_conditional_32 = false;
    }
    sysml_value_8 = sysml_conditional_32;
    bool sysml_value_9 = 0;
    if ((((double)sysml_state_currentNumber) / 10) < ((double)((int)(((double)sysml_state_currentNumber) / 10)))) {
        sysml_conditional_33 = 1;
    }
    else {
        sysml_conditional_33 = 0;
    }
    if ((((int)(((double)sysml_state_currentNumber) / 10)) - sysml_conditional_33) >= 9) {
        sysml_conditional_34 = true;
    }
    else {
        sysml_conditional_34 = false;
    }
    sysml_value_9 = sysml_conditional_34;
    bool sysml_value_10 = 0;
    if ((((double)sysml_state_currentNumber) / 10) < ((double)((int)(((double)sysml_state_currentNumber) / 10)))) {
        sysml_conditional_35 = 1;
    }
    else {
        sysml_conditional_35 = 0;
    }
    if ((((int)(((double)sysml_state_currentNumber) / 10)) - sysml_conditional_35) >= 10) {
        sysml_conditional_36 = true;
    }
    else {
        sysml_conditional_36 = false;
    }
    sysml_value_10 = sysml_conditional_36;
    *io_3 = sysml_value_0;
    *io_4_segment_0 = sysml_value_1;
    *io_4_segment_1 = sysml_value_2;
    *io_4_segment_2 = sysml_value_3;
    *io_4_segment_3 = sysml_value_4;
    *io_4_segment_4 = sysml_value_5;
    *io_4_segment_5 = sysml_value_6;
    *io_4_segment_6 = sysml_value_7;
    *io_4_segment_7 = sysml_value_8;
    *io_4_segment_8 = sysml_value_9;
    *io_4_segment_9 = sysml_value_10;
}