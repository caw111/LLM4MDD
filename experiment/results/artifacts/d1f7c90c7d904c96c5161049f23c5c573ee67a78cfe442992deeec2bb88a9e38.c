#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_doorOpenMs = 0;
/* Global Variable define of  */
bool sysml_state_prevDoorOpen = false;
/* Global Variable define of  */
int sysml_state_doorOpenSeconds = 0;
/* Global Variable define of  */
bool sysml_state_fanCoolRelayOn = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_doorOpenMs = 0;
    sysml_state_prevDoorOpen = false;
    sysml_state_doorOpenSeconds = 0;
    sysml_state_fanCoolRelayOn = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, bool* io_2, int* io_3, int* io_4, bool* io_5, bool* serial_0_trigger, int* serial_0_value_0, bool* serial_1_trigger, double* serial_1_value_0, int* serial_1_value_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    int sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    int sysml_conditional_5 = 0;
    int sysml_conditional_6 = 0;
    int sysml_conditional_7 = 0;
    int sysml_conditional_8 = 0;
    bool sysml_conditional_9 = 0;
    bool sysml_lazy_10 = 0;
    bool sysml_lazy_11 = 0;
    if (io_1) {
        sysml_state_doorOpenMs = (sysml_state_doorOpenMs + 10);
    }
    else {
        sysml_state_doorOpenMs = 0;
    }
    if (sysml_state_doorOpenMs >= 0) {
        if ((((double)sysml_state_doorOpenMs) / 1000.0) < ((double)((int)(((double)sysml_state_doorOpenMs) / 1000.0)))) {
            sysml_conditional_1 = 1;
        }
        else {
            sysml_conditional_1 = 0;
        }
        sysml_conditional_0 = ((int)(((double)sysml_state_doorOpenMs) / 1000.0)) - sysml_conditional_1;
    }
    else {
        sysml_conditional_0 = 0;
    }
    sysml_state_doorOpenSeconds = sysml_conditional_0;
    sysml_state_fanCoolRelayOn = sysml_state_fanCoolRelayOn;
    sysml_lazy_2 = io_0 >= 300.0;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !io_1;
    }
    if (sysml_lazy_2) {
        sysml_state_fanCoolRelayOn = true;
    }
    sysml_lazy_3 = io_0 <= 280.0;
    if (sysml_lazy_3) {
        sysml_lazy_3 = !io_1;
    }
    if (sysml_lazy_3) {
        sysml_state_fanCoolRelayOn = false;
    }
    sysml_lazy_4 = io_1;
    if (sysml_lazy_4) {
        sysml_lazy_4 = sysml_state_fanCoolRelayOn;
    }
    if (sysml_lazy_4) {
        sysml_state_fanCoolRelayOn = false;
    }
    sysml_state_prevDoorOpen = io_1;
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_fanCoolRelayOn;
    int sysml_value_1 = 0;
    if (sysml_state_doorOpenMs > 10000) {
        sysml_conditional_5 = 2500;
    }
    else {
        sysml_conditional_5 = 0;
    }
    sysml_value_1 = sysml_conditional_5;
    int sysml_value_2 = 0;
    if (io_1) {
        if (io_0 < ((double)((int)io_0))) {
            sysml_conditional_7 = 1;
        }
        else {
            sysml_conditional_7 = 0;
        }
        sysml_conditional_6 = ((int)io_0) - sysml_conditional_7;
    }
    else {
        if (io_0 < ((double)((int)io_0))) {
            sysml_conditional_8 = 1;
        }
        else {
            sysml_conditional_8 = 0;
        }
        sysml_conditional_6 = ((int)io_0) - sysml_conditional_8;
    }
    sysml_value_2 = sysml_conditional_6;
    bool sysml_value_3 = 0;
    if (sysml_state_doorOpenMs > 10000) {
        sysml_conditional_9 = true;
    }
    else {
        sysml_conditional_9 = false;
    }
    sysml_value_3 = sysml_conditional_9;
    bool sysml_value_4 = 0;
    sysml_lazy_10 = io_0 > 30.0;
    if (sysml_lazy_10) {
        sysml_lazy_10 = !io_1;
    }
    sysml_value_4 = sysml_lazy_10;
    int sysml_value_5 = 0;
    sysml_value_5 = 1;
    bool sysml_value_6 = 0;
    sysml_lazy_11 = io_0 > 30.0;
    if (sysml_lazy_11) {
        sysml_lazy_11 = !io_1;
    }
    sysml_value_6 = !sysml_lazy_11;
    double sysml_value_7 = 0;
    sysml_value_7 = io_0;
    int sysml_value_8 = 0;
    sysml_value_8 = sysml_state_doorOpenSeconds;
    *io_2 = sysml_value_0;
    *io_3 = sysml_value_1;
    *io_4 = sysml_value_2;
    *io_5 = sysml_value_3;
    *serial_0_trigger = sysml_value_4;
    *serial_0_value_0 = sysml_value_5;
    *serial_1_trigger = sysml_value_6;
    *serial_1_value_0 = sysml_value_7;
    *serial_1_value_1 = sysml_value_8;
}