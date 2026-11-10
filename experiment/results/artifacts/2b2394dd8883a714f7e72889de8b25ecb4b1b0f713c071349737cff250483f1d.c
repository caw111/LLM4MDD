#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_lastHumidity = 0.0;
/* Global Variable define of  */
bool sysml_state_initialized = false;
/* Global Variable define of  */
int sysml_state_nextReportDue = 0;
/* Global Variable define of  */
bool sysml_state_serialGate = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_lastHumidity = 0.0;
    sysml_state_initialized = false;
    sysml_state_nextReportDue = 0;
    sysml_state_serialGate = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, int io_2, bool* io_1, bool* serial_0_trigger, double* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    if ((!sysml_state_initialized)) {
        sysml_state_initialized = true;
        sysml_state_nextReportDue = 0;
        sysml_state_serialGate = false;
    }
    else {
        sysml_state_nextReportDue = (sysml_state_nextReportDue + io_2);
        if ((sysml_state_nextReportDue >= 2000)) {
            sysml_state_nextReportDue = (sysml_state_nextReportDue - 2000);
            sysml_state_serialGate = true;
        }
        else {
            sysml_state_serialGate = false;
        }
        if ((!(!sysml_state_initialized))) {
        }
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_serialGate;
    bool sysml_value_1 = 0;
    sysml_value_1 = sysml_state_serialGate;
    double sysml_value_2 = 0;
    sysml_value_2 = sysml_state_lastHumidity;
    *io_1 = sysml_value_0;
    *serial_0_trigger = sysml_value_1;
    *serial_0_value_0 = sysml_value_2;
}