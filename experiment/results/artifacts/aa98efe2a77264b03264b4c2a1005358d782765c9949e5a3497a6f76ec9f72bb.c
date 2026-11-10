#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_sevsegHoldDigit = 0;
/* Global Variable define of  */
double sysml_state_servo1HoldAngle = 0.0;
/* Global Variable define of  */
double sysml_state_servo2HoldAngle = 90.0;
/* Global Variable define of  */
double sysml_state_servo3HoldAngle = 180.0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_sevsegHoldDigit = 0;
    sysml_state_servo1HoldAngle = 0.0;
    sysml_state_servo2HoldAngle = 90.0;
    sysml_state_servo3HoldAngle = 180.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, double* io_2, double* io_3, double* io_4, int* io_5) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    double sysml_conditional_3 = 0;
    double sysml_conditional_4 = 0;
    double sysml_conditional_5 = 0;
    double sysml_conditional_6 = 0;
    double sysml_conditional_7 = 0;
    double sysml_conditional_8 = 0;
    if (io_0 <= 0.0) {
        sysml_conditional_0 = 0;
    }
    else {
        if (io_0 >= 1023.0) {
            sysml_conditional_1 = 9;
        }
        else {
            if ((((double)(io_0 * 10.0)) / 1023.0) < ((double)((int)(((double)(io_0 * 10.0)) / 1023.0)))) {
                sysml_conditional_2 = 1;
            }
            else {
                sysml_conditional_2 = 0;
            }
            sysml_conditional_1 = ((int)(((double)(io_0 * 10.0)) / 1023.0)) - sysml_conditional_2;
        }
        sysml_conditional_0 = sysml_conditional_1;
    }
    sysml_state_sevsegHoldDigit = sysml_conditional_0;
    if (io_1 <= 0.0) {
        sysml_conditional_3 = 0.0;
    }
    else {
        if (io_1 >= 1023.0) {
            sysml_conditional_4 = 90.0;
        }
        else {
            sysml_conditional_4 = ((double)(io_1 * 90.0)) / 1023.0;
        }
        sysml_conditional_3 = sysml_conditional_4;
    }
    sysml_state_servo1HoldAngle = sysml_conditional_3;
    if (io_1 <= 0.0) {
        sysml_conditional_5 = 90.0;
    }
    else {
        if (io_1 >= 1023.0) {
            sysml_conditional_6 = 180.0;
        }
        else {
            sysml_conditional_6 = 90.0 + (((double)(io_1 * 90.0)) / 1023.0);
        }
        sysml_conditional_5 = sysml_conditional_6;
    }
    sysml_state_servo2HoldAngle = sysml_conditional_5;
    if (io_1 <= 0.0) {
        sysml_conditional_7 = 180.0;
    }
    else {
        if (io_1 >= 1023.0) {
            sysml_conditional_8 = 270.0;
        }
        else {
            sysml_conditional_8 = 180.0 + (((double)(io_1 * 90.0)) / 1023.0);
        }
        sysml_conditional_7 = sysml_conditional_8;
    }
    sysml_state_servo3HoldAngle = sysml_conditional_7;
    double sysml_value_0 = 0;
    sysml_value_0 = sysml_state_servo1HoldAngle;
    double sysml_value_1 = 0;
    sysml_value_1 = sysml_state_servo2HoldAngle;
    double sysml_value_2 = 0;
    sysml_value_2 = sysml_state_servo3HoldAngle;
    int sysml_value_3 = 0;
    sysml_value_3 = sysml_state_sevsegHoldDigit;
    *io_2 = sysml_value_0;
    *io_3 = sysml_value_1;
    *io_4 = sysml_value_2;
    *io_5 = sysml_value_3;
}