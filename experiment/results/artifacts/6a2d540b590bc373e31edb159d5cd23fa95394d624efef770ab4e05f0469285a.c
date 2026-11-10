#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_3, bool* io_0, bool* io_1, bool* io_2, int* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_value_0 = 0;
    int sysml_conditional_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    bool sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    int sysml_conditional_6 = 0;
    int sysml_conditional_7 = 0;
    int sysml_conditional_8 = 0;
    bool sysml_lazy_9 = 0;
    int sysml_conditional_10 = 0;
    int sysml_conditional_11 = 0;
    int sysml_conditional_12 = 0;
    int sysml_conditional_13 = 0;
    int sysml_conditional_14 = 0;
    int sysml_conditional_15 = 0;
    int sysml_conditional_16 = 0;
    int sysml_conditional_17 = 0;
    bool sysml_lazy_18 = 0;
    int sysml_conditional_19 = 0;
    int sysml_conditional_20 = 0;
    int sysml_conditional_21 = 0;
    int sysml_conditional_22 = 0;
    int sysml_conditional_23 = 0;
    int sysml_conditional_24 = 0;
    int sysml_conditional_25 = 0;
    int sysml_conditional_26 = 0;
    bool sysml_lazy_27 = 0;
    int sysml_conditional_28 = 0;
    int sysml_conditional_29 = 0;
    int sysml_conditional_30 = 0;
    int sysml_conditional_31 = 0;
    bool sysml_conditional_32 = 0;
    int sysml_conditional_33 = 0;
    int sysml_conditional_34 = 0;
    int sysml_conditional_35 = 0;
    int sysml_conditional_36 = 0;
    bool sysml_conditional_37 = 0;
    int sysml_conditional_38 = 0;
    int sysml_conditional_39 = 0;
    int sysml_conditional_40 = 0;
    int sysml_conditional_41 = 0;
    bool sysml_conditional_42 = 0;
    int sysml_conditional_43 = 0;
    int sysml_conditional_44 = 0;
    int sysml_conditional_45 = 0;
    int sysml_conditional_46 = 0;
    int sysml_conditional_47 = 0;
    int sysml_conditional_48 = 0;
    int sysml_conditional_49 = 0;
    int sysml_conditional_50 = 0;
    int sysml_conditional_51 = 0;
    int sysml_conditional_52 = 0;
    int sysml_conditional_53 = 0;
    int sysml_conditional_54 = 0;
    int sysml_conditional_55 = 0;
    if ((((double)(io_3 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 99.0)) / 1023.0)))) {
        sysml_conditional_0 = 1;
    }
    else {
        sysml_conditional_0 = 0;
    }
    if ((((double)(io_3 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 99.0)) / 1023.0)))) {
        sysml_conditional_1 = 1;
    }
    else {
        sysml_conditional_1 = 0;
    }
    if ((((double)(io_3 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 99.0)) / 1023.0)))) {
        sysml_conditional_2 = 1;
    }
    else {
        sysml_conditional_2 = 0;
    }
    if ((((int)(((double)(io_3 * 99.0)) / 1023.0)) - sysml_conditional_1) < ((double)((int)(((int)(((double)(io_3 * 99.0)) / 1023.0)) - sysml_conditional_2)))) {
        sysml_conditional_3 = 1;
    }
    else {
        sysml_conditional_3 = 0;
    }
    if (((((int)(((int)(((double)(io_3 * 99.0)) / 1023.0)) - sysml_conditional_0)) - sysml_conditional_3) * 1.0) >= 0.0) {
        if ((((double)(io_3 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 99.0)) / 1023.0)))) {
            sysml_conditional_5 = 1;
        }
        else {
            sysml_conditional_5 = 0;
        }
        if ((((double)(io_3 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 99.0)) / 1023.0)))) {
            sysml_conditional_6 = 1;
        }
        else {
            sysml_conditional_6 = 0;
        }
        if ((((double)(io_3 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 99.0)) / 1023.0)))) {
            sysml_conditional_7 = 1;
        }
        else {
            sysml_conditional_7 = 0;
        }
        if ((((int)(((double)(io_3 * 99.0)) / 1023.0)) - sysml_conditional_6) < ((double)((int)(((int)(((double)(io_3 * 99.0)) / 1023.0)) - sysml_conditional_7)))) {
            sysml_conditional_8 = 1;
        }
        else {
            sysml_conditional_8 = 0;
        }
        sysml_lazy_9 = (((int)(((int)(((double)(io_3 * 99.0)) / 1023.0)) - sysml_conditional_5)) - sysml_conditional_8) >= 0;
        if (sysml_lazy_9) {
            if ((((double)(io_3 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 99.0)) / 1023.0)))) {
                sysml_conditional_10 = 1;
            }
            else {
                sysml_conditional_10 = 0;
            }
            if ((((double)(io_3 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 99.0)) / 1023.0)))) {
                sysml_conditional_11 = 1;
            }
            else {
                sysml_conditional_11 = 0;
            }
            if ((((double)(io_3 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 99.0)) / 1023.0)))) {
                sysml_conditional_12 = 1;
            }
            else {
                sysml_conditional_12 = 0;
            }
            if ((((int)(((double)(io_3 * 99.0)) / 1023.0)) - sysml_conditional_11) < ((double)((int)(((int)(((double)(io_3 * 99.0)) / 1023.0)) - sysml_conditional_12)))) {
                sysml_conditional_13 = 1;
            }
            else {
                sysml_conditional_13 = 0;
            }
            sysml_lazy_9 = (((int)(((int)(((double)(io_3 * 99.0)) / 1023.0)) - sysml_conditional_10)) - sysml_conditional_13) <= 33;
        }
        sysml_conditional_4 = sysml_lazy_9;
    }
    else {
        if ((((double)(io_3 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 99.0)) / 1023.0)))) {
            sysml_conditional_14 = 1;
        }
        else {
            sysml_conditional_14 = 0;
        }
        if ((((double)(io_3 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 99.0)) / 1023.0)))) {
            sysml_conditional_15 = 1;
        }
        else {
            sysml_conditional_15 = 0;
        }
        if ((((double)(io_3 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 99.0)) / 1023.0)))) {
            sysml_conditional_16 = 1;
        }
        else {
            sysml_conditional_16 = 0;
        }
        if ((((int)(((double)(io_3 * 99.0)) / 1023.0)) - sysml_conditional_15) < ((double)((int)(((int)(((double)(io_3 * 99.0)) / 1023.0)) - sysml_conditional_16)))) {
            sysml_conditional_17 = 1;
        }
        else {
            sysml_conditional_17 = 0;
        }
        sysml_lazy_18 = (((int)(((int)(((double)(io_3 * 99.0)) / 1023.0)) - sysml_conditional_14)) - sysml_conditional_17) >= 34;
        if (sysml_lazy_18) {
            if ((((double)(io_3 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 99.0)) / 1023.0)))) {
                sysml_conditional_19 = 1;
            }
            else {
                sysml_conditional_19 = 0;
            }
            if ((((double)(io_3 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 99.0)) / 1023.0)))) {
                sysml_conditional_20 = 1;
            }
            else {
                sysml_conditional_20 = 0;
            }
            if ((((double)(io_3 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 99.0)) / 1023.0)))) {
                sysml_conditional_21 = 1;
            }
            else {
                sysml_conditional_21 = 0;
            }
            if ((((int)(((double)(io_3 * 99.0)) / 1023.0)) - sysml_conditional_20) < ((double)((int)(((int)(((double)(io_3 * 99.0)) / 1023.0)) - sysml_conditional_21)))) {
                sysml_conditional_22 = 1;
            }
            else {
                sysml_conditional_22 = 0;
            }
            sysml_lazy_18 = (((int)(((int)(((double)(io_3 * 99.0)) / 1023.0)) - sysml_conditional_19)) - sysml_conditional_22) <= 99;
        }
        sysml_conditional_4 = sysml_lazy_18;
    }
    sysml_value_0 = sysml_conditional_4;
    bool sysml_value_1 = 0;
    if ((((double)(io_3 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 99.0)) / 1023.0)))) {
        sysml_conditional_23 = 1;
    }
    else {
        sysml_conditional_23 = 0;
    }
    if ((((double)(io_3 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 99.0)) / 1023.0)))) {
        sysml_conditional_24 = 1;
    }
    else {
        sysml_conditional_24 = 0;
    }
    if ((((double)(io_3 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 99.0)) / 1023.0)))) {
        sysml_conditional_25 = 1;
    }
    else {
        sysml_conditional_25 = 0;
    }
    if ((((int)(((double)(io_3 * 99.0)) / 1023.0)) - sysml_conditional_24) < ((double)((int)(((int)(((double)(io_3 * 99.0)) / 1023.0)) - sysml_conditional_25)))) {
        sysml_conditional_26 = 1;
    }
    else {
        sysml_conditional_26 = 0;
    }
    sysml_lazy_27 = (((int)(((int)(((double)(io_3 * 99.0)) / 1023.0)) - sysml_conditional_23)) - sysml_conditional_26) >= 34;
    if (sysml_lazy_27) {
        if ((((double)(io_3 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 99.0)) / 1023.0)))) {
            sysml_conditional_28 = 1;
        }
        else {
            sysml_conditional_28 = 0;
        }
        if ((((double)(io_3 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 99.0)) / 1023.0)))) {
            sysml_conditional_29 = 1;
        }
        else {
            sysml_conditional_29 = 0;
        }
        if ((((double)(io_3 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 99.0)) / 1023.0)))) {
            sysml_conditional_30 = 1;
        }
        else {
            sysml_conditional_30 = 0;
        }
        if ((((int)(((double)(io_3 * 99.0)) / 1023.0)) - sysml_conditional_29) < ((double)((int)(((int)(((double)(io_3 * 99.0)) / 1023.0)) - sysml_conditional_30)))) {
            sysml_conditional_31 = 1;
        }
        else {
            sysml_conditional_31 = 0;
        }
        sysml_lazy_27 = (((int)(((int)(((double)(io_3 * 99.0)) / 1023.0)) - sysml_conditional_28)) - sysml_conditional_31) <= 66;
    }
    if (sysml_lazy_27) {
        sysml_conditional_32 = true;
    }
    else {
        if ((((double)(io_3 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 99.0)) / 1023.0)))) {
            sysml_conditional_33 = 1;
        }
        else {
            sysml_conditional_33 = 0;
        }
        if ((((double)(io_3 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 99.0)) / 1023.0)))) {
            sysml_conditional_34 = 1;
        }
        else {
            sysml_conditional_34 = 0;
        }
        if ((((double)(io_3 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 99.0)) / 1023.0)))) {
            sysml_conditional_35 = 1;
        }
        else {
            sysml_conditional_35 = 0;
        }
        if ((((int)(((double)(io_3 * 99.0)) / 1023.0)) - sysml_conditional_34) < ((double)((int)(((int)(((double)(io_3 * 99.0)) / 1023.0)) - sysml_conditional_35)))) {
            sysml_conditional_36 = 1;
        }
        else {
            sysml_conditional_36 = 0;
        }
        if ((((int)(((int)(((double)(io_3 * 99.0)) / 1023.0)) - sysml_conditional_33)) - sysml_conditional_36) >= 67) {
            sysml_conditional_37 = true;
        }
        else {
            sysml_conditional_37 = false;
        }
        sysml_conditional_32 = sysml_conditional_37;
    }
    sysml_value_1 = sysml_conditional_32;
    bool sysml_value_2 = 0;
    if ((((double)(io_3 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 99.0)) / 1023.0)))) {
        sysml_conditional_38 = 1;
    }
    else {
        sysml_conditional_38 = 0;
    }
    if ((((double)(io_3 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 99.0)) / 1023.0)))) {
        sysml_conditional_39 = 1;
    }
    else {
        sysml_conditional_39 = 0;
    }
    if ((((double)(io_3 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 99.0)) / 1023.0)))) {
        sysml_conditional_40 = 1;
    }
    else {
        sysml_conditional_40 = 0;
    }
    if ((((int)(((double)(io_3 * 99.0)) / 1023.0)) - sysml_conditional_39) < ((double)((int)(((int)(((double)(io_3 * 99.0)) / 1023.0)) - sysml_conditional_40)))) {
        sysml_conditional_41 = 1;
    }
    else {
        sysml_conditional_41 = 0;
    }
    if ((((int)(((int)(((double)(io_3 * 99.0)) / 1023.0)) - sysml_conditional_38)) - sysml_conditional_41) >= 67) {
        sysml_conditional_42 = true;
    }
    else {
        sysml_conditional_42 = false;
    }
    sysml_value_2 = sysml_conditional_42;
    int sysml_value_3 = 0;
    if ((((double)(io_3 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 99.0)) / 1023.0)))) {
        sysml_conditional_43 = 1;
    }
    else {
        sysml_conditional_43 = 0;
    }
    if ((((double)(io_3 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 99.0)) / 1023.0)))) {
        sysml_conditional_44 = 1;
    }
    else {
        sysml_conditional_44 = 0;
    }
    if ((((double)(io_3 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 99.0)) / 1023.0)))) {
        sysml_conditional_45 = 1;
    }
    else {
        sysml_conditional_45 = 0;
    }
    if ((((int)(((double)(io_3 * 99.0)) / 1023.0)) - sysml_conditional_44) < ((double)((int)(((int)(((double)(io_3 * 99.0)) / 1023.0)) - sysml_conditional_45)))) {
        sysml_conditional_46 = 1;
    }
    else {
        sysml_conditional_46 = 0;
    }
    if ((((double)(io_3 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 99.0)) / 1023.0)))) {
        sysml_conditional_47 = 1;
    }
    else {
        sysml_conditional_47 = 0;
    }
    if ((((double)(io_3 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 99.0)) / 1023.0)))) {
        sysml_conditional_48 = 1;
    }
    else {
        sysml_conditional_48 = 0;
    }
    if ((((double)(io_3 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 99.0)) / 1023.0)))) {
        sysml_conditional_49 = 1;
    }
    else {
        sysml_conditional_49 = 0;
    }
    if ((((int)(((double)(io_3 * 99.0)) / 1023.0)) - sysml_conditional_48) < ((double)((int)(((int)(((double)(io_3 * 99.0)) / 1023.0)) - sysml_conditional_49)))) {
        sysml_conditional_50 = 1;
    }
    else {
        sysml_conditional_50 = 0;
    }
    if ((((double)(io_3 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 99.0)) / 1023.0)))) {
        sysml_conditional_51 = 1;
    }
    else {
        sysml_conditional_51 = 0;
    }
    if ((((double)(io_3 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 99.0)) / 1023.0)))) {
        sysml_conditional_52 = 1;
    }
    else {
        sysml_conditional_52 = 0;
    }
    if ((((double)(io_3 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 99.0)) / 1023.0)))) {
        sysml_conditional_53 = 1;
    }
    else {
        sysml_conditional_53 = 0;
    }
    if ((((int)(((double)(io_3 * 99.0)) / 1023.0)) - sysml_conditional_52) < ((double)((int)(((int)(((double)(io_3 * 99.0)) / 1023.0)) - sysml_conditional_53)))) {
        sysml_conditional_54 = 1;
    }
    else {
        sysml_conditional_54 = 0;
    }
    if (((((int)(((int)(((double)(io_3 * 99.0)) / 1023.0)) - sysml_conditional_47)) - sysml_conditional_50) * 1.0) < ((double)((int)((((int)(((int)(((double)(io_3 * 99.0)) / 1023.0)) - sysml_conditional_51)) - sysml_conditional_54) * 1.0)))) {
        sysml_conditional_55 = 1;
    }
    else {
        sysml_conditional_55 = 0;
    }
    sysml_value_3 = ((int)((((int)(((int)(((double)(io_3 * 99.0)) / 1023.0)) - sysml_conditional_43)) - sysml_conditional_46) * 1.0)) - sysml_conditional_55;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_4 = sysml_value_3;
}