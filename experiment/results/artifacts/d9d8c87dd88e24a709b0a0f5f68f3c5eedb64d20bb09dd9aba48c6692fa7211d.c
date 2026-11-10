#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_pageIndex = 0;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
double sysml_state_dhtTempLastValid = 0.0;
/* Global Variable define of  */
double sysml_state_dhtHumLastValid = 0.0;
/* Global Variable define of  */
int sysml_state_reportAccumMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_pageIndex = 0;
    sysml_state_previousPressed = false;
    sysml_state_dhtTempLastValid = 0.0;
    sysml_state_dhtHumLastValid = 0.0;
    sysml_state_reportAccumMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, double io_2, double io_3, bool io_4, int io_6, int* io_5, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, double* serial_0_value_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
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
    double sysml_conditional_14 = 0;
    double sysml_conditional_15 = 0;
    if (io_4) {
        sysml_state_dhtTempLastValid = io_2;
        sysml_state_dhtHumLastValid = io_3;
    }
    sysml_lazy_0 = io_1;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        if (sysml_state_pageIndex == 0) {
            sysml_conditional_1 = 1;
        }
        else {
            if (sysml_state_pageIndex == 1) {
                sysml_conditional_2 = 2;
            }
            else {
                sysml_conditional_2 = 0;
            }
            sysml_conditional_1 = sysml_conditional_2;
        }
        sysml_state_pageIndex = sysml_conditional_1;
    }
    sysml_state_previousPressed = io_1;
    sysml_state_reportAccumMs = (sysml_state_reportAccumMs + io_6);
    if ((sysml_state_reportAccumMs >= 2000)) {
        sysml_state_reportAccumMs = (sysml_state_reportAccumMs - 2000);
    }
    int sysml_value_0 = 0;
    if (sysml_state_pageIndex == 0) {
        if (io_0 >= 1000.0) {
            sysml_conditional_4 = 9999;
        }
        else {
            if (io_0 <= 0.0) {
                sysml_conditional_5 = 0;
            }
            else {
                if (io_0 < ((double)((int)io_0))) {
                    sysml_conditional_6 = 1;
                }
                else {
                    sysml_conditional_6 = 0;
                }
                sysml_conditional_5 = ((int)io_0) - sysml_conditional_6;
            }
            sysml_conditional_4 = sysml_conditional_5;
        }
        sysml_conditional_3 = sysml_conditional_4;
    }
    else {
        if (sysml_state_pageIndex == 1) {
            if (io_4) {
                if (io_2 < ((double)((int)io_2))) {
                    sysml_conditional_9 = 1;
                }
                else {
                    sysml_conditional_9 = 0;
                }
                sysml_conditional_8 = ((int)io_2) - sysml_conditional_9;
            }
            else {
                if (sysml_state_dhtTempLastValid < ((double)((int)sysml_state_dhtTempLastValid))) {
                    sysml_conditional_10 = 1;
                }
                else {
                    sysml_conditional_10 = 0;
                }
                sysml_conditional_8 = ((int)sysml_state_dhtTempLastValid) - sysml_conditional_10;
            }
            sysml_conditional_7 = sysml_conditional_8;
        }
        else {
            if (io_4) {
                if (io_3 < ((double)((int)io_3))) {
                    sysml_conditional_12 = 1;
                }
                else {
                    sysml_conditional_12 = 0;
                }
                sysml_conditional_11 = ((int)io_3) - sysml_conditional_12;
            }
            else {
                if (sysml_state_dhtHumLastValid < ((double)((int)sysml_state_dhtHumLastValid))) {
                    sysml_conditional_13 = 1;
                }
                else {
                    sysml_conditional_13 = 0;
                }
                sysml_conditional_11 = ((int)sysml_state_dhtHumLastValid) - sysml_conditional_13;
            }
            sysml_conditional_7 = sysml_conditional_11;
        }
        sysml_conditional_3 = sysml_conditional_7;
    }
    sysml_value_0 = sysml_conditional_3;
    bool sysml_value_1 = 0;
    sysml_value_1 = (sysml_state_reportAccumMs >= 2000);
    double sysml_value_2 = 0;
    sysml_value_2 = io_0;
    double sysml_value_3 = 0;
    if (io_4) {
        sysml_conditional_14 = io_2;
    }
    else {
        sysml_conditional_14 = sysml_state_dhtTempLastValid;
    }
    sysml_value_3 = sysml_conditional_14;
    double sysml_value_4 = 0;
    if (io_4) {
        sysml_conditional_15 = io_3;
    }
    else {
        sysml_conditional_15 = sysml_state_dhtHumLastValid;
    }
    sysml_value_4 = sysml_conditional_15;
    *io_5 = sysml_value_0;
    *serial_0_trigger = sysml_value_1;
    *serial_0_value_0 = sysml_value_2;
    *serial_0_value_1 = sysml_value_3;
    *serial_0_value_2 = sysml_value_4;
}