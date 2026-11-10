#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_servoAngle = 90.0;
/* Global Variable define of  */
bool sysml_state_previousBtn1 = false;
/* Global Variable define of  */
bool sysml_state_previousBtn2 = false;
/* Global Variable define of  */
bool sysml_state_movingLed = false;
/* Global Variable define of  */
double sysml_state_movingUntilAngle = 90.0;
/* Global Variable define of  */
bool sysml_state_movingHoldActive = false;
/* Global Variable define of  */
int sysml_state_movingHoldMsRemaining = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_servoAngle = 90.0;
    sysml_state_previousBtn1 = false;
    sysml_state_previousBtn2 = false;
    sysml_state_movingLed = false;
    sysml_state_movingUntilAngle = 90.0;
    sysml_state_movingHoldActive = false;
    sysml_state_movingHoldMsRemaining = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_1, bool io_2, bool* io_0, double* io_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    double sysml_conditional_1 = 0;
    double sysml_conditional_2 = 0;
    bool sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    bool sysml_lazy_5 = 0;
    double sysml_conditional_6 = 0;
    double sysml_conditional_7 = 0;
    bool sysml_conditional_8 = 0;
    int sysml_conditional_9 = 0;
    bool sysml_lazy_10 = 0;
    sysml_state_previousBtn1 = io_1;
    sysml_state_previousBtn2 = io_2;
    sysml_state_movingLed = false;
    sysml_state_movingHoldActive = sysml_state_movingHoldActive;
    sysml_lazy_0 = io_1;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousBtn1;
    }
    if (sysml_lazy_0) {
        if (sysml_state_servoAngle <= 150.0) {
            sysml_conditional_1 = sysml_state_servoAngle + 30.0;
        }
        else {
            sysml_conditional_1 = sysml_state_servoAngle;
        }
        sysml_state_servoAngle = sysml_conditional_1;
        if (sysml_state_servoAngle <= 150.0) {
            sysml_conditional_2 = sysml_state_servoAngle + 30.0;
        }
        else {
            sysml_conditional_2 = sysml_state_servoAngle;
        }
        sysml_state_movingUntilAngle = sysml_conditional_2;
        if (sysml_state_servoAngle <= 150.0) {
            sysml_conditional_3 = true;
        }
        else {
            sysml_conditional_3 = false;
        }
        sysml_state_movingHoldActive = sysml_conditional_3;
        if (sysml_state_servoAngle <= 150.0) {
            sysml_conditional_4 = 2000;
        }
        else {
            sysml_conditional_4 = 0;
        }
        sysml_state_movingHoldMsRemaining = sysml_conditional_4;
    }
    sysml_lazy_5 = io_2;
    if (sysml_lazy_5) {
        sysml_lazy_5 = !sysml_state_previousBtn2;
    }
    if (sysml_lazy_5) {
        if (sysml_state_servoAngle >= 30.0) {
            sysml_conditional_6 = sysml_state_servoAngle - 30.0;
        }
        else {
            sysml_conditional_6 = sysml_state_servoAngle;
        }
        sysml_state_servoAngle = sysml_conditional_6;
        if (sysml_state_servoAngle >= 30.0) {
            sysml_conditional_7 = sysml_state_servoAngle - 30.0;
        }
        else {
            sysml_conditional_7 = sysml_state_servoAngle;
        }
        sysml_state_movingUntilAngle = sysml_conditional_7;
        if (sysml_state_servoAngle >= 30.0) {
            sysml_conditional_8 = true;
        }
        else {
            sysml_conditional_8 = false;
        }
        sysml_state_movingHoldActive = sysml_conditional_8;
        if (sysml_state_servoAngle >= 30.0) {
            sysml_conditional_9 = 2000;
        }
        else {
            sysml_conditional_9 = 0;
        }
        sysml_state_movingHoldMsRemaining = sysml_conditional_9;
    }
    if (sysml_state_movingHoldActive) {
        sysml_state_movingLed = true;
    }
    sysml_lazy_10 = sysml_state_movingHoldActive;
    if (sysml_lazy_10) {
        sysml_lazy_10 = sysml_state_servoAngle == sysml_state_movingUntilAngle;
    }
    if (sysml_lazy_10) {
        sysml_state_movingLed = false;
        sysml_state_movingHoldActive = false;
        sysml_state_movingHoldMsRemaining = 0;
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_movingLed;
    double sysml_value_1 = 0;
    sysml_value_1 = sysml_state_servoAngle;
    *io_0 = sysml_value_0;
    *io_3 = sysml_value_1;
}