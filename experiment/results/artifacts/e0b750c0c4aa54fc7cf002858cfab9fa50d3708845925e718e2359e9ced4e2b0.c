#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_elapsedSeconds_i32 = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_elapsedSeconds_i32 = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_0, bool* io_1_segment_0, bool* io_1_segment_1, bool* io_1_segment_2, bool* io_1_segment_3, bool* io_1_segment_4, bool* io_1_segment_5, bool* io_1_segment_6, bool* io_1_segment_7) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_conditional_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_conditional_10 = 0;
    bool sysml_lazy_11 = 0;
    bool sysml_lazy_12 = 0;
    bool sysml_lazy_13 = 0;
    bool sysml_conditional_14 = 0;
    bool sysml_lazy_15 = 0;
    bool sysml_lazy_16 = 0;
    bool sysml_lazy_17 = 0;
    bool sysml_conditional_18 = 0;
    bool sysml_lazy_19 = 0;
    bool sysml_lazy_20 = 0;
    bool sysml_lazy_21 = 0;
    bool sysml_lazy_22 = 0;
    bool sysml_lazy_23 = 0;
    bool sysml_conditional_24 = 0;
    bool sysml_lazy_25 = 0;
    bool sysml_lazy_26 = 0;
    bool sysml_lazy_27 = 0;
    bool sysml_lazy_28 = 0;
    bool sysml_lazy_29 = 0;
    bool sysml_conditional_30 = 0;
    bool sysml_lazy_31 = 0;
    bool sysml_lazy_32 = 0;
    bool sysml_lazy_33 = 0;
    bool sysml_conditional_34 = 0;
    bool sysml_lazy_35 = 0;
    bool sysml_conditional_36 = 0;
    if ((((double)io_0) / 1000.0) < ((double)((int)(((double)io_0) / 1000.0)))) {
        sysml_conditional_0 = 1;
    }
    else {
        sysml_conditional_0 = 0;
    }
    sysml_state_elapsedSeconds_i32 = sysml_state_elapsedSeconds_i32 + (((int)(((double)io_0) / 1000.0)) - sysml_conditional_0);
    if ((sysml_state_elapsedSeconds_i32 < 0)) {
        sysml_state_elapsedSeconds_i32 = 0;
    }
    bool sysml_value_0 = 0;
    sysml_lazy_1 = sysml_state_elapsedSeconds_i32 >= 1;
    if (sysml_lazy_1) {
        sysml_lazy_1 = sysml_state_elapsedSeconds_i32 < 2;
    }
    sysml_lazy_2 = sysml_lazy_1;
    if (!sysml_lazy_2) {
        sysml_lazy_3 = sysml_state_elapsedSeconds_i32 >= 3;
        if (sysml_lazy_3) {
            sysml_lazy_3 = sysml_state_elapsedSeconds_i32 < 4;
        }
        sysml_lazy_2 = sysml_lazy_3;
    }
    if (sysml_lazy_2) {
        sysml_conditional_4 = false;
    }
    else {
        sysml_conditional_4 = true;
    }
    sysml_value_0 = sysml_conditional_4;
    bool sysml_value_1 = 0;
    sysml_lazy_5 = sysml_state_elapsedSeconds_i32 >= 1;
    if (sysml_lazy_5) {
        sysml_lazy_5 = sysml_state_elapsedSeconds_i32 < 2;
    }
    sysml_lazy_6 = sysml_lazy_5;
    if (!sysml_lazy_6) {
        sysml_lazy_7 = sysml_state_elapsedSeconds_i32 >= 2;
        if (sysml_lazy_7) {
            sysml_lazy_7 = sysml_state_elapsedSeconds_i32 < 3;
        }
        sysml_lazy_6 = sysml_lazy_7;
    }
    sysml_lazy_8 = sysml_lazy_6;
    if (!sysml_lazy_8) {
        sysml_lazy_9 = sysml_state_elapsedSeconds_i32 >= 3;
        if (sysml_lazy_9) {
            sysml_lazy_9 = sysml_state_elapsedSeconds_i32 < 4;
        }
        sysml_lazy_8 = sysml_lazy_9;
    }
    if (sysml_lazy_8) {
        sysml_conditional_10 = true;
    }
    else {
        sysml_conditional_10 = false;
    }
    sysml_value_1 = sysml_conditional_10;
    bool sysml_value_2 = 0;
    sysml_lazy_11 = sysml_state_elapsedSeconds_i32 >= 1;
    if (sysml_lazy_11) {
        sysml_lazy_11 = sysml_state_elapsedSeconds_i32 < 2;
    }
    sysml_lazy_12 = sysml_lazy_11;
    if (!sysml_lazy_12) {
        sysml_lazy_13 = sysml_state_elapsedSeconds_i32 >= 2;
        if (sysml_lazy_13) {
            sysml_lazy_13 = sysml_state_elapsedSeconds_i32 < 3;
        }
        sysml_lazy_12 = sysml_lazy_13;
    }
    if (sysml_lazy_12) {
        sysml_conditional_14 = true;
    }
    else {
        sysml_conditional_14 = false;
    }
    sysml_value_2 = sysml_conditional_14;
    bool sysml_value_3 = 0;
    sysml_lazy_15 = sysml_state_elapsedSeconds_i32 >= 2;
    if (sysml_lazy_15) {
        sysml_lazy_15 = sysml_state_elapsedSeconds_i32 < 3;
    }
    sysml_lazy_16 = sysml_lazy_15;
    if (!sysml_lazy_16) {
        sysml_lazy_17 = sysml_state_elapsedSeconds_i32 >= 3;
        if (sysml_lazy_17) {
            sysml_lazy_17 = sysml_state_elapsedSeconds_i32 < 4;
        }
        sysml_lazy_16 = sysml_lazy_17;
    }
    if (sysml_lazy_16) {
        sysml_conditional_18 = true;
    }
    else {
        sysml_conditional_18 = false;
    }
    sysml_value_3 = sysml_conditional_18;
    bool sysml_value_4 = 0;
    sysml_lazy_19 = sysml_state_elapsedSeconds_i32 >= 1;
    if (sysml_lazy_19) {
        sysml_lazy_19 = sysml_state_elapsedSeconds_i32 < 2;
    }
    sysml_lazy_20 = sysml_lazy_19;
    if (!sysml_lazy_20) {
        sysml_lazy_21 = sysml_state_elapsedSeconds_i32 >= 2;
        if (sysml_lazy_21) {
            sysml_lazy_21 = sysml_state_elapsedSeconds_i32 < 3;
        }
        sysml_lazy_20 = sysml_lazy_21;
    }
    sysml_lazy_22 = sysml_lazy_20;
    if (!sysml_lazy_22) {
        sysml_lazy_23 = sysml_state_elapsedSeconds_i32 >= 3;
        if (sysml_lazy_23) {
            sysml_lazy_23 = sysml_state_elapsedSeconds_i32 < 4;
        }
        sysml_lazy_22 = sysml_lazy_23;
    }
    if (sysml_lazy_22) {
        sysml_conditional_24 = true;
    }
    else {
        sysml_conditional_24 = false;
    }
    sysml_value_4 = sysml_conditional_24;
    bool sysml_value_5 = 0;
    sysml_lazy_25 = sysml_state_elapsedSeconds_i32 >= 1;
    if (sysml_lazy_25) {
        sysml_lazy_25 = sysml_state_elapsedSeconds_i32 < 2;
    }
    sysml_lazy_26 = sysml_lazy_25;
    if (!sysml_lazy_26) {
        sysml_lazy_27 = sysml_state_elapsedSeconds_i32 >= 2;
        if (sysml_lazy_27) {
            sysml_lazy_27 = sysml_state_elapsedSeconds_i32 < 3;
        }
        sysml_lazy_26 = sysml_lazy_27;
    }
    sysml_lazy_28 = sysml_lazy_26;
    if (!sysml_lazy_28) {
        sysml_lazy_29 = sysml_state_elapsedSeconds_i32 >= 3;
        if (sysml_lazy_29) {
            sysml_lazy_29 = sysml_state_elapsedSeconds_i32 < 4;
        }
        sysml_lazy_28 = sysml_lazy_29;
    }
    if (sysml_lazy_28) {
        sysml_conditional_30 = true;
    }
    else {
        sysml_conditional_30 = false;
    }
    sysml_value_5 = sysml_conditional_30;
    bool sysml_value_6 = 0;
    sysml_lazy_31 = sysml_state_elapsedSeconds_i32 >= 2;
    if (sysml_lazy_31) {
        sysml_lazy_31 = sysml_state_elapsedSeconds_i32 < 3;
    }
    sysml_lazy_32 = sysml_lazy_31;
    if (!sysml_lazy_32) {
        sysml_lazy_33 = sysml_state_elapsedSeconds_i32 >= 3;
        if (sysml_lazy_33) {
            sysml_lazy_33 = sysml_state_elapsedSeconds_i32 < 4;
        }
        sysml_lazy_32 = sysml_lazy_33;
    }
    if (sysml_lazy_32) {
        sysml_conditional_34 = false;
    }
    else {
        sysml_conditional_34 = true;
    }
    sysml_value_6 = sysml_conditional_34;
    bool sysml_value_7 = 0;
    sysml_lazy_35 = sysml_state_elapsedSeconds_i32 >= 3;
    if (sysml_lazy_35) {
        sysml_lazy_35 = sysml_state_elapsedSeconds_i32 < 4;
    }
    if (sysml_lazy_35) {
        sysml_conditional_36 = false;
    }
    else {
        sysml_conditional_36 = true;
    }
    sysml_value_7 = sysml_conditional_36;
    *io_1_segment_0 = sysml_value_0;
    *io_1_segment_1 = sysml_value_1;
    *io_1_segment_2 = sysml_value_2;
    *io_1_segment_3 = sysml_value_3;
    *io_1_segment_4 = sysml_value_4;
    *io_1_segment_5 = sysml_value_5;
    *io_1_segment_6 = sysml_value_6;
    *io_1_segment_7 = sysml_value_7;
}