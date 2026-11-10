#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_servoAngleDeg = 0.0;
/* Global Variable define of  */
bool sysml_state_directionUp = true;
/* Global Variable define of  */
bool sysml_state_started = false;
/* Global Variable define of  */
bool sysml_state_reportPending = true;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_servoAngleDeg = 0.0;
    sysml_state_directionUp = true;
    sysml_state_started = false;
    sysml_state_reportPending = true;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double* io_0, bool* serial_0_trigger, int* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_conditional_0 = 0;
    double sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    if ((!sysml_state_started)) {
        sysml_state_started = true;
        sysml_state_directionUp = true;
        sysml_state_servoAngleDeg = 0.0;
        sysml_state_reportPending = true;
    }
    if (sysml_state_directionUp) {
        if (sysml_state_servoAngleDeg >= 180.0) {
            sysml_conditional_0 = 180.0;
        }
        else {
            sysml_conditional_0 = sysml_state_servoAngleDeg + 1.0;
        }
        sysml_state_servoAngleDeg = sysml_conditional_0;
        if ((sysml_state_servoAngleDeg >= 179.0)) {
            sysml_state_directionUp = false;
            sysml_state_reportPending = true;
        }
        else {
            sysml_state_reportPending = false;
        }
    }
    else {
        if (sysml_state_servoAngleDeg <= 0.0) {
            sysml_conditional_1 = 0.0;
        }
        else {
            sysml_conditional_1 = sysml_state_servoAngleDeg - 1.0;
        }
        sysml_state_servoAngleDeg = sysml_conditional_1;
        if ((sysml_state_servoAngleDeg <= 1.0)) {
            sysml_state_directionUp = true;
            sysml_state_reportPending = true;
        }
        else {
            sysml_state_reportPending = false;
        }
    }
    double sysml_value_0 = 0;
    sysml_value_0 = sysml_state_servoAngleDeg;
    bool sysml_value_1 = 0;
    sysml_value_1 = sysml_state_reportPending;
    int sysml_value_2 = 0;
    if (sysml_state_servoAngleDeg < ((double)((int)sysml_state_servoAngleDeg))) {
        sysml_conditional_2 = 1;
    }
    else {
        sysml_conditional_2 = 0;
    }
    sysml_value_2 = ((int)sysml_state_servoAngleDeg) - sysml_conditional_2;
    *io_0 = sysml_value_0;
    *serial_0_trigger = sysml_value_1;
    *serial_0_value_0 = sysml_value_2;
}