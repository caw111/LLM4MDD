#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_brightness = 0.0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_brightness = 0.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_1, double* io_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_conditional_0 = 0;
    double sysml_conditional_1 = 0;
    if (sysml_state_brightness >= 255.0) {
        sysml_conditional_0 = 0.0;
    }
    else {
        sysml_conditional_0 = sysml_state_brightness + (((double)(io_1 * 255.0)) / 1000.0);
    }
    sysml_state_brightness = sysml_conditional_0;
    if (sysml_state_brightness >= 255.0) {
        sysml_conditional_1 = 255.0;
    }
    else {
        sysml_conditional_1 = sysml_state_brightness;
    }
    sysml_state_brightness = sysml_conditional_1;
    double sysml_value_0 = 0;
    sysml_value_0 = sysml_state_brightness;
    *io_0 = sysml_value_0;
}