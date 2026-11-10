#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_accum3sMs = 0;
/* Global Variable define of  */
int sysml_state_consecCloseCount = 0;
/* Global Variable define of  */
double sysml_state_prevDhtTemp = 0.0;
/* Global Variable define of  */
double sysml_state_prevDhtHum = 0.0;
/* Global Variable define of  */
double sysml_state_prevDistCm = 0.0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_accum3sMs = 0;
    sysml_state_consecCloseCount = 0;
    sysml_state_prevDhtTemp = 0.0;
    sysml_state_prevDhtHum = 0.0;
    sysml_state_prevDistCm = 0.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_1, double io_2, bool io_3, double io_4, bool io_5, int io_6, int io_7, bool io_7_valid, bool* io_0, bool* io_8, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, double* serial_0_value_2, bool* serial_0_value_3, bool* serial_0_value_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    double sysml_conditional_5 = 0;
    double sysml_conditional_6 = 0;
    double sysml_conditional_7 = 0;
    sysml_state_accum3sMs = (sysml_state_accum3sMs + io_6);
    if ((sysml_state_accum3sMs >= 3000)) {
        sysml_state_accum3sMs = (sysml_state_accum3sMs - 3000);
    }
    if (io_5) {
        if ((io_4 < 20.0)) {
            sysml_state_consecCloseCount = (sysml_state_consecCloseCount + 1);
        }
        else {
            sysml_state_consecCloseCount = 0;
        }
    }
    else {
        sysml_state_consecCloseCount = 0;
    }
    if (io_3) {
        sysml_state_prevDhtTemp = io_1;
        sysml_state_prevDhtHum = io_2;
    }
    if (io_5) {
        sysml_state_prevDistCm = io_4;
    }
    if ((sysml_state_consecCloseCount > 3)) {
        sysml_state_consecCloseCount = 3;
    }
    bool sysml_value_0 = 0;
    sysml_lazy_0 = sysml_state_consecCloseCount >= 3;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_5;
    }
    sysml_value_0 = sysml_lazy_0;
    bool sysml_value_1 = 0;
    sysml_lazy_1 = sysml_state_consecCloseCount >= 3;
    if (sysml_lazy_1) {
        sysml_lazy_1 = io_5;
    }
    sysml_lazy_2 = sysml_lazy_1;
    if (sysml_lazy_2) {
        sysml_lazy_2 = sysml_state_accum3sMs >= 0;
    }
    sysml_value_1 = sysml_lazy_2;
    bool sysml_value_2 = 0;
    sysml_lazy_3 = sysml_state_consecCloseCount >= 0;
    if (sysml_lazy_3) {
        sysml_lazy_4 = sysml_state_accum3sMs >= 3000;
        if (!sysml_lazy_4) {
            sysml_lazy_4 = (sysml_state_accum3sMs + io_6) >= 3000;
        }
        sysml_lazy_3 = sysml_lazy_4;
    }
    sysml_value_2 = sysml_lazy_3;
    double sysml_value_3 = 0;
    if (io_3) {
        sysml_conditional_5 = io_1;
    }
    else {
        sysml_conditional_5 = sysml_state_prevDhtTemp;
    }
    sysml_value_3 = sysml_conditional_5;
    double sysml_value_4 = 0;
    if (io_3) {
        sysml_conditional_6 = io_2;
    }
    else {
        sysml_conditional_6 = sysml_state_prevDhtHum;
    }
    sysml_value_4 = sysml_conditional_6;
    double sysml_value_5 = 0;
    if (io_5) {
        sysml_conditional_7 = io_4;
    }
    else {
        sysml_conditional_7 = sysml_state_prevDistCm;
    }
    sysml_value_5 = sysml_conditional_7;
    bool sysml_value_6 = 0;
    sysml_value_6 = io_5;
    bool sysml_value_7 = 0;
    sysml_value_7 = io_3;
    *io_0 = sysml_value_0;
    *io_8 = sysml_value_1;
    *serial_0_trigger = sysml_value_2;
    *serial_0_value_0 = sysml_value_3;
    *serial_0_value_1 = sysml_value_4;
    *serial_0_value_2 = sysml_value_5;
    *serial_0_value_3 = sysml_value_6;
    *serial_0_value_4 = sysml_value_7;
}