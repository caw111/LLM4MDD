#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_phaseIndex = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_phaseIndex = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double* io_0, double* io_1, double* io_2, double* io_3, double* io_4, double* io_5, double* io_6, double* io_7, double* io_8, bool* io_9_segment_0, bool* io_9_segment_1, bool* io_9_segment_2, bool* io_9_segment_3, bool* io_9_segment_4, bool* io_10, bool* io_11) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_conditional_0 = 0;
    double sysml_conditional_1 = 0;
    double sysml_conditional_2 = 0;
    double sysml_conditional_3 = 0;
    double sysml_conditional_4 = 0;
    double sysml_conditional_5 = 0;
    double sysml_conditional_6 = 0;
    double sysml_conditional_7 = 0;
    double sysml_conditional_8 = 0;
    double sysml_conditional_9 = 0;
    double sysml_conditional_10 = 0;
    double sysml_conditional_11 = 0;
    double sysml_conditional_12 = 0;
    double sysml_conditional_13 = 0;
    double sysml_conditional_14 = 0;
    double sysml_conditional_15 = 0;
    double sysml_conditional_16 = 0;
    double sysml_conditional_17 = 0;
    double sysml_conditional_18 = 0;
    bool sysml_lazy_19 = 0;
    bool sysml_lazy_20 = 0;
    bool sysml_conditional_21 = 0;
    bool sysml_lazy_22 = 0;
    bool sysml_lazy_23 = 0;
    bool sysml_lazy_24 = 0;
    bool sysml_conditional_25 = 0;
    bool sysml_lazy_26 = 0;
    bool sysml_lazy_27 = 0;
    bool sysml_conditional_28 = 0;
    bool sysml_lazy_29 = 0;
    bool sysml_conditional_30 = 0;
    bool sysml_conditional_31 = 0;
    sysml_state_phaseIndex = (sysml_state_phaseIndex + 1);
    double sysml_value_0 = 0;
    if (sysml_state_phaseIndex == 0) {
        sysml_conditional_0 = 1.0;
    }
    else {
        sysml_conditional_0 = 0.0;
    }
    sysml_value_0 = sysml_conditional_0;
    double sysml_value_1 = 0;
    if (sysml_state_phaseIndex == 3) {
        sysml_conditional_1 = 1.0;
    }
    else {
        if (sysml_state_phaseIndex == 4) {
            sysml_conditional_2 = 1.0;
        }
        else {
            sysml_conditional_2 = 0.0;
        }
        sysml_conditional_1 = sysml_conditional_2;
    }
    sysml_value_1 = sysml_conditional_1;
    double sysml_value_2 = 0;
    if (sysml_state_phaseIndex == 2) {
        sysml_conditional_3 = 1.0;
    }
    else {
        if (sysml_state_phaseIndex == 4) {
            sysml_conditional_4 = 1.0;
        }
        else {
            sysml_conditional_4 = 0.0;
        }
        sysml_conditional_3 = sysml_conditional_4;
    }
    sysml_value_2 = sysml_conditional_3;
    double sysml_value_3 = 0;
    if (sysml_state_phaseIndex == 1) {
        sysml_conditional_5 = 1.0;
    }
    else {
        if (sysml_state_phaseIndex == 3) {
            sysml_conditional_6 = 1.0;
        }
        else {
            if (sysml_state_phaseIndex == 4) {
                sysml_conditional_7 = 1.0;
            }
            else {
                sysml_conditional_7 = 0.0;
            }
            sysml_conditional_6 = sysml_conditional_7;
        }
        sysml_conditional_5 = sysml_conditional_6;
    }
    sysml_value_3 = sysml_conditional_5;
    double sysml_value_4 = 0;
    if (sysml_state_phaseIndex == 1) {
        sysml_conditional_8 = 1.0;
    }
    else {
        if (sysml_state_phaseIndex == 3) {
            sysml_conditional_9 = 1.0;
        }
        else {
            if (sysml_state_phaseIndex == 4) {
                sysml_conditional_10 = 1.0;
            }
            else {
                sysml_conditional_10 = 0.0;
            }
            sysml_conditional_9 = sysml_conditional_10;
        }
        sysml_conditional_8 = sysml_conditional_9;
    }
    sysml_value_4 = sysml_conditional_8;
    double sysml_value_5 = 0;
    if (sysml_state_phaseIndex == 2) {
        sysml_conditional_11 = 1.0;
    }
    else {
        if (sysml_state_phaseIndex == 4) {
            sysml_conditional_12 = 1.0;
        }
        else {
            sysml_conditional_12 = 0.0;
        }
        sysml_conditional_11 = sysml_conditional_12;
    }
    sysml_value_5 = sysml_conditional_11;
    double sysml_value_6 = 0;
    if (sysml_state_phaseIndex == 2) {
        sysml_conditional_13 = 1.0;
    }
    else {
        if (sysml_state_phaseIndex == 4) {
            sysml_conditional_14 = 1.0;
        }
        else {
            sysml_conditional_14 = 0.0;
        }
        sysml_conditional_13 = sysml_conditional_14;
    }
    sysml_value_6 = sysml_conditional_13;
    double sysml_value_7 = 0;
    if (sysml_state_phaseIndex == 2) {
        sysml_conditional_15 = 0.0;
    }
    else {
        if (sysml_state_phaseIndex == 4) {
            sysml_conditional_16 = 1.0;
        }
        else {
            sysml_conditional_16 = 0.0;
        }
        sysml_conditional_15 = sysml_conditional_16;
    }
    sysml_value_7 = sysml_conditional_15;
    double sysml_value_8 = 0;
    if (sysml_state_phaseIndex == 2) {
        sysml_conditional_17 = 1.0;
    }
    else {
        if (sysml_state_phaseIndex == 4) {
            sysml_conditional_18 = 1.0;
        }
        else {
            sysml_conditional_18 = 0.0;
        }
        sysml_conditional_17 = sysml_conditional_18;
    }
    sysml_value_8 = sysml_conditional_17;
    bool sysml_value_9 = 0;
    sysml_lazy_19 = sysml_state_phaseIndex == 0;
    if (!sysml_lazy_19) {
        sysml_lazy_19 = sysml_state_phaseIndex == 3;
    }
    sysml_lazy_20 = sysml_lazy_19;
    if (!sysml_lazy_20) {
        sysml_lazy_20 = sysml_state_phaseIndex == 4;
    }
    if (sysml_lazy_20) {
        sysml_conditional_21 = true;
    }
    else {
        sysml_conditional_21 = false;
    }
    sysml_value_9 = sysml_conditional_21;
    bool sysml_value_10 = 0;
    sysml_lazy_22 = sysml_state_phaseIndex == 1;
    if (!sysml_lazy_22) {
        sysml_lazy_22 = sysml_state_phaseIndex == 2;
    }
    sysml_lazy_23 = sysml_lazy_22;
    if (!sysml_lazy_23) {
        sysml_lazy_23 = sysml_state_phaseIndex == 3;
    }
    sysml_lazy_24 = sysml_lazy_23;
    if (!sysml_lazy_24) {
        sysml_lazy_24 = sysml_state_phaseIndex == 4;
    }
    if (sysml_lazy_24) {
        sysml_conditional_25 = true;
    }
    else {
        sysml_conditional_25 = false;
    }
    sysml_value_10 = sysml_conditional_25;
    bool sysml_value_11 = 0;
    sysml_lazy_26 = sysml_state_phaseIndex == 2;
    if (!sysml_lazy_26) {
        sysml_lazy_26 = sysml_state_phaseIndex == 3;
    }
    sysml_lazy_27 = sysml_lazy_26;
    if (!sysml_lazy_27) {
        sysml_lazy_27 = sysml_state_phaseIndex == 4;
    }
    if (sysml_lazy_27) {
        sysml_conditional_28 = true;
    }
    else {
        sysml_conditional_28 = false;
    }
    sysml_value_11 = sysml_conditional_28;
    bool sysml_value_12 = 0;
    sysml_lazy_29 = sysml_state_phaseIndex == 3;
    if (!sysml_lazy_29) {
        sysml_lazy_29 = sysml_state_phaseIndex == 4;
    }
    if (sysml_lazy_29) {
        sysml_conditional_30 = true;
    }
    else {
        sysml_conditional_30 = false;
    }
    sysml_value_12 = sysml_conditional_30;
    bool sysml_value_13 = 0;
    if (sysml_state_phaseIndex == 4) {
        sysml_conditional_31 = true;
    }
    else {
        sysml_conditional_31 = false;
    }
    sysml_value_13 = sysml_conditional_31;
    bool sysml_value_14 = 0;
    sysml_value_14 = false;
    bool sysml_value_15 = 0;
    sysml_value_15 = false;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
    *io_4 = sysml_value_4;
    *io_5 = sysml_value_5;
    *io_6 = sysml_value_6;
    *io_7 = sysml_value_7;
    *io_8 = sysml_value_8;
    *io_9_segment_0 = sysml_value_9;
    *io_9_segment_1 = sysml_value_10;
    *io_9_segment_2 = sysml_value_11;
    *io_9_segment_3 = sysml_value_12;
    *io_9_segment_4 = sysml_value_13;
    *io_10 = sysml_value_14;
    *io_11 = sysml_value_15;
}