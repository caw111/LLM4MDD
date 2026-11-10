#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_reportParity = false;
/* Global Variable define of  */
bool sysml_state_pendingReport = true;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_reportParity = false;
    sysml_state_pendingReport = true;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool* io_1, bool* serial_0_trigger, double* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_conditional_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    if ((!sysml_state_pendingReport)) {
    }
    else {
        if (sysml_state_reportParity) {
            sysml_conditional_0 = false;
        }
        else {
            sysml_conditional_0 = true;
        }
        sysml_state_pendingReport = sysml_conditional_0;
        sysml_state_reportParity = (!sysml_state_reportParity);
    }
    sysml_lazy_1 = sysml_state_pendingReport;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_reportParity;
    }
    if (sysml_lazy_1) {
    }
    bool sysml_value_0 = 0;
    sysml_lazy_2 = sysml_state_pendingReport;
    if (sysml_lazy_2) {
        sysml_lazy_2 = sysml_state_reportParity;
    }
    sysml_value_0 = sysml_lazy_2;
    bool sysml_value_1 = 0;
    sysml_lazy_3 = sysml_state_pendingReport;
    if (sysml_lazy_3) {
        sysml_lazy_3 = sysml_state_reportParity;
    }
    sysml_value_1 = sysml_lazy_3;
    double sysml_value_2 = 0;
    sysml_value_2 = io_0;
    *io_1 = sysml_value_0;
    *serial_0_trigger = sysml_value_1;
    *serial_0_value_0 = sysml_value_2;
}