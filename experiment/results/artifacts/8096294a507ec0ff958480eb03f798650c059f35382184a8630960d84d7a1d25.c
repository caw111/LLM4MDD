#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousAnyPIR = false;
/* Global Variable define of  */
bool sysml_state_isLighting = false;
/* Global Variable define of  */
int sysml_state_todayCount = 0;
/* Global Variable define of  */
int sysml_state_illuminatedOnTimeMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousAnyPIR = false;
    sysml_state_isLighting = false;
    sysml_state_todayCount = 0;
    sysml_state_illuminatedOnTimeMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double io_2, double* io_3, int* io_4, double* io_5, bool* serial_0_trigger, int* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    double sysml_conditional_5 = 0;
    double sysml_conditional_6 = 0;
    bool sysml_conditional_7 = 0;
    sysml_lazy_0 = io_2 < 300.0;
    if (sysml_lazy_0) {
        sysml_lazy_1 = io_0;
        if (!sysml_lazy_1) {
            sysml_lazy_1 = io_1;
        }
        sysml_lazy_0 = sysml_lazy_1;
    }
    sysml_state_isLighting = sysml_lazy_0;
    sysml_lazy_2 = io_0;
    if (!sysml_lazy_2) {
        sysml_lazy_2 = io_1;
    }
    sysml_lazy_3 = sysml_lazy_2;
    if (sysml_lazy_3) {
        sysml_lazy_3 = !sysml_state_previousAnyPIR;
    }
    if (sysml_lazy_3) {
        sysml_state_todayCount = (sysml_state_todayCount + 1);
    }
    if (sysml_state_isLighting) {
        sysml_state_illuminatedOnTimeMs = (sysml_state_illuminatedOnTimeMs + 10);
    }
    sysml_lazy_4 = io_0;
    if (!sysml_lazy_4) {
        sysml_lazy_4 = io_1;
    }
    sysml_state_previousAnyPIR = sysml_lazy_4;
    double sysml_value_0 = 0;
    if (sysml_state_isLighting) {
        sysml_conditional_5 = 255.0;
    }
    else {
        sysml_conditional_5 = 0.0;
    }
    sysml_value_0 = sysml_conditional_5;
    int sysml_value_1 = 0;
    sysml_value_1 = sysml_state_todayCount;
    double sysml_value_2 = 0;
    if (sysml_state_isLighting) {
        sysml_conditional_6 = 255.0;
    }
    else {
        sysml_conditional_6 = 0.0;
    }
    sysml_value_2 = sysml_conditional_6;
    bool sysml_value_3 = 0;
    if (sysml_state_isLighting) {
        sysml_conditional_7 = true;
    }
    else {
        sysml_conditional_7 = false;
    }
    sysml_value_3 = sysml_conditional_7;
    int sysml_value_4 = 0;
    sysml_value_4 = sysml_state_illuminatedOnTimeMs;
    *io_3 = sysml_value_0;
    *io_4 = sysml_value_1;
    *io_5 = sysml_value_2;
    *serial_0_trigger = sysml_value_3;
    *serial_0_value_0 = sysml_value_4;
}