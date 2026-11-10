#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_colorIndex = 0;
/* Global Variable define of  */
int sysml_state_digit = 9;
/* Global Variable define of  */
double sysml_state_servoAngle = 0.0;
/* Global Variable define of  */
bool sysml_state_phaseInitialized = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_colorIndex = 0;
    sysml_state_digit = 9;
    sysml_state_servoAngle = 0.0;
    sysml_state_phaseInitialized = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double* io_0, double* io_1, double* io_2, double* io_3, int* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    if ((!sysml_state_phaseInitialized)) {
        sysml_state_colorIndex = 0;
        sysml_state_digit = 9;
        sysml_state_servoAngle = 0.0;
        sysml_state_phaseInitialized = true;
    }
    double sysml_value_0 = 0;
    sysml_value_0 = 0.0;
    double sysml_value_1 = 0;
    sysml_value_1 = 0.0;
    double sysml_value_2 = 0;
    sysml_value_2 = 0.0;
    double sysml_value_3 = 0;
    sysml_value_3 = sysml_state_servoAngle;
    int sysml_value_4 = 0;
    sysml_value_4 = sysml_state_digit;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
    *io_4 = sysml_value_4;
}