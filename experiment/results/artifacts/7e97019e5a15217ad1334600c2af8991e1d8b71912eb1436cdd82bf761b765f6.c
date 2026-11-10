#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_stageId = 1;
/* Global Variable define of  */
int sysml_state_remainingSeconds = 0;
/* Global Variable define of  */
double sysml_state_targetDegC = 0.0;
/* Global Variable define of  */
bool sysml_state_prevButtonD2 = false;
/* Global Variable define of  */
bool sysml_state_prevButtonD3 = false;
/* Global Variable define of  */
bool sysml_state_prevButtonD4 = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_stageId = 1;
    sysml_state_remainingSeconds = 0;
    sysml_state_targetDegC = 0.0;
    sysml_state_prevButtonD2 = false;
    sysml_state_prevButtonD3 = false;
    sysml_state_prevButtonD4 = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, bool io_2, bool io_3, bool* io_4, int* io_5, int* io_6, bool* serial_0_trigger, int* serial_0_value_0, int* serial_0_value_1, double* serial_0_value_2, int* serial_0_value_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    int sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    int sysml_conditional_5 = 0;
    int sysml_conditional_6 = 0;
    int sysml_conditional_7 = 0;
    int sysml_conditional_8 = 0;
    bool sysml_conditional_9 = 0;
    int sysml_conditional_10 = 0;
    int sysml_conditional_11 = 0;
    bool sysml_conditional_12 = 0;
    sysml_lazy_0 = io_1;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_prevButtonD2;
    }
    if (sysml_lazy_0) {
        sysml_state_stageId = 1;
        sysml_state_remainingSeconds = 0;
        sysml_state_targetDegC = 0.0;
    }
    sysml_lazy_1 = io_2;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_prevButtonD3;
    }
    if (sysml_lazy_1) {
        if (sysml_state_stageId == 1) {
            sysml_conditional_2 = 2;
        }
        else {
            if (sysml_state_stageId == 2) {
                sysml_conditional_3 = 3;
            }
            else {
                sysml_conditional_3 = 1;
            }
            sysml_conditional_2 = sysml_conditional_3;
        }
        sysml_state_stageId = sysml_conditional_2;
        sysml_state_remainingSeconds = 0;
        sysml_state_targetDegC = 0.0;
    }
    sysml_lazy_4 = io_3;
    if (sysml_lazy_4) {
        sysml_lazy_4 = !sysml_state_prevButtonD4;
    }
    if (sysml_lazy_4) {
        if (sysml_state_stageId == 1) {
            sysml_conditional_5 = 3;
        }
        else {
            if (sysml_state_stageId == 3) {
                sysml_conditional_6 = 2;
            }
            else {
                sysml_conditional_6 = 1;
            }
            sysml_conditional_5 = sysml_conditional_6;
        }
        sysml_state_stageId = sysml_conditional_5;
        sysml_state_remainingSeconds = 0;
        sysml_state_targetDegC = 0.0;
    }
    if ((io_0 >= sysml_state_targetDegC)) {
        if (sysml_state_stageId == 1) {
            sysml_conditional_7 = 2;
        }
        else {
            if (sysml_state_stageId == 2) {
                sysml_conditional_8 = 3;
            }
            else {
                sysml_conditional_8 = 1;
            }
            sysml_conditional_7 = sysml_conditional_8;
        }
        sysml_state_stageId = sysml_conditional_7;
        sysml_state_remainingSeconds = 0;
    }
    sysml_state_prevButtonD2 = io_1;
    sysml_state_prevButtonD3 = io_2;
    sysml_state_prevButtonD4 = io_3;
    bool sysml_value_0 = 0;
    if (sysml_state_stageId != 0) {
        sysml_conditional_9 = true;
    }
    else {
        sysml_conditional_9 = false;
    }
    sysml_value_0 = sysml_conditional_9;
    int sysml_value_1 = 0;
    if (io_0 >= sysml_state_targetDegC) {
        sysml_conditional_10 = 440;
    }
    else {
        sysml_conditional_10 = 0;
    }
    sysml_value_1 = sysml_conditional_10;
    int sysml_value_2 = 0;
    if (sysml_state_stageId >= 1) {
        sysml_conditional_11 = sysml_state_stageId;
    }
    else {
        sysml_conditional_11 = 0;
    }
    sysml_value_2 = sysml_conditional_11;
    bool sysml_value_3 = 0;
    if (io_0 >= sysml_state_targetDegC) {
        sysml_conditional_12 = true;
    }
    else {
        sysml_conditional_12 = false;
    }
    sysml_value_3 = sysml_conditional_12;
    int sysml_value_4 = 0;
    sysml_value_4 = 0;
    int sysml_value_5 = 0;
    sysml_value_5 = sysml_state_stageId;
    double sysml_value_6 = 0;
    sysml_value_6 = io_0;
    int sysml_value_7 = 0;
    sysml_value_7 = sysml_state_remainingSeconds;
    *io_4 = sysml_value_0;
    *io_5 = sysml_value_1;
    *io_6 = sysml_value_2;
    *serial_0_trigger = sysml_value_3;
    *serial_0_value_0 = sysml_value_4;
    *serial_0_value_1 = sysml_value_5;
    *serial_0_value_2 = sysml_value_6;
    *serial_0_value_3 = sysml_value_7;
}