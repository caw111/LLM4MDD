#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_servo1_angle_deg = 0.0;
/* Global Variable define of  */
double sysml_state_servo2_angle_deg = 0.0;
/* Global Variable define of  */
int sysml_state_display_value_i32 = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_servo1_angle_deg = 0.0;
    sysml_state_servo2_angle_deg = 0.0;
    sysml_state_display_value_i32 = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_1, double io_2, int io_5, int* io_0, double* io_3, double* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    double sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    double sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    double sysml_conditional_5 = 0;
    int sysml_conditional_6 = 0;
    double sysml_conditional_7 = 0;
    int sysml_conditional_8 = 0;
    int sysml_conditional_9 = 0;
    int sysml_conditional_10 = 0;
    sysml_lazy_0 = io_1 >= 0.0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_1 <= 1023.0;
    }
    if (sysml_lazy_0) {
        if ((((double)(io_1 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_1 * 99.0)) / 1023.0)))) {
            sysml_conditional_2 = 1;
        }
        else {
            sysml_conditional_2 = 0;
        }
        sysml_conditional_1 = ((int)(((double)(io_1 * 99.0)) / 1023.0)) - sysml_conditional_2;
    }
    else {
        if (io_1 < 0.0) {
            sysml_conditional_3 = 0.0;
        }
        else {
            sysml_conditional_3 = 99.0;
        }
        sysml_conditional_1 = sysml_conditional_3;
    }
    sysml_state_servo1_angle_deg = sysml_conditional_1;
    sysml_lazy_4 = io_2 >= 0.0;
    if (sysml_lazy_4) {
        sysml_lazy_4 = io_2 <= 1023.0;
    }
    if (sysml_lazy_4) {
        if ((((double)(io_2 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_2 * 99.0)) / 1023.0)))) {
            sysml_conditional_6 = 1;
        }
        else {
            sysml_conditional_6 = 0;
        }
        sysml_conditional_5 = ((int)(((double)(io_2 * 99.0)) / 1023.0)) - sysml_conditional_6;
    }
    else {
        if (io_2 < 0.0) {
            sysml_conditional_7 = 0.0;
        }
        else {
            sysml_conditional_7 = 99.0;
        }
        sysml_conditional_5 = sysml_conditional_7;
    }
    sysml_state_servo2_angle_deg = sysml_conditional_5;
    if (sysml_state_servo1_angle_deg < ((double)((int)sysml_state_servo1_angle_deg))) {
        sysml_conditional_8 = 1;
    }
    else {
        sysml_conditional_8 = 0;
    }
    sysml_state_display_value_i32 = ((int)sysml_state_servo1_angle_deg) - sysml_conditional_8;
    int sysml_value_0 = 0;
    if (sysml_state_display_value_i32 >= 99) {
        sysml_conditional_9 = 99;
    }
    else {
        if (sysml_state_display_value_i32 < 0) {
            sysml_conditional_10 = 0;
        }
        else {
            sysml_conditional_10 = sysml_state_display_value_i32;
        }
        sysml_conditional_9 = sysml_conditional_10;
    }
    sysml_value_0 = sysml_conditional_9;
    double sysml_value_1 = 0;
    sysml_value_1 = sysml_state_servo1_angle_deg;
    double sysml_value_2 = 0;
    sysml_value_2 = sysml_state_servo2_angle_deg;
    *io_0 = sysml_value_0;
    *io_3 = sysml_value_1;
    *io_4 = sysml_value_2;
}