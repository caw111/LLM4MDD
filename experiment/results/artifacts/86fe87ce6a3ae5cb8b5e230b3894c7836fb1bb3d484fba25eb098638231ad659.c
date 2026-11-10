#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_stage = 1;
/* Global Variable define of  */
bool sysml_state_inFault = false;
/* Global Variable define of  */
bool sysml_state_prevButton = false;
/* Global Variable define of  */
int sysml_state_silenceRun = 0;
/* Global Variable define of  */
bool sysml_state_faultLatched = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_stage = 1;
    sysml_state_inFault = false;
    sysml_state_prevButton = false;
    sysml_state_silenceRun = 0;
    sysml_state_faultLatched = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_4, double io_5, bool* io_1, int* io_2, int* io_3, bool* serial_0_trigger, int* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    int sysml_conditional_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    int sysml_conditional_9 = 0;
    int sysml_conditional_10 = 0;
    bool sysml_lazy_11 = 0;
    bool sysml_lazy_12 = 0;
    bool sysml_lazy_13 = 0;
    bool sysml_conditional_14 = 0;
    int sysml_conditional_15 = 0;
    int sysml_arithmetic_16 = 0;
    bool sysml_conditional_17 = 0;
    bool sysml_lazy_18 = 0;
    int sysml_conditional_19 = 0;
    int sysml_conditional_20 = 0;
    sysml_state_prevButton = io_0;
    if (sysml_state_inFault) {
        sysml_state_stage = 1;
        sysml_state_inFault = false;
        sysml_state_faultLatched = false;
        sysml_state_silenceRun = 0;
    }
    sysml_lazy_0 = sysml_state_silenceRun >= 50;
    if (sysml_lazy_0) {
        sysml_lazy_1 = io_5 < 10.0;
        if (!sysml_lazy_1) {
            sysml_lazy_1 = io_5 > 950.0;
        }
        sysml_lazy_0 = sysml_lazy_1;
    }
    if (sysml_lazy_0) {
        sysml_state_faultLatched = true;
        sysml_state_inFault = true;
        sysml_state_stage = 1;
    }
    else {
        sysml_state_faultLatched = false;
    }
    if (!io_4) {
        sysml_conditional_2 = sysml_state_silenceRun + 1;
    }
    else {
        sysml_conditional_2 = 0;
    }
    sysml_state_silenceRun = sysml_conditional_2;
    sysml_lazy_3 = io_5 >= 10.0;
    if (sysml_lazy_3) {
        sysml_lazy_3 = io_5 <= 950.0;
    }
    sysml_lazy_4 = sysml_lazy_3;
    if (sysml_lazy_4) {
        sysml_lazy_4 = sysml_state_silenceRun >= 20;
    }
    if (sysml_lazy_4) {
        sysml_state_stage = 4;
    }
    sysml_lazy_5 = !(sysml_state_stage == 4);
    if (sysml_lazy_5) {
        sysml_lazy_5 = io_5 < 10.0;
    }
    if (sysml_lazy_5) {
        sysml_state_stage = 1;
    }
    sysml_lazy_6 = !(sysml_state_stage == 4);
    if (sysml_lazy_6) {
        sysml_lazy_6 = io_5 >= 10.0;
    }
    sysml_lazy_7 = sysml_lazy_6;
    if (sysml_lazy_7) {
        sysml_lazy_7 = io_5 <= 950.0;
    }
    sysml_lazy_8 = sysml_lazy_7;
    if (sysml_lazy_8) {
        sysml_lazy_8 = !io_4;
    }
    if (sysml_lazy_8) {
        sysml_state_stage = 2;
    }
    else {
        if (!(sysml_state_stage == 4)) {
            if (sysml_state_silenceRun >= 20) {
                sysml_conditional_10 = 3;
            }
            else {
                sysml_conditional_10 = sysml_state_stage;
            }
            sysml_conditional_9 = sysml_conditional_10;
        }
        else {
            sysml_conditional_9 = sysml_state_stage;
        }
        sysml_state_stage = sysml_conditional_9;
    }
    sysml_lazy_11 = sysml_state_stage == 4;
    if (sysml_lazy_11) {
        sysml_lazy_11 = io_0;
    }
    sysml_lazy_12 = sysml_lazy_11;
    if (sysml_lazy_12) {
        sysml_lazy_12 = !sysml_state_prevButton;
    }
    if (sysml_lazy_12) {
        sysml_state_stage = 1;
        sysml_state_faultLatched = false;
        sysml_state_inFault = false;
        sysml_state_silenceRun = 0;
    }
    bool sysml_value_0 = 0;
    sysml_lazy_13 = sysml_state_stage == 4;
    if (sysml_lazy_13) {
        sysml_lazy_13 = !sysml_state_faultLatched;
    }
    if (sysml_lazy_13) {
        if ((((double)sysml_state_silenceRun) / 2) < ((double)((int)(((double)sysml_state_silenceRun) / 2)))) {
            sysml_conditional_15 = 1;
        }
        else {
            sysml_conditional_15 = 0;
        }
        sysml_arithmetic_16 = ((int)(((double)sysml_state_silenceRun) / 2)) - sysml_conditional_15;
        if ((sysml_state_silenceRun - (2 * sysml_arithmetic_16)) == 0) {
            sysml_conditional_17 = true;
        }
        else {
            sysml_conditional_17 = false;
        }
        sysml_conditional_14 = sysml_conditional_17;
    }
    else {
        sysml_conditional_14 = false;
    }
    sysml_value_0 = sysml_conditional_14;
    int sysml_value_1 = 0;
    sysml_lazy_18 = sysml_state_stage == 4;
    if (sysml_lazy_18) {
        sysml_lazy_18 = !sysml_state_faultLatched;
    }
    if (sysml_lazy_18) {
        sysml_conditional_19 = 1000;
    }
    else {
        sysml_conditional_19 = 0;
    }
    sysml_value_1 = sysml_conditional_19;
    int sysml_value_2 = 0;
    sysml_value_2 = sysml_state_stage;
    bool sysml_value_3 = 0;
    sysml_value_3 = sysml_state_inFault;
    int sysml_value_4 = 0;
    if (sysml_state_inFault) {
        sysml_conditional_20 = 1;
    }
    else {
        sysml_conditional_20 = 0;
    }
    sysml_value_4 = sysml_conditional_20;
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
    *io_3 = sysml_value_2;
    *serial_0_trigger = sysml_value_3;
    *serial_0_value_0 = sysml_value_4;
}