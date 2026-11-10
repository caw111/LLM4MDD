#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_redLocked = 0.0;
/* Global Variable define of  */
double sysml_state_greenLocked = 0.0;
/* Global Variable define of  */
double sysml_state_blueLocked = 0.0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_redLocked = 0.0;
    sysml_state_greenLocked = 0.0;
    sysml_state_blueLocked = 0.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, double io_3, double io_4, double io_5, double* io_6, double* io_7, double* io_8) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_conditional_0 = 0;
    double sysml_conditional_1 = 0;
    double sysml_conditional_2 = 0;
    double sysml_conditional_3 = 0;
    double sysml_conditional_4 = 0;
    double sysml_conditional_5 = 0;
    if (io_0) {
        if (io_3 >= 255.0) {
            sysml_conditional_0 = 255.0;
        }
        else {
            if (io_3 <= 0.0) {
                sysml_conditional_1 = 0.0;
            }
            else {
                sysml_conditional_1 = io_3;
            }
            sysml_conditional_0 = sysml_conditional_1;
        }
        sysml_state_redLocked = sysml_conditional_0;
    }
    if (io_1) {
        if (io_4 >= 255.0) {
            sysml_conditional_2 = 255.0;
        }
        else {
            if (io_4 <= 0.0) {
                sysml_conditional_3 = 0.0;
            }
            else {
                sysml_conditional_3 = io_4;
            }
            sysml_conditional_2 = sysml_conditional_3;
        }
        sysml_state_greenLocked = sysml_conditional_2;
    }
    if (io_2) {
        if (io_5 >= 255.0) {
            sysml_conditional_4 = 255.0;
        }
        else {
            if (io_5 <= 0.0) {
                sysml_conditional_5 = 0.0;
            }
            else {
                sysml_conditional_5 = io_5;
            }
            sysml_conditional_4 = sysml_conditional_5;
        }
        sysml_state_blueLocked = sysml_conditional_4;
    }
    double sysml_value_0 = 0;
    sysml_value_0 = sysml_state_redLocked;
    double sysml_value_1 = 0;
    sysml_value_1 = sysml_state_greenLocked;
    double sysml_value_2 = 0;
    sysml_value_2 = sysml_state_blueLocked;
    *io_6 = sysml_value_0;
    *io_7 = sysml_value_1;
    *io_8 = sysml_value_2;
}