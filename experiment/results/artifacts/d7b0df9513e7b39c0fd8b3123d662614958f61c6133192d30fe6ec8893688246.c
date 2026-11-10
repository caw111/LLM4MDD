#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_btn1PrevPressed = false;
/* Global Variable define of  */
bool sysml_state_btn2PrevPressed = false;
/* Global Variable define of  */
bool sysml_state_btn3PrevPressed = false;
/* Global Variable define of  */
bool sysml_state_btn1StablePressed = false;
/* Global Variable define of  */
bool sysml_state_btn2StablePressed = false;
/* Global Variable define of  */
bool sysml_state_btn3StablePressed = false;
/* Global Variable define of  */
int sysml_state_btn1StableTimerMs = 0;
/* Global Variable define of  */
int sysml_state_btn2StableTimerMs = 0;
/* Global Variable define of  */
int sysml_state_btn3StableTimerMs = 0;
/* Global Variable define of  */
bool sysml_state_valueBit0 = false;
/* Global Variable define of  */
bool sysml_state_valueBit1 = false;
/* Global Variable define of  */
bool sysml_state_valueBit2 = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_btn1PrevPressed = false;
    sysml_state_btn2PrevPressed = false;
    sysml_state_btn3PrevPressed = false;
    sysml_state_btn1StablePressed = false;
    sysml_state_btn2StablePressed = false;
    sysml_state_btn3StablePressed = false;
    sysml_state_btn1StableTimerMs = 0;
    sysml_state_btn2StableTimerMs = 0;
    sysml_state_btn3StableTimerMs = 0;
    sysml_state_valueBit0 = false;
    sysml_state_valueBit1 = false;
    sysml_state_valueBit2 = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6_segment_0, bool* io_6_segment_1, bool* io_6_segment_2, bool* io_6_segment_3, bool* io_6_segment_4, bool* io_6_segment_5, bool* io_6_segment_6, bool* io_6_segment_7) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_conditional_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_lazy_10 = 0;
    bool sysml_conditional_11 = 0;
    bool sysml_lazy_12 = 0;
    bool sysml_lazy_13 = 0;
    bool sysml_conditional_14 = 0;
    bool sysml_lazy_15 = 0;
    bool sysml_lazy_16 = 0;
    bool sysml_conditional_17 = 0;
    bool sysml_lazy_18 = 0;
    bool sysml_lazy_19 = 0;
    bool sysml_conditional_20 = 0;
    bool sysml_lazy_21 = 0;
    bool sysml_lazy_22 = 0;
    bool sysml_conditional_23 = 0;
    bool sysml_lazy_24 = 0;
    bool sysml_lazy_25 = 0;
    bool sysml_conditional_26 = 0;
    bool sysml_lazy_27 = 0;
    bool sysml_lazy_28 = 0;
    bool sysml_conditional_29 = 0;
    if ((!io_0)) {
        sysml_state_btn1StablePressed = false;
        sysml_state_btn1StableTimerMs = 0;
    }
    else {
        if (sysml_state_btn1StablePressed) {
        }
        else {
            sysml_state_btn1StableTimerMs = (sysml_state_btn1StableTimerMs + 5);
            if (((sysml_state_btn1StableTimerMs + 5) >= 150)) {
                sysml_state_btn1StablePressed = true;
            }
        }
    }
    if ((!io_1)) {
        sysml_state_btn2StablePressed = false;
        sysml_state_btn2StableTimerMs = 0;
    }
    else {
        if (sysml_state_btn2StablePressed) {
        }
        else {
            sysml_state_btn2StableTimerMs = (sysml_state_btn2StableTimerMs + 5);
            if (((sysml_state_btn2StableTimerMs + 5) >= 150)) {
                sysml_state_btn2StablePressed = true;
            }
        }
    }
    if ((!io_2)) {
        sysml_state_btn3StablePressed = false;
        sysml_state_btn3StableTimerMs = 0;
    }
    else {
        if (sysml_state_btn3StablePressed) {
        }
        else {
            sysml_state_btn3StableTimerMs = (sysml_state_btn3StableTimerMs + 5);
            if (((sysml_state_btn3StableTimerMs + 5) >= 150)) {
                sysml_state_btn3StablePressed = true;
            }
        }
    }
    sysml_lazy_0 = sysml_state_btn1StablePressed;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_btn1PrevPressed;
    }
    if (sysml_lazy_0) {
        if (sysml_state_valueBit0 == false) {
            sysml_conditional_1 = true;
        }
        else {
            sysml_conditional_1 = false;
        }
        sysml_state_valueBit0 = sysml_conditional_1;
    }
    sysml_lazy_2 = sysml_state_btn2StablePressed;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_btn2PrevPressed;
    }
    if (sysml_lazy_2) {
        if (sysml_state_valueBit1 == false) {
            sysml_conditional_3 = true;
        }
        else {
            sysml_conditional_3 = false;
        }
        sysml_state_valueBit1 = sysml_conditional_3;
    }
    sysml_lazy_4 = sysml_state_btn3StablePressed;
    if (sysml_lazy_4) {
        sysml_lazy_4 = !sysml_state_btn3PrevPressed;
    }
    if (sysml_lazy_4) {
        if (sysml_state_valueBit2 == false) {
            sysml_conditional_5 = true;
        }
        else {
            sysml_conditional_5 = false;
        }
        sysml_state_valueBit2 = sysml_conditional_5;
    }
    sysml_state_btn1PrevPressed = sysml_state_btn1StablePressed;
    sysml_state_btn2PrevPressed = sysml_state_btn2StablePressed;
    sysml_state_btn3PrevPressed = sysml_state_btn3StablePressed;
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_valueBit0;
    bool sysml_value_1 = 0;
    sysml_value_1 = sysml_state_valueBit1;
    bool sysml_value_2 = 0;
    sysml_value_2 = sysml_state_valueBit2;
    bool sysml_value_3 = 0;
    sysml_lazy_6 = !sysml_state_valueBit0;
    if (sysml_lazy_6) {
        sysml_lazy_6 = !sysml_state_valueBit1;
    }
    sysml_lazy_7 = sysml_lazy_6;
    if (sysml_lazy_7) {
        sysml_lazy_7 = !sysml_state_valueBit2;
    }
    if (sysml_lazy_7) {
        sysml_conditional_8 = true;
    }
    else {
        sysml_conditional_8 = false;
    }
    sysml_value_3 = sysml_conditional_8;
    bool sysml_value_4 = 0;
    sysml_lazy_9 = sysml_state_valueBit0;
    if (sysml_lazy_9) {
        sysml_lazy_9 = !sysml_state_valueBit1;
    }
    sysml_lazy_10 = sysml_lazy_9;
    if (sysml_lazy_10) {
        sysml_lazy_10 = !sysml_state_valueBit2;
    }
    if (sysml_lazy_10) {
        sysml_conditional_11 = true;
    }
    else {
        sysml_conditional_11 = false;
    }
    sysml_value_4 = sysml_conditional_11;
    bool sysml_value_5 = 0;
    sysml_lazy_12 = !sysml_state_valueBit0;
    if (sysml_lazy_12) {
        sysml_lazy_12 = sysml_state_valueBit1;
    }
    sysml_lazy_13 = sysml_lazy_12;
    if (sysml_lazy_13) {
        sysml_lazy_13 = !sysml_state_valueBit2;
    }
    if (sysml_lazy_13) {
        sysml_conditional_14 = true;
    }
    else {
        sysml_conditional_14 = false;
    }
    sysml_value_5 = sysml_conditional_14;
    bool sysml_value_6 = 0;
    sysml_lazy_15 = sysml_state_valueBit0;
    if (sysml_lazy_15) {
        sysml_lazy_15 = sysml_state_valueBit1;
    }
    sysml_lazy_16 = sysml_lazy_15;
    if (sysml_lazy_16) {
        sysml_lazy_16 = !sysml_state_valueBit2;
    }
    if (sysml_lazy_16) {
        sysml_conditional_17 = true;
    }
    else {
        sysml_conditional_17 = false;
    }
    sysml_value_6 = sysml_conditional_17;
    bool sysml_value_7 = 0;
    sysml_lazy_18 = !sysml_state_valueBit0;
    if (sysml_lazy_18) {
        sysml_lazy_18 = !sysml_state_valueBit1;
    }
    sysml_lazy_19 = sysml_lazy_18;
    if (sysml_lazy_19) {
        sysml_lazy_19 = sysml_state_valueBit2;
    }
    if (sysml_lazy_19) {
        sysml_conditional_20 = true;
    }
    else {
        sysml_conditional_20 = false;
    }
    sysml_value_7 = sysml_conditional_20;
    bool sysml_value_8 = 0;
    sysml_lazy_21 = sysml_state_valueBit0;
    if (sysml_lazy_21) {
        sysml_lazy_21 = !sysml_state_valueBit1;
    }
    sysml_lazy_22 = sysml_lazy_21;
    if (sysml_lazy_22) {
        sysml_lazy_22 = sysml_state_valueBit2;
    }
    if (sysml_lazy_22) {
        sysml_conditional_23 = true;
    }
    else {
        sysml_conditional_23 = false;
    }
    sysml_value_8 = sysml_conditional_23;
    bool sysml_value_9 = 0;
    sysml_lazy_24 = !sysml_state_valueBit0;
    if (sysml_lazy_24) {
        sysml_lazy_24 = sysml_state_valueBit1;
    }
    sysml_lazy_25 = sysml_lazy_24;
    if (sysml_lazy_25) {
        sysml_lazy_25 = sysml_state_valueBit2;
    }
    if (sysml_lazy_25) {
        sysml_conditional_26 = true;
    }
    else {
        sysml_conditional_26 = false;
    }
    sysml_value_9 = sysml_conditional_26;
    bool sysml_value_10 = 0;
    sysml_lazy_27 = sysml_state_valueBit0;
    if (sysml_lazy_27) {
        sysml_lazy_27 = sysml_state_valueBit1;
    }
    sysml_lazy_28 = sysml_lazy_27;
    if (sysml_lazy_28) {
        sysml_lazy_28 = sysml_state_valueBit2;
    }
    if (sysml_lazy_28) {
        sysml_conditional_29 = true;
    }
    else {
        sysml_conditional_29 = false;
    }
    sysml_value_10 = sysml_conditional_29;
    *io_3 = sysml_value_0;
    *io_4 = sysml_value_1;
    *io_5 = sysml_value_2;
    *io_6_segment_0 = sysml_value_3;
    *io_6_segment_1 = sysml_value_4;
    *io_6_segment_2 = sysml_value_5;
    *io_6_segment_3 = sysml_value_6;
    *io_6_segment_4 = sysml_value_7;
    *io_6_segment_5 = sysml_value_8;
    *io_6_segment_6 = sysml_value_9;
    *io_6_segment_7 = sysml_value_10;
}