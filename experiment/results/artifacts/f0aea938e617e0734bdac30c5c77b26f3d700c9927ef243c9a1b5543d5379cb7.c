#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_prevVehApproach = false;
/* Global Variable define of  */
bool sysml_state_prevPedApproach = false;
/* Global Variable define of  */
bool sysml_state_inDanger = false;
/* Global Variable define of  */
int sysml_state_dangerStartMs = 0;
/* Global Variable define of  */
int sysml_state_lastDangerDurationMs = 0;
/* Global Variable define of  */
bool sysml_state_dangerEventReported = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_prevVehApproach = false;
    sysml_state_prevPedApproach = false;
    sysml_state_inDanger = false;
    sysml_state_dangerStartMs = 0;
    sysml_state_lastDangerDurationMs = 0;
    sysml_state_dangerEventReported = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, double io_2, bool io_3, bool* io_4, bool* io_5, int* io_6, int* io_7, bool* io_8, bool* serial_0_trigger, int* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_lazy_10 = 0;
    bool sysml_lazy_11 = 0;
    int sysml_conditional_12 = 0;
    bool sysml_lazy_13 = 0;
    bool sysml_lazy_14 = 0;
    bool sysml_lazy_15 = 0;
    int sysml_conditional_16 = 0;
    bool sysml_lazy_17 = 0;
    bool sysml_lazy_18 = 0;
    bool sysml_lazy_19 = 0;
    int sysml_conditional_20 = 0;
    int sysml_conditional_21 = 0;
    sysml_lazy_0 = io_1;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_0 >= 5.0;
    }
    sysml_lazy_1 = sysml_lazy_0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = io_3;
    }
    sysml_lazy_2 = sysml_lazy_1;
    if (sysml_lazy_2) {
        sysml_lazy_2 = io_2 >= 5.0;
    }
    if (sysml_lazy_2) {
        sysml_lazy_3 = !sysml_state_inDanger;
        if (sysml_lazy_3) {
            sysml_lazy_4 = io_1;
            if (sysml_lazy_4) {
                sysml_lazy_4 = io_0 >= 5.0;
            }
            sysml_lazy_5 = sysml_lazy_4;
            if (sysml_lazy_5) {
                sysml_lazy_6 = io_3;
                if (sysml_lazy_6) {
                    sysml_lazy_6 = io_2 >= 5.0;
                }
                sysml_lazy_5 = sysml_lazy_6;
            }
            sysml_lazy_3 = sysml_lazy_5;
        }
        if (sysml_lazy_3) {
            sysml_state_inDanger = true;
            sysml_state_dangerStartMs = 0;
            sysml_state_dangerEventReported = false;
        }
    }
    sysml_lazy_7 = io_1;
    if (sysml_lazy_7) {
        sysml_lazy_7 = io_0 >= 5.0;
    }
    sysml_lazy_8 = sysml_lazy_7;
    if (sysml_lazy_8) {
        sysml_lazy_8 = io_3;
    }
    sysml_lazy_9 = sysml_lazy_8;
    if (sysml_lazy_9) {
        sysml_lazy_9 = io_2 >= 5.0;
    }
    if (!sysml_lazy_9) {
        if (sysml_state_inDanger) {
            sysml_state_inDanger = false;
            sysml_state_lastDangerDurationMs = sysml_state_dangerStartMs;
            sysml_state_dangerEventReported = true;
        }
    }
    sysml_lazy_10 = io_1;
    if (sysml_lazy_10) {
        sysml_lazy_10 = io_0 >= 5.0;
    }
    sysml_state_prevVehApproach = sysml_lazy_10;
    sysml_lazy_11 = io_3;
    if (sysml_lazy_11) {
        sysml_lazy_11 = io_2 >= 5.0;
    }
    sysml_state_prevPedApproach = sysml_lazy_11;
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_inDanger;
    bool sysml_value_1 = 0;
    sysml_value_1 = (!sysml_state_inDanger);
    int sysml_value_2 = 0;
    if (sysml_state_inDanger) {
        sysml_conditional_12 = 2000;
    }
    else {
        sysml_conditional_12 = 0;
    }
    sysml_value_2 = sysml_conditional_12;
    int sysml_value_3 = 0;
    sysml_lazy_13 = io_1;
    if (sysml_lazy_13) {
        sysml_lazy_13 = io_0 < 20.0;
    }
    sysml_lazy_14 = sysml_lazy_13;
    if (sysml_lazy_14) {
        sysml_lazy_14 = io_3;
    }
    sysml_lazy_15 = sysml_lazy_14;
    if (sysml_lazy_15) {
        sysml_lazy_15 = io_2 < 20.0;
    }
    if (sysml_lazy_15) {
        sysml_conditional_16 = 3;
    }
    else {
        sysml_lazy_17 = io_1;
        if (sysml_lazy_17) {
            sysml_lazy_17 = io_0 < 50.0;
        }
        sysml_lazy_18 = sysml_lazy_17;
        if (sysml_lazy_18) {
            sysml_lazy_18 = io_3;
        }
        sysml_lazy_19 = sysml_lazy_18;
        if (sysml_lazy_19) {
            sysml_lazy_19 = io_2 < 50.0;
        }
        if (sysml_lazy_19) {
            sysml_conditional_20 = 2;
        }
        else {
            if (sysml_state_inDanger) {
                sysml_conditional_21 = 1;
            }
            else {
                sysml_conditional_21 = 0;
            }
            sysml_conditional_20 = sysml_conditional_21;
        }
        sysml_conditional_16 = sysml_conditional_20;
    }
    sysml_value_3 = sysml_conditional_16;
    bool sysml_value_4 = 0;
    sysml_value_4 = false;
    bool sysml_value_5 = 0;
    sysml_value_5 = sysml_state_dangerEventReported;
    int sysml_value_6 = 0;
    sysml_value_6 = sysml_state_lastDangerDurationMs;
    *io_4 = sysml_value_0;
    *io_5 = sysml_value_1;
    *io_6 = sysml_value_2;
    *io_7 = sysml_value_3;
    *io_8 = sysml_value_4;
    *serial_0_trigger = sysml_value_5;
    *serial_0_value_0 = sysml_value_6;
}