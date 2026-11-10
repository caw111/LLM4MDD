#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_brightness = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_brightness = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_1, double* io_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    double sysml_conditional_3 = 0;
    if (sysml_state_brightness >= 255) {
        if (sysml_state_brightness == 255) {
            if (io_1 > 0) {
                sysml_conditional_2 = 0;
            }
            else {
                sysml_conditional_2 = 0;
            }
            sysml_conditional_1 = sysml_conditional_2;
        }
        else {
            sysml_conditional_1 = 0;
        }
        sysml_conditional_0 = sysml_conditional_1;
    }
    else {
        sysml_conditional_0 = sysml_state_brightness + io_1;
    }
    sysml_state_brightness = sysml_conditional_0;
    if ((sysml_state_brightness > 255)) {
        sysml_state_brightness = 0;
    }
    double sysml_value_0 = 0;
    if (sysml_state_brightness <= 255) {
        sysml_conditional_3 = sysml_state_brightness;
    }
    else {
        sysml_conditional_3 = 0.0;
    }
    sysml_value_0 = sysml_conditional_3;
    *io_0 = sysml_value_0;
}