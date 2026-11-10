#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_io_2_lastValidDistCmQuantized = 10000;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_io_2_lastValidDistCmQuantized = 10000;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, int* io_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    int sysml_conditional_6 = 0;
    if (io_1) {
        if (io_0 < 2.0) {
            sysml_conditional_0 = 10000;
        }
        else {
            if (io_0 > 400.0) {
                sysml_conditional_1 = 10000;
            }
            else {
                if ((io_0 + 0.5) < ((double)((int)(io_0 + 0.5)))) {
                    sysml_conditional_2 = 1;
                }
                else {
                    sysml_conditional_2 = 0;
                }
                sysml_conditional_1 = ((int)(io_0 + 0.5)) - sysml_conditional_2;
            }
            sysml_conditional_0 = sysml_conditional_1;
        }
        sysml_state_io_2_lastValidDistCmQuantized = sysml_conditional_0;
    }
    int sysml_value_0 = 0;
    if (io_1) {
        if (io_0 < 2.0) {
            sysml_conditional_4 = 10000;
        }
        else {
            if (io_0 > 400.0) {
                sysml_conditional_5 = 10000;
            }
            else {
                if ((io_0 + 0.5) < ((double)((int)(io_0 + 0.5)))) {
                    sysml_conditional_6 = 1;
                }
                else {
                    sysml_conditional_6 = 0;
                }
                sysml_conditional_5 = ((int)(io_0 + 0.5)) - sysml_conditional_6;
            }
            sysml_conditional_4 = sysml_conditional_5;
        }
        sysml_conditional_3 = sysml_conditional_4;
    }
    else {
        sysml_conditional_3 = sysml_state_io_2_lastValidDistCmQuantized;
    }
    sysml_value_0 = sysml_conditional_3;
    *io_2 = sysml_value_0;
}