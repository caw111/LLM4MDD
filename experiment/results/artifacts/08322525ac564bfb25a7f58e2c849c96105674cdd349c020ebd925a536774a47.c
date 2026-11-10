#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousAlarm = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousAlarm = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_3, bool io_4, bool* io_1, int* io_2, bool* serial_0_trigger, double* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    int sysml_conditional_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_lazy_10 = 0;
    double sysml_conditional_11 = 0;
    sysml_lazy_0 = io_0 < 500.0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_4;
    }
    sysml_lazy_1 = sysml_lazy_0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = io_3 <= 20.0;
    }
    sysml_state_previousAlarm = sysml_lazy_1;
    bool sysml_value_0 = 0;
    sysml_lazy_2 = io_0 < 500.0;
    if (sysml_lazy_2) {
        sysml_lazy_2 = io_4;
    }
    sysml_lazy_3 = sysml_lazy_2;
    if (sysml_lazy_3) {
        sysml_lazy_3 = io_3 <= 20.0;
    }
    sysml_value_0 = sysml_lazy_3;
    int sysml_value_1 = 0;
    sysml_lazy_4 = io_0 < 500.0;
    if (sysml_lazy_4) {
        sysml_lazy_4 = io_4;
    }
    sysml_lazy_5 = sysml_lazy_4;
    if (sysml_lazy_5) {
        sysml_lazy_5 = io_3 <= 20.0;
    }
    if (sysml_lazy_5) {
        sysml_conditional_6 = 2000;
    }
    else {
        sysml_conditional_6 = 0;
    }
    sysml_value_1 = sysml_conditional_6;
    bool sysml_value_2 = 0;
    sysml_lazy_7 = io_0 < 500.0;
    if (sysml_lazy_7) {
        sysml_lazy_7 = io_4;
    }
    sysml_lazy_8 = sysml_lazy_7;
    if (sysml_lazy_8) {
        sysml_lazy_8 = io_3 <= 20.0;
    }
    sysml_value_2 = sysml_lazy_8;
    double sysml_value_3 = 0;
    sysml_lazy_9 = io_0 < 500.0;
    if (sysml_lazy_9) {
        sysml_lazy_9 = io_4;
    }
    sysml_lazy_10 = sysml_lazy_9;
    if (sysml_lazy_10) {
        sysml_lazy_10 = io_3 <= 20.0;
    }
    if (sysml_lazy_10) {
        sysml_conditional_11 = io_3;
    }
    else {
        sysml_conditional_11 = 0.0;
    }
    sysml_value_3 = sysml_conditional_11;
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
    *serial_0_trigger = sysml_value_2;
    *serial_0_value_0 = sysml_value_3;
}