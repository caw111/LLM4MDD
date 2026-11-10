#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_prevAngle = 0.0;
/* Global Variable define of  */
int sysml_state_holdMsCounter = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_prevAngle = 0.0;
    sysml_state_holdMsCounter = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_2, bool* io_0, bool* io_1, double* io_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_conditional_0 = 0;
    double sysml_conditional_1 = 0;
    double sysml_conditional_2 = 0;
    double sysml_conditional_3 = 0;
    double sysml_conditional_4 = 0;
    double sysml_conditional_5 = 0;
    double sysml_conditional_6 = 0;
    double sysml_conditional_7 = 0;
    sysml_state_holdMsCounter = 0;
    if (io_2 >= 0.0) {
        if (io_2 <= 1023.0) {
            sysml_conditional_1 = ((double)(io_2 * 180.0)) / 1023.0;
        }
        else {
            if (io_2 > 1023.0) {
                sysml_conditional_2 = 180.0;
            }
            else {
                sysml_conditional_2 = 0.0;
            }
            sysml_conditional_1 = sysml_conditional_2;
        }
        sysml_conditional_0 = sysml_conditional_1;
    }
    else {
        if (io_2 < 0.0) {
            sysml_conditional_3 = 0.0;
        }
        else {
            sysml_conditional_3 = ((double)(io_2 * 180.0)) / 1023.0;
        }
        sysml_conditional_0 = sysml_conditional_3;
    }
    sysml_state_prevAngle = sysml_conditional_0;
    double sysml_value_0 = 0;
    if (io_2 >= 0.0) {
        if (io_2 <= 1023.0) {
            sysml_conditional_5 = ((double)(io_2 * 180.0)) / 1023.0;
        }
        else {
            if (io_2 > 1023.0) {
                sysml_conditional_6 = 180.0;
            }
            else {
                sysml_conditional_6 = 0.0;
            }
            sysml_conditional_5 = sysml_conditional_6;
        }
        sysml_conditional_4 = sysml_conditional_5;
    }
    else {
        if (io_2 < 0.0) {
            sysml_conditional_7 = 0.0;
        }
        else {
            sysml_conditional_7 = ((double)(io_2 * 180.0)) / 1023.0;
        }
        sysml_conditional_4 = sysml_conditional_7;
    }
    sysml_value_0 = sysml_conditional_4;
    bool sysml_value_1 = 0;
    sysml_value_1 = false;
    bool sysml_value_2 = 0;
    sysml_value_2 = false;
    *io_0 = sysml_value_1;
    *io_1 = sysml_value_2;
    *io_3 = sysml_value_0;
}