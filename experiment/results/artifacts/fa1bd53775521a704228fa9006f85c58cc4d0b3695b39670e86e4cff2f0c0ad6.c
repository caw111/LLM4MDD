#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_number = 0;
/* Global Variable define of  */
bool sysml_state_prevBtn1 = false;
/* Global Variable define of  */
bool sysml_state_prevBtn2 = false;
/* Global Variable define of  */
int sysml_state_accIncMs = 0;
/* Global Variable define of  */
int sysml_state_accDecMs = 0;
/* Global Variable define of  */
bool sysml_state_initialized = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_number = 0;
    sysml_state_prevBtn1 = false;
    sysml_state_prevBtn2 = false;
    sysml_state_accIncMs = 0;
    sysml_state_accDecMs = 0;
    sysml_state_initialized = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double io_2, double io_3, int io_5, int* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    bool sysml_lazy_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    int sysml_conditional_7 = 0;
    int sysml_conditional_8 = 0;
    bool sysml_lazy_9 = 0;
    int sysml_conditional_10 = 0;
    bool sysml_lazy_11 = 0;
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
    bool sysml_lazy_29 = 0;
    int sysml_conditional_30 = 0;
    bool sysml_lazy_31 = 0;
    int sysml_conditional_32 = 0;
    int sysml_conditional_33 = 0;
    int sysml_conditional_34 = 0;
    int sysml_conditional_35 = 0;
    int sysml_conditional_36 = 0;
    int sysml_conditional_37 = 0;
    int sysml_conditional_38 = 0;
    int sysml_conditional_39 = 0;
    int sysml_conditional_40 = 0;
    int sysml_conditional_41 = 0;
    int sysml_conditional_42 = 0;
    int sysml_conditional_43 = 0;
    int sysml_conditional_44 = 0;
    int sysml_conditional_45 = 0;
    int sysml_conditional_46 = 0;
    int sysml_conditional_47 = 0;
    int sysml_conditional_48 = 0;
    if ((!sysml_state_initialized)) {
        if (io_2 >= 0.0) {
            if ((((double)(io_3 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 99.0)) / 1023.0)))) {
                sysml_conditional_1 = 1;
            }
            else {
                sysml_conditional_1 = 0;
            }
            if ((((double)(io_2 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_2 * 99.0)) / 1023.0)))) {
                sysml_conditional_2 = 1;
            }
            else {
                sysml_conditional_2 = 0;
            }
            sysml_conditional_0 = ((((int)(((double)(io_3 * 99.0)) / 1023.0)) - sysml_conditional_1) * 10) + (((int)(((double)(io_2 * 99.0)) / 1023.0)) - sysml_conditional_2);
        }
        else {
            sysml_conditional_0 = 0;
        }
        sysml_state_number = sysml_conditional_0;
        sysml_state_initialized = true;
        sysml_state_prevBtn1 = io_0;
        sysml_state_prevBtn2 = io_1;
    }
    sysml_lazy_3 = io_0;
    if (sysml_lazy_3) {
        sysml_lazy_3 = !sysml_state_prevBtn1;
    }
    if (sysml_lazy_3) {
        sysml_conditional_4 = 0;
    }
    else {
        if (io_0) {
            sysml_conditional_5 = sysml_state_accIncMs + io_5;
        }
        else {
            sysml_conditional_5 = 0;
        }
        sysml_conditional_4 = sysml_conditional_5;
    }
    sysml_state_accIncMs = sysml_conditional_4;
    sysml_lazy_6 = io_1;
    if (sysml_lazy_6) {
        sysml_lazy_6 = !sysml_state_prevBtn2;
    }
    if (sysml_lazy_6) {
        sysml_conditional_7 = 0;
    }
    else {
        if (io_1) {
            sysml_conditional_8 = sysml_state_accDecMs + io_5;
        }
        else {
            sysml_conditional_8 = 0;
        }
        sysml_conditional_7 = sysml_conditional_8;
    }
    sysml_state_accDecMs = sysml_conditional_7;
    sysml_lazy_9 = io_0;
    if (sysml_lazy_9) {
        sysml_lazy_9 = !sysml_state_prevBtn1;
    }
    if (sysml_lazy_9) {
        if (sysml_state_number == 99) {
            sysml_conditional_10 = 0;
        }
        else {
            sysml_conditional_10 = sysml_state_number + 1;
        }
        sysml_state_number = sysml_conditional_10;
    }
    sysml_lazy_11 = io_0;
    if (sysml_lazy_11) {
        if ((((double)(io_3 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 99.0)) / 1023.0)))) {
            sysml_conditional_12 = 1;
        }
        else {
            sysml_conditional_12 = 0;
        }
        if ((((int)(((double)(io_3 * 99.0)) / 1023.0)) - sysml_conditional_12) == 0) {
            sysml_conditional_13 = 100;
        }
        else {
            if ((((double)(io_3 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 99.0)) / 1023.0)))) {
                sysml_conditional_14 = 1;
            }
            else {
                sysml_conditional_14 = 0;
            }
            if ((((int)(((double)(io_3 * 99.0)) / 1023.0)) - sysml_conditional_14) >= 99) {
                sysml_conditional_15 = 1000;
            }
            else {
                if ((((double)(io_3 * 900.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 900.0)) / 1023.0)))) {
                    sysml_conditional_16 = 1;
                }
                else {
                    sysml_conditional_16 = 0;
                }
                if ((((double)(io_3 * 900.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 900.0)) / 1023.0)))) {
                    sysml_conditional_17 = 1;
                }
                else {
                    sysml_conditional_17 = 0;
                }
                if ((((double)(io_3 * 900.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 900.0)) / 1023.0)))) {
                    sysml_conditional_18 = 1;
                }
                else {
                    sysml_conditional_18 = 0;
                }
                if ((100 + ((((int)(((double)(io_3 * 900.0)) / 1023.0)) - sysml_conditional_17) * 1.0)) < ((double)((int)(100 + ((((int)(((double)(io_3 * 900.0)) / 1023.0)) - sysml_conditional_18) * 1.0))))) {
                    sysml_conditional_19 = 1;
                }
                else {
                    sysml_conditional_19 = 0;
                }
                sysml_conditional_15 = ((int)(100 + ((((int)(((double)(io_3 * 900.0)) / 1023.0)) - sysml_conditional_16) * 1.0))) - sysml_conditional_19;
            }
            sysml_conditional_13 = sysml_conditional_15;
        }
        sysml_lazy_11 = sysml_state_accIncMs >= sysml_conditional_13;
    }
    if (sysml_lazy_11) {
        if ((((double)(io_3 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 99.0)) / 1023.0)))) {
            sysml_conditional_20 = 1;
        }
        else {
            sysml_conditional_20 = 0;
        }
        if ((((int)(((double)(io_3 * 99.0)) / 1023.0)) - sysml_conditional_20) == 0) {
            sysml_conditional_21 = 100;
        }
        else {
            if ((((double)(io_3 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 99.0)) / 1023.0)))) {
                sysml_conditional_22 = 1;
            }
            else {
                sysml_conditional_22 = 0;
            }
            if ((((int)(((double)(io_3 * 99.0)) / 1023.0)) - sysml_conditional_22) >= 99) {
                sysml_conditional_23 = 1000;
            }
            else {
                if ((((double)(io_3 * 900.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 900.0)) / 1023.0)))) {
                    sysml_conditional_24 = 1;
                }
                else {
                    sysml_conditional_24 = 0;
                }
                if ((((double)(io_3 * 900.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 900.0)) / 1023.0)))) {
                    sysml_conditional_25 = 1;
                }
                else {
                    sysml_conditional_25 = 0;
                }
                if ((((double)(io_3 * 900.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 900.0)) / 1023.0)))) {
                    sysml_conditional_26 = 1;
                }
                else {
                    sysml_conditional_26 = 0;
                }
                if ((100 + ((((int)(((double)(io_3 * 900.0)) / 1023.0)) - sysml_conditional_25) * 1.0)) < ((double)((int)(100 + ((((int)(((double)(io_3 * 900.0)) / 1023.0)) - sysml_conditional_26) * 1.0))))) {
                    sysml_conditional_27 = 1;
                }
                else {
                    sysml_conditional_27 = 0;
                }
                sysml_conditional_23 = ((int)(100 + ((((int)(((double)(io_3 * 900.0)) / 1023.0)) - sysml_conditional_24) * 1.0))) - sysml_conditional_27;
            }
            sysml_conditional_21 = sysml_conditional_23;
        }
        sysml_state_accIncMs = sysml_state_accIncMs - sysml_conditional_21;
        if (sysml_state_number == 99) {
            sysml_conditional_28 = 0;
        }
        else {
            sysml_conditional_28 = sysml_state_number + 1;
        }
        sysml_state_number = sysml_conditional_28;
    }
    sysml_lazy_29 = io_1;
    if (sysml_lazy_29) {
        sysml_lazy_29 = !sysml_state_prevBtn2;
    }
    if (sysml_lazy_29) {
        if (sysml_state_number == 0) {
            sysml_conditional_30 = 99;
        }
        else {
            sysml_conditional_30 = sysml_state_number - 1;
        }
        sysml_state_number = sysml_conditional_30;
    }
    sysml_lazy_31 = io_1;
    if (sysml_lazy_31) {
        if ((((double)(io_2 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_2 * 99.0)) / 1023.0)))) {
            sysml_conditional_32 = 1;
        }
        else {
            sysml_conditional_32 = 0;
        }
        if ((((int)(((double)(io_2 * 99.0)) / 1023.0)) - sysml_conditional_32) == 0) {
            sysml_conditional_33 = 100;
        }
        else {
            if ((((double)(io_2 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_2 * 99.0)) / 1023.0)))) {
                sysml_conditional_34 = 1;
            }
            else {
                sysml_conditional_34 = 0;
            }
            if ((((int)(((double)(io_2 * 99.0)) / 1023.0)) - sysml_conditional_34) >= 99) {
                sysml_conditional_35 = 1000;
            }
            else {
                if ((((double)(io_2 * 900.0)) / 1023.0) < ((double)((int)(((double)(io_2 * 900.0)) / 1023.0)))) {
                    sysml_conditional_36 = 1;
                }
                else {
                    sysml_conditional_36 = 0;
                }
                if ((((double)(io_2 * 900.0)) / 1023.0) < ((double)((int)(((double)(io_2 * 900.0)) / 1023.0)))) {
                    sysml_conditional_37 = 1;
                }
                else {
                    sysml_conditional_37 = 0;
                }
                if ((((double)(io_2 * 900.0)) / 1023.0) < ((double)((int)(((double)(io_2 * 900.0)) / 1023.0)))) {
                    sysml_conditional_38 = 1;
                }
                else {
                    sysml_conditional_38 = 0;
                }
                if ((100 + ((((int)(((double)(io_2 * 900.0)) / 1023.0)) - sysml_conditional_37) * 1.0)) < ((double)((int)(100 + ((((int)(((double)(io_2 * 900.0)) / 1023.0)) - sysml_conditional_38) * 1.0))))) {
                    sysml_conditional_39 = 1;
                }
                else {
                    sysml_conditional_39 = 0;
                }
                sysml_conditional_35 = ((int)(100 + ((((int)(((double)(io_2 * 900.0)) / 1023.0)) - sysml_conditional_36) * 1.0))) - sysml_conditional_39;
            }
            sysml_conditional_33 = sysml_conditional_35;
        }
        sysml_lazy_31 = sysml_state_accDecMs >= sysml_conditional_33;
    }
    if (sysml_lazy_31) {
        if ((((double)(io_2 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_2 * 99.0)) / 1023.0)))) {
            sysml_conditional_40 = 1;
        }
        else {
            sysml_conditional_40 = 0;
        }
        if ((((int)(((double)(io_2 * 99.0)) / 1023.0)) - sysml_conditional_40) == 0) {
            sysml_conditional_41 = 100;
        }
        else {
            if ((((double)(io_2 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_2 * 99.0)) / 1023.0)))) {
                sysml_conditional_42 = 1;
            }
            else {
                sysml_conditional_42 = 0;
            }
            if ((((int)(((double)(io_2 * 99.0)) / 1023.0)) - sysml_conditional_42) >= 99) {
                sysml_conditional_43 = 1000;
            }
            else {
                if ((((double)(io_2 * 900.0)) / 1023.0) < ((double)((int)(((double)(io_2 * 900.0)) / 1023.0)))) {
                    sysml_conditional_44 = 1;
                }
                else {
                    sysml_conditional_44 = 0;
                }
                if ((((double)(io_2 * 900.0)) / 1023.0) < ((double)((int)(((double)(io_2 * 900.0)) / 1023.0)))) {
                    sysml_conditional_45 = 1;
                }
                else {
                    sysml_conditional_45 = 0;
                }
                if ((((double)(io_2 * 900.0)) / 1023.0) < ((double)((int)(((double)(io_2 * 900.0)) / 1023.0)))) {
                    sysml_conditional_46 = 1;
                }
                else {
                    sysml_conditional_46 = 0;
                }
                if ((100 + ((((int)(((double)(io_2 * 900.0)) / 1023.0)) - sysml_conditional_45) * 1.0)) < ((double)((int)(100 + ((((int)(((double)(io_2 * 900.0)) / 1023.0)) - sysml_conditional_46) * 1.0))))) {
                    sysml_conditional_47 = 1;
                }
                else {
                    sysml_conditional_47 = 0;
                }
                sysml_conditional_43 = ((int)(100 + ((((int)(((double)(io_2 * 900.0)) / 1023.0)) - sysml_conditional_44) * 1.0))) - sysml_conditional_47;
            }
            sysml_conditional_41 = sysml_conditional_43;
        }
        sysml_state_accDecMs = sysml_state_accDecMs - sysml_conditional_41;
        if (sysml_state_number == 0) {
            sysml_conditional_48 = 99;
        }
        else {
            sysml_conditional_48 = sysml_state_number - 1;
        }
        sysml_state_number = sysml_conditional_48;
    }
    sysml_state_prevBtn1 = io_0;
    sysml_state_prevBtn2 = io_1;
    int sysml_value_0 = 0;
    sysml_value_0 = sysml_state_number;
    *io_4 = sysml_value_0;
}