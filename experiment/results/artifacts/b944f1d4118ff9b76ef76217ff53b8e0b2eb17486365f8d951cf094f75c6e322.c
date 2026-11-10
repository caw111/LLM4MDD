#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_recyclableCount = 0;
/* Global Variable define of  */
int sysml_state_otherCount = 0;
/* Global Variable define of  */
int sysml_state_totalCount = 0;
/* Global Variable define of  */
bool sysml_state_fullBinAlarmed = false;
/* Global Variable define of  */
bool sysml_state_previousResetPressed = false;
/* Global Variable define of  */
double sysml_state_fillHeightCmLatched = 0.0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_recyclableCount = 0;
    sysml_state_otherCount = 0;
    sysml_state_totalCount = 0;
    sysml_state_fullBinAlarmed = false;
    sysml_state_previousResetPressed = false;
    sysml_state_fillHeightCmLatched = 0.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double io_2, bool io_3, bool io_7, int* io_4, int* io_5, bool* io_6, bool* serial_0_trigger, int* serial_0_value_0, double* serial_0_value_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    if (io_3) {
        sysml_state_fillHeightCmLatched = io_2;
    }
    sysml_lazy_0 = sysml_state_fillHeightCmLatched >= 20.0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_fullBinAlarmed;
    }
    if (sysml_lazy_0) {
        sysml_state_fullBinAlarmed = true;
    }
    sysml_lazy_1 = !sysml_state_fullBinAlarmed;
    if (sysml_lazy_1) {
        sysml_lazy_1 = io_0;
    }
    if (sysml_lazy_1) {
        sysml_state_recyclableCount = (sysml_state_recyclableCount + 1);
    }
    sysml_lazy_2 = !sysml_state_fullBinAlarmed;
    if (sysml_lazy_2) {
        sysml_lazy_2 = io_1;
    }
    if (sysml_lazy_2) {
        sysml_state_otherCount = (sysml_state_otherCount + 1);
    }
    sysml_state_totalCount = (sysml_state_recyclableCount + sysml_state_otherCount);
    sysml_lazy_3 = io_7;
    if (sysml_lazy_3) {
        sysml_lazy_3 = !sysml_state_previousResetPressed;
    }
    if (sysml_lazy_3) {
        sysml_state_recyclableCount = 0;
        sysml_state_otherCount = 0;
        sysml_state_totalCount = 0;
        sysml_state_fullBinAlarmed = false;
    }
    sysml_state_previousResetPressed = io_7;
    int sysml_value_0 = 0;
    sysml_value_0 = sysml_state_totalCount;
    int sysml_value_1 = 0;
    if (sysml_state_fullBinAlarmed) {
        sysml_conditional_4 = 1000;
    }
    else {
        sysml_conditional_4 = 0;
    }
    sysml_value_1 = sysml_conditional_4;
    bool sysml_value_2 = 0;
    sysml_value_2 = sysml_state_fullBinAlarmed;
    bool sysml_value_3 = 0;
    sysml_value_3 = sysml_state_fullBinAlarmed;
    int sysml_value_4 = 0;
    if (sysml_state_fullBinAlarmed) {
        sysml_conditional_5 = 1;
    }
    else {
        sysml_conditional_5 = 0;
    }
    sysml_value_4 = sysml_conditional_5;
    double sysml_value_5 = 0;
    sysml_value_5 = sysml_state_fillHeightCmLatched;
    *io_4 = sysml_value_0;
    *io_5 = sysml_value_1;
    *io_6 = sysml_value_2;
    *serial_0_trigger = sysml_value_3;
    *serial_0_value_0 = sysml_value_4;
    *serial_0_value_1 = sysml_value_5;
}