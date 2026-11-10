#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousValid = false;
/* Global Variable define of  */
int sysml_state_lastReportedEpoch = 0;
/* Global Variable define of  */
int sysml_state_lastReportedOffset = 0;
/* Global Variable define of  */
bool sysml_state_reportReady = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousValid = false;
    sysml_state_lastReportedEpoch = 0;
    sysml_state_lastReportedOffset = 0;
    sysml_state_reportReady = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_0, bool io_1, bool io_2, bool io_3, bool io_4, bool io_5, bool* serial_0_trigger, double* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    int sysml_conditional_6 = 0;
    int sysml_conditional_7 = 0;
    int sysml_conditional_8 = 0;
    int sysml_conditional_9 = 0;
    int sysml_conditional_10 = 0;
    int sysml_conditional_11 = 0;
    int sysml_conditional_12 = 0;
    int sysml_conditional_13 = 0;
    int sysml_conditional_14 = 0;
    sysml_lazy_0 = io_1;
    if (sysml_lazy_0) {
        sysml_lazy_1 = !sysml_state_previousValid;
        if (!sysml_lazy_1) {
            sysml_lazy_1 = io_0 != sysml_state_lastReportedEpoch;
        }
        sysml_lazy_2 = sysml_lazy_1;
        if (!sysml_lazy_2) {
            if (io_2) {
                sysml_conditional_3 = 32400;
            }
            else {
                if (io_3) {
                    sysml_conditional_4 = 14400;
                }
                else {
                    if (io_4) {
                        sysml_conditional_5 = -36000;
                    }
                    else {
                        if (io_5) {
                            sysml_conditional_6 = -18000;
                        }
                        else {
                            sysml_conditional_6 = 0;
                        }
                        sysml_conditional_5 = sysml_conditional_6;
                    }
                    sysml_conditional_4 = sysml_conditional_5;
                }
                sysml_conditional_3 = sysml_conditional_4;
            }
            sysml_lazy_2 = sysml_conditional_3 != sysml_state_lastReportedOffset;
        }
        sysml_lazy_0 = sysml_lazy_2;
    }
    sysml_state_reportReady = sysml_lazy_0;
    if (sysml_state_reportReady) {
        sysml_state_lastReportedEpoch = io_0;
        if (io_2) {
            sysml_conditional_7 = 32400;
        }
        else {
            if (io_3) {
                sysml_conditional_8 = 14400;
            }
            else {
                if (io_4) {
                    sysml_conditional_9 = -36000;
                }
                else {
                    if (io_5) {
                        sysml_conditional_10 = -18000;
                    }
                    else {
                        sysml_conditional_10 = 0;
                    }
                    sysml_conditional_9 = sysml_conditional_10;
                }
                sysml_conditional_8 = sysml_conditional_9;
            }
            sysml_conditional_7 = sysml_conditional_8;
        }
        sysml_state_lastReportedOffset = sysml_conditional_7;
    }
    sysml_state_previousValid = io_1;
    int sysml_value_0 = 0;
    if (io_2) {
        sysml_conditional_11 = 32400;
    }
    else {
        if (io_3) {
            sysml_conditional_12 = 14400;
        }
        else {
            if (io_4) {
                sysml_conditional_13 = -36000;
            }
            else {
                if (io_5) {
                    sysml_conditional_14 = -18000;
                }
                else {
                    sysml_conditional_14 = 0;
                }
                sysml_conditional_13 = sysml_conditional_14;
            }
            sysml_conditional_12 = sysml_conditional_13;
        }
        sysml_conditional_11 = sysml_conditional_12;
    }
    sysml_value_0 = sysml_conditional_11;
    double sysml_value_1 = 0;
    sysml_value_1 = (io_0 * 1.0);
    bool sysml_value_2 = 0;
    sysml_value_2 = sysml_state_reportReady;
    double sysml_value_3 = 0;
    sysml_value_3 = (sysml_value_1 + sysml_value_0);
    *serial_0_trigger = sysml_value_2;
    *serial_0_value_0 = sysml_value_3;
}