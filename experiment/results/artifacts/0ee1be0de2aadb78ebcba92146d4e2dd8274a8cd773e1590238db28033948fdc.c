#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_ch = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_ch = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool* io_1, bool* io_2, bool* io_3, bool* io_4, bool* serial_0_trigger, int* serial_0_value_0) {
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
    sysml_lazy_0 = sysml_state_ch >= 0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = sysml_state_ch <= 5;
    }
    if (sysml_lazy_0) {
        sysml_state_ch = (sysml_state_ch + 1);
    }
    else {
        sysml_state_ch = 0;
    }
    if ((sysml_state_ch >= 6)) {
        sysml_state_ch = 0;
    }
    bool sysml_value_0 = 0;
    sysml_lazy_1 = sysml_state_ch == 0;
    if (!sysml_lazy_1) {
        sysml_lazy_1 = sysml_state_ch == 2;
    }
    sysml_lazy_2 = sysml_lazy_1;
    if (!sysml_lazy_2) {
        sysml_lazy_2 = sysml_state_ch == 4;
    }
    sysml_value_0 = sysml_lazy_2;
    bool sysml_value_1 = 0;
    sysml_lazy_3 = sysml_state_ch == 1;
    if (!sysml_lazy_3) {
        sysml_lazy_3 = sysml_state_ch == 2;
    }
    sysml_lazy_4 = sysml_lazy_3;
    if (!sysml_lazy_4) {
        sysml_lazy_4 = sysml_state_ch == 5;
    }
    sysml_value_1 = sysml_lazy_4;
    bool sysml_value_2 = 0;
    sysml_lazy_5 = sysml_state_ch == 2;
    if (!sysml_lazy_5) {
        sysml_lazy_5 = sysml_state_ch == 3;
    }
    sysml_lazy_6 = sysml_lazy_5;
    if (!sysml_lazy_6) {
        sysml_lazy_6 = sysml_state_ch == 6;
    }
    sysml_value_2 = sysml_lazy_6;
    bool sysml_value_3 = 0;
    sysml_lazy_7 = sysml_state_ch == 4;
    if (!sysml_lazy_7) {
        sysml_lazy_7 = sysml_state_ch == 5;
    }
    sysml_lazy_8 = sysml_lazy_7;
    if (!sysml_lazy_8) {
        sysml_lazy_8 = sysml_state_ch == 6;
    }
    sysml_value_3 = sysml_lazy_8;
    bool sysml_value_4 = 0;
    sysml_value_4 = true;
    int sysml_value_5 = 0;
    if (sysml_state_ch == 0) {
        if (true) {
            if (io_0 < ((double)((int)io_0))) {
                sysml_conditional_11 = 1;
            }
            else {
                sysml_conditional_11 = 0;
            }
            sysml_conditional_10 = ((int)io_0) - sysml_conditional_11;
        }
        else {
            sysml_conditional_10 = 0;
        }
        sysml_conditional_9 = sysml_conditional_10;
    }
    else {
        if (sysml_state_ch == 1) {
            if (true) {
                if (io_0 < ((double)((int)io_0))) {
                    sysml_conditional_14 = 1;
                }
                else {
                    sysml_conditional_14 = 0;
                }
                sysml_conditional_13 = ((int)io_0) - sysml_conditional_14;
            }
            else {
                sysml_conditional_13 = 0;
            }
            sysml_conditional_12 = sysml_conditional_13;
        }
        else {
            if (sysml_state_ch == 2) {
                if (true) {
                    if (io_0 < ((double)((int)io_0))) {
                        sysml_conditional_17 = 1;
                    }
                    else {
                        sysml_conditional_17 = 0;
                    }
                    sysml_conditional_16 = ((int)io_0) - sysml_conditional_17;
                }
                else {
                    sysml_conditional_16 = 0;
                }
                sysml_conditional_15 = sysml_conditional_16;
            }
            else {
                if (sysml_state_ch == 3) {
                    if (true) {
                        if (io_0 < ((double)((int)io_0))) {
                            sysml_conditional_20 = 1;
                        }
                        else {
                            sysml_conditional_20 = 0;
                        }
                        sysml_conditional_19 = ((int)io_0) - sysml_conditional_20;
                    }
                    else {
                        sysml_conditional_19 = 0;
                    }
                    sysml_conditional_18 = sysml_conditional_19;
                }
                else {
                    if (sysml_state_ch == 4) {
                        if (true) {
                            if (io_0 < ((double)((int)io_0))) {
                                sysml_conditional_23 = 1;
                            }
                            else {
                                sysml_conditional_23 = 0;
                            }
                            sysml_conditional_22 = ((int)io_0) - sysml_conditional_23;
                        }
                        else {
                            sysml_conditional_22 = 0;
                        }
                        sysml_conditional_21 = sysml_conditional_22;
                    }
                    else {
                        if (sysml_state_ch == 5) {
                            if (true) {
                                if (io_0 < ((double)((int)io_0))) {
                                    sysml_conditional_26 = 1;
                                }
                                else {
                                    sysml_conditional_26 = 0;
                                }
                                sysml_conditional_25 = ((int)io_0) - sysml_conditional_26;
                            }
                            else {
                                sysml_conditional_25 = 0;
                            }
                            sysml_conditional_24 = sysml_conditional_25;
                        }
                        else {
                            sysml_conditional_24 = 0;
                        }
                        sysml_conditional_21 = sysml_conditional_24;
                    }
                    sysml_conditional_18 = sysml_conditional_21;
                }
                sysml_conditional_15 = sysml_conditional_18;
            }
            sysml_conditional_12 = sysml_conditional_15;
        }
        sysml_conditional_9 = sysml_conditional_12;
    }
    sysml_value_5 = sysml_conditional_9;
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
    *io_3 = sysml_value_2;
    *io_4 = sysml_value_3;
    *serial_0_trigger = sysml_value_4;
    *serial_0_value_0 = sysml_value_5;
}