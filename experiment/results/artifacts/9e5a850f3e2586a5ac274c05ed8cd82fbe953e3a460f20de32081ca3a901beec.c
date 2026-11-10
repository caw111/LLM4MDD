#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_mode = 0;
/* Global Variable define of  */
int sysml_state_phaseElapsedMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_mode = 0;
    sysml_state_phaseElapsedMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double* io_0, double* io_1, double* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6_segment_0, bool* io_6_segment_1, bool* io_6_segment_2, bool* io_6_segment_3, bool* io_6_segment_4, bool* io_6_segment_5, bool* io_6_segment_6, bool* io_6_segment_7) {
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
    if ((sysml_state_mode == 0)) {
        sysml_state_mode = 1;
    }
    else {
        if ((sysml_state_mode == 1)) {
            sysml_state_mode = 2;
        }
        else {
            if ((sysml_state_mode == 2)) {
                sysml_state_mode = 3;
            }
            else {
                if ((sysml_state_mode == 3)) {
                    sysml_state_mode = 4;
                }
                else {
                    sysml_state_mode = 1;
                }
            }
        }
    }
    sysml_state_phaseElapsedMs = 0;
    double sysml_value_0 = 0;
    if (sysml_state_mode == 0) {
        sysml_conditional_0 = 0.0;
    }
    else {
        if (sysml_state_mode == 1) {
            sysml_conditional_1 = 60.0;
        }
        else {
            if (sysml_state_mode == 2) {
                sysml_conditional_2 = 0.0;
            }
            else {
                if (sysml_state_mode == 3) {
                    sysml_conditional_3 = 0.0;
                }
                else {
                    sysml_conditional_3 = 0.0;
                }
                sysml_conditional_2 = sysml_conditional_3;
            }
            sysml_conditional_1 = sysml_conditional_2;
        }
        sysml_conditional_0 = sysml_conditional_1;
    }
    sysml_value_0 = sysml_conditional_0;
    double sysml_value_1 = 0;
    if (sysml_state_mode == 0) {
        sysml_conditional_4 = 0.0;
    }
    else {
        if (sysml_state_mode == 1) {
            sysml_conditional_5 = 0.0;
        }
        else {
            if (sysml_state_mode == 2) {
                sysml_conditional_6 = 120.0;
            }
            else {
                if (sysml_state_mode == 3) {
                    sysml_conditional_7 = 0.0;
                }
                else {
                    sysml_conditional_7 = 0.0;
                }
                sysml_conditional_6 = sysml_conditional_7;
            }
            sysml_conditional_5 = sysml_conditional_6;
        }
        sysml_conditional_4 = sysml_conditional_5;
    }
    sysml_value_1 = sysml_conditional_4;
    double sysml_value_2 = 0;
    if (sysml_state_mode == 0) {
        sysml_conditional_8 = 0.0;
    }
    else {
        if (sysml_state_mode == 1) {
            sysml_conditional_9 = 0.0;
        }
        else {
            if (sysml_state_mode == 2) {
                sysml_conditional_10 = 0.0;
            }
            else {
                if (sysml_state_mode == 3) {
                    sysml_conditional_11 = 180.0;
                }
                else {
                    sysml_conditional_11 = 0.0;
                }
                sysml_conditional_10 = sysml_conditional_11;
            }
            sysml_conditional_9 = sysml_conditional_10;
        }
        sysml_conditional_8 = sysml_conditional_9;
    }
    sysml_value_2 = sysml_conditional_8;
    bool sysml_value_3 = 0;
    if (sysml_state_mode == 1) {
        sysml_conditional_12 = true;
    }
    else {
        sysml_conditional_12 = false;
    }
    sysml_value_3 = sysml_conditional_12;
    bool sysml_value_4 = 0;
    if (sysml_state_mode == 2) {
        sysml_conditional_13 = true;
    }
    else {
        sysml_conditional_13 = false;
    }
    sysml_value_4 = sysml_conditional_13;
    bool sysml_value_5 = 0;
    if (sysml_state_mode == 3) {
        sysml_conditional_14 = true;
    }
    else {
        sysml_conditional_14 = false;
    }
    sysml_value_5 = sysml_conditional_14;
    bool sysml_value_6 = 0;
    if (sysml_state_mode == 1) {
        sysml_conditional_15 = true;
    }
    else {
        sysml_conditional_15 = false;
    }
    sysml_value_6 = sysml_conditional_15;
    bool sysml_value_7 = 0;
    if (sysml_state_mode == 1) {
        sysml_conditional_16 = true;
    }
    else {
        sysml_conditional_16 = false;
    }
    sysml_value_7 = sysml_conditional_16;
    bool sysml_value_8 = 0;
    if (sysml_state_mode == 2) {
        sysml_conditional_17 = true;
    }
    else {
        sysml_conditional_17 = false;
    }
    sysml_value_8 = sysml_conditional_17;
    bool sysml_value_9 = 0;
    if (sysml_state_mode == 2) {
        sysml_conditional_18 = true;
    }
    else {
        sysml_conditional_18 = false;
    }
    sysml_value_9 = sysml_conditional_18;
    bool sysml_value_10 = 0;
    if (sysml_state_mode == 2) {
        sysml_conditional_19 = true;
    }
    else {
        sysml_conditional_19 = false;
    }
    sysml_value_10 = sysml_conditional_19;
    bool sysml_value_11 = 0;
    if (sysml_state_mode == 3) {
        sysml_conditional_20 = true;
    }
    else {
        sysml_conditional_20 = false;
    }
    sysml_value_11 = sysml_conditional_20;
    bool sysml_value_12 = 0;
    if (sysml_state_mode == 3) {
        sysml_conditional_21 = true;
    }
    else {
        sysml_conditional_21 = false;
    }
    sysml_value_12 = sysml_conditional_21;
    bool sysml_value_13 = 0;
    if (sysml_state_mode == 3) {
        sysml_conditional_22 = true;
    }
    else {
        sysml_conditional_22 = false;
    }
    sysml_value_13 = sysml_conditional_22;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
    *io_4 = sysml_value_4;
    *io_5 = sysml_value_5;
    *io_6_segment_0 = sysml_value_6;
    *io_6_segment_1 = sysml_value_7;
    *io_6_segment_2 = sysml_value_8;
    *io_6_segment_3 = sysml_value_9;
    *io_6_segment_4 = sysml_value_10;
    *io_6_segment_5 = sysml_value_11;
    *io_6_segment_6 = sysml_value_12;
    *io_6_segment_7 = sysml_value_13;
}