#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_lastValidTempC = 0.0;
/* Global Variable define of  */
int sysml_state_lastUpdatedMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_lastValidTempC = 0.0;
    sysml_state_lastUpdatedMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, bool io_2, int* io_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    int sysml_conditional_6 = 0;
    if (io_2) {
        sysml_state_lastValidTempC = io_0;
    }
    int sysml_value_0 = 0;
    if (io_2) {
        if (io_0 < 0.0) {
            sysml_conditional_1 = 0;
        }
        else {
            if (io_0 > 2147483647.0) {
                sysml_conditional_2 = 2147483647;
            }
            else {
                if (io_0 < ((double)((int)io_0))) {
                    sysml_conditional_3 = 1;
                }
                else {
                    sysml_conditional_3 = 0;
                }
                sysml_conditional_2 = ((int)io_0) - sysml_conditional_3;
            }
            sysml_conditional_1 = sysml_conditional_2;
        }
        sysml_conditional_0 = sysml_conditional_1;
    }
    else {
        if (sysml_state_lastValidTempC < 0.0) {
            sysml_conditional_4 = 0;
        }
        else {
            if (sysml_state_lastValidTempC > 2147483647.0) {
                sysml_conditional_5 = 2147483647;
            }
            else {
                if (sysml_state_lastValidTempC < ((double)((int)sysml_state_lastValidTempC))) {
                    sysml_conditional_6 = 1;
                }
                else {
                    sysml_conditional_6 = 0;
                }
                sysml_conditional_5 = ((int)sysml_state_lastValidTempC) - sysml_conditional_6;
            }
            sysml_conditional_4 = sysml_conditional_5;
        }
        sysml_conditional_0 = sysml_conditional_4;
    }
    sysml_value_0 = sysml_conditional_0;
    *io_3 = sysml_value_0;
}