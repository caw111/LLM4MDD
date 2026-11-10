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
double sysml_state_reportPhase = 0.0;
/* Global Variable define of  */
double sysml_state_cachedX = 0.0;
/* Global Variable define of  */
double sysml_state_cachedY = 0.0;
/* Global Variable define of  */
double sysml_state_cachedZ = 0.0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_readySent = false;
    sysml_state_readyPulse = false;
    sysml_state_reportPulse = false;
    sysml_state_reportPhase = 0.0;
    sysml_state_cachedX = 0.0;
    sysml_state_cachedY = 0.0;
    sysml_state_cachedZ = 0.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, double io_2, bool io_3, int io_5, bool* io_4, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, double* serial_0_value_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    int sysml_arithmetic_1 = 0;
    sysml_state_readyPulse = false;
    sysml_state_reportPulse = false;
    if (io_3) {
        sysml_state_cachedX = io_0;
        sysml_state_cachedY = io_1;
        sysml_state_cachedZ = io_2;
    }
    if (sysml_state_readySent) {
        sysml_state_reportPhase = (sysml_state_reportPhase + io_5);
        if ((sysml_state_reportPhase >= 500.0)) {
            sysml_state_reportPulse = io_3;
            if ((((double)sysml_state_reportPhase) / 500.0) < ((double)((int)(((double)sysml_state_reportPhase) / 500.0)))) {
                sysml_conditional_0 = 1;
            }
            else {
                sysml_conditional_0 = 0;
            }
            sysml_arithmetic_1 = ((int)(((double)sysml_state_reportPhase) / 500.0)) - sysml_conditional_0;
            sysml_state_reportPhase = sysml_state_reportPhase - (500.0 * sysml_arithmetic_1);
        }
    }
    else {
        if (io_3) {
            sysml_state_readyPulse = true;
            sysml_state_readySent = true;
            sysml_state_reportPhase = 0.0;
        }
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_readyPulse;
    bool sysml_value_1 = 0;
    sysml_value_1 = sysml_state_reportPulse;
    double sysml_value_2 = 0;
    sysml_value_2 = sysml_state_cachedX;
    double sysml_value_3 = 0;
    sysml_value_3 = sysml_state_cachedY;
    double sysml_value_4 = 0;
    sysml_value_4 = sysml_state_cachedZ;
    *io_4 = sysml_value_0;
    *serial_0_trigger = sysml_value_1;
    *serial_0_value_0 = sysml_value_2;
    *serial_0_value_1 = sysml_value_3;
    *serial_0_value_2 = sysml_value_4;
}