#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_servo1Angle = 0.0;
/* Global Variable define of  */
double sysml_state_servo2Angle = 0.0;
/* Global Variable define of  */
double sysml_state_servo3Angle = 0.0;
/* Global Variable define of  */
bool sysml_state_servo1Increasing = true;
/* Global Variable define of  */
bool sysml_state_servo2Increasing = true;
/* Global Variable define of  */
bool sysml_state_servo3Increasing = true;
/* Global Variable define of  */
int sysml_state_accumElapsedMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_servo1Angle = 0.0;
    sysml_state_servo2Angle = 0.0;
    sysml_state_servo3Angle = 0.0;
    sysml_state_servo1Increasing = true;
    sysml_state_servo2Increasing = true;
    sysml_state_servo3Increasing = true;
    sysml_state_accumElapsedMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double* io_0, double* io_1, double* io_2, bool* io_3_segment_0, bool* io_3_segment_1, bool* io_3_segment_2, bool* io_3_segment_3, bool* io_3_segment_4, bool* io_3_segment_5, bool* io_3_segment_6, bool* io_3_segment_7, bool* io_3_segment_8, bool* io_3_segment_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    bool sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    bool sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    bool sysml_conditional_5 = 0;
    int sysml_conditional_6 = 0;
    bool sysml_conditional_7 = 0;
    int sysml_conditional_8 = 0;
    bool sysml_conditional_9 = 0;
    int sysml_conditional_10 = 0;
    bool sysml_conditional_11 = 0;
    int sysml_conditional_12 = 0;
    bool sysml_conditional_13 = 0;
    int sysml_conditional_14 = 0;
    bool sysml_conditional_15 = 0;
    int sysml_conditional_16 = 0;
    bool sysml_conditional_17 = 0;
    int sysml_conditional_18 = 0;
    bool sysml_conditional_19 = 0;
    if ((sysml_state_accumElapsedMs >= 2000)) {
        sysml_state_accumElapsedMs = (sysml_state_accumElapsedMs - 2000);
        if (sysml_state_servo1Increasing) {
            if (((sysml_state_servo1Angle + 18.0) >= 180.0)) {
                sysml_state_servo1Angle = 180.0;
                sysml_state_servo1Increasing = false;
            }
            else {
                sysml_state_servo1Angle = (sysml_state_servo1Angle + 18.0);
            }
        }
        else {
            if (((sysml_state_servo1Angle - 18.0) <= 0.0)) {
                sysml_state_servo1Angle = 0.0;
                sysml_state_servo1Increasing = true;
            }
            else {
                sysml_state_servo1Angle = (sysml_state_servo1Angle - 18.0);
            }
        }
        if (sysml_state_servo2Increasing) {
            if (((sysml_state_servo2Angle + 18.0) >= 180.0)) {
                sysml_state_servo2Angle = 180.0;
                sysml_state_servo2Increasing = false;
            }
            else {
                sysml_state_servo2Angle = (sysml_state_servo2Angle + 18.0);
            }
        }
        else {
            if (((sysml_state_servo2Angle - 18.0) <= 0.0)) {
                sysml_state_servo2Angle = 0.0;
                sysml_state_servo2Increasing = true;
            }
            else {
                sysml_state_servo2Angle = (sysml_state_servo2Angle - 18.0);
            }
        }
        if (sysml_state_servo3Increasing) {
            if (((sysml_state_servo3Angle + 18.0) >= 180.0)) {
                sysml_state_servo3Angle = 180.0;
                sysml_state_servo3Increasing = false;
            }
            else {
                sysml_state_servo3Angle = (sysml_state_servo3Angle + 18.0);
            }
        }
        else {
            if (((sysml_state_servo3Angle - 18.0) <= 0.0)) {
                sysml_state_servo3Angle = 0.0;
                sysml_state_servo3Increasing = true;
            }
            else {
                sysml_state_servo3Angle = (sysml_state_servo3Angle - 18.0);
            }
        }
    }
    else {
        sysml_state_accumElapsedMs = (sysml_state_accumElapsedMs + 0);
    }
    double sysml_value_0 = 0;
    sysml_value_0 = sysml_state_servo1Angle;
    double sysml_value_1 = 0;
    sysml_value_1 = sysml_state_servo2Angle;
    double sysml_value_2 = 0;
    sysml_value_2 = sysml_state_servo3Angle;
    bool sysml_value_3 = 0;
    if ((((double)((sysml_state_servo1Angle + sysml_state_servo2Angle) + sysml_state_servo3Angle)) / 54.0) < ((double)((int)(((double)((sysml_state_servo1Angle + sysml_state_servo2Angle) + sysml_state_servo3Angle)) / 54.0)))) {
        sysml_conditional_0 = 1;
    }
    else {
        sysml_conditional_0 = 0;
    }
    if ((((int)(((double)((sysml_state_servo1Angle + sysml_state_servo2Angle) + sysml_state_servo3Angle)) / 54.0)) - sysml_conditional_0) >= 1) {
        sysml_conditional_1 = true;
    }
    else {
        sysml_conditional_1 = false;
    }
    sysml_value_3 = sysml_conditional_1;
    bool sysml_value_4 = 0;
    if ((((double)((sysml_state_servo1Angle + sysml_state_servo2Angle) + sysml_state_servo3Angle)) / 54.0) < ((double)((int)(((double)((sysml_state_servo1Angle + sysml_state_servo2Angle) + sysml_state_servo3Angle)) / 54.0)))) {
        sysml_conditional_2 = 1;
    }
    else {
        sysml_conditional_2 = 0;
    }
    if ((((int)(((double)((sysml_state_servo1Angle + sysml_state_servo2Angle) + sysml_state_servo3Angle)) / 54.0)) - sysml_conditional_2) >= 2) {
        sysml_conditional_3 = true;
    }
    else {
        sysml_conditional_3 = false;
    }
    sysml_value_4 = sysml_conditional_3;
    bool sysml_value_5 = 0;
    if ((((double)((sysml_state_servo1Angle + sysml_state_servo2Angle) + sysml_state_servo3Angle)) / 54.0) < ((double)((int)(((double)((sysml_state_servo1Angle + sysml_state_servo2Angle) + sysml_state_servo3Angle)) / 54.0)))) {
        sysml_conditional_4 = 1;
    }
    else {
        sysml_conditional_4 = 0;
    }
    if ((((int)(((double)((sysml_state_servo1Angle + sysml_state_servo2Angle) + sysml_state_servo3Angle)) / 54.0)) - sysml_conditional_4) >= 3) {
        sysml_conditional_5 = true;
    }
    else {
        sysml_conditional_5 = false;
    }
    sysml_value_5 = sysml_conditional_5;
    bool sysml_value_6 = 0;
    if ((((double)((sysml_state_servo1Angle + sysml_state_servo2Angle) + sysml_state_servo3Angle)) / 54.0) < ((double)((int)(((double)((sysml_state_servo1Angle + sysml_state_servo2Angle) + sysml_state_servo3Angle)) / 54.0)))) {
        sysml_conditional_6 = 1;
    }
    else {
        sysml_conditional_6 = 0;
    }
    if ((((int)(((double)((sysml_state_servo1Angle + sysml_state_servo2Angle) + sysml_state_servo3Angle)) / 54.0)) - sysml_conditional_6) >= 4) {
        sysml_conditional_7 = true;
    }
    else {
        sysml_conditional_7 = false;
    }
    sysml_value_6 = sysml_conditional_7;
    bool sysml_value_7 = 0;
    if ((((double)((sysml_state_servo1Angle + sysml_state_servo2Angle) + sysml_state_servo3Angle)) / 54.0) < ((double)((int)(((double)((sysml_state_servo1Angle + sysml_state_servo2Angle) + sysml_state_servo3Angle)) / 54.0)))) {
        sysml_conditional_8 = 1;
    }
    else {
        sysml_conditional_8 = 0;
    }
    if ((((int)(((double)((sysml_state_servo1Angle + sysml_state_servo2Angle) + sysml_state_servo3Angle)) / 54.0)) - sysml_conditional_8) >= 5) {
        sysml_conditional_9 = true;
    }
    else {
        sysml_conditional_9 = false;
    }
    sysml_value_7 = sysml_conditional_9;
    bool sysml_value_8 = 0;
    if ((((double)((sysml_state_servo1Angle + sysml_state_servo2Angle) + sysml_state_servo3Angle)) / 54.0) < ((double)((int)(((double)((sysml_state_servo1Angle + sysml_state_servo2Angle) + sysml_state_servo3Angle)) / 54.0)))) {
        sysml_conditional_10 = 1;
    }
    else {
        sysml_conditional_10 = 0;
    }
    if ((((int)(((double)((sysml_state_servo1Angle + sysml_state_servo2Angle) + sysml_state_servo3Angle)) / 54.0)) - sysml_conditional_10) >= 6) {
        sysml_conditional_11 = true;
    }
    else {
        sysml_conditional_11 = false;
    }
    sysml_value_8 = sysml_conditional_11;
    bool sysml_value_9 = 0;
    if ((((double)((sysml_state_servo1Angle + sysml_state_servo2Angle) + sysml_state_servo3Angle)) / 54.0) < ((double)((int)(((double)((sysml_state_servo1Angle + sysml_state_servo2Angle) + sysml_state_servo3Angle)) / 54.0)))) {
        sysml_conditional_12 = 1;
    }
    else {
        sysml_conditional_12 = 0;
    }
    if ((((int)(((double)((sysml_state_servo1Angle + sysml_state_servo2Angle) + sysml_state_servo3Angle)) / 54.0)) - sysml_conditional_12) >= 7) {
        sysml_conditional_13 = true;
    }
    else {
        sysml_conditional_13 = false;
    }
    sysml_value_9 = sysml_conditional_13;
    bool sysml_value_10 = 0;
    if ((((double)((sysml_state_servo1Angle + sysml_state_servo2Angle) + sysml_state_servo3Angle)) / 54.0) < ((double)((int)(((double)((sysml_state_servo1Angle + sysml_state_servo2Angle) + sysml_state_servo3Angle)) / 54.0)))) {
        sysml_conditional_14 = 1;
    }
    else {
        sysml_conditional_14 = 0;
    }
    if ((((int)(((double)((sysml_state_servo1Angle + sysml_state_servo2Angle) + sysml_state_servo3Angle)) / 54.0)) - sysml_conditional_14) >= 8) {
        sysml_conditional_15 = true;
    }
    else {
        sysml_conditional_15 = false;
    }
    sysml_value_10 = sysml_conditional_15;
    bool sysml_value_11 = 0;
    if ((((double)((sysml_state_servo1Angle + sysml_state_servo2Angle) + sysml_state_servo3Angle)) / 54.0) < ((double)((int)(((double)((sysml_state_servo1Angle + sysml_state_servo2Angle) + sysml_state_servo3Angle)) / 54.0)))) {
        sysml_conditional_16 = 1;
    }
    else {
        sysml_conditional_16 = 0;
    }
    if ((((int)(((double)((sysml_state_servo1Angle + sysml_state_servo2Angle) + sysml_state_servo3Angle)) / 54.0)) - sysml_conditional_16) >= 9) {
        sysml_conditional_17 = true;
    }
    else {
        sysml_conditional_17 = false;
    }
    sysml_value_11 = sysml_conditional_17;
    bool sysml_value_12 = 0;
    if ((((double)((sysml_state_servo1Angle + sysml_state_servo2Angle) + sysml_state_servo3Angle)) / 54.0) < ((double)((int)(((double)((sysml_state_servo1Angle + sysml_state_servo2Angle) + sysml_state_servo3Angle)) / 54.0)))) {
        sysml_conditional_18 = 1;
    }
    else {
        sysml_conditional_18 = 0;
    }
    if ((((int)(((double)((sysml_state_servo1Angle + sysml_state_servo2Angle) + sysml_state_servo3Angle)) / 54.0)) - sysml_conditional_18) >= 10) {
        sysml_conditional_19 = true;
    }
    else {
        sysml_conditional_19 = false;
    }
    sysml_value_12 = sysml_conditional_19;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3_segment_0 = sysml_value_3;
    *io_3_segment_1 = sysml_value_4;
    *io_3_segment_2 = sysml_value_5;
    *io_3_segment_3 = sysml_value_6;
    *io_3_segment_4 = sysml_value_7;
    *io_3_segment_5 = sysml_value_8;
    *io_3_segment_6 = sysml_value_9;
    *io_3_segment_7 = sysml_value_10;
    *io_3_segment_8 = sysml_value_11;
    *io_3_segment_9 = sysml_value_12;
}