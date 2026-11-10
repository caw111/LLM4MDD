#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_servo3Phase = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_servo3Phase = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, double* io_2, double* io_3, bool* io_4, double* io_5, double* io_6, bool* io_7, double* io_8, double* io_9, bool* io_10, double* io_11, double* io_12, double* io_13) {
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
    double sysml_conditional_19 = 0;
    double sysml_conditional_20 = 0;
    double sysml_conditional_21 = 0;
    double sysml_conditional_22 = 0;
    sysml_state_servo3Phase = sysml_state_servo3Phase;
    double sysml_value_0 = 0;
    if (true) {
        if (io_0 <= 255.0) {
            sysml_conditional_1 = io_0;
        }
        else {
            if (io_0 <= 1023.0) {
                sysml_conditional_2 = ((double)(io_0 * 255.0)) / 1023.0;
            }
            else {
                sysml_conditional_2 = 255.0;
            }
            sysml_conditional_1 = sysml_conditional_2;
        }
        sysml_conditional_0 = sysml_conditional_1;
    }
    else {
        sysml_conditional_0 = 0.0;
    }
    sysml_value_0 = sysml_conditional_0;
    double sysml_value_1 = 0;
    if (true) {
        if (io_0 <= 255.0) {
            sysml_conditional_4 = io_0;
        }
        else {
            if (io_0 <= 1023.0) {
                sysml_conditional_5 = ((double)(io_0 * 255.0)) / 1023.0;
            }
            else {
                sysml_conditional_5 = 255.0;
            }
            sysml_conditional_4 = sysml_conditional_5;
        }
        sysml_conditional_3 = sysml_conditional_4;
    }
    else {
        sysml_conditional_3 = 0.0;
    }
    sysml_value_1 = sysml_conditional_3;
    bool sysml_value_2 = 0;
    sysml_value_2 = false;
    double sysml_value_3 = 0;
    if (true) {
        if (io_1 <= 255.0) {
            sysml_conditional_7 = io_1;
        }
        else {
            if (io_1 <= 1023.0) {
                sysml_conditional_8 = ((double)(io_1 * 255.0)) / 1023.0;
            }
            else {
                sysml_conditional_8 = 255.0;
            }
            sysml_conditional_7 = sysml_conditional_8;
        }
        sysml_conditional_6 = sysml_conditional_7;
    }
    else {
        sysml_conditional_6 = 0.0;
    }
    sysml_value_3 = sysml_conditional_6;
    double sysml_value_4 = 0;
    if (true) {
        if (io_1 <= 255.0) {
            sysml_conditional_10 = io_1;
        }
        else {
            if (io_1 <= 1023.0) {
                sysml_conditional_11 = ((double)(io_1 * 255.0)) / 1023.0;
            }
            else {
                sysml_conditional_11 = 255.0;
            }
            sysml_conditional_10 = sysml_conditional_11;
        }
        sysml_conditional_9 = sysml_conditional_10;
    }
    else {
        sysml_conditional_9 = 0.0;
    }
    sysml_value_4 = sysml_conditional_9;
    bool sysml_value_5 = 0;
    sysml_value_5 = false;
    double sysml_value_6 = 0;
    if (true) {
        if (io_0 <= 255.0) {
            sysml_conditional_13 = io_0;
        }
        else {
            if (io_0 <= 1023.0) {
                sysml_conditional_14 = ((double)(io_0 * 255.0)) / 1023.0;
            }
            else {
                sysml_conditional_14 = 255.0;
            }
            sysml_conditional_13 = sysml_conditional_14;
        }
        sysml_conditional_12 = sysml_conditional_13;
    }
    else {
        sysml_conditional_12 = 0.0;
    }
    sysml_value_6 = sysml_conditional_12;
    double sysml_value_7 = 0;
    if (true) {
        if (io_1 <= 255.0) {
            sysml_conditional_16 = io_1;
        }
        else {
            if (io_1 <= 1023.0) {
                sysml_conditional_17 = ((double)(io_1 * 255.0)) / 1023.0;
            }
            else {
                sysml_conditional_17 = 255.0;
            }
            sysml_conditional_16 = sysml_conditional_17;
        }
        sysml_conditional_15 = sysml_conditional_16;
    }
    else {
        sysml_conditional_15 = 0.0;
    }
    sysml_value_7 = sysml_conditional_15;
    bool sysml_value_8 = 0;
    sysml_value_8 = false;
    double sysml_value_9 = 0;
    if (io_0 <= 180.0) {
        sysml_conditional_18 = io_0;
    }
    else {
        if (io_0 <= 1023.0) {
            sysml_conditional_19 = ((double)(io_0 * 180.0)) / 1023.0;
        }
        else {
            sysml_conditional_19 = 180.0;
        }
        sysml_conditional_18 = sysml_conditional_19;
    }
    sysml_value_9 = sysml_conditional_18;
    double sysml_value_10 = 0;
    if (io_1 <= 180.0) {
        sysml_conditional_20 = io_1;
    }
    else {
        if (io_1 <= 1023.0) {
            sysml_conditional_21 = ((double)(io_1 * 180.0)) / 1023.0;
        }
        else {
            sysml_conditional_21 = 180.0;
        }
        sysml_conditional_20 = sysml_conditional_21;
    }
    sysml_value_10 = sysml_conditional_20;
    double sysml_value_11 = 0;
    if (sysml_state_servo3Phase) {
        sysml_conditional_22 = 180.0;
    }
    else {
        sysml_conditional_22 = 0.0;
    }
    sysml_value_11 = sysml_conditional_22;
    *io_2 = sysml_value_0;
    *io_3 = sysml_value_1;
    *io_4 = sysml_value_2;
    *io_5 = sysml_value_3;
    *io_6 = sysml_value_4;
    *io_7 = sysml_value_5;
    *io_8 = sysml_value_6;
    *io_9 = sysml_value_7;
    *io_10 = sysml_value_8;
    *io_11 = sysml_value_9;
    *io_12 = sysml_value_10;
    *io_13 = sysml_value_11;
}