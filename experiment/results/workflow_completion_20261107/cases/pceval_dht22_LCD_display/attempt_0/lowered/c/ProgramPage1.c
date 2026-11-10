#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_loopWaitRemaining = 0;
/* Global Variable define of  */
double sysml_state_displayedHumidity = 0.0;
/* Global Variable define of  */
double sysml_state_displayedTemperature = 0.0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_loopWaitRemaining = 0;
    sysml_state_displayedHumidity = 0.0;
    sysml_state_displayedTemperature = 0.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, bool io_2, int io_5, double* io_3, double* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    if (io_5 >= sysml_state_loopWaitRemaining) {
        sysml_conditional_0 = 0;
    }
    else {
        sysml_conditional_0 = sysml_state_loopWaitRemaining - io_5;
    }
    sysml_state_loopWaitRemaining = sysml_conditional_0;
    if ((sysml_state_loopWaitRemaining == 0)) {
        sysml_lazy_1 = io_2;
        if (sysml_lazy_1) {
            sysml_lazy_2 = io_1 != sysml_state_displayedHumidity;
            if (!sysml_lazy_2) {
                sysml_lazy_2 = io_0 != sysml_state_displayedTemperature;
            }
            sysml_lazy_1 = sysml_lazy_2;
        }
        if (sysml_lazy_1) {
            sysml_state_displayedHumidity = io_1;
            sysml_state_displayedTemperature = io_0;
        }
        sysml_state_loopWaitRemaining = 100;
    }
    double sysml_value_0 = 0;
    sysml_value_0 = sysml_state_displayedHumidity;
    double sysml_value_1 = 0;
    sysml_value_1 = sysml_state_displayedTemperature;
    *io_3 = sysml_value_0;
    *io_4 = sysml_value_1;
}