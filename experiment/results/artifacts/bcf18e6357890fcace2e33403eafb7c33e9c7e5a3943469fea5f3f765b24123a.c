#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_servoHoldAccumMs1 = 0;
/* Global Variable define of  */
int sysml_state_servoHoldAccumMs2 = 0;
/* Global Variable define of  */
int sysml_state_servoHoldAccumMs3 = 0;
/* Global Variable define of  */
double sysml_state_servoAngle1 = 0.0;
/* Global Variable define of  */
double sysml_state_servoAngle2 = 0.0;
/* Global Variable define of  */
double sysml_state_servoAngle3 = 0.0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_servoHoldAccumMs1 = 0;
    sysml_state_servoHoldAccumMs2 = 0;
    sysml_state_servoHoldAccumMs3 = 0;
    sysml_state_servoAngle1 = 0.0;
    sysml_state_servoAngle2 = 0.0;
    sysml_state_servoAngle3 = 0.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, int io_10, double* io_1, double* io_2, double* io_3, double* io_4, double* io_5, double* io_6, double* io_7, double* io_8, double* io_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_conditional_0 = 0;
    double sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    double sysml_conditional_3 = 0;
    double sysml_conditional_4 = 0;
    double sysml_conditional_5 = 0;
    double sysml_conditional_6 = 0;
    double sysml_conditional_7 = 0;
    bool sysml_lazy_8 = 0;
    double sysml_conditional_9 = 0;
    double sysml_conditional_10 = 0;
    double sysml_conditional_11 = 0;
    sysml_state_servoHoldAccumMs1 = (sysml_state_servoHoldAccumMs1 + io_10);
    sysml_state_servoHoldAccumMs2 = (sysml_state_servoHoldAccumMs2 + io_10);
    sysml_state_servoHoldAccumMs3 = (sysml_state_servoHoldAccumMs3 + io_10);
    if ((sysml_state_servoHoldAccumMs1 >= 2000)) {
        sysml_state_servoHoldAccumMs1 = (sysml_state_servoHoldAccumMs1 - 2000);
        sysml_state_servoAngle1 = ((double)((io_0 * 180.0)) / 1023.0);
    }
    if ((sysml_state_servoHoldAccumMs2 >= 2000)) {
        sysml_state_servoHoldAccumMs2 = (sysml_state_servoHoldAccumMs2 - 2000);
        sysml_state_servoAngle2 = ((double)((io_0 * 180.0)) / 1023.0);
    }
    if ((sysml_state_servoHoldAccumMs3 >= 2000)) {
        sysml_state_servoHoldAccumMs3 = (sysml_state_servoHoldAccumMs3 - 2000);
        sysml_state_servoAngle3 = ((double)((io_0 * 180.0)) / 1023.0);
    }
    double sysml_value_0 = 0;
    if (io_0 <= 341.0) {
        sysml_conditional_0 = ((double)(io_0 * 255.0)) / 341.0;
    }
    else {
        if (io_0 <= 682.0) {
            sysml_conditional_1 = 255.0;
        }
        else {
            sysml_conditional_1 = 0.0;
        }
        sysml_conditional_0 = sysml_conditional_1;
    }
    sysml_value_0 = sysml_conditional_0;
    double sysml_value_1 = 0;
    sysml_lazy_2 = io_0 >= 342.0;
    if (sysml_lazy_2) {
        sysml_lazy_2 = io_0 <= 682.0;
    }
    if (sysml_lazy_2) {
        sysml_conditional_3 = ((double)((io_0 - 341.0) * 255.0)) / 341.0;
    }
    else {
        if (io_0 < 342.0) {
            sysml_conditional_4 = 0.0;
        }
        else {
            sysml_conditional_4 = 255.0;
        }
        sysml_conditional_3 = sysml_conditional_4;
    }
    sysml_value_1 = sysml_conditional_3;
    double sysml_value_2 = 0;
    if (io_0 >= 683.0) {
        sysml_conditional_5 = ((double)((io_0 - 682.0) * 255.0)) / 341.0;
    }
    else {
        sysml_conditional_5 = 0.0;
    }
    sysml_value_2 = sysml_conditional_5;
    double sysml_value_3 = 0;
    if (io_0 <= 341.0) {
        sysml_conditional_6 = ((double)(io_0 * 255.0)) / 341.0;
    }
    else {
        if (io_0 <= 682.0) {
            sysml_conditional_7 = 255.0;
        }
        else {
            sysml_conditional_7 = 0.0;
        }
        sysml_conditional_6 = sysml_conditional_7;
    }
    sysml_value_3 = sysml_conditional_6;
    double sysml_value_4 = 0;
    sysml_lazy_8 = io_0 >= 342.0;
    if (sysml_lazy_8) {
        sysml_lazy_8 = io_0 <= 682.0;
    }
    if (sysml_lazy_8) {
        sysml_conditional_9 = ((double)((io_0 - 341.0) * 255.0)) / 341.0;
    }
    else {
        if (io_0 < 342.0) {
            sysml_conditional_10 = 0.0;
        }
        else {
            sysml_conditional_10 = 255.0;
        }
        sysml_conditional_9 = sysml_conditional_10;
    }
    sysml_value_4 = sysml_conditional_9;
    double sysml_value_5 = 0;
    if (io_0 >= 683.0) {
        sysml_conditional_11 = ((double)((io_0 - 682.0) * 255.0)) / 341.0;
    }
    else {
        sysml_conditional_11 = 0.0;
    }
    sysml_value_5 = sysml_conditional_11;
    double sysml_value_6 = 0;
    sysml_value_6 = sysml_state_servoAngle1;
    double sysml_value_7 = 0;
    sysml_value_7 = sysml_state_servoAngle2;
    double sysml_value_8 = 0;
    sysml_value_8 = sysml_state_servoAngle3;
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
    *io_3 = sysml_value_2;
    *io_4 = sysml_value_3;
    *io_5 = sysml_value_4;
    *io_6 = sysml_value_5;
    *io_7 = sysml_value_6;
    *io_8 = sysml_value_7;
    *io_9 = sysml_value_8;
}