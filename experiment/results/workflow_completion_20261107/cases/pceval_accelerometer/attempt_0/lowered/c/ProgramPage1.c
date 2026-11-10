#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_readySent = false;
/* Global Variable define of  */
bool sysml_state_readyPulse = false;
/* Global Variable define of  */
bool sysml_state_reportPulse = false;
/* Global Variable define of  */
bool sysml_state_waitingForDelay = false;
/* Global Variable define of  */
bool sysml_state_delayStartPending = false;
/* Global Variable define of  */
int sysml_state_delayAccumulated = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_readySent = false;
    sysml_state_readyPulse = false;
    sysml_state_reportPulse = false;
    sysml_state_waitingForDelay = false;
    sysml_state_delayStartPending = false;
    sysml_state_delayAccumulated = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, double io_2, bool io_3, int io_5, bool* io_4, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, double* serial_0_value_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    sysml_state_readyPulse = false;
    sysml_state_reportPulse = false;
    if ((!sysml_state_readySent)) {
        if (io_3) {
            sysml_state_readyPulse = true;
            sysml_state_readySent = true;
        }
    }
    else {
        if (sysml_state_waitingForDelay) {
            if (sysml_state_delayStartPending) {
                sysml_state_delayAccumulated = 0;
                sysml_state_delayStartPending = false;
            }
            else {
                if ((io_5 >= (500 - sysml_state_delayAccumulated))) {
                    sysml_state_delayAccumulated = 500;
                }
                else {
                    if ((io_5 > 0)) {
                        sysml_state_delayAccumulated = (sysml_state_delayAccumulated + io_5);
                    }
                }
            }
            if ((sysml_state_delayAccumulated >= 500)) {
                sysml_state_waitingForDelay = false;
            }
        }
        sysml_lazy_0 = !sysml_state_waitingForDelay;
        if (sysml_lazy_0) {
            sysml_lazy_0 = io_3;
        }
        if (sysml_lazy_0) {
            sysml_state_reportPulse = true;
            sysml_state_waitingForDelay = true;
            sysml_state_delayStartPending = true;
            sysml_state_delayAccumulated = 0;
        }
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_readyPulse;
    bool sysml_value_1 = 0;
    sysml_value_1 = sysml_state_reportPulse;
    double sysml_value_2 = 0;
    sysml_value_2 = io_0;
    double sysml_value_3 = 0;
    sysml_value_3 = io_1;
    double sysml_value_4 = 0;
    sysml_value_4 = io_2;
    *io_4 = sysml_value_0;
    *serial_0_trigger = sysml_value_1;
    *serial_0_value_0 = sysml_value_2;
    *serial_0_value_1 = sysml_value_3;
    *serial_0_value_2 = sysml_value_4;
}