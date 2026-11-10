#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_fanOn = false;
/* Global Variable define of  */
int sysml_state_lastDisplayedC = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_fanOn = false;
    sysml_state_lastDisplayedC = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_1, double io_2, bool io_3, double* io_0, int* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    double sysml_conditional_3 = 0;
    if (io_3) {
        sysml_state_fanOn = (io_1 > 30.0);
        if (io_1 >= 0.0) {
            if (io_1 < ((double)((int)io_1))) {
                sysml_conditional_1 = 1;
            }
            else {
                sysml_conditional_1 = 0;
            }
            sysml_conditional_0 = ((int)io_1) - sysml_conditional_1;
        }
        else {
            if ((0.0 - io_1) < ((double)((int)(0.0 - io_1)))) {
                sysml_conditional_2 = 1;
            }
            else {
                sysml_conditional_2 = 0;
            }
            sysml_conditional_0 = -(((int)(0.0 - io_1)) - sysml_conditional_2);
        }
        sysml_state_lastDisplayedC = sysml_conditional_0;
    }
    double sysml_value_0 = 0;
    if (sysml_state_fanOn) {
        sysml_conditional_3 = 255.0;
    }
    else {
        sysml_conditional_3 = 0.0;
    }
    sysml_value_0 = sysml_conditional_3;
    int sysml_value_1 = 0;
    sysml_value_1 = sysml_state_lastDisplayedC;
    *io_0 = sysml_value_0;
    *io_4 = sysml_value_1;
}