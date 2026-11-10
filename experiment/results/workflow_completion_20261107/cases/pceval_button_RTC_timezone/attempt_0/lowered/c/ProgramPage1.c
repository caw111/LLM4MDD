#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_reportAge = 0;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
bool sysml_state_previousValid = false;
/* Global Variable define of  */
bool sysml_state_reportNow = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_reportAge = 0;
    sysml_state_previousPressed = false;
    sysml_state_previousValid = false;
    sysml_state_reportNow = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, int io_1, bool io_2, int io_3, bool* serial_0_trigger, int* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_arithmetic_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_arithmetic_4 = 0;
    int sysml_conditional_5 = 0;
    sysml_state_reportNow = false;
    if (io_2) {
        sysml_lazy_0 = !sysml_state_previousValid;
        if (!sysml_lazy_0) {
            sysml_lazy_0 = io_0 != sysml_state_previousPressed;
        }
        if (sysml_lazy_0) {
            sysml_state_reportNow = true;
            sysml_state_reportAge = 0;
        }
        else {
            if ((io_3 >= (100 - sysml_state_reportAge))) {
                sysml_state_reportNow = true;
            }
            if ((((double)io_3) / 100) < ((double)((int)(((double)io_3) / 100)))) {
                sysml_conditional_1 = 1;
            }
            else {
                sysml_conditional_1 = 0;
            }
            sysml_arithmetic_2 = ((int)(((double)io_3) / 100)) - sysml_conditional_1;
            sysml_state_reportAge = sysml_state_reportAge + (io_3 - (100 * sysml_arithmetic_2));
            if ((sysml_state_reportAge >= 100)) {
                sysml_state_reportAge = (sysml_state_reportAge - 100);
            }
        }
    }
    else {
        sysml_state_reportAge = 0;
    }
    sysml_state_previousValid = io_2;
    sysml_state_previousPressed = io_0;
    int sysml_value_0 = 0;
    if ((((double)io_3) / 100) < ((double)((int)(((double)io_3) / 100)))) {
        sysml_conditional_3 = 1;
    }
    else {
        sysml_conditional_3 = 0;
    }
    sysml_arithmetic_4 = ((int)(((double)io_3) / 100)) - sysml_conditional_3;
    sysml_value_0 = io_3 - (100 * sysml_arithmetic_4);
    bool sysml_value_1 = 0;
    sysml_value_1 = sysml_state_reportNow;
    int sysml_value_2 = 0;
    if (io_0) {
        sysml_conditional_5 = io_1 + 32400;
    }
    else {
        sysml_conditional_5 = io_1;
    }
    sysml_value_2 = sysml_conditional_5;
    *serial_0_trigger = sysml_value_1;
    *serial_0_value_0 = sysml_value_2;
}