#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_reportedDays = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_reportedDays = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_0, bool io_1, int io_2, bool io_2_valid, bool* serial_0_trigger, int* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    sysml_lazy_0 = io_1;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_2_valid;
    }
    if (sysml_lazy_0) {
        if (((io_2 * 1.0) - io_0) >= 0.0) {
            if ((((double)((io_2 * 1.0) - io_0)) / 86400.0) < ((double)((int)(((double)((io_2 * 1.0) - io_0)) / 86400.0)))) {
                sysml_conditional_2 = 1;
            }
            else {
                sysml_conditional_2 = 0;
            }
            sysml_conditional_1 = ((int)(((double)((io_2 * 1.0) - io_0)) / 86400.0)) - sysml_conditional_2;
        }
        else {
            if ((((double)(-((io_2 * 1.0) - io_0))) / 86400.0) < ((double)((int)(((double)(-((io_2 * 1.0) - io_0))) / 86400.0)))) {
                sysml_conditional_3 = 1;
            }
            else {
                sysml_conditional_3 = 0;
            }
            sysml_conditional_1 = -(((int)(((double)(-((io_2 * 1.0) - io_0))) / 86400.0)) - sysml_conditional_3);
        }
        sysml_state_reportedDays = sysml_conditional_1;
    }
    double sysml_value_0 = 0;
    sysml_value_0 = ((io_2 * 1.0) - io_0);
    bool sysml_value_1 = 0;
    sysml_lazy_4 = io_1;
    if (sysml_lazy_4) {
        sysml_lazy_4 = io_2_valid;
    }
    sysml_value_1 = sysml_lazy_4;
    int sysml_value_2 = 0;
    sysml_value_2 = sysml_state_reportedDays;
    *serial_0_trigger = sysml_value_1;
    *serial_0_value_0 = sysml_value_2;
}