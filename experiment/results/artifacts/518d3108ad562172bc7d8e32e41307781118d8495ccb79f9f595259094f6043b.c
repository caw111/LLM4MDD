#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_targetPerMinute = 60;
/* Global Variable define of  */
int sysml_state_productCountTotal = 0;
/* Global Variable define of  */
int sysml_state_productCountActual = 0;
/* Global Variable define of  */
int sysml_state_productCountAtLastToggle = 0;
/* Global Variable define of  */
int sysml_state_stopCount = 0;
/* Global Variable define of  */
int sysml_state_missedInactivityMs = 0;
/* Global Variable define of  */
bool sysml_state_alarmLatched = false;
/* Global Variable define of  */
bool sysml_state_prevProductPulse = false;
/* Global Variable define of  */
bool sysml_state_prevStopButtonPressed = false;
/* Global Variable define of  */
int sysml_state_displayMode = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_targetPerMinute = 60;
    sysml_state_productCountTotal = 0;
    sysml_state_productCountActual = 0;
    sysml_state_productCountAtLastToggle = 0;
    sysml_state_stopCount = 0;
    sysml_state_missedInactivityMs = 0;
    sysml_state_alarmLatched = false;
    sysml_state_prevProductPulse = false;
    sysml_state_prevStopButtonPressed = false;
    sysml_state_displayMode = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_0, bool io_1, bool io_2, bool io_3, bool* io_4, int* io_5, int* io_6, bool* serial_0_trigger, int* serial_0_value_0, double* serial_0_value_1, int* serial_0_value_2, int* serial_0_value_3, bool* serial_0_value_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    int sysml_conditional_5 = 0;
    int sysml_conditional_6 = 0;
    int sysml_conditional_7 = 0;
    int sysml_conditional_8 = 0;
    double sysml_conditional_9 = 0;
    sysml_lazy_0 = io_3;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_prevStopButtonPressed;
    }
    if (sysml_lazy_0) {
        if (sysml_state_displayMode == 2) {
            sysml_conditional_1 = 0;
        }
        else {
            sysml_conditional_1 = sysml_state_displayMode + 1;
        }
        sysml_state_displayMode = sysml_conditional_1;
    }
    if (io_2) {
        sysml_state_missedInactivityMs = 0;
        if ((!sysml_state_alarmLatched)) {
            sysml_state_alarmLatched = false;
        }
    }
    if ((!io_2)) {
        sysml_state_missedInactivityMs = (sysml_state_missedInactivityMs + io_0);
        sysml_lazy_2 = !sysml_state_alarmLatched;
        if (sysml_lazy_2) {
            sysml_lazy_2 = sysml_state_missedInactivityMs >= 30000;
        }
        if (sysml_lazy_2) {
            sysml_state_alarmLatched = true;
            sysml_state_stopCount = (sysml_state_stopCount + 1);
        }
    }
    sysml_lazy_3 = io_1;
    if (sysml_lazy_3) {
        sysml_lazy_3 = !sysml_state_prevProductPulse;
    }
    if (sysml_lazy_3) {
        sysml_state_productCountTotal = (sysml_state_productCountTotal + 1);
        sysml_state_productCountActual = (sysml_state_productCountActual + 1);
        sysml_state_missedInactivityMs = 0;
        sysml_state_alarmLatched = false;
    }
    sysml_state_prevProductPulse = io_1;
    sysml_state_prevStopButtonPressed = io_2;
    sysml_lazy_4 = io_3;
    if (sysml_lazy_4) {
        sysml_lazy_4 = !sysml_state_prevStopButtonPressed;
    }
    if (sysml_lazy_4) {
        sysml_state_productCountAtLastToggle = sysml_state_productCountTotal;
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_alarmLatched;
    int sysml_value_1 = 0;
    if (sysml_state_alarmLatched) {
        sysml_conditional_5 = 2000;
    }
    else {
        sysml_conditional_5 = 0;
    }
    sysml_value_1 = sysml_conditional_5;
    int sysml_value_2 = 0;
    if (sysml_state_displayMode == 0) {
        sysml_conditional_6 = sysml_state_productCountTotal;
    }
    else {
        if (sysml_state_displayMode == 1) {
            if ((((double)(sysml_state_productCountActual * 100.0)) / sysml_state_targetPerMinute) < ((double)((int)(((double)(sysml_state_productCountActual * 100.0)) / sysml_state_targetPerMinute)))) {
                sysml_conditional_8 = 1;
            }
            else {
                sysml_conditional_8 = 0;
            }
            sysml_conditional_7 = ((int)(((double)(sysml_state_productCountActual * 100.0)) / sysml_state_targetPerMinute)) - sysml_conditional_8;
        }
        else {
            sysml_conditional_7 = sysml_state_stopCount;
        }
        sysml_conditional_6 = sysml_conditional_7;
    }
    sysml_value_2 = sysml_conditional_6;
    bool sysml_value_3 = 0;
    sysml_value_3 = sysml_state_alarmLatched;
    int sysml_value_4 = 0;
    sysml_value_4 = sysml_state_productCountTotal;
    double sysml_value_5 = 0;
    if (sysml_state_targetPerMinute == 0) {
        sysml_conditional_9 = 0.0;
    }
    else {
        sysml_conditional_9 = ((double)(sysml_state_productCountActual * 100.0)) / sysml_state_targetPerMinute;
    }
    sysml_value_5 = sysml_conditional_9;
    int sysml_value_6 = 0;
    sysml_value_6 = sysml_state_stopCount;
    int sysml_value_7 = 0;
    sysml_value_7 = sysml_state_productCountActual;
    bool sysml_value_8 = 0;
    sysml_value_8 = sysml_state_alarmLatched;
    *io_4 = sysml_value_0;
    *io_5 = sysml_value_1;
    *io_6 = sysml_value_2;
    *serial_0_trigger = sysml_value_3;
    *serial_0_value_0 = sysml_value_4;
    *serial_0_value_1 = sysml_value_5;
    *serial_0_value_2 = sysml_value_6;
    *serial_0_value_3 = sysml_value_7;
    *serial_0_value_4 = sysml_value_8;
}