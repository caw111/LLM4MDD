#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_selectedChannel = 0;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
int sysml_state_pwmBrightness = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_selectedChannel = 0;
    sysml_state_previousPressed = false;
    sysml_state_pwmBrightness = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, double* io_2, double* io_3, double* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    int sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    double sysml_conditional_5 = 0;
    double sysml_conditional_6 = 0;
    double sysml_conditional_7 = 0;
    if (true) {
        if ((((double)(io_0 * 255.0)) / 1023.0) < ((double)((int)(((double)(io_0 * 255.0)) / 1023.0)))) {
            sysml_conditional_1 = 1;
        }
        else {
            sysml_conditional_1 = 0;
        }
        sysml_conditional_0 = ((int)(((double)(io_0 * 255.0)) / 1023.0)) - sysml_conditional_1;
    }
    else {
        sysml_conditional_0 = 0;
    }
    sysml_state_pwmBrightness = sysml_conditional_0;
    sysml_lazy_2 = io_1;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_2) {
        if (sysml_state_selectedChannel == 0) {
            sysml_conditional_3 = 1;
        }
        else {
            if (sysml_state_selectedChannel == 1) {
                sysml_conditional_4 = 2;
            }
            else {
                sysml_conditional_4 = 0;
            }
            sysml_conditional_3 = sysml_conditional_4;
        }
        sysml_state_selectedChannel = sysml_conditional_3;
    }
    sysml_state_previousPressed = io_1;
    double sysml_value_0 = 0;
    if (sysml_state_selectedChannel == 0) {
        sysml_conditional_5 = sysml_state_pwmBrightness;
    }
    else {
        sysml_conditional_5 = 0.0;
    }
    sysml_value_0 = sysml_conditional_5;
    double sysml_value_1 = 0;
    if (sysml_state_selectedChannel == 1) {
        sysml_conditional_6 = sysml_state_pwmBrightness;
    }
    else {
        sysml_conditional_6 = 0.0;
    }
    sysml_value_1 = sysml_conditional_6;
    double sysml_value_2 = 0;
    if (sysml_state_selectedChannel == 2) {
        sysml_conditional_7 = sysml_state_pwmBrightness;
    }
    else {
        sysml_conditional_7 = 0.0;
    }
    sysml_value_2 = sysml_conditional_7;
    *io_2 = sysml_value_0;
    *io_3 = sysml_value_1;
    *io_4 = sysml_value_2;
}