#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_armed = false;
/* Global Variable define of  */
bool sysml_state_nightAuto = false;
/* Global Variable define of  */
int sysml_state_eventCount = 0;
/* Global Variable define of  */
bool sysml_state_previousDoorOpened = false;
/* Global Variable define of  */
bool sysml_state_previousPIR = false;
/* Global Variable define of  */
bool sysml_state_serialCommandActive = false;
/* Global Variable define of  */
int sysml_state_pendingReportCmd = 0;
/* Global Variable define of  */
bool sysml_state_doorRising = false;
/* Global Variable define of  */
bool sysml_state_pirRising = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_armed = false;
    sysml_state_nightAuto = false;
    sysml_state_eventCount = 0;
    sysml_state_previousDoorOpened = false;
    sysml_state_previousPIR = false;
    sysml_state_serialCommandActive = false;
    sysml_state_pendingReportCmd = 0;
    sysml_state_doorRising = false;
    sysml_state_pirRising = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double io_2, int* io_3, bool* io_4, int* io_5, bool* serial_0_trigger, int* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    int sysml_conditional_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_lazy_10 = 0;
    bool sysml_lazy_11 = 0;
    bool sysml_conditional_12 = 0;
    int sysml_conditional_13 = 0;
    if ((sysml_state_pendingReportCmd == 1)) {
        sysml_state_armed = true;
    }
    if ((sysml_state_pendingReportCmd == 2)) {
        sysml_state_armed = false;
    }
    if ((sysml_state_pendingReportCmd == 3)) {
        sysml_state_armed = false;
        sysml_state_eventCount = 0;
        sysml_state_nightAuto = false;
    }
    if ((sysml_state_pendingReportCmd == 4)) {
        sysml_state_serialCommandActive = true;
    }
    if ((sysml_state_serialCommandActive == true)) {
        sysml_state_serialCommandActive = false;
    }
    sysml_lazy_0 = io_1;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousDoorOpened;
    }
    sysml_state_doorRising = sysml_lazy_0;
    sysml_lazy_1 = io_0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_previousPIR;
    }
    sysml_state_pirRising = sysml_lazy_1;
    sysml_state_nightAuto = (io_2 < 512.0);
    sysml_lazy_2 = sysml_state_armed;
    if (!sysml_lazy_2) {
        sysml_lazy_2 = sysml_state_nightAuto;
    }
    sysml_lazy_3 = sysml_lazy_2;
    if (sysml_lazy_3) {
        sysml_lazy_3 = sysml_state_doorRising;
    }
    sysml_lazy_4 = sysml_lazy_3;
    if (sysml_lazy_4) {
        sysml_lazy_4 = sysml_state_pirRising;
    }
    if (sysml_lazy_4) {
        sysml_state_eventCount = (sysml_state_eventCount + 1);
    }
    sysml_state_previousDoorOpened = io_1;
    sysml_state_previousPIR = io_0;
    int sysml_value_0 = 0;
    sysml_lazy_5 = sysml_state_armed;
    if (!sysml_lazy_5) {
        sysml_lazy_5 = sysml_state_nightAuto;
    }
    sysml_lazy_6 = sysml_lazy_5;
    if (sysml_lazy_6) {
        sysml_lazy_6 = sysml_state_doorRising;
    }
    sysml_lazy_7 = sysml_lazy_6;
    if (sysml_lazy_7) {
        sysml_lazy_7 = sysml_state_pirRising;
    }
    if (sysml_lazy_7) {
        sysml_conditional_8 = 2000;
    }
    else {
        sysml_conditional_8 = 0;
    }
    sysml_value_0 = sysml_conditional_8;
    bool sysml_value_1 = 0;
    sysml_lazy_9 = sysml_state_armed;
    if (!sysml_lazy_9) {
        sysml_lazy_9 = sysml_state_nightAuto;
    }
    sysml_lazy_10 = sysml_lazy_9;
    if (sysml_lazy_10) {
        sysml_lazy_10 = sysml_state_doorRising;
    }
    sysml_lazy_11 = sysml_lazy_10;
    if (sysml_lazy_11) {
        sysml_lazy_11 = sysml_state_pirRising;
    }
    sysml_value_1 = sysml_lazy_11;
    int sysml_value_2 = 0;
    sysml_value_2 = sysml_state_eventCount;
    bool sysml_value_3 = 0;
    if (sysml_state_pendingReportCmd == 4) {
        sysml_conditional_12 = true;
    }
    else {
        sysml_conditional_12 = false;
    }
    sysml_value_3 = sysml_conditional_12;
    int sysml_value_4 = 0;
    if (sysml_state_pendingReportCmd == 4) {
        sysml_conditional_13 = sysml_state_eventCount;
    }
    else {
        sysml_conditional_13 = 0;
    }
    sysml_value_4 = sysml_conditional_13;
    *io_3 = sysml_value_0;
    *io_4 = sysml_value_1;
    *io_5 = sysml_value_2;
    *serial_0_trigger = sysml_value_3;
    *serial_0_value_0 = sysml_value_4;
}