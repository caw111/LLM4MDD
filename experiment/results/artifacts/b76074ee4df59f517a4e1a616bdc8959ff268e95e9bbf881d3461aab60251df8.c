#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_servoAngle = 90.0;
/* Global Variable define of  */
bool sysml_state_ledOn = false;
/* Global Variable define of  */
bool sysml_state_previousBtn1 = true;
/* Global Variable define of  */
bool sysml_state_previousBtn2 = true;
/* Global Variable define of  */
bool sysml_state_moving = false;
/* Global Variable define of  */
int sysml_state_remainingMoveMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_servoAngle = 90.0;
    sysml_state_ledOn = false;
    sysml_state_previousBtn1 = true;
    sysml_state_previousBtn2 = true;
    sysml_state_moving = false;
    sysml_state_remainingMoveMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double* io_2, bool* io_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    double sysml_conditional_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    double sysml_conditional_8 = 0;
    sysml_lazy_0 = sysml_state_moving;
    if (sysml_lazy_0) {
        sysml_lazy_0 = sysml_state_remainingMoveMs <= 0;
    }
    if (sysml_lazy_0) {
        sysml_state_moving = false;
        sysml_state_ledOn = false;
    }
    sysml_lazy_1 = io_0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_previousBtn1;
    }
    sysml_lazy_2 = sysml_lazy_1;
    if (sysml_lazy_2) {
        sysml_lazy_2 = sysml_state_servoAngle >= 0.0;
    }
    sysml_lazy_3 = sysml_lazy_2;
    if (sysml_lazy_3) {
        sysml_lazy_3 = sysml_state_servoAngle < 180.0;
    }
    if (sysml_lazy_3) {
        if ((sysml_state_servoAngle + 30.0) >= 180.0) {
            sysml_conditional_4 = 180.0;
        }
        else {
            sysml_conditional_4 = sysml_state_servoAngle + 30.0;
        }
        sysml_state_servoAngle = sysml_conditional_4;
        sysml_state_moving = true;
        sysml_state_remainingMoveMs = 2000;
        sysml_state_ledOn = true;
    }
    sysml_lazy_5 = io_1;
    if (sysml_lazy_5) {
        sysml_lazy_5 = !sysml_state_previousBtn2;
    }
    sysml_lazy_6 = sysml_lazy_5;
    if (sysml_lazy_6) {
        sysml_lazy_6 = sysml_state_servoAngle > 0.0;
    }
    sysml_lazy_7 = sysml_lazy_6;
    if (sysml_lazy_7) {
        sysml_lazy_7 = sysml_state_servoAngle <= 180.0;
    }
    if (sysml_lazy_7) {
        if ((sysml_state_servoAngle - 30.0) <= 0.0) {
            sysml_conditional_8 = 0.0;
        }
        else {
            sysml_conditional_8 = sysml_state_servoAngle - 30.0;
        }
        sysml_state_servoAngle = sysml_conditional_8;
        sysml_state_moving = true;
        sysml_state_remainingMoveMs = 2000;
        sysml_state_ledOn = true;
    }
    sysml_state_previousBtn1 = io_0;
    sysml_state_previousBtn2 = io_1;
    double sysml_value_0 = 0;
    sysml_value_0 = sysml_state_servoAngle;
    bool sysml_value_1 = 0;
    sysml_value_1 = sysml_state_ledOn;
    *io_2 = sysml_value_0;
    *io_3 = sysml_value_1;
}