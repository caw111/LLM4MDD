#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_currentPositionDeg = 0;
/* Global Variable define of  */
bool sysml_state_directionForward = true;
/* Global Variable define of  */
bool sysml_state_moving = true;
/* Global Variable define of  */
bool sysml_state_reportPending = true;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_currentPositionDeg = 0;
    sysml_state_directionForward = true;
    sysml_state_moving = true;
    sysml_state_reportPending = true;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double* io_0, bool* serial_0_trigger, int* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    if (sysml_state_moving) {
        if (sysml_state_directionForward) {
            if ((sysml_state_currentPositionDeg < 180)) {
                sysml_state_currentPositionDeg = (sysml_state_currentPositionDeg + 1);
            }
            else {
                sysml_state_moving = false;
                sysml_state_reportPending = true;
            }
        }
        else {
            if ((sysml_state_currentPositionDeg > 0)) {
                sysml_state_currentPositionDeg = (sysml_state_currentPositionDeg - 1);
            }
            else {
                sysml_state_moving = false;
                sysml_state_reportPending = true;
            }
        }
    }
    else {
        if (sysml_state_reportPending) {
            sysml_state_reportPending = false;
            sysml_state_directionForward = (!sysml_state_directionForward);
            sysml_state_moving = true;
        }
    }
    double sysml_value_0 = 0;
    sysml_value_0 = sysml_state_currentPositionDeg;
    bool sysml_value_1 = 0;
    sysml_value_1 = sysml_state_reportPending;
    int sysml_value_2 = 0;
    if (sysml_state_reportPending) {
        sysml_conditional_0 = sysml_state_currentPositionDeg;
    }
    else {
        sysml_conditional_0 = 0;
    }
    sysml_value_2 = sysml_conditional_0;
    *io_0 = sysml_value_0;
    *serial_0_trigger = sysml_value_1;
    *serial_0_value_0 = sysml_value_2;
}