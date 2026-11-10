#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
int sysml_state_pressedStartStep = 0;
/* Global Variable define of  */
int sysml_state_accumulatedPressedMs = 0;
/* Global Variable define of  */
bool sysml_state_reportPending = false;
/* Global Variable define of  */
double sysml_state_reportedDurationSeconds = 0.0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousPressed = false;
    sysml_state_pressedStartStep = 0;
    sysml_state_accumulatedPressedMs = 0;
    sysml_state_reportPending = false;
    sysml_state_reportedDurationSeconds = 0.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, int io_1, bool io_1_valid, bool* serial_0_trigger, double* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        sysml_state_pressedStartStep = 0;
        sysml_state_accumulatedPressedMs = 0;
        sysml_state_reportPending = false;
    }
    if (io_0) {
        sysml_state_accumulatedPressedMs = (sysml_state_accumulatedPressedMs + 5);
    }
    sysml_lazy_1 = !io_0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = sysml_state_previousPressed;
    }
    if (sysml_lazy_1) {
        sysml_state_reportedDurationSeconds = ((double)(sysml_state_accumulatedPressedMs) / 1000.0);
        sysml_state_reportPending = true;
        sysml_state_accumulatedPressedMs = 0;
    }
    if (sysml_state_reportPending) {
        sysml_state_reportPending = false;
    }
    sysml_state_previousPressed = io_0;
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_reportPending;
    double sysml_value_1 = 0;
    sysml_value_1 = sysml_state_reportedDurationSeconds;
    *serial_0_trigger = sysml_value_0;
    *serial_0_value_0 = sysml_value_1;
}