#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_servoAngleDeg = 0.0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_servoAngleDeg = 0.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, int io_2, double* io_1, bool* serial_0_trigger, double* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_conditional_0 = 0;
    double sysml_conditional_1 = 0;
    if ((((double)io_0) / 1023.0) <= 1.0) {
        sysml_conditional_0 = (((double)io_0) / 1023.0) * 180.0;
    }
    else {
        sysml_conditional_0 = 180.0;
    }
    sysml_state_servoAngleDeg = sysml_conditional_0;
    if (sysml_state_servoAngleDeg >= 0.0) {
        sysml_conditional_1 = sysml_state_servoAngleDeg;
    }
    else {
        sysml_conditional_1 = 0.0;
    }
    sysml_state_servoAngleDeg = sysml_conditional_1;
    double sysml_value_0 = 0;
    sysml_value_0 = sysml_state_servoAngleDeg;
    bool sysml_value_1 = 0;
    sysml_value_1 = true;
    double sysml_value_2 = 0;
    sysml_value_2 = sysml_state_servoAngleDeg;
    *io_1 = sysml_value_0;
    *serial_0_trigger = sysml_value_1;
    *serial_0_value_0 = sysml_value_2;
}