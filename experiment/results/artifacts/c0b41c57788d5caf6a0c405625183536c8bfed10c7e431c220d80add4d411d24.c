#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_elapsedAccumMs = 0;
/* Global Variable define of  */
double sysml_state_lightThreshold = 0.0;
/* Global Variable define of  */
double sysml_state_tempThreshold = 0.0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_elapsedAccumMs = 0;
    sysml_state_lightThreshold = 0.0;
    sysml_state_tempThreshold = 0.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, int io_3, bool* io_2, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, bool* serial_1_trigger, int* serial_1_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    sysml_state_elapsedAccumMs = (sysml_state_elapsedAccumMs + io_3);
    if ((sysml_state_elapsedAccumMs >= 1000)) {
        sysml_state_elapsedAccumMs = (sysml_state_elapsedAccumMs - 1000);
    }
    bool sysml_value_0 = 0;
    sysml_lazy_0 = io_0 > sysml_state_lightThreshold;
    if (!sysml_lazy_0) {
        sysml_lazy_0 = io_1 > sysml_state_tempThreshold;
    }
    sysml_value_0 = sysml_lazy_0;
    bool sysml_value_1 = 0;
    sysml_lazy_1 = sysml_state_elapsedAccumMs >= 0;
    if (sysml_lazy_1) {
        sysml_lazy_2 = io_0 > sysml_state_lightThreshold;
        if (!sysml_lazy_2) {
            sysml_lazy_2 = io_1 > sysml_state_tempThreshold;
        }
        sysml_lazy_3 = sysml_lazy_2;
        if (!sysml_lazy_3) {
            sysml_lazy_3 = true;
        }
        sysml_lazy_1 = sysml_lazy_3;
    }
    sysml_value_1 = sysml_lazy_1;
    double sysml_value_2 = 0;
    sysml_value_2 = io_0;
    double sysml_value_3 = 0;
    sysml_value_3 = io_1;
    bool sysml_value_4 = 0;
    sysml_lazy_4 = io_0 > sysml_state_lightThreshold;
    if (!sysml_lazy_4) {
        sysml_lazy_4 = io_1 > sysml_state_tempThreshold;
    }
    sysml_value_4 = sysml_lazy_4;
    int sysml_value_5 = 0;
    sysml_value_5 = 1;
    *io_2 = sysml_value_0;
    *serial_0_trigger = sysml_value_1;
    *serial_0_value_0 = sysml_value_2;
    *serial_0_value_1 = sysml_value_3;
    *serial_1_trigger = sysml_value_4;
    *serial_1_value_0 = sysml_value_5;
}