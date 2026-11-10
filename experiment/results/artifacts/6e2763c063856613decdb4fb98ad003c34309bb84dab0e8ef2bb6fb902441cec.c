#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_elapsedAccMs = 0;
/* Global Variable define of  */
double sysml_state_lastTemp = 0.0;
/* Global Variable define of  */
double sysml_state_lastHum = 0.0;
/* Global Variable define of  */
bool sysml_state_lastValid = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_elapsedAccMs = 0;
    sysml_state_lastTemp = 0.0;
    sysml_state_lastHum = 0.0;
    sysml_state_lastValid = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, bool io_2, int io_3, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, bool* serial_0_value_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    sysml_state_elapsedAccMs = (sysml_state_elapsedAccMs + io_3);
    if (io_2) {
        sysml_state_lastTemp = io_0;
        sysml_state_lastHum = io_1;
        sysml_state_lastValid = true;
    }
    sysml_lazy_0 = sysml_state_elapsedAccMs >= 5000;
    if (sysml_lazy_0) {
        sysml_lazy_0 = true;
    }
    if (sysml_lazy_0) {
        sysml_state_elapsedAccMs = (sysml_state_elapsedAccMs - 5000);
        if (io_2) {
            sysml_state_lastTemp = io_0;
            sysml_state_lastHum = io_1;
            sysml_state_lastValid = true;
        }
        else {
            sysml_state_lastValid = false;
        }
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = (sysml_state_elapsedAccMs >= 5000);
    double sysml_value_1 = 0;
    sysml_value_1 = sysml_state_lastTemp;
    double sysml_value_2 = 0;
    sysml_value_2 = sysml_state_lastHum;
    bool sysml_value_3 = 0;
    sysml_value_3 = sysml_state_lastValid;
    *serial_0_trigger = sysml_value_0;
    *serial_0_value_0 = sysml_value_1;
    *serial_0_value_1 = sysml_value_2;
    *serial_0_value_2 = sysml_value_3;
}