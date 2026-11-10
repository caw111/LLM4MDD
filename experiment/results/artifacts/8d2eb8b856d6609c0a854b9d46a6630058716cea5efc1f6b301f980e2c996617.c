#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_servo1Angle = 0.0;
/* Global Variable define of  */
double sysml_state_servo2Angle = 0.0;
/* Global Variable define of  */
int sysml_state_timeAccumMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_servo1Angle = 0.0;
    sysml_state_servo2Angle = 0.0;
    sysml_state_timeAccumMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_9, double* io_0, double* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7, bool* io_8_segment_0, bool* io_8_segment_1, bool* io_8_segment_2, bool* io_8_segment_3, bool* io_8_segment_4, bool* io_8_segment_5) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_conditional_3 = 0;
    bool sysml_conditional_4 = 0;
    bool sysml_conditional_5 = 0;
    bool sysml_conditional_6 = 0;
    bool sysml_conditional_7 = 0;
    sysml_state_timeAccumMs = (sysml_state_timeAccumMs + io_9);
    if ((sysml_state_timeAccumMs >= 2000)) {
        sysml_state_timeAccumMs = (sysml_state_timeAccumMs - 2000);
        if (((sysml_state_servo1Angle + 30.0) > 180.0)) {
            sysml_state_servo1Angle = 0.0;
            sysml_state_servo2Angle = 0.0;
        }
        else {
            sysml_state_servo1Angle = (sysml_state_servo1Angle + 30.0);
            sysml_state_servo2Angle = (sysml_state_servo2Angle + 30.0);
        }
    }
    double sysml_value_0 = 0;
    sysml_value_0 = sysml_state_servo1Angle;
    double sysml_value_1 = 0;
    sysml_value_1 = sysml_state_servo2Angle;
    bool sysml_value_2 = 0;
    sysml_value_2 = false;
    bool sysml_value_3 = 0;
    sysml_value_3 = false;
    bool sysml_value_4 = 0;
    sysml_value_4 = true;
    bool sysml_value_5 = 0;
    sysml_value_5 = false;
    bool sysml_value_6 = 0;
    sysml_value_6 = false;
    bool sysml_value_7 = 0;
    sysml_value_7 = true;
    bool sysml_value_8 = 0;
    sysml_lazy_0 = (((double)(sysml_state_servo1Angle + sysml_state_servo2Angle)) / 2.0) >= 0.0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = (((double)(sysml_state_servo1Angle + sysml_state_servo2Angle)) / 2.0) <= 90.0;
    }
    if (sysml_lazy_0) {
        sysml_conditional_1 = true;
    }
    else {
        sysml_conditional_1 = false;
    }
    sysml_value_8 = sysml_conditional_1;
    bool sysml_value_9 = 0;
    sysml_lazy_2 = (((double)(sysml_state_servo1Angle + sysml_state_servo2Angle)) / 2.0) > 90.0;
    if (sysml_lazy_2) {
        sysml_lazy_2 = (((double)(sysml_state_servo1Angle + sysml_state_servo2Angle)) / 2.0) <= 180.0;
    }
    if (sysml_lazy_2) {
        sysml_conditional_3 = true;
    }
    else {
        sysml_conditional_3 = false;
    }
    sysml_value_9 = sysml_conditional_3;
    bool sysml_value_10 = 0;
    if (false) {
        sysml_conditional_4 = true;
    }
    else {
        sysml_conditional_4 = false;
    }
    sysml_value_10 = sysml_conditional_4;
    bool sysml_value_11 = 0;
    if (false) {
        sysml_conditional_5 = true;
    }
    else {
        sysml_conditional_5 = false;
    }
    sysml_value_11 = sysml_conditional_5;
    bool sysml_value_12 = 0;
    if (false) {
        sysml_conditional_6 = true;
    }
    else {
        sysml_conditional_6 = false;
    }
    sysml_value_12 = sysml_conditional_6;
    bool sysml_value_13 = 0;
    if (false) {
        sysml_conditional_7 = true;
    }
    else {
        sysml_conditional_7 = false;
    }
    sysml_value_13 = sysml_conditional_7;
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