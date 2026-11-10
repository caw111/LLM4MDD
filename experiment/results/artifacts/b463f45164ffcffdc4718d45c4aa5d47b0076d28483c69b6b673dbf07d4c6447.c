#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_channelIndex = 0;
/* Global Variable define of  */
int sysml_state_prevChannelIndex = 0;
/* Global Variable define of  */
bool sysml_state_initialized = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_channelIndex = 0;
    sysml_state_prevChannelIndex = 0;
    sysml_state_initialized = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_4, bool* io_0, bool* io_1, bool* io_2, bool* io_3, bool* serial_0_trigger, int* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_conditional_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_conditional_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_conditional_6 = 0;
    bool sysml_conditional_7 = 0;
    bool sysml_conditional_8 = 0;
    int sysml_conditional_9 = 0;
    int sysml_conditional_10 = 0;
    int sysml_conditional_11 = 0;
    int sysml_conditional_12 = 0;
    int sysml_conditional_13 = 0;
    int sysml_conditional_14 = 0;
    int sysml_conditional_15 = 0;
    int sysml_conditional_16 = 0;
    int sysml_conditional_17 = 0;
    int sysml_conditional_18 = 0;
    int sysml_conditional_19 = 0;
    int sysml_conditional_20 = 0;
    int sysml_conditional_21 = 0;
    int sysml_conditional_22 = 0;
    int sysml_conditional_23 = 0;
    int sysml_conditional_24 = 0;
    int sysml_conditional_25 = 0;
    int sysml_conditional_26 = 0;
    int sysml_conditional_27 = 0;
    int sysml_conditional_28 = 0;
    int sysml_conditional_29 = 0;
    int sysml_conditional_30 = 0;
    int sysml_conditional_31 = 0;
    if ((!sysml_state_initialized)) {
        sysml_state_initialized = true;
        sysml_state_prevChannelIndex = 0;
        sysml_state_channelIndex = 0;
    }
    else {
        sysml_state_prevChannelIndex = sysml_state_channelIndex;
        if ((sysml_state_channelIndex == 5)) {
            sysml_state_channelIndex = 0;
        }
        else {
            sysml_state_channelIndex = (sysml_state_channelIndex + 1);
        }
    }
    bool sysml_value_0 = 0;
    sysml_lazy_0 = sysml_state_channelIndex == 1;
    if (!sysml_lazy_0) {
        sysml_lazy_0 = sysml_state_channelIndex == 3;
    }
    sysml_lazy_1 = sysml_lazy_0;
    if (!sysml_lazy_1) {
        sysml_lazy_1 = sysml_state_channelIndex == 5;
    }
    if (sysml_lazy_1) {
        sysml_conditional_2 = true;
    }
    else {
        sysml_conditional_2 = false;
    }
    sysml_value_0 = sysml_conditional_2;
    bool sysml_value_1 = 0;
    sysml_lazy_3 = sysml_state_channelIndex >= 2;
    if (sysml_lazy_3) {
        sysml_lazy_3 = sysml_state_channelIndex <= 5;
    }
    if (sysml_lazy_3) {
        sysml_lazy_5 = sysml_state_channelIndex == 2;
        if (!sysml_lazy_5) {
            sysml_lazy_5 = sysml_state_channelIndex == 3;
        }
        if (sysml_lazy_5) {
            sysml_conditional_6 = false;
        }
        else {
            sysml_conditional_6 = true;
        }
        sysml_conditional_4 = sysml_conditional_6;
    }
    else {
        sysml_conditional_4 = false;
    }
    sysml_value_1 = sysml_conditional_4;
    bool sysml_value_2 = 0;
    if (sysml_state_channelIndex >= 4) {
        sysml_conditional_7 = true;
    }
    else {
        sysml_conditional_7 = false;
    }
    sysml_value_2 = sysml_conditional_7;
    bool sysml_value_3 = 0;
    sysml_value_3 = false;
    bool sysml_value_4 = 0;
    if (sysml_state_initialized) {
        sysml_conditional_8 = true;
    }
    else {
        sysml_conditional_8 = false;
    }
    sysml_value_4 = sysml_conditional_8;
    int sysml_value_5 = 0;
    if (sysml_state_channelIndex == 0) {
        if (io_4 >= 0) {
            if (io_4 < ((double)((int)io_4))) {
                sysml_conditional_11 = 1;
            }
            else {
                sysml_conditional_11 = 0;
            }
            sysml_conditional_10 = ((int)io_4) - sysml_conditional_11;
        }
        else {
            if (io_4 < ((double)((int)io_4))) {
                sysml_conditional_12 = 1;
            }
            else {
                sysml_conditional_12 = 0;
            }
            sysml_conditional_10 = ((int)io_4) - sysml_conditional_12;
        }
        sysml_conditional_9 = sysml_conditional_10;
    }
    else {
        if (sysml_state_channelIndex == 1) {
            if (io_4 >= 0) {
                if (io_4 < ((double)((int)io_4))) {
                    sysml_conditional_15 = 1;
                }
                else {
                    sysml_conditional_15 = 0;
                }
                sysml_conditional_14 = ((int)io_4) - sysml_conditional_15;
            }
            else {
                if (io_4 < ((double)((int)io_4))) {
                    sysml_conditional_16 = 1;
                }
                else {
                    sysml_conditional_16 = 0;
                }
                sysml_conditional_14 = ((int)io_4) - sysml_conditional_16;
            }
            sysml_conditional_13 = sysml_conditional_14;
        }
        else {
            if (sysml_state_channelIndex == 2) {
                if (io_4 >= 0) {
                    if (io_4 < ((double)((int)io_4))) {
                        sysml_conditional_19 = 1;
                    }
                    else {
                        sysml_conditional_19 = 0;
                    }
                    sysml_conditional_18 = ((int)io_4) - sysml_conditional_19;
                }
                else {
                    if (io_4 < ((double)((int)io_4))) {
                        sysml_conditional_20 = 1;
                    }
                    else {
                        sysml_conditional_20 = 0;
                    }
                    sysml_conditional_18 = ((int)io_4) - sysml_conditional_20;
                }
                sysml_conditional_17 = sysml_conditional_18;
            }
            else {
                if (sysml_state_channelIndex == 3) {
                    if (io_4 >= 0) {
                        if (io_4 < ((double)((int)io_4))) {
                            sysml_conditional_23 = 1;
                        }
                        else {
                            sysml_conditional_23 = 0;
                        }
                        sysml_conditional_22 = ((int)io_4) - sysml_conditional_23;
                    }
                    else {
                        if (io_4 < ((double)((int)io_4))) {
                            sysml_conditional_24 = 1;
                        }
                        else {
                            sysml_conditional_24 = 0;
                        }
                        sysml_conditional_22 = ((int)io_4) - sysml_conditional_24;
                    }
                    sysml_conditional_21 = sysml_conditional_22;
                }
                else {
                    if (sysml_state_channelIndex == 4) {
                        if (io_4 >= 0) {
                            if (io_4 < ((double)((int)io_4))) {
                                sysml_conditional_27 = 1;
                            }
                            else {
                                sysml_conditional_27 = 0;
                            }
                            sysml_conditional_26 = ((int)io_4) - sysml_conditional_27;
                        }
                        else {
                            if (io_4 < ((double)((int)io_4))) {
                                sysml_conditional_28 = 1;
                            }
                            else {
                                sysml_conditional_28 = 0;
                            }
                            sysml_conditional_26 = ((int)io_4) - sysml_conditional_28;
                        }
                        sysml_conditional_25 = sysml_conditional_26;
                    }
                    else {
                        if (io_4 >= 0) {
                            if (io_4 < ((double)((int)io_4))) {
                                sysml_conditional_30 = 1;
                            }
                            else {
                                sysml_conditional_30 = 0;
                            }
                            sysml_conditional_29 = ((int)io_4) - sysml_conditional_30;
                        }
                        else {
                            if (io_4 < ((double)((int)io_4))) {
                                sysml_conditional_31 = 1;
                            }
                            else {
                                sysml_conditional_31 = 0;
                            }
                            sysml_conditional_29 = ((int)io_4) - sysml_conditional_31;
                        }
                        sysml_conditional_25 = sysml_conditional_29;
                    }
                    sysml_conditional_21 = sysml_conditional_25;
                }
                sysml_conditional_17 = sysml_conditional_21;
            }
            sysml_conditional_13 = sysml_conditional_17;
        }
        sysml_conditional_9 = sysml_conditional_13;
    }
    sysml_value_5 = sysml_conditional_9;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
    *serial_0_trigger = sysml_value_4;
    *serial_0_value_0 = sysml_value_5;
}