#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_servoAngleCurrent = 0.0;
/* Global Variable define of  */
double sysml_state_servoAngleTarget = 0.0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_servoAngleCurrent = 0.0;
    sysml_state_servoAngleTarget = 0.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, double io_2, double* io_3, bool* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_conditional_0 = 0;
    sysml_state_servoAngleTarget = ((double)((io_0 * 180.0)) / 1023.0);
    if ((sysml_state_servoAngleCurrent != sysml_state_servoAngleTarget)) {
        sysml_state_servoAngleCurrent = sysml_state_servoAngleTarget;
    }
    double sysml_value_0 = 0;
    sysml_value_0 = sysml_state_servoAngleCurrent;
    bool sysml_value_1 = 0;
    if (io_1 >= 512.0) {
        sysml_conditional_0 = true;
    }
    else {
        sysml_conditional_0 = false;
    }
    sysml_value_1 = sysml_conditional_0;
    *io_3 = sysml_value_0;
    *io_4 = sysml_value_1;
}