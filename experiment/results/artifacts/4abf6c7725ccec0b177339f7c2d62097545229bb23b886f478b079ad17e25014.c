#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_elapsedTotalMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_elapsedTotalMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_8, bool* io_0, bool* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_conditional_7 = 0;
    bool sysml_lazy_8 = 0;
    bool sysml_conditional_9 = 0;
    bool sysml_lazy_10 = 0;
    bool sysml_conditional_11 = 0;
    bool sysml_lazy_12 = 0;
    bool sysml_conditional_13 = 0;
    bool sysml_lazy_14 = 0;
    bool sysml_conditional_15 = 0;
    bool sysml_lazy_16 = 0;
    bool sysml_conditional_17 = 0;
    bool sysml_lazy_18 = 0;
    bool sysml_conditional_19 = 0;
    bool sysml_lazy_20 = 0;
    bool sysml_conditional_21 = 0;
    bool sysml_lazy_22 = 0;
    bool sysml_conditional_23 = 0;
    bool sysml_lazy_24 = 0;
    bool sysml_conditional_25 = 0;
    bool sysml_lazy_26 = 0;
    bool sysml_conditional_27 = 0;
    bool sysml_lazy_28 = 0;
    bool sysml_conditional_29 = 0;
    bool sysml_lazy_30 = 0;
    bool sysml_conditional_31 = 0;
    bool sysml_lazy_32 = 0;
    bool sysml_conditional_33 = 0;
    bool sysml_lazy_34 = 0;
    bool sysml_conditional_35 = 0;
    bool sysml_lazy_36 = 0;
    bool sysml_conditional_37 = 0;
    bool sysml_lazy_38 = 0;
    bool sysml_conditional_39 = 0;
    bool sysml_lazy_40 = 0;
    bool sysml_conditional_41 = 0;
    bool sysml_lazy_42 = 0;
    bool sysml_conditional_43 = 0;
    bool sysml_lazy_44 = 0;
    bool sysml_conditional_45 = 0;
    bool sysml_lazy_46 = 0;
    bool sysml_conditional_47 = 0;
    sysml_state_elapsedTotalMs = (sysml_state_elapsedTotalMs + io_8);
    bool sysml_value_0 = 0;
    sysml_lazy_0 = sysml_state_elapsedTotalMs >= 1000;
    if (sysml_lazy_0) {
        sysml_lazy_0 = sysml_state_elapsedTotalMs < 2000;
    }
    if (sysml_lazy_0) {
        sysml_conditional_1 = true;
    }
    else {
        sysml_lazy_2 = sysml_state_elapsedTotalMs >= 2000;
        if (sysml_lazy_2) {
            sysml_lazy_2 = sysml_state_elapsedTotalMs < 3000;
        }
        if (sysml_lazy_2) {
            sysml_conditional_3 = true;
        }
        else {
            sysml_lazy_4 = sysml_state_elapsedTotalMs >= 3000;
            if (sysml_lazy_4) {
                sysml_lazy_4 = sysml_state_elapsedTotalMs < 4000;
            }
            if (sysml_lazy_4) {
                sysml_conditional_5 = true;
            }
            else {
                sysml_conditional_5 = false;
            }
            sysml_conditional_3 = sysml_conditional_5;
        }
        sysml_conditional_1 = sysml_conditional_3;
    }
    sysml_value_0 = sysml_conditional_1;
    bool sysml_value_1 = 0;
    sysml_lazy_6 = sysml_state_elapsedTotalMs >= 1000;
    if (sysml_lazy_6) {
        sysml_lazy_6 = sysml_state_elapsedTotalMs < 2000;
    }
    if (sysml_lazy_6) {
        sysml_conditional_7 = true;
    }
    else {
        sysml_lazy_8 = sysml_state_elapsedTotalMs >= 2000;
        if (sysml_lazy_8) {
            sysml_lazy_8 = sysml_state_elapsedTotalMs < 3000;
        }
        if (sysml_lazy_8) {
            sysml_conditional_9 = false;
        }
        else {
            sysml_lazy_10 = sysml_state_elapsedTotalMs >= 3000;
            if (sysml_lazy_10) {
                sysml_lazy_10 = sysml_state_elapsedTotalMs < 4000;
            }
            if (sysml_lazy_10) {
                sysml_conditional_11 = false;
            }
            else {
                sysml_conditional_11 = false;
            }
            sysml_conditional_9 = sysml_conditional_11;
        }
        sysml_conditional_7 = sysml_conditional_9;
    }
    sysml_value_1 = sysml_conditional_7;
    bool sysml_value_2 = 0;
    sysml_lazy_12 = sysml_state_elapsedTotalMs >= 1000;
    if (sysml_lazy_12) {
        sysml_lazy_12 = sysml_state_elapsedTotalMs < 2000;
    }
    if (sysml_lazy_12) {
        sysml_conditional_13 = true;
    }
    else {
        sysml_lazy_14 = sysml_state_elapsedTotalMs >= 2000;
        if (sysml_lazy_14) {
            sysml_lazy_14 = sysml_state_elapsedTotalMs < 3000;
        }
        if (sysml_lazy_14) {
            sysml_conditional_15 = false;
        }
        else {
            sysml_lazy_16 = sysml_state_elapsedTotalMs >= 3000;
            if (sysml_lazy_16) {
                sysml_lazy_16 = sysml_state_elapsedTotalMs < 4000;
            }
            if (sysml_lazy_16) {
                sysml_conditional_17 = false;
            }
            else {
                sysml_conditional_17 = false;
            }
            sysml_conditional_15 = sysml_conditional_17;
        }
        sysml_conditional_13 = sysml_conditional_15;
    }
    sysml_value_2 = sysml_conditional_13;
    bool sysml_value_3 = 0;
    sysml_lazy_18 = sysml_state_elapsedTotalMs >= 1000;
    if (sysml_lazy_18) {
        sysml_lazy_18 = sysml_state_elapsedTotalMs < 2000;
    }
    if (sysml_lazy_18) {
        sysml_conditional_19 = true;
    }
    else {
        sysml_lazy_20 = sysml_state_elapsedTotalMs >= 2000;
        if (sysml_lazy_20) {
            sysml_lazy_20 = sysml_state_elapsedTotalMs < 3000;
        }
        if (sysml_lazy_20) {
            sysml_conditional_21 = true;
        }
        else {
            sysml_lazy_22 = sysml_state_elapsedTotalMs >= 3000;
            if (sysml_lazy_22) {
                sysml_lazy_22 = sysml_state_elapsedTotalMs < 4000;
            }
            if (sysml_lazy_22) {
                sysml_conditional_23 = true;
            }
            else {
                sysml_conditional_23 = false;
            }
            sysml_conditional_21 = sysml_conditional_23;
        }
        sysml_conditional_19 = sysml_conditional_21;
    }
    sysml_value_3 = sysml_conditional_19;
    bool sysml_value_4 = 0;
    sysml_lazy_24 = sysml_state_elapsedTotalMs >= 1000;
    if (sysml_lazy_24) {
        sysml_lazy_24 = sysml_state_elapsedTotalMs < 2000;
    }
    if (sysml_lazy_24) {
        sysml_conditional_25 = true;
    }
    else {
        sysml_lazy_26 = sysml_state_elapsedTotalMs >= 2000;
        if (sysml_lazy_26) {
            sysml_lazy_26 = sysml_state_elapsedTotalMs < 3000;
        }
        if (sysml_lazy_26) {
            sysml_conditional_27 = false;
        }
        else {
            sysml_lazy_28 = sysml_state_elapsedTotalMs >= 3000;
            if (sysml_lazy_28) {
                sysml_lazy_28 = sysml_state_elapsedTotalMs < 4000;
            }
            if (sysml_lazy_28) {
                sysml_conditional_29 = false;
            }
            else {
                sysml_conditional_29 = false;
            }
            sysml_conditional_27 = sysml_conditional_29;
        }
        sysml_conditional_25 = sysml_conditional_27;
    }
    sysml_value_4 = sysml_conditional_25;
    bool sysml_value_5 = 0;
    sysml_lazy_30 = sysml_state_elapsedTotalMs >= 1000;
    if (sysml_lazy_30) {
        sysml_lazy_30 = sysml_state_elapsedTotalMs < 2000;
    }
    if (sysml_lazy_30) {
        sysml_conditional_31 = false;
    }
    else {
        sysml_lazy_32 = sysml_state_elapsedTotalMs >= 2000;
        if (sysml_lazy_32) {
            sysml_lazy_32 = sysml_state_elapsedTotalMs < 3000;
        }
        if (sysml_lazy_32) {
            sysml_conditional_33 = false;
        }
        else {
            sysml_lazy_34 = sysml_state_elapsedTotalMs >= 3000;
            if (sysml_lazy_34) {
                sysml_lazy_34 = sysml_state_elapsedTotalMs < 4000;
            }
            if (sysml_lazy_34) {
                sysml_conditional_35 = false;
            }
            else {
                sysml_conditional_35 = false;
            }
            sysml_conditional_33 = sysml_conditional_35;
        }
        sysml_conditional_31 = sysml_conditional_33;
    }
    sysml_value_5 = sysml_conditional_31;
    bool sysml_value_6 = 0;
    sysml_lazy_36 = sysml_state_elapsedTotalMs >= 1000;
    if (sysml_lazy_36) {
        sysml_lazy_36 = sysml_state_elapsedTotalMs < 2000;
    }
    if (sysml_lazy_36) {
        sysml_conditional_37 = false;
    }
    else {
        sysml_lazy_38 = sysml_state_elapsedTotalMs >= 2000;
        if (sysml_lazy_38) {
            sysml_lazy_38 = sysml_state_elapsedTotalMs < 3000;
        }
        if (sysml_lazy_38) {
            sysml_conditional_39 = false;
        }
        else {
            sysml_lazy_40 = sysml_state_elapsedTotalMs >= 3000;
            if (sysml_lazy_40) {
                sysml_lazy_40 = sysml_state_elapsedTotalMs < 4000;
            }
            if (sysml_lazy_40) {
                sysml_conditional_41 = false;
            }
            else {
                sysml_conditional_41 = false;
            }
            sysml_conditional_39 = sysml_conditional_41;
        }
        sysml_conditional_37 = sysml_conditional_39;
    }
    sysml_value_6 = sysml_conditional_37;
    bool sysml_value_7 = 0;
    sysml_lazy_42 = sysml_state_elapsedTotalMs >= 1000;
    if (sysml_lazy_42) {
        sysml_lazy_42 = sysml_state_elapsedTotalMs < 2000;
    }
    if (sysml_lazy_42) {
        sysml_conditional_43 = false;
    }
    else {
        sysml_lazy_44 = sysml_state_elapsedTotalMs >= 2000;
        if (sysml_lazy_44) {
            sysml_lazy_44 = sysml_state_elapsedTotalMs < 3000;
        }
        if (sysml_lazy_44) {
            sysml_conditional_45 = false;
        }
        else {
            sysml_lazy_46 = sysml_state_elapsedTotalMs >= 3000;
            if (sysml_lazy_46) {
                sysml_lazy_46 = sysml_state_elapsedTotalMs < 4000;
            }
            if (sysml_lazy_46) {
                sysml_conditional_47 = false;
            }
            else {
                sysml_conditional_47 = false;
            }
            sysml_conditional_45 = sysml_conditional_47;
        }
        sysml_conditional_43 = sysml_conditional_45;
    }
    sysml_value_7 = sysml_conditional_43;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
    *io_4 = sysml_value_4;
    *io_5 = sysml_value_5;
    *io_6 = sysml_value_6;
    *io_7 = sysml_value_7;
}