#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_serialReportArmed = true;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_serialReportArmed = true;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool* serial_0_trigger, double* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    sysml_lazy_0 = !sysml_state_serialReportArmed;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_0 >= 0.0;
    }
    if (sysml_lazy_0) {
        sysml_state_serialReportArmed = true;
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_serialReportArmed;
    double sysml_value_1 = 0;
    sysml_value_1 = io_0;
    *serial_0_trigger = sysml_value_0;
    *serial_0_value_0 = sysml_value_1;
}