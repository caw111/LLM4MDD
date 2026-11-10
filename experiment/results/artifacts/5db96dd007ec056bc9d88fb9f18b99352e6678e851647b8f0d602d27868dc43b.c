#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_servoAngle = 90.0;
/* Global Variable define of  */
bool sysml_state_previousBtn1Pressed = false;
/* Global Variable define of  */
bool sysml_state_previousBtn2Pressed = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_servoAngle = 90.0;
    sysml_state_previousBtn1Pressed = false;
    sysml_state_previousBtn2Pressed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double* io_2, bool* io_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    double sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    double sysml_conditional_3 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousBtn1Pressed;
    }
    if (sysml_lazy_0) {
        if (sysml_state_servoAngle >= 150.0) {
            sysml_conditional_1 = sysml_state_servoAngle;
        }
        else {
            sysml_conditional_1 = sysml_state_servoAngle + 30.0;
        }
        sysml_state_servoAngle = sysml_conditional_1;
    }
    sysml_lazy_2 = io_1;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_previousBtn2Pressed;
    }
    if (sysml_lazy_2) {
        if (sysml_state_servoAngle <= 30.0) {
            sysml_conditional_3 = sysml_state_servoAngle;
        }
        else {
            sysml_conditional_3 = sysml_state_servoAngle - 30.0;
        }
        sysml_state_servoAngle = sysml_conditional_3;
    }
    sysml_state_previousBtn1Pressed = io_0;
    sysml_state_previousBtn2Pressed = io_1;
    double sysml_value_0 = 0;
    sysml_value_0 = sysml_state_servoAngle;
    bool sysml_value_1 = 0;
    sysml_value_1 = false;
    *io_2 = sysml_value_0;
    *io_3 = sysml_value_1;
}