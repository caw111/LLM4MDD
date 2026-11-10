#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_lastServoAngleDeg = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_lastServoAngleDeg = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, int io_2, bool io_2_valid, double* io_1, bool* serial_0_trigger, int* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    if (true) {
        if (io_0 <= 0.0) {
            sysml_conditional_0 = 0;
        }
        else {
            if (io_0 >= 1023.0) {
                sysml_conditional_1 = 180;
            }
            else {
                if ((((double)(io_0 * 180.0)) / 1023.0) < ((double)((int)(((double)(io_0 * 180.0)) / 1023.0)))) {
                    sysml_conditional_2 = 1;
                }
                else {
                    sysml_conditional_2 = 0;
                }
                sysml_conditional_1 = ((int)(((double)(io_0 * 180.0)) / 1023.0)) - sysml_conditional_2;
            }
            sysml_conditional_0 = sysml_conditional_1;
        }
        sysml_state_lastServoAngleDeg = sysml_conditional_0;
    }
    if (io_2_valid) {
    }
    double sysml_value_0 = 0;
    sysml_value_0 = sysml_state_lastServoAngleDeg;
    bool sysml_value_1 = 0;
    sysml_value_1 = true;
    int sysml_value_2 = 0;
    sysml_value_2 = sysml_state_lastServoAngleDeg;
    *io_1 = sysml_value_0;
    *serial_0_trigger = sysml_value_1;
    *serial_0_value_0 = sysml_value_2;
}