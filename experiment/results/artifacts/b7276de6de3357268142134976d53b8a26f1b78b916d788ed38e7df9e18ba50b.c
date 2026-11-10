#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_servo1Deg = 0.0;
/* Global Variable define of  */
double sysml_state_servo2Deg = 0.0;
/* Global Variable define of  */
double sysml_state_servo3Deg = 0.0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_servo1Deg = 0.0;
    sysml_state_servo2Deg = 0.0;
    sysml_state_servo3Deg = 0.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, double* io_2, double* io_3, double* io_4, bool* io_5_segment_0, bool* io_5_segment_1, bool* io_5_segment_2, bool* io_5_segment_3, bool* io_5_segment_4, bool* io_5_segment_5, bool* io_5_segment_6, bool* io_5_segment_7, bool* io_5_segment_8, bool* io_5_segment_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_conditional_0 = 0;
    double sysml_conditional_1 = 0;
    double sysml_conditional_2 = 0;
    double sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    double sysml_conditional_7 = 0;
    bool sysml_lazy_8 = 0;
    double sysml_conditional_9 = 0;
    bool sysml_lazy_10 = 0;
    double sysml_conditional_11 = 0;
    int sysml_conditional_12 = 0;
    double sysml_conditional_13 = 0;
    int sysml_conditional_14 = 0;
    int sysml_conditional_15 = 0;
    bool sysml_conditional_16 = 0;
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
    if (true) {
        if (io_0 <= 0.0) {
            sysml_conditional_0 = 0.0;
        }
        else {
            if (io_0 >= 1023.0) {
                sysml_conditional_1 = 180.0;
            }
            else {
                sysml_conditional_1 = ((double)(io_0 * 180.0)) / 1023.0;
            }
            sysml_conditional_0 = sysml_conditional_1;
        }
        sysml_state_servo1Deg = sysml_conditional_0;
        if (io_1 <= 0.0) {
            sysml_conditional_2 = 0.0;
        }
        else {
            if (io_1 >= 1023.0) {
                sysml_conditional_3 = 180.0;
            }
            else {
                sysml_conditional_3 = ((double)(io_1 * 180.0)) / 1023.0;
            }
            sysml_conditional_2 = sysml_conditional_3;
        }
        sysml_state_servo2Deg = sysml_conditional_2;
        sysml_lazy_4 = io_0 >= 0.0;
        if (sysml_lazy_4) {
            sysml_lazy_4 = io_0 <= 1023.0;
        }
        sysml_lazy_5 = sysml_lazy_4;
        if (sysml_lazy_5) {
            sysml_lazy_5 = io_1 >= 0.0;
        }
        sysml_lazy_6 = sysml_lazy_5;
        if (sysml_lazy_6) {
            sysml_lazy_6 = io_1 <= 1023.0;
        }
        if (sysml_lazy_6) {
            sysml_lazy_8 = io_0 >= 1023.0;
            if (sysml_lazy_8) {
                sysml_lazy_8 = io_1 >= 1023.0;
            }
            if (sysml_lazy_8) {
                sysml_conditional_9 = 180.0;
            }
            else {
                sysml_lazy_10 = io_0 <= 0.0;
                if (sysml_lazy_10) {
                    sysml_lazy_10 = io_1 <= 0.0;
                }
                if (sysml_lazy_10) {
                    sysml_conditional_11 = 0.0;
                }
                else {
                    if ((((io_0 + io_1) * 0.5) + 0.5) < ((double)((int)(((io_0 + io_1) * 0.5) + 0.5)))) {
                        sysml_conditional_12 = 1;
                    }
                    else {
                        sysml_conditional_12 = 0;
                    }
                    if ((((double)((((int)(((io_0 + io_1) * 0.5) + 0.5)) - sysml_conditional_12) * 180.0)) / 1023.0) >= 0.0) {
                        if ((((io_0 + io_1) * 0.5) + 0.5) < ((double)((int)(((io_0 + io_1) * 0.5) + 0.5)))) {
                            sysml_conditional_14 = 1;
                        }
                        else {
                            sysml_conditional_14 = 0;
                        }
                        sysml_conditional_13 = ((double)((((int)(((io_0 + io_1) * 0.5) + 0.5)) - sysml_conditional_14) * 180.0)) / 1023.0;
                    }
                    else {
                        sysml_conditional_13 = 0.0;
                    }
                    sysml_conditional_11 = sysml_conditional_13;
                }
                sysml_conditional_9 = sysml_conditional_11;
            }
            sysml_conditional_7 = sysml_conditional_9;
        }
        else {
            sysml_conditional_7 = 0.0;
        }
        sysml_state_servo3Deg = sysml_conditional_7;
    }
    double sysml_value_0 = 0;
    sysml_value_0 = sysml_state_servo1Deg;
    double sysml_value_1 = 0;
    sysml_value_1 = sysml_state_servo2Deg;
    double sysml_value_2 = 0;
    sysml_value_2 = sysml_state_servo3Deg;
    bool sysml_value_3 = 0;
    if ((((io_0 + io_1) * 0.5) + 0.5) < ((double)((int)(((io_0 + io_1) * 0.5) + 0.5)))) {
        sysml_conditional_15 = 1;
    }
    else {
        sysml_conditional_15 = 0;
    }
    if ((((double)((((int)(((io_0 + io_1) * 0.5) + 0.5)) - sysml_conditional_15) * 10.0)) / 1023.0) >= 1) {
        sysml_conditional_16 = true;
    }
    else {
        sysml_conditional_16 = false;
    }
    sysml_value_3 = sysml_conditional_16;
    bool sysml_value_4 = 0;
    if ((((io_0 + io_1) * 0.5) + 0.5) < ((double)((int)(((io_0 + io_1) * 0.5) + 0.5)))) {
        sysml_conditional_17 = 1;
    }
    else {
        sysml_conditional_17 = 0;
    }
    if ((((double)((((int)(((io_0 + io_1) * 0.5) + 0.5)) - sysml_conditional_17) * 10.0)) / 1023.0) >= 2) {
        sysml_conditional_18 = true;
    }
    else {
        sysml_conditional_18 = false;
    }
    sysml_value_4 = sysml_conditional_18;
    bool sysml_value_5 = 0;
    if ((((io_0 + io_1) * 0.5) + 0.5) < ((double)((int)(((io_0 + io_1) * 0.5) + 0.5)))) {
        sysml_conditional_19 = 1;
    }
    else {
        sysml_conditional_19 = 0;
    }
    if ((((double)((((int)(((io_0 + io_1) * 0.5) + 0.5)) - sysml_conditional_19) * 10.0)) / 1023.0) >= 3) {
        sysml_conditional_20 = true;
    }
    else {
        sysml_conditional_20 = false;
    }
    sysml_value_5 = sysml_conditional_20;
    bool sysml_value_6 = 0;
    if ((((io_0 + io_1) * 0.5) + 0.5) < ((double)((int)(((io_0 + io_1) * 0.5) + 0.5)))) {
        sysml_conditional_21 = 1;
    }
    else {
        sysml_conditional_21 = 0;
    }
    if ((((double)((((int)(((io_0 + io_1) * 0.5) + 0.5)) - sysml_conditional_21) * 10.0)) / 1023.0) >= 4) {
        sysml_conditional_22 = true;
    }
    else {
        sysml_conditional_22 = false;
    }
    sysml_value_6 = sysml_conditional_22;
    bool sysml_value_7 = 0;
    if ((((io_0 + io_1) * 0.5) + 0.5) < ((double)((int)(((io_0 + io_1) * 0.5) + 0.5)))) {
        sysml_conditional_23 = 1;
    }
    else {
        sysml_conditional_23 = 0;
    }
    if ((((double)((((int)(((io_0 + io_1) * 0.5) + 0.5)) - sysml_conditional_23) * 10.0)) / 1023.0) >= 5) {
        sysml_conditional_24 = true;
    }
    else {
        sysml_conditional_24 = false;
    }
    sysml_value_7 = sysml_conditional_24;
    bool sysml_value_8 = 0;
    if ((((io_0 + io_1) * 0.5) + 0.5) < ((double)((int)(((io_0 + io_1) * 0.5) + 0.5)))) {
        sysml_conditional_25 = 1;
    }
    else {
        sysml_conditional_25 = 0;
    }
    if ((((double)((((int)(((io_0 + io_1) * 0.5) + 0.5)) - sysml_conditional_25) * 10.0)) / 1023.0) >= 6) {
        sysml_conditional_26 = true;
    }
    else {
        sysml_conditional_26 = false;
    }
    sysml_value_8 = sysml_conditional_26;
    bool sysml_value_9 = 0;
    if ((((io_0 + io_1) * 0.5) + 0.5) < ((double)((int)(((io_0 + io_1) * 0.5) + 0.5)))) {
        sysml_conditional_27 = 1;
    }
    else {
        sysml_conditional_27 = 0;
    }
    if ((((double)((((int)(((io_0 + io_1) * 0.5) + 0.5)) - sysml_conditional_27) * 10.0)) / 1023.0) >= 7) {
        sysml_conditional_28 = true;
    }
    else {
        sysml_conditional_28 = false;
    }
    sysml_value_9 = sysml_conditional_28;
    bool sysml_value_10 = 0;
    if ((((io_0 + io_1) * 0.5) + 0.5) < ((double)((int)(((io_0 + io_1) * 0.5) + 0.5)))) {
        sysml_conditional_29 = 1;
    }
    else {
        sysml_conditional_29 = 0;
    }
    if ((((double)((((int)(((io_0 + io_1) * 0.5) + 0.5)) - sysml_conditional_29) * 10.0)) / 1023.0) >= 8) {
        sysml_conditional_30 = true;
    }
    else {
        sysml_conditional_30 = false;
    }
    sysml_value_10 = sysml_conditional_30;
    bool sysml_value_11 = 0;
    if ((((io_0 + io_1) * 0.5) + 0.5) < ((double)((int)(((io_0 + io_1) * 0.5) + 0.5)))) {
        sysml_conditional_31 = 1;
    }
    else {
        sysml_conditional_31 = 0;
    }
    if ((((double)((((int)(((io_0 + io_1) * 0.5) + 0.5)) - sysml_conditional_31) * 10.0)) / 1023.0) >= 9) {
        sysml_conditional_32 = true;
    }
    else {
        sysml_conditional_32 = false;
    }
    sysml_value_11 = sysml_conditional_32;
    bool sysml_value_12 = 0;
    if ((((io_0 + io_1) * 0.5) + 0.5) < ((double)((int)(((io_0 + io_1) * 0.5) + 0.5)))) {
        sysml_conditional_33 = 1;
    }
    else {
        sysml_conditional_33 = 0;
    }
    if ((((double)((((int)(((io_0 + io_1) * 0.5) + 0.5)) - sysml_conditional_33) * 10.0)) / 1023.0) >= 10) {
        sysml_conditional_34 = true;
    }
    else {
        sysml_conditional_34 = false;
    }
    sysml_value_12 = sysml_conditional_34;
    *io_2 = sysml_value_0;
    *io_3 = sysml_value_1;
    *io_4 = sysml_value_2;
    *io_5_segment_0 = sysml_value_3;
    *io_5_segment_1 = sysml_value_4;
    *io_5_segment_2 = sysml_value_5;
    *io_5_segment_3 = sysml_value_6;
    *io_5_segment_4 = sysml_value_7;
    *io_5_segment_5 = sysml_value_8;
    *io_5_segment_6 = sysml_value_9;
    *io_5_segment_7 = sysml_value_10;
    *io_5_segment_8 = sysml_value_11;
    *io_5_segment_9 = sysml_value_12;
}