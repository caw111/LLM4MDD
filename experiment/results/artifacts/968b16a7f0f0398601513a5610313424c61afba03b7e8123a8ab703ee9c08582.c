#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_servo1Deg = 0.0;
/* Global Variable define of  */
double sysml_state_servo2Deg = 0.0;
/* Global Variable define of  */
int sysml_state_accumMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_servo1Deg = 0.0;
    sysml_state_servo2Deg = 0.0;
    sysml_state_accumMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_9, double* io_0, double* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7, bool* io_8_segment_0, bool* io_8_segment_1, bool* io_8_segment_2, bool* io_8_segment_3, bool* io_8_segment_4, bool* io_8_segment_5) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_conditional_0 = 0;
    double sysml_conditional_1 = 0;
    double sysml_conditional_2 = 0;
    double sysml_conditional_3 = 0;
    bool sysml_conditional_4 = 0;
    bool sysml_conditional_5 = 0;
    bool sysml_conditional_6 = 0;
    bool sysml_conditional_7 = 0;
    bool sysml_conditional_8 = 0;
    bool sysml_conditional_9 = 0;
    bool sysml_conditional_10 = 0;
    bool sysml_conditional_11 = 0;
    bool sysml_conditional_12 = 0;
    bool sysml_conditional_13 = 0;
    bool sysml_conditional_14 = 0;
    bool sysml_conditional_15 = 0;
    if (((sysml_state_accumMs + io_9) >= 2000)) {
        sysml_state_accumMs = ((sysml_state_accumMs + io_9) - 2000);
        if (sysml_state_servo1Deg >= 180.0) {
            sysml_conditional_0 = 0.0;
        }
        else {
            if ((sysml_state_servo1Deg + 30.0) > 180.0) {
                sysml_conditional_1 = 0.0;
            }
            else {
                sysml_conditional_1 = sysml_state_servo1Deg + 30.0;
            }
            sysml_conditional_0 = sysml_conditional_1;
        }
        sysml_state_servo1Deg = sysml_conditional_0;
        if (sysml_state_servo2Deg >= 180.0) {
            sysml_conditional_2 = 0.0;
        }
        else {
            if ((sysml_state_servo2Deg + 30.0) > 180.0) {
                sysml_conditional_3 = 0.0;
            }
            else {
                sysml_conditional_3 = sysml_state_servo2Deg + 30.0;
            }
            sysml_conditional_2 = sysml_conditional_3;
        }
        sysml_state_servo2Deg = sysml_conditional_2;
    }
    else {
        sysml_state_accumMs = (sysml_state_accumMs + io_9);
    }
    double sysml_value_0 = 0;
    sysml_value_0 = sysml_state_servo1Deg;
    double sysml_value_1 = 0;
    sysml_value_1 = sysml_state_servo2Deg;
    bool sysml_value_2 = 0;
    sysml_value_2 = false;
    bool sysml_value_3 = 0;
    sysml_value_3 = false;
    bool sysml_value_4 = 0;
    sysml_value_4 = true;
    bool sysml_value_5 = 0;
    sysml_value_5 = true;
    bool sysml_value_6 = 0;
    sysml_value_6 = false;
    bool sysml_value_7 = 0;
    sysml_value_7 = false;
    bool sysml_value_8 = 0;
    if ((((double)(sysml_state_servo1Deg + sysml_state_servo2Deg)) / 2.0) <= 90.0) {
        if ((((double)(sysml_state_servo1Deg + sysml_state_servo2Deg)) / 2.0) >= 0.0) {
            sysml_conditional_5 = true;
        }
        else {
            sysml_conditional_5 = false;
        }
        sysml_conditional_4 = sysml_conditional_5;
    }
    else {
        sysml_conditional_4 = false;
    }
    sysml_value_8 = sysml_conditional_4;
    bool sysml_value_9 = 0;
    if ((((double)(sysml_state_servo1Deg + sysml_state_servo2Deg)) / 2.0) > 90.0) {
        if ((((double)(sysml_state_servo1Deg + sysml_state_servo2Deg)) / 2.0) <= 120.0) {
            sysml_conditional_7 = true;
        }
        else {
            sysml_conditional_7 = false;
        }
        sysml_conditional_6 = sysml_conditional_7;
    }
    else {
        sysml_conditional_6 = false;
    }
    sysml_value_9 = sysml_conditional_6;
    bool sysml_value_10 = 0;
    if ((((double)(sysml_state_servo1Deg + sysml_state_servo2Deg)) / 2.0) > 120.0) {
        if ((((double)(sysml_state_servo1Deg + sysml_state_servo2Deg)) / 2.0) <= 135.0) {
            sysml_conditional_9 = true;
        }
        else {
            sysml_conditional_9 = false;
        }
        sysml_conditional_8 = sysml_conditional_9;
    }
    else {
        sysml_conditional_8 = false;
    }
    sysml_value_10 = sysml_conditional_8;
    bool sysml_value_11 = 0;
    if ((((double)(sysml_state_servo1Deg + sysml_state_servo2Deg)) / 2.0) > 135.0) {
        if ((((double)(sysml_state_servo1Deg + sysml_state_servo2Deg)) / 2.0) <= 150.0) {
            sysml_conditional_11 = true;
        }
        else {
            sysml_conditional_11 = false;
        }
        sysml_conditional_10 = sysml_conditional_11;
    }
    else {
        sysml_conditional_10 = false;
    }
    sysml_value_11 = sysml_conditional_10;
    bool sysml_value_12 = 0;
    if ((((double)(sysml_state_servo1Deg + sysml_state_servo2Deg)) / 2.0) > 150.0) {
        if ((((double)(sysml_state_servo1Deg + sysml_state_servo2Deg)) / 2.0) <= 165.0) {
            sysml_conditional_13 = true;
        }
        else {
            sysml_conditional_13 = false;
        }
        sysml_conditional_12 = sysml_conditional_13;
    }
    else {
        sysml_conditional_12 = false;
    }
    sysml_value_12 = sysml_conditional_12;
    bool sysml_value_13 = 0;
    if ((((double)(sysml_state_servo1Deg + sysml_state_servo2Deg)) / 2.0) > 165.0) {
        if ((((double)(sysml_state_servo1Deg + sysml_state_servo2Deg)) / 2.0) <= 180.0) {
            sysml_conditional_15 = true;
        }
        else {
            sysml_conditional_15 = false;
        }
        sysml_conditional_14 = sysml_conditional_15;
    }
    else {
        sysml_conditional_14 = false;
    }
    sysml_value_13 = sysml_conditional_14;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
    *io_4 = sysml_value_4;
    *io_5 = sysml_value_5;
    *io_6 = sysml_value_6;
    *io_7 = sysml_value_7;
    *io_8_segment_0 = sysml_value_8;
    *io_8_segment_1 = sysml_value_9;
    *io_8_segment_2 = sysml_value_10;
    *io_8_segment_3 = sysml_value_11;
    *io_8_segment_4 = sysml_value_12;
    *io_8_segment_5 = sysml_value_13;
}