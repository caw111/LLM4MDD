#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_channelIndex = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_channelIndex = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_4, bool* io_0, bool* io_1, bool* io_2, bool* io_3, bool* serial_0_trigger, int* serial_0_value_0, int* serial_0_value_1, int* serial_0_value_2, int* serial_0_value_3, int* serial_0_value_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
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
    bool sysml_lazy_12 = 0;
    bool sysml_lazy_13 = 0;
    bool sysml_lazy_14 = 0;
    bool sysml_lazy_15 = 0;
    bool sysml_lazy_16 = 0;
    bool sysml_lazy_17 = 0;
    int sysml_conditional_18 = 0;
    int sysml_conditional_19 = 0;
    int sysml_conditional_20 = 0;
    int sysml_conditional_21 = 0;
    int sysml_conditional_22 = 0;
    if (sysml_state_channelIndex == 4) {
        sysml_conditional_0 = 0;
    }
    else {
        sysml_conditional_0 = sysml_state_channelIndex + 1;
    }
    sysml_state_channelIndex = sysml_conditional_0;
    bool sysml_value_0 = 0;
    sysml_lazy_1 = sysml_state_channelIndex == 1;
    if (!sysml_lazy_1) {
        sysml_lazy_1 = sysml_state_channelIndex == 3;
    }
    sysml_lazy_2 = sysml_lazy_1;
    if (!sysml_lazy_2) {
        sysml_lazy_2 = sysml_state_channelIndex == 5;
    }
    sysml_lazy_3 = sysml_lazy_2;
    if (!sysml_lazy_3) {
        sysml_lazy_3 = sysml_state_channelIndex == 7;
    }
    sysml_lazy_4 = sysml_lazy_3;
    if (!sysml_lazy_4) {
        sysml_lazy_4 = sysml_state_channelIndex == 9;
    }
    sysml_lazy_5 = sysml_lazy_4;
    if (!sysml_lazy_5) {
        sysml_lazy_5 = sysml_state_channelIndex == 11;
    }
    sysml_lazy_6 = sysml_lazy_5;
    if (!sysml_lazy_6) {
        sysml_lazy_6 = sysml_state_channelIndex == 13;
    }
    sysml_lazy_7 = sysml_lazy_6;
    if (!sysml_lazy_7) {
        sysml_lazy_7 = sysml_state_channelIndex == 15;
    }
    sysml_value_0 = sysml_lazy_7;
    bool sysml_value_1 = 0;
    sysml_lazy_8 = sysml_state_channelIndex == 2;
    if (!sysml_lazy_8) {
        sysml_lazy_8 = sysml_state_channelIndex == 3;
    }
    sysml_lazy_9 = sysml_lazy_8;
    if (!sysml_lazy_9) {
        sysml_lazy_9 = sysml_state_channelIndex == 6;
    }
    sysml_lazy_10 = sysml_lazy_9;
    if (!sysml_lazy_10) {
        sysml_lazy_10 = sysml_state_channelIndex == 7;
    }
    sysml_lazy_11 = sysml_lazy_10;
    if (!sysml_lazy_11) {
        sysml_lazy_11 = sysml_state_channelIndex == 10;
    }
    sysml_lazy_12 = sysml_lazy_11;
    if (!sysml_lazy_12) {
        sysml_lazy_12 = sysml_state_channelIndex == 11;
    }
    sysml_lazy_13 = sysml_lazy_12;
    if (!sysml_lazy_13) {
        sysml_lazy_13 = sysml_state_channelIndex == 14;
    }
    sysml_lazy_14 = sysml_lazy_13;
    if (!sysml_lazy_14) {
        sysml_lazy_14 = sysml_state_channelIndex == 15;
    }
    sysml_value_1 = sysml_lazy_14;
    bool sysml_value_2 = 0;
    sysml_lazy_15 = sysml_state_channelIndex >= 4;
    if (sysml_lazy_15) {
        sysml_lazy_15 = sysml_state_channelIndex <= 7;
    }
    sysml_lazy_16 = sysml_lazy_15;
    if (!sysml_lazy_16) {
        sysml_lazy_17 = sysml_state_channelIndex >= 12;
        if (sysml_lazy_17) {
            sysml_lazy_17 = sysml_state_channelIndex <= 15;
        }
        sysml_lazy_16 = sysml_lazy_17;
    }
    sysml_value_2 = sysml_lazy_16;
    bool sysml_value_3 = 0;
    sysml_value_3 = false;
    bool sysml_value_4 = 0;
    sysml_value_4 = (sysml_state_channelIndex == 0);
    int sysml_value_5 = 0;
    if (io_4 < ((double)((int)io_4))) {
        sysml_conditional_18 = 1;
    }
    else {
        sysml_conditional_18 = 0;
    }
    sysml_value_5 = ((int)io_4) - sysml_conditional_18;
    int sysml_value_6 = 0;
    if (io_4 < ((double)((int)io_4))) {
        sysml_conditional_19 = 1;
    }
    else {
        sysml_conditional_19 = 0;
    }
    sysml_value_6 = ((int)io_4) - sysml_conditional_19;
    int sysml_value_7 = 0;
    if (io_4 < ((double)((int)io_4))) {
        sysml_conditional_20 = 1;
    }
    else {
        sysml_conditional_20 = 0;
    }
    sysml_value_7 = ((int)io_4) - sysml_conditional_20;
    int sysml_value_8 = 0;
    if (io_4 < ((double)((int)io_4))) {
        sysml_conditional_21 = 1;
    }
    else {
        sysml_conditional_21 = 0;
    }
    sysml_value_8 = ((int)io_4) - sysml_conditional_21;
    int sysml_value_9 = 0;
    if (io_4 < ((double)((int)io_4))) {
        sysml_conditional_22 = 1;
    }
    else {
        sysml_conditional_22 = 0;
    }
    sysml_value_9 = ((int)io_4) - sysml_conditional_22;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
    *serial_0_trigger = sysml_value_4;
    *serial_0_value_0 = sysml_value_5;
    *serial_0_value_1 = sysml_value_6;
    *serial_0_value_2 = sysml_value_7;
    *serial_0_value_3 = sysml_value_8;
    *serial_0_value_4 = sysml_value_9;
}