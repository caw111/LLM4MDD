#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_servo1Angle = 0.0;
/* Global Variable define of  */
double sysml_state_servo2Angle = 0.0;
/* Global Variable define of  */
bool sysml_state_previousBtn1 = false;
/* Global Variable define of  */
bool sysml_state_previousBtn2 = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_servo1Angle = 0.0;
    sysml_state_servo2Angle = 0.0;
    sysml_state_previousBtn1 = false;
    sysml_state_previousBtn2 = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double io_2, double io_3, double* io_4, double* io_5) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    double sysml_conditional_1 = 0;
    double sysml_conditional_2 = 0;
    bool sysml_lazy_3 = 0;
    double sysml_conditional_4 = 0;
    double sysml_conditional_5 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousBtn1;
    }
    if (sysml_lazy_0) {
        if (io_2 >= 1023.0) {
            sysml_conditional_1 = 180.0;
        }
        else {
            if (io_2 <= 0.0) {
                sysml_conditional_2 = 0.0;
            }
            else {
                sysml_conditional_2 = ((double)(io_2 * 180.0)) / 1023.0;
            }
            sysml_conditional_1 = sysml_conditional_2;
        }
        sysml_state_servo1Angle = sysml_conditional_1;
    }
    sysml_lazy_3 = io_1;
    if (sysml_lazy_3) {
        sysml_lazy_3 = !sysml_state_previousBtn2;
    }
    if (sysml_lazy_3) {
        if (io_3 >= 1023.0) {
            sysml_conditional_4 = 180.0;
        }
        else {
            if (io_3 <= 0.0) {
                sysml_conditional_5 = 0.0;
            }
            else {
                sysml_conditional_5 = ((double)(io_3 * 180.0)) / 1023.0;
            }
            sysml_conditional_4 = sysml_conditional_5;
        }
        sysml_state_servo2Angle = sysml_conditional_4;
    }
    sysml_state_previousBtn1 = io_0;
    sysml_state_previousBtn2 = io_1;
    double sysml_value_0 = 0;
    sysml_value_0 = sysml_state_servo1Angle;
    double sysml_value_1 = 0;
    sysml_value_1 = sysml_state_servo2Angle;
    *io_4 = sysml_value_0;
    *io_5 = sysml_value_1;
}