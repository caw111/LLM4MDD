#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_step = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_step = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool* io_0, bool* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7, bool* io_8, bool* io_9, bool* io_10, bool* io_11) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_conditional_0 = 0;
    bool sysml_conditional_1 = 0;
    bool sysml_conditional_2 = 0;
    bool sysml_conditional_3 = 0;
    bool sysml_conditional_4 = 0;
    bool sysml_conditional_5 = 0;
    bool sysml_conditional_6 = 0;
    bool sysml_conditional_7 = 0;
    bool sysml_conditional_8 = 0;
    bool sysml_conditional_9 = 0;
    bool sysml_conditional_10 = 0;
    bool sysml_conditional_11 = 0;
    bool sysml_conditional_12 = 0;
    bool sysml_conditional_13 = 0;
    bool sysml_conditional_14 = 0;
    bool sysml_conditional_15 = 0;
    bool sysml_conditional_16 = 0;
    bool sysml_conditional_17 = 0;
    bool sysml_conditional_18 = 0;
    bool sysml_conditional_19 = 0;
    bool sysml_conditional_20 = 0;
    bool sysml_conditional_21 = 0;
    bool sysml_conditional_22 = 0;
    bool sysml_conditional_23 = 0;
    bool sysml_conditional_24 = 0;
    bool sysml_conditional_25 = 0;
    if ((sysml_state_step < 4)) {
        sysml_state_step = (sysml_state_step + 1);
    }
    else {
        sysml_state_step = 0;
    }
    bool sysml_value_0 = 0;
    if (sysml_state_step == 1) {
        sysml_conditional_0 = true;
    }
    else {
        if (sysml_state_step == 2) {
            sysml_conditional_1 = false;
        }
        else {
            if (sysml_state_step == 3) {
                sysml_conditional_2 = true;
            }
            else {
                sysml_conditional_2 = false;
            }
            sysml_conditional_1 = sysml_conditional_2;
        }
        sysml_conditional_0 = sysml_conditional_1;
    }
    sysml_value_0 = sysml_conditional_0;
    bool sysml_value_1 = 0;
    if (sysml_state_step == 1) {
        sysml_conditional_3 = false;
    }
    else {
        if (sysml_state_step == 2) {
            sysml_conditional_4 = true;
        }
        else {
            if (sysml_state_step == 3) {
                sysml_conditional_5 = true;
            }
            else {
                sysml_conditional_5 = false;
            }
            sysml_conditional_4 = sysml_conditional_5;
        }
        sysml_conditional_3 = sysml_conditional_4;
    }
    sysml_value_1 = sysml_conditional_3;
    bool sysml_value_2 = 0;
    if (sysml_state_step == 1) {
        sysml_conditional_6 = true;
    }
    else {
        if (sysml_state_step == 2) {
            sysml_conditional_7 = true;
        }
        else {
            if (sysml_state_step == 3) {
                sysml_conditional_8 = true;
            }
            else {
                sysml_conditional_8 = false;
            }
            sysml_conditional_7 = sysml_conditional_8;
        }
        sysml_conditional_6 = sysml_conditional_7;
    }
    sysml_value_2 = sysml_conditional_6;
    bool sysml_value_3 = 0;
    if (sysml_state_step == 1) {
        sysml_conditional_9 = false;
    }
    else {
        if (sysml_state_step == 2) {
            sysml_conditional_10 = false;
        }
        else {
            if (sysml_state_step == 3) {
                sysml_conditional_11 = false;
            }
            else {
                sysml_conditional_11 = false;
            }
            sysml_conditional_10 = sysml_conditional_11;
        }
        sysml_conditional_9 = sysml_conditional_10;
    }
    sysml_value_3 = sysml_conditional_9;
    bool sysml_value_4 = 0;
    if (sysml_state_step == 1) {
        sysml_conditional_12 = false;
    }
    else {
        if (sysml_state_step == 2) {
            sysml_conditional_13 = true;
        }
        else {
            if (sysml_state_step == 3) {
                sysml_conditional_14 = false;
            }
            else {
                sysml_conditional_14 = true;
            }
            sysml_conditional_13 = sysml_conditional_14;
        }
        sysml_conditional_12 = sysml_conditional_13;
    }
    sysml_value_4 = sysml_conditional_12;
    bool sysml_value_5 = 0;
    if (sysml_state_step == 1) {
        sysml_conditional_15 = false;
    }
    else {
        if (sysml_state_step == 2) {
            sysml_conditional_16 = true;
        }
        else {
            if (sysml_state_step == 3) {
                sysml_conditional_17 = false;
            }
            else {
                sysml_conditional_17 = true;
            }
            sysml_conditional_16 = sysml_conditional_17;
        }
        sysml_conditional_15 = sysml_conditional_16;
    }
    sysml_value_5 = sysml_conditional_15;
    bool sysml_value_6 = 0;
    if (sysml_state_step == 1) {
        sysml_conditional_18 = false;
    }
    else {
        if (sysml_state_step == 2) {
            sysml_conditional_19 = true;
        }
        else {
            if (sysml_state_step == 3) {
                sysml_conditional_20 = true;
            }
            else {
                sysml_conditional_20 = true;
            }
            sysml_conditional_19 = sysml_conditional_20;
        }
        sysml_conditional_18 = sysml_conditional_19;
    }
    sysml_value_6 = sysml_conditional_18;
    bool sysml_value_7 = 0;
    sysml_value_7 = false;
    bool sysml_value_8 = 0;
    if (sysml_state_step == 1) {
        sysml_conditional_21 = true;
    }
    else {
        sysml_conditional_21 = false;
    }
    sysml_value_8 = sysml_conditional_21;
    bool sysml_value_9 = 0;
    if (sysml_state_step == 2) {
        sysml_conditional_22 = true;
    }
    else {
        sysml_conditional_22 = false;
    }
    sysml_value_9 = sysml_conditional_22;
    bool sysml_value_10 = 0;
    if (sysml_state_step == 3) {
        sysml_conditional_23 = true;
    }
    else {
        sysml_conditional_23 = false;
    }
    sysml_value_10 = sysml_conditional_23;
    bool sysml_value_11 = 0;
    if (sysml_state_step == 0) {
        sysml_conditional_24 = true;
    }
    else {
        if (sysml_state_step == 4) {
            sysml_conditional_25 = true;
        }
        else {
            sysml_conditional_25 = false;
        }
        sysml_conditional_24 = sysml_conditional_25;
    }
    sysml_value_11 = sysml_conditional_24;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
    *io_4 = sysml_value_4;
    *io_5 = sysml_value_5;
    *io_6 = sysml_value_6;
    *io_7 = sysml_value_7;
    *io_8 = sysml_value_8;
    *io_9 = sysml_value_9;
    *io_10 = sysml_value_10;
    *io_11 = sysml_value_11;
}