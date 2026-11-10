#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_dummyInitOff = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_dummyInitOff = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_6, double io_7, double* io_0, double* io_1, double* io_2, double* io_3, double* io_4, double* io_5, bool* io_8_segment_0, bool* io_8_segment_1, bool* io_8_segment_2, bool* io_8_segment_3, bool* io_8_segment_4, bool* io_8_segment_5, bool* io_8_segment_6, bool* io_8_segment_7) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_value_0 = 0;
    double sysml_conditional_0 = 0;
    bool sysml_lazy_1 = 0;
    double sysml_conditional_2 = 0;
    double sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    double sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    double sysml_conditional_7 = 0;
    double sysml_conditional_8 = 0;
    double sysml_conditional_9 = 0;
    double sysml_conditional_10 = 0;
    bool sysml_lazy_11 = 0;
    double sysml_conditional_12 = 0;
    double sysml_conditional_13 = 0;
    bool sysml_lazy_14 = 0;
    double sysml_conditional_15 = 0;
    double sysml_conditional_16 = 0;
    double sysml_conditional_17 = 0;
    bool sysml_conditional_18 = 0;
    bool sysml_conditional_19 = 0;
    bool sysml_conditional_20 = 0;
    bool sysml_conditional_21 = 0;
    bool sysml_conditional_22 = 0;
    bool sysml_conditional_23 = 0;
    bool sysml_conditional_24 = 0;
    bool sysml_conditional_25 = 0;
    if (io_6 <= 340.0) {
        sysml_conditional_0 = 255.0;
    }
    else {
        sysml_lazy_1 = io_6 <= 681.0;
        if (sysml_lazy_1) {
            sysml_lazy_1 = io_6 > 340.0;
        }
        if (sysml_lazy_1) {
            sysml_conditional_2 = 255.0;
        }
        else {
            if (io_6 >= 682.0) {
                sysml_conditional_3 = 0.0;
            }
            else {
                sysml_conditional_3 = 0.0;
            }
            sysml_conditional_2 = sysml_conditional_3;
        }
        sysml_conditional_0 = sysml_conditional_2;
    }
    sysml_value_0 = sysml_conditional_0;
    double sysml_value_1 = 0;
    sysml_lazy_4 = io_6 > 340.0;
    if (sysml_lazy_4) {
        sysml_lazy_4 = io_6 <= 681.0;
    }
    if (sysml_lazy_4) {
        sysml_conditional_5 = 255.0;
    }
    else {
        sysml_lazy_6 = io_6 <= 340.0;
        if (sysml_lazy_6) {
            sysml_lazy_6 = io_6 <= 340.0;
        }
        if (sysml_lazy_6) {
            sysml_conditional_7 = 0.0;
        }
        else {
            if (io_6 >= 682.0) {
                sysml_conditional_8 = 0.0;
            }
            else {
                sysml_conditional_8 = 0.0;
            }
            sysml_conditional_7 = sysml_conditional_8;
        }
        sysml_conditional_5 = sysml_conditional_7;
    }
    sysml_value_1 = sysml_conditional_5;
    double sysml_value_2 = 0;
    if (io_6 >= 682.0) {
        sysml_conditional_9 = 255.0;
    }
    else {
        sysml_conditional_9 = 0.0;
    }
    sysml_value_2 = sysml_conditional_9;
    double sysml_value_3 = 0;
    if (io_7 <= 340.0) {
        sysml_conditional_10 = 255.0;
    }
    else {
        sysml_lazy_11 = io_7 <= 681.0;
        if (sysml_lazy_11) {
            sysml_lazy_11 = io_7 > 340.0;
        }
        if (sysml_lazy_11) {
            sysml_conditional_12 = 255.0;
        }
        else {
            if (io_7 >= 682.0) {
                sysml_conditional_13 = 0.0;
            }
            else {
                sysml_conditional_13 = 0.0;
            }
            sysml_conditional_12 = sysml_conditional_13;
        }
        sysml_conditional_10 = sysml_conditional_12;
    }
    sysml_value_3 = sysml_conditional_10;
    double sysml_value_4 = 0;
    sysml_lazy_14 = io_7 > 340.0;
    if (sysml_lazy_14) {
        sysml_lazy_14 = io_7 <= 681.0;
    }
    if (sysml_lazy_14) {
        sysml_conditional_15 = 255.0;
    }
    else {
        if (io_7 >= 682.0) {
            sysml_conditional_16 = 0.0;
        }
        else {
            sysml_conditional_16 = 0.0;
        }
        sysml_conditional_15 = sysml_conditional_16;
    }
    sysml_value_4 = sysml_conditional_15;
    double sysml_value_5 = 0;
    if (io_7 >= 682.0) {
        sysml_conditional_17 = 255.0;
    }
    else {
        sysml_conditional_17 = 0.0;
    }
    sysml_value_5 = sysml_conditional_17;
    bool sysml_value_6 = 0;
    if ((((double)(io_6 + io_7)) / 2.0) >= 127.875) {
        sysml_conditional_18 = true;
    }
    else {
        sysml_conditional_18 = false;
    }
    sysml_value_6 = sysml_conditional_18;
    bool sysml_value_7 = 0;
    if ((((double)(io_6 + io_7)) / 2.0) >= 255.75) {
        sysml_conditional_19 = true;
    }
    else {
        sysml_conditional_19 = false;
    }
    sysml_value_7 = sysml_conditional_19;
    bool sysml_value_8 = 0;
    if ((((double)(io_6 + io_7)) / 2.0) >= 383.625) {
        sysml_conditional_20 = true;
    }
    else {
        sysml_conditional_20 = false;
    }
    sysml_value_8 = sysml_conditional_20;
    bool sysml_value_9 = 0;
    if ((((double)(io_6 + io_7)) / 2.0) >= 511.5) {
        sysml_conditional_21 = true;
    }
    else {
        sysml_conditional_21 = false;
    }
    sysml_value_9 = sysml_conditional_21;
    bool sysml_value_10 = 0;
    if ((((double)(io_6 + io_7)) / 2.0) >= 639.375) {
        sysml_conditional_22 = true;
    }
    else {
        sysml_conditional_22 = false;
    }
    sysml_value_10 = sysml_conditional_22;
    bool sysml_value_11 = 0;
    if ((((double)(io_6 + io_7)) / 2.0) >= 767.25) {
        sysml_conditional_23 = true;
    }
    else {
        sysml_conditional_23 = false;
    }
    sysml_value_11 = sysml_conditional_23;
    bool sysml_value_12 = 0;
    if ((((double)(io_6 + io_7)) / 2.0) >= 895.125) {
        sysml_conditional_24 = true;
    }
    else {
        sysml_conditional_24 = false;
    }
    sysml_value_12 = sysml_conditional_24;
    bool sysml_value_13 = 0;
    if ((((double)(io_6 + io_7)) / 2.0) >= 1023.0) {
        sysml_conditional_25 = true;
    }
    else {
        sysml_conditional_25 = false;
    }
    sysml_value_13 = sysml_conditional_25;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
    *io_4 = sysml_value_4;
    *io_5 = sysml_value_5;
    *io_8_segment_0 = sysml_value_6;
    *io_8_segment_1 = sysml_value_7;
    *io_8_segment_2 = sysml_value_8;
    *io_8_segment_3 = sysml_value_9;
    *io_8_segment_4 = sysml_value_10;
    *io_8_segment_5 = sysml_value_11;
    *io_8_segment_6 = sysml_value_12;
    *io_8_segment_7 = sysml_value_13;
}