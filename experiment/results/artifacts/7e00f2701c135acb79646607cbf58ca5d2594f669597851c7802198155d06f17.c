#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_prevPot = 0.0;
/* Global Variable define of  */
bool sysml_state_servoHold1 = false;
/* Global Variable define of  */
bool sysml_state_servoHold2 = false;
/* Global Variable define of  */
bool sysml_state_servoHold3 = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_prevPot = 0.0;
    sysml_state_servoHold1 = false;
    sysml_state_servoHold2 = false;
    sysml_state_servoHold3 = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_6, double* io_0, double* io_1, double* io_2, double* io_3, double* io_4, double* io_5, double* io_7, double* io_8, double* io_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_conditional_0 = 0;
    bool sysml_lazy_1 = 0;
    double sysml_conditional_2 = 0;
    double sysml_conditional_3 = 0;
    double sysml_conditional_4 = 0;
    double sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    double sysml_conditional_7 = 0;
    double sysml_conditional_8 = 0;
    double sysml_conditional_9 = 0;
    sysml_state_prevPot = io_6;
    sysml_state_servoHold1 = false;
    sysml_state_servoHold2 = false;
    sysml_state_servoHold3 = false;
    double sysml_value_0 = 0;
    if (io_6 <= 341.0) {
        sysml_conditional_0 = ((double)(io_6 * 255.0)) / 341.0;
    }
    else {
        sysml_conditional_0 = 0.0;
    }
    sysml_value_0 = sysml_conditional_0;
    double sysml_value_1 = 0;
    sysml_lazy_1 = io_6 >= 342.0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = io_6 <= 682.0;
    }
    if (sysml_lazy_1) {
        sysml_conditional_2 = ((double)((io_6 - 342.0) * 255.0)) / 340.0;
    }
    else {
        if (io_6 < 342.0) {
            sysml_conditional_3 = 0.0;
        }
        else {
            sysml_conditional_3 = 0.0;
        }
        sysml_conditional_2 = sysml_conditional_3;
    }
    sysml_value_1 = sysml_conditional_2;
    double sysml_value_2 = 0;
    if (io_6 >= 683.0) {
        sysml_conditional_4 = ((double)((io_6 - 683.0) * 255.0)) / 340.0;
    }
    else {
        sysml_conditional_4 = 0.0;
    }
    sysml_value_2 = sysml_conditional_4;
    double sysml_value_3 = 0;
    if (io_6 <= 341.0) {
        sysml_conditional_5 = ((double)(io_6 * 255.0)) / 341.0;
    }
    else {
        sysml_conditional_5 = 0.0;
    }
    sysml_value_3 = sysml_conditional_5;
    double sysml_value_4 = 0;
    sysml_lazy_6 = io_6 >= 342.0;
    if (sysml_lazy_6) {
        sysml_lazy_6 = io_6 <= 682.0;
    }
    if (sysml_lazy_6) {
        sysml_conditional_7 = ((double)((io_6 - 342.0) * 255.0)) / 340.0;
    }
    else {
        if (io_6 < 342.0) {
            sysml_conditional_8 = 0.0;
        }
        else {
            sysml_conditional_8 = 0.0;
        }
        sysml_conditional_7 = sysml_conditional_8;
    }
    sysml_value_4 = sysml_conditional_7;
    double sysml_value_5 = 0;
    if (io_6 >= 683.0) {
        sysml_conditional_9 = ((double)((io_6 - 683.0) * 255.0)) / 340.0;
    }
    else {
        sysml_conditional_9 = 0.0;
    }
    sysml_value_5 = sysml_conditional_9;
    double sysml_value_6 = 0;
    sysml_value_6 = ((double)((io_6 * 180.0)) / 1023.0);
    double sysml_value_7 = 0;
    sysml_value_7 = ((double)((io_6 * 180.0)) / 1023.0);
    double sysml_value_8 = 0;
    sysml_value_8 = ((double)((io_6 * 180.0)) / 1023.0);
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
    *io_4 = sysml_value_4;
    *io_5 = sysml_value_5;
    *io_7 = sysml_value_6;
    *io_8 = sysml_value_7;
    *io_9 = sysml_value_8;
}